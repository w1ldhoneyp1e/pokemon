#include <SFML/Graphics.hpp>
#include "../Entity.h"
#include <iostream>

void playerMovementSystem(Entity* player, sf::Keyboard::Key key) {
    auto position = player->getComponent<PositionComponent>();
    switch (key)
    {
    case sf::Keyboard::W:
        position->y -= player->getComponent<SpeedComponent>()->speed;
        break;
    
    case sf::Keyboard::A:
        position->x -= player->getComponent<SpeedComponent>()->speed;
        break;

    case sf::Keyboard::S:
        position->y += player->getComponent<SpeedComponent>()->speed;
    break;

    case sf::Keyboard::D:
        position->x += player->getComponent<SpeedComponent>()->speed;
        break;

    default:
        break;
    }
}