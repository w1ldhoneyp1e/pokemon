#pragma once
#include <string>
#include "../Components/ParentComponent.h"

struct PokemonStats {
    std::string name;
    int baseHealth;
    int minDamage;
    int maxDamage;
    std::string texturePath;
};

const std::vector<PokemonStats> POKEMON_TYPES = {
    {
        "Bulbasaur", 
        100, 
        20, 
        30, 
        "../res/bulbasour(48x48).png"
    },
    {
        "Charmander", 
        90, 
        25, 
        35, 
        "../res/charmander(48x48).png"
    },
    {
        "Squirtle", 
        110, 
        15, 
        25, 
        "../res/squirtle(48x48).png"
    },
    {
        "Pikachu", 
        85, 
        30, 
        40, 
        "../res/pikachu(48x48).png"
    }
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

class PokemonSellComponent : public Component {
private:
    int id;
    int price;
public:
    PokemonSellComponent(int id, int price) : id(id), price(price) {}

    const int& getId() const {
        return id;
    }

    int getPrice() const {
        return price;
    }
};

class AttackedPokemonComponent : public Component {};