#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "../Location/LocationType.h"

class CollisionMap {
public:
    CollisionMap(const std::string& filePath, int cellWidth, int cellHeight) : cellWidth(cellWidth), cellHeight(cellHeight) {
        loadFromFile(filePath);
    }

    bool isCollision(float x, float y) const {
        int cellX = static_cast<int>(x / cellWidth);
        int cellY = static_cast<int>(y / cellHeight);
      
        if (cellX < 0 || cellX >= collisionMap[0].size() || cellY < 0 || cellY >= collisionMap.size()) {
            return true;
        }

        return collisionMap[cellY][cellX];
    }

    int getCellWidth() const {
        return cellWidth;
    }

    int getCellHeight() const {
        return cellHeight;
    }
    
    int getWidth() const {
        if(!collisionMap.empty()) return collisionMap[0].size();
        return 0;
    }
    
    int getHeight() const{
        return collisionMap.size();
    }

    const std::vector<std::vector<bool>>& getMap() const {
        return collisionMap;
    }
    
private:
    void loadFromFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filePath << std::endl;
            return;
        }

        collisionMap.clear();
        std::string line;
        while (std::getline(file, line)) {
            std::vector<bool> row;
            for (char c : line) {
                if (c == '#' || c == '-') {
                    row.push_back(c == '#');
                }
            }
            collisionMap.push_back(row);
        }

        file.close();
    }
    
private:
    std::vector<std::vector<bool>> collisionMap;
    int cellWidth;
    int cellHeight;
};

bool isMapCollision(std::unordered_map<LocationType, CollisionMap>* maps, LocationType location, float x, float y) {
	switch (location)
	{
	case LocationType::Town:
		return maps->at(LocationType::Town).isCollision(x, y);
		break;

	case LocationType::Forest:
		return maps->at(LocationType::Forest).isCollision(x, y);
		break;
	
	default:
		return false;
	}
}
