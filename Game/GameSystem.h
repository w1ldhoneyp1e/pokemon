#pragma once
#include "../systems/EntityManager.h"
#include "../systems/RenderSystem.h"
#include "../Pokemon/PokemonSystem.h"
#include "../Onboarding/OnboardingSystem.h"
#include "../Shop/ShopSystem.h"
#include <SFML/Graphics.hpp>
#include "../Entity.h"
#include "../const.h"


void initPlayer(EntityManager* em);
void initGameLocation(EntityManager* em);

void initTrainer(Controller* controller);
void initTrainerPokemons(EntityManager *em, TrainerPokemonsComponent *pokemons);
void initAreaForFinalQuest(Controller* controller, Entity* trainer);

void initGameEntities(Controller* controller) {
    auto [input, em, render, state, battleContext, collisionMaps, currentLocation] = controller->getAll();
    initPlayer(em);
    initGameLocation(em);
	
    for(int i = 0; i < 3; ++i) {
        generatePokemon(controller);
    }
    initTrainer(controller);
	initShop(em);
}

std::array<std::vector<sf::Texture>, 4> getPlayerTextures();

void initPlayer(EntityManager* em) {
	auto player = em->createEntity();
	player->addComponent<PositionComponent>(
		PLAYER_START_POSITION_X,
		PLAYER_START_POSITION_Y
	);
	player->addComponent<SizeComponent>(
		PLAYER_WIDTH,
		PLAYER_HEIGHT
	);
	sf::Texture playerSprites;
	if (playerSprites.loadFromFile("../res/playerSprites(204x20).png")) {
		player->addComponent<AnimationComponent>(
			playerSprites,
			playerSprites.getSize().x / 12,
			playerSprites.getSize().y,
			5.0f
		);
	}
	player->getComponent<AnimationComponent>()->currentFrameIndex = 1;
	player->getComponent<AnimationComponent>()->currentDirection = 1;
	player->addComponent<SpeedComponent>(PLAYER_SPEED);
    player->addComponent<GameTypeEntityComponent>();
	player->addComponent<RenderLayerComponent>(2);
	player->addComponent<PlayersInventoryComponent>();
	auto inventory = player->getComponent<PlayersInventoryComponent>();
	inventory->addPokeballs(3);
	player->addComponent<PlayerControlComponent>(true);
	sf::Texture playerTexture;
    if (playerTexture.loadFromFile("../res/Ash_sprite(16x32).png")) {
        player->addComponent<TextureComponent>(
			playerTexture, 
			PLAYER_WIDTH, 
			PLAYER_HEIGHT
		);
    }
}

void initGameLocation(EntityManager* em) {
	auto location = em->createEntity();
    location->addComponent<PositionComponent>(0, 0);
    location->addComponent<SizeComponent>(WINDOW_WIDTH, WINDOW_HEIGHT);
	location->addComponent<RenderLayerComponent>(0);
    location->addComponent<GameTypeEntityComponent>();
    sf::Texture locationTexture;
    if (locationTexture.loadFromFile("../res/townLocation(383x320).png")) {
        location->addComponent<TextureComponent>(
			locationTexture,
			WINDOW_WIDTH, 
			WINDOW_HEIGHT
		);
    }
}

void initTrainer(Controller* controller) {
	auto em = controller->getEntityManager();

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
	initAreaForFinalQuest(controller, trainer);
}

void initAreaForFinalQuest(Controller* controller, Entity* trainer) {
	auto [input, em, render, state, battleContext, collisionMaps, currentLocation] = controller->getAll();

	auto area = em->createEntity();
	area->addComponent<PositionComponent>(
		trainer->getComponent<PositionComponent>()->getX(), 
		trainer->getComponent<PositionComponent>()->getY() + trainer->getComponent<SizeComponent>()->getHeight()
	);
	area->addComponent<SizeComponent>(16, 11);
	area->addComponent<GameTypeEntityComponent>();
	area->addComponent<QuestAreaComponent>(trainer->getId());
	area->addComponent<RenderLayerComponent>(1);
    area->addComponent<ShapeComponent>(ShapeType::Ellipse, sf::Color(68, 68, 68, 204));
}

void initTrainerPokemons(EntityManager *em, TrainerPokemonsComponent *pokemons) {
	auto charmander = em->createEntity();
	charmander->addComponent<PokemonComponent>("charmander");
	pokemons->addPokemon(charmander->getId());
}

std::array<std::vector<sf::Texture>, 4> getPlayerTextures() {
	return {
		std::vector<sf::Texture> {
			sf::Texture(),
			sf::Texture()
		},
		std::vector<sf::Texture> {
			sf::Texture(),
			sf::Texture()
		},
		std::vector<sf::Texture> {
			sf::Texture(),
			sf::Texture()
		},
		std::vector<sf::Texture> {
			sf::Texture(),
			sf::Texture()
		}
	};
}
