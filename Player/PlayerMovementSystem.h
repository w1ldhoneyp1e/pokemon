#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.h"
#include <iostream>

void playerMovementSystem(Entity* player, const std::vector<sf::Keyboard::Key>& keys, float deltaTime) {
    auto position = player->getComponent<PositionComponent>();
    auto speed = player->getComponent<SpeedComponent>();

    if (!position || !speed) return;

    float movementSpeed = speed->speed * deltaTime;

    for (sf::Keyboard::Key key : keys) {
        switch (key) {
            case sf::Keyboard::W:
                position->y -= movementSpeed;
                break;
            case sf::Keyboard::A:
                position->x -= movementSpeed;
                break;
            case sf::Keyboard::S:
                position->y += movementSpeed;
                break;
            case sf::Keyboard::D:
                position->x += movementSpeed;
                break;
            default:
                break;
        }
    }
}