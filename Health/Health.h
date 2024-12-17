#include <SFML/Graphics.hpp>
#include "../Components/Components.h"
#include "../Entity.h"

float getWidth(float length, HealthComponent *healthComp);

void showHealth(sf::RenderWindow *window, float length, HealthComponent *healthComp, PositionComponent *positionComp) {
	sf::RectangleShape rectangle(sf::Vector2f(getWidth(length, healthComp), 20));
    rectangle.setFillColor(sf::Color::Green);
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(.5f);
    rectangle.setPosition(positionComp->getX(), positionComp->getY() - 50);
	window->draw(rectangle);
}

float getWidth(float length, HealthComponent *healthComp) {
	return length * healthComp->getCurrent() / healthComp->getTotal();
}

void damageHandler(Entity *entity, float dp) {
	auto health = entity->getComponent<HealthComponent>();
	health->setCurrent(health->getCurrent() - dp);
}