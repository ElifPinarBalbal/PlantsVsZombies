//
// Created by Elif Balbal on 19.08.2025.
//

#include "ZombieBase.h"

bool ZombieBase::init() {
    if (!Sprite::initWithFile(spriteFile())) return false;
    setupDefaults();
    scheduleUpdate();  // we used scheduler to move the zombie periodically
    return true;
}

void ZombieBase::update(float dt) {
    float x = this->getPositionX();
    x -= dt * speed_;
    if (x <= stopX_) {
        x = stopX_;
    }
    setPositionX(x);
}