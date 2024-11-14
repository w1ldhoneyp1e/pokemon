#pragma once
#include <string>
#include <functional>

class Component {
public:
    virtual ~Component() = default;
};

class PositionComponent : public Component {
public:
    float x, y;
    PositionComponent(float x, float y) : x(x), y(y) {}
};

class SpeedComponent : public Component {
public:
    float speed;
    SpeedComponent(float speed) : speed(speed) {}
};

class SizeComponent : public Component {
public:
    int width, height;
    SizeComponent(int width, int height) : width(width), height(height) {}
};

class PlayerControlComponent : public Component {
public:
    bool isControlable;
    PlayerControlComponent(bool isControlable) : isControlable(isControlable) {}
};

class GameTypeEntityComponent : public Component {
public:
    GameTypeEntityComponent() {}
};

class OnClickComponent : public Component {
public:
    std::function<void()> onClick;

    OnClickComponent(std::function<void()> onClick) : onClick(onClick) {}

    void triggerClick() {
        if (onClick) {
            onClick();
        }
    }
};

class TextureComponent : public Component {
public:
    int width;
    int height;
    sf::Texture texture;
    sf::Sprite sprite;

    explicit TextureComponent(const sf::Texture& tex, int width, int height) : texture(tex) {
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, width, height));
    }
};