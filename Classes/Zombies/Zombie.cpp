//
// Created by Elif Balbal on 19.08.2025.
//

#include "Zombie.h"

bool Zombie::init() {
    if (!Sprite::initWithFile("zombie.png")) return false;
    setScale(0.85f);
    scheduleUpdate();  // we used scheduler to move the zombie periodically
    return true;
}

void Zombie::update(float dt) {
    float x = this->getPositionX();
    x -= dt * speed_;
    if (x <= stopX_) {
        x = stopX_;
    }
    setPositionX(x);
}