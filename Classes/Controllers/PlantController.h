//
// Created by Elif Balbal on 27.08.2025.
//

#ifndef PLANTSVSZOMBIES_PLANTCONTROLLER_H
#define PLANTSVSZOMBIES_PLANTCONTROLLER_H
#include <unordered_map>
#include <vector>
#include "cocos/2d/CCNode.h"

class PlantBase;
class PeaShooter;

class PlantController : public cocos2d::Node {
public:
    CREATE_FUNC(PlantController);
    bool init() override {return true;}
    void addPlant(PlantBase* plant, int row);

    // death callback
    void onPlantDeath(PlantBase* plant);

    const std::vector<PlantBase*>& getPlantsInRow(int row) const;
    PlantBase* getNearestPlantInRow(int row, float xMin) const;

private:
    std::unordered_map<int, std::vector<PlantBase*>> plantsByRow_;
};

#endif //PLANTSVSZOMBIES_PLANTCONTROLLER_H