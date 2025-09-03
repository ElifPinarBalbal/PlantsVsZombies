//
// Created by Elif Balbal on 28.08.2025.
//

#ifndef PLANTSVSZOMBIES_SUNCONTROLLER_H
#define PLANTSVSZOMBIES_SUNCONTROLLER_H

#include "Sun/Sun.h"
#include "2d/CCActionInstant.h"
#include "2d/CCActionInterval.h"
#include "2d/CCActionEase.h"

namespace zOrders
{
    constexpr int SUN_Z_ORDER = 999;
}

class SunController : public cocos2d::Node{
public:
    CREATE_FUNC(SunController);
    bool init() override { scheduleSunSpawn(); return true; }

    void setFallArea(const cocos2d::Rect& rect) { fallArea_ = rect; }
    const std::vector<Sun*>& suns() const { return suns_; }

private:
    void scheduleSunSpawn() {
        this->schedule([this](float){
            auto* sun = Sun::create();
            this->addChild(sun, zOrders::SUN_Z_ORDER);
            sun->startSunFalling(fallArea_);
            suns_.push_back(sun);
            sun->addOnSunCollected([this](Sun* whichSun){
                suns_.erase(std::remove(suns_.begin(), suns_.end(), whichSun), suns_.end());
            });
            sun->runAction(cocos2d::Sequence::create(
                cocos2d::DelayTime::create(5.0f),
                cocos2d::CallFunc::create([this, sun]{
                    suns_.erase(std::remove(suns_.begin(), suns_.end(), sun), suns_.end());
                }),
                nullptr));
        }, 3.0f, "sun_spawn_timer");
    }

    cocos2d::Rect fallArea_;
    std::vector<Sun*> suns_;
};

#endif //PLANTSVSZOMBIES_SUNCONTROLLER_H