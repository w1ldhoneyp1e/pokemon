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
    auto animation = player->getComponent<AnimationComponent>();

    if (!position || !speed) return;

    float movementSpeed = speed->speed * deltaTime;
    bool moved = false;
    int direction = 0;

    for (sf::Keyboard::Key key : keys) {
        switch (key) {
            case sf::Keyboard::W: {
                direction = 2; 
                moved = true;
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
                direction = 3; 
                moved = true;
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
                direction = 1; 
                moved = true;
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
                direction = 4; 
                moved = true;
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

    std::cout << "Frame: " << animation->currentFrameIndex << std::endl;
    std::cout << "Direction: " << animation->currentDirection << std::endl;
    if (moved) {
        animation->animationTimer += deltaTime;
        if (animation->animationTimer >= 1.0f / animation->animationSpeed) {
            animation->animationTimer -= 1.0f / animation->animationSpeed;
            animation->currentFrameIndex = (animation->currentFrameIndex + 1) % 3;
        }
        animation->currentDirection = direction;
    }
    if (!direction) {
        animation->currentFrameIndex = 1;
    }
}