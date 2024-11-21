#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.h"

bool isClickOnEntity(const sf::Event::MouseButtonEvent& mouseEvent, Entity* entity) {
    auto positionComp = entity->getComponent<PositionComponent>();
    auto sizeComp = entity->getComponent<SizeComponent>();

    if (!positionComp || !sizeComp) {
        return false;
    }

    const float left = positionComp->x;
    const float top = positionComp->y;
    const float right = left + sizeComp->width;
    const float bottom = top + sizeComp->height;
    
    return (mouseEvent.x >= left 
        && mouseEvent.x <= right 
        && mouseEvent.y >= top 
        && mouseEvent.y <= bottom
    );
}