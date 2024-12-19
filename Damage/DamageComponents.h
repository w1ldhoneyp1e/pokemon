#pragma once
#include "../Components/ParentComponent.h"

class DamageComponent : public Component {
private:
    int minDamage;
    int maxDamage;

public:
    DamageComponent(int minDamage, int maxDamage)
        : minDamage(minDamage), maxDamage(maxDamage) {}

    int getMinDamage() const {
        return minDamage;
    }

    void setMinDamage(int value) {
        minDamage = value;
    }

    int getMaxDamage() const {
        return maxDamage;
    }

    void setMaxDamage(int value) {
        maxDamage = value;
    }
};

