#include <cmath>
#include "./systems/WindowManager.h"
#include "./systems/ClickHandler.h"
#include "./systems/EntityManager.h"
#include "./systems/RenderSystem.h"
#include "./systems/InputSystem.h"
#include "./Player/PlayerMovementSystem.h"
#include "Entity.h"
#include "Initialize.h"
#include <iostream>



void pollEvents(sf::RenderWindow* window, InputSystem* inputSystem) {
	sf::Event event;
	while (window->pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window->close();
		inputSystem->handleEvent(event);
	}
}

void update(EntityManager* entityManager, InputSystem* inputSystem, RenderSystem* renderSystem, GameState* state) {
	auto keys = inputSystem->getPressedKeys();
	bool isClicked = false;
	switch (*state)
	{
	case GameState::Menu: {
		auto button = entityManager->getEntity("startGame");
		if (inputSystem->hasMouseClick()) 
			if (isClickOnEntity(inputSystem->getMouseClick(), button)) {
				*state = GameState::Game;
				renderSystem->removeEntities();
				renderSystem->addEntities(entityManager->getEntitiesWithComponent<GameTypeEntityComponent>());
			}
		break;
	}
	case GameState::Game:
		if (keys.empty()) return;
		playerMovementSystem(entityManager->getEntitiesWithComponent<PlayerControlComponent>().back(), keys.back());
	break;
	
	default:
		break;
	}
	
}

int main() {
	GameState state = GameState::Menu;
	WindowManager windowManager;
	windowManager.createWindow();
	sf::RenderWindow* window = windowManager.getWindow();
	EntityManager entityManager;
	RenderSystem renderSystem(window, &entityManager);

	std::unordered_map<GameState, std::vector<Entity*>> allEntities;
	init(&allEntities, &entityManager);
	

	InputSystem inputSystem;

	renderSystem.addEntities(allEntities[GameState::Menu]);

	while (window->isOpen()) {
		pollEvents(window, &inputSystem);
		update(&entityManager, &inputSystem, &renderSystem, &state);
		renderSystem.render();
	}
	return 0;
}
