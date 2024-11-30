#pragma once
#include <string>
#include <functional>

class Component {
public:
    virtual ~Component() = default;
};



// Костыль

class StartButtonComponent : public Component {
public:
    StartButtonComponent() {}
};
class CatchingArrowComponent : public Component {
public:
    CatchingArrowComponent() {}
};
class CatchingLocationComponent : public Component {
public:
    CatchingLocationComponent() {}
};
class PokeballComponent : public Component {
public:
    PokeballComponent() {}
};
class InventoryButtonCloseComponent : public Component {
public:
    InventoryButtonCloseComponent() {}
};
class ChestButtonCloseComponent : public Component {
public:
    ChestButtonCloseComponent() {}
};



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

class RotationComponent : public Component {
public:
    float speed;
    float angle;
    float posX;
    float posY;
    RotationComponent(float speed, float posX, float posY) : speed(speed), posX(posX), posY(posY) {}
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

class PlayerControlComponent : public Component {
public:
    bool isControlable;
    PlayerControlComponent(bool isControlable) : isControlable(isControlable) {}
};

class GameTypeEntityComponent : public Component {
public:
    GameTypeEntityComponent() {}
};

class MenuTypeEntityComponent : public Component {
public:
    MenuTypeEntityComponent() {}
};

class CatchingTypeEntityComponent : public Component {
public:
    CatchingTypeEntityComponent() {}
};

class InventoryTypeEntityComponent : public Component {
public:
    InventoryTypeEntityComponent() {}
};

class PokemonComponent : public Component {
private:
    std::string name;
    bool collected;
public:
    PokemonComponent(const std::string& name) 
        : name(name), collected(false) {}

    const std::string& getName() const {
        return name;
    }

    void setName(const std::string& newName) {
        name = newName;
    }

    bool isCollected() {
        return collected;
    }

    void setCollected(bool status) {
        collected = status;
    }
};

class AttackedPokemonComponent : public Component {
public:
    AttackedPokemonComponent() {}
};

class ThrowablePokeballComponent : public Component {
private:
    bool _isThrown;
    float angle;
public:
    ThrowablePokeballComponent() : _isThrown(false) {}
    void setThrown() {
        _isThrown = true;
    }
    bool isThrown() {
        return _isThrown;
    }
    void setAngle(float _angle) {
        angle = _angle;
    }
    float getAngle() {
        return angle;
    }
};

class PlayersInventoryComponent : public Component {
public:
    void addPokemon(int id) {
        collection.push_back(id);
    }

    void clearInventory() {
        collection.clear();
    }

    std::vector<int> getPokemons() {
        return collection;
    }

    size_t getPokemonCount() const {
        return collection.size();
    }

private:
    std::vector<int> collection;
};

class ChestComponent : public Component {
private:
    bool _isOpened;
public:
    ChestComponent() : _isOpened(false) {}
    void setOpened() {
        _isOpened = true;
    }
    bool isOpened() {
        return _isOpened;
    }
};

class ItemComponent : public Component {
public:
    ItemComponent() {}
};

class ChestContentComponent : public Component {
public:
    ChestContentComponent() {}
};

class ChestInterfaceComponent : public Component {
public:
    ChestInterfaceComponent() {}
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
    float originX;
    float originY;
    int width;
    int height;
    sf::Texture texture;
    sf::Sprite sprite;

    explicit TextureComponent(const sf::Texture& tex, int width, int height) : texture(tex) {
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, width, height));
        // sprite.setScale(
        //     width / tex.getSize().x,
        //     height / tex.getSize().y
        // );
    }
};