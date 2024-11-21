#pragma once
#include "Components.h"
#include <functional>
#include <SFML/Window.hpp>
#include <typeindex>
#include <memory>

class Entity {
private:
    std::string id;
public:
    Entity(std::string id) : id(id) {}

    std::unordered_map<std::type_index, Component*> components;
    // TODO: заменить на умные указатели

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

    std::string getId() {
        return id;
    }

    ~Entity() {
        for (auto it = components.begin(); it != components.end(); ++it) {
            delete it->second;
        }
    }
};