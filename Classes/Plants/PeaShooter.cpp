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
ProjectileBase* PeaShooter::createProjectile() {
    return Pea::create();
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
        auto* projectile = createProjectile();
        if (!projectile) return;

        const cocos2d::Vec2 muzzleOffset(40.f, 20.f);
        projectile->setPosition(this->getPosition() + muzzleOffset);
        projectile->setVelocity({300.f, 0.f});
        projectile->giveDamage(this->getDamage());
        projectile->setTarget(target);
        world->addChild(projectile, pea_Z_Order);
        projectile->startShootingProjectile();
       }, intervals::SHOOT_INTERVAL_SECONDS, "auto_fire_timer");
}
