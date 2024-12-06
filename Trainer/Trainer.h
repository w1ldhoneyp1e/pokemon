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
	auto [input, em, render, state] = controller->getAll();

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
			initBattle(em, render);
		}
	}
}

void initTrainer(EntityManager *em) {
	auto trainer = em->createEntity();
    trainer->addComponent<PositionComponent>(
		TRAINER_POSITION_X, 
		TRAINER_POSITION_Y
	);
    trainer->addComponent<SizeComponent>(TRAINER_WIDTH, TRAINER_HEIGHT);
	trainer->addComponent<RenderLayerComponent>(1);
	trainer->addComponent<TrainerComponent>("First");
	trainer->addComponent<TrainerPokemonsComponent>();
	auto pokemons = trainer->getComponent<TrainerPokemonsComponent>();
	initTrainerPokemons(em, pokemons);
    trainer->addComponent<GameTypeEntityComponent>();
    sf::Texture trainerTexture;
    if (trainerTexture.loadFromFile("../res/trainer(16x24).png")) {
        trainer->addComponent<TextureComponent>(
			trainerTexture,
			16, 
			24
		);
    }
}

bool doesTrainerConditionSatisfy(ConditionProps props) {
	return isCollision(props.trainer, props.player) 
		&& isEnterPressed(props.keys);
}

void initTrainerPokemons(EntityManager *em, TrainerPokemonsComponent *pokemons) {
	auto charmander = em->createEntity();
	charmander->addComponent<PokemonComponent>("charmander");
	pokemons->addPokemon(charmander->getId());
}