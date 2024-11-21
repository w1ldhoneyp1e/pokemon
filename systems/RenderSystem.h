#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <vector>
#include "../Entity.h"
#include <iostream>

class RenderSystem {
public:
    const sf::Color COLOR_WHITE = sf::Color(255, 255, 255);
    
    RenderSystem(sf::RenderWindow* window, EntityManager* entityManager)
        : window(window), entityManager(entityManager) {}

    void addEntity(Entity* entity) {
        entities.push_back(entity);
    }

    void addEntities(std::vector<Entity*> incomeEntities) {
        for (auto it = incomeEntities.begin(); it != incomeEntities.end(); ++it) {
            entities.push_back(*it);
        }
    }

    void removeEntity(std::string entityId) {
        for (auto it = entities.begin(); it != entities.end(); ++it) {
            if ((*it)->getId() == entityId) {
                entities.erase(it);
                break;
            }
        }
    }

    void removeEntities() {
        entities.clear();
    }

    void render() {
        if (!window) return;

        window->clear(COLOR_WHITE);

        // Сортировка по слоям
        std::sort(entities.begin(), entities.end(), [](Entity* a, Entity* b) {
            auto layerA = a->getComponent<RenderLayerComponent>();
            auto layerB = b->getComponent<RenderLayerComponent>();
            
            // Обработка случая, когда компонента нет
            return (layerA && layerB) ? layerA->getLayer() < layerB->getLayer() : (layerA == nullptr); 
        });

        for (Entity* entity : entities) {
            // Проверка на валидность entity -  лучше делать проверку на null
            if (!entity) continue;


            auto textureComp = entity->getComponent<TextureComponent>();
            auto positionComp = entity->getComponent<PositionComponent>();
            auto sizeComp = entity->getComponent<SizeComponent>();

            if (textureComp && positionComp) {
                textureComp->sprite.setPosition(positionComp->getX(), positionComp->getY());

                if (sizeComp) {
                    textureComp->sprite.setScale(
                        sizeComp->getWidth() / textureComp->sprite.getLocalBounds().width,
                        sizeComp->getHeight() / textureComp->sprite.getLocalBounds().height
                    );
                }
                window->draw(textureComp->sprite);
            }
        }

        window->display();
    }

private:
    sf::RenderWindow* window;
    EntityManager* entityManager;
    std::vector<Entity*> entities;
};