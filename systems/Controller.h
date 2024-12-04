#pragma once
#include "./RenderSystem.h"
#include "./InputSystem.h"
#include "./EntityManager.h"
#include "../GameState.h"

class Controller {
private:
    EntityManager* entityManager;
    InputSystem* inputSystem;
    RenderSystem* renderSystem;
    GameState* state;

public:
    Controller(EntityManager* em, InputSystem* is, RenderSystem* rs, GameState* st)
        : entityManager(em), inputSystem(is), renderSystem(rs), state(st) {}

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

	struct Systems {
        InputSystem* input;
        EntityManager* entityManager;
        RenderSystem* renderSystem;
        GameState* state;
    };

    Systems getAll() {
        return {inputSystem, entityManager, renderSystem, state};
    }
};