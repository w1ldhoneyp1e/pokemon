#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.h"

bool isClickOnEntity(const sf::Event::MouseButtonEvent& mouseEvent, Entity* entity) {
    auto positionComp = entity->getComponent<PositionComponent>();
    auto sizeComp = entity->getComponent<SizeComponent>();

    if (!positionComp || !sizeComp) {
        return false;
    }

    const float left = positionComp->getX();
    const float top = positionComp->getY();
    const float right = left + sizeComp->getWidth();
    const float bottom = top + sizeComp->getHeight();
    
    return (mouseEvent.x >= left 
        && mouseEvent.x <= right 
        && mouseEvent.y >= top 
        && mouseEvent.y <= bottom
    );
}