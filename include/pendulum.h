#ifndef PENDULUM
#define PENDULUM
#include <vector>
#include <SFML/Graphics.hpp>  // Для графики (спрайты, текстуры, шрифты и т.д.)
#include <SFML/Window.hpp>     // Для работы с окнами и событиями
#include <SFML/System.hpp>  
#include "physics_engine.h"
#include "Vec2D.h"

class Pendulum{
private:
    std::vector<size_t> particles;
    std::vector<sf::CircleShape> circles;
    PhysicsEngine engine;
    sf::RenderWindow win;

public:
    void create_pendulum(Vec2d position, double mass, size_t constraint_with){

        particles.push_back(engine.createParticle(position, mass));
        float radius = 15.0f;
        sf::CircleShape circle(radius);
        circles.push_back(circle);

        circle.setOrigin(sf::Vector2f(radius, radius));
        circle.setPosition(sf::Vector2f(
            static_cast<float>(position.x),
            static_cast<float>(position.y)
        ));
        
        

        win.draw(circle);

    }


    std::vector<sf::CircleShape> get_circles(){
        return circles;
    }


};

#endif
