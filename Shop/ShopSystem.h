#pragma once
#include <string>
#include "../systems/Controller.h"
#include "../Components/Components.h"
#include "../systems/CollisionSystem.h"
#include "../const.h"

void createShopInterface(EntityManager* em);
void createPlayerAvatar(EntityManager* em);
void createShopAvatar(EntityManager* em);
void createShopButtons(EntityManager* em);
void createShopItems(EntityManager* em);
void createShop(EntityManager* em, RenderSystem* render);
void handleShopInput(Controller* controller);

void createOkButton(EntityManager* em);
void createBackButton(EntityManager* em);

void createPokeballShopItem(EntityManager* em);
void createPotionShopItem(EntityManager* em);

void shopCollision(Controller* controller) {
    auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();
    auto keys = input->getPressedKeys();
    
    if (std::find(keys.begin(), keys.end(), sf::Keyboard::Enter) == keys.end()) return;
    
    auto player = em->getEntitiesWithComponent<PlayerControlComponent>()[0];
    auto shops = em->getEntitiesWithComponent<ShopComponent>();
    
    if (shops.empty()) return;
    auto shop = shops[0];
    
    if (isCollision(player, shop)) {
        *state = GameState::Shop;
        createShop(em, render);
        render->addEntities(em->getEntitiesWithComponent<ShopTypeEntityComponent>());
        input->clear();
    }
}

void initShop(EntityManager* em) {
    auto shopEntity = em->createEntity();
    shopEntity->addComponent<ShopComponent>();
    shopEntity->addComponent<SizeComponent>(16, 11);
    shopEntity->addComponent<PositionComponent>(SHOP_POS_X, SHOP_POS_Y);
    shopEntity->addComponent<GameTypeEntityComponent>();
    shopEntity->addComponent<RenderLayerComponent>(1);
    
    sf::Color shopColor(68, 68, 68, 204);
    shopEntity->addComponent<ShapeComponent>(ShapeType::Ellipse, shopColor);
}

void createShop(EntityManager* em, RenderSystem* render) {
    createShopInterface(em);
    createPlayerAvatar(em);
    createShopAvatar(em);

    createShopItems(em);
    createShopButtons(em);
}

void selectPreviousItem(EntityManager* em) {
    auto items = em->getEntitiesWithComponent<ShopItemComponent>();
    int currentIndex = -1;
    
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i]->getComponent<ShopItemComponent>()->getIsSelected()) {
            currentIndex = i;
            items[i]->getComponent<ShopItemComponent>()->setSelected(false);
            break;
        }
    }
    
    int newIndex = (currentIndex - 1 + items.size()) % items.size();
    items[newIndex]->getComponent<ShopItemComponent>()->setSelected(true);
}

void selectNextItem(EntityManager* em) {
    auto items = em->getEntitiesWithComponent<ShopItemComponent>();
    int currentIndex = -1;
    
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i]->getComponent<ShopItemComponent>()->getIsSelected()) {
            currentIndex = i;
            items[i]->getComponent<ShopItemComponent>()->setSelected(false);
            break;
        }
    }
    
    int newIndex = (currentIndex + 1) % items.size();
    items[newIndex]->getComponent<ShopItemComponent>()->setSelected(true);
}

void tryToBuy(EntityManager* em) {
    auto player = em->getEntitiesWithComponent<PlayerControlComponent>()[0];
    auto inventory = player->getComponent<PlayersInventoryComponent>();
    
    auto items = em->getEntitiesWithComponent<ShopItemComponent>();
    for (auto item : items) {
        auto shopItem = item->getComponent<ShopItemComponent>();
        if (shopItem->getIsSelected()) {
            if (inventory->getCoinCount() >= shopItem->getPrice()) {
                if (!shopItem->getIsPokemon()) {
                    if (shopItem->getName() == "Potion") {
                        inventory->addPotions(1);
                    } else if (shopItem->getName() == "Pokeball") {
                        inventory->addPokeballs(1);
                    }
                    inventory->removeCoins(shopItem->getPrice());
                }
            }
            break;
        }
    }
}

void tryToSell(EntityManager* em) {
    auto player = em->getEntitiesWithComponent<PlayerControlComponent>()[0];
    auto inventory = player->getComponent<PlayersInventoryComponent>();
    
    auto items = em->getEntitiesWithComponent<ShopItemComponent>();
    for (auto item : items) {
        auto shopItem = item->getComponent<ShopItemComponent>();
        if (shopItem->getIsSelected()) {
            if (shopItem->getIsPokemon()) {
                if (inventory->getPokemonCount() > 0) {
                    inventory->addCoins(shopItem->getPrice());
                }
            } else {
                if (shopItem->getName() == "Potion" && inventory->getPotionCount() > 0) {
                    inventory->removePotions(1);
                    inventory->addCoins(shopItem->getPrice() / 2);
                } else if (shopItem->getName() == "Pokeball" && inventory->getPokeballCount() > 0) {
                    inventory->removePokeballs(1);
                    inventory->addCoins(shopItem->getPrice() / 2);
                }
            }
            break;
        }
    }
}

void handleShopInput(Controller* controller) {
    auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();
    auto keys = input->getPressedKeys();
    
    if (keys.empty()) return;
    
    if (std::find(keys.begin(), keys.end(), sf::Keyboard::Up) != keys.end()) {
        selectPreviousItem(em);
    }
    if (std::find(keys.begin(), keys.end(), sf::Keyboard::Down) != keys.end()) {
        selectNextItem(em);
    }
    
    if (std::find(keys.begin(), keys.end(), sf::Keyboard::Left) != keys.end()) {
        tryToBuy(em);
    }
    if (std::find(keys.begin(), keys.end(), sf::Keyboard::Right) != keys.end()) {
        tryToSell(em);
    }

	if (
        std::find(keys.begin(), keys.end(), sf::Keyboard::Escape) != keys.end() 
        || (std::find(keys.begin(), keys.end(), sf::Keyboard::Enter) != keys.end() 
        && !em->getEntitiesWithComponent<ShopButtonBackComponent>().empty())
    ) {
        *state = GameState::Game;
        auto shopEntities = em->getEntitiesWithComponent<ShopTypeEntityComponent>();
        for (auto entity : shopEntities) {
            render->removeEntity(entity->getId());
            em->removeEntity(entity);
        }
        input->clear();
    }
} 

void createShopInterface(EntityManager* em) {
    auto shopInterface = em->createEntity();
    shopInterface->addComponent<ShopTypeEntityComponent>();
    shopInterface->addComponent<PositionComponent>(
        WINDOW_WIDTH / 2 - SHOP_INTERFACE_WIDTH / 2,
        WINDOW_HEIGHT / 2 - SHOP_INTERFACE_HEIGHT / 2
    );
    shopInterface->addComponent<SizeComponent>(SHOP_INTERFACE_WIDTH, SHOP_INTERFACE_HEIGHT);
    shopInterface->addComponent<RenderLayerComponent>(5);
	sf::Texture shopTexture;
    if (shopTexture.loadFromFile("../res/background_menu(62x46).png")) {
        shopInterface->addComponent<TextureComponent>(shopTexture, 62, 46);
    }
}

void createPlayerAvatar(EntityManager* em) {
    auto playerAvatar = em->createEntity();
    playerAvatar->addComponent<PlayerAvatarComponent>();
    playerAvatar->addComponent<ShopTypeEntityComponent>();
    playerAvatar->addComponent<RenderLayerComponent>(6);
	playerAvatar->addComponent<SizeComponent>(64, 64);
    playerAvatar->addComponent<PositionComponent>(
        PLAYER_AVATAR_X,
        PLAYER_AVATAR_Y
    );
    sf::Texture playerTexture;

    if (playerTexture.loadFromFile("../res/ash_avatar(64x64).png")) {
        playerAvatar->addComponent<TextureComponent>(playerTexture, 64, 64);
    }
}

void createShopAvatar(EntityManager* em) {
    auto shopAvatar = em->createEntity();
    shopAvatar->addComponent<ShopAvatarComponent>();
    shopAvatar->addComponent<ShopTypeEntityComponent>();
    shopAvatar->addComponent<RenderLayerComponent>(6);
	shopAvatar->addComponent<SizeComponent>(64, 48);
    shopAvatar->addComponent<PositionComponent>(
        SHOP_AVATAR_X,
        SHOP_AVATAR_Y
    );

    sf::Texture shopAvatarTexture;
    if (shopAvatarTexture.loadFromFile("../res/shop_avatar(64x48).png")) {
        shopAvatar->addComponent<TextureComponent>(shopAvatarTexture, 64, 48);
    }
}

void createShopItems(EntityManager* em) {
    createPokeballShopItem(em);
    createPotionShopItem(em);
}

void createPokeballShopItem(EntityManager* em) {
    auto pokeballItem = em->createEntity();
    pokeballItem->addComponent<ShopItemComponent>("Pokeball", POKEBALL_PRICE, false);
    pokeballItem->addComponent<ShopTypeEntityComponent>();
    pokeballItem->addComponent<PositionComponent>(
        SHOP_ITEM_POS_X,
        SHOP_ITEM_POS_Y
    );
    pokeballItem->addComponent<RenderLayerComponent>(6);
    pokeballItem->addComponent<SizeComponent>(SHOP_ITEM_WIDTH, SHOP_ITEM_HEIGHT);
    sf::Texture pokeballTexture;
    if (pokeballTexture.loadFromFile("../res/pokeball(12x12).png")) {
        pokeballItem->addComponent<TextureComponent>(pokeballTexture, 12, 12);
    }   

    auto pokeballText = em->createEntity();
    pokeballText->addComponent<TextComponent>(
        "Pokeball - " + std::to_string(POKEBALL_PRICE) + " coins",
        SHOP_ITEM_POS_X + SHOP_ITEM_WIDTH + 10,
        SHOP_ITEM_POS_Y + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 4,
        SHOP_TEXT_HEIGHT
    );
    pokeballText->addComponent<ShopTypeEntityComponent>();
    pokeballText->addComponent<RenderLayerComponent>(6);
}

void createPotionShopItem(EntityManager* em) {
    auto potionItem = em->createEntity();
    potionItem->addComponent<ShopItemComponent>("Potion", POTION_PRICE, false);
    potionItem->addComponent<ShopTypeEntityComponent>();
    potionItem->addComponent<PositionComponent>(
        SHOP_ITEM_POS_X,
        SHOP_ITEM_POS_Y + SHOP_ITEM_HEIGHT + SHOP_ITEM_GAP
    );
    potionItem->addComponent<RenderLayerComponent>(6);
    potionItem->addComponent<SizeComponent>(SHOP_ITEM_WIDTH, SHOP_ITEM_HEIGHT);
    sf::Texture potionTexture;
    if (potionTexture.loadFromFile("../res/healingPotion(12x12).png")) {
        potionItem->addComponent<TextureComponent>(potionTexture, 12, 12);
    }

    auto potionText = em->createEntity();
    potionText->addComponent<TextComponent>(
        "Potion - " + std::to_string(POTION_PRICE) + " coins",
        SHOP_ITEM_POS_X + SHOP_ITEM_WIDTH + 10,
        SHOP_ITEM_POS_Y + SHOP_ITEM_HEIGHT + SHOP_ITEM_GAP + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 4,
        SHOP_TEXT_HEIGHT
    );
    potionText->addComponent<ShopTypeEntityComponent>();
    potionText->addComponent<RenderLayerComponent>(6);
}

void createShopButtons(EntityManager* em) {
    createOkButton(em);
    createBackButton(em);
}

void createOkButton(EntityManager* em) {
    auto okButton = em->createEntity();
    okButton->addComponent<ShopButtonOkComponent>();
    okButton->addComponent<ShopTypeEntityComponent>();
    okButton->addComponent<PositionComponent>(
        SHOP_BUTTON_OK_X,
        SHOP_BUTTON_OK_Y
    );
    okButton->addComponent<RenderLayerComponent>(6);
    okButton->addComponent<SizeComponent>(SHOP_BUTTON_WIDTH, SHOP_BUTTON_HEIGHT);
    sf::Texture okTexture;
    if (okTexture.loadFromFile("../res/okButton(32x13).png")) {
        okButton->addComponent<TextureComponent>(okTexture, 32, 13);
    }
}

void createBackButton(EntityManager* em) {
    auto backButton = em->createEntity();
    backButton->addComponent<ShopButtonBackComponent>();
    backButton->addComponent<ShopTypeEntityComponent>();
    backButton->addComponent<PositionComponent>(
        SHOP_BUTTON_BACK_X,
        SHOP_BUTTON_BACK_Y
    );
    backButton->addComponent<RenderLayerComponent>(6);
    backButton->addComponent<SizeComponent>(SHOP_BUTTON_WIDTH, SHOP_BUTTON_HEIGHT);
    sf::Texture backTexture;
    if (backTexture.loadFromFile("../res/backButton(32x13).png")) {
        backButton->addComponent<TextureComponent>(backTexture, 32, 13);
    }
}
