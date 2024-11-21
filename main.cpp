#include <cmath>
#include "./systems/WindowManager.h"
#include "./systems/ClickHandler.h"
#include "./systems/EntityManager.h"
#include "./systems/RenderSystem.h"
#include "./systems/InputSystem.h"
#include "./Player/PlayerMovementSystem.h"
#include "./Inventory/InventorySystem.h"
#include "./Pokemon/PokemonSystem.h"
#include "GameState.h"
#include "Entity.h"
#include "Initialize.h"
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

void update(
		EntityManager* entityManager, 
		InputSystem* inputSystem, 
		RenderSystem* renderSystem, 
		GameState* state, 
		float deltaTime,
		sf::RenderWindow* window
	) {
	// TODO: Вынести в объект аргументы
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
				inputSystem->clear();
			}
		break;
	}

	case GameState::Game:
		if (keys.empty()) return;
		playerMovementSystem(entityManager->getEntitiesWithComponent<PlayerControlComponent>().back(), keys, deltaTime);
		if (std::find(keys.begin(), keys.end(), sf::Keyboard::Escape) != keys.end()) {
			*state = GameState::Menu;
			renderSystem->removeEntities();
			renderSystem->addEntities(entityManager->getEntitiesWithComponent<MenuTypeEntityComponent>());
			inputSystem->clear();
		}
		// Открытие инвентаря
		if (std::find(keys.begin(), keys.end(), sf::Keyboard::E) != keys.end()) {
			*state = GameState::Inventory;
			createInventory(entityManager);
			renderSystem->addEntities(entityManager->getEntitiesWithComponent<InventoryTypeEntityComponent>());
			auto pokemons = entityManager->getEntitiesWithComponent<PokemonComponent>();
			for (auto pokemon : pokemons) {
				if (pokemon->getComponent<PokemonComponent>()->isCollected()) {
					renderSystem->addEntity(pokemon);
				}
			}
			inputSystem->clear();
		}
		pokemonCollision(entityManager, renderSystem);
	break;

	case GameState::Inventory: {
		closeInventory(
			entityManager, 
			inputSystem,
			renderSystem,
			state
		);
		break;
	}
	
	default:
		break;
	}
	
}

void gameLoop(
		sf::RenderWindow* window, 
		EntityManager* entityManager, 
		RenderSystem* renderSystem, 
		InputSystem* inputSystem, 
		GameState* state
	) {
    auto startTime = std::chrono::high_resolution_clock::now();
    sf::Clock clock;

    while (window->isOpen()) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 1000.0f;
        startTime = currentTime;

        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
            inputSystem->handleEvent(event);
        }

        update(entityManager, inputSystem, renderSystem, state, deltaTime, window);

        renderSystem->render();
    }
}

int main() {
	GameState state = GameState::Menu;
	WindowManager windowManager;
	windowManager.createWindow();
	sf::RenderWindow* window = windowManager.getWindow();
	EntityManager entityManager;
	RenderSystem renderSystem(window, &entityManager);

	init(window, &entityManager);
	renderSystem.addEntities(entityManager.getEntitiesWithComponent<MenuTypeEntityComponent>());

	InputSystem inputSystem;

	gameLoop(
		window,
		&entityManager, 
		&renderSystem, 
		&inputSystem, 
		&state
	);

	return 0;
}
