//
// Created by Elif Balbal on 19.08.2025.
//

#include "ZombieBase.h"
#include "2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCActionEase.h"
#include <functional>
#include "Plants/PlantBase.h"

namespace zombieDieAnimation
{
    constexpr float FALL_DURATION                   = 0.6f;
    constexpr float FALL_DELTA_ANGLE                = -90.f;
    constexpr float FALL_DELTA_POSITION_XX          = 0.f;
    constexpr float FALL_DELTA_POSITION_YY          = -10.f;
}

namespace zombieEat
{
    constexpr float ZOMBIE_STAND_OFF_OFFSET         = 50.f;
}

bool ZombieBase::init() {
    if (!Sprite::initWithFile(getImagePath())) return false;
    setupDefaults();

    baseScaleX_ = this->getScaleX();
    baseScaleY_ = this->getScaleY();
    baseOpacity_ = getOpacity();

    startWalkSqueeze();
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

    if (!isDead() && getPositionX() <= losePositionX_) {
        if (onReachedHouse) onReachedHouse(this);
    }
}


void ZombieBase::startWalkSqueeze()
{
    if (walkSqueezeAction_) return;

    const float sx1 = baseScaleX_ * 1.00f;
    const float sy1 = baseScaleY_ * 0.985f;
    const float sx2 = baseScaleX_ * 1.00f;
    const float sy2 = baseScaleY_ * 1.015f;

    auto down = cocos2d::EaseSineInOut::create(
                    cocos2d::ScaleTo::create(0.35f, sx1, sy1));
    auto up   = cocos2d::EaseSineInOut::create(
                    cocos2d::ScaleTo::create(0.35f, sx2, sy2));
    auto back = cocos2d::EaseSineInOut::create(
                    cocos2d::ScaleTo::create(0.35f, baseScaleX_, baseScaleY_));

    auto seq  = cocos2d::Sequence::create(down, up, back, nullptr);
    walkSqueezeAction_ = cocos2d::RepeatForever::create(seq);
    this->runAction(walkSqueezeAction_);
}

void ZombieBase::stopWalkSqueeze()
{
    if (walkSqueezeAction_) {
        this->stopAction(walkSqueezeAction_);
        walkSqueezeAction_ = nullptr;
    }
    this->setScaleX(baseScaleX_);
    this->setScaleY(baseScaleY_);
}

void ZombieBase::addHitFlashEffect()
{
    if (hitFlashEffect_) {
        stopAction(hitFlashEffect_);
        hitFlashEffect_ = nullptr;
    }
    auto flashUp = cocos2d::Spawn::create(
        cocos2d::TintTo::create(0.05f, 255, 255, 255),
        cocos2d::FadeTo::create(0.05f, baseOpacity_),
        cocos2d::ScaleTo::create(0.05f, baseScaleX_ * 1.01f, baseScaleY_ * 0.99f),
        nullptr
    );

    auto flashDown = cocos2d::Spawn::create(
        cocos2d::TintTo::create(0.08f, 255, 255, 255),
        cocos2d::FadeTo::create(0.08f, baseOpacity_),
        cocos2d::ScaleTo::create(0.08f, baseScaleX_, baseScaleY_),
        nullptr
    );

    hitFlashEffect_ = cocos2d::Sequence::create(
        flashUp, flashDown,
        cocos2d::CallFunc::create([this]{ hitFlashEffect_ = nullptr; }),
        nullptr
    );
    runAction(hitFlashEffect_);
}

int ZombieBase::getRow() const
{
    return row_;
}

void ZombieBase::zombieSetRow(int row) {
    row_ = row;
}

void ZombieBase::zombieTakeDamage(float dmg)
{
    if (isDead()) return;
    hp_ -= dmg;
    //cocos2d::log("Zombie HP: %.1f", hp_);
    if (hp_ <= 0.f) zombieDie();
}

void ZombieBase::zombieStartEating(PlantBase* plant)
{
    if (!plant || plant->isDead()) return;
    if (!eatingTarget_) savedStopX_ = stopX_;
    eatingTarget_ = plant;

    cocos2d::log("startEating");

    const float standOff = zombieEat::ZOMBIE_STAND_OFF_OFFSET;            // a LITTLE BIT front
    setStopX(plant->getPositionX() + standOff);

    this->unschedule("z_eat");
    this->schedule([this](float){
        if (isDead() || !eatingTarget_ || !eatingTarget_->getParent() || eatingTarget_->isDead()) {
            zombieStopEating();
            return;
        }
        eatingTarget_->takeDamage(biteDamage_);
    }, biteInterval_, "z_eat");
}

void ZombieBase::zombieStopEating()
{
    if (!eatingTarget_) return;
    unschedule("z_eat");
    eatingTarget_ = nullptr;
    setStopX(savedStopX_);
}

void ZombieBase::zombieDie()
{
    if (isDead()) return;

    dead_ = true;
    unscheduleUpdate();
    stopWalkSqueeze();
    if (hitFlashEffect_) { stopAction(hitFlashEffect_); hitFlashEffect_ = nullptr; }
    setOpacity(baseOpacity_);
    setScaleX(baseScaleX_);
    setScaleY(baseScaleY_);
    setColor(cocos2d::Color3B::WHITE);

    auto fall  = cocos2d::EaseSineIn::create(
                  cocos2d::Spawn::create(
                      cocos2d::RotateBy::create(zombieDieAnimation::FALL_DURATION, zombieDieAnimation::FALL_DELTA_ANGLE),
                      cocos2d::MoveBy::create(zombieDieAnimation::FALL_DURATION, {zombieDieAnimation::FALL_DELTA_POSITION_XX, -zombieDieAnimation::FALL_DELTA_POSITION_YY}),
                      nullptr));
    auto fade  = cocos2d::FadeOut::create(0.35f);
    auto clean = cocos2d::CallFunc::create([this]{
            if (onDeath_) onDeath_(this);
            for (auto& callback : deathListeners_) callback(this);  // notify Gamescene to update score
        });  // clean - notifies the controller and pass the dying zombie with "this"

    runAction(cocos2d::Sequence::create(fall, fade, clean, nullptr));
}

void ZombieBase::addOnDeath(DeathCallbackScore callback) {
    if (callback) deathListeners_.push_back(std::move(callback));
}