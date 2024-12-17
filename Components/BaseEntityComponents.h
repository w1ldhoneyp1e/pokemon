#pragma once
#include <SFML/Graphics.hpp>
#include "ParentComponent.h"

class PositionComponent : public Component {
private:
    float x, y;
public:
    PositionComponent(float x, float y) : x(x), y(y) {}

    float getX() {
        return x;
    }

    float getY() {
        return y;
    }

    void setPos(float _x, float _y) {
        x = _x;
        y = _y;
    }
};

class SpeedComponent : public Component {
public:
    float speed;
    SpeedComponent(float speed) : speed(speed) {}
};

class OriginComponent : public Component {
public:
    float x;
    float y;

    OriginComponent(float x, float y)
        : x(x), y(y) {}

    float getX() const {
        return x;
    }

    float getY() const {
        return y;
    }

    void setX(float newX) {
        x = newX;
    }

    void setY(float newY) {
        y = newY;
    }
};

class RotationComponent : public Component {
public:
    float speed;
    float angle;
    float posX;
    float posY;

    RotationComponent(float speed, float posX, float posY, float angle = 0.0f)
        : speed(speed), posX(posX), posY(posY), angle(angle) {}

    float getSpeed() const {
        return speed;
    }

    float getAngle() const {
        return angle;
    }

    float getPosX() const {
        return posX;
    }

    float getPosY() const {
        return posY;
    }

    void setSpeed(float newSpeed) {
        speed = newSpeed;
    }

    void setAngle(float newAngle) {
        angle = newAngle;
    }

    void setPosX(float newPosX) {
        posX = newPosX;
    }

    void setPosY(float newPosY) {
        posY = newPosY;
    }
};

class SizeComponent : public Component {
private:
    int width, height;
public:
    SizeComponent(int width, int height) : width(width), height(height) {}

    void setSize(int _width, int _height) {
        width = _width;
        height = _height;
    }

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }
};

class RenderLayerComponent : public Component {
private:
    int layer;
public:
    RenderLayerComponent(int layer) : layer(layer) {}

    int getLayer() {
        return layer;
    }

    void setLayer(int _layer) {
        layer = _layer;
    }
};

class TextureComponent : public Component {
public:
    float originX;
    float originY;
    int width;
    int height;
    sf::Texture texture;
    sf::Sprite sprite;

    explicit TextureComponent(const sf::Texture& tex, int width, int height) 
        : texture(tex), width(width), height(height), originX(0), originY(0) {
        sprite.setTexture(texture, true);
        updateSprite();
    }

    void setTexture(const sf::Texture& tex) {
        texture = tex;
        updateSprite();
    }

    void setWidth(int w) {
        width = w;
        updateSprite();
    }

    void setHeight(int h) {
        height = h;
        updateSprite();
    }

    void setOriginX(float x) {
        originX = x;
        sprite.setOrigin(originX, originY);
    }

    void setOriginY(float y) {
        originY = y;
        sprite.setOrigin(originX, originY);
    }

    const sf::Texture& getTexture() { return texture; }
    int getWidth() { return width; }
    int getHeight() { return height; }
    float getOriginX() { return originX; }
    float getOriginY() { return originY; }
    const sf::Sprite& getSprite() { return sprite; }

private:
    void updateSprite() {
        sprite.setTexture(texture, true);
        sprite.setTextureRect(sf::IntRect(0, 0, width, height));
        sprite.setOrigin(originX, originY);
    }
};
