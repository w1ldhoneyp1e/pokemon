#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.h"
#include <iostream>
#include "../systems/CollisionMap.h"


void playerMovementSystem(Controller* controller, float deltaTime, sf::RenderWindow* window) {
    auto [input, em, renderSystem, state, battleContext, collisionMaps, currentLocation] = controller->getAll();
    auto keys = input->getPressedKeys();
    auto player = em->getEntitiesWithComponent<PlayerControlComponent>().back();
    auto position = player->getComponent<PositionComponent>();
    auto speed = player->getComponent<SpeedComponent>();

    if (!position || !speed) return;

    float movementSpeed = speed->speed * deltaTime;

    for (sf::Keyboard::Key key : keys) {
        switch (key) {
            case sf::Keyboard::W: {
                bool isCollision = isMapCollision(collisionMaps, currentLocation, position->getX(), position->getY() - movementSpeed);
                if (isCollision) {
                    break;
                }
                position->setPos(
                    position->getX(),
                    position->getY() - movementSpeed
                );
                break;
            }
            case sf::Keyboard::A: {
                bool isCollision = isMapCollision(collisionMaps, currentLocation, position->getX() - movementSpeed, position->getY());
                if (isCollision) {
                    break;
                }
                position->setPos(
                    position->getX() - movementSpeed,
                    position->getY()
                );
                break;
            }
            case sf::Keyboard::S: {
                bool isCollision = isMapCollision(collisionMaps, currentLocation, position->getX(), position->getY() + movementSpeed);
                if (isCollision) {
                    break;
                }
                position->setPos(
                    position->getX(),
                    position->getY() + movementSpeed
                );
                break;
            }
            case sf::Keyboard::D: {
                bool isCollision = isMapCollision(collisionMaps, currentLocation, position->getX() + movementSpeed, position->getY());
                if (isCollision) {
                    break;
                }
                position->setPos(
                    position->getX() + movementSpeed,
                    position->getY()
                );
                break;
                }
            default:
                break;
        }
    }
}