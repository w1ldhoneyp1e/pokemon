#pragma once
#include <SFML/Graphics.hpp>

class WindowManager {
public:

    void createWindow() {
        settings.antialiasingLevel = 8;
        window.create(
            sf::VideoMode::getDesktopMode(),
            NAME, 
            sf::Style::Default, 
            settings
        );
    }

    sf::RenderWindow* getWindow() {
        return &window;
    }
    
private:
    const std::string NAME = "Pokemon";
    sf::ContextSettings settings;
    sf::RenderWindow window;
};