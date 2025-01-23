#pragma once
#include "../systems/EntityManager.h"
#include "../systems/InputSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/CollisionSystem.h"
#include "../systems/KeyboardSystem.h"
#include "../Catching/CatchingSystem.h"
#include "../GameState.h"
#include "../const.h"

struct PokemonConditionProps {
	Entity* pokemon;
	Entity* player;
	PlayersInventoryComponent* inventory;
	std::vector<sf::Keyboard::Key> keys;
};

bool isInventoryFull(PlayersInventoryComponent *inventory);
bool isPokemonCollected(Entity* pokemon);
bool doesConditionSatisfy(PokemonConditionProps props);

void pokemonCollision(Controller* controller) {
	// Селектим покемонов. Для каждого проверяем, не было ли коллизии
	auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();

	auto pokemons = em->getEntitiesWithComponent<PokemonComponent>();
	auto player = em->getEntitiesWithComponent<PlayerControlComponent>()[0];
	auto inventory = player->getComponent<PlayersInventoryComponent>();
	auto keys = input->getPressedKeys();
	if (keys.empty()) return;
	for (auto pokemon : pokemons) {
		if (pokemon->getComponent<GameTypeEntityComponent>() == nullptr) continue;;
		if (doesConditionSatisfy({
				pokemon, 
				player,
				inventory,
				keys
			})) {
			*state = GameState::Catching;
			input->clear();
			pokemon->addComponent<AttackedPokemonComponent>();
			render->removeEntity(pokemon->getId());
			pokemon->removeComponent<GameTypeEntityComponent>();
			initCatching(em, render);
		}
	}
}

bool doesConditionSatisfy(PokemonConditionProps props) {
	return isCollision(props.pokemon, props.player) 
		&& isInventoryFull(props.inventory)
		&& isPokemonCollected(props.pokemon)
		&& isEnterPressed(props.keys);
}

bool isInventoryFull(PlayersInventoryComponent *inventory) {
	return inventory->getPokemonCount() < INVENTORY_CELLS_LIMIT;
}

bool isPokemonCollected(Entity* pokemon) {
	return !pokemon->getComponent<PokemonComponent>()->isCollected();
}