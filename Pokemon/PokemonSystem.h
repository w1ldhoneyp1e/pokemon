#pragma once
#include "../systems/EntityManager.h"
#include "../systems/InputSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/CollisionSystem.h"
#include "../systems/KeyboardSystem.h"
#include "../Catching/CatchingSystem.h"
#include "../GameState.h"
#include "../const.h"
#include <ctime>

struct PokemonConditionProps {
	Entity* pokemon;
	Entity* player;
	PlayersInventoryComponent* inventory;
	std::vector<sf::Keyboard::Key> keys;
};

bool isInventoryFull(PlayersInventoryComponent *inventory);
bool isPokemonCollected(Entity* pokemon);
bool doesConditionSatisfy(PokemonConditionProps props);
Entity* findPokemonByID(std::vector<Entity*> pokemons, int id);
void sellPokemon(Controller* controller, Entity* pokemon);
void updateCoins(Controller* controller);

void pokemonCollision(Controller* controller) {
	auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();

	auto player = em->getEntitiesWithComponent<PlayerControlComponent>()[0];
	auto inventory = player->getComponent<PlayersInventoryComponent>();
	auto pokeballs = inventory->getPokeballCount();
	if (pokeballs == 0) return;

	auto pokemons = em->getEntitiesWithComponent<PokemonComponent>();
	auto keys = input->getPressedKeys();
	if (keys.empty()) return;
	for (auto pokemon : pokemons) {
		if (pokemon->getComponent<GameTypeEntityComponent>() == nullptr) continue;;
		if (doesConditionSatisfy({
				pokemon, 
				player,
				inventory,
				keys
			})) {
			*state = GameState::Catching;
			input->clear();
			pokemon->addComponent<AttackedPokemonComponent>();
			render->removeEntity(pokemon->getId());
			pokemon->removeComponent<GameTypeEntityComponent>();
			initCatching(em, render);
			inventory->removePokeballs(1);
			return;
		}
	}
}


bool doesConditionSatisfy(PokemonConditionProps props) {
	return isCollision(props.pokemon, props.player) 
		&& isInventoryFull(props.inventory)
		&& isPokemonCollected(props.pokemon)
		&& isEnterPressed(props.keys);
}

bool isInventoryFull(PlayersInventoryComponent *inventory) {
	return inventory->getPokemonCount() < INVENTORY_CELLS_LIMIT;
}

bool isPokemonCollected(Entity* pokemon) {
	return !pokemon->getComponent<PokemonComponent>()->isCollected();
}

void generatePokemon(Controller* controller) {
	auto [input, em, render, state, battleContext, collisionMaps, currentLocation] = controller->getAll();
	
	auto currentMap = collisionMaps->at(currentLocation);
	int mapWidth = currentMap.getWidth() * currentMap.getCellWidth();
	int mapHeight = currentMap.getHeight() * currentMap.getCellHeight();
	
	float x = std::rand() % (mapWidth - POKEMON_INVENTORY_WIDTH);
	float y = std::rand() % (mapHeight - POKEMON_INVENTORY_HEIGHT);
	
	bool validPosition = false;
	int attempts = 0;
	const int maxAttempts = 100;
	
	while (!validPosition && attempts < maxAttempts) {
		if (attempts > 0) {
			x = std::rand() % (mapWidth - POKEMON_INVENTORY_WIDTH);
			y = std::rand() % (mapHeight - POKEMON_INVENTORY_HEIGHT);
		}
		
		bool hasCollision = currentMap.isCollision(x, y);
		
		if (!hasCollision) {
			validPosition = true;
			auto pokemon = em->createEntity();
			
			int pokemonTypeIndex = std::rand() % POKEMON_TYPES.size();
			const auto& stats = POKEMON_TYPES[pokemonTypeIndex];
			
			pokemon->addComponent<PositionComponent>(x, y);
			pokemon->addComponent<SizeComponent>(POKEMON_INVENTORY_WIDTH, POKEMON_INVENTORY_HEIGHT);
			pokemon->addComponent<RenderLayerComponent>(1);
			pokemon->addComponent<HealthComponent>(stats.baseHealth, stats.baseHealth);
			pokemon->addComponent<DamageComponent>(stats.minDamage, stats.maxDamage);
			pokemon->addComponent<PokemonComponent>(stats.name);
			pokemon->addComponent<GameTypeEntityComponent>();
			
			auto texture = new sf::Texture();
			if (texture->loadFromFile(stats.texturePath)) {
				pokemon->addComponent<TextureComponent>(
					*texture,
					64,
					64
				);
			}
			render->addEntity(pokemon);
		}
		attempts++;
	}
}

void pokemonGenerating(Controller* controller, float deltaTime) {
	static float timeSinceLastGeneration = 0.0f;
	static const float generationInterval = 500.0f;
	
	auto [em, input, render, state, battleContext, collisionMaps, currentLocation] = controller->getAll();
	
	timeSinceLastGeneration += deltaTime;
	
	if (timeSinceLastGeneration >= generationInterval) {
		timeSinceLastGeneration = 0.0f;
		generatePokemon(controller);
	}
}

void onPokemonSellButtonClick(Controller* controller) {
	auto input = controller->getInputSystem();
	if (!input->hasMouseClick()) return;

	auto em = controller->getEntityManager();

	auto sellButtons = em->getEntitiesWithComponent<PokemonSellComponent>();
	for (auto sellButton : sellButtons) {
		if (!isClickOnEntity(input->getMouseClick(), sellButton)) continue;

		int id = sellButton->getComponent<PokemonSellComponent>()->getId();
		auto pokemons = em->getEntitiesWithComponent<PokemonComponent>();
		auto pokemon = findPokemonByID(pokemons, id);
		if (pokemon) {
			sellPokemon(controller, pokemon);
			em->removeEntity(sellButton);
			updateCoins(controller);
		}
		input->clear();
		return;
	}
}

Entity* findPokemonByID(std::vector<Entity*> pokemons, int id) {
	for (auto pokemon : pokemons) {
		if (pokemon->getId() == id) {
			return pokemon;
		}
	}
	return nullptr;
}

void sellPokemon(Controller* controller, Entity* pokemon) {
	auto em = controller->getEntityManager();
	auto player = em->getEntitiesWithComponent<PlayerControlComponent>()[0];

	auto inventory = player->getComponent<PlayersInventoryComponent>();
	inventory->addCoins(POKEMON_SELL_PRICE);
	
	auto render = controller->getRenderSystem();
	render->removeEntity(pokemon->getId());
	em->removeEntity(pokemon);
}

void updateCoins(Controller* controller) {
	auto em = controller->getEntityManager();

	auto coinsAmount = em->getEntitiesWithComponent<InventoryCoinsAmountComponent>();
	if (coinsAmount.empty()) return;

	auto text = coinsAmount[0]->getComponent<TextComponent>();
	if (!text) return;

	auto player = em->getEntitiesWithComponent<PlayerControlComponent>()[0];
	if (!player) return;

	auto inventory = player->getComponent<PlayersInventoryComponent>();
	if (!inventory) return;

	text->setText(std::to_string(inventory->getCoinCount()));
}


