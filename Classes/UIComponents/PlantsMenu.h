//
// Created by Elif Balbal on 1.09.2025.
//

#ifndef PLANTSVSZOMBIES_PLANTSMENU_H
#define PLANTSVSZOMBIES_PLANTSMENU_H

#include "PlantCard.h"
#include <vector>
#include <unordered_map>

enum class PlantType {PeaShooter, PotatoMine};

class PlantsMenu : public cocos2d::Node{
public:
    CREATE_FUNC(PlantsMenu);
    bool init() override;

    PlantCard* addSlot(PlantType plantType, int plantCost, const std::string& plantImage, float imageScale = 0.35f);
    void setSunAmount(int sunAmount);    // to enable or disable the plant card

    bool hasPlantSelection() const { return selectedPlant_ != nullptr; }
    PlantType selectedPlantType() const { return plantsAndTypes_.at(selectedPlant_); }
    int selectedPlantCost() const { return selectedPlant_ ? selectedPlant_->getPlantCardCost() : 0; }
    void clearSelection();
    std::function<void(PlantType, PlantCard*)> onCardDragBegan;

protected:

    void relayoutPlantsMenu();
    void refreshSunAmountRemaining();

    PlantCard* selectedPlant_ = nullptr;
    std::vector<PlantCard*> plantCards_;
    std::unordered_map<PlantCard*, PlantType> plantsAndTypes_;
    int sunAmount_ = 0;
};

#endif //PLANTSVSZOMBIES_PLANTSMENU_H