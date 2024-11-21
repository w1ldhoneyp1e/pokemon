#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

class InputSystem {
public:
    void handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::KeyPressed) {
            bool keyExists = false;
            for (auto it = pressedKeys.begin(); it != pressedKeys.end(); ++it) {
                if (*it == event.key.code) {
                    keyExists = true;
                    break;
                }
            }
            if (!keyExists) {
                pressedKeys.push_back(event.key.code);
            }
        } else if (event.type == sf::Event::KeyReleased) {
            for (auto it = pressedKeys.begin(); it != pressedKeys.end(); ++it) {
                if (*it == event.key.code) {
                    pressedKeys.erase(it);
                    break;
                }
            }
        } else if (event.type == sf::Event::MouseButtonPressed) {
            mouseClick = event.mouseButton;
            mouseClicked = true;
        }
    }

    const std::vector<sf::Keyboard::Key>& getPressedKeys() {
        return pressedKeys;
    }

    bool hasMouseClick() {
        return mouseClicked;
    }

    sf::Event::MouseButtonEvent& getMouseClick() {
        return mouseClick;
    }

    void clear() {
        mouseClicked = false;
        pressedKeys.clear();
    }

private:
    std::vector<sf::Keyboard::Key> pressedKeys;
    sf::Event::MouseButtonEvent mouseClick;
    bool mouseClicked = false;
};