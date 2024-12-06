#pragma once
#include "../systems/EntityManager.h"
#include "../systems/RenderSystem.h"
#include <SFML/Graphics.hpp>
#include "../Entity.h"
#include "../Trainer/Trainer.h"
#include "../const.h"

void initPlayer(EntityManager* em);
void initGameLocation(EntityManager* em);
void initBulbasour(EntityManager* em);
void initChest(EntityManager* em);

void initGameEntities(EntityManager* em) {
    initPlayer(em);
    initGameLocation(em);
    initBulbasour(em);
    initChest(em);
	initTrainer(em);
}

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
	player->addComponent<SpeedComponent>(PLAYER_SPEED);
    player->addComponent<GameTypeEntityComponent>();
	player->addComponent<RenderLayerComponent>(1);
	player->addComponent<PlayersInventoryComponent>();
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
    if (locationTexture.loadFromFile("../res/location(767x639).png")) {
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

void initChest(EntityManager* em) {
	auto chest = em->createEntity();
    chest->addComponent<PositionComponent>(
		PLAYER_START_POSITION_X - 100, 
		PLAYER_START_POSITION_Y
	);
    chest->addComponent<SizeComponent>(CHEST_WIDTH, CHEST_HEIGHT);
	chest->addComponent<RenderLayerComponent>(1);
    chest->addComponent<ChestComponent>();
    chest->addComponent<GameTypeEntityComponent>();
    sf::Texture chestTexture;
    if (chestTexture.loadFromFile("../res/chestClosed(24x23).png")) {
        chest->addComponent<TextureComponent>(
			chestTexture,
			CHEST_WIDTH, 
			CHEST_HEIGHT
		);
    }
}