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
#include "./Location/LocationType.h"
#include "GameState.h"
#include "TheEnd/TheEnd.h"
#include "Entity.h"
#include <iostream>
#include <windows.h>
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

void update(Controller* controller, float deltaTime, sf::RenderWindow* window) {
	auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();
	auto keys = input->getPressedKeys();
	bool isClicked = false;
	switch (*state)
	{
	case GameState::Menu: {
		onStartGameButtonPress(controller);
		break;
	}

	case GameState::Game:
		playerMovementSystem(controller, deltaTime, window);
		pokemonGenerating(controller, deltaTime);
		chestsGenerating(controller, deltaTime);
		if (keys.empty()) return;
		backToMenu(controller);
		openInventory(controller);
		pokemonCollision(controller);
		chestOpening(controller);
		trainerCollision(controller);
		shopCollision(controller);
	break;

	case GameState::Inventory: {
		closeInventory(controller);
		onPokemonSellButtonClick(controller);
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

	case GameState::Shop: {
		handleShopInput(controller, deltaTime);
		break;
	}

	case GameState::TheEnd: {
		onGameEnd(controller, deltaTime);
		break;
	}

	default:
		break;
	}
	
}

void gameLoop(sf::RenderWindow* window, Controller* controller) {
    auto startTime = std::chrono::high_resolution_clock::now();

    while (window->isOpen()) {
        const float deltaTime = updateTime(&startTime);
        handleEvent(window, controller);
        update(controller, deltaTime, window);
        controller->getRenderSystem()->render();
    }
}

int main() {
	setlocale(LC_ALL, "RU");
	std::srand(std::time(0));
	GameState state = GameState::Menu;

	WindowManager windowManager;
	windowManager.createWindow();
	sf::RenderWindow* window = windowManager.getWindow();
	HWND hwnd = window->getSystemHandle();

    RECT rect;
    GetWindowRect(hwnd, &rect);

	SCREEN_WIDTH = window->getSize().x;
	SCREEN_HEIGHT = window->getSize().y;

	EntityManager entityManager;
	RenderSystem renderSystem(window, &entityManager);
	InputSystem inputSystem;
	BattleContext battleContext;
	std::unordered_map<LocationType, CollisionMap> collisionMaps;

	sf::Vector2u baseSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	float scaleX = SCREEN_WIDTH / baseSize.x;
	float scaleY = SCREEN_HEIGHT / baseSize.y;
	SCALE = std::min(scaleX, scaleY);

	collisionMaps.emplace(LocationType::Town, CollisionMap("../res/collisionMap/town.txt", WINDOW_WIDTH / 76, WINDOW_HEIGHT / 64));
	collisionMaps.emplace(LocationType::Forest, CollisionMap("../res/collisionMap/forest.txt", WINDOW_WIDTH / 38, WINDOW_HEIGHT / 32));
	Controller controller(&entityManager, &inputSystem, &renderSystem, &state, &battleContext, &collisionMaps, LocationType::Town);

	initMenu(&entityManager, &renderSystem);

	gameLoop(window, &controller);

	return 0;
}
