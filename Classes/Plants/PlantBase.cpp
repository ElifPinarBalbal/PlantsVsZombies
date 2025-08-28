//
// Created by Elif Balbal on 19.08.2025.
//

#include "PlantBase.h"

#include "Zombies/ZombieBase.h"
#include "cocos/base/CCDirector.h"
#include "2d/CCActionEase.h"
#include "2d/CCActionInstant.h"

namespace plantDieAnimation
{
    constexpr float FALL_DURATION                   = 0.6f;
    constexpr float FALL_DELTA_ANGLE                = -90.f;
    constexpr float FALL_DELTA_POSITION_XX          = 0.f;
    constexpr float FALL_DELTA_POSITION_YY          = -10.f;
}

bool PlantBase::init()
{
    if (!Sprite::initWithFile(getImagePath())) return false;
    setupDefaults();
    return true;
}

void PlantBase::takeDamage(float dmg)
{
    if (dead_) return;
    hp_ -= dmg;
    if (hp_ <= 0.f) plantDie();
}

void PlantBase::setOnDeath(DeathCallback callback) {
    onDeath_ = std::move(callback);
    if (callback) {
        deathListeners_.push_back(std::move(callback));
    }
}


void PlantBase::plantDie()
{
    if (dead_) return;

    dead_ = true;

    cocos2d::log("Plant died");
    auto fall  = cocos2d::EaseSineIn::create(
                  cocos2d::Spawn::create(
                      cocos2d::RotateBy::create(plantDieAnimation::FALL_DURATION, plantDieAnimation::FALL_DELTA_ANGLE),
                      cocos2d::MoveBy::create(plantDieAnimation::FALL_DURATION, {plantDieAnimation::FALL_DELTA_POSITION_XX, -plantDieAnimation::FALL_DELTA_POSITION_YY}),
                      nullptr));
    auto fade  = cocos2d::FadeOut::create(0.35f);
    auto clean = cocos2d::CallFunc::create([this]{
        if (onDeath_) onDeath_(this);    // PlantController removes it
        for (auto& callback : deathListeners_) callback(this);

    });
    runAction(cocos2d::Sequence::create(fall, fade, clean, nullptr));
}

