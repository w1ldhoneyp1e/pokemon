#pragma once


enum class BattleState {
    Help,
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