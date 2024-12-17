#pragma once
#include "../Components/ParentComponent.h"

class ChestButtonGetComponent : public Component {};
class ChestButtonCloseComponent : public Component {};
class ChestInterfaceComponent : public Component {};

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