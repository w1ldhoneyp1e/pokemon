#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <vector>
#include "../Entity.h"
#include "../Health/Health.h"
#include <iostream>
#include "../const.h"
#include "../Inventory/InventoryComponents.h"

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
            auto animationComp = entity->getComponent<AnimationComponent>();
            auto textComp = entity->getComponent<TextComponent>();
            auto shapeComp = entity->getComponent<ShapeComponent>();

            float offsetX = SCREEN_WIDTH / 2 - SCALE * WINDOW_WIDTH / 2;

            if (textureComp && positionComp) {
                auto posX = positionComp->getX();
                auto posY = positionComp->getY();

                if (originComp) {
                    textureComp->sprite.setOrigin(
                        originComp->getX() * SCALE,
                        originComp->getY() * SCALE
                    );
                }

                if (sizeComp) {
                    textureComp->sprite.setScale(
                        sizeComp->getWidth() * SCALE / textureComp->sprite.getLocalBounds().width,
                        sizeComp->getHeight() * SCALE / textureComp->sprite.getLocalBounds().height
                    );
                }
                
                textureComp->sprite.setPosition(posX * SCALE + offsetX, posY * SCALE);

                if (rotationComp) {
                    textureComp->sprite.setRotation(rotationComp->angle * 180 / PI);
                }

                if (healthComp && healthComp->isVisible()) {
                    showHealth(
                        window,
                        sizeComp->getWidth() * SCALE,
                        sizeComp->getHeight() * SCALE,
                        healthComp, positionComp,
                        SCALE
                    );
                }
                if (animationComp) {
                    int baseIndex = 0;
                    switch (animationComp->currentDirection) {
                        case 1: baseIndex = 0; break;
                        case 2: baseIndex = 3; break;
                        case 3: baseIndex = 6; break;
                        case 4: baseIndex = 9; break;
                        default: baseIndex = 1; break;
                    }

                    int textureX = (baseIndex + animationComp->currentFrameIndex) * animationComp->frameWidth;
                    int textureY = 0;

                    textureComp->sprite.setTexture(animationComp->texture);
                    textureComp->sprite.setTextureRect(sf::IntRect(textureX, textureY, animationComp->frameWidth, animationComp->frameHeight));
                }

                window->draw(textureComp->sprite);
            }
            if (textComp) {
                window->draw(textComp->getText());
            }

            if (shapeComp) {
                if (shapeComp->getType() == ShapeType::Ellipse) {
                    sf::CircleShape* ellipse = static_cast<sf::CircleShape*>(shapeComp->getShape());
                    ellipse->setPosition(positionComp->getX() * SCALE + offsetX, positionComp->getY() * SCALE);
                    ellipse->setRadius(sizeComp->getWidth() / 2);
                    ellipse->setFillColor(shapeComp->getColor());
                    ellipse->setScale(SCALE, float(sizeComp->getHeight()) / float(sizeComp->getWidth()) * SCALE);
                    window->draw(*ellipse);
                }
                if (shapeComp->getType() == ShapeType::Rectangle) {
                    sf::RectangleShape* rectangle = static_cast<sf::RectangleShape*>(shapeComp->getShape());
                    rectangle->setPosition(positionComp->getX() * SCALE + offsetX, positionComp->getY() * SCALE);
                    rectangle->setSize(sf::Vector2f(sizeComp->getWidth() * SCALE, sizeComp->getHeight() * SCALE));
                    rectangle->setFillColor(shapeComp->getColor());
                    window->draw(*rectangle);
                }
            }
        }

        window->display();
    }


private:
    sf::RenderWindow* window;
    EntityManager* entityManager;
    std::vector<Entity*> entities;
};
