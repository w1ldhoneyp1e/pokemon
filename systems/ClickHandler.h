#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.h"
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

    // Получаем параметры спрайта
    float left = positionComp->getX();
    float top = positionComp->getY();
    float width = sizeComp->getWidth();
    float height = sizeComp->getHeight();

    float originX = originComp ? originComp->getX() : 0.0f; // Центр по умолчанию
    float originY = originComp ? originComp->getY() : 0.0f;

    float angle = rotationComp ? rotationComp->getAngle() : 0.0f; // Угол в радианах

    // Центр спрайта
    float centerX = left + originX;
    float centerY = top + originY;

    // Перевод координат клика в систему координат спрайта с учётом поворота
    float dx = mouseEvent.x - centerX;
    float dy = mouseEvent.y - centerY;

    // Учет поворота точки (обратное вращение на угол спрайта)
    float cosAngle = std::cos(-angle);
    float sinAngle = std::sin(-angle);
    float localX = dx * cosAngle - dy * sinAngle;
    float localY = dx * sinAngle + dy * cosAngle;

    // Границы прямоугольника в локальных координатах
    float rectLeft = -originX;
    float rectTop = -originY;
    float rectRight = rectLeft + width;
    float rectBottom = rectTop + height;

    std::cout << "Mouse Global: (" << mouseEvent.x << ", " << mouseEvent.y << ")\n";
    std::cout << "Mouse Local: (" << localX << ", " << localY << ")\n";
    std::cout << "Rect Left Top: (" << rectLeft << ", " << rectTop << ")\n";
    std::cout << "Rect Right Bottom: (" << rectRight << ", " << rectBottom << ")\n";


    // Проверка, попала ли точка в прямоугольник
    return (localX >= rectLeft && localX <= rectRight &&
            localY >= rectTop && localY <= rectBottom);
}


