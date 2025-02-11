#pragma once
#include "../systems/Controller.h"
#include "../systems/EntityManager.h"
#include "../systems/RenderSystem.h"
#include "../systems/ClickHandler.h"
#include "../Game/GameSystem.h"
#include "../TheEnd/TheEnd.h"
#include "../const.h"
#include <random>
#include <thread>
#include <chrono>
#include <ctime>

void initBattle(EntityManager *em, RenderSystem *render, BattleContext *ctx);
void initBattleLocation(EntityManager *em);
void initArrows(EntityManager *em);
void initMyPokemon(EntityManager *em);
void initEnemyPokemon(EntityManager *em);
void initPotionButton(EntityManager *em);
void initPotionButtonTexture(EntityManager *em);
void initPotionButtonTriggerKey(EntityManager *em);
void initPotionButtonAmount(EntityManager *em);
Entity *getEnemy(EntityManager *em);
Entity *getPokemon(EntityManager *em, Entity *enemy);


void onArrowClick(EntityManager *em, InputSystem *input, BattleContext *ctx);
void damageHandler(EntityManager *em, BattleContext *ctx);
void enemyAttack(EntityManager *em, BattleContext *ctx);
int getRandomDamage(int minDamage, int maxDamage);

void onClickArrow(EntityManager *em, InputSystem *input, BattleContext *ctx);
void onUsePotion(EntityManager *em, InputSystem *input);
void healPlayerPokemon(EntityManager *em);
bool isSomePotions(EntityManager *em);
void decreasePotionCount(EntityManager *em);

void closeBattle(EntityManager *em, RenderSystem *render);

void handleDialog(Controller* controller) {
	auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();
	if (!input->hasMouseClick()) return;

	auto buttons = em->getEntitiesWithComponent<QuestButtonComponent>();
	for (auto button : buttons) {
		auto buttonComponent = button->getComponent<QuestButtonComponent>();
		if (isClickOnEntity(input->getMouseClick(), button)) {
			if (buttonComponent->getType() == QuestButtonType::Cancel) {
				clearDialog(em, render);
				*state = GameState::Game;
				input->clear();
				return;
			}

			if (buttonComponent->getType() == QuestButtonType::Ok) {
				clearDialog(em, render);
				auto player = em->getEntitiesWithComponent<PlayerControlComponent>()[0];
				auto inventory = player->getComponent<PlayersInventoryComponent>();
				input->clear();
				*state = GameState::Game;
				if (inventory->getPokemonCount() == 0) return;
				initBattle(em, render, battleContext);
				*state = GameState::Battle;
				return;
			}
		}
	}
}

void updateBattle(Controller *controller) {
    auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();

    switch (battleContext->state) {
    case BattleState::PlayerTurn: {
		onUsePotion(em, input);
		onClickArrow(em, input, battleContext);
        break;
	}
    case BattleState::EnemyTurn:
		enemyAttack(em, battleContext);
        break;

    case BattleState::PlayerWon:
		closeBattle(em, render);
		clearGameEntities(em, render);
		createTheEnd(controller);
		*state = GameState::TheEnd;
        break;


    case BattleState::EnemyWon:
		closeBattle(em, render);
		*state = GameState::Game;
        break;
    }
}

void initBattle(EntityManager *em, RenderSystem *render, BattleContext *ctx) {
	initBattleLocation(em);
	initArrows(em);
	initMyPokemon(em);
	initEnemyPokemon(em);
	initPotionButton(em);

	render->removeEntities();
    render->addEntities(em->getEntitiesWithComponent<BattleTypeEntityComponent>());
	ctx->state = BattleState::PlayerTurn;
}

void initBattleLocation(EntityManager *em) {
	auto location = em->createEntity();
    location->addComponent<PositionComponent>(0, 0);
    location->addComponent<SizeComponent>(WINDOW_WIDTH, WINDOW_HEIGHT);
	location->addComponent<RenderLayerComponent>(0);
    location->addComponent<BattleTypeEntityComponent>();
    sf::Texture locationTexture;
    if (locationTexture.loadFromFile("../res/battleLocation(156x116).png")) {
        location->addComponent<TextureComponent>(
			locationTexture,
			156, 
			116
		);
    }
}

void initArrows(EntityManager *em) {
	for(int i = 0; i < 3; ++i) {
		auto arrow = em->createEntity();
		arrow->addComponent<PositionComponent>(
			BATTLE_ARROWS_POS_X,
			BATTLE_ARROWS_POS_Y + i * BATTLE_ARROW_WIDTH
		);
		arrow->addComponent<SizeComponent>(BATTLE_ARROW_WIDTH, BATTLE_ARROW_HEIGHT);
		arrow->addComponent<RenderLayerComponent>(1);
		arrow->addComponent<BattleArrowComponent>(ATTACK_DIR_ARR[i]);
		arrow->addComponent<RotationComponent>(0, 0, 0, PI * 1/3 + i * PI * 1/6);
		arrow->addComponent<OriginComponent>(BATTLE_ARROW_WIDTH / 2, BATTLE_ARROW_HEIGHT );
		arrow->addComponent<BattleTypeEntityComponent>();
		sf::Texture arrowTexture;
		if (arrowTexture.loadFromFile("../res/arrow(57x162).png")) {
			arrow->addComponent<TextureComponent>(
				arrowTexture,
				57, 
				162
			);
		}
	} 
}

void initMyPokemon(EntityManager *em) {
    auto player = em->getEntitiesWithComponent<PlayerControlComponent>()[0];
    auto inventory = player->getComponent<PlayersInventoryComponent>();
	auto pokemon = em->getEntity(inventory->getPokemons()[0]);
	pokemon->getComponent<PositionComponent>()->setPos(
		BATTLE_MY_POKEMON_X,
		BATTLE_MY_POKEMON_Y
	);
	pokemon->getComponent<SizeComponent>()->setSize(BATTLE_POKEMON_WIDTH, BATTLE_POKEMON_HEIGHT);
	pokemon->addComponent<BattleTypeEntityComponent>();
	pokemon->addComponent<PlayerPokemonComponent>();
	pokemon->getComponent<RenderLayerComponent>()->setLayer(1);
	pokemon->getComponent<HealthComponent>()->setVisible(true);
	sf::Texture pokemonTexture;
	auto texture = pokemon->getComponent<TextureComponent>();
	texture->setWidth(64);
	texture->setHeight(64);
	if (pokemonTexture.loadFromFile("../res/" + pokemon->getComponent<PokemonComponent>()->getName() + "(64x64).png")) {
		texture->setTexture(pokemonTexture);
	}
}

void initEnemyPokemon(EntityManager *em) {
	auto enemy = getEnemy(em);
	auto pokemon = getPokemon(em, enemy);

    pokemon->addComponent<PositionComponent>(
		BATTLE_ENEMY_POKEMON_X,
		BATTLE_ENEMY_POKEMON_Y
	);
    pokemon->addComponent<SizeComponent>(BATTLE_POKEMON_WIDTH, BATTLE_POKEMON_HEIGHT);
	pokemon->addComponent<RenderLayerComponent>(1);
	pokemon->addComponent<HealthComponent>(500, 500, true);
    pokemon->addComponent<BattleTypeEntityComponent>();
    pokemon->addComponent<DamageComponent>(10, 25);
    pokemon->addComponent<EnemyPokemonComponent>();
    sf::Texture pokemonTexture;
    if (pokemonTexture.loadFromFile("../res/" + pokemon->getComponent<PokemonComponent>()->getName() + "(64x64).png")) {
        pokemon->addComponent<TextureComponent>(
			pokemonTexture,
			64, 
			64
		);
    }
}

Entity *getEnemy(EntityManager *em) {
	auto enemies = em->getEntitiesWithComponent<TrainerComponent>();
	Entity *enemy;
	for (auto e : enemies) {
		if (e->getComponent<TrainerComponent>()->isAttacked()) enemy = e;
	}
	return enemy;
}

Entity *getPokemon(EntityManager *em, Entity *enemy) {
	auto pokemons = enemy->getComponent<TrainerPokemonsComponent>();
	std::srand(std::time(0));
    int index = std::rand() % pokemons->getPokemonCount();
	int id = pokemons->getPokemons()[index];
	return em->getEntity(id);
}

void initPotionButton(EntityManager *em) {
	initPotionButtonTexture(em);
	initPotionButtonTriggerKey(em);
	initPotionButtonAmount(em);
}

void initPotionButtonTexture(EntityManager *em) {
	auto potionButtonTexture = em->createEntity();
	potionButtonTexture->addComponent<PositionComponent>(BATTLE_POTION_BUTTON_X, BATTLE_POTION_BUTTON_Y);
	potionButtonTexture->addComponent<SizeComponent>(BATTLE_POTION_BUTTON_WIDTH, BATTLE_POTION_BUTTON_HEIGHT);
	potionButtonTexture->addComponent<RenderLayerComponent>(1);
	potionButtonTexture->addComponent<BattleTypeEntityComponent>();
	potionButtonTexture->addComponent<PotionButtonComponent>();
	potionButtonTexture->addComponent<PotionButtonLayoutComponent>();
	sf::Texture texture;
	if (texture.loadFromFile("../res/potion-button(67x64).png")) {
		potionButtonTexture->addComponent<TextureComponent>(texture, 67, 64);
	}
}

void initPotionButtonTriggerKey(EntityManager *em) {
	auto potionButtonTriggerKey = em->createEntity();
	potionButtonTriggerKey->addComponent<PositionComponent>(BATTLE_POTION_BUTTON_TEXT_X, BATTLE_POTION_BUTTON_UPPER_TEXT_Y);
	potionButtonTriggerKey->addComponent<RenderLayerComponent>(2);
	potionButtonTriggerKey->addComponent<BattleTypeEntityComponent>();
	potionButtonTriggerKey->addComponent<PotionButtonComponent>();
	potionButtonTriggerKey->addComponent<TextComponent>(
		"1", 
		BATTLE_POTION_BUTTON_TEXT_X, 
		BATTLE_POTION_BUTTON_UPPER_TEXT_Y, 
		28, 
		sf::Color(68, 68, 68)
	);
}

void initPotionButtonAmount(EntityManager *em) {
	auto player = em->getEntitiesWithComponent<PlayerControlComponent>()[0];
	if (!player) return;

	auto inventory = player->getComponent<PlayersInventoryComponent>();
	if (!inventory) return;

	auto potionAmount = inventory->getPotionCount();

	auto potionButtonAmount = em->createEntity();
	potionButtonAmount->addComponent<PositionComponent>(BATTLE_POTION_BUTTON_TEXT_X, BATTLE_POTION_BUTTON_LOWER_TEXT_Y);
	potionButtonAmount->addComponent<RenderLayerComponent>(2);
	potionButtonAmount->addComponent<BattleTypeEntityComponent>();
	potionButtonAmount->addComponent<PotionButtonComponent>();
	potionButtonAmount->addComponent<TextComponent>(
		std::to_string(potionAmount), 
		BATTLE_POTION_BUTTON_TEXT_X, 
		BATTLE_POTION_BUTTON_LOWER_TEXT_Y, 
		28, 
		sf::Color(68, 68, 68)

	);
}

void onArrowClick(EntityManager *em, InputSystem *input, BattleContext  *ctx) {
	auto arrows = em->getEntitiesWithComponent<BattleArrowComponent>();
	if (input->hasMouseClick()) {
	}
	for (auto arrow : arrows) {
		if (!input->hasMouseClick() || !isClickOnEntity(input->getMouseClick(), arrow)) continue;
		// Получение покемона, который не ходит -> переключать состояние, когда сходил
		input->clear();
		damageHandler(em, ctx);
	}
}

void onClickArrow(EntityManager *em, InputSystem *input, BattleContext *ctx) {
	auto keys = input->getPressedKeys();
	for (auto key : keys) {
        switch (key) {
            case sf::Keyboard::Up:
                damageHandler(em, ctx);
                break;
            case sf::Keyboard::Right:
                damageHandler(em, ctx);
                break;
            case sf::Keyboard::Down:
                damageHandler(em, ctx);
                break;
            default:
                break;
        }
    }
}

void damageHandler(EntityManager *em, BattleContext  *ctx) {
	auto enemyPokemon = em->getEntitiesWithComponent<EnemyPokemonComponent>().front();
	auto playerPokemon = em->getEntitiesWithComponent<PlayerPokemonComponent>().front();
	auto damageComponent = playerPokemon->getComponent<DamageComponent>();
	auto damage = getRandomDamage(damageComponent->getMinDamage(), damageComponent->getMaxDamage());
	auto healthComponent = enemyPokemon->getComponent<HealthComponent>();
	int const healthPoints = healthComponent->getCurrent() - damage;
	healthComponent->setCurrent(healthPoints);
	if (healthComponent->getCurrent() == 0) {
		ctx->state = BattleState::PlayerWon;
		return;
	}
	ctx->state = BattleState::EnemyTurn;
}

int getRandomDamage(int minDamage, int maxDamage) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(minDamage, maxDamage);
 
    return dist(gen);
}

void enemyAttack(EntityManager *em, BattleContext  *ctx) {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	auto playerPokemon = em->getEntitiesWithComponent<PlayerPokemonComponent>().front();
	auto enemyPokemon = em->getEntitiesWithComponent<EnemyPokemonComponent>().front();
	auto damageComponent = enemyPokemon->getComponent<DamageComponent>();
	auto damage = getRandomDamage(damageComponent->getMinDamage(), damageComponent->getMaxDamage());
	auto healthComponent = playerPokemon->getComponent<HealthComponent>();
	int const healthPoints = healthComponent->getCurrent() - damage;
	healthComponent->setCurrent(healthPoints);
	if (healthComponent->getCurrent() == 0) {
		ctx->state = BattleState::EnemyWon;
		return;
	}
	ctx->state = BattleState::PlayerTurn;
}

void closeBattle(EntityManager *em, RenderSystem *render) {
	auto battleEntities = em->getEntitiesWithComponent<BattleTypeEntityComponent>();
	render->removeEntities();
    initGameLocation(em);
	auto gameEntities = em->getEntitiesWithComponent<GameTypeEntityComponent>();
	render->addEntities(gameEntities);
}

void onUsePotion(EntityManager *em, InputSystem *input) {
	if (!isSomePotions(em)) return;

	if (!input->hasMouseClick() && input->getPressedKeys().empty()) return;

	auto keys = input->getPressedKeys();
	if (std::find(keys.begin(), keys.end(), sf::Keyboard::Num1) != keys.end()) {
		input->clear();
		healPlayerPokemon(em);
		decreasePotionCount(em);
	}

	auto potionButton = em->getEntitiesWithComponent<PotionButtonLayoutComponent>()[0];
	if (input->hasMouseClick() && isClickOnEntity(input->getMouseClick(), potionButton)) {
		input->clear();
		healPlayerPokemon(em);
		decreasePotionCount(em);
	}
}

void healPlayerPokemon(EntityManager *em) {
	auto playerPokemon = em->getEntitiesWithComponent<PlayerPokemonComponent>().front();

	auto healthComponent = playerPokemon->getComponent<HealthComponent>();
	int const increasedHealth = healthComponent->getCurrent() + HEAL_POKEMON_HEALTH_POINTS;
	int const healthPoints = increasedHealth > healthComponent->getTotal() 
		? healthComponent->getTotal() 
		: increasedHealth;
	healthComponent->setCurrent(healthPoints);
}

void decreasePotionCount(EntityManager *em) {
	auto player = em->getEntitiesWithComponent<PlayerControlComponent>().front();
	if (!player) return;

	auto inventory = player->getComponent<PlayersInventoryComponent>();
	if (!inventory) return;

	inventory->removePotions(1);
}

bool isSomePotions(EntityManager *em) {
	auto player = em->getEntitiesWithComponent<PlayerControlComponent>().front();

	if (!player) return false;

	auto inventory = player->getComponent<PlayersInventoryComponent>();
	if (!inventory) return false;

	auto potionAmount = inventory->getPotionCount();
	return potionAmount > 0;
}
