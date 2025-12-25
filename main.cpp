#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>
#include <cmath>
#include <vector>
#include "physics_engine.h"

int main() {
    std::cout << "Двойной маятник с нашим PBD движком\n";
    

    sf::RenderWindow window(sf::VideoMode({1000, 800}), "Двойной маятник PBD");
    window.setFramerateLimit(60);
    
 
    PhysicsEngine engine(Vec2d(0, 300.0), 0.016, 10, 0);
    

    double l1 = 150.0;
    double l2 = 150.0;
    double m1 = 0.8;
    double m2 = 0.1;
    
    // Точка крепления
    Vec2d pivot(500, 200);
    
    size_t p0 = engine.createParticle(pivot, 1000.0, true); 
    size_t p1 = engine.createParticle(pivot + Vec2d(0, l1), m1, false);
    size_t p2 = engine.createParticle(pivot + Vec2d(0, l1 + l2), m2, false);
    
    engine.createConstraint(p0, p1, l1, 0.95);
    engine.createConstraint(p1, p2, l2, 0.9);
    
    auto& bob1 = engine.getParticle(p1);
    auto& bob2 = engine.getParticle(p2);
    
    double angle1 = 1.047; 
    double angle2 = 0.524;  
    
    bob1.position = pivot + Vec2d(l1 * std::sin(angle1), l1 * std::cos(angle1));
    bob2.position = bob1.position + Vec2d(l2 * std::sin(angle2), l2 * std::cos(angle2));
    

    bob1.velocity = Vec2d(0, 0);
    bob2.velocity = Vec2d(0, 0);
    
    std::cout << "Двойной маятник создан!\n";
    std::cout << "Частиц: " << engine.getParticleCount() << "\n";
    std::cout << "Связей: " << engine.getConstraintCount() << "\n";
    std::cout << "Гравитация: 300 пикс/с²\n";








    // Цвета
    sf::Color colors[] = {
        sf::Color(255, 255, 100),  //точка крепления (желтый)
        sf::Color(255, 100, 100),  //первый груз (красный)
        sf::Color(100, 255, 100),  //второй груз (зеленый)
        sf::Color(255, 200, 100),  //первая связь (оранжевый)
        sf::Color(100, 200, 255)   //вторая связь (голубой)
    };
    
    bool isRunning = true;
    bool showTrails = true;
    bool showVectors = false;
    sf::Clock clock;
    
    std::vector<std::vector<sf::Vertex>> trails;
    trails.resize(engine.getParticleCount());
    const size_t maxTrailLength = 300;
    
    int frameCount = 0;

    
    float currentGravity = 300.0;
    
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            
            if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
                switch (key->scancode) {
                    case sf::Keyboard::Scan::Space:
                        isRunning = !isRunning;
                        std::cout << (isRunning ? "Старт" : "Стоп") << "\n";
                        break;
                        
                    case sf::Keyboard::Scan::R:
                        // Сброс в начальное положение
                        bob1.position = pivot + Vec2d(l1 * std::sin(angle1), l1 * std::cos(angle1));
                        bob2.position = bob1.position + Vec2d(l2 * std::sin(angle2), l2 * std::cos(angle2));
                        bob1.velocity = Vec2d(0, 0);
                        bob2.velocity = Vec2d(0, 0);
                        trails.clear();
                        trails.resize(engine.getParticleCount());
                        std::cout << "Маятник сброшен\n";
                        break;
                        
                    case sf::Keyboard::Scan::T:
                        showTrails = !showTrails;
                        std::cout << "Траектории: " << (showTrails ? "ВКЛ" : "ВЫКЛ") << "\n";
                        break;
                        
                    case sf::Keyboard::Scan::V:
                        showVectors = !showVectors;
                        std::cout << "Векторы скорости: " << (showVectors ? "ВКЛ" : "ВЫКЛ") << "\n";
                        break;
                        
                    case sf::Keyboard::Scan::G:
                        currentGravity = (currentGravity > 0) ? -300.0 : 300.0;
                        engine.setGravity(Vec2d(0, currentGravity));
                        std::cout << "Гравитация: " << currentGravity << " пикс/с²\n";
                        break;
                        
                    case sf::Keyboard::Scan::Escape:
                        window.close();
                        break;
                        
                }
            }
            
            if (auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                
                

            }
        }
        if (isRunning) {
            engine.step();

            if (++frameCount % 120 == 0) {
                const auto& pivot_p = engine.getParticle(p0);
                const auto& bob1_p = engine.getParticle(p1);
                const auto& bob2_p = engine.getParticle(p2);
                
                Vec2d delta1 = bob1_p.position - pivot_p.position;
                Vec2d delta2 = bob2_p.position - bob1_p.position;
                
                double angle1_deg = std::atan2(delta1.x, delta1.y) * 180.0 / 3.14159;
                double angle2_deg = std::atan2(delta2.x, delta2.y) * 180.0 / 3.14159;
                

                double speed1 = std::sqrt(bob1_p.velocity.x*bob1_p.velocity.x + 
                                         bob1_p.velocity.y*bob1_p.velocity.y);
                double speed2 = std::sqrt(bob2_p.velocity.x*bob2_p.velocity.x + 
                                         bob2_p.velocity.y*bob2_p.velocity.y);
                
                std::cout << "Время: " << engine.getTime() << "с | ";
                std::cout << "Углы: " << angle1_deg << "°, " << angle2_deg << "° | ";
                std::cout << "Скорости: " << speed1 << ", " << speed2 << " пикс/с\n";
            }
            

            if (showTrails) {
                for (size_t i = 0; i < engine.getParticleCount(); i++) {
                    const auto& p = engine.getParticle(i);
                    if (p.fixed) continue;
                    
                    sf::Vertex point;
                    point.position = sf::Vector2f(
                        static_cast<float>(p.position.x),
                        static_cast<float>(p.position.y)
                    );
                    

                    if (i == p1) {
                        point.color = sf::Color(255, 100, 100, 80);  //красный
                    } else if (i == p2) {
                        point.color = sf::Color(100, 255, 100, 80);  //зеленый
                    } else {
                        point.color = sf::Color(200, 200, 255, 60);  //голубой
                    }
                    
                    trails[i].push_back(point);
                    
                    if (trails[i].size() > maxTrailLength) {
                        trails[i].erase(trails[i].begin());
                    }
                }
            }
        }
        

        window.clear(sf::Color(15, 15, 25));  //темно-синий
        
        // 1 траектории
        if (showTrails) {
            for (const auto& trail : trails) {
                if (trail.size() > 1) {
                    window.draw(trail.data(), trail.size(), sf::PrimitiveType::LineStrip);
                }
            }
        }
        
        // 2 связи
        for (size_t i = 0; i < engine.getConstraintCount(); i++) {
            const auto& c = engine.getConstraint(i);
            const auto& p1_particle = engine.getParticle(c.particle1_idx);
            const auto& p2_particle = engine.getParticle(c.particle2_idx);
            
            sf::Vertex line[2];
            line[0].position = sf::Vector2f(
                static_cast<float>(p1_particle.position.x),
                static_cast<float>(p1_particle.position.y)
            );
            line[1].position = sf::Vector2f(
                static_cast<float>(p2_particle.position.x),
                static_cast<float>(p2_particle.position.y)
            );
            
            if (c.particle1_idx == p0 && c.particle2_idx == p1) {
                line[0].color = colors[3];  // первая связь
                line[1].color = colors[3];
            } else {
                line[0].color = colors[4];  // вторая связь
                line[1].color = colors[4];
            }
            
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }
        
        // 3 частицы
        for (size_t i = 0; i < engine.getParticleCount(); i++) {
            const auto& p = engine.getParticle(i);
            
            float radius = p.fixed ? 10.0f : 15.0f;
            sf::CircleShape circle(radius);
            circle.setOrigin(sf::Vector2f(radius, radius));
            circle.setPosition(sf::Vector2f(
                static_cast<float>(p.position.x),
                static_cast<float>(p.position.y)
            ));
            
            if (p.fixed) {
                circle.setFillColor(colors[0]);  //точка крепления
            } else if (i == p1) {
                circle.setFillColor(colors[1]);  //первый груз
            } else if (i == p2) {
                circle.setFillColor(colors[2]);  //второй груз
            } else {
                circle.setFillColor(sf::Color::White);
            }
            
            window.draw(circle);
            
        }
        
        // 4 информационная панель
        sf::RectangleShape panel(sf::Vector2f(250.0f, 160.0f));
        panel.setPosition(sf::Vector2f(10.0f, 10.0f));
        panel.setFillColor(sf::Color(0, 0, 0, 150));
        panel.setOutlineColor(sf::Color::White);
        panel.setOutlineThickness(1.0f);
        window.draw(panel);
        
        //индикаторы
        float y_pos = 25.0f;
        float x_indicator = 25.0f;
        float x_legend = 50.0f;
        
        //статус
        sf::CircleShape status_indicator(6.0f);
        status_indicator.setOrigin(sf::Vector2f(6.0f, 6.0f));
        status_indicator.setPosition(sf::Vector2f(x_indicator, y_pos));
        status_indicator.setFillColor(isRunning ? sf::Color::Green : sf::Color::Red);
        window.draw(status_indicator);
        
        //легенда - точка крепления
        y_pos += 25.0f;
        sf::CircleShape legend_pivot(5.0f);
        legend_pivot.setOrigin(sf::Vector2f(5.0f, 5.0f));
        legend_pivot.setPosition(sf::Vector2f(x_indicator, y_pos));
        legend_pivot.setFillColor(colors[0]);
        window.draw(legend_pivot);
        
        //легенда - первый груз
        y_pos += 25.0f;
        sf::CircleShape legend_bob1(7.0f);
        legend_bob1.setOrigin(sf::Vector2f(7.0f, 7.0f));
        legend_bob1.setPosition(sf::Vector2f(x_indicator, y_pos));
        legend_bob1.setFillColor(colors[1]);
        window.draw(legend_bob1);
        
        //легенда - второй груз
        y_pos += 25.0f;
        sf::CircleShape legend_bob2(7.0f);
        legend_bob2.setOrigin(sf::Vector2f(7.0f, 7.0f));
        legend_bob2.setPosition(sf::Vector2f(x_indicator, y_pos));
        legend_bob2.setFillColor(colors[2]);
        window.draw(legend_bob2);
        
        window.display();
    }

    return 0;
}