#pragma once
#include "../Components/ParentComponent.h"

class HealthComponent : public Component {
private:
    float current;
    float total;
    bool _isVisible;
public:
    HealthComponent(float total, float current, bool isVisible = false)
        : total(total), current(current), _isVisible(isVisible) {}

    float getTotal() const {
        return total;
    }

    float getCurrent() const {
        return current;
    }

    float isVisible() const {
        return _isVisible;
    }

    void setTotal(float value) {
        total = value;
    }

    void setCurrent(float value) {
        current = value;
    }

    void setVisible(float value) {
        _isVisible = value;
    }
};