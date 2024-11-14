#pragma once
#include "../Entity.h"

class EntityManager {
public:
    std::unordered_map<std::string, Entity*> entities;
    // TODO: заменить на умные указатели

    Entity* createEntity(std::string id) {
        auto* entity = new Entity(id);
        entities[id] = entity;
        return entity;
    }

    Entity* getEntity(std::string entityId) {
        auto it = entities.find(entityId);
        return (it != entities.end()) 
            ? entities[entityId] 
            : nullptr;
    }

    template <typename T>
    std::vector<Entity*> getEntitiesWithComponent() {
        std::vector<Entity*> result;
        for (auto it = entities.begin(); it != entities.end(); ++it) {
            if (it->second->getComponent<T>() != nullptr) {
                result.push_back(it->second);
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
        for (auto it = entities.begin(); it != entities.end(); ++it) {
            delete it->second;
        }
        entities.clear();
    }
};