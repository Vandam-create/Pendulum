#ifndef MODAL_WIN_H
#define MODAL_WIN_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <memory>
#include "../include/physics_engine.h"
#include "visual_config.h"

class ModalWindow {
private:
////////////////////
PhysicsEngine& engine;

/////////////////////
    bool visible = false;
    sf::RectangleShape background;
    sf::RectangleShape dialog;
    std::shared_ptr<sf::Font> font;
    
    //поля ввода
    std::string mass_input = "1.0";
    std::string speed_input = "0.0";
    bool direction_right = true;
    int active_field = 0;
    
    //рамочки
    sf::RectangleShape mass_field;
    sf::RectangleShape speed_field;
    
    //цвета дял них
    sf::Color inactive_color = sf::Color(40, 40, 50);
    sf::Color active_color = sf::Color(60, 60, 80);
    sf::Color outline_color = sf::Color::White;

    //тексты
    std::unique_ptr<sf::Text> title;
    std::unique_ptr<sf::Text> mass_label;
    std::unique_ptr<sf::Text> speed_label;
    std::unique_ptr<sf::Text> direction_text;
    std::unique_ptr<sf::Text> mass_value_text;
    std::unique_ptr<sf::Text> speed_value_text;
    
    //кнопки
    sf::RectangleShape ok_btn;
    sf::RectangleShape cancel_btn;
    sf::RectangleShape delete_btn;
    std::unique_ptr<sf::Text> ok_text;
    std::unique_ptr<sf::Text> cancel_text;
    std::unique_ptr<sf::Text> delete_text;
    
    //callback
    std::function<void(float, float, bool, bool, bool)> callback;
    

    
public:
    ModalWindow(PhysicsEngine& engine): engine(engine) {
        font = std::make_shared<sf::Font>();
        
        font->openFromFile("C:/Windows/Fonts/Calibri.ttf");
        
        
        background.setFillColor(sf::Color(0, 0, 0, 150));
        dialog.setSize({(float)Config::MODAL_WIDTH, (float)Config::MODAL_HEIGHT});
        dialog.setFillColor(sf::Color(50, 50, 70));
        dialog.setOutlineColor(sf::Color::White);
        dialog.setOutlineThickness(2);
        
        setup_ui();
    }
    sf::Vector2f get_size(){

        return dialog.getSize();
    }
    
    void setup_ui() {

        mass_field.setSize({(float)Config::FIELD_WIDTH, (float)Config::FIELD_HEIGHT});
        mass_field.setFillColor(inactive_color);
        mass_field.setOutlineColor(outline_color);
        mass_field.setOutlineThickness(1);
        
        speed_field.setSize({(float)Config::FIELD_WIDTH, (float)Config::FIELD_HEIGHT});
        speed_field.setFillColor(inactive_color);
        speed_field.setOutlineColor(outline_color);
        speed_field.setOutlineThickness(1);

        title = std::unique_ptr<sf::Text>(new sf::Text(*font));
        title->setString("Pendulum Settings");
        title->setCharacterSize(Config::TITLE_FONT_SIZE);
        title->setFillColor(sf::Color::White);
    
        mass_label = std::unique_ptr<sf::Text>(new sf::Text(*font));
        mass_label->setString("Mass:");
        mass_label->setCharacterSize(Config::LABEL_FONT_SIZE);
        mass_label->setFillColor(sf::Color::White);
        
        speed_label = std::unique_ptr<sf::Text>(new sf::Text(*font));
        speed_label->setString("Velocity:");
        speed_label->setCharacterSize(Config::LABEL_FONT_SIZE);
        speed_label->setFillColor(sf::Color::White);

        direction_text = std::unique_ptr<sf::Text>(new sf::Text(*font));
        direction_text->setString("Direction: Right");
        direction_text->setCharacterSize(Config::LABEL_FONT_SIZE);
        direction_text->setFillColor(sf::Color::White);
        
        mass_value_text = std::unique_ptr<sf::Text>(new sf::Text(*font));
        mass_value_text->setString(mass_input);
        mass_value_text->setCharacterSize(Config::VALUE_FONT_SIZE);
        mass_value_text->setFillColor(sf::Color::White);
        
        speed_value_text = std::unique_ptr<sf::Text>(new sf::Text(*font));
        speed_value_text->setString(speed_input);
        speed_value_text->setCharacterSize(Config::VALUE_FONT_SIZE);
        speed_value_text->setFillColor(sf::Color::White);
        
        //кнопки
        ok_btn.setSize({(float)Config::BUTTON_WIDTH, (float)Config::BUTTON_HEIGHT});
        ok_btn.setFillColor(sf::Color(60, 140, 60));
        ok_btn.setOutlineColor(sf::Color::White);
        ok_btn.setOutlineThickness(1);
        
        cancel_btn.setSize({(float)Config::BUTTON_WIDTH, (float)Config::BUTTON_HEIGHT});
        cancel_btn.setFillColor(sf::Color(140, 60, 60));
        cancel_btn.setOutlineColor(sf::Color::White);
        cancel_btn.setOutlineThickness(1);
        
        delete_btn.setSize({(float)Config::BUTTON_WIDTH, (float)Config::BUTTON_HEIGHT});
        delete_btn.setFillColor(sf::Color(180, 100, 60));
        delete_btn.setOutlineColor(sf::Color::White);
        delete_btn.setOutlineThickness(1);
        
        ok_text = std::unique_ptr<sf::Text>(new sf::Text(*font));
        ok_text->setString("OK");
        ok_text->setCharacterSize(Config::BUTTON_FONT_SIZE);
        ok_text->setFillColor(sf::Color::White);
        
        cancel_text = std::unique_ptr<sf::Text>(new sf::Text(*font));
        cancel_text->setString("Cancel");
        cancel_text->setCharacterSize(Config::BUTTON_FONT_SIZE);
        cancel_text->setFillColor(sf::Color::White);
        
        delete_text = std::unique_ptr<sf::Text>(new sf::Text(*font));
        delete_text->setString("Remove");
        delete_text->setCharacterSize(Config::BUTTON_FONT_SIZE);
        delete_text->setFillColor(sf::Color::White);
    }
    
    void show( bool isCreate, size_t index, const sf::Vector2f& pos, 
              std::function<void(float, float, bool, bool, bool)> cb) {
        
        visible = true;
        callback = cb;
        
        if(!isCreate){

           std::cerr<< "i m here " << index<<" ";
           Particle p = engine.getParticle(index);
           direction_right = false;
           

           mass_input = std::to_string(1/p.inv_mass);
           speed_input = std::to_string(leight(p.velocity)); 
        }else{

            mass_input = "1.0";
            speed_input = "0.0";
        }


        mass_value_text->setString(mass_input);
        speed_value_text->setString(speed_input);
        dialog.setPosition(pos);

        mass_field.setPosition({pos.x + Config::FIELD_X, pos.y + Config::FIELD_Y});
        speed_field.setPosition({pos.x + Config::FIELD_X, pos.y + Config::FIELD_Y + 50});

        title->setPosition({pos.x + Config::TITLE_X, pos.y + Config::TITLE_Y});
        mass_label->setPosition({pos.x + Config::LABEL_X, pos.y + Config::LABEL_Y});
        speed_label->setPosition({pos.x + Config::LABEL_X, pos.y + Config::LABEL_Y + 50});
        direction_text->setPosition({pos.x + Config::LABEL_X, pos.y + Config::LABEL_Y + 100});

        mass_value_text->setPosition({pos.x + Config::FIELD_X + 5, pos.y + Config::FIELD_Y + 5});
        speed_value_text->setPosition({pos.x + Config::FIELD_X + 5, pos.y + Config::FIELD_Y + 55});

        float button_spacing = (Config::MODAL_WIDTH - 3 * Config::BUTTON_WIDTH) / 4.0f;

        ok_btn.setPosition({pos.x + button_spacing, pos.y + Config::BUTTON_Y});
        cancel_btn.setPosition({pos.x + button_spacing * 2 + Config::BUTTON_WIDTH, 
                                pos.y + Config::BUTTON_Y});
        delete_btn.setPosition({pos.x + button_spacing * 3 + Config::BUTTON_WIDTH * 2, 
                                pos.y + Config::BUTTON_Y});


        ok_text->setString("OK");
        cancel_text->setString("Cancel");
        delete_text->setString("Remove");

        sf::FloatRect okBounds = ok_text->getLocalBounds();

        ok_text->setPosition({
            ok_btn.getPosition().x + (Config::BUTTON_WIDTH - okBounds.size.x) * 0.5f,
            ok_btn.getPosition().y + (Config::BUTTON_HEIGHT - okBounds.size.y) * 0.4f
        });

        sf::FloatRect cancelBounds = cancel_text->getLocalBounds();
        cancel_text->setPosition({
            cancel_btn.getPosition().x + (Config::BUTTON_WIDTH - cancelBounds.size.x) * 0.5f,
            cancel_btn.getPosition().y + (Config::BUTTON_HEIGHT - cancelBounds.size.y) * 0.4f
        });

        sf::FloatRect deleteBounds = delete_text->getLocalBounds();
        delete_text->setPosition({
            delete_btn.getPosition().x + (Config::BUTTON_WIDTH - deleteBounds.size.x) * 0.5f,
            delete_btn.getPosition().y + (Config::BUTTON_HEIGHT - deleteBounds.size.y) * 0.4f
        });

        update_field_highlight();
    }
    
    void hide() {
        visible = false;
        mass_input = "1.0";
        speed_input = "0.0";
        direction_right = true;
        active_field = 0;
    }
    

    void update_field_highlight() {
        if (active_field == 0) {
            mass_field.setFillColor(active_color);
            speed_field.setFillColor(inactive_color);
            mass_field.setOutlineThickness(2);
            speed_field.setOutlineThickness(1);
        } else {
            mass_field.setFillColor(inactive_color);
            speed_field.setFillColor(active_color);
            mass_field.setOutlineThickness(1);
            speed_field.setOutlineThickness(2);
        }
    }

    bool handle_event(const sf::Event& event) {
        if (!visible) return false;
        
        if (auto* mouse_press = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mouse_press->button == sf::Mouse::Button::Left) {
                sf::Vector2f mouse_pos = sf::Vector2f(mouse_press->position);
                
                if (direction_text->getGlobalBounds().contains(mouse_pos)) {
                    direction_right = !direction_right;
                    direction_text->setString(direction_right ? "Direction: Right" : "Direction: Left");
                    return true;
                }
                
                sf::FloatRect mass_field_rect(mass_value_text->getPosition(), 
                              {(float)Config::FIELD_WIDTH, (float)Config::FIELD_HEIGHT});

                if (mass_field_rect.contains(mouse_pos)) {
                    active_field = 0;
                    update_field_highlight();
                    return true;
                }
                
                sf::FloatRect speed_field_rect(speed_value_text->getPosition(), 
                               {(float)Config::FIELD_WIDTH, (float)Config::FIELD_HEIGHT});

                if (speed_field_rect.contains(mouse_pos)) {
                    active_field = 1;
                    update_field_highlight();
                    return true;
                }
                
                if (ok_btn.getGlobalBounds().contains(mouse_pos)) {
                    try {
                        float mass = std::stof(mass_input);
                        float speed = std::stof(speed_input);
                        if (callback) callback(mass, speed, direction_right, true, false);
                    } catch (...) {
                        if (callback) callback(1.0f, 0.0f, true, true, false);
                    }
                    hide();
                    return true;
                }
                
                if (cancel_btn.getGlobalBounds().contains(mouse_pos)) {
                    if (callback) callback(0, 0, true, false, false);
                    hide();
                    return true;
                }
                
                if (delete_btn.getGlobalBounds().contains(mouse_pos)) {
                    if (callback) callback(0, 0, true, false, true);
                    hide();
                    return true;
                }
                
                //блокируем клики вне диалога
                return true;
            }
        }
        else if (auto* text_event = event.getIf<sf::Event::TextEntered>()) {
            if (text_event->unicode == 9 || text_event->unicode == 13) {  // Tab и Enter
                return false;  // Пропускаем
    }
            if (active_field == 0) {
                if (text_event->unicode == 8 && !mass_input.empty()) {
                    mass_input.pop_back();
                }
                else if ((text_event->unicode >= 48 && text_event->unicode <= 57) || 
                         text_event->unicode == 46) {
                    mass_input += static_cast<char>(text_event->unicode);
                }
                mass_value_text->setString(mass_input);
                return true;
            }
            //ввод скорости
            else if (active_field == 1) {
                if (text_event->unicode == 8 && !speed_input.empty()) {
                    speed_input.pop_back();
                }
                else if ((text_event->unicode >= 48 && text_event->unicode <= 57) || 
                         text_event->unicode == 46) {
                    speed_input += static_cast<char>(text_event->unicode);
                }
                speed_value_text->setString(speed_input);
                return true;
            }
        }
        else if (auto* key_event = event.getIf<sf::Event::KeyPressed>()) {
            if (key_event->scancode == sf::Keyboard::Scan::Tab) {
                active_field = (active_field + 1) % 2;
                update_field_highlight();
                return true;
            }
            else if (key_event->scancode == sf::Keyboard::Scan::Enter) {
                try {
                    float mass = std::stof(mass_input);
                    float speed = std::stof(speed_input);
                    if (callback) callback(mass, speed, direction_right, true, false);
                } catch (...) {
                    if (callback) callback(1.0f, 0.0f, true, true, false);
                }
                hide();
                return true;
            }
            else if (key_event->scancode == sf::Keyboard::Scan::Escape) {
                if (callback) callback(0, 0, true, false, false);
                hide();
                return true;
            }
        }
        
        return true;
    }
    
    void draw(sf::RenderWindow& window) {
        if (!visible) return;
        
        background.setSize(sf::Vector2f(window.getSize()));
        window.draw(background);
        window.draw(dialog);
        window.draw(*title);
        window.draw(*mass_label);
        window.draw(*speed_label);
        window.draw(*direction_text);
        window.draw(mass_field);
        window.draw(speed_field);
        window.draw(*mass_value_text);
        window.draw(*speed_value_text);
        window.draw(ok_btn);
        window.draw(cancel_btn);
        window.draw(delete_btn);
        window.draw(*ok_text);
        window.draw(*cancel_text);
        window.draw(*delete_text);
    }
    
    bool is_visible() const { return visible; }
};

#endif