#include <SFML/Graphics.hpp>
#include "../Components/Components.h"
#include "../Entity.h"

float getWidth(float length, HealthComponent *healthComp);

void showHealth(sf::RenderWindow *window, float length, float height, HealthComponent *healthComp, PositionComponent *positionComp, float scale) {
	sf::RectangleShape rectangle(sf::Vector2f(getWidth(length, healthComp), height * 0.1));
    rectangle.setFillColor(sf::Color::Green);
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(.5f);
    rectangle.setPosition(positionComp->getX() * scale, (positionComp->getY()) * scale);
	window->draw(rectangle);
}

float getWidth(float length, HealthComponent *healthComp) {
	return length * healthComp->getCurrent() / healthComp->getTotal();
}

void damageHandler(Entity *entity, float dp) {
	auto health = entity->getComponent<HealthComponent>();
	health->setCurrent(health->getCurrent() - dp);
}