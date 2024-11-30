#pragma once
#include "../Entity.h"
#include <unordered_map>
#include <vector>

class EntityManager {
private:
    std::unordered_map<int, Entity*> entities;
    int idCounter = 0;

public:
    Entity* createEntity() {
        int id = idCounter++;
        auto* entity = new Entity(id);
        entities[id] = entity;
        return entity;
    }

    Entity* getEntity(int entityId) {
        auto it = entities.find(entityId);
        return (it != entities.end()) 
            ? it->second 
            : nullptr;
    }

    template <typename T>
    std::vector<Entity*> getEntitiesWithComponent() {
        std::vector<Entity*> result;
        for (auto& pair : entities) {
            if (pair.second->getComponent<T>() != nullptr) {
                result.push_back(pair.second);
            }
        }
        return result;
    }

    void removeEntity(Entity* entity) {
        if (!entity) return;
        for (auto it = entities.begin(); it != entities.end(); ++it) {
            if (it->second == entity) {
                delete it->second;
                entities.erase(it);
                return;
            }
        }
    }

    ~EntityManager() {
        for (auto& pair : entities) {
            delete pair.second;
        }
        entities.clear();
    }
};
