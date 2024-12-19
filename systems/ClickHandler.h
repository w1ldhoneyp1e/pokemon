#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.h"
#include <cmath>
#include <iostream>

bool isClickOnEntity(const sf::Event::MouseButtonEvent& mouseEvent, Entity* entity) {
    // Получаем компоненты позиции, размера, центра и угла поворота
    auto positionComp = entity->getComponent<PositionComponent>();
    auto sizeComp = entity->getComponent<SizeComponent>();
    auto originComp = entity->getComponent<OriginComponent>();
    auto rotationComp = entity->getComponent<RotationComponent>();

    if (!positionComp || !sizeComp) {
        return false; // Если ключевые компоненты отсутствуют, обработка невозможна
    }

    float x = positionComp->getX();
    float y = positionComp->getY();
    float width = sizeComp->getWidth();
    float height = sizeComp->getHeight();
    float angle = rotationComp ? rotationComp->getAngle() : 0.0f;
    float originX = originComp ? originComp->getX() : 0.0f;
    float originY = originComp ? originComp->getY() : 0.0f;

    // Переводим позицию точки клика в локальные координаты объекта
    sf::Vector2f mousePos(mouseEvent.x, mouseEvent.y);

    // Смещаем точку клика относительно центра объекта
    double radius = std::sqrt(std::pow(mousePos.x - x - originX, 2) + std::pow(mousePos.y - y - originY, 2));

    // Учитываем вращение объекта (обратное преобразование)
    float rotatedX = std::sin(angle) * radius;
    float rotatedY = std::cos(angle) * radius;

    // Смещаем относительно начала координат (origin)
    rotatedX += originX;
    rotatedY += originY;

    // Проверяем попадание в прямоугольник
    return rotatedX >= 0 && rotatedX <= width && rotatedY >= -height/2 && rotatedY <= height/2;
}


