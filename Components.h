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
class ChestButtonGetComponent : public Component {
public:
    ChestButtonGetComponent() {}
};
enum AttackDir {
    Head,
    Body,
	Feet
};
class BattleArrowComponent : public Component {
private:
    AttackDir dir;
public:
    BattleArrowComponent(AttackDir dir) : dir(dir) {}
    AttackDir getDir() {
        return dir;
    }
    void setDir(AttackDir _dir) {
        dir = _dir;
    }
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

class HealthComponent : public Component {
private:
    float current;
    float total;
    bool _isVisible;
public:
    HealthComponent(float total, float current, bool isVisible = false)
        : total(total), current(current), _isVisible(isVisible) {}

    float getTotal() const {
        return total;
    }

    float getCurrent() const {
        return current;
    }

    float isVisible() const {
        return _isVisible;
    }

    void setTotal(float value) {
        total = value;
    }

    void setCurrent(float value) {
        current = value;
    }

    void setVisible(float value) {
        _isVisible = value;
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

class BattleTypeEntityComponent : public Component {
public:
    BattleTypeEntityComponent() {}
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

class TrainerComponent : public Component {
private:
    std::string name;
    bool attacked;
public:
    TrainerComponent(const std::string& name) 
        : name(name), attacked(false) {}

    const std::string& getName() const {
        return name;
    }

    void setName(const std::string& newName) {
        name = newName;
    }

    bool isAttacked() {
        return attacked;
    }

    void setAttacked(bool status) {
        attacked = status;
    }
};

class TrainerPokemonsComponent : public Component {
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
    int id;
    static int idCounter;
public:
    ChestComponent() : _isOpened(false), id(idCounter++) {}

    void setOpened() {
        _isOpened = true;
    }

    bool isOpened() const {
        return _isOpened;
    }

    int getId() const {
        return id;
    }
};
int ChestComponent::idCounter = 0;

class ItemComponent : public Component {
public:
    ItemComponent() {}
};

class ChestContentComponent : public Component {
    int chestId;
public:
    ChestContentComponent(int _chestId) : chestId(_chestId) {}
    int getId() const {
        return chestId;
    }
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

    explicit TextureComponent(const sf::Texture& tex, int width, int height) : 
        texture(tex), width(width), height(height), originX(0), originY(0) {
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

    const sf::Texture& getTexture() {
        return texture;
    }
    int getWidth() { 
        return width;
    }
    int getHeight() { 
        return height; 
    }
    float getOriginX() { 
        return originX; 
    }
    float getOriginY() { 
        return originY; 
    }
    const sf::Sprite& getSprite() {
        return sprite;
    }


private:
    void updateSprite() {
        sprite.setTexture(texture, true);
        sprite.setTextureRect(sf::IntRect(0, 0, width, height));
        sprite.setOrigin(originX, originY);
    }
};