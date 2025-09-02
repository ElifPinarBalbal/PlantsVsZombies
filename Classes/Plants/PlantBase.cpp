//
// Created by Elif Balbal on 19.08.2025.
//

#include "PlantBase.h"
#include "Zombies/ZombieBase.h"
#include "cocos/base/CCDirector.h"
#include "2d/CCActionEase.h"
#include "2d/CCActionInstant.h"

bool PlantBase::init()
{
    if (!Sprite::initWithFile(getImagePath())) return false;
    setupDefaults();
    return true;
}

void PlantBase::takeDamage(float dmg)
{
    if (isDead_) return;
    currentHp_ -= dmg;
    if (currentHp_ <= 0.f) plantDie();
}

void PlantBase::setOnDeath(DeathCallback callback) {
    onDeath_ = std::move(callback);
    if (callback) {
        deathListeners_.push_back(onDeath_);
    }
}

void PlantBase::plantDie()
{
    if (isDead_) return;
    isDead_ = true;

    cocos2d::log("Plant died");
    auto fall  = cocos2d::EaseSineIn::create(
                  cocos2d::Spawn::create(
                      cocos2d::RotateBy::create(0.6f, -90.f),
                      cocos2d::MoveBy::create(0.6f, {0.f, -10.f}),
                      nullptr));
    auto fade  = cocos2d::FadeOut::create(0.35f);
    auto clean = cocos2d::CallFunc::create([this]{
        if (onDeath_) onDeath_(this);    // PlantController removes it
        for (auto& callback : deathListeners_) callback(this);
    });
    runAction(cocos2d::Sequence::create(fall, fade, clean, nullptr));
}

