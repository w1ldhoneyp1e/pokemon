#pragma once
#include <string>
#include "../Components/ParentComponent.h"

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

class AttackedPokemonComponent : public Component {};