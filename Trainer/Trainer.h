#pragma once
#include "../systems/EntityManager.h"
#include "../systems/Controller.h"
#include "../systems/CollisionSystem.h"
#include "../Battle/Battle.h"
#include "../systems/KeyboardSystem.h"
#include <SFML/Graphics.hpp>
#include "../const.h"

struct ConditionProps {
	Entity* trainer;
	Entity* player;
	std::vector<sf::Keyboard::Key> keys;
};
bool doesTrainerConditionSatisfy(ConditionProps props);
void initTrainerPokemons(EntityManager *em, TrainerPokemonsComponent *pokemons);

void trainerCollision(Controller *controller) {
	auto [input, em, render, state, battleContext] = controller->getAll();

	auto player = em->getEntitiesWithComponent<PlayerControlComponent>()[0];
	if (!player->getComponent<PlayersInventoryComponent>()->getPokemonCount()) return;
	auto trainers = em->getEntitiesWithComponent<TrainerComponent>();
	if (trainers.empty() || !player) return;
	auto keys = input->getPressedKeys();
	for (auto trainer : trainers) {
		if (trainer && doesTrainerConditionSatisfy({
				trainer, 
				player,
				keys
			})) {
			*state = GameState::Battle;
			input->clear();
			trainer->getComponent<TrainerComponent>()->setAttacked(true);
			initBattle(em, render, battleContext);
		}
	}
}

bool doesTrainerConditionSatisfy(ConditionProps props) {
	return isCollision(props.trainer, props.player) 
		&& isEnterPressed(props.keys);
}