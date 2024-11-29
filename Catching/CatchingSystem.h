#pragma once
#include "../Components.h"
#include "../systems/EntityManager.h"
#include "../systems/InputSystem.h"
#include "../systems/RenderSystem.h"
#include "../const.h"
#include <iostream>

void initCatching(EntityManager* entityManager, RenderSystem* renderSystem);
void arrowRotation(EntityManager* entityManager, float dt);
void onSpacePressed(EntityManager* entityManager, InputSystem* inputSystem, float dt);
void throwPokeball(EntityManager* entityManager, float dt);
void pokeballCollision(EntityManager* entityManager, RenderSystem* renderSystem, GameState* state);
void removeCatchingEntities(EntityManager* entityManager, RenderSystem* renderSystem);

void initCatchingLocation(EntityManager* entityManager);
void initCatchingArrow(EntityManager* entityManager);
void initCatchingPokeball(EntityManager* entityManager);
void initCatchingPokemon(EntityManager* entityManager);

void handlePokeballPokemonCollision(EntityManager* entityManager, RenderSystem* renderSystem, GameState* state);
void handlePokeballOutOfMap(EntityManager* entityManager, RenderSystem* renderSystem, GameState* state);

void updateCatching(
	EntityManager* entityManager,
    InputSystem* inputSystem,
    RenderSystem* renderSystem,
    GameState* state,
    float dt
) {
    arrowRotation(entityManager, dt);
    onSpacePressed(entityManager, inputSystem, dt);
    throwPokeball(entityManager, dt);
    pokeballCollision(entityManager, renderSystem, state);
    // Обработка коллизии покебола с покемоном
}

void initCatching(EntityManager* entityManager, RenderSystem* renderSystem) {
    // TODO:  функцию сократить на 30 строк кода
    // Локация ловли
    initCatchingLocation(entityManager);
    // Стрелка
    initCatchingArrow(entityManager);
    // Покебол
    initCatchingPokeball(entityManager);
    // Получить покемона, присвоить позицию и размеры
    initCatchingPokemon(entityManager);

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

    position->setPos(x, y);
}

void initCatchingPokemon(EntityManager* entityManager) {
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
}


void initCatchingPokeball(EntityManager* entityManager) {
    auto pokeball = entityManager->createEntity("pokeball");
    pokeball->addComponent<PositionComponent>(
        POKEBALL_POS_X, 
        POKEBALL_POS_Y
    );
    pokeball->addComponent<SizeComponent>(POKEBALL_WIDTH * 5, POKEBALL_HEIGHT * 5);
	pokeball->addComponent<ThrowablePokeballComponent>();
    pokeball->addComponent<SpeedComponent>(POKEBALL_SPEED);
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
}

void initCatchingLocation(EntityManager* entityManager) {
    initCatchingLocation(entityManager);
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
}

void initCatchingArrow(EntityManager* entityManager) {
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
}

void onSpacePressed(EntityManager* entityManager, InputSystem* inputSystem, float dt) {
    auto throwAbility = entityManager->getEntity("pokeball")->getComponent<ThrowablePokeballComponent>();
    if (throwAbility->isThrown()) return;
    auto keys = inputSystem->getPressedKeys();
    if (keys.empty()) return;
    if (std::find(keys.begin(), keys.end(), sf::Keyboard::Space) != keys.end()
        || std::find(keys.begin(), keys.end(), sf::Keyboard::Enter) != keys.end()
    ) {
        auto arrowAngle = entityManager->getEntity("catchingArrow")->getComponent<RotationComponent>()->angle;
        throwAbility->setAngle(arrowAngle - PI / 2);
        throwAbility->setThrown();
        keys.clear();
    }
}

void throwPokeball(EntityManager* entityManager, float dt) {
    auto pokeball = entityManager->getEntity("pokeball");
    auto rotation = pokeball->getComponent<ThrowablePokeballComponent>();
    if (!rotation->isThrown()) {
        return;
    }
    auto speed = pokeball->getComponent<SpeedComponent>()->speed;
    auto position = pokeball->getComponent<PositionComponent>();
    position->setPos(
        position->getX() + dt * speed * cosf(rotation->getAngle()),
        position->getY() + dt * speed * sinf(rotation->getAngle())
    );
}

void pokeballCollision(EntityManager* entityManager, RenderSystem* renderSystem, GameState* state) {
    //  TODO: сократить минимум в 2 раза
    auto pokemon = entityManager->getEntitiesWithComponent<AttackedPokemonComponent>()[0];
    auto pokeball = entityManager->getEntity("pokeball");
    if (isCollision(pokeball, pokemon)) {
        handlePokeballPokemonCollision(entityManager, renderSystem, state);
    }
    auto location = entityManager->getEntity("catchingLocation");
    if (pokeball != nullptr && location != nullptr && !isCollision(pokeball, location)) {
        handlePokeballOutOfMap(entityManager, renderSystem, state);
    }
}

void handlePokeballOutOfMap(EntityManager* entityManager, RenderSystem* renderSystem, GameState* state) {
    auto pokeball = entityManager->getEntity("pokeball");
    auto location = entityManager->getEntity("catchingLocation");
    auto pokemon = entityManager->getEntitiesWithComponent<AttackedPokemonComponent>()[0];

    pokemon->removeComponent<CatchingTypeEntityComponent>();
    setPokemonPropsIfLose(pokemon);
    removeCatchingEntities(entityManager, renderSystem);
    
    auto entities = entityManager->getEntitiesWithComponent<GameTypeEntityComponent>();
    renderSystem->addEntities(entities);

    *state = GameState::Game;
}

void handlePokeballPokemonCollision(EntityManager* entityManager, RenderSystem* renderSystem, GameState* state) {
    auto pokemon = entityManager->getEntitiesWithComponent<AttackedPokemonComponent>()[0];
    pokemon->removeComponent<CatchingTypeEntityComponent>();
    auto player = entityManager->getEntity("player");
    auto inventory = player->getComponent<PlayersInventoryComponent>();
    inventory->addPokemon(pokemon->getId());
    pokemon->getComponent<PokemonComponent>()->setCollected(true);
    
    removeCatchingEntities(entityManager, renderSystem);

    auto entities = entityManager->getEntitiesWithComponent<GameTypeEntityComponent>();
    renderSystem->addEntities(entities);

    *state = GameState::Game;
}


void removeCatchingEntities(EntityManager* entityManager, RenderSystem* renderSystem) {
    auto entities = entityManager->getEntitiesWithComponent<CatchingTypeEntityComponent>();
    renderSystem->removeEntities();
    for (auto entity : entities) {
        entityManager->removeEntity(entity->getId());
    }
}

void setPokemonPropsIfLose(Entity *pokemon) {
    pokemon->getComponent<PositionComponent>()->setPos(
        BULBASOUR_POSITION_X,
        BULBASOUR_POSITION_Y
    );
    pokemon->getComponent<SizeComponent>()->setSize(
        POKEMON_INVENTORY_WIDTH,
        POKEMON_INVENTORY_HEIGHT
    );
    pokemon->addComponent<GameTypeEntityComponent>();
}