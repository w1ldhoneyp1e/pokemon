#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.h"
#include "../const.h"
#include <cmath>
#include <iostream>

bool isClickOnEntity(const sf::Event::MouseButtonEvent& mouseEvent, Entity* entity) {
    auto positionComp = entity->getComponent<PositionComponent>();
    auto sizeComp = entity->getComponent<SizeComponent>();
    auto originComp = entity->getComponent<OriginComponent>();
    auto rotationComp = entity->getComponent<RotationComponent>();

    if (!positionComp || !sizeComp) {
        return false;
    }

    float left = positionComp->getX();
    float top = positionComp->getY();
    float width = sizeComp->getWidth();
    float height = sizeComp->getHeight();

    float originX = originComp ? originComp->getX() : 0.0f;
    float originY = originComp ? originComp->getY() : 0.0f;

    float angle = rotationComp ? rotationComp->getAngle() : 0.0f;

    float centerX = left + originX;
    float centerY = top + originY;

    float offsetX = SCREEN_WIDTH / 2 - SCALE * WINDOW_WIDTH / 2;

    float dx = (mouseEvent.x - offsetX) / SCALE - centerX;
    float dy = mouseEvent.y / SCALE - centerY;

    float cosAngle = std::cos(-angle);
    float sinAngle = std::sin(-angle);
    float localX = dx * cosAngle - dy * sinAngle;
    float localY = dx * sinAngle + dy * cosAngle;

    float rectLeft = -originX;
    float rectTop = -originY;
    float rectRight = rectLeft + width;
    float rectBottom = rectTop + height;

    return (localX >= rectLeft && localX <= rectRight &&
            localY >= rectTop && localY <= rectBottom);
}



