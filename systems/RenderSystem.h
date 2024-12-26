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

            sf::Vector2u baseSize(WINDOW_WIDTH, WINDOW_HEIGHT);
            float scaleX = SCREEN_WIDTH / baseSize.x;
            float scaleY = SCREEN_HEIGHT / baseSize.y;
            float scale = std::min(scaleX, scaleY);

            float offsetX = SCREEN_WIDTH / 2 - scale * WINDOW_WIDTH / 2;

            if (textureComp && positionComp) {
                auto posX = positionComp->getX();
                auto posY = positionComp->getY();

                if (originComp) {
                    textureComp->sprite.setOrigin(
                        originComp->getX() * scale,
                        originComp->getY() * scale
                    );
                }

                if (sizeComp) {
                    textureComp->sprite.setScale(
                        sizeComp->getWidth() / textureComp->sprite.getLocalBounds().width,
                        sizeComp->getHeight() / textureComp->sprite.getLocalBounds().height
                    );
                }
                
                auto currScale = textureComp->sprite.getScale();
                textureComp->sprite.setScale(scale * currScale.x, scale * currScale.y);

                textureComp->sprite.setPosition(posX * scale + offsetX, posY * scale);

                if (rotationComp) {
                    textureComp->sprite.setRotation(rotationComp->angle * 180 / PI);
                }

                if (healthComp && healthComp->isVisible()) {
                    showHealth(
                        window,
                        sizeComp->getWidth() * scale,
                        sizeComp->getHeight() * scale,
                        healthComp, positionComp,
                        scale
                    );
                }

                window->draw(textureComp->sprite);
            }

            // sf::RectangleShape outline;

            // sf::FloatRect localBounds = textureComp->sprite.getLocalBounds();
            // outline.setSize(sf::Vector2f(localBounds.width, localBounds.height));

            // outline.setOrigin(
            //     localBounds.left + textureComp->sprite.getOrigin().x,
            //     localBounds.top + textureComp->sprite.getOrigin().y
            // );

            // outline.setPosition(textureComp->sprite.getPosition());
            // outline.setRotation(textureComp->sprite.getRotation());
            // outline.setScale(textureComp->sprite.getScale());

            // outline.setFillColor(sf::Color::Transparent);
            // outline.setOutlineThickness(2.f);
            // outline.setOutlineColor(sf::Color::Green);

            // window->draw(outline);
        }

        window->display();
    }

private:
    sf::RenderWindow* window;
    EntityManager* entityManager;
    std::vector<Entity*> entities;
};





