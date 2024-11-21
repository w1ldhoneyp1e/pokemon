#pragma once
#include "../systems/ClickHandler.h"
#include "../systems/RenderSystem.h"
#include "../systems/InputSystem.h"
#include "../systems/EntityManager.h"
#include "../GameState.h"
#include "../const.h"
#include <SFML/Graphics.hpp>

void createInventory(EntityManager* entityManager) {

	// Инвентарь
	auto inventory = entityManager->createEntity("inventory");
	inventory->addComponent<SizeComponent>(
		INVENTORY_WIDTH * 5, 
		INVENTORY_HEIGHT * 4
	);
	inventory->addComponent<PositionComponent>(
		WINDOW_WIDTH / 2 - INVENTORY_WIDTH * 2.5, 
		WINDOW_HEIGHT / 2 - INVENTORY_HEIGHT * 2
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
	auto inventoryButtonClose = entityManager->createEntity("inventoryButtonClose");
	inventoryButtonClose->addComponent<SizeComponent>(
		INVENTORY_BUTTON_CLOSE_WIDTH * 4, 
		INVENTORY_BUTTON_CLOSE_HEIGHT * 4
	);
	inventoryButtonClose->addComponent<PositionComponent>(
		WINDOW_WIDTH / 2 - INVENTORY_BUTTON_CLOSE_WIDTH * 6, 
		WINDOW_HEIGHT / 2 + INVENTORY_BUTTON_CLOSE_HEIGHT * 11
	);
	inventoryButtonClose->addComponent<RenderLayerComponent>(3);
    inventoryButtonClose->addComponent<InventoryTypeEntityComponent>();
	sf::Texture inventoryButtonCloseTexture;
	if (inventoryButtonCloseTexture.loadFromFile("../res/backButton(32x13).png")) {
		inventoryButtonClose->addComponent<TextureComponent>(
			inventoryButtonCloseTexture, 
			INVENTORY_BUTTON_CLOSE_WIDTH, 
			INVENTORY_BUTTON_CLOSE_HEIGHT
		);
	}
}

void closeInventory(
		EntityManager* entityManager, 
		InputSystem* inputSystem,
		RenderSystem* renderSystem,
		GameState* state
	) {
	auto keys = inputSystem->getPressedKeys();
	auto button = entityManager->getEntity("inventoryButtonClose");
	if (
		inputSystem->hasMouseClick() && isClickOnEntity(inputSystem->getMouseClick(), button)
		|| !keys.empty() && std::find(keys.begin(), keys.end(), sf::Keyboard::Escape) != keys.end()
	) {
		*state = GameState::Game;
		auto menuEntities = entityManager->getEntitiesWithComponent<InventoryTypeEntityComponent>();
		for (Entity* entity : menuEntities) {
			renderSystem->removeEntity(entity->getId());
		}
		inputSystem->clear();
	}
}