#pragma once
#include "../systems/EntityManager.h"
#include "../systems/InputSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/CollisionSystem.h"
#include "../Catching/CatchingSystem.h"
#include "../GameState.h"
#include "../const.h"

struct ConditionProps {
	Entity* pokemon;
	Entity* player;
	PlayersInventoryComponent* inventory;
	std::vector<sf::Keyboard::Key> keys;
};

bool isInventoryFull(PlayersInventoryComponent *inventory);
bool isPokemonCollected(Entity* pokemon);
bool isEnterPressed(std::vector<sf::Keyboard::Key> keys);
bool doesConditionSatisfy(ConditionProps props);

void pokemonCollision(
	InputSystem* inputSystem,
	EntityManager* entityManager,
	RenderSystem* renderSystem,
	GameState* state
) {
	// Селектим покемонов. Для каждого проверяем, не было ли коллизии
	auto pokemons = entityManager->getEntitiesWithComponent<PokemonComponent>();
	auto player = entityManager->getEntity("player");
	auto inventory = player->getComponent<PlayersInventoryComponent>();
	auto keys = inputSystem->getPressedKeys();
	for (auto pokemon : pokemons) {
		if (doesConditionSatisfy({ // условие должно быть простым
				pokemon, 
				player,
				inventory,
				keys
			})) {
			*state = GameState::Catching;
			inputSystem->clear();
			pokemon->addComponent<AttackedPokemonComponent>();
			renderSystem->removeEntity(pokemon->getId());
			pokemon->removeComponent<GameTypeEntityComponent>();
			initCatching(entityManager, renderSystem);
		}
	}
}

bool doesConditionSatisfy(ConditionProps props) {
	return isCollision(props.pokemon, props.player) 
		&& isInventoryFull(props.inventory)
		&& isPokemonCollected(props.pokemon)
		&& isEnterPressed(props.keys);
}

bool isEnterPressed(std::vector<sf::Keyboard::Key> keys) {
	return std::find(keys.begin(), keys.end(), sf::Keyboard::Enter) != keys.end();
}

bool isInventoryFull(PlayersInventoryComponent *inventory) {
	return inventory->getPokemonCount() < INVENTORY_CELLS_LIMIT;
}

bool isPokemonCollected(Entity* pokemon) {
	return !pokemon->getComponent<PokemonComponent>()->isCollected();
}