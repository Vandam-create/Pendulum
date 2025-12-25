#ifndef PHYSICSENGINE
#define PHYSICSENGINE

#include <iostream>
#include <vector>
#include "Vec2D.h"
#include <memory>

struct Particle{
    Vec2d position; 
    Vec2d old_position;
    Vec2d acceleration;
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

    Particle(const Vec2d pos = {0, 0}, double mass = 1.0, bool is_fixed = false):
              position(pos), old_position(pos)
            , acceleration(0, 0), fixed(is_fixed){

        setMass(mass);

    }

   

    void integrate(double dt, const Vec2d& total_acceleration) {
        if (fixed || inv_mass <= 0.0) return;

        Vec2d temp = position;
        position = position * 2.0 - old_position + total_acceleration * (dt * dt);
        old_position = temp;
    }
    void move(const Vec2d& delta) {
        if (!fixed) {
            position += delta;
            old_position += delta;
        }
    }

    Vec2d getVelocity(double dt) const {
        if (dt <= 1e-9) return {0, 0};
        return (position - old_position) / dt;
    }

};


struct Constraint {
    size_t particle1_idx;
    size_t particle2_idx;
    double target_length;
    double stiffness; //Жесткость [0, 1]
    
    // Конструктор
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
    

    void solve(std::vector<Particle>& particles) const {
        if (stiffness < 1e-9) return;
        
        Particle& p1 = particles[particle1_idx];
        Particle& p2 = particles[particle2_idx];
        
        // Если обе частицы зафиксированы - ничего не делаем
        if (p1.fixed && p2.fixed) return;
        
        //Вектор от p1 к p2
        Vec2d delta = p2.position - p1.position;
        double current_len = cross(delta);
        
        // Избегаем деления на ноль
        if (current_len < 1e-9) return;
        
        // Растяжение связи
        double stretch = current_len - target_length;
        
        // Если связь почти идеальная - пропускаем
        if (std::abs(stretch) < 1e-9) return;
        
        // Направляющий вектор
        Vec2d direction = delta / current_len;
        
        double w1 = p1.inv_mass;
        double w2 = p2.inv_mass;
        
        // Если обе частицы имеют нулевую обратную массу
        if (w1 + w2 < 1e-9) return;
        
        // Коэффициент коррекции (с учетом жесткости)
        double lambda = (stretch / (w1 + w2)) * stiffness;
        
        //Коррекции для каждой частицы
        Vec2d correction1 = direction * (lambda * w1);
        Vec2d correction2 = direction * (-lambda * w2);
        

        p1.move(correction1);
        p2.move(correction2);
    }
    
};


#endif