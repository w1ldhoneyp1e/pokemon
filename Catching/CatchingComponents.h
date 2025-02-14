#pragma once
#include "../Components/ParentComponent.h"

class CatchingTypeEntityComponent : public Component {};

class CatchingArrowComponent : public Component {};
class CatchingLocationComponent : public Component {};
class PokeballComponent : public Component {};
class TimerComponent : public Component {};
class HelpCatchingComponent : public Component {};
class HelpCatchingButtonComponent : public Component {};
class AfterCatchingPopupComponent : public Component {};
class AfterCatchingPopupButtonComponent : public Component {};

class ThrowablePokeballComponent : public Component {
private:
    bool _isThrown;
    float angle;
public:
    ThrowablePokeballComponent() : _isThrown(false) {}
    void setThrown() {
        _isThrown = true;
    }
    bool isThrown() {
        return _isThrown;
    }
    void setAngle(float _angle) {
        angle = _angle;
    }
    float getAngle() {
        return angle;
    }
};