//
// Created by Elif Balbal on 28.08.2025.
//

#include "Sun.h"
#include "CCEventDispatcher.h"
#include "CCEventListenerTouch.h"
#include "2d/CCActionInstant.h"
#include "2d/CCActionInterval.h"
#include "2d/CCActionEase.h"

using namespace cocos2d;

bool Sun::init() {
    if (!Sprite::initWithFile("sun.png")) return false;
    setScale(0.35f);
    setOpacity(240);
    enableTouchToSun();
    return true;
}

void Sun::enableTouchToSun() {
    auto* listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](cocos2d::Touch* t, cocos2d::Event*) {
        if (!getParent()) return false;
        cocos2d::Vec2 p = getParent()->convertToNodeSpace(t->getLocation());
        if (getBoundingBox().containsPoint(p)) { collectSunNow(); return true; }
        return false;
    };
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Sun::collectSunNow() {
    stopAllActions();
    runAction(cocos2d::Sequence::create(
        cocos2d::Spawn::create(cocos2d::ScaleTo::create(0.12f, getScale()*1.3f), FadeOut::create(0.12f), nullptr),
        CallFunc::create([this]{
            for (auto& cb : collectedSunListeners_) cb(this);
            removeFromParent();  // after collecting delete
        }),
        nullptr));
}

void Sun::startSunFalling(const cocos2d::Rect &area) {
    // let's determine the start position X for sun
    float sunStartPosX = RandomHelper::random_real(area.getMinX() + 30.f , area.getMaxX() - 30.f);
    setPosition(sunStartPosX, area.getMaxY() + 30.f);

    // I want my sun to draw zigzags -- so I defined segments
    const int segments = 6;
    float segmentDistanceY = (area.size.height + 120.f) / segments;
    Vector<FiniteTimeAction*> moves;

    float direction = (RandomHelper::random_int(0, 1) == 0) ? -1.f : 1.f;

    for (int i = 0; i < segments; ++i) {
        float distanceX = direction * RandomHelper::random_real(40.f, 90.f);
        // keep inside play area horizontally
        float nextXPos = clampf(getPositionX() + distanceX, area.getMinX() + 20.f, area.getMaxX() - 20.f);
        distanceX = nextXPos - getPositionX();
        moves.pushBack(EaseSineInOut::create(MoveBy::create(1.f, {distanceX, -segmentDistanceY})));
        direction *= -1.f;
    }

    // make the animation
    // if not collected in time, fade away
    runAction(Sequence::create(
        Sequence::create(moves),
        DelayTime::create(1.0f),
        FadeOut::create(0.25f),
        CallFunc::create([this]{ removeFromParent(); }),
        nullptr));
}
