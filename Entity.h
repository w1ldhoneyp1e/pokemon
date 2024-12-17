#pragma once
#include "./Components/Components.h"
#include <functional>
#include <SFML/Window.hpp>
#include <typeindex>
#include <memory>

class Entity {
private:
    int id;
public:
    Entity(int id) : id(id) {}

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

    void removeAllComponents() {
    for (auto& [type, component] : components) {
        delete component;
    }
    components.clear();
}

    template <typename T>
    T* removeComponent() {
        auto it = components.find(typeid(T));
        if (it != components.end()) {
            components.erase(it);
        }
        return nullptr;
    }

    int getId() {
        return id;
    }

    ~Entity() {
        for (auto it = components.begin(); it != components.end(); ++it) {
            delete it->second;
        }
    }
};