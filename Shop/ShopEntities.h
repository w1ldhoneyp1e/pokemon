#pragma once
#include "../const.h"
#include "../systems/Controller.h"
#include "../systems/CollisionSystem.h"

struct ShopItemInfo {
    std::string name;
    int price;
    std::string texturePath;
    bool isPokemon;
};

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
        text->addComponent<PositionComponent>(
            xOffset,
            SHOP_ITEM_POS_Y - SHOP_HEADER_HEIGHT
        );
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

void createCountText(EntityManager* em, int itemCount, ShopItemInfo itemInfo, float yPos) {
	auto countText = em->createEntity();
    countText->addComponent<TextComponent>(
        std::to_string(itemCount),
        SHOW_HEADER_START_X,
        yPos + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 8,
        SHOP_TEXT_HEIGHT / 2
    );
    countText->addComponent<ShopTypeEntityComponent>();
    countText->addComponent<ShopItemAmountTextComponent>();
    countText->addComponent<ShopItemChangeableComponent>(itemInfo.name);
    countText->addComponent<PositionComponent>(
        SHOW_HEADER_START_X,
        yPos + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 8
    );
    countText->addComponent<RenderLayerComponent>(7);
}

ShopItemComponent* createItemPicture(EntityManager* em, ShopItemInfo itemInfo, float yPos) {
	auto item = em->createEntity();
    item->addComponent<ShopItemComponent>(itemInfo.name, itemInfo.price, itemInfo.isPokemon);
    auto shopItem = item->getComponent<ShopItemComponent>();


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

    return shopItem;
}

void createNameText(EntityManager* em, ShopItemInfo itemInfo, float yPos) {
	auto nameText = em->createEntity();
    nameText->addComponent<TextComponent>(
        itemInfo.name,
        SHOW_HEADER_START_X + SHOP_COLUMN_WIDTH_MEDIUM + SHOP_ITEM_WIDTH + 10,
        yPos + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 8,
        SHOP_TEXT_HEIGHT / 2
    );
    nameText->addComponent<ShopTypeEntityComponent>();
    nameText->addComponent<PositionComponent>(
        SHOW_HEADER_START_X + SHOP_COLUMN_WIDTH_MEDIUM + SHOP_ITEM_WIDTH + 10,
        yPos + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 8
    );
    nameText->addComponent<RenderLayerComponent>(7);
}

void createSellPriceText(EntityManager* em, ShopItemInfo itemInfo, float yPos) {
	auto sellPriceText = em->createEntity();
    sellPriceText->addComponent<TextComponent>(
        std::to_string(itemInfo.price / 2),
        SHOW_HEADER_START_X + SHOP_COLUMN_WIDTH_MEDIUM + SHOP_COLUMN_WIDTH_BIG,
        yPos + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 8,
        SHOP_TEXT_HEIGHT / 2

    );
    sellPriceText->addComponent<ShopTypeEntityComponent>();
    sellPriceText->addComponent<PositionComponent>(
        SHOW_HEADER_START_X + SHOP_COLUMN_WIDTH_MEDIUM + SHOP_COLUMN_WIDTH_BIG,
        yPos + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 8
    );
    sellPriceText->addComponent<RenderLayerComponent>(7);
}

void createBuyPriceText(EntityManager* em, ShopItemInfo itemInfo, float yPos) {
	auto buyPriceText = em->createEntity();
    buyPriceText->addComponent<TextComponent>(
        std::to_string(itemInfo.price),
        SHOW_HEADER_START_X + SHOP_COLUMN_WIDTH_BIG + SHOP_COLUMN_WIDTH_MEDIUM * 2,
        yPos + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 8,
        SHOP_TEXT_HEIGHT / 2

    );
    buyPriceText->addComponent<ShopTypeEntityComponent>();
    buyPriceText->addComponent<PositionComponent>(
        SHOW_HEADER_START_X + SHOP_COLUMN_WIDTH_BIG + SHOP_COLUMN_WIDTH_MEDIUM * 2,
        yPos + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 8
    );
    buyPriceText->addComponent<RenderLayerComponent>(7);
}

void createSellText(EntityManager* em, ShopItemComponent* shopItem, float yPos) {
	auto sellText = em->createEntity();
    sellText->addComponent<TextComponent>(
        std::to_string(shopItem->getSellCount()),
        SHOW_HEADER_START_X + SHOP_COLUMN_WIDTH_BIG + SHOP_COLUMN_WIDTH_MEDIUM * 3,
        yPos + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 8,
        SHOP_TEXT_HEIGHT / 2
    );
    sellText->addComponent<PositionComponent>(
        SHOW_HEADER_START_X + SHOP_COLUMN_WIDTH_BIG + SHOP_COLUMN_WIDTH_MEDIUM * 3,
        yPos + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 8
    );
    sellText->addComponent<ShopTypeEntityComponent>();
    sellText->addComponent<ShopItemSellTextComponent>();
    sellText->addComponent<ShopItemChangeableComponent>(shopItem->getName());
    sellText->addComponent<RenderLayerComponent>(7);
}

void createBuyText(EntityManager* em, ShopItemComponent* shopItem, float yPos) {
    auto buyText = em->createEntity();
    buyText->addComponent<TextComponent>(
        std::to_string(shopItem->getBuyCount()),
        SHOW_HEADER_START_X + SHOP_COLUMN_WIDTH_BIG + SHOP_COLUMN_WIDTH_MEDIUM * 3 + SHOP_COLUMN_WIDTH_SMALL,
        yPos + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 8,
        SHOP_TEXT_HEIGHT / 2
    );
    buyText->addComponent<PositionComponent>(
        SHOW_HEADER_START_X + SHOP_COLUMN_WIDTH_BIG + SHOP_COLUMN_WIDTH_MEDIUM * 3 + SHOP_COLUMN_WIDTH_SMALL,
        yPos + SHOP_ITEM_HEIGHT / 2 - SHOP_TEXT_HEIGHT / 8
    );
    buyText->addComponent<ShopTypeEntityComponent>();
    buyText->addComponent<ShopItemBuyTextComponent>();
    buyText->addComponent<ShopItemChangeableComponent>(shopItem->getName());
    buyText->addComponent<RenderLayerComponent>(7);
}
