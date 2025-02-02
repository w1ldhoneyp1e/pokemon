#pragma once
#include <string>
#include "../systems/Controller.h"
#include "../Components/Components.h"
#include "../systems/CollisionSystem.h"
#include "../const.h"


void createShopButtons(EntityManager* em);
void createShopItems(EntityManager* em);
void createShopInterface(EntityManager* em, RenderSystem* render);
void handleShopInput(Controller* controller);



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

void createShopInterface(EntityManager* em, RenderSystem* render) {
    auto shopInterface = em->createEntity();
    shopInterface->addComponent<ShopTypeEntityComponent>();
    shopInterface->addComponent<PositionComponent>(
        WINDOW_WIDTH / 2 - SHOP_INTERFACE_WIDTH / 2,
        WINDOW_HEIGHT / 2 - SHOP_INTERFACE_HEIGHT / 2
    );
    shopInterface->addComponent<SizeComponent>(SHOP_INTERFACE_WIDTH, SHOP_INTERFACE_HEIGHT);
    shopInterface->addComponent<RenderLayerComponent>(4);
    
    auto playerAvatar = em->createEntity();
    playerAvatar->addComponent<PlayerAvatarComponent>();
    playerAvatar->addComponent<ShopTypeEntityComponent>();
    playerAvatar->addComponent<RenderLayerComponent>(5);
    playerAvatar->addComponent<PositionComponent>(
        WINDOW_WIDTH / 2 - SHOP_INTERFACE_WIDTH / 2 + 20,
        WINDOW_HEIGHT / 2 - SHOP_INTERFACE_HEIGHT / 2 + 20
    );
    sf::Texture playerTexture;
    if (playerTexture.loadFromFile("../res/ash_avatar(64x64).png")) {
        playerAvatar->addComponent<TextureComponent>(playerTexture, 64, 64);
    }
    
    auto shopAvatar = em->createEntity();
    shopAvatar->addComponent<ShopAvatarComponent>();
    shopAvatar->addComponent<ShopTypeEntityComponent>();
    shopAvatar->addComponent<RenderLayerComponent>(5);
    shopAvatar->addComponent<PositionComponent>(
        WINDOW_WIDTH / 2 + SHOP_INTERFACE_WIDTH / 2 - 84,
        WINDOW_HEIGHT / 2 - SHOP_INTERFACE_HEIGHT / 2 + 20
    );
    sf::Texture shopTexture;
    if (shopTexture.loadFromFile("../res/shop_avatar(64x48).png")) {
        shopAvatar->addComponent<TextureComponent>(shopTexture, 64, 48);
    }

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

	if (std::find(keys.begin(), keys.end(), sf::Keyboard::Escape) != keys.end() ||
    (std::find(keys.begin(), keys.end(), sf::Keyboard::Enter) != keys.end() && 
     !em->getEntitiesWithComponent<ShopButtonBackComponent>().empty())) {
    *state = GameState::Game;
    auto shopEntities = em->getEntitiesWithComponent<ShopTypeEntityComponent>();
    for (auto entity : shopEntities) {
        render->removeEntity(entity->getId());
        em->removeEntity(entity);
    }
    input->clear();
}
} 

void createShopItems(EntityManager* em) {
    float startY = WINDOW_HEIGHT / 2 - SHOP_INTERFACE_HEIGHT / 2 + 100;
    
    auto pokeballItem = em->createEntity();
    pokeballItem->addComponent<ShopItemComponent>("Pokeball", POKEBALL_PRICE, false);
    pokeballItem->addComponent<ShopTypeEntityComponent>();
    pokeballItem->addComponent<PositionComponent>(
        WINDOW_WIDTH / 2 - SHOP_INTERFACE_WIDTH / 2 + 60,
        startY
    );
    pokeballItem->addComponent<RenderLayerComponent>(5);
    pokeballItem->addComponent<SizeComponent>(16, 16);
    sf::Texture pokeballTexture;
    if (pokeballTexture.loadFromFile("../res/pokeball(12x12).png")) {
        pokeballItem->addComponent<TextureComponent>(pokeballTexture, 12, 12);
    }
    
    auto potionItem = em->createEntity();
    potionItem->addComponent<ShopItemComponent>("Potion", POTION_PRICE, false);
    potionItem->addComponent<ShopTypeEntityComponent>();
    potionItem->addComponent<PositionComponent>(
        WINDOW_WIDTH / 2 - SHOP_INTERFACE_WIDTH / 2 + 60,
        startY + SHOP_ITEM_HEIGHT
    );
    potionItem->addComponent<RenderLayerComponent>(5);
    potionItem->addComponent<SizeComponent>(16, 16);
    sf::Texture potionTexture;
    if (potionTexture.loadFromFile("../res/healingPotion(12x12).png")) {
        potionItem->addComponent<TextureComponent>(potionTexture, 12, 12);
    }
    
    auto pokeballText = em->createEntity();
    pokeballText->addComponent<TextComponent>(
        "Pokeball - " + std::to_string(POKEBALL_PRICE) + " coins",
        WINDOW_WIDTH / 2 - SHOP_INTERFACE_WIDTH / 2 + 80,
        startY,
        20
    );
    pokeballText->addComponent<ShopTypeEntityComponent>();
    pokeballText->addComponent<RenderLayerComponent>(5);
    
    auto potionText = em->createEntity();
    potionText->addComponent<TextComponent>(
        "Potion - " + std::to_string(POTION_PRICE) + " coins",
        WINDOW_WIDTH / 2 - SHOP_INTERFACE_WIDTH / 2 + 80,
        startY + SHOP_ITEM_HEIGHT,
        20
    );
    potionText->addComponent<ShopTypeEntityComponent>();
    potionText->addComponent<RenderLayerComponent>(5);
}

void createShopButtons(EntityManager* em) {
    auto okButton = em->createEntity();
    okButton->addComponent<ShopButtonOkComponent>();
    okButton->addComponent<ShopTypeEntityComponent>();
    okButton->addComponent<PositionComponent>(
        WINDOW_WIDTH / 2 - SHOP_INTERFACE_WIDTH / 4,
        WINDOW_HEIGHT / 2 + SHOP_INTERFACE_HEIGHT / 2 - 40
    );
    okButton->addComponent<RenderLayerComponent>(5);
    okButton->addComponent<SizeComponent>(32, 13);
    sf::Texture okTexture;
    if (okTexture.loadFromFile("../res/okButton(32x13).png")) {
        okButton->addComponent<TextureComponent>(okTexture, 32, 13);
    }
    
    auto backButton = em->createEntity();
    backButton->addComponent<ShopButtonBackComponent>();
    backButton->addComponent<ShopTypeEntityComponent>();
    backButton->addComponent<PositionComponent>(
        WINDOW_WIDTH / 2 + SHOP_INTERFACE_WIDTH / 4 - 32,
        WINDOW_HEIGHT / 2 + SHOP_INTERFACE_HEIGHT / 2 - 40
    );
    backButton->addComponent<RenderLayerComponent>(5);
    backButton->addComponent<SizeComponent>(32, 13);
    sf::Texture backTexture;
    if (backTexture.loadFromFile("../res/backButton(32x13).png")) {
        backButton->addComponent<TextureComponent>(backTexture, 32, 13);
    }
}

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
        createShopInterface(em, render);
        render->addEntities(em->getEntitiesWithComponent<ShopTypeEntityComponent>());
        input->clear();
    }
}