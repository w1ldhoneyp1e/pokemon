#pragma once
#include <string>
#include <functional>
#include "../Components/ParentComponent.h"

class EnemyPokemonComponent : public Component {};

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