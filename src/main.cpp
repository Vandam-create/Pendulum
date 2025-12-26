#include <SFML/Graphics.hpp>
#include <iostream>
#include "../include/physics_engine.h"
#include "../include/pendulum.h"
#include "../include/Modal_win.h"

int main() {

    
    sf::RenderWindow window(sf::VideoMode({1000, 800}), "Pendulum");
    window.setFramerateLimit(60);
    
    PhysicsEngine engine(Vec2d(0, 300.0), 0.016, 10, 0);
    Pendulum pendulum(engine, window);
    ModalWindow dialog(engine);
    
    
    pendulum.create_firs_part(sf::Vector2f(500, 200), 0, 0, 0, 0, 1);

    bool is_dragging = false;
    size_t drag_from_idx = 0;
    sf::Vector2f drag_start_pos;
    sf::Vector2f drag_current_pos;
    
    bool is_paused = true;
    bool is_changing = false;
    
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (dialog.is_visible()) {
                if (dialog.handle_event(*event)) {
                    continue;
                }
            }

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            
            if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scan::Space) {
                    is_paused = !is_paused;
                }
                else if (key->scancode == sf::Keyboard::Scan::Escape) {
                    window.close();
                }
                else if (key->scancode == sf::Keyboard::Scan::CapsLock) {
                    is_changing = !is_changing;
                }
            }

            if (is_paused) {
                if(is_changing){
                    if (auto* mouse_release = event->getIf<sf::Event::MouseButtonReleased>()){
                        if(!is_dragging && mouse_release->button == sf::Mouse::Button::Left){
                            
                            sf::Vector2f mouse_pos = window.mapPixelToCoords(mouse_release->position);
                            const auto& circles = pendulum.get_circles();

                            sf::Vector2f screen_pos = {
                                (window.getSize().x - dialog.get_size().x)/2,
                                (window.getSize().y - dialog.get_size().y)/2
                            };

                            for (size_t i = 0; i < circles.size(); i++) {
                                if (circles[i].getGlobalBounds().contains(mouse_pos)) {
                                    
                                    dialog.show(0, i, screen_pos, 
                                    [&pendulum, i]
                                    (float mass, float speed, bool direction_right, bool change, bool remove) {
                                        if (change && mass > 0){
                                            pendulum.change_state(i, mass, (direction_right ? -speed: speed));
                                            std::cout << " Pendulum changed:" << std::endl;
                                            std::cout << "  Mass: " << mass << std::endl;
                                            std::cout << "  Speed: " << speed << std::endl;
                                            std::cout << "  Direction: " << (direction_right ? "Right" : "Left") << std::endl;

                                        } else if(remove){
                                            pendulum.change_state(i, mass, (direction_right ? -speed: speed), true);
                                            std::cout << "Pendulum was delet"<< std::endl;

                                        } 
                                        else{
                                            std::cout << "Change was canceled" << std::endl;
                                        }
                                    });
                                    break;
                                    
                                }
                            }

                        }
                    }
                }else if (auto* mouse_press = event->getIf<sf::Event::MouseButtonPressed>()) {

                    if (mouse_press->button == sf::Mouse::Button::Left) {
                        sf::Vector2f mouse_pos = window.mapPixelToCoords(mouse_press->position);

                        const auto& circles = pendulum.get_circles();
                        for (size_t i = 0; i < circles.size(); i++) {
                            if (circles[i].getGlobalBounds().contains(mouse_pos)) {
                                is_dragging = true;
                                drag_from_idx = i;
                                drag_start_pos = circles[i].getPosition();
                                drag_current_pos = mouse_pos;
                                break;
                            }
                        }
                    }
                }
                else if (auto* mouse_move = event->getIf<sf::Event::MouseMoved>()) {
                    if (is_dragging) {
                        drag_current_pos = window.mapPixelToCoords(mouse_move->position);
                    }
                }
                else if (auto* mouse_release = event->getIf<sf::Event::MouseButtonReleased>()) {
                    if (is_dragging && mouse_release->button == sf::Mouse::Button::Left) {
                        is_dragging = false;
                        
                        sf::Vector2f end_pos = window.mapPixelToCoords(mouse_release->position);
                        
                        sf::Vector2f screen_pos = {
                            (window.getSize().x - dialog.get_size().x)/2,
                            (window.getSize().y - dialog.get_size().y)/2
                        };

                        float length = std::sqrt(
                            std::pow(end_pos.x - drag_start_pos.x, 2) +
                            std::pow(end_pos.y - drag_start_pos.y, 2)
                        );
                        
                        if (length > 30.0f) {
                            dialog.show(1,  -1, screen_pos, 
                                [&pendulum, drag_from_idx, end_pos, length]
                                (float mass, float speed, bool direction_right, bool create, bool remove) {
                                    if (create && mass > 0){
                                        pendulum.create_pendulum(end_pos, drag_from_idx, length, mass, (direction_right ? -speed: speed));
                                        std::cout << " Pendulum created:" << std::endl;
                                        std::cout << "  Mass: " << mass << std::endl;
                                        std::cout << "  Speed: " << speed << std::endl;
                                        std::cout << "  Direction: " << (direction_right ? "Right" : "Left") << std::endl;
                                        std::cout << "  Leight: " << length << std::endl;
                                    } else {
                                        std::cout << "Creation was canceled" << std::endl;
                                    }
                                });
                        } else {
                            std::cout << "Too little" << std::endl;
                        }
                    }
                }
            }
        }
        
        if (!is_paused) {
            engine.step();
            pendulum.update_animation();
        }
        window.clear(sf::Color(20, 20, 30));
        

        pendulum.draw_all();
    
        if (is_dragging) {
            pendulum.show_drag_preview(drag_current_pos, drag_from_idx);
        }
        
        dialog.draw(window);
        
        window.display();
    }
    return 0;
}