#include "./systems/EntityManager.h"
#include <SFML/Graphics.hpp>
#include "Entity.h"

enum GameState {
    Menu,
    Game,
};

void init(std::unordered_map<GameState, std::vector<Entity*>>* entities, EntityManager* entityManager) {
    // Игрок
	auto player = entityManager->createEntity("player");
	player->addComponent<PositionComponent>(300, 300);
	player->addComponent<SizeComponent>(16, 32);
	player->addComponent<SpeedComponent>(0.25);
    player->addComponent<GameTypeEntityComponent>();
	player->addComponent<PlayerControlComponent>(true);
	sf::Texture playerTexture;
    if (playerTexture.loadFromFile("../res/Ash_sprite(16x32).png")) {
        player->addComponent<TextureComponent>(playerTexture, 16, 32);
    }
	std::vector<Entity*> gameEntities;
	gameEntities.push_back(player);

    // Локация
    auto location = entityManager->createEntity("location");
    location->addComponent<PositionComponent>(0, 0);
    location->addComponent<SizeComponent>(767, 639);
    location->addComponent<GameTypeEntityComponent>();
    sf::Texture locationTexture;
    if (locationTexture.loadFromFile("../res/location(767x639).png")) {
        location->addComponent<TextureComponent>(locationTexture, 767, 639);
    }
    gameEntities.push_back(location);

    // Кнопка начала игры
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
	entities->insert({GameState::Game, gameEntities});
}