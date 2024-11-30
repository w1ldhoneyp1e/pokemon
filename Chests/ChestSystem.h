#pragma once
#include "../Components.h"
#include "../systems/EntityManager.h"
#include "../systems/InputSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/ClickHandler.h"
#include "../systems/CollisionSystem.h"
#include "../systems/KeyboardSystem.h"
#include "../const.h"
#include "../GameState.h"
#include <ctime>

void generateRandomItem(EntityManager *entityManager, int index);
void generateChestContent(EntityManager *entityManager);
Entity* createHealingPotion(EntityManager *entityManager);
Entity* createCoin(EntityManager *entityManager);
void initChestContent(EntityManager *entityManager, RenderSystem* renderSystem);
void initChestButtonCollect(EntityManager *entityManager, RenderSystem* renderSystem);
void initChestButtonClose(EntityManager *entityManager, RenderSystem* renderSystem);
void initChestInterface(EntityManager *entityManager, RenderSystem* renderSystem);

void chestOpening(
	InputSystem* inputSystem,
	EntityManager* entityManager,
	RenderSystem* renderSystem,
	GameState* state
);
void closeChest(
	InputSystem* inputSystem,
	EntityManager* entityManager,
	RenderSystem* renderSystem,
	GameState* state
);

bool doesConditionSatisfy(Entity *chest, Entity *player, std::vector<sf::Keyboard::Key> keys);
bool isEnterPressed(std::vector<sf::Keyboard::Key> keys);

void updateChests(
	InputSystem* inputSystem,
	EntityManager* entityManager,
	RenderSystem* renderSystem,
	GameState* state
) {
	closeChest(
	inputSystem,
	entityManager,
	renderSystem,
	state);
}

void chestInit(
	EntityManager* entityManager,
    RenderSystem* renderSystem
) {
	generateChestContent(entityManager);
	initChestContent(entityManager, renderSystem);
	// initChestButtonCollect(entityManager, renderSystem);
	initChestButtonClose(entityManager, renderSystem);
	initChestInterface(entityManager, renderSystem);
	auto entities = entityManager->getEntitiesWithComponent<ChestContentComponent>();
	auto interface = entityManager->getEntitiesWithComponent<ChestInterfaceComponent>();
	renderSystem->addEntities(entities);
	renderSystem->addEntities(interface);
}

void chestCollisionHandler(
	EntityManager* entityManager,
    InputSystem* inputSystem,
    RenderSystem* renderSystem,
    GameState* state
) {

}

void generateChestContent(EntityManager *entityManager) {
	std::srand(std::time(0));
    int amountOfItems = 1 + std::rand() % 7;
	for (int i=1; i <= amountOfItems; ++i) {
		generateRandomItem(entityManager, i);
	}
	// Сгенерить число до 7
	// В цикле создавать сущность рандомно, добавлять в стор
}

void generateRandomItem(EntityManager *entityManager, int index) {
	std::srand(std::time(0));
    int typeIdOfItem = std::rand() % 2; // 0 => Healing Potion, 1 => Coin
	switch (typeIdOfItem)
	{
	case 0:{
		auto healingPotion = createHealingPotion(entityManager);
		healingPotion->addComponent<PositionComponent>(
			CHEST_INTERFACE_X + 12 + (index % MAX_ITEMS_PER_ROW - 1) * 51,
			CHEST_INTERFACE_Y + 12 + index / MAX_ITEMS_PER_ROW * 51
		);
		break;
	}

	case 1: {
		auto coin = createCoin(entityManager);
		coin->addComponent<PositionComponent>(
			CHEST_INTERFACE_X + 12 + (index % MAX_ITEMS_PER_ROW - 1) * 51,
			CHEST_INTERFACE_Y + 12 + index / MAX_ITEMS_PER_ROW * 51
		);
		break;
	}
	
	default:
		break;
	}
}

Entity* createHealingPotion(EntityManager *entityManager) {
	auto healingPotion = entityManager->createEntity();
	healingPotion->addComponent<ItemComponent>();
	healingPotion->addComponent<RenderLayerComponent>(2);
	healingPotion->addComponent<SizeComponent>(
		ITEM_SIDE,
		ITEM_SIDE
	);
	healingPotion->addComponent<ChestContentComponent>();
	sf::Texture healingPotionTexture;
    if (healingPotionTexture.loadFromFile("../res/healingPotion(500x500).png")) {
        healingPotion->addComponent<TextureComponent>(
			healingPotionTexture,
			500, 
			500
		);
    }
	return healingPotion;
}

Entity* createCoin(EntityManager *entityManager) {
	auto coin = entityManager->createEntity();
	coin->addComponent<ItemComponent>();
	coin->addComponent<RenderLayerComponent>(2);
	coin->addComponent<SizeComponent>(
		ITEM_SIDE,
		ITEM_SIDE
	);
	coin->addComponent<ChestContentComponent>();
	sf::Texture coinTexture;
    if (coinTexture.loadFromFile("../res/Coin(12x12).png")) {
        coin->addComponent<TextureComponent>(
			coinTexture,
			12, 
			12
		);
    }
	return coin;
}

void initChestContent(EntityManager *entityManager, RenderSystem* renderSystem) {
	// Сгенерированные сущности добавить в renderSystem
}

void initChestButtonCollect(EntityManager *entityManager, RenderSystem* renderSystem) {

}
void initChestButtonClose(EntityManager *entityManager, RenderSystem* renderSystem) {
	auto button = entityManager->createEntity();
	button->addComponent<ChestButtonCloseComponent>();
	button->addComponent<PositionComponent>(
			CHEST_INTERFACE_BUTTON_CLOSE_X,
			CHEST_INTERFACE_BUTTON_CLOSE_Y
		);
	button->addComponent<RenderLayerComponent>(2);
	button->addComponent<SizeComponent>(
		CHEST_INTERFACE_BUTTON_CLOSE_WIDTH,
		CHEST_INTERFACE_BUTTON_CLOSE_HEIGHT
	);
	button->addComponent<ChestInterfaceComponent>();
	sf::Texture buttonTexture;
    if (buttonTexture.loadFromFile("../res/backButton(32x13).png")) {
        button->addComponent<TextureComponent>(
			buttonTexture,
			32, 
			13
		);
    }
}
void initChestInterface(EntityManager *entityManager, RenderSystem* renderSystem) {
	auto inventoryInterface = entityManager->createEntity();
	inventoryInterface->addComponent<ItemComponent>();
	inventoryInterface->addComponent<RenderLayerComponent>(1);
	inventoryInterface->addComponent<PositionComponent>(
		CHEST_INTERFACE_X,
		CHEST_INTERFACE_Y
	);
	inventoryInterface->addComponent<SizeComponent>(
		CHEST_INTERFACE_WIDTH,
		CHEST_INTERFACE_HEIGHT
	);
	inventoryInterface->addComponent<ChestInterfaceComponent>();
	sf::Texture inventoryInterfaceTexture;
    if (inventoryInterfaceTexture.loadFromFile("../res/chestInterface(230x79).png")) {
        inventoryInterface->addComponent<TextureComponent>(
			inventoryInterfaceTexture,
			CHEST_INTERFACE_TEXTURE_WIDTH, 
			CHEST_INTERFACE_TEXTURE_HEIGHT
		);
    }
}

void chestOpening(
	InputSystem* inputSystem,
	EntityManager* entityManager,
	RenderSystem* renderSystem,
	GameState* state
) {
	auto player = entityManager->getEntitiesWithComponent<PlayerControlComponent>()[0];
	auto chests = entityManager->getEntitiesWithComponent<ChestComponent>();
	auto keys = inputSystem->getPressedKeys();
	for (auto chest : chests) {
		if (doesConditionSatisfy(chest, player, keys)) {
			*state = GameState::Chest;
			inputSystem->clear();
			chestInit(entityManager, renderSystem);
		}
	}
}

void closeChest(
	InputSystem* inputSystem,
	EntityManager* entityManager,
	RenderSystem* renderSystem,
	GameState* state
) {
	auto button = entityManager->getEntitiesWithComponent<ChestButtonCloseComponent>()[0];
	if (
		!isEscapePressed(inputSystem->getPressedKeys())
		&& !isClickOnEntity(inputSystem->getMouseClick(), button)
	) return;
	inputSystem->clear();
	auto entities = entityManager->getEntitiesWithComponent<ChestContentComponent>();
	auto interface = entityManager->getEntitiesWithComponent<ChestInterfaceComponent>();
	for(auto entity : entities) {
		renderSystem->removeEntity(entity->getId());
		entityManager->removeEntity(entity);
	} 
	for(auto interfaceComp : interface) {
		renderSystem->removeEntity(interfaceComp->getId());
		entityManager->removeEntity(interfaceComp);
	}
	*state = GameState::Game;
}

bool doesConditionSatisfy(Entity *chest, Entity *player, std::vector<sf::Keyboard::Key> keys) {
	return isEnterPressed(keys)
	&& isCollision(chest, player) ;
}