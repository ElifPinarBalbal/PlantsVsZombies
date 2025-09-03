//
// Created by Elif Balbal on 29.08.2025.
//
#include "PlantCard.h"
#include "2d/CCDrawNode.h"
#include "2d/CCSprite.h"
#include "2d/CCLabel.h"
#include "CCEventListenerTouch.h"
#include "base/CCEventDispatcher.h"

namespace zOrders
{
    constexpr int PLANT_CARD_BOX_GEOMETRY_Z_ORDER = 0;
    constexpr int PLANT_CARD_IMAGE_Z_ORDER = 1;
    constexpr int PLANT_CARD_COST_LABEL_Z_ORDER = 2;
}
bool PlantCard::init() {
    setCascadeOpacityEnabled(true);

    plantCardBoxGeometry_ = cocos2d::DrawNode::create();
    addChild(plantCardBoxGeometry_, zOrders::PLANT_CARD_BOX_GEOMETRY_Z_ORDER);

    plantCardCostLabel_ = cocos2d::Label::createWithTTF("0", "fonts/Marker Felt.ttf", 20);
    plantCardCostLabel_->setAnchorPoint({1,1});
    addChild(plantCardCostLabel_, zOrders::PLANT_CARD_COST_LABEL_Z_ORDER);

    setPlantCardSize({110.f, 75.f});
    setPlantCardCost(0);
    setPlantCardImageScale(plantCardImageScale_);
    setPlantCardEnabled(false);
    setPlantCardSelected(false);
    enablePlantTouch();
    return true;
}

void PlantCard::setPlantCardImage(const std::string& plantCardImage) {
    plantCardImagePath_ = plantCardImage;
    if (!plantCardImage_) {
        plantCardImage_ = cocos2d::Sprite::create(plantCardImage);
        addChild(plantCardImage_, zOrders::PLANT_CARD_IMAGE_Z_ORDER);
    } else {
        plantCardImage_->setTexture(plantCardImage);
    }
    plantCardImage_->setScale(plantCardImageScale_);
    plantCardLayout();
}

void PlantCard::setPlantCardCost(int plantCardCost) {
    plantCardCost_ = plantCardCost;
    if (plantCardCostLabel_) plantCardCostLabel_->setString(std::to_string(plantCardCost));
    plantCardLayout();
}

void PlantCard::setPlantCardSize(const cocos2d::Size& size) {
    plantCardSize_ = size;
    plantCardLayout();
    redrawPlantCardBoxGeometry();
    setContentSize(plantCardSize_);
}

void PlantCard::setPlantCardEnabled(bool enabled) {
    plantCardEnabled_ = enabled;
    setOpacity(enabled ? 255 : 120);
}

void PlantCard::setPlantCardSelected(bool onSelected) {
    plantCardSelected_ = onSelected;
    redrawPlantCardBoxGeometry();
}

void PlantCard::setPlantCardImageScale(float plantCardImageScale) {
    plantCardImageScale_ = plantCardImageScale;
    if (plantCardImage_) {
        plantCardImage_->setScale(plantCardImageScale_);
    }}

void PlantCard::redrawPlantCardBoxGeometry() {
    plantCardBoxGeometry_->clear();

    const cocos2d::Color4F fill(0.95f, 0.92f, 0.78f, 0.85f); // parchment-like
    const cocos2d::Color4F border = plantCardSelected_
                           ? cocos2d::Color4F(1.f, 0.9f, 0.3f, 1.f) // yellow if selected
                           : cocos2d::Color4F(1.f, 1.f, 1.f, 0.9f);

    const cocos2d::Vec2 bl(0,0), tr(plantCardSize_.width, plantCardSize_.height);
    plantCardBoxGeometry_->drawSolidRect(bl, tr, fill);
    plantCardBoxGeometry_->drawRect(bl + cocos2d::Vec2(1,1), tr - cocos2d::Vec2(1,1), border);
}

void PlantCard::plantCardLayout() {
    setAnchorPoint(cocos2d::Vec2::ANCHOR_TOP_LEFT);
    if (plantCardBoxGeometry_) { plantCardBoxGeometry_->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT); plantCardBoxGeometry_->setPosition({0,0}); }
    if (plantCardImage_) plantCardImage_->setPosition({30.f, plantCardSize_.height - 34.f});
    if (plantCardCostLabel_) plantCardCostLabel_->setPosition({plantCardSize_.width - 6.f, plantCardSize_.height - 6.f});
}

void PlantCard::enablePlantTouch() {
    auto* listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](cocos2d::Touch* t, cocos2d::Event*) -> bool {
        if (!plantCardEnabled_ || !getParent()) return false;
        cocos2d::Vec2 parent = getParent()->convertToNodeSpace(t->getLocation());
        cocos2d::Rect rect(getPositionX(),
               getPositionY() - getContentSize().height,
               getContentSize().width,
               getContentSize().height);
        if (!rect.containsPoint(parent)) return false;
        if (onPlantCardPressed) onPlantCardPressed(this);
        if (onDragBegan) onDragBegan(this);
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}


