#pragma once
#include <SFML/Graphics.hpp>

bool isEnterPressed(std::vector<sf::Keyboard::Key> keys) {
	return std::find(keys.begin(), keys.end(), sf::Keyboard::Enter) != keys.end();
}

bool isEscapePressed(std::vector<sf::Keyboard::Key> keys) {
	return std::find(keys.begin(), keys.end(), sf::Keyboard::Escape) != keys.end();
}