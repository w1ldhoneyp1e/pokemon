#pragma once
#include "../systems/Controller.h"
#include "../Menu/MenuSystem.h"
#include "../const.h"

void createBackToMenuButton(Controller* controller);
void onBackToMenuClick(Controller* controller);


void clearGameEntities(EntityManager* em, RenderSystem* render) {
	em->clear();
	render->removeEntities();
}

void createTheEndBackground(Controller* controller) {
	auto em = controller->getEntityManager();
	auto render = controller->getRenderSystem();
	auto background = em->createEntity();
	background->addComponent<PositionComponent>(0, 0);
	background->addComponent<SizeComponent>(WINDOW_WIDTH, WINDOW_HEIGHT);
	background->addComponent<RenderLayerComponent>(0);
	sf::Texture theEndTexture;
	if (theEndTexture.loadFromFile("../res/the-end(384x320).png")) {
		background->addComponent<TextureComponent>(theEndTexture, 384, 320);
	}
	render->addEntity(background);
}

void createTheEndText(Controller* controller) {
	auto em = controller->getEntityManager();
	auto render = controller->getRenderSystem();
	auto text = em->createEntity();
	text->addComponent<PositionComponent>(THE_END_TEXT_X, THE_END_TEXT_Y);
	text->addComponent<RenderLayerComponent>(1);
	text->addComponent<TextComponent>("The End", THE_END_TEXT_X, THE_END_TEXT_Y, THE_END_TEXT_SIZE, sf::Color(68, 68, 68));
	render->addEntity(text);
}


void createTheEnd(Controller* controller) {
	createTheEndBackground(controller);
	createTheEndText(controller);
}


float theEndTimer = 0.0f;

void onGameEnd(Controller* controller, float deltaTime) {
	auto em = controller->getEntityManager();
	auto render = controller->getRenderSystem();
	auto input = controller->getInputSystem();

	bool mouseClicked = input->hasMouseClick();
	auto mouseClick = input->getMouseClick();

	auto backToMenuButtons = em->getEntitiesWithComponent<BackToMenuButtonComponent>();

	theEndTimer += deltaTime;
	if (backToMenuButtons.empty() && theEndTimer >= THE_END_TIMER) {
		createBackToMenuButton(controller);
	}
	if (!backToMenuButtons.empty() && mouseClicked && isClickOnEntity(mouseClick, backToMenuButtons[0])) {
		input->clear();
		onBackToMenuClick(controller);
	}
}

void createBackToMenuButton(Controller* controller) {
	auto render = controller->getRenderSystem();
	auto em = controller->getEntityManager();

	auto backToMenuButtonTexture = em->createEntity();
	backToMenuButtonTexture->addComponent<PositionComponent>(THE_END_BACK_TO_MENU_TEXT_X - 23, THE_END_BACK_TO_MENU_TEXT_Y - 12);
	backToMenuButtonTexture->addComponent<SizeComponent>(82 * 2, 34);
	backToMenuButtonTexture->addComponent<RenderLayerComponent>(1);
	backToMenuButtonTexture->addComponent<BackToMenuButtonComponent>();
	sf::Texture theEndTexture;

	if (theEndTexture.loadFromFile("../res/sky-button(82x34).png")) {
		backToMenuButtonTexture->addComponent<TextureComponent>(theEndTexture, 82, 34);
	}
	render->addEntity(backToMenuButtonTexture);

	auto backToMenuButtonText = em->createEntity();
	backToMenuButtonText->addComponent<PositionComponent>(THE_END_BACK_TO_MENU_TEXT_X, THE_END_BACK_TO_MENU_TEXT_Y);
	backToMenuButtonText->addComponent<SizeComponent>(200, 100);
	backToMenuButtonText->addComponent<RenderLayerComponent>(1);
	backToMenuButtonText->addComponent<BackToMenuButtonComponent>();
	backToMenuButtonText->addComponent<TextComponent>(
		"Back to Menu", 
		THE_END_BACK_TO_MENU_TEXT_X, 
		THE_END_BACK_TO_MENU_TEXT_Y, 
		THE_END_BACK_TO_MENU_TEXT_SIZE, 
		sf::Color(68, 68, 68)
	);
	render->addEntity(backToMenuButtonText);
}


void onBackToMenuClick(Controller* controller) {
	auto em = controller->getEntityManager();
	auto render = controller->getRenderSystem();

	clearGameEntities(em, render);
	initMenu(em, render);
	*controller->getState() = GameState::Menu;
}

