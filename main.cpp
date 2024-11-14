#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "./systems/WindowManager.h"
#include "./systems/EntityManager.h"
#include "./systems/RenderSystem.h"
#include "./systems/InputSystem.h"
#include "./Player/PlayerMovementSystem.h"
#include "Entity.h"
#include <iostream>

void pollEvents(sf::RenderWindow* window, InputSystem* inputSystem) {
	sf::Event event;
	while (window->pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window->close();
		inputSystem->handleEvent(event);
	}
}

void update(Entity* player, InputSystem inputSystem) {
	auto keys = inputSystem.getPressedKeys();
	if (keys.size()) {
        playerMovementSystem(player, keys.back());
    }
}

void init() {
}

int main() {
	sf::Clock clock;
	WindowManager windowManager;
	windowManager.createWindow();
	sf::RenderWindow* window = windowManager.getWindow();
	EntityManager entityManager;
	RenderSystem renderSystem(window, &entityManager);

	auto player = entityManager.createEntity();
	player->addComponent<PositionComponent>(300, 300);
	player->addComponent<SizeComponent>(48, 96);
	player->addComponent<SpeedComponent>(0.25);
	sf::Texture texture;
    if (texture.loadFromFile("../res/Ash_sprite(16x32).png")) {
        player->addComponent<TextureComponent>(texture, 16, 32);
    }

	InputSystem inputSystem;

	renderSystem.addEntity(player);

	while (window->isOpen()) {
		pollEvents(window, &inputSystem);
		sf::Time elapsed = clock.restart();
		float dt = elapsed.asSeconds();
		update(player, inputSystem);
		renderSystem.render();
	}
	return 0;
}
