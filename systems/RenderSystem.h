#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <vector>
#include "../Entity.h"
#include "../Health/Health.h"
#include <iostream>
#include "../const.h"

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

    void removeEntity(int entityId) {
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

        std::sort(entities.begin(), entities.end(), [](Entity* a, Entity* b) {
            auto layerA = a->getComponent<RenderLayerComponent>();
            auto layerB = b->getComponent<RenderLayerComponent>();
            
            return (layerA && layerB) ? layerA->getLayer() < layerB->getLayer() : (layerA == nullptr); 
        });

        for (Entity* entity : entities) {
            if (!entity) continue;


            auto textureComp = entity->getComponent<TextureComponent>();
            auto positionComp = entity->getComponent<PositionComponent>();
            auto rotationComp = entity->getComponent<RotationComponent>();
            auto sizeComp = entity->getComponent<SizeComponent>();
            auto originComp = entity->getComponent<OriginComponent>();
            auto healthComp = entity->getComponent<HealthComponent>();

            if (textureComp && positionComp) {
                auto posX = positionComp->getX();
                auto posY = positionComp->getY();
                // if (textureComp->originX) {
                //     posX = textureComp->originX;
                // }
                // if (textureComp->originY) {
                //     posY = textureComp->originY;
                // }

                if (originComp) {
                    textureComp->sprite.setOrigin(
                        originComp->getX(),
                        originComp->getY()
                    );
                }

                textureComp->sprite.setPosition(posX, posY);

                if (sizeComp) {
                    textureComp->sprite.setScale(
                        sizeComp->getWidth() / textureComp->sprite.getLocalBounds().width,
                        sizeComp->getHeight() / textureComp->sprite.getLocalBounds().height
                    );
                }

                if (rotationComp) {
                    auto x = textureComp->sprite.getOrigin().x;
                    auto y = textureComp->sprite.getOrigin().y;

                    textureComp->sprite.setRotation(rotationComp->angle * 180 / PI);
                }

                if (healthComp && healthComp->isVisible()) {
                    showHealth(window, sizeComp->getWidth(), healthComp, positionComp);
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