#pragma once
#include <SFML/Graphics.hpp>

class WindowManager {
public:

    void createWindow() {
        settings.antialiasingLevel = 8;
        window.create(
            sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
            NAME, 
            sf::Style::Default, 
            settings
        );
    }

    sf::RenderWindow* getWindow() {
        return &window;
    }
    
private:
    const unsigned int WINDOW_WIDTH = 800;
    const unsigned int WINDOW_HEIGHT = 600;
    const std::string NAME = "Pokemon";
    sf::ContextSettings settings;
    sf::RenderWindow window;
};