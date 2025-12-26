#ifndef PENDULUM_H
#define PENDULUM_H

#include <vector>
#include <array>
#include <SFML/Graphics.hpp>
#include "physics_engine.h"
#include "Vec2D.h"
#include <iostream>

class Pendulum {
private:
    std::vector<Particle> default_particles;

    std::vector<sf::CircleShape> circles;
    std::vector<std::array<sf::Vertex, 2>> links;

    PhysicsEngine& engine;
    sf::RenderWindow& win;

public:
    Pendulum(PhysicsEngine& eng, sf::RenderWindow& win) : engine{eng}, win{win} {}
    Pendulum() = delete;


    void show_drag_preview(const sf::Vector2f& mouse_pos, size_t constraint_idx) {
        if (constraint_idx >= circles.size()) return;
        

        sf::Vertex line[2];
        line[0].position = circles[constraint_idx].getPosition();
        line[1].position = mouse_pos;
        line[0].color = sf::Color(255, 200, 100, 180);
        line[1].color = sf::Color(255, 200, 100, 180);
        
        sf::CircleShape temp_circle(15.0f);
        temp_circle.setOrigin({15.0f, 15.0f});
        temp_circle.setPosition(mouse_pos);
        temp_circle.setFillColor(sf::Color(255, 100, 100, 180));
        
        win.draw(line, 2, sf::PrimitiveType::Lines);
        win.draw(temp_circle);
    }

    void change_state(size_t i, double mass, double velosity, bool remove = false){
       
        if(remove){
            links.erase(links.end() - engine.getConstraintCount_with(i), links.end());
            engine.removeParticle(i);
            circles.erase(circles.begin() + i);

            return;
        }

        Particle& p = engine.getParticle(i);
        p.setMass(mass);

        p.velocity = (leight(p.velocity) > 0.00001) ? 
             (p.velocity / leight(p.velocity)) * velosity : 
              Vec2d(1.0, 0.0) * velosity;

    }
    
    void create_pendulum(const sf::Vector2f& position, size_t constraint_with, 
                    double length, double mass, double velosity, bool isfixed = 0) {
        
        Vec2d pos{position.x, position.y};
        Vec2d pos2 = (engine.getParticle(constraint_with)).position;
        Vec2d vector_between = (pos-pos2)/leight(pos-pos2);
        Vec2d vec_velosity;
        vec_velosity.x = -vector_between.y;
        vec_velosity.y = vector_between.x;
        vec_velosity *= velosity;

        size_t new_particle_idx = engine.createParticle(pos, mass, vec_velosity, isfixed);
        default_particles.push_back(Particle(pos, mass, vec_velosity));
        
        sf::CircleShape circle(15.0f);
        circle.setOrigin({15.0f, 15.0f});
        circle.setPosition(position);
        circle.setFillColor(sf::Color::Red);
        circles.push_back(circle);
        
        if (engine.getParticleCount() >= 1) {
            size_t existing_idx = constraint_with;
            
            if (existing_idx != new_particle_idx) {
                try {
                    engine.createConstraint(existing_idx, new_particle_idx, length);
                    
                    //создаём графическую связь
                    std::array<sf::Vertex, 2> link;
                    link[0].position = circles[constraint_with].getPosition();
                    link[1].position = position;
                    link[0].color = sf::Color::Cyan;
                    link[1].color = sf::Color::Cyan;
                    links.push_back(link);
                }
                catch (const std::invalid_argument& e) {
                    std::cerr << "Failed to create constraint: " << e.what() << std::endl;
                }
            }
        }
    }
    
    void create_firs_part( const sf::Vector2f& position, size_t constraint_with, 
                           double length, double mass, double velosity, bool isfixed = 0) {
        
        Vec2d pos{position.x, position.y};

        size_t new_particle_idx = engine.createParticle(pos, mass, {0, 0}, isfixed);
        default_particles.push_back(Particle(pos, mass, {0, 0}));
        
        sf::CircleShape circle(15.0f);
        circle.setOrigin({15.0f, 15.0f});
        circle.setPosition(position);
        circle.setFillColor(sf::Color::Red);
        circles.push_back(circle);
        
    }

    void restart_animation() {
        for (size_t i = 0; i < default_particles.size() && i < engine.getParticleCount(); i++) {
            auto& p = engine.getParticle(i);
            p.position = default_particles[i].position;
            p.velocity = default_particles[i].velocity;
        }
        engine.reset_time();
    }

    void update_animation() {
        for (size_t i = 0; i <engine.getParticleCount(); ++i) {
            const auto& particle = engine.getParticle(i);
            circles[i].setPosition({
                static_cast<float>(particle.position.x),
                static_cast<float>(particle.position.y)
            });
        }
        
        for (size_t i = 0; i < links.size(); i++) {
            size_t i1, i2;
            (engine.getConstraint(i)).getIndexes(i1, i2);

            if (i < links.size()) {
                links[i][0].position = circles[i1].getPosition();
                links[i][1].position = circles[i2].getPosition();
            }
        }
    }
    
    void draw_all() {
        for (const auto& link : links) {
            win.draw(&link[0], 2, sf::PrimitiveType::Lines);
        }
        
        for (const auto& circle : circles) {
            win.draw(circle);
        }
    }
    
    const std::vector<sf::CircleShape>& get_circles() const {
        return circles;
    }
};

#endif