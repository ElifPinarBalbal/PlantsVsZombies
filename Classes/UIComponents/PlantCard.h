//
// Created by Elif Balbal on 29.08.2025.
//

#ifndef PLANTSVSZOMBIES_PLANTCARD_H
#define PLANTSVSZOMBIES_PLANTCARD_H

#include "2d/CCNode.h"
#include <functional>
#include "cocos/2d/CCSprite.h"
#include "cocos/2d/CCLabel.h"

class PlantCard : public cocos2d::Node{
public:
    CREATE_FUNC(PlantCard);
    bool init() override;

    void setPlantCardImage(const std::string& plantCardImage);
    void setPlantCardCost(int plantCardCost);
    int getPlantCardCost() const { return plantCardCost_; }
    void setPlantCardSize(const cocos2d::Size& plantCardSize);

    void setPlantCardEnabled(bool plantCardEnabled);
    bool getPlantCardEnabled() const {return plantCardEnabled_;}

    void setPlantCardSelected(bool plantCardSelected);
    bool isPlantCardSelected() const {return plantCardSelected_;}

    void setPlantCardImageScale(float plantCardImageScale);
    float getPlantCardImageScale() const {return plantCardImageScale_;};

    const std::string& getPlantCardImagePath() const { return plantCardImagePath_; }

    // callback when the card is pressed or drag & dropped
    std::function<void(PlantCard*)> onPlantCardPressed;
    std::function<void(PlantCard*)> onDragBegan;

private:
    void redrawPlantCardBoxGeometry();
    void plantCardLayout();
    void enablePlantTouch();

    cocos2d::DrawNode* plantCardBoxGeometry_ = nullptr;
    cocos2d::Sprite*   plantCardImage_ = nullptr;
    cocos2d::Label*    plantCardCostLabel_ = nullptr;

    cocos2d::Size plantCardSize_{100.f, 60.f};
    int  plantCardCost_      = 0;
    bool plantCardEnabled_   = false;
    bool plantCardSelected_  = false;
    float plantCardImageScale_ = 0.20f;
    std::string plantCardImagePath_;

};

#endif //PLANTSVSZOMBIES_PLANTCARD_H