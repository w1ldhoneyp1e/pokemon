#pragma once
#include <string>
#include "../systems/Controller.h"
#include "../Components/Components.h"
#include "../systems/CollisionSystem.h"
#include "../const.h"

struct ShopItemInfo {
    std::string name;
    int price;
    std::string texturePath;
    bool isPokemon;
};

void createShopInterface(EntityManager* em);
void createPlayerAvatar(EntityManager* em);
void createShopAvatar(EntityManager* em);
void createShopButtons(EntityManager* em);
void createShopItems(EntityManager* em);
void createShop(EntityManager* em, RenderSystem* render);
void handleShopInput(Controller* controller, float deltaTime);

void createOkButton(EntityManager* em);
void createBackButton(EntityManager* em);
void createShopHeader(EntityManager* em);

void createShopRow(EntityManager* em, const ShopItemInfo& itemInfo, float yPos);

PlayersInventoryComponent* getPlayerInventory(EntityManager* em);
int getItemCount(PlayersInventoryComponent* inventory, const std::string& itemName);

void updateRowHighlighting(EntityManager* em);

Entity* getSelectedItem(EntityManager* em) {
    auto items = em->getEntitiesWithComponent<ShopItemComponent>();
    for (auto item : items) {
        auto shopItem = item->getComponent<ShopItemComponent>();
        if (shopItem->getIsSelected()) {
            return item;
        }
    }
    return nullptr;
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
    createShopHeader(em);
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
    updateRowHighlighting(em);
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
    updateRowHighlighting(em);
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

float selectionCooldown = 0.0f;
const float SELECTION_DELAY = 0.5f;

float buySellCooldown = 0.0f;
const float BUYSELL_DELAY = 0.2f;

void handleShopInput(Controller* controller, float deltaTime) {
    auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();
    auto keys = input->getPressedKeys();
    auto mouseClick = input->getMouseClick();
    bool mouseClicked = input->hasMouseClick();
    
    if (keys.empty() && !mouseClicked) return;
    
    auto okButton = em->getEntitiesWithComponent<ShopButtonOkComponent>()[0];
    auto backButton = em->getEntitiesWithComponent<ShopButtonBackComponent>()[0];

    selectionCooldown = std::max(0.0f, selectionCooldown - deltaTime);
    buySellCooldown = std::max(0.0f, buySellCooldown - deltaTime);
    
    if (selectionCooldown <= 0) {
        if (std::find(keys.begin(), keys.end(), sf::Keyboard::Up) != keys.end()) {
            selectPreviousItem(em);
            selectionCooldown = SELECTION_DELAY;
            return;
        }
        
        if (std::find(keys.begin(), keys.end(), sf::Keyboard::Down) != keys.end()) {
            selectNextItem(em);
            selectionCooldown = SELECTION_DELAY;
            return;
        }
    }
    
    auto selectedItem = getSelectedItem(em);
    if (selectedItem && buySellCooldown <= 0) {
        auto shopItem = selectedItem->getComponent<ShopItemComponent>();
        auto inventory = getPlayerInventory(em);
        
        if (std::find(keys.begin(), keys.end(), sf::Keyboard::Left) != keys.end()) {
            std::cout << "shopItem->getBuyCount(): " << shopItem->getBuyCount() << std::endl;
            std::cout << "shopItem->getSellCount(): " << shopItem->getSellCount() << std::endl;
            if (shopItem->getBuyCount() > 0) {
                shopItem->setBuyCount(shopItem->getBuyCount() - 1);
            } else {

                int maxSell = getItemCount(inventory, shopItem->getName());
                if (shopItem->getSellCount() < maxSell) {
                    shopItem->setSellCount(shopItem->getSellCount() + 1);
                }
            }
            buySellCooldown = BUYSELL_DELAY;
        }
        
        if (std::find(keys.begin(), keys.end(), sf::Keyboard::Right) != keys.end()) {
            std::cout << "shopItem->getBuyCount(): " << shopItem->getBuyCount() << std::endl;
            std::cout << "shopItem->getSellCount(): " << shopItem->getSellCount() << std::endl;
            if (shopItem->getSellCount() > 0) {
                shopItem->setSellCount(shopItem->getSellCount() - 1);
            } else {
                int maxBuy = inventory->getCoinCount() / shopItem->getPrice();
                if (shopItem->getBuyCount() < maxBuy) {
                    shopItem->setBuyCount(shopItem->getBuyCount() + 1);
                }
            }
            buySellCooldown = BUYSELL_DELAY;
        }
    }
    
    if (mouseClicked) {
        if (isClickOnEntity(mouseClick, backButton)) {
            *state = GameState::Game;
            auto shopEntities = em->getEntitiesWithComponent<ShopTypeEntityComponent>();
            for (auto entity : shopEntities) {
                render->removeEntity(entity->getId());
                em->removeEntity(entity);
            }
            input->clear();
            return;
        }
        
        if (isClickOnEntity(mouseClick, okButton)) {
            auto selectedItem = getSelectedItem(em);
            if (selectedItem) {
                auto shopItem = selectedItem->getComponent<ShopItemComponent>();
                if (shopItem->getBuyCount() > 0) {
                    tryToBuy(em);
                } else if (shopItem->getSellCount() > 0) {
                    tryToSell(em);
                }
                
                *state = GameState::Game;
                auto shopEntities = em->getEntitiesWithComponent<ShopTypeEntityComponent>();
                for (auto entity : shopEntities) {
                    render->removeEntity(entity->getId());
                    em->removeEntity(entity);
                }
                input->clear();
            }
            return;
        }
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
    auto selectionBackground = em->createEntity();
    selectionBackground->addComponent<ShopTypeEntityComponent>();
    selectionBackground->addComponent<ShopSelectedBackgroundComponent>();
    selectionBackground->addComponent<PositionComponent>(
        SHOW_HEADER_START_X,
        SHOP_ITEM_POS_Y
    );
    selectionBackground->addComponent<SizeComponent>(
        SHOP_COLUMN_WIDTH_SMALL + SHOP_COLUMN_WIDTH_BIG + SHOP_COLUMN_WIDTH_MEDIUM * 2 + SHOP_COLUMN_WIDTH_SMALL * 2,
        SHOP_ITEM_HEIGHT
    );
    selectionBackground->addComponent<RenderLayerComponent>(6);
    selectionBackground->addComponent<ShapeComponent>(ShapeType::Rectangle, sf::Color(52, 52, 52, 150));

    std::vector<ShopItemInfo> items = {
        {"Pokeball", POKEBALL_PRICE, "../res/pokeball(12x12).png", false},
        {"Potion", POTION_PRICE, "../res/healingPotion(12x12).png", false}
    };

    float yOffset = SHOP_ITEM_POS_Y;
    for (const auto& itemInfo : items) {
        createShopRow(em, itemInfo, yOffset);
        yOffset += SHOP_ITEM_HEIGHT + SHOP_ITEM_GAP;
    }

    auto shopItems = em->getEntitiesWithComponent<ShopItemComponent>();
    if (!shopItems.empty()) {
        shopItems[0]->getComponent<ShopItemComponent>()->setSelected(true);
        updateRowHighlighting(em);
    }
}

void createShopRow(EntityManager* em, const ShopItemInfo& itemInfo, float yPos) {
    auto inventory = getPlayerInventory(em);
    auto itemCount = getItemCount(inventory, itemInfo.name);
    
    auto countText = em->createEntity();
    countText->addComponent<TextComponent>(
        std::to_string(itemCount),
        SHOW_HEADER_START_X,
        yPos + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 8,
        SHOP_TEXT_HEIGHT / 2
    );
    countText->addComponent<ShopTypeEntityComponent>();
    countText->addComponent<RenderLayerComponent>(7);

    auto item = em->createEntity();
    item->addComponent<ShopItemComponent>(itemInfo.name, itemInfo.price, itemInfo.isPokemon);
    item->addComponent<ShopTypeEntityComponent>();
    item->addComponent<PositionComponent>(
        SHOW_HEADER_START_X + SHOP_COLUMN_WIDTH_MEDIUM,
        yPos
    );
    item->addComponent<RenderLayerComponent>(7);
    item->addComponent<SizeComponent>(SHOP_ITEM_WIDTH, SHOP_ITEM_HEIGHT);

    sf::Texture itemTexture;
    if (itemTexture.loadFromFile(itemInfo.texturePath)) {
        item->addComponent<TextureComponent>(itemTexture, 12, 12);
    }

    auto nameText = em->createEntity();
    nameText->addComponent<TextComponent>(
        itemInfo.name,
        SHOW_HEADER_START_X + SHOP_COLUMN_WIDTH_MEDIUM + SHOP_ITEM_WIDTH + 10,
        yPos + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 8,
        SHOP_TEXT_HEIGHT / 2
    );
    nameText->addComponent<ShopTypeEntityComponent>();
    nameText->addComponent<RenderLayerComponent>(7);

    auto sellPriceText = em->createEntity();
    sellPriceText->addComponent<TextComponent>(
        std::to_string(itemInfo.price / 2),
        SHOW_HEADER_START_X + SHOP_COLUMN_WIDTH_MEDIUM + SHOP_COLUMN_WIDTH_BIG,
        yPos + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 8,
        SHOP_TEXT_HEIGHT / 2

    );
    sellPriceText->addComponent<ShopTypeEntityComponent>();
    sellPriceText->addComponent<RenderLayerComponent>(7);

    auto buyPriceText = em->createEntity();
    buyPriceText->addComponent<TextComponent>(
        std::to_string(itemInfo.price),
        SHOW_HEADER_START_X + SHOP_COLUMN_WIDTH_BIG + SHOP_COLUMN_WIDTH_MEDIUM * 2,
        yPos + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 8,
        SHOP_TEXT_HEIGHT / 2

    );
    buyPriceText->addComponent<ShopTypeEntityComponent>();
    buyPriceText->addComponent<RenderLayerComponent>(7);

    auto sellText = em->createEntity();
    sellText->addComponent<TextComponent>(
        "0",
        SHOW_HEADER_START_X + SHOP_COLUMN_WIDTH_BIG + SHOP_COLUMN_WIDTH_MEDIUM * 3,
        yPos + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 8,
        SHOP_TEXT_HEIGHT / 2

    );
    sellText->addComponent<ShopTypeEntityComponent>();
    sellText->addComponent<RenderLayerComponent>(7);

    auto buyText = em->createEntity();
    buyText->addComponent<TextComponent>(
        "0",
        SHOW_HEADER_START_X + SHOP_COLUMN_WIDTH_BIG + SHOP_COLUMN_WIDTH_MEDIUM * 3 + SHOP_COLUMN_WIDTH_SMALL,
        yPos + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 8,
        SHOP_TEXT_HEIGHT / 2
    );
    buyText->addComponent<ShopTypeEntityComponent>();
    buyText->addComponent<RenderLayerComponent>(7);
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

void createShopHeader(EntityManager* em) {    
    std::vector<std::string> headers = {
        "Amount", "Item", "Price -", "Price +", "Sell", "Buy"
    };
    
    float xOffset = SHOW_HEADER_START_X;
    for (const auto& header : headers) {
        auto text = em->createEntity();
        text->addComponent<TextComponent>(
            header,
            xOffset,
            SHOP_ITEM_POS_Y - SHOP_HEADER_HEIGHT,
            16
        );
        text->addComponent<ShopTypeEntityComponent>();
        text->addComponent<RenderLayerComponent>(6);
        if (header == "Amount") {
            xOffset += SHOP_COLUMN_WIDTH_MEDIUM;
        } else if (header == "Item") {
            xOffset += SHOP_COLUMN_WIDTH_BIG;
        } else if (header == "Price -") {

            xOffset += SHOP_COLUMN_WIDTH_MEDIUM;
        } else if (header == "Price +") {
            xOffset += SHOP_COLUMN_WIDTH_MEDIUM;
        } else if (header == "Sell") {
                xOffset += SHOP_COLUMN_WIDTH_SMALL;
        } else if (header == "Buy") {
            xOffset += SHOP_COLUMN_WIDTH_SMALL;
        }
    }
}

PlayersInventoryComponent* getPlayerInventory(EntityManager* em) {
    auto players = em->getEntitiesWithComponent<PlayerControlComponent>();
    if (players.empty()) return nullptr;
    return players[0]->getComponent<PlayersInventoryComponent>();
}

int getItemCount(PlayersInventoryComponent* inventory, const std::string& itemName) {
    if (!inventory) return 0;
    
    if (itemName == "Pokeball") {
        return inventory->getPokeballCount();
    } else if (itemName == "Potion") {
        return inventory->getPotionCount();
    } else {
        return inventory->getPokemonCount();
    }
}

void updateItemCounts(EntityManager* em) {
    auto inventory = getPlayerInventory(em);
    auto items = em->getEntitiesWithComponent<ShopItemComponent>();
    
    for (auto item : items) {
        auto shopItem = item->getComponent<ShopItemComponent>();
        auto itemCount = getItemCount(inventory, shopItem->getName());
        
        auto countText = em->createEntity();
        countText->addComponent<TextComponent>(
            std::to_string(itemCount),
            SHOW_HEADER_START_X,
            item->getComponent<PositionComponent>()->getY() + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 4,
            SHOP_TEXT_HEIGHT
        );
        countText->addComponent<ShopTypeEntityComponent>();
        countText->addComponent<RenderLayerComponent>(6);
        
        auto sellText = em->createEntity();
        sellText->addComponent<TextComponent>(
            std::to_string(shopItem->getSellCount()),
            SHOW_HEADER_START_X + SHOP_COLUMN_WIDTH_SMALL + SHOP_COLUMN_WIDTH_BIG + SHOP_COLUMN_WIDTH_MEDIUM * 2,
            item->getComponent<PositionComponent>()->getY() + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 4,
            SHOP_TEXT_HEIGHT
        );
        sellText->addComponent<ShopTypeEntityComponent>();
        sellText->addComponent<RenderLayerComponent>(6);
        
        auto buyText = em->createEntity();
        buyText->addComponent<TextComponent>(
            std::to_string(shopItem->getBuyCount()),
            SHOW_HEADER_START_X + SHOP_COLUMN_WIDTH_SMALL + SHOP_COLUMN_WIDTH_BIG + SHOP_COLUMN_WIDTH_MEDIUM * 2 + SHOP_COLUMN_WIDTH_SMALL,
            item->getComponent<PositionComponent>()->getY() + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 4,
            SHOP_TEXT_HEIGHT
        );
        buyText->addComponent<ShopTypeEntityComponent>();
        buyText->addComponent<RenderLayerComponent>(6);
    }
}

void updateRowHighlighting(EntityManager* em) {
    auto selectedItem = getSelectedItem(em);
    if (!selectedItem) return;

    auto background = em->getEntitiesWithComponent<ShopSelectedBackgroundComponent>()[0];
    auto pos = selectedItem->getComponent<PositionComponent>();
    background->getComponent<PositionComponent>()->setPos(pos->getX() - SHOP_COLUMN_WIDTH_MEDIUM, pos->getY());
}
