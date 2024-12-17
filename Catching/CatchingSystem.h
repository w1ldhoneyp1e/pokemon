#pragma once
#include "../Components/Components.h"
#include "../systems/EntityManager.h"
#include "../systems/InputSystem.h"
#include "../systems/RenderSystem.h"
#include "../const.h"
#include <iostream>

void initCatching(EntityManager* em, RenderSystem* render);
void arrowRotation(EntityManager* em, float dt);
void onSpacePressed(EntityManager* em, InputSystem* input, float dt);
void throwPokeball(EntityManager* em, float dt);
void pokeballCollision(EntityManager* em, RenderSystem* render, GameState* state);
void removeCatchingEntities(EntityManager* em, RenderSystem* render);

void setPokemonPropsIfLose(Entity *pokemon);

void initCatchingLocation(EntityManager* em);
void initCatchingArrow(EntityManager* em);
void initCatchingPokeball(EntityManager* em);
void initCatchingPokemon(EntityManager* em);

void handlePokeballPokemonCollision(EntityManager* em, RenderSystem* render, GameState* state);
void handlePokeballOutOfMap(EntityManager* em, RenderSystem* render, GameState* state);

void updateCatching(Controller* controller, float dt) {
	auto [input, em, render, state] = controller->getAll();

    arrowRotation(em, dt);
    onSpacePressed(em, input, dt);
    throwPokeball(em, dt);
    pokeballCollision(em, render, state);
}

void initCatching(EntityManager* em, RenderSystem* render) {
    // Локация ловли
    initCatchingLocation(em);
    // Стрелка
    initCatchingArrow(em);
    // Покебол
    initCatchingPokeball(em);
    // Получить покемона, присвоить позицию и размеры
    initCatchingPokemon(em);

    render->removeEntities();
    render->addEntities(em->getEntitiesWithComponent<CatchingTypeEntityComponent>());
}

void arrowRotation(EntityManager* em, float dt) {
    auto arrow = em->getEntitiesWithComponent<CatchingArrowComponent>()[0];
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

void initCatchingPokemon(EntityManager* em) {
    auto pokemon = em->getEntitiesWithComponent<AttackedPokemonComponent>()[0];
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


void initCatchingPokeball(EntityManager* em) {
    auto pokeball = em->createEntity();
    pokeball->addComponent<PokeballComponent>();
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

void initCatchingLocation(EntityManager* em) {
    auto catchingLocation = em->createEntity();
    catchingLocation->addComponent<CatchingLocationComponent>();
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

void initCatchingArrow(EntityManager* em) {
    auto catchingArrow = em->createEntity();
    catchingArrow->addComponent<CatchingArrowComponent>();
    catchingArrow->addComponent<PositionComponent>(
        ARROW_POS_X, 
        ARROW_POS_Y
    );
    catchingArrow->addComponent<SizeComponent>(ARROW_WIDTH, ARROW_HEIGHT);
	catchingArrow->addComponent<RotationComponent>(ARROW_SPEED, ARROW_WIDTH / 2, ARROW_HEIGHT + ARROW_POKEBALL_GAP + POKEBALL_HEIGHT * 5 / 2);
	catchingArrow->addComponent<RenderLayerComponent>(2);
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

void onSpacePressed(EntityManager* em, InputSystem* input, float dt) {
    auto throwAbility = em->getEntitiesWithComponent<PokeballComponent>()[0]->getComponent<ThrowablePokeballComponent>();
    if (throwAbility->isThrown()) return;
    auto keys = input->getPressedKeys();
    if (keys.empty()) return;
    if (std::find(keys.begin(), keys.end(), sf::Keyboard::Space) != keys.end()
        || std::find(keys.begin(), keys.end(), sf::Keyboard::Enter) != keys.end()
    ) {
        auto arrowAngle = em->getEntitiesWithComponent<CatchingArrowComponent>()[0]->getComponent<RotationComponent>()->angle;
        throwAbility->setAngle(arrowAngle - PI / 2);
        throwAbility->setThrown();
        keys.clear();
    }
}

void throwPokeball(EntityManager* em, float dt) {
    auto pokeball = em->getEntitiesWithComponent<PokeballComponent>()[0];
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

void pokeballCollision(EntityManager* em, RenderSystem* render, GameState* state) {
    auto pokemon = em->getEntitiesWithComponent<AttackedPokemonComponent>()[0];
    auto pokeball = em->getEntitiesWithComponent<PokeballComponent>()[0];
    if (isCollision(pokeball, pokemon)) {
        handlePokeballPokemonCollision(em, render, state);
    }
    auto location = em->getEntitiesWithComponent<CatchingLocationComponent>().empty()
        ? nullptr
        : em->getEntitiesWithComponent<CatchingLocationComponent>()[0];
    if (pokeball != nullptr && location != nullptr && !isCollision(pokeball, location)) {
        handlePokeballOutOfMap(em, render, state);
    }
}

void handlePokeballOutOfMap(EntityManager* em, RenderSystem* render, GameState* state) {
    auto pokeball = em->getEntitiesWithComponent<PokeballComponent>()[0];
    auto location = em->getEntitiesWithComponent<CatchingLocationComponent>()[0];
    auto pokemon = em->getEntitiesWithComponent<AttackedPokemonComponent>()[0];

    pokemon->removeComponent<CatchingTypeEntityComponent>();
    setPokemonPropsIfLose(pokemon);
    removeCatchingEntities(em, render);
    
    auto entities = em->getEntitiesWithComponent<GameTypeEntityComponent>();
    render->addEntities(entities);

    *state = GameState::Game;
}

void handlePokeballPokemonCollision(EntityManager* em, RenderSystem* render, GameState* state) {
    auto pokemon = em->getEntitiesWithComponent<AttackedPokemonComponent>()[0];
    pokemon->removeComponent<CatchingTypeEntityComponent>();
    auto player = em->getEntitiesWithComponent<PlayerControlComponent>()[0];
    auto inventory = player->getComponent<PlayersInventoryComponent>();
    inventory->addPokemon(pokemon->getId());
    pokemon->getComponent<PokemonComponent>()->setCollected(true);
    
    removeCatchingEntities(em, render);

    auto entities = em->getEntitiesWithComponent<GameTypeEntityComponent>();
    render->addEntities(entities);

    *state = GameState::Game;
}


void removeCatchingEntities(EntityManager* em, RenderSystem* render) {
    auto entities = em->getEntitiesWithComponent<CatchingTypeEntityComponent>();
    render->removeEntities();
    for (auto entity : entities) {
        em->removeEntity(entity);
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