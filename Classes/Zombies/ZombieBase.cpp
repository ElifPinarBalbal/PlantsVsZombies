//
// Created by Elif Balbal on 19.08.2025.
//

#include "ZombieBase.h"
#include "2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCActionEase.h"
#include <functional>
#include "Controllers/ZombieController.h"

namespace zombieDieAnimation
{
    constexpr float FALL_DURATION                   = 0.6f;
    constexpr float FALL_DELTA_ANGLE                = -90.f;
    constexpr float FALL_DELTA_POSITION_XX          = 0.f;
    constexpr float FALL_DELTA_POSITION_YY          = -10.f;
}

bool ZombieBase::init() {
    if (!Sprite::initWithFile(getImagePath())) return false;
    setupDefaults();
    scheduleUpdate();  // we used scheduler to move the zombie periodically
    return true;
}

void ZombieBase::update(float dt)
{
    if (isDead()) return;
    float x = this->getPositionX();
    x -= dt * speed_;
    if (x <= stopX_) {
        x = stopX_;
    }
    setPositionX(x);
}

void ZombieBase::takeDamage(float dmg)
{
    if (isDead()) return;
    hp_ -= dmg;
    //cocos2d::log("Zombie HP: %.1f", hp_);
    if (hp_ <= 0.f) zombieDie();
}

void ZombieBase::zombieDie()
{
    if (isDead()) return;

    dead_ = true;
    unscheduleUpdate();

    auto fall  = cocos2d::EaseSineIn::create(
                  cocos2d::Spawn::create(
                      cocos2d::RotateBy::create(zombieDieAnimation::FALL_DURATION, zombieDieAnimation::FALL_DELTA_ANGLE),
                      cocos2d::MoveBy::create(zombieDieAnimation::FALL_DURATION, {zombieDieAnimation::FALL_DELTA_POSITION_XX, -zombieDieAnimation::FALL_DELTA_POSITION_YY}),
                      nullptr));
    auto fade  = cocos2d::FadeOut::create(0.35f);
    auto clean = cocos2d::CallFunc::create([this]{
            if (onDeath_) onDeath_(this);
        });  // clean - notifies the controller and pass the dying zombie with "this"

    runAction(cocos2d::Sequence::create(fall, fade, clean, nullptr));
}