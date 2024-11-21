#pragma once
#include "./systems/EntityManager.h"
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "const.h"



void init(sf::RenderWindow* window, EntityManager* entityManager) {
    const sf::Vector2i WINDOW_POSITION = window->getPosition();
    const sf::Vector2u WINDOW_SIZE = window->getSize();

	// Игрок
	auto player = entityManager->createEntity("player");
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
	player->addComponent<PlayerControlComponent>(true);
	sf::Texture playerTexture;
    if (playerTexture.loadFromFile("../res/Ash_sprite(16x32).png")) {
        player->addComponent<TextureComponent>(
			playerTexture, 
			PLAYER_WIDTH, 
			PLAYER_HEIGHT
		);
    }

    // Локация
    auto location = entityManager->createEntity("location");
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

	// Кнопка начала игры
	auto startButton = entityManager->createEntity("startGame");
	startButton->addComponent<PositionComponent>(
		WINDOW_WIDTH / 2 - START_BUTTON_WIDTH * 2, 
		WINDOW_HEIGHT / 2 - START_BUTTON_HEIGHT * 2
	);
	startButton->addComponent<SizeComponent>(
		START_BUTTON_WIDTH * 4, 
		START_BUTTON_HEIGHT * 4
	);
	startButton->addComponent<RenderLayerComponent>(1);
    startButton->addComponent<MenuTypeEntityComponent>();
	sf::Texture buttonTexture;
	if (buttonTexture.loadFromFile("../res/button(54x20).png")) {
		startButton->addComponent<TextureComponent>(
			buttonTexture, 
			START_BUTTON_WIDTH, 
			START_BUTTON_HEIGHT
		);
	}

	// Задний фон меню
	auto backgroundMenu = entityManager->createEntity("backgroundMenu");
	backgroundMenu->addComponent<PositionComponent>(0, 0);
	backgroundMenu->addComponent<SizeComponent>(WINDOW_WIDTH, WINDOW_HEIGHT);
	backgroundMenu->addComponent<RenderLayerComponent>(0);
    backgroundMenu->addComponent<MenuTypeEntityComponent>();
	sf::Texture backgroundMenuTexture;
	if (backgroundMenuTexture.loadFromFile("../res/background_menu(62x46).png")) {
		backgroundMenu->addComponent<TextureComponent>(
			backgroundMenuTexture, 
			BACKGROUND_MENU_WIDTH, 
			BACKGROUND_MENU_HEIGHT
		);
	}

	
}