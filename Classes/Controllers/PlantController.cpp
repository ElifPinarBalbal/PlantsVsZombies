// Created by Elif Balbal on 27.08.2025.
#include "PlantController.h"
#include "Plants/PlantBase.h"
#include <algorithm>
#include <limits>

void PlantController::addPlant(PlantBase *plant, int row)
{
    if (!plant) return;
    plantsByRow_[row].push_back(plant);

    plant->setOnDeath([this](PlantBase* plant){
        this->onPlantDeath(plant);
    });
}

void PlantController::onPlantDeath(PlantBase* plant)
{
    if (!plant) return;
    for (auto& pair : plantsByRow_) {
        auto& vectorPlant = pair.second;
        vectorPlant.erase(std::remove(vectorPlant.begin(), vectorPlant.end(), plant), vectorPlant.end());
    }
    if (plant->getParent()) {
        plant->removeFromParent();
    }
}

const std::vector<PlantBase*>& PlantController::getPlantsInRow(int row) const
{
    static const std::vector<PlantBase*> kEmpty;  // constant naming
    auto plantsInRow = plantsByRow_.find(row);
    return (plantsInRow == plantsByRow_.end()) ? kEmpty : plantsInRow->second;
}

PlantBase* PlantController::getNearestPlantInRow(int row, float xMax) const
{
    auto plantInRow = plantsByRow_.find(row);
    if (plantInRow == plantsByRow_.end()) return nullptr;

    PlantBase* nearestPlant = nullptr;
    float maxX = -std::numeric_limits<float>::infinity();   // I used this assigning to max (to compare) - but I'm not sure that is it the best practice :)

    for (auto* plant : plantInRow->second) {
        if (!plant || !plant->getParent() || plant->isDead()) continue;
        const float px = plant->getPositionX();
        if (px <= xMax && px > maxX) {
            maxX = px;
            nearestPlant  = plant;
        }
    }
    return nearestPlant;
}
