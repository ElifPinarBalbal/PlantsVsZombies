//
// Created by Elif Balbal on 1.09.2025.
//

#include "PlantsMenu.h"

bool PlantsMenu::init(){
    setCascadeOpacityEnabled(true);
    return true;
}

PlantCard *PlantsMenu::addSlot(PlantType plantType, int plantCost, const std::string &plantImage, float imageScale)
{
    auto* plantCard = PlantCard::create();
    plantCard->setPlantCardImageScale(imageScale);
    plantCard->setPlantCardImage(plantImage);
    plantCard->setPlantCardCost(plantCost);
    plantCard->setPlantCardEnabled(sunAmount_ >= plantCost); // if our sunAmount is enough for that type of plant, set the card enabled

    plantCard->onPlantCardPressed = [this] (PlantCard* plantCard) {
        if (!plantCard->getPlantCardEnabled()) return;  // already not pressable
        for (auto* card : plantCards_) card->setPlantCardSelected(false);
        plantCard->setPlantCardSelected(true);
        selectedPlant_ = plantCard;
    };

    plantCard->onDragBegan = [this] (PlantCard* plantCard) {
        auto type = plantsAndTypes_.at(plantCard);
        if (onCardDragBegan) onCardDragBegan(type, plantCard);
    };

    this->addChild(plantCard, 0);
    plantCards_.push_back(plantCard);
    plantsAndTypes_[plantCard] = plantType;
    relayoutPlantsMenu();
    return plantCard;
}

void PlantsMenu::setSunAmount(int sunAmount) {
    sunAmount_ = sunAmount;
    refreshSunAmountRemaining();
}

void PlantsMenu::clearSelection() {
    if (selectedPlant_) selectedPlant_->setPlantCardSelected(false);
    selectedPlant_ = nullptr;
}

void PlantsMenu::refreshSunAmountRemaining() {
    for (auto* card : plantCards_) {
        const int cost = card->getPlantCardCost();
        const bool isAffordable = sunAmount_ >= cost;
        card->setPlantCardEnabled(isAffordable);

        if (!isAffordable && card == selectedPlant_) {
            card->setPlantCardSelected(false);
            selectedPlant_ = nullptr;
        }
    }
}

void PlantsMenu::relayoutPlantsMenu() {
    float yIndex = 0.f;
    const float gapBetweenCards = 10.f;
    for (auto* card : plantCards_) {
        card->setAnchorPoint(cocos2d::Vec2::ANCHOR_TOP_LEFT);
        card->setPosition({0.f, -yIndex});
        yIndex += card->getContentSize().height + gapBetweenCards;
    }
    setContentSize({ plantCards_.empty()?0.f:plantCards_.front()->getContentSize().width,
                     plantCards_.empty()?0.f:yIndex-gapBetweenCards });
}


