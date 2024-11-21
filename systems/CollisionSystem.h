#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.h"
bool isCollision(Entity* entity1, Entity* entity2) {
    sf::FloatRect rect1 = entity1->getComponent<TextureComponent>()->sprite.getGlobalBounds();
    sf::FloatRect rect2 = entity2->getComponent<TextureComponent>()->sprite.getGlobalBounds();
    return rect1.intersects(rect2);
}