#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.h"
#include "../Components/BaseEntityComponents.h"

bool isCollision(Entity* entity1, Entity* entity2) {
    auto pos1 = entity1->getComponent<PositionComponent>();
    auto size1 = entity1->getComponent<SizeComponent>();
    auto pos2 = entity2->getComponent<PositionComponent>();
    auto size2 = entity2->getComponent<SizeComponent>();
    
    float left1 = pos1->getX();
    float right1 = pos1->getX() + size1->getWidth();
    float top1 = pos1->getY();
    float bottom1 = pos1->getY() + size1->getHeight();
    
    float left2 = pos2->getX();
    float right2 = pos2->getX() + size2->getWidth();
    float top2 = pos2->getY();
    float bottom2 = pos2->getY() + size2->getHeight();
    
    return !(right1 < left2 || left1 > right2 || bottom1 < top2 || top1 > bottom2);
}