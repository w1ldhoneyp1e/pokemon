#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.h"
#include <iostream>

void playerMovementSystem(EntityManager* em, InputSystem* input, float deltaTime) {
    auto keys = input->getPressedKeys();
    auto player = em->getEntitiesWithComponent<PlayerControlComponent>().back();
    auto position = player->getComponent<PositionComponent>();
    auto speed = player->getComponent<SpeedComponent>();

    if (!position || !speed) return;
	
    float movementSpeed = speed->speed * deltaTime;


    for (sf::Keyboard::Key key : keys) {
        switch (key) {
            case sf::Keyboard::W:
                position->setPos(
                    position->getX(),
                    position->getY() - movementSpeed
                );
                break;
            case sf::Keyboard::A:
                position->setPos(
                    position->getX() - movementSpeed,
                    position->getY()
                );
                break;
            case sf::Keyboard::S:
                position->setPos(
                    position->getX(),
                    position->getY() + movementSpeed
                );
                break;
            case sf::Keyboard::D:
                position->setPos(
                    position->getX() + movementSpeed,
                    position->getY()
                );
                break;
            default:
                break;
        }
    }
}