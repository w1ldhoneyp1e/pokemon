#include <cmath>
#include "./systems/WindowManager.h"
#include "./systems/ClickHandler.h"
#include "./systems/EntityManager.h"
#include "./systems/Controller.h"
#include "./systems/RenderSystem.h"
#include "./systems/InputSystem.h"
#include "./Player/PlayerMovementSystem.h"
#include "./Inventory/InventorySystem.h"
#include "./Chests/ChestSystem.h"
#include "./Battle/Battle.h"
#include "./Trainer/Trainer.h"
#include "./Pokemon/PokemonSystem.h"
#include "./Menu/MenuSystem.h"
#include "./Time/TimeSystem.h"
#include "./Catching/CatchingSystem.h"
#include "GameState.h"
#include "Entity.h"
#include <iostream>
#include <chrono>
#include <random>


void pollEvents(sf::RenderWindow* window, InputSystem* inputSystem) {
	sf::Event event;
	while (window->pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window->close();
		inputSystem->handleEvent(event);
	}
}

void handleEvent(sf::RenderWindow* window, Controller* controller) {
	sf::Event event;
	while (window->pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window->close();
		controller->getInputSystem()->handleEvent(event);
	}
}

void update(Controller* controller, float deltaTime) {
	auto [input, em, render, state, battleContext] = controller->getAll();
	auto keys = input->getPressedKeys();
	bool isClicked = false;
	switch (*state)
	{
	case GameState::Menu: {
		onStartGameButtonPress(controller);
		break;
	}

	case GameState::Game:
		if (keys.empty()) return;
		playerMovementSystem(em, input, deltaTime);
		backToMenu(controller);
		openInventory(controller);
		pokemonCollision(controller);
		chestOpening(controller);
		trainerCollision(controller);
	break;

	case GameState::Inventory: {
		closeInventory(controller);
		break;
	}

	case GameState::Catching: {
		updateCatching(controller, deltaTime);
		break;
	}

	case GameState::Chest: {
		updateChests(controller);
		break;
	}

	case GameState::Battle: {
		updateBattle(controller);
		break;
	}
	
	default:
		break;
	}
	
}

void gameLoop(sf::RenderWindow* window, Controller* controller) {
    auto startTime = std::chrono::high_resolution_clock::now();
    sf::Clock clock;

    while (window->isOpen()) {
        const float deltaTime = updateTime(&startTime);
        handleEvent(window, controller);
        update(controller, deltaTime);
        controller->getRenderSystem()->render();
    }
}

int main() {
	GameState state = GameState::Menu;

	WindowManager windowManager;
	windowManager.createWindow();
	sf::RenderWindow* window = windowManager.getWindow();

	EntityManager entityManager;
	RenderSystem renderSystem(window, &entityManager);
	InputSystem inputSystem;
	BattleContext battleContext;
	Controller controller(&entityManager, &inputSystem, &renderSystem, &state, &battleContext);

	initMenu(&entityManager, &renderSystem);

	gameLoop(window, &controller);

	return 0;
}
