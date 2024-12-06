#pragma once
#include "../systems/Controller.h"
#include "../systems/EntityManager.h"
#include "../systems/RenderSystem.h"
#include "../const.h"
#include <ctime>

void initBattleLocation(EntityManager *em);
void initArrows(EntityManager *em);
void initMyPokemon(EntityManager *em);
void initEnemyPokemon(EntityManager *em);

Entity *getEnemy(EntityManager *em);
Entity *getPokemon(EntityManager *em, Entity *enemy);

void onArrowClick();

void updateBattle(Controller *controller) {
	onArrowClick();
}

void initBattle(EntityManager *em, RenderSystem *render) {
	initBattleLocation(em);
	initArrows(em);
	initMyPokemon(em);
	initEnemyPokemon(em);

	render->removeEntities();
    render->addEntities(em->getEntitiesWithComponent<BattleTypeEntityComponent>());
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
	pokemon->addComponent<HealthComponent>(100, 100, true);
    pokemon->addComponent<BattleTypeEntityComponent>();
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

void onArrowClick() {

}