#pragma once
#include "../Entity.h"

class EntityManager {
public:
    std::unordered_map<std::string, Entity*> entities; // Обычные указатели вместо shared_ptr

    Entity* createEntity(std::string id) {
        auto* entity = new Entity(id);
        entities[entity->id] = entity;
        return entity;
    }

    Entity* getEntity(std::string entityId) {
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

    void removeEntity(std::string entityId) {
        auto it = entities.find(entityId);
        if (it != entities.end()) {
            delete it->second;
            entities.erase(it);
        }
    }

    ~EntityManager() {
        for (auto& [id, entity] : entities) {
            delete entity;
        }
        entities.clear();
    }
};