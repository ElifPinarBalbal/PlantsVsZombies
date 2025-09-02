//
// Created by Elif Balbal on 20.08.2025.
//

#include "PeaShooter.h"
#include "Zombies/ZombieBase.h"
#include "cocos/base/CCDirector.h"
#include "Projectiles/Pea.h"

namespace intervals
{
    constexpr float SHOOT_INTERVAL_SECONDS = 0.8f;
}

void PeaShooter::startAutoFire(class ZombieBase *target, cocos2d::Node *world, int pea_Z_Order)
{
    if (!world) world = this->getParent();
    if (!world) return;

    if (!target || !target->getParent()) return;   // if no zombie, then no shoot

    currentTarget_ = target;
    this->unschedule("auto_fire_timer");
    // fire pea every 0.8 seconds
    this->schedule([this, target, world, pea_Z_Order](float) {
        if (!this->getParent() || !target || !target->getParent() || target->isDead() ) {
            this->unschedule("auto_fire_timer");
            currentTarget_ = nullptr;  // zombie öldü
            return;
        }

        // STOP when zombie is no longer in front of the peashooter
        if (target->getPositionX() < this->getPositionX()) {
            this->unschedule("auto_fire_timer");
            CCLOG("PeaShooter: stopped firing - zombie passed it");
            return;
        }

        // create pea - there is zombie :)
        auto* pea = Pea::create();
        if (!pea) return;

        const cocos2d::Vec2 muzzleOffset(40.f, 20.f);
        pea->setPosition(this->getPosition() + muzzleOffset);
        pea->setVelocity({300.f, 0.f});
        pea->giveDamage(20.f);
        pea->setTarget(target);
        world->addChild(pea, pea_Z_Order);
        pea->startShootingProjectile();
       }, intervals::SHOOT_INTERVAL_SECONDS, "auto_fire_timer");
}
