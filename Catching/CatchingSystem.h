#pragma once
#include "../Components/Components.h"
#include "../systems/EntityManager.h"
#include "../systems/InputSystem.h"
#include "../systems/RenderSystem.h"
#include "../const.h"
#include <iostream>

float timer = 0;
void initCatching(EntityManager* em, RenderSystem* render, GameState* state);
void arrowRotation(EntityManager* em, float dt);
void onSpacePressed(EntityManager* em, InputSystem* input, float dt);
void throwPokeball(EntityManager* em, float dt);
void pokeballCollision(EntityManager* em, RenderSystem* render, GameState* state);
void removeCatchingEntities(EntityManager* em, RenderSystem* render);

void initCatchingLocation(EntityManager* em);
void initCatchingArrow(EntityManager* em);
void initCatchingPokeball(EntityManager* em);
void initCatchingPokemon(EntityManager* em);

void handlePokeballPokemonCollision(EntityManager* em, RenderSystem* render, GameState* state);
void handlePokeballOutOfMap(EntityManager* em, RenderSystem* render, GameState* state);

void initHelpCatching(EntityManager* em);
void initHelpCatchingLayout(EntityManager* em);
void initHelpCatchingText(EntityManager* em);
void initHelpCatchingButton(EntityManager* em);
void handleHelpCatching(Controller* controller);
void clearHelpCatching(EntityManager* em, RenderSystem* render);
void closeHelpCatching(Controller* controller);

void initTimer(EntityManager* em);
void updateTimer(EntityManager* em, float dt);
bool checkTimer(EntityManager* em, RenderSystem* render, GameState* state);

void updateCatching(Controller* controller, float dt) {
	auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();

    updateTimer(em, dt);
    if (checkTimer(em, render, state)) {
        return;
    }
    arrowRotation(em, dt);
    onSpacePressed(em, input, dt);
    throwPokeball(em, dt);
    pokeballCollision(em, render, state);
}

bool hasPlayerSeenHelpCatching = false;

void initCatching(EntityManager* em, RenderSystem* render, GameState* state) {
    timer = 20;
    initTimer(em);
    initCatchingLocation(em);
    initCatchingArrow(em);
    initCatchingPokeball(em);
    initCatchingPokemon(em);
    if (!hasPlayerSeenHelpCatching) {
        initHelpCatching(em);
        hasPlayerSeenHelpCatching = true;
    } else {
        *state = GameState::Catching;
    }
    render->removeEntities();
    render->addEntities(em->getEntitiesWithComponent<CatchingTypeEntityComponent>());
}

void initTimer(EntityManager* em) {
    timer = TIMER_DURATION;
    auto timerText = em->createEntity();
    timerText->addComponent<TextComponent>(
        std::to_string(timer),
        TIMER_POS_X,
        TIMER_POS_Y,
        TIMER_TEXT_SIZE,
        sf::Color(246, 246, 246)
    );
    timerText->addComponent<PositionComponent>(TIMER_POS_X, TIMER_POS_Y);
    timerText->addComponent<RenderLayerComponent>(3);
    timerText->addComponent<CatchingTypeEntityComponent>();
    timerText->addComponent<TimerComponent>();
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
        WINDOW_WIDTH / 2.5 + 10,
        WINDOW_HEIGHT / 10 + 20
    );
    pokemon->getComponent<SizeComponent>()->setSize(
        WINDOW_WIDTH / 6,
        WINDOW_WIDTH / 6
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
    pokeball->addComponent<SizeComponent>(POKEBALL_WIDTH, POKEBALL_HEIGHT);
	pokeball->addComponent<ThrowablePokeballComponent>();
    pokeball->addComponent<SpeedComponent>(POKEBALL_SPEED);
	pokeball->addComponent<RenderLayerComponent>(1);
    pokeball->addComponent<CatchingTypeEntityComponent>();
    sf::Texture pokeballTexture;
    if (pokeballTexture.loadFromFile("../res/redPokeball(14x50).png")) {
        pokeball->addComponent<TextureComponent>(
			pokeballTexture,
			13, 
			12
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
			57, 
			162
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
    removeCatchingEntities(em, render);
    
    auto entities = em->getEntitiesWithComponent<GameTypeEntityComponent>();
    render->addEntities(entities);

    *state = GameState::Game;
}

void handlePokeballPokemonCollision(EntityManager* em, RenderSystem* render, GameState* state) {
    auto pokemon = em->getEntitiesWithComponent<AttackedPokemonComponent>()[0];
    pokemon->removeComponent<CatchingTypeEntityComponent>();
    pokemon->removeComponent<AttackedPokemonComponent>();
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

void updateTimer(EntityManager* em, float dt) {
    timer -= dt;
    auto timerText = em->getEntitiesWithComponent<TimerComponent>()[0];
    timerText->getComponent<TextComponent>()->setText(std::to_string(static_cast<int>(timer + 0.5f)));
}

bool checkTimer(EntityManager* em, RenderSystem* render, GameState* state) {
    if (timer <= 0) {
        timer = TIMER_DURATION;
        handlePokeballOutOfMap(em, render, state);
        return true;
    }
    return false;
}

void initHelpCatching(EntityManager* em) {
    initHelpCatchingLayout(em);
    initHelpCatchingText(em);
    initHelpCatchingButton(em);
}

void initHelpCatchingLayout(EntityManager* em) {
    auto helpCatchingLayout = em->createEntity();
    helpCatchingLayout->addComponent<PositionComponent>(
        ONBOARDING_LAYOUT_POSITION_X,
        ONBOARDING_LAYOUT_POSITION_Y
    );
    helpCatchingLayout->addComponent<SizeComponent>(
        ONBOARDING_LAYOUT_WIDTH,
        ONBOARDING_LAYOUT_HEIGHT
    );
    helpCatchingLayout->addComponent<RenderLayerComponent>(4);
    helpCatchingLayout->addComponent<HelpCatchingComponent>();
    helpCatchingLayout->addComponent<CatchingTypeEntityComponent>();
    sf::Texture helpCatchingLayoutTexture;
    if (helpCatchingLayoutTexture.loadFromFile("../res/background_menu(62x46).png")) {
        helpCatchingLayout->addComponent<TextureComponent>(
            helpCatchingLayoutTexture,
            62,
            46
        );
    }
}

void initHelpCatchingText(EntityManager* em) {
    auto helpCatchingText = em->createEntity();
    helpCatchingText->addComponent<HelpCatchingComponent>();
    helpCatchingText->addComponent<PositionComponent>(
        ONBOARDING_LAYOUT_POSITION_X,
        ONBOARDING_LAYOUT_POSITION_Y
    );
    helpCatchingText->addComponent<SizeComponent>(
        ONBOARDING_LAYOUT_WIDTH,
        ONBOARDING_LAYOUT_HEIGHT
    );
    helpCatchingText->addComponent<RenderLayerComponent>(5);
    helpCatchingText->addComponent<HelpCatchingComponent>();
    helpCatchingText->addComponent<CatchingTypeEntityComponent>();
    helpCatchingText->addComponent<TextComponent>(
        HELP_CATCHING_TEXT_VALUE,
        ONBOARDING_TEXT_X,
        ONBOARDING_TEXT_Y,
        ONBOARDING_TEXT_SIZE,
        sf::Color(68, 68, 68)
    );
}

void initHelpCatchingButton(EntityManager* em) {
    auto helpCatchingButton = em->createEntity();
    helpCatchingButton->addComponent<HelpCatchingComponent>();
    helpCatchingButton->addComponent<HelpCatchingButtonComponent>();
    helpCatchingButton->addComponent<PositionComponent>(
        ONBOARDING_BUTTON_X,
        ONBOARDING_BUTTON_Y
    );
    helpCatchingButton->addComponent<SizeComponent>(
        ONBOARDING_BUTTON_WIDTH,
        ONBOARDING_BUTTON_HEIGHT
    );
    helpCatchingButton->addComponent<RenderLayerComponent>(5);
    helpCatchingButton->addComponent<CatchingTypeEntityComponent>();
    sf::Texture helpCatchingButtonTexture;
    if (helpCatchingButtonTexture.loadFromFile("../res/backButton(32x13).png")) {
        helpCatchingButton->addComponent<TextureComponent>(
            helpCatchingButtonTexture,
            32,
            13
        );
    }
}

void handleHelpCatching(Controller* controller) {
    auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();

	if (input->hasMouseClick()) {
		auto button = em->getEntitiesWithComponent<HelpCatchingButtonComponent>()[0];
		if (isClickOnEntity(input->getMouseClick(), button)) {
			closeHelpCatching(controller);
			return;
		}
	}

	auto keys = input->getPressedKeys();
	if (
		std::find(keys.begin(), keys.end(), sf::Keyboard::Escape) != keys.end() || 
		std::find(keys.begin(), keys.end(), sf::Keyboard::Enter) != keys.end()
	) {
		closeHelpCatching(controller);
	}
}

void clearHelpCatching(EntityManager* em, RenderSystem* render) {
	auto helpCatching = em->getEntitiesWithComponent<HelpCatchingComponent>();
	for (auto entity : helpCatching) {
		render->removeEntity(entity->getId());
		em->removeEntity(entity);
	}
}

void closeHelpCatching(Controller* controller) {
	auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();
	clearHelpCatching(em, render);
	*state = GameState::Catching;
	input->clear();
}
