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
            if ((*it)->id == entityId) {
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

        for (auto it = entities.begin(); it != entities.end(); ++it) {
            Entity* entity = *it;
            
            if (!entity || entityManager->getEntity(entity->id) != entity) {
                it = entities.erase(it);
                continue;
            }

            auto textureComp = entity->getComponent<TextureComponent>();
            auto positionComp = entity->getComponent<PositionComponent>();
            auto sizeComp = entity->getComponent<SizeComponent>();

            if (textureComp && positionComp) {
                textureComp->sprite.setPosition(positionComp->x, positionComp->y);

                if (sizeComp) {
                    textureComp->sprite.setScale(
                        sizeComp->width / textureComp->sprite.getLocalBounds().width,
                        sizeComp->height / textureComp->sprite.getLocalBounds().height
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