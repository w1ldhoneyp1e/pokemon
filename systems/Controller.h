#pragma once
#include "./RenderSystem.h"
#include "./InputSystem.h"
#include "./EntityManager.h"
#include "../GameState.h"
#include "./BattleSystem.h"
#include "./CollisionMap.h"
#include "../Location/LocationType.h"

class Controller {
private:
    EntityManager* entityManager;
    InputSystem* inputSystem;
    RenderSystem* renderSystem;
    GameState* state;
    BattleContext* battleContext;
    std::unordered_map<LocationType, CollisionMap>* maps;
    LocationType currentLocation;

public:
    Controller(EntityManager* em, InputSystem* is, RenderSystem* rs, GameState* st, BattleContext *ctx, std::unordered_map<LocationType, CollisionMap>* maps, LocationType currentLocation)
        : entityManager(em), inputSystem(is), renderSystem(rs), state(st), battleContext(ctx), maps(maps), currentLocation(currentLocation) {}

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

    LocationType getCurrLocation() {
        return currentLocation;
    }

    void setCurrLocation(LocationType loc) {
        currentLocation = loc;
    }

    std::unordered_map<LocationType, CollisionMap>* getCollisionMaps() {
        return maps;
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
        std::unordered_map<LocationType, CollisionMap>* maps;
        LocationType currentLocation;
    };

    Systems getAll() {
        return {inputSystem, entityManager, renderSystem, state, battleContext, maps, currentLocation};
    }
};