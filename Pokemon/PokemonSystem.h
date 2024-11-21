#pragma once
#include "../systems/EntityManager.h"
#include "../systems/RenderSystem.h"
#include "../systems/CollisionSystem.h"
#include "../const.h"
void pokemonCollision(
	EntityManager* entityManager,
	RenderSystem* renderSystem
) {
	// Селектим покемонов. Для каждого проверяем, не было ли коллизии
	auto pokemons = entityManager->getEntitiesWithComponent<PokemonComponent>();
	auto player = entityManager->getEntity("player");
	auto inventory = player->getComponent<PlayersInventoryComponent>();
	for (auto pokemon : pokemons) {
		if (
			isCollision(pokemon, player) 
			&& inventory->getPokemonCount() < INVENTORY_CELLS_LIMIT
			&& !pokemon->getComponent<PokemonComponent>()->isCollected()
		) {
			renderSystem->removeEntity(pokemon->getId());
			inventory->addPokemon(pokemon->getId());
			pokemon->getComponent<PokemonComponent>()->setCollected(true);
		}
	}
}