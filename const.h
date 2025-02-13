#pragma once
#include "./Battle/BattleComponents.h"
#include <cmath>

const float WINDOW_WIDTH = 383;
const float WINDOW_HEIGHT = 320;
float SCREEN_WIDTH = 383;
float SCREEN_HEIGHT = 320;

float SCALE = 1.0;

const float PI = std::acos(-1.0);

const int START_BUTTON_WIDTH = 54;
const int START_BUTTON_HEIGHT = 20;

const int PLAYER_START_POSITION_X = 170;
const int PLAYER_START_POSITION_Y = 120;
const int PLAYER_WIDTH = 17;
const int PLAYER_HEIGHT = 20;
const int PLAYER_SPEED = 50;

const int BACKGROUND_MENU_WIDTH = 62;
const int BACKGROUND_MENU_HEIGHT = 46;

// Inventory

const float INVENTORY_WIDTH = 80 * 2.3;
const float INVENTORY_HEIGHT = 156 * 1.3;

const int INVENTORY_POSITION_X = WINDOW_WIDTH / 2 - INVENTORY_WIDTH / 2;
const int INVENTORY_POSITION_Y = WINDOW_HEIGHT / 2 - INVENTORY_HEIGHT / 2;

const int INVENTORY_CELLS_POSITION_START_X = INVENTORY_POSITION_X + 17;
const int INVENTORY_CELLS_POSITION_START_Y = INVENTORY_POSITION_Y + 15;

const int INVENTORY_CELLS_PER_ROW = 6;
const int INVENTORY_CELLS_LIMIT = 24;

const int INVENTORY_BUTTON_CLOSE_WIDTH = 64;
const int INVENTORY_BUTTON_CLOSE_HEIGHT = 26;

const int INVENTORY_BUTTON_CLOSE_X = INVENTORY_POSITION_X + INVENTORY_WIDTH - INVENTORY_BUTTON_CLOSE_WIDTH - 7;
const int INVENTORY_BUTTON_CLOSE_Y = INVENTORY_POSITION_Y + INVENTORY_HEIGHT / 2 + 11;

const int INVENTORY_ICON_X = INVENTORY_POSITION_X + 15;
const int INVENTORY_ICON_Y = INVENTORY_POSITION_Y + INVENTORY_HEIGHT / 2 + 11;

// Pokemons

const int BULBASOUR_POSITION_X = PLAYER_START_POSITION_X + 100;
const int BULBASOUR_POSITION_Y = PLAYER_START_POSITION_Y;
const int POKEMON_INVENTORY_WIDTH = 36;
const int POKEMON_INVENTORY_HEIGHT = 33;

const int POKEMON_SELL_PRICE = 5;

const int HEAL_POKEMON_HEALTH_POINTS = 10;

// Catching

const int CATCHING_LOCATION_WIDTH = 383;
const int CATCHING_LOCATION_HEIGHT = 640;

const int ARROW_WIDTH = 19;
const int ARROW_HEIGHT = 54;

const int ARROW_POS_X = WINDOW_WIDTH / 2;
const int ARROW_POS_Y = WINDOW_HEIGHT - 70;

const int ARROW_SPEED = 2;

const int ARROW_POKEBALL_GAP = 50;

const float ARROW_MAX_ANGLE = PI / 6 * 2;
const float ARROW_MIN_ANGLE = - PI / 6 * 2;

const int POKEBALL_WIDTH = 23;
const int POKEBALL_HEIGHT = 22;

const int POKEBALL_SPEED = 200;

const int POKEBALL_POS_X = ARROW_POS_X + ARROW_WIDTH / 2;
const int POKEBALL_POS_Y = ARROW_POS_Y + POKEBALL_HEIGHT / 2;

const float ARROW_RADIUS = ARROW_POKEBALL_GAP + POKEBALL_HEIGHT / 2;

const int TIMER_POS_X = WINDOW_WIDTH / 2 - 13;
const int TIMER_POS_Y = WINDOW_HEIGHT / 10;

const int TIMER_TEXT_SIZE = 48;

const int TIMER_DURATION = 20;

// Items Chest

const int ITEM_SIDE = 14;

const int CHEST_INTERFACE_TEXTURE_WIDTH = 230;
const int CHEST_INTERFACE_TEXTURE_HEIGHT = 79;

const int CHEST_INTERFACE_WIDTH = CHEST_INTERFACE_TEXTURE_WIDTH;
const int CHEST_INTERFACE_HEIGHT = CHEST_INTERFACE_TEXTURE_HEIGHT;

const int CHEST_INTERFACE_X = WINDOW_WIDTH / 2 - CHEST_INTERFACE_WIDTH / 2;
const int CHEST_INTERFACE_Y = WINDOW_HEIGHT / 2 - CHEST_INTERFACE_HEIGHT / 2;

const int MAX_ITEMS_PER_ROW = 9;

const int CHEST_WIDTH = 24;
const int CHEST_HEIGHT = 23;

const int CHEST_INTERFACE_BUTTON_CLOSE_X = CHEST_INTERFACE_X;
const int CHEST_INTERFACE_BUTTON_CLOSE_Y = CHEST_INTERFACE_HEIGHT + CHEST_INTERFACE_Y;

const int CHEST_INTERFACE_BUTTON_WIDTH = INVENTORY_BUTTON_CLOSE_WIDTH;
const int CHEST_INTERFACE_BUTTON_HEIGHT = INVENTORY_BUTTON_CLOSE_HEIGHT;

const int CHEST_INTERFACE_BUTTON_GET_X = CHEST_INTERFACE_X + CHEST_INTERFACE_WIDTH - CHEST_INTERFACE_BUTTON_WIDTH;
const int CHEST_INTERFACE_BUTTON_GET_Y = CHEST_INTERFACE_HEIGHT + CHEST_INTERFACE_Y;

// Trainer

const int TRAINER_WIDTH = PLAYER_WIDTH;
const int TRAINER_HEIGHT = PLAYER_HEIGHT;

const int TRAINER_POSITION_X = PLAYER_START_POSITION_X - 40;
const int TRAINER_POSITION_Y = PLAYER_START_POSITION_Y;

// Battle 

const int BATTLE_ARROW_WIDTH = 19;
const int BATTLE_ARROW_HEIGHT = 54;

const int BATTLE_ARROWS_POS_X = WINDOW_WIDTH / 2 - BATTLE_ARROW_HEIGHT / 2;
const int BATTLE_ARROWS_POS_Y = WINDOW_HEIGHT / 2 - BATTLE_ARROW_WIDTH;

const int BATTLE_POKEMON_WIDTH = 100;
const int BATTLE_POKEMON_HEIGHT = 100;

const int BATTLE_MY_POKEMON_X = WINDOW_WIDTH / 4 - BATTLE_POKEMON_WIDTH / 2;
const int BATTLE_MY_POKEMON_Y = WINDOW_HEIGHT / 2 - BATTLE_POKEMON_HEIGHT / 2;

const int BATTLE_ENEMY_POKEMON_X = WINDOW_WIDTH * 3 / 4 - BATTLE_POKEMON_WIDTH / 2;
const int BATTLE_ENEMY_POKEMON_Y = WINDOW_HEIGHT / 2 - BATTLE_POKEMON_HEIGHT / 2;

const AttackDir ATTACK_DIR_ARR[3] = {AttackDir::Head, AttackDir::Body, AttackDir::Feet};

const float BATTLE_POTION_BUTTON_WIDTH = 33.5;
const int BATTLE_POTION_BUTTON_HEIGHT = 32;

const int BATTLE_POTION_BUTTON_X = WINDOW_WIDTH / 2 - BATTLE_POTION_BUTTON_WIDTH / 2;
const int BATTLE_POTION_BUTTON_Y = WINDOW_HEIGHT - BATTLE_POTION_BUTTON_HEIGHT - 15;

const int BATTLE_POTION_BUTTON_TEXT_SIZE = 28;

const int BATTLE_POTION_BUTTON_TEXT_X = BATTLE_POTION_BUTTON_X + BATTLE_POTION_BUTTON_WIDTH - 11;
const int BATTLE_POTION_BUTTON_UPPER_TEXT_Y = BATTLE_POTION_BUTTON_Y + 3;
const int BATTLE_POTION_BUTTON_LOWER_TEXT_Y = BATTLE_POTION_BUTTON_Y + BATTLE_POTION_BUTTON_HEIGHT - 10;

// Shop

const float SHOP_POS_X = 256.0f;
const float SHOP_POS_Y = 223.0f;

const float SHOP_INTERFACE_WIDTH = 300.0f;
const float SHOP_INTERFACE_HEIGHT = 300.0f;
const int SHOP_INTERFACE_X = WINDOW_WIDTH / 2 - SHOP_INTERFACE_WIDTH / 2;
const int SHOP_INTERFACE_Y = WINDOW_HEIGHT / 2 - SHOP_INTERFACE_HEIGHT / 2;

const float SHOP_AVATAR_SIZE = 64.0f;
const int POKEBALL_PRICE = 10;
const int POTION_PRICE = 15;

const int PLAYER_AVATAR_WIDTH = 64;
const int PLAYER_AVATAR_HEIGHT = 64;

const int PLAYER_AVATAR_X = SHOP_INTERFACE_X + 25;
const int PLAYER_AVATAR_Y = SHOP_INTERFACE_Y + 30;

const int SHOP_AVATAR_WIDTH = 64;
const int SHOP_AVATAR_HEIGHT = 48;

const int SHOP_AVATAR_X = SHOP_INTERFACE_X + SHOP_INTERFACE_WIDTH - SHOP_AVATAR_WIDTH - 35;
const int SHOP_AVATAR_Y = SHOP_INTERFACE_Y + 40;

// Shop buttons

const int SHOP_BUTTON_WIDTH = 48;
const int SHOP_BUTTON_HEIGHT = 20;

const int SHOP_BUTTON_OK_X = SHOP_INTERFACE_X + SHOP_INTERFACE_WIDTH / 4;
const int SHOP_BUTTON_OK_Y = SHOP_INTERFACE_Y + SHOP_INTERFACE_HEIGHT - SHOP_BUTTON_HEIGHT - 35;

const int SHOP_BUTTON_BACK_X = SHOP_INTERFACE_X + SHOP_INTERFACE_WIDTH - SHOP_BUTTON_WIDTH - SHOP_INTERFACE_WIDTH / 4;
const int SHOP_BUTTON_BACK_Y = SHOP_INTERFACE_Y + SHOP_INTERFACE_HEIGHT - SHOP_BUTTON_HEIGHT - 35;

// Shop items

const int SHOP_ITEM_WIDTH = 10;
const int SHOP_ITEM_HEIGHT = 10;

const int SHOP_ITEM_POS_X = SHOP_INTERFACE_X + 60;
const int SHOP_ITEM_POS_Y = SHOP_INTERFACE_Y + PLAYER_AVATAR_HEIGHT + 50;

const int SHOP_TEXT_HEIGHT = 20;

const int SHOP_ITEM_GAP = 10;

const int SHOW_HEADER_START_X = SHOP_INTERFACE_X + 30;

const float SHOP_HEADER_HEIGHT = 10.0f;
const float SHOP_COLUMN_WIDTH_SMALL = 30.0f;
const float SHOP_COLUMN_WIDTH_MEDIUM = 40.0f;
const float SHOP_COLUMN_WIDTH_BIG = 80.0f;

const float BUYSELL_DELAY = 0.2f;
const float SELECTION_DELAY = 0.2f;

// The End

const float THE_END_TIMER = 4.0f;

const int THE_END_TEXT_SIZE = 100;
const int THE_END_TEXT_X = WINDOW_WIDTH / 2 - 100;
const int THE_END_TEXT_Y = WINDOW_HEIGHT / 2 - THE_END_TEXT_SIZE / 2;

const int THE_END_BACK_TO_MENU_TEXT_X = THE_END_TEXT_X + THE_END_TEXT_SIZE * 1 / 3 + 5;
const int THE_END_BACK_TO_MENU_TEXT_Y = THE_END_TEXT_Y + 50;


const int THE_END_BACK_TO_MENU_TEXT_SIZE = THE_END_TEXT_SIZE / 3;

// Pause

const int PAUSE_LAYOUT_WIDTH = 150;
const int PAUSE_LAYOUT_HEIGHT = 150;

const int PAUSE_LAYOUT_POSITION_X = WINDOW_WIDTH / 2 - PAUSE_LAYOUT_WIDTH / 2;
const int PAUSE_LAYOUT_POSITION_Y = WINDOW_HEIGHT / 2 - PAUSE_LAYOUT_HEIGHT / 2;

// Quests

const int DIALOG_WIDTH = 200;
const int DIALOG_HEIGHT = 100;

const int DIALOG_POSITION_X = WINDOW_WIDTH / 2 - DIALOG_WIDTH / 2;
const int DIALOG_POSITION_Y = WINDOW_HEIGHT / 2 - DIALOG_HEIGHT / 2;

const int DIALOG_BUTTON_WIDTH = 32;
const int DIALOG_BUTTON_HEIGHT = 13;

const int DIALOG_CANCEL_BUTTON_POSITION_X = DIALOG_POSITION_X + 30;
const int DIALOG_CANCEL_BUTTON_POSITION_Y = DIALOG_POSITION_Y + DIALOG_HEIGHT - DIALOG_BUTTON_HEIGHT - 15;

const int DIALOG_OK_BUTTON_POSITION_X = DIALOG_POSITION_X + DIALOG_WIDTH - DIALOG_BUTTON_WIDTH - 30;
const int DIALOG_OK_BUTTON_POSITION_Y = DIALOG_CANCEL_BUTTON_POSITION_Y;


