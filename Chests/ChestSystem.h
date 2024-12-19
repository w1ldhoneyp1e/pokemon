#pragma once
#include "../Components/Components.h"
#include "../systems/EntityManager.h"
#include "../systems/InputSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/ClickHandler.h"
#include "../systems/CollisionSystem.h"
#include "../systems/KeyboardSystem.h"
#include "../const.h"
#include "../GameState.h"
#include <ctime>

void generateRandomItem(EntityManager *em, int index, int chestId);
void generateChestContent(EntityManager *em, int chestId);
Entity* createHealingPotion(EntityManager *em);
Entity* createCoin(EntityManager *em);
void initChestContent(EntityManager *em, RenderSystem* render);
void initChestButtonCollect(EntityManager *em);
void initChestButtonClose(EntityManager *em);
void initChestInterface(EntityManager *em, RenderSystem* render);

void chestOpening(Controller* controller);
void collectChest(Controller* controller);
void closeChest(Controller* controller);

bool doesChestConditionSatisfy(Entity *chest, Entity *player, std::vector<sf::Keyboard::Key> keys);
bool isEnterPressed(std::vector<sf::Keyboard::Key> keys);

void updateChests(Controller* controller) {
	closeChest(controller);
	collectChest(controller);
}

void chestInit(
	EntityManager* em,
    RenderSystem* render,
	int chestId
) {
	generateChestContent(em, chestId);
	initChestContent(em, render);
	initChestButtonCollect(em);
	initChestButtonClose(em);
	initChestInterface(em, render);
	auto entities = em->getEntitiesWithComponent<ChestContentComponent>();
	auto interface = em->getEntitiesWithComponent<ChestInterfaceComponent>();
	render->addEntities(entities);
	render->addEntities(interface);
}

void chestCollisionHandler(Controller* controller) {

}

void generateChestContent(EntityManager *em, int chestId) {
	std::srand(std::time(0));
    int amountOfItems = 1 + std::rand() % 7;
	for (int i=1; i <= amountOfItems; ++i) {
		generateRandomItem(em, i, chestId);
	}
	// Сгенерить число до 7
	// В цикле создавать сущность рандомно, добавлять в стор
}

void generateRandomItem(EntityManager *em, int index, int chestId) {
	std::srand(std::time(0));
    int typeIdOfItem = std::rand() % 2; // 0 => Healing Potion, 1 => Coin
	switch (typeIdOfItem)
	{
	case 0:{
		auto healingPotion = createHealingPotion(em);
		healingPotion->addComponent<ChestContentComponent>(chestId);
		healingPotion->addComponent<PositionComponent>(
			CHEST_INTERFACE_X + 12 + (index % MAX_ITEMS_PER_ROW - 1) * 51,
			CHEST_INTERFACE_Y + 12 + index / MAX_ITEMS_PER_ROW * 51
		);
		break;
	}

	case 1: {
		auto coin = createCoin(em);
		coin->addComponent<ChestContentComponent>(chestId);
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

Entity* createHealingPotion(EntityManager *em) {
	auto healingPotion = em->createEntity();
	healingPotion->addComponent<RenderLayerComponent>(2);
	healingPotion->addComponent<SizeComponent>(
		ITEM_SIDE,
		ITEM_SIDE
	);
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

Entity* createCoin(EntityManager *em) {
	auto coin = em->createEntity();
	coin->addComponent<ItemComponent>();
	coin->addComponent<RenderLayerComponent>(2);
	coin->addComponent<SizeComponent>(
		ITEM_SIDE,
		ITEM_SIDE
	);
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

void initChestContent(EntityManager *em, RenderSystem* render) {
	// Сгенерированные сущности добавить в render
}

void initChestButtonCollect(EntityManager *em) {
	auto button = em->createEntity();
	button->addComponent<ChestButtonGetComponent>();
	button->addComponent<PositionComponent>(
			CHEST_INTERFACE_BUTTON_GET_X,
			CHEST_INTERFACE_BUTTON_GET_Y
		);
	button->addComponent<RenderLayerComponent>(2);
	button->addComponent<SizeComponent>(
		CHEST_INTERFACE_BUTTON_WIDTH,
		CHEST_INTERFACE_BUTTON_HEIGHT
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
void initChestButtonClose(EntityManager *em) {
	auto button = em->createEntity();
	button->addComponent<ChestButtonCloseComponent>();
	button->addComponent<PositionComponent>(
			CHEST_INTERFACE_BUTTON_CLOSE_X,
			CHEST_INTERFACE_BUTTON_CLOSE_Y
		);
	button->addComponent<RenderLayerComponent>(2);
	button->addComponent<SizeComponent>(
		CHEST_INTERFACE_BUTTON_WIDTH,
		CHEST_INTERFACE_BUTTON_HEIGHT
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
void initChestInterface(EntityManager *em, RenderSystem* render) {
	auto inventoryInterface = em->createEntity();
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

void chestOpening(Controller* controller) {
	auto [input, em, render, state, battleContext] = controller->getAll();

	auto chests = em->getEntitiesWithComponent<ChestComponent>();
	auto player = em->getEntitiesWithComponent<PlayerControlComponent>()[0];
	auto keys = input->getPressedKeys();
	for (auto chest : chests) {
		if (doesChestConditionSatisfy(chest, player, keys)) {
			int chestId = chest->getComponent<ChestComponent>()->getId();
			*state = GameState::Chest;
			input->clear();
			chestInit(em, render, chestId);
			chest->getComponent<ChestComponent>()->setOpened();
		}
	}
}

void collectChest(Controller* controller) {
	auto [input, em, render, state, battleContext] = controller->getAll();

	auto button = em->getEntitiesWithComponent<ChestButtonGetComponent>().empty()
		? nullptr
		: em->getEntitiesWithComponent<ChestButtonGetComponent>()[0];
	if (
		button == nullptr
		|| !isClickOnEntity(input->getMouseClick(), button)
	) return;
	input->clear();
	auto entities = em->getEntitiesWithComponent<ChestContentComponent>();
	auto interface = em->getEntitiesWithComponent<ChestInterfaceComponent>();
	for(auto entity : entities) {
		render->removeEntity(entity->getId());
		entity->addComponent<ItemComponent>();
		entity->removeComponent<ChestContentComponent>();
	} 
	for(auto interfaceComp : interface) {
		render->removeEntity(interfaceComp->getId());
		em->removeEntity(interfaceComp);
	}
	*state = GameState::Game;
}

void closeChest(Controller* controller) {
	auto [input, em, render, state, battleContext] = controller->getAll();

	auto button = em->getEntitiesWithComponent<ChestButtonCloseComponent>()[0];
	if (
		!isEscapePressed(input->getPressedKeys())
		&& !isClickOnEntity(input->getMouseClick(), button)
	) return;
	input->clear();
	auto entities = em->getEntitiesWithComponent<ChestContentComponent>();
	auto interface = em->getEntitiesWithComponent<ChestInterfaceComponent>();
	for(auto entity : entities) {
		render->removeEntity(entity->getId());
		em->removeEntity(entity);
	} 
	for(auto interfaceComp : interface) {
		render->removeEntity(interfaceComp->getId());
		em->removeEntity(interfaceComp);
	}
	*state = GameState::Game;
}

bool doesChestConditionSatisfy(Entity *chest, Entity *player, std::vector<sf::Keyboard::Key> keys) {
	return isEnterPressed(keys)
	&& isCollision(chest, player)
	&& !chest->getComponent<ChestComponent>()->isOpened();
}