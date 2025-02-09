#pragma once
#include "../systems/Controller.h"
#include "../Components/Components.h"
#include "../systems/ClickHandler.h"
#include "../Menu/MenuSystem.h"
#include "../TheEnd/TheEnd.h"

void initPauseMenu(EntityManager* em, RenderSystem* render);
void initPauseBackground(EntityManager* em);
void initPauseLayout(EntityManager* em);
void initPauseButtons(EntityManager* em);
void initContinueButton(EntityManager* em);

void clearPauseMenu(EntityManager* em, RenderSystem* render);
void initRestartButton(EntityManager* em);

void onPauseGame(Controller* controller) {
	auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();

	auto keys = input->getPressedKeys();

	if (std::find(keys.begin(), keys.end(), sf::Keyboard::Escape) != keys.end()) {
		*state = GameState::Pause;
		initPauseMenu(em, render);
		input->clear();
	}
}

void handlePauseGame(Controller* controller) {
	auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();

	if (!input->hasMouseClick()) return;
	
	auto pauseButtons = em->getEntitiesWithComponent<PauseButtonComponent>();
	for (auto pauseButton : pauseButtons) {
		if (pauseButton->getComponent<PauseButtonComponent>()->getType() == PauseButtonType::Continue) {
			if (isClickOnEntity(input->getMouseClick(), pauseButton)) {
				clearPauseMenu(em, render);
				*state = GameState::Game;
				input->clear();
				return;
			}
		}

		if (pauseButton->getComponent<PauseButtonComponent>()->getType() == PauseButtonType::Restart) {
			if (isClickOnEntity(input->getMouseClick(), pauseButton)) {
				clearGameEntities(em, render);
				initMenu(em, render);
				*state = GameState::Menu;
				input->clear();
				return;
			}
		}
	}

}

void initPauseMenu(EntityManager* em, RenderSystem* render) {
	initPauseBackground(em);
	initPauseLayout(em);
	initPauseButtons(em);

	render->addEntities(em->getEntitiesWithComponent<PauseComponent>());
}

void initPauseBackground(EntityManager* em) {
	auto pauseBackground = em->createEntity();
	pauseBackground->addComponent<PauseComponent>();
	pauseBackground->addComponent<PositionComponent>(0, 0);
	pauseBackground->addComponent<SizeComponent>(WINDOW_WIDTH, WINDOW_HEIGHT);
	pauseBackground->addComponent<ShapeComponent>(ShapeType::Rectangle, sf::Color(68, 68, 68, 90));
	pauseBackground->addComponent<RenderLayerComponent>(10);
}

void initPauseLayout(EntityManager* em) {
	auto pauseLayout = em->createEntity();
	pauseLayout->addComponent<PauseComponent>();
	pauseLayout->addComponent<PositionComponent>(PAUSE_LAYOUT_POSITION_X, PAUSE_LAYOUT_POSITION_Y);
	pauseLayout->addComponent<SizeComponent>(PAUSE_LAYOUT_WIDTH, PAUSE_LAYOUT_HEIGHT);
	pauseLayout->addComponent<RenderLayerComponent>(11);
	sf::Texture pauseLayoutTexture;

	if (pauseLayoutTexture.loadFromFile("../res/background_menu(62x46).png")) {
		pauseLayout->addComponent<TextureComponent>(pauseLayoutTexture, 62, 46);
	}
}

void initPauseButtons(EntityManager* em) {
	initContinueButton(em);
	initRestartButton(em);
}

void initContinueButton(EntityManager* em) {
	auto continueButton = em->createEntity();
	continueButton->addComponent<PauseButtonComponent>(PauseButtonType::Continue);
	continueButton->addComponent<PauseComponent>();
	continueButton->addComponent<PositionComponent>(PAUSE_LAYOUT_POSITION_X + 30, PAUSE_LAYOUT_POSITION_Y + 30);
	continueButton->addComponent<SizeComponent>(80, 40);
	continueButton->addComponent<RenderLayerComponent>(12);
	continueButton->addComponent<TextComponent>(
		"Continue", 
		PAUSE_LAYOUT_POSITION_X + 30, 
		PAUSE_LAYOUT_POSITION_Y + 30, 
		28, 
		sf::Color(68, 68, 68)
	);
}

void initRestartButton(EntityManager* em) {
	auto restartButton = em->createEntity();
	restartButton->addComponent<PauseButtonComponent>(PauseButtonType::Restart);
	restartButton->addComponent<PauseComponent>();
	restartButton->addComponent<PositionComponent>(PAUSE_LAYOUT_POSITION_X + 30, PAUSE_LAYOUT_POSITION_Y + 30 + 30);
	restartButton->addComponent<SizeComponent>(80, 40);
	restartButton->addComponent<RenderLayerComponent>(12);
	restartButton->addComponent<TextComponent>(
		"Restart", 
		PAUSE_LAYOUT_POSITION_X + 30, 
		PAUSE_LAYOUT_POSITION_Y + 30 + 30, 
		28, 
		sf::Color(68, 68, 68)
	);
}

void clearPauseMenu(EntityManager* em, RenderSystem* render) {
	auto pauseEntities = em->getEntitiesWithComponent<PauseComponent>();
	for (auto pauseEntity : pauseEntities) {
		render->removeEntity(pauseEntity->getId());
		em->removeEntity(pauseEntity);
	}
}

