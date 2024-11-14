#include "WindowManager.h"
#include "EntityManager.h"
#include "RenderSystem.h"
#include "InputSystem.h"
#include "../Player/PlayerMovementSystem.h"
#include "ClickHandler.h"
#include "../Entity.h"

class State {
public:
    virtual ~State() = default;
    virtual void handleInput(InputSystem* inputSystem) = 0;
    virtual void update(InputSystem* inputSystem) = 0;
    virtual void render(RenderSystem* renderSystem, EntityManager* entityManager) = 0;
};

class StateManager {
public:
    StateManager() : currentState(nullptr) {}

    // Установить текущее состояние
    void setMenu() {
        if (currentState != nullptr) {
            delete currentState;
        }
        currentState = menuState;
    }

    void setGame() {
        if (currentState != nullptr) {
            delete currentState;
        }
        currentState = gameState;
    }

    // Обработка входных данных, обновление и рендеринг
    void handleInput(InputSystem* inputSystem) {
        if (currentState) {
            currentState->handleInput(inputSystem);
        }
    }

    void update(InputSystem* inputSystem) {
        if (currentState) {
            currentState->update(inputSystem);
        }
    }

    void render(RenderSystem* renderSystem, EntityManager* entityManager) {
        if (currentState) {
            currentState->render(renderSystem, entityManager);
        }
    }

    void addMenu(State* menu) {
        menuState = menu;
    }
    void addGame(State* game) {
        gameState = game;
    }

    // Деструктор для освобождения памяти
    ~StateManager() {
        delete currentState;
    }

private:
    State* currentState;  // Указатель на текущее состояние
    State* gameState;  // Указатель на текущее состояние
    State* menuState;  // Указатель на текущее состояние
};

class GameState : public State {
public:
    GameState(StateManager* stateManager, EntityManager* entityManager, RenderSystem* renderSystem) 
    : stateManager(stateManager), entityManager(entityManager), renderSystem(renderSystem) {
        // Создаем объекты кнопок и заднего фона, передаем их в RenderSystem
        std::cout << "zashli" << std::endl;
        auto player = entityManager->createEntity();
        player->addComponent<PositionComponent>(300, 300);
        player->addComponent<SizeComponent>(48, 96);
        player->addComponent<SpeedComponent>(0.25);
        sf::Texture texture;
        if (texture.loadFromFile("../res/Ash_sprite(16x32).png")) {
            player->addComponent<TextureComponent>(texture, 16, 32);
        }
	    renderSystem->addEntity(player);
        thisPlayer = player;
    }

    void handleInput(InputSystem* inputSystem) override {
    }

    void update(InputSystem* inputSystem) override {
        auto keys = inputSystem->getPressedKeys();
        if (keys.empty()) return;
        playerMovementSystem(thisPlayer, keys.back());
    }

    void render(RenderSystem* renderSystem, EntityManager* entityManager) override {
        renderSystem->render();
    }
private:
    StateManager* stateManager;
    EntityManager* entityManager;
    RenderSystem* renderSystem;
    Entity* thisPlayer;
};

class MenuState : public State {
public:
    MenuState(StateManager* stateManager, EntityManager* entityManager, RenderSystem* renderSystem) 
    : stateManager(stateManager), entityManager(entityManager), renderSystem(renderSystem) {
        // Создаем объекты кнопок и заднего фона, передаем их в RenderSystem
        auto startButton = entityManager->createEntity();
        startButton->addComponent<PositionComponent>(400-108, 200-40);
        startButton->addComponent<SizeComponent>(216, 80);
        sf::Texture texture;
        if (texture.loadFromFile("../res/button(54x20).png")) {
            startButton->addComponent<TextureComponent>(texture, 54, 20);
        }
        button = startButton;
        renderSystem->addEntity(startButton);
    }

    ~MenuState() {
    // Удаляем кнопку из RenderSystem, если она была добавлена
        if (button != nullptr) {
            renderSystem->removeEntity(button->id);
        }
    }

    void handleInput(InputSystem* inputSystem) override {
        // Хз, что вообще тут будет
        if (isClickOnEntity(inputSystem->getMouseClick(), button)) {
            stateManager->setGame();
        }
    }

    void update(InputSystem* inputSystem) override {
        // Нажали на кнопку "играть" -> меняем стейт на гейм
    }

    void render(RenderSystem* renderSystem, EntityManager* entityManager) override {
        // Рисуем все объекты
        renderSystem->render();
    }
private:
    StateManager* stateManager;
    EntityManager* entityManager;
    RenderSystem* renderSystem;
    Entity* button;
};