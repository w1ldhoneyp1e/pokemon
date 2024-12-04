#pragma once
#include "../systems/Controller.h"
#include "../systems/ClickHandler.h"
#include "../systems/RenderSystem.h"
#include "../systems/InputSystem.h"
#include "../systems/EntityManager.h"
#include "../GameState.h"
#include "../const.h"
#include <SFML/Graphics.hpp>

void createInventory(EntityManager* em);

void openInventory(Controller* controller) {
	auto [input, em, render, state] = controller->getAll();
	auto keys = input->getPressedKeys();

	if ((std::find(keys.begin(), keys.end(), sf::Keyboard::E) == keys.end())) return;
	*state = GameState::Inventory;
	createInventory(em);
	render->addEntities(em->getEntitiesWithComponent<InventoryTypeEntityComponent>());
	auto pokemons = em->getEntitiesWithComponent<PokemonComponent>();
	for (auto pokemon : pokemons) {
		if (!pokemon->getComponent<PokemonComponent>()->isCollected()) continue;
		render->addEntity(pokemon);
	}
	input->clear();
}

void createInventory(EntityManager* em) {
	// Инвентарь
	auto inventory = em->createEntity();
	inventory->addComponent<SizeComponent>(
		INVENTORY_WIDTH * 5, 
		INVENTORY_HEIGHT * 4
	);
	inventory->addComponent<PositionComponent>(
		INVENTORY_POSITION_X, 
		INVENTORY_POSITION_Y
	);
	inventory->addComponent<RenderLayerComponent>(3);
    inventory->addComponent<InventoryTypeEntityComponent>();
	sf::Texture inventoryTexture;
	if (inventoryTexture.loadFromFile("../res/inventory(80x102).png")) {
		inventory->addComponent<TextureComponent>(
			inventoryTexture, 
			INVENTORY_WIDTH, 
			INVENTORY_HEIGHT
		);
	}

	// Кнопка зарытия
	auto inventoryButtonClose = em->createEntity();
	std::cout << "ID: " << inventoryButtonClose->getId() << std::endl;
	inventoryButtonClose->addComponent<InventoryButtonCloseComponent>();
	inventoryButtonClose->addComponent<SizeComponent>(
		INVENTORY_BUTTON_CLOSE_WIDTH * 4, 
		INVENTORY_BUTTON_CLOSE_HEIGHT * 4
	);
	inventoryButtonClose->addComponent<PositionComponent>(
		WINDOW_WIDTH / 2 - INVENTORY_BUTTON_CLOSE_WIDTH * 6, 
		WINDOW_HEIGHT / 2 + INVENTORY_BUTTON_CLOSE_HEIGHT * 11
	);
	inventoryButtonClose->addComponent<RenderLayerComponent>(4);
    inventoryButtonClose->addComponent<InventoryTypeEntityComponent>();
	sf::Texture inventoryButtonCloseTexture;
	if (inventoryButtonCloseTexture.loadFromFile("../res/backButton(32x13).png")) {
		inventoryButtonClose->addComponent<TextureComponent>(
			inventoryButtonCloseTexture, 
			INVENTORY_BUTTON_CLOSE_WIDTH, 
			INVENTORY_BUTTON_CLOSE_HEIGHT
		);
	}

	// Покемоны в инвентаре
	auto player = em->getEntitiesWithComponent<PlayerControlComponent>()[0];
	auto ids = player->getComponent<PlayersInventoryComponent>()->getPokemons();
	int i = 0;
	int j = 0;
	for (auto id : ids) {
		auto pokemon = em->getEntity(id);
		pokemon->getComponent<PositionComponent>()->setPos(
			INVENTORY_CELLS_POSITION_START_X + i * 40,
			INVENTORY_CELLS_POSITION_START_Y + j * 40
		);
		pokemon->getComponent<SizeComponent>()->setSize(
			POKEMON_INVENTORY_WIDTH,
			POKEMON_INVENTORY_HEIGHT
		);
		pokemon->getComponent<RenderLayerComponent>()->setLayer(4);
		i++;
		if (i / INVENTORY_CELLS_PER_ROW) {
			j++;
			i = 0;
		} 
	}
}

void closeInventory(Controller* controller) {
	auto [input, em, render, state] = controller->getAll();

	auto keys = input->getPressedKeys();
	auto button = em->getEntitiesWithComponent<InventoryButtonCloseComponent>()[0];
	if (
		input->hasMouseClick() 
		&& isClickOnEntity(input->getMouseClick(), button)
		|| !keys.empty()
		&& (
			std::find(keys.begin(), keys.end(), sf::Keyboard::Escape) != keys.end()
			|| std::find(keys.begin(), keys.end(), sf::Keyboard::E) != keys.end()
		)
	) {
		// Удаляем элементы инвентаря с экрана
		*state = GameState::Game;
		auto menuEntities = em->getEntitiesWithComponent<InventoryTypeEntityComponent>();
		for (Entity* entity : menuEntities) {
			render->removeEntity(entity->getId());
		}

		// Удаляем покемонов с экрана
		auto pokemons = em->getEntitiesWithComponent<PokemonComponent>();
		for (auto pokemon : pokemons) {
			if (pokemon->getComponent<PokemonComponent>()->isCollected()) {
				render->removeEntity(pokemon->getId());
			}
		}

		input->clear();
	}
}