#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "./systems/WindowManager.h"
#include "./systems/EntityManager.h"
#include "./systems/EntityManager.h"
#include "./systems/RenderSystem.h"
#include "./systems/InputSystem.h"
#include "./Player/PlayerMovementSystem.h"
#include "Entity.h"
#include <iostream>

enum struct GameState {
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

void update(EntityManager* entityManager, InputSystem* inputSystem) {
	auto keys = inputSystem->getPressedKeys();
	if (keys.empty()) return;
	playerMovementSystem(entityManager->getEntitiesWithComponent<PlayerControlComponent>().back(), keys.back());
}

void init() {
}

int main() {
	GameState state = GameState::Menu;
	WindowManager windowManager;
	windowManager.createWindow();
	sf::RenderWindow* window = windowManager.getWindow();
	EntityManager entityManager;
	RenderSystem renderSystem(window, &entityManager);

	auto player = entityManager.createEntity();
	player->addComponent<PositionComponent>(300, 300);
	player->addComponent<SizeComponent>(48, 96);
	player->addComponent<SpeedComponent>(0.25);
	player->addComponent<PlayerControlComponent>(true);
	sf::Texture playerTexture;
    if (playerTexture.loadFromFile("../res/Ash_sprite(16x32).png")) {
        player->addComponent<TextureComponent>(playerTexture, 16, 32);
    }

	auto startButton = entityManager.createEntity();
	startButton->addComponent<PositionComponent>(400-108, 200-40);
	startButton->addComponent<SizeComponent>(216, 80);
	sf::Texture buttonTexture;
	if (buttonTexture.loadFromFile("../res/button(54x20).png")) {
		startButton->addComponent<TextureComponent>(buttonTexture, 54, 20);
	}
	Entity* button = startButton;
	renderSystem.addEntity(startButton);

	InputSystem inputSystem;

	renderSystem.addEntity(player);

	while (window->isOpen()) {
		pollEvents(window, &inputSystem);
		update(&entityManager, &inputSystem);
		renderSystem.render();
	}
	return 0;
}
