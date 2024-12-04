#pragma once
#include "../systems/EntityManager.h"
#include "../systems/RenderSystem.h"
#include <SFML/Graphics.hpp>
#include "../Entity.h"
#include "../const.h"

void initPlayer(EntityManager* entityManager);
void initLocation(EntityManager* entityManager);
void initBulbasour(EntityManager* entityManager);
void initChest(EntityManager* entityManager);

void initGameEntities(EntityManager* entityManager) {
    initPlayer(entityManager);
    initLocation(entityManager);
    initBulbasour(entityManager);
    initChest(entityManager);
}

void initPlayer(EntityManager* entityManager) {
	auto player = entityManager->createEntity();
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

void initLocation(EntityManager* entityManager) {
	auto location = entityManager->createEntity();
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

void initBulbasour(EntityManager* entityManager) {
	auto bulbasour = entityManager->createEntity();
    bulbasour->addComponent<PositionComponent>(
		BULBASOUR_POSITION_X, 
		BULBASOUR_POSITION_Y
	);
    bulbasour->addComponent<SizeComponent>(POKEMON_INVENTORY_WIDTH, POKEMON_INVENTORY_HEIGHT);
	bulbasour->addComponent<RenderLayerComponent>(1);
	bulbasour->addComponent<PokemonComponent>("Bulbasour");
    bulbasour->addComponent<GameTypeEntityComponent>();
    sf::Texture bulbasourTexture;
    if (bulbasourTexture.loadFromFile("../res/bulbasour(36x33).png")) {
        bulbasour->addComponent<TextureComponent>(
			bulbasourTexture,
			POKEMON_INVENTORY_WIDTH, 
			POKEMON_INVENTORY_HEIGHT
		);
    }
}

void initChest(EntityManager* entityManager) {
	auto chest = entityManager->createEntity();
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