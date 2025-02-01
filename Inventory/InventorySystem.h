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
	auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();
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
		INVENTORY_WIDTH, 
		INVENTORY_HEIGHT
	);
	inventory->addComponent<PositionComponent>(
		INVENTORY_POSITION_X,
		INVENTORY_POSITION_Y
	);
	inventory->addComponent<RenderLayerComponent>(3);
    inventory->addComponent<InventoryTypeEntityComponent>();
	sf::Texture inventoryTexture;
	if (inventoryTexture.loadFromFile("../res/inventory(80x156).png")) {
		inventory->addComponent<TextureComponent>(
			inventoryTexture, 
			80, 
			156
		);
	}

	// Кнопка зарытия
	auto inventoryButtonClose = em->createEntity();
	inventoryButtonClose->addComponent<InventoryButtonCloseComponent>();
	inventoryButtonClose->addComponent<SizeComponent>(
		INVENTORY_BUTTON_CLOSE_WIDTH, 
		INVENTORY_BUTTON_CLOSE_HEIGHT
	);
	inventoryButtonClose->addComponent<PositionComponent>(
		INVENTORY_BUTTON_CLOSE_X, 
		INVENTORY_BUTTON_CLOSE_Y
	);
	inventoryButtonClose->addComponent<RenderLayerComponent>(4);
    inventoryButtonClose->addComponent<InventoryTypeEntityComponent>();
	sf::Texture inventoryButtonCloseTexture;
	if (inventoryButtonCloseTexture.loadFromFile("../res/backButton(32x13).png")) {
		inventoryButtonClose->addComponent<TextureComponent>(
			inventoryButtonCloseTexture, 
			32, 
			13
		);
	}

	auto player = em->getEntitiesWithComponent<PlayerControlComponent>()[0];
	auto playerInventory = player->getComponent<PlayersInventoryComponent>();

	auto coinCounter = em->createEntity();
	coinCounter->addComponent<TextComponent>(
		std::to_string(playerInventory->getCoinCount()),
		INVENTORY_ICON_X + 30,
		INVENTORY_ICON_Y + 1,
		52,
		sf::Color(68, 68, 68)

	);
	coinCounter->addComponent<InventoryTypeEntityComponent>();
	coinCounter->addComponent<RenderLayerComponent>(4);

	auto coinIcon = em->createEntity();
	coinIcon->addComponent<InventoryTypeEntityComponent>();
	coinIcon->addComponent<RenderLayerComponent>(4);
	coinIcon->addComponent<SizeComponent>(16, 16);
	coinIcon->addComponent<PositionComponent>(
		INVENTORY_ICON_X,
		INVENTORY_ICON_Y
	);
	sf::Texture coinIconTexture;

	if (coinIconTexture.loadFromFile("../res/Coin(12x12).png")) {
		coinIcon->addComponent<TextureComponent>(coinIconTexture, 12, 12);
	}

	auto potionCounter = em->createEntity();
	potionCounter->addComponent<TextComponent>(
		std::to_string(playerInventory->getPotionCount()),
		INVENTORY_ICON_X + 30,
		INVENTORY_ICON_Y + 21,
		52,
		sf::Color(68, 68, 68)

	);
	potionCounter->addComponent<InventoryTypeEntityComponent>();
	potionCounter->addComponent<RenderLayerComponent>(4);

	auto potionIcon = em->createEntity();
	potionIcon->addComponent<InventoryTypeEntityComponent>();
	potionIcon->addComponent<RenderLayerComponent>(4);
	potionIcon->addComponent<SizeComponent>(16, 16);
	potionIcon->addComponent<PositionComponent>(
		INVENTORY_ICON_X,
		INVENTORY_ICON_Y + 20
	);
	sf::Texture potionIconTexture;

	if (potionIconTexture.loadFromFile("../res/healingPotion(12x12).png")) {
		potionIcon->addComponent<TextureComponent>(potionIconTexture, 12, 12);
	}

	auto pokeballCounter = em->createEntity();
	pokeballCounter->addComponent<TextComponent>(
		std::to_string(playerInventory->getPokeballCount()),
		INVENTORY_ICON_X + 30,
		INVENTORY_ICON_Y + 42,
		52,
		sf::Color(68, 68, 68)

	);
	pokeballCounter->addComponent<InventoryTypeEntityComponent>();
	pokeballCounter->addComponent<RenderLayerComponent>(4);

	auto pokeballIcon = em->createEntity();
	pokeballIcon->addComponent<InventoryTypeEntityComponent>();
	pokeballIcon->addComponent<RenderLayerComponent>(4);
	pokeballIcon->addComponent<SizeComponent>(16, 16);
	pokeballIcon->addComponent<PositionComponent>(
		INVENTORY_ICON_X,
		INVENTORY_ICON_Y + 40
	);
	sf::Texture pokeballIconTexture;

	if (pokeballIconTexture.loadFromFile("../res/pokeball(12x12).png")) {
		pokeballIcon->addComponent<TextureComponent>(pokeballIconTexture, 12, 12);
	}

	auto pokemons = em->getEntitiesWithComponent<PokemonComponent>();
	auto ids = playerInventory->getPokemons();
	int i = 0;
	int j = 0;
	for (auto pokemon : pokemons) {
		if (!pokemon->getComponent<PokemonComponent>()->isCollected()) continue;
		pokemon->getComponent<PositionComponent>()->setPos(
			INVENTORY_CELLS_POSITION_START_X + i * 7 * SCALE,
			INVENTORY_CELLS_POSITION_START_Y + j * 7 * SCALE
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
	auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();

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
		*state = GameState::Game;
		auto menuEntities = em->getEntitiesWithComponent<InventoryTypeEntityComponent>();
		for (Entity* entity : menuEntities) {
			render->removeEntity(entity->getId());
			em->removeEntity(entity);
		}

		auto pokemons = em->getEntitiesWithComponent<PokemonComponent>();
		for (auto pokemon : pokemons) {
			if (pokemon->getComponent<PokemonComponent>()->isCollected()) {
				render->removeEntity(pokemon->getId());
			}
		}

		input->clear();
	}
}