#pragma once
#include "../systems/EntityManager.h"
#include "../systems/InputSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/CollisionSystem.h"
#include "../Catching/CatchingSystem.h"
#include "../GameState.h"
#include "../const.h"
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
		if (
			isCollision(pokemon, player) 
			&& inventory->getPokemonCount() < INVENTORY_CELLS_LIMIT
			&& !pokemon->getComponent<PokemonComponent>()->isCollected()
			&& std::find(keys.begin(), keys.end(), sf::Keyboard::Enter) != keys.end()
		) {
			*state = GameState::Catching;
			inputSystem->clear();
			pokemon->addComponent<AttackedPokemonComponent>();
			renderSystem->removeEntity(pokemon->getId());
			// inventory->addPokemon(pokemon->getId());
			// pokemon->getComponent<PokemonComponent>()->setCollected(true);
			pokemon->removeComponent<GameTypeEntityComponent>();
			initCatching(entityManager, renderSystem);
		}
	}
}