//
// Created by Elif Balbal on 19.08.2025.
//

#include "PlantBase.h"

#include "Zombies/ZombieBase.h"
#include "cocos/base/CCDirector.h"
#include "Projectiles/Pea.h"

bool PlantBase::init()
{
    if (!Sprite::initWithFile(spriteFile())) return false;
    setupDefaults();
    return true;
}

void PlantBase::startAutoFire(class ZombieBase *target, cocos2d::Node *world, int pea_Z_Order)
{
    if (!world) world = this->getParent();
    if (!world) return;

    // fire pea every 0.8 seconds
    this->schedule([this, target, world, pea_Z_Order](float) {
        if (!target || !target->getParent()) return;   // if no zombie, then no shoot

        // create pea - there is zombie :)
        auto* pea = Pea::create();
        if (!pea) return;

        const cocos2d::Vec2 muzzleOffset(40.f, 20.f);
        pea->setPosition(this->getPosition() + muzzleOffset);
        pea->setVelocity({300.f, 0.f});
        pea->giveDamage(20.f);
        pea->setTarget(target);
        world->addChild(pea, pea_Z_Order);
        pea->start();
       }, 0.8f, "auto_fire_timer");
}

