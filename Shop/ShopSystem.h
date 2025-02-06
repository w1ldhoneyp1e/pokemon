#pragma once
#include <string>
#include "../systems/Controller.h"
#include "../Components/Components.h"
#include "../systems/CollisionSystem.h"
#include "../const.h"
#include "./ShopEntities.h"

void createShop(EntityManager* em, RenderSystem* render);
void createShopItems(EntityManager* em);
void handleShopInput(Controller* controller, float deltaTime);
void createShopRow(EntityManager* em, const ShopItemInfo& itemInfo, float yPos);
PlayersInventoryComponent* getPlayerInventory(EntityManager* em);
int getItemCount(PlayersInventoryComponent* inventory, const std::string& itemName);
void updateRowHighlighting(EntityManager* em);
void updateBuyText(EntityManager* em, Entity* selectedItem);
void updateSellText(EntityManager* em, Entity* selectedItem);
void updateShopRowValues(EntityManager* em);
void doTransaction(Controller* controller);

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
                        inventory->addPotions(shopItem->getBuyCount());
                    } else if (shopItem->getName() == "Pokeball") {
                        inventory->addPokeballs(shopItem->getBuyCount());
                    }
                    inventory->removeCoins(shopItem->getPrice() * shopItem->getBuyCount());
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
                    inventory->removePotions(shopItem->getSellCount());
                    inventory->addCoins(shopItem->getPrice() * shopItem->getSellCount() / 2);
                } else if (shopItem->getName() == "Pokeball" && inventory->getPokeballCount() > 0) {
                    inventory->removePokeballs(shopItem->getSellCount());
                    inventory->addCoins(shopItem->getPrice() * shopItem->getSellCount() / 2);
                }
            }
            break;
        }
    }
}

float selectionCooldown = 0.0f;
float buySellCooldown = 0.0f;

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
        
        if (std::find(keys.begin(), keys.end(), sf::Keyboard::Right) != keys.end()) {
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
        
        if (std::find(keys.begin(), keys.end(), sf::Keyboard::Left) != keys.end()) {
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
		updateShopRowValues(em);
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
            doTransaction(controller);
            return;
        }
    }
    
    if (std::find(keys.begin(), keys.end(), sf::Keyboard::Enter) != keys.end()) {
        doTransaction(controller);
        return;
    }

    if (
        std::find(keys.begin(), keys.end(), sf::Keyboard::Escape) != keys.end() 
        && !em->getEntitiesWithComponent<ShopButtonBackComponent>().empty()
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

    createCountText(em, itemCount, itemInfo, yPos);
    auto shopItem = createItemPicture(em, itemInfo, yPos);
    createSellPriceText(em, itemInfo, yPos);
    createBuyPriceText(em, itemInfo, yPos);
    createSellText(em, shopItem, yPos);
    createBuyText(em, shopItem, yPos);

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

void updateRowHighlighting(EntityManager* em) {
    auto selectedItem = getSelectedItem(em);
    if (!selectedItem) return;

    auto background = em->getEntitiesWithComponent<ShopSelectedBackgroundComponent>()[0];
    auto pos = selectedItem->getComponent<PositionComponent>();
    background->getComponent<PositionComponent>()->setPos(pos->getX() - SHOP_COLUMN_WIDTH_MEDIUM, pos->getY());
}

void updateBuyText(EntityManager* em, Entity* selectedItem) {
    auto shopItem = selectedItem->getComponent<ShopItemComponent>();
    auto pos = selectedItem->getComponent<PositionComponent>();
    auto buyTexts = em->getEntitiesWithComponent<ShopItemBuyTextComponent>();
    for (auto entity : buyTexts) {
        auto textPos = entity->getComponent<PositionComponent>();
        if (textPos->getY() == pos->getY()) {
            entity->getComponent<TextComponent>()->setText(std::to_string(shopItem->getBuyCount()));
            return;
        }
    }
}

void updateSellText(EntityManager* em, Entity* selectedItem) {
    auto shopItem = selectedItem->getComponent<ShopItemComponent>();
    auto pos = selectedItem->getComponent<PositionComponent>();

    auto sellTexts = em->getEntitiesWithComponent<ShopItemSellTextComponent>();
    for (auto entity : sellTexts) {
        auto textPos = entity->getComponent<PositionComponent>();

        if (textPos->getY() == pos->getY()) {
            entity->getComponent<TextComponent>()->setText(std::to_string(shopItem->getSellCount()));
            return;
        }
    }
}

void updateShopRowValues(EntityManager* em) {
    auto selectedItem = getSelectedItem(em);
    if (!selectedItem) return;
    
    auto shopItem = selectedItem->getComponent<ShopItemComponent>();
    auto pos = selectedItem->getComponent<PositionComponent>();
    auto inventory = getPlayerInventory(em);

    if (!shopItem || !pos || !inventory) return;
    auto texts = em->getEntitiesWithComponent<TextComponent>();
    for (auto entity : texts) {
        if (!entity->getComponent<ShopTypeEntityComponent>()) continue;
        auto itemComponent = entity->getComponent<ShopItemChangeableComponent>();
        if (!itemComponent) continue;
        if (itemComponent->getName() == shopItem->getName()) {

            if (entity->getComponent<ShopItemAmountTextComponent>()) {
                entity->getComponent<TextComponent>()->setText(
                    std::to_string(getItemCount(inventory, shopItem->getName()) - shopItem->getSellCount())
                );

            }
            else if (entity->getComponent<ShopItemSellTextComponent>()) {
                entity->getComponent<TextComponent>()->setText(
                    std::to_string(shopItem->getSellCount())
                );
            }
            else if (entity->getComponent<ShopItemBuyTextComponent>()) {
                entity->getComponent<TextComponent>()->setText(
                    std::to_string(shopItem->getBuyCount())
                );
            }
        }
    }
}

void doTransaction(Controller* controller) {
    auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();

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
}

void createShopButtons(EntityManager* em) {
    createOkButton(em);
    createBackButton(em);
}

void createShop(EntityManager* em, RenderSystem* render) {
    createShopInterface(em);
    createPlayerAvatar(em);
    createShopAvatar(em);

    createShopItems(em);
    createShopButtons(em);
    createShopHeader(em);
}
