#pragma once
#include "../Components.h"
#include "../systems/EntityManager.h"
#include "../systems/InputSystem.h"
#include "../systems/RenderSystem.h"
#include "../const.h"
#include <iostream>

void initCatching(EntityManager* entityManager, RenderSystem* renderSystem);
void arrowRotation(EntityManager* entityManager, float dt);

void updateCatching(
	EntityManager* entityManager,
    float dt
) {
    arrowRotation(entityManager, dt);
	// Поворот стрелки
    // Обработка нажатия пробела
        // Создать покебол перед стрелкой и выкинуть по направлению стрелки
    // Обработка коллизии покебола с покемоном
}

void initCatching(EntityManager* entityManager, RenderSystem* renderSystem) {
    // Локация ловли
    auto catchingLocation = entityManager->createEntity("catchingLocation");
    catchingLocation->addComponent<PositionComponent>(0, 0);
    catchingLocation->addComponent<SizeComponent>(WINDOW_WIDTH, WINDOW_HEIGHT);
	catchingLocation->addComponent<RenderLayerComponent>(0);
    catchingLocation->addComponent<CatchingTypeEntityComponent>();
    sf::Texture catchingLocationTexture;
    if (catchingLocationTexture.loadFromFile("../res/beach(383x640).png")) {
        catchingLocation->addComponent<TextureComponent>(
			catchingLocationTexture,
			CATCHING_LOCATION_WIDTH, 
			CATCHING_LOCATION_HEIGHT
		);
    }

    // Стрелка
    auto catchingArrow = entityManager->createEntity("catchingArrow");
    catchingArrow->addComponent<PositionComponent>(
        ARROW_POS_X, 
        ARROW_POS_Y
    );
    catchingArrow->addComponent<SizeComponent>(ARROW_WIDTH, ARROW_HEIGHT);
	catchingArrow->addComponent<RotationComponent>(ARROW_SPEED, ARROW_WIDTH / 2, ARROW_HEIGHT + ARROW_POKEBALL_GAP + POKEBALL_HEIGHT * 5 / 2);
	catchingArrow->addComponent<RenderLayerComponent>(1);
    catchingArrow->addComponent<CatchingTypeEntityComponent>();
    sf::Texture catchingArrowTexture;
    if (catchingArrowTexture.loadFromFile("../res/arrow(57x162).png")) {
        catchingArrow->addComponent<TextureComponent>(
			catchingArrowTexture,
			ARROW_WIDTH, 
			ARROW_HEIGHT
		);
    }
    catchingArrow->getComponent<TextureComponent>()->sprite.setOrigin(ARROW_WIDTH / 2, ARROW_HEIGHT);

    // Покебол
    auto pokeball = entityManager->createEntity("pokeball");
    pokeball->addComponent<PositionComponent>(
        POKEBALL_POS_X, 
        POKEBALL_POS_Y
    );
    pokeball->addComponent<SizeComponent>(POKEBALL_WIDTH * 5, POKEBALL_HEIGHT * 5);
	pokeball->addComponent<RenderLayerComponent>(1);
    pokeball->addComponent<CatchingTypeEntityComponent>();
    sf::Texture pokeballTexture;
    if (pokeballTexture.loadFromFile("../res/redPokeball(14x50).png")) {
        pokeball->addComponent<TextureComponent>(
			pokeballTexture,
			POKEBALL_WIDTH, 
			POKEBALL_HEIGHT
		);
    }
    pokeball->getComponent<TextureComponent>()->sprite.setOrigin(POKEBALL_WIDTH / 2, POKEBALL_HEIGHT / 2);

    // Получить покемона, присвоить позицию и размеры
    auto pokemon = entityManager->getEntitiesWithComponent<AttackedPokemonComponent>()[0];
    pokemon->getComponent<PositionComponent>()->setPos(
        WINDOW_WIDTH / 2.5,
        WINDOW_HEIGHT / 10
    );
    pokemon->getComponent<SizeComponent>()->setSize(
        WINDOW_WIDTH / 5,
        WINDOW_WIDTH / 5
    );
    pokemon->addComponent<CatchingTypeEntityComponent>();

    renderSystem->removeEntities();
    renderSystem->addEntities(entityManager->getEntitiesWithComponent<CatchingTypeEntityComponent>());
}

void arrowRotation(EntityManager* entityManager, float dt) {
    auto arrow = entityManager->getEntity("catchingArrow");
    auto rotation = arrow->getComponent<RotationComponent>();
    auto position = arrow->getComponent<PositionComponent>();

    rotation->angle += dt * rotation->speed;
    rotation->angle = std::fmod(rotation->angle, PI);

    

    if (rotation->angle < ARROW_MIN_ANGLE) {
        rotation->angle = ARROW_MIN_ANGLE;
        rotation->speed *= -1;
    } else if (rotation->angle > ARROW_MAX_ANGLE) {
        rotation->angle = ARROW_MAX_ANGLE;
        rotation->speed *= -1;
    }

    float x = ARROW_POS_X - ARROW_RADIUS * cosf(rotation->angle + PI / 2);
    float y = ARROW_POS_Y - ARROW_RADIUS * sinf(rotation->angle + PI / 2);

    // Устанавливаем новую позицию стрелки
    position->setPos(x, y);
}