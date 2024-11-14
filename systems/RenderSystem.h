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

    // Добавляем сущность в систему
    void addEntity(Entity* entity) {
        entities.push_back(entity);
    }

    // Удаляем сущность из системы по ID
    void removeEntity(int entityId) {
        for (auto it = entities.begin(); it != entities.end(); ++it) {
            if ((*it)->id == entityId) {
                entities.erase(it);
                break;  // Выходим из цикла после удаления
            }
        }
    }

    // Метод для рендеринга всех зарегистрированных сущностей
    void render() {
        if (!window) return;

        window->clear(COLOR_WHITE);

        for (auto it = entities.begin(); it != entities.end(); ) {
            Entity* entity = *it;
            
            // Проверка на существование сущности в EntityManager
            if (!entity || entityManager->getEntity(entity->id) != entity) {
                it = entities.erase(it); // Удаляем из списка, если сущность недействительна
                continue;
            }

            // Проверка на существование компонентов
            auto textureComp = entity->getComponent<TextureComponent>();
            auto positionComp = entity->getComponent<PositionComponent>();
            auto sizeComp = entity->getComponent<SizeComponent>();

            if (textureComp && positionComp) {
                // Устанавливаем позицию спрайта перед рендером
                textureComp->sprite.setPosition(positionComp->x, positionComp->y);

                if (sizeComp) {
                    textureComp->sprite.setScale(
                        sizeComp->width / textureComp->sprite.getLocalBounds().width,
                        sizeComp->height / textureComp->sprite.getLocalBounds().height
                    );
                }
                window->draw(textureComp->sprite);
            }
            ++it; // Переход к следующей сущности
        }
        window->display();
    }

private:
    sf::RenderWindow* window;
    EntityManager* entityManager;
    std::vector<Entity*> entities;  // Список сущностей для рендеринга
};