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

void generateChests(Controller* controller, float deltaTime);

void updateChests(Controller* controller) {
	closeChest(controller);
	collectChest(controller);
}

void handleOpenedChests(Controller* controller, std::vector<Entity*> chests, float deltaTime);

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
			CHEST_INTERFACE_X + 5 + (index % MAX_ITEMS_PER_ROW - 1) * 25,
			CHEST_INTERFACE_Y + 5 + index / MAX_ITEMS_PER_ROW * 25
		);
		break;
	}

	case 1: {
		auto coin = createCoin(em);
		coin->addComponent<ChestContentComponent>(chestId);
		coin->addComponent<PositionComponent>(
			CHEST_INTERFACE_X + 5 + (index % MAX_ITEMS_PER_ROW - 1) * 25,
			CHEST_INTERFACE_Y + 5 + index / MAX_ITEMS_PER_ROW * 25
		);
		break;
	}
	
	default:
		break;
	}
}

Entity* createHealingPotion(EntityManager *em) {
    auto healingPotion = em->createEntity();
    healingPotion->addComponent<HealingPotionComponent>();
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
    coin->addComponent<CoinComponent>();
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
    if (buttonTexture.loadFromFile("../res/okButton(32x13).png")) {
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
	auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();

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
			chest->getComponent<SizeComponent>()->setSize(26, 27);
			sf::Texture openedTexture;
			if (openedTexture.loadFromFile("../res/chestOpened(26x27).png")) {
				auto texture = chest->getComponent<TextureComponent>();
				texture->setTexture(openedTexture);
				texture->setWidth(26);
				texture->setHeight(27);
			}
		}
	}
}


void collectChest(Controller* controller) {
	auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();

	auto button = em->getEntitiesWithComponent<ChestButtonGetComponent>().empty()
		? nullptr
		: em->getEntitiesWithComponent<ChestButtonGetComponent>()[0];
	if (
		button == nullptr
		|| !isClickOnEntity(input->getMouseClick(), button)
	) return;
	
	input->clear();
	auto player = em->getEntitiesWithComponent<PlayerControlComponent>()[0];
	auto inventory = player->getComponent<PlayersInventoryComponent>();
	
	auto entities = em->getEntitiesWithComponent<ChestContentComponent>();
	for(auto entity : entities) {
		render->removeEntity(entity->getId());
		if (entity->getComponent<CoinComponent>() != nullptr) {
			inventory->addCoins(1);
		} else if (entity->getComponent<HealingPotionComponent>() != nullptr) {
			inventory->addPotions(1);
		}
		em->removeEntity(entity);
	}
	
	auto interface = em->getEntitiesWithComponent<ChestInterfaceComponent>();
	for(auto interfaceElem : interface) {
		render->removeEntity(interfaceElem->getId());
		em->removeEntity(interfaceElem);
	}

	*state = GameState::Game;
}


void closeChest(Controller* controller) {
	auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();

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

void generateChest(Controller* controller) {
    auto [input, em, render, state, battleContext, collisionMaps, currentLocation] = controller->getAll();

    auto currentMap = collisionMaps->at(currentLocation);
    int mapWidth = currentMap.getWidth() * currentMap.getCellWidth();
    int mapHeight = currentMap.getHeight() * currentMap.getCellHeight();
    
    float x = std::rand() % (mapWidth - CHEST_WIDTH);
    float y = std::rand() % (mapHeight - CHEST_HEIGHT);
    
    bool validPosition = false;
    int attempts = 0;
    const int maxAttempts = 100;
    
    while (!validPosition && attempts < maxAttempts) {
        if (attempts > 0) {
            x = std::rand() % (mapWidth - CHEST_WIDTH);
            y = std::rand() % (mapHeight - CHEST_HEIGHT);
        }
        
        bool hasCollision = currentMap.isCollision(x, y);
        
        if (!hasCollision) {
            auto chest = em->createEntity();
            chest->addComponent<PositionComponent>(x, y);
            chest->addComponent<SizeComponent>(CHEST_WIDTH, CHEST_HEIGHT);
            chest->addComponent<RenderLayerComponent>(1);
            chest->addComponent<ChestComponent>();
            chest->addComponent<GameTypeEntityComponent>();
            
            sf::Texture chestTexture;
            if (chestTexture.loadFromFile("../res/chestClosedOpasity(24x23).png")) {
                chest->addComponent<TextureComponent>(
					chestTexture,
					24,
					23
				);
            }
            render->addEntity(chest);
            validPosition = true;

        }
        attempts++;
    }
}

void chestsGenerating(Controller* controller, float deltaTime) {
    static float timeSinceLastChest = 0.0f;
    const float chestGenerationInterval = 5.0f;

    auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();
    auto chests = em->getEntitiesWithComponent<ChestComponent>();
    
	if (!chests.empty()) {
		handleOpenedChests(controller, chests, deltaTime);
	}

    bool hasClosedChest = false;
    for (auto chest : chests) {
        if (!chest->getComponent<ChestComponent>()->isOpened()) {
            hasClosedChest = true;
            break;
        }
    }
    
    if (!hasClosedChest) {
        timeSinceLastChest += deltaTime;
        if (timeSinceLastChest >= chestGenerationInterval) {
            timeSinceLastChest = 0.0f;
            generateChest(controller);
        }
    }
}

void handleOpenedChests(Controller* controller, std::vector<Entity*> chests, float deltaTime) {
	auto em = controller->getEntityManager();
	auto render = controller->getRenderSystem();

	for (auto chest : chests) {
        if (chest->getComponent<ChestComponent>()->isOpened()) {
            chest->getComponent<ChestComponent>()->addTimeAfterOpening(deltaTime);
            
            if (chest->getComponent<ChestComponent>()->getTimeAfterOpening() >= 30.0f) {
                render->removeEntity(chest->getId());
                em->removeEntity(chest);
            }
        }
    }
}