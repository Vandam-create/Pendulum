#ifndef PHYSICSENGINE
#define PHYSICSENGINE

#include <iostream>
#include <vector>
#include "Vec2D.h"
#include <memory>


struct Particle{
    Vec2d position;
    Vec2d predicted_position;
    Vec2d velocity;
    double inv_mass;
    bool fixed;

    void setMass(double mass) {
        if (mass <= 0.0) {
            inv_mass = 0.0;
        } else {
            inv_mass = 1.0 / mass;
        }
        if (fixed) {
            inv_mass = 0.0;
        }
    }

    Particle(const Vec2d pos = {0, 0}, double mass = 1.0, bool is_fixed = false)
        : position(pos), predicted_position(pos), velocity(0, 0), fixed(is_fixed) {
        setMass(mass);
    }
    
    void applyForce(const Vec2d& force, double dt) {
        if (!fixed) {
            velocity += force * inv_mass * dt;
        }
    }
};

struct Constraint {
    size_t particle1_idx;
    size_t particle2_idx;
    double target_length;
    double stiffness; // жёсткость [0, 1]
    
    Constraint(size_t idx1, size_t idx2, double length, double stiff = 1.0)
        : particle1_idx(idx1), particle2_idx(idx2), 
          target_length(length), stiffness(stiff) {
        if (length <= 0.0) {
            throw std::invalid_argument("Constraint length must be positive");
        }
        if (stiff < 0.0 || stiff > 1.0) {
            throw std::invalid_argument("Stiffness must be between 0 and 1");
        }
    }
    
    void solve(std::vector<Particle>& particles) const;
    
    bool contains(size_t idx) const {
        return (particle1_idx == idx) || (particle2_idx == idx);
    }
};

class PhysicsEngine {
private:
    std::vector<Particle> particles;
    std::vector<Constraint> constraints;
    
    Vec2d gravity{0.0, 100.0};  // Гравитация в пикселях/с² ???
    double time_step = 0.016;   // ~60 FPS
    double current_time = 0.0;
    int solver_iterations = 10;
    double damping = 0;
    
public:
    PhysicsEngine() = default;
    
    PhysicsEngine(const Vec2d& grav, double dt, int iter, double damp = 0)
        : gravity(grav), time_step(dt), solver_iterations(iter), damping(damp) {}
    
    //запрещаем копирование
    PhysicsEngine(const PhysicsEngine&) = delete;
    PhysicsEngine& operator=(const PhysicsEngine&) = delete;
    
    //разрешаем перемещение
    PhysicsEngine(PhysicsEngine&&) = default;
    PhysicsEngine& operator=(PhysicsEngine&&) = default;
    
    //создание частицы
    size_t createParticle(const Vec2d& position, double mass = 1.0, bool fixed = false) {
        particles.emplace_back(position, mass, fixed);
        return particles.size() - 1;
    }
    
    //удаление частицы
    void removeParticle(size_t idx);
    
    //создание связи
    void createConstraint(size_t idx1, size_t idx2, double length, double stiffness = 1.0);
    
    //основной метод симуляции "PBD" (вельвет говно)
    void step();
    
    //геттеры
    size_t getParticleCount() const { return particles.size(); }
    Particle& getParticle(size_t idx) { return particles[idx]; }
    const Particle& getParticle(size_t idx) const { return particles[idx]; }
    size_t getConstraintCount() const { return constraints.size(); }
    const Constraint& getConstraint(size_t idx) const { return constraints[idx]; }
    double getTime() const { return current_time; }
    double getTimeStep() const { return time_step; }
    
    //сеттеры
    void setGravity(const Vec2d& grav) { gravity = grav; }
    void setTimeStep(double dt) { if (dt > 0.0) time_step = dt; }
    void setSolverIterations(int iter) { if (iter > 0) solver_iterations = iter; }
    void setDamping(double damp) { damping = std::max(0.0, damp); }
    
    //очистка
    void clear() {
        particles.clear();
        constraints.clear();
        current_time = 0.0;
    }
    
    //прессеты
    size_t createSimplePendulum(const Vec2d& pivot, double length, double mass);
    void createDoublePendulum(const Vec2d& pivot, double l1, double l2, double m1, double m2);
    
    //применение силы
    void applyForceToParticle(size_t idx, const Vec2d& force) {
        if (idx < particles.size()) {
            particles[idx].applyForce(force, time_step);
        }
    }
    
    //применение импульса
    void applyImpulseToParticle(size_t idx, const Vec2d& impulse) {
        if (idx < particles.size()) {
            Particle& p = particles[idx];
            if (!p.fixed) {
                p.velocity += impulse * p.inv_mass;
            }
        }
    }
};

#endif