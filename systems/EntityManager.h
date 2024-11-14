#pragma once
#include "../Entity.h"

class EntityManager {
public:
    int nextEntityId = 0;
    std::unordered_map<int, Entity*> entities; // Обычные указатели вместо shared_ptr

    // Создание новой сущности
    Entity* createEntity() {
        auto* entity = new Entity(nextEntityId++);
        entities[entity->id] = entity;
        return entity;
    }

    // Получение сущности по ID
    Entity* getEntity(int entityId) {
        auto it = entities.find(entityId);
        return (it != entities.end()) ? it->second : nullptr;
    }

    template <typename T>
    std::vector<Entity*> getEntitiesWithComponent() const {
        std::vector<Entity*> result;
        for (const auto& [id, entity] : entities) {
            if (entity->getComponent<T>() != nullptr) { // Проверяем, есть ли компонент T
                result.push_back(entity);
            }
        }
        return result;
    }

    // Удаление сущности по ID
    void removeEntity(int entityId) {
        auto it = entities.find(entityId);
        if (it != entities.end()) {
            delete it->second; // Удаляем сущность
            entities.erase(it);
        }
    }

    // Деструктор для удаления всех сущностей
    ~EntityManager() {
        for (auto& [id, entity] : entities) {
            delete entity; // Освобождаем память под каждый объект Entity
        }
        entities.clear();
    }
};