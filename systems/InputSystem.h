#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

class InputSystem {
public:
    // Обработка и сохранение событий
    void handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::KeyPressed) {
            // Добавляем клавишу, если её нет в векторе
            if (std::find(pressedKeys.begin(), pressedKeys.end(), event.key.code) == pressedKeys.end()) {
                pressedKeys.push_back(event.key.code);
            }
        } else if (event.type == sf::Event::KeyReleased) {
            // Удаляем клавишу из вектора при отпускании
            pressedKeys.erase(
                std::remove(pressedKeys.begin(), pressedKeys.end(), event.key.code),
                pressedKeys.end()
            );
        } else if (event.type == sf::Event::MouseButtonPressed) {
            // Сохраняем последний клик мыши и устанавливаем флаг
            mouseClick = event.mouseButton;
            mouseClicked = true;
        }
    }

    // Получение всех нажатых клавиш
    const std::vector<sf::Keyboard::Key>& getPressedKeys() const {
        return pressedKeys;
    }

    // Проверка, был ли клик мыши
    bool hasMouseClick() const {
        return mouseClicked;
    }

    // Получение последнего клика мыши
    sf::Event::MouseButtonEvent getMouseClick() const {
        return mouseClick;
    }

    // Очистка состояния ввода (для начала нового кадра)
    void clear() {
        mouseClicked = false;  // Сбрасываем флаг клика мыши
    }

private:
    std::vector<sf::Keyboard::Key> pressedKeys;  // Все текущие нажатые клавиши
    sf::Event::MouseButtonEvent mouseClick{}; // Последний клик мыши (если был)
    bool mouseClicked = false;  // Флаг для проверки наличия клика мыши
};