#pragma once
#include "../Components/ParentComponent.h"
#include <string>

class ShopTypeEntityComponent : public Component {};
class ShopComponent : public Component {};
class ShopButtonBackComponent : public Component {};
class ShopButtonOkComponent : public Component {};
class ShopAvatarComponent : public Component {};
class PlayerAvatarComponent : public Component {};
class ShopItemComponent : public Component {
private:
    std::string name;
    int price;
    bool isSelected;
    bool isPokemon;
public:
    ShopItemComponent(const std::string& name, int price, bool isPokemon) 
        : name(name), price(price), isSelected(false), isPokemon(isPokemon) {}
    
    const std::string& getName() const { return name; }
    int getPrice() const { return price; }
    bool getIsSelected() const { return isSelected; }
    bool getIsPokemon() const { return isPokemon; }
    
    void setSelected(bool selected) { isSelected = selected; }
}; 