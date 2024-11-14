#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "./systems/WindowManager.h"
#include "./systems/ClickHandler.h"
#include "./systems/EntityManager.h"
#include "./systems/EntityManager.h"
#include "./systems/RenderSystem.h"
#include "./systems/InputSystem.h"
#include "./Player/PlayerMovementSystem.h"
#include "Entity.h"
#include <iostream>

enum GameState {
    Menu,
    Game,
};

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
				renderSystem->addEntity(entityManager->getEntity("player"));
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

void init(std::unordered_map<GameState, std::vector<Entity*>>* entities, EntityManager* entityManager) {
	auto player = entityManager->createEntity("player");
	player->addComponent<PositionComponent>(300, 300);
	player->addComponent<SizeComponent>(48, 96);
	player->addComponent<SpeedComponent>(0.25);
	player->addComponent<PlayerControlComponent>(true);
	sf::Texture playerTexture;
    if (playerTexture.loadFromFile("../res/Ash_sprite(16x32).png")) {
        player->addComponent<TextureComponent>(playerTexture, 16, 32);
    }
	std::vector<Entity*> gameEntities;
	gameEntities.push_back(player);

	auto startButton = entityManager->createEntity("startGame");
	startButton->addComponent<PositionComponent>(400-108, 200-40);
	startButton->addComponent<SizeComponent>(216, 80);
	sf::Texture buttonTexture;
	if (buttonTexture.loadFromFile("../res/button(54x20).png")) {
		startButton->addComponent<TextureComponent>(buttonTexture, 54, 20);
	}
	Entity* button = startButton;
	std::vector<Entity*> menuEntities;
	menuEntities.push_back(button);

	entities->insert({GameState::Menu, menuEntities});
	entities->insert({GameState::Game, menuEntities});
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

	renderSystem.addEntities(&allEntities[GameState::Menu]);

	while (window->isOpen()) {
		pollEvents(window, &inputSystem);
		update(&entityManager, &inputSystem, &renderSystem, &state);
		renderSystem.render();
	}
	return 0;
}
