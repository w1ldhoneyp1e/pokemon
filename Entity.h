#pragma once
#include "Components.h"
#include <functional>
#include <SFML/Window.hpp>
#include <typeindex>
#include <memory>

class Entity {
public:
    std::string id;

    Entity(std::string id) : id(id) {}

    std::unordered_map<std::type_index, Component*> components;

    template <typename T, typename... Args>
    void addComponent(Args&&... args) {
        components[typeid(T)] = new T(std::forward<Args>(args)...);
    }

    template <typename T>
    T* getComponent() {
        auto it = components.find(typeid(T));
        if (it != components.end()) {
            return dynamic_cast<T*>(it->second);
        }
        return nullptr;
    }

    ~Entity() {
        for (auto& [type, component] : components) {
            delete component;
        }
    }
};