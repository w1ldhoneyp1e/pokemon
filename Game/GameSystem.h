#pragma once
#include "../systems/EntityManager.h"
#include "../systems/RenderSystem.h"
#include "../Pokemon/PokemonSystem.h"
#include "../Shop/ShopSystem.h"
#include <SFML/Graphics.hpp>
#include "../Entity.h"
#include "../const.h"


void initPlayer(EntityManager* em);
void initGameLocation(EntityManager* em);
void initBulbasour(EntityManager* em);

void initTrainer(EntityManager* em);
void initTrainerPokemons(EntityManager *em, TrainerPokemonsComponent *pokemons);

void initGameEntities(Controller* controller	) {
    auto [input, em, render, state, battleContext, collisionMaps, currentLocation] = controller->getAll();
    initPlayer(em);
    initGameLocation(em);
	
    for(int i = 0; i < 3; ++i) {
        generatePokemon(controller);
    }
    initTrainer(em);
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
	player->addComponent<RenderLayerComponent>(1);
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

void initBulbasour(EntityManager* em) {
	auto bulbasour = em->createEntity();
    bulbasour->addComponent<PositionComponent>(
		BULBASOUR_POSITION_X, 
		BULBASOUR_POSITION_Y
	);
    bulbasour->addComponent<SizeComponent>(POKEMON_INVENTORY_WIDTH, POKEMON_INVENTORY_HEIGHT);
	bulbasour->addComponent<RenderLayerComponent>(1);
	bulbasour->addComponent<HealthComponent>(100, 100);
	bulbasour->addComponent<DamageComponent>(20, 30);
	bulbasour->addComponent<PokemonComponent>("Bulbasour");
    bulbasour->addComponent<GameTypeEntityComponent>();
    sf::Texture bulbasourTexture;
    if (bulbasourTexture.loadFromFile("../res/bulbasour(64x64).png")) {
        bulbasour->addComponent<TextureComponent>(
			bulbasourTexture,
			64, 
			64
		);
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
