#pragma once
#include "Components.h"
#include <functional>
#include <SFML/Window.hpp>
#include <typeindex>
#include <memory>

class Entity {
public:
    int id;

    Entity(int id) : id(id) {}

    // Хранение компонентов с обычными указателями
    std::unordered_map<std::type_index, Component*> components;

    // Добавление компонента
    template <typename T, typename... Args>
    void addComponent(Args&&... args) {
        components[typeid(T)] = new T(std::forward<Args>(args)...);
    }

    // Получение компонента
    template <typename T>
    T* getComponent() {
        auto it = components.find(typeid(T));
        if (it != components.end()) {
            return dynamic_cast<T*>(it->second);
        }
        return nullptr;
    }

    // Удаление всех компонентов
    ~Entity() {
        for (auto& [type, component] : components) {
            delete component; // Освобождаем память под каждый компонент
        }
    }
};