#pragma once
#include "../Components/ParentComponent.h"

class PlayerControlComponent : public Component {
public:
    bool isControlable;
    PlayerControlComponent(bool isControlable) : isControlable(isControlable) {}
};