#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

int main() {
    std::cout << "Запуск маятника SFML 3.0...\n";
    
    // 1. Создаем окно
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Маятник SFML 3.0");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    if (!window.isOpen()) {
        std::cout << "Ошибка создания окна!\n";
        return 1;
    }

    // 2. Параметры маятника
    float length = 200.0f;
    float angle = 0.5f; // начальный угол в радианах
    float angularVelocity = 0.0f;
    const float gravity = 9.81f;
    const float damping = 0.995f;
    
    // 3. Точка крепления
    sf::Vector2f pivot(400.0f, 100.0f);
    
    bool isRunning = false;
    sf::Clock clock;
    
    // 4. Главный цикл
    while (window.isOpen()) {
        // 4.1 Обработка событий (SFML 3.0 синтаксис)
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            
            if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->scancode == sf::Keyboard::Scan::Space) {
                    isRunning = !isRunning;
                    std::cout << (isRunning ? "Старт" : "Стоп") << "\n";
                }
                if (keyEvent->scancode == sf::Keyboard::Scan::R) {
                    // Сброс
                    angle = 0.5f;
                    angularVelocity = 0.0f;
                    std::cout << "Сброс\n";
                }
                if (keyEvent->scancode == sf::Keyboard::Scan::Escape) {
                    window.close();
                }
            }
        }
        
        // 4.2 Физика маятника
        if (isRunning) {
            float dt = clock.restart().asSeconds();
            float angularAcceleration = -(gravity / length) * std::sin(angle);
            angularVelocity += angularAcceleration * dt;
            angle += angularVelocity * dt;
            angularVelocity *= damping; // демпфирование
        }
        
        // 4.3 Расчет позиции шарика
        sf::Vector2f ballPos(
            pivot.x + length * std::sin(angle),
            pivot.y + length * std::cos(angle)
        );
        
        // 4.4 Отрисовка
        window.clear(sf::Color(30, 30, 40)); // темно-синий фон
        
        // ============================================
        // ИСПРАВЛЕНО: Создание sf::Vertex для SFML 3.0
        // ============================================
        // В SFML 3.0 sf::Vertex не имеет конструктора с двумя параметрами
        // Используем отдельные объекты:
        sf::Vertex lineVertices[2];
        lineVertices[0].position = pivot;
        lineVertices[0].color = sf::Color::White;
        lineVertices[1].position = ballPos;
        lineVertices[1].color = sf::Color::White;
        

        window.draw(lineVertices, 2, sf::PrimitiveType::Lines);
        
        // Альтернативный вариант (еще проще):
        // sf::Vertex line[] = {
        //     sf::Vertex{ .position = pivot, .color = sf::Color::White },
        //     sf::Vertex{ .position = ballPos, .color = sf::Color::White }
        // };
        // window.draw(line, 2, sf::PrimitiveType::Lines);
        
        // Точка крепления
        sf::CircleShape pivotCircle(8);
        pivotCircle.setFillColor(sf::Color::Yellow);
        pivotCircle.setOrigin({8, 8});
        pivotCircle.setPosition(pivot);
        window.draw(pivotCircle);
        
        // Шарик
        sf::CircleShape ball(25);
        ball.setFillColor(sf::Color::Red);
        ball.setOrigin({25, 25});
        ball.setPosition(ballPos);
        window.draw(ball);
        
        window.display();
    }
    
    std::cout << "Программа завершена.\n";
    return 0;
}