#pragma once
#include "./RenderSystem.h"
#include "./InputSystem.h"
#include "./EntityManager.h"
#include "../GameState.h"
#include "./BattleSystem.h"

class Controller {
private:
    EntityManager* entityManager;
    InputSystem* inputSystem;
    RenderSystem* renderSystem;
    GameState* state;
    BattleContext* battleContext;

public:
    Controller(EntityManager* em, InputSystem* is, RenderSystem* rs, GameState* st, BattleContext *ctx)
        : entityManager(em), inputSystem(is), renderSystem(rs), state(st), battleContext(ctx) {}

    ~Controller() {
        delete entityManager;
        delete inputSystem;
        delete renderSystem;
        delete state;
    }

    EntityManager* getEntityManager() {
        return entityManager;
    }

    InputSystem* getInputSystem() {
        return inputSystem;
    }

    RenderSystem* getRenderSystem() {
        return renderSystem;
    }

    GameState* getState() {
        return state;
    }

    BattleContext* getBattleContext() {
        return battleContext;
    }

    void setBattleContext(BattleContext *ctx) {
        battleContext = ctx;
    }

	struct Systems {
        InputSystem* input;
        EntityManager* entityManager;
        RenderSystem* renderSystem;
        GameState* state;
        BattleContext* battleContext;
    };

    Systems getAll() {
        return {inputSystem, entityManager, renderSystem, state, battleContext};
    }
};