#pragma once
#include "../Components/ParentComponent.h"
#include <string>

class ShopTypeEntityComponent : public Component {};
class ShopComponent : public Component {};
class ShopButtonBackComponent : public Component {};
class ShopButtonOkComponent : public Component {};
class ShopAvatarComponent : public Component {};
class PlayerAvatarComponent : public Component {};
class ShopSelectedBackgroundComponent : public Component {};
class ShopItemComponent : public Component {
private:
    std::string name;
    int price;
    bool isSelected;
    bool isPokemon;
    int buyCount;
    int sellCount;
public:
    ShopItemComponent(const std::string& name, int price, bool isPokemon) 
        : name(name), price(price), isSelected(false), isPokemon(isPokemon),
          buyCount(0), sellCount(0) {}
    
    const std::string& getName() const { return name; }
    int getPrice() const { return price; }
    bool getIsSelected() const { return isSelected; }
    bool getIsPokemon() const { return isPokemon; }
    
    void setSelected(bool selected) { isSelected = selected; }
    
    int getBuyCount() const { return buyCount; }
    int getSellCount() const { return sellCount; }
    void setBuyCount(int count) { buyCount = count; }
    void setSellCount(int count) { sellCount = count; }
}; 