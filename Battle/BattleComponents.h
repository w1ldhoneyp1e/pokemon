#pragma once
#include "../Components/ParentComponent.h"

class BattleTypeEntityComponent : public Component {};
class PlayerPokemonComponent : public Component {};
class PotionButtonComponent : public Component {};
class PotionButtonLayoutComponent : public Component {};

enum AttackDir {
    Head,
    Body,
	Feet
};
class BattleArrowComponent : public Component {
private:
    AttackDir dir;
public:
    BattleArrowComponent(AttackDir dir) : dir(dir) {}
    AttackDir getDir() {
        return dir;
    }
    void setDir(AttackDir _dir) {
        dir = _dir;
    }
};
