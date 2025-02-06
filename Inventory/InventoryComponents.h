#pragma once
#include "../Components/ParentComponent.h"
#include "../const.h"
#include <vector>

class InventoryButtonCloseComponent : public Component {};
class InventoryTypeEntityComponent : public Component {};
class InventoryCoinsAmountComponent : public Component {};

class PlayersInventoryComponent : public Component {
private:
	std::vector<int> pokemonCollection;
	int coins = 0;
	int potions = 0;
	int pokeballs = 0;

public:
    void addPokemon(int id) {
        pokemonCollection.push_back(id);
    }

    void clearInventory() {
        pokemonCollection.clear();
    }

    std::vector<int> getPokemons() {
        return pokemonCollection;
    }

    size_t getPokemonCount() const {
        return pokemonCollection.size();
    }

    void addCoins(int amount) { coins += amount; }
    void removeCoins(int amount) { coins = std::max(0, coins - amount); }
    int getCoinCount() const { return coins; }

    void addPotions(int amount) { potions += amount; }
    void removePotions(int amount) { potions = std::max(0, potions - amount); }
    int getPotionCount() const { return potions; }

    void addPokeballs(int amount) { pokeballs += amount; }
    void removePokeballs(int amount) { pokeballs = std::max(0, pokeballs - amount); }
    int getPokeballCount() const { return pokeballs; }
};

class CoinComponent : public Component {};
class HealingPotionComponent : public Component {}; 