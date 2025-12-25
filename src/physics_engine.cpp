#include "../include/physics_engine.h"
#include <cmath>

void Constraint::solve(std::vector<Particle>& particles) const {
    if (stiffness < 1e-9) return;
    
    Particle& p1 = particles[particle1_idx];
    Particle& p2 = particles[particle2_idx];
    
    if (p1.fixed && p2.fixed) return;
    
    //вектор между предсказанными позициями
    Vec2d delta = p2.predicted_position - p1.predicted_position;
    double current_len_sq = delta.x * delta.x + delta.y * delta.y;
    
    if (current_len_sq < 1e-18) return;
    
    double current_len = std::sqrt(current_len_sq);
    double stretch = current_len - target_length;
    
    if (std::abs(stretch) < 1e-9) return;
    
    Vec2d direction = delta / current_len;
    
    double w1 = p1.inv_mass;
    double w2 = p2.inv_mass;
    double total_weight = w1 + w2;
    
    if (total_weight < 1e-9) return;
    
    //каоррекция позиций
    double lambda = (stretch / total_weight) * stiffness;
    
    if (!p1.fixed) {
        p1.predicted_position += direction * (lambda * w1);
    }
    if (!p2.fixed) {
        p2.predicted_position -= direction * (lambda * w2);
    }
}

void PhysicsEngine::removeParticle(size_t idx) {
    if (idx >= particles.size()) return;
    
    //удаляем связанные связи
    auto it = constraints.begin();
    while (it != constraints.end()) {
        if (it->contains(idx)) {
            it = constraints.erase(it);
        } else {
            ++it;
        }
    }
    
    //удаляем частицу
    particles.erase(particles.begin() + idx);
    
    //обновляем индексы для потомков связей
    for (auto& constraint : constraints) {
        if (constraint.particle1_idx > idx) constraint.particle1_idx--;
        if (constraint.particle2_idx > idx) constraint.particle2_idx--;
    }
}

void PhysicsEngine::createConstraint(size_t idx1, size_t idx2, double length, double stiffness) {
    if (idx1 >= particles.size() || idx2 >= particles.size()) {
        throw std::out_of_range("Invalid particle index");
    }
    if (idx1 == idx2) {
        throw std::invalid_argument("Cannot create constraint with itself");
    }
    
    //проверяем на дубликаты
    for (const auto& c : constraints) {
        if ((c.particle1_idx == idx1 && c.particle2_idx == idx2) ||
            (c.particle1_idx == idx2 && c.particle2_idx == idx1)) {
            throw std::invalid_argument("Constraint already exists");
        }
    }
    
    constraints.emplace_back(idx1, idx2, length, stiffness);
}

void PhysicsEngine::step() {
    //шаг 1:Обновляем скорости внешними силами
    for (auto& particle : particles) {
        if (!particle.fixed) {
            particle.velocity += gravity * time_step;
            
            //сопротивление
            particle.velocity *= (1.0 - damping);
        }
    }
    
    //шаг 2: Предсказываем позиции(без связей)
    for (auto& particle : particles) {
        particle.predicted_position = particle.position + particle.velocity * time_step;
    }
    
    //шаг 3: Решаем связи (корректируем предсказанные позиции)
    for (int iter = 0; iter < solver_iterations; iter++) {
        for (const auto& constraint : constraints) {
            constraint.solve(particles);
        }
    }
    
    //шаг 4: Обновляем позиции и вычисляем новые скорости
    for (auto& particle : particles) {
        if (!particle.fixed) {
            //новая скорость
            particle.velocity = (particle.predicted_position - particle.position) / time_step;
            
            //обновляем позицию
            particle.position = particle.predicted_position;
        }
    }
    
    current_time += time_step;
}
