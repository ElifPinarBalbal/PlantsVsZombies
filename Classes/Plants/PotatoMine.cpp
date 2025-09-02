//
// Created by Elif Balbal on 1.09.2025.
//

#include "PotatoMine.h"
#include "Zombies/ZombieBase.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCActionInstant.h"

void PotatoMine::explode(class ZombieBase *target) {
    if (!target || target->isDead() || this->isDead()) return;

    // explosion animation
    auto scale = cocos2d::ScaleBy::create(0.2f, 1.5f);
    auto fade  = cocos2d::FadeOut::create(0.2f);
    auto killBoth = cocos2d::CallFunc::create([this, target]() {
        target->zombieDie();      // kill zombie
        this->plantDie();   // kill plant (controller removes it)
    });

    this->runAction(cocos2d::Sequence::create(cocos2d::Spawn::create(scale, fade, nullptr),
                                     killBoth, nullptr));
}
