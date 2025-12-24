#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    std::cout << "Проверка SFML 3.0...\n";
    
    // Создаем окно - в SFML 3.0 другой синтаксис
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Тест SFML 3.0");
    
    if (!window.isOpen()) {
        std::cout << "Ошибка создания окна!\n";
        return 1;
    }
    
    std::cout << "Окно создано!\n";
    
    // Создаем зеленый круг
    sf::CircleShape circle(100.0f);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition({300, 200});
    
    // Главный цикл
    while (window.isOpen()) {
        // ============================================
        // ОБРАБОТКА СОБЫТИЙ ДЛЯ SFML 3.0 (НОВЫЙ СИНТАКСИС)
        // ============================================
        
        // В SFML 3.0 pollEvent() возвращает optional
        while (auto event = window.pollEvent()) {
            // Проверяем тип события с помощью is()
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            
            // Проверяем нажатие клавиши
            if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                // В SFML 3.0 используется scancode, а не code
                if (keyEvent->scancode == sf::Keyboard::Scan::Escape) {
                    window.close();
                }
            }
        }
        
        // Отрисовка
        window.clear(sf::Color::Black);
        window.draw(circle);
        window.display();
    }
    
    std::cout << "Программа завершена. Нажми Enter...\n";
    std::cin.get();
    return 0;
}