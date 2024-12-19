#pragma once


enum class BattleState {
    PlayerTurn,
    EnemyTurn,
    PlayerWon,
    EnemyWon
};

struct BattleContext {
    BattleState state;
    int playerHealth;
    int enemyHealth;
};