#pragma once
#include "../systems/EntityManager.h"
#include "../systems/RenderSystem.h"
#include "../Game/GameSystem.h"
#include <SFML/Graphics.hpp>
#include "../Entity.h"
#include "../const.h"

void onStartGameButtonPress(Controller* controller) {
	auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();
	auto button = em->getEntitiesWithComponent<StartButtonComponent>()[0];
	if (!input->hasMouseClick()) return;
	if (!isClickOnEntity(input->getMouseClick(), button)) return;
	*state = GameState::Game;
	initGameEntities(controller);
	render->removeEntities();

	render->addEntities(em->getEntitiesWithComponent<GameTypeEntityComponent>());
	input->clear();
}

void backToMenu(Controller* controller) {
	auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();
	auto keys = input->getPressedKeys();

	if (std::find(keys.begin(), keys.end(), sf::Keyboard::Escape) == keys.end()) return;
	*state = GameState::Menu;
	render->removeEntities();
	render->addEntities(em->getEntitiesWithComponent<MenuTypeEntityComponent>());
	input->clear();
}

void initButtonGameStart(EntityManager* em);
void initBackgroundMenu(EntityManager* em);

void initMenu(EntityManager* em, RenderSystem* render) {
	initButtonGameStart(em);
    initBackgroundMenu(em);
	render->addEntities(em->getEntitiesWithComponent<MenuTypeEntityComponent>());
}

void initButtonGameStart(EntityManager* em) {
	auto startButton = em->createEntity();
	startButton->addComponent<StartButtonComponent>(); // Убрать
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
}

void initBackgroundMenu(EntityManager* em) {
	auto backgroundMenu = em->createEntity();
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