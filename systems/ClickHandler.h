#include <SFML/Graphics.hpp>
#include "../Entity.h"

bool isClickOnEntity(const sf::Event::MouseButtonEvent& mouseEvent, Entity* entity) {
    // Получаем компоненты позиции и размера
    auto positionComp = entity->getComponent<PositionComponent>();
    auto sizeComp = entity->getComponent<SizeComponent>();

    if (!positionComp || !sizeComp) {
        return false;
    }

    // Проверяем, попадает ли клик внутрь объекта
    float left = positionComp->x;
    float top = positionComp->y;
    float right = left + sizeComp->width;
    float bottom = top + sizeComp->height;

    // Проверка, находится ли точка клика внутри прямоугольника
    return (mouseEvent.x >= left && mouseEvent.x <= right && mouseEvent.y >= top && mouseEvent.y <= bottom);
}