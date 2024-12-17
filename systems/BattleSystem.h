#pragma once


enum class BattleState {
    PlayerTurn,
    EnemyTurn,
    Animation,
    EndCheck
};

struct BattleContext {
    BattleState state;
    int playerHealth;
    int enemyHealth;
};