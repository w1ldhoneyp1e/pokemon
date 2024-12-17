#pragma once
#include "../Components/ParentComponent.h"
#include <vector>

class InventoryButtonCloseComponent : public Component {};
class InventoryTypeEntityComponent : public Component {};

class PlayersInventoryComponent : public Component {
private:
	std::vector<int> collection;
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


};