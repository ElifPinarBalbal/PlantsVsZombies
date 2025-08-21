//
// Created by Elif Balbal on 19.08.2025.
//

#include "PlantBase.h"

#include "Zombies/ZombieBase.h"
#include "cocos/base/CCDirector.h"


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
        auto* pea = cocos2d::Sprite::create("pea.png");
        if (!pea) return;

        const cocos2d::Vec2 muzzleOffset(40.f, 20.f);
        pea->setPosition(this->getPosition() + muzzleOffset);
        pea->setScale(1.0f);
        world->addChild(pea, pea_Z_Order);

        const float peaSpeed = 300.0f;

        pea->schedule([pea, target, peaSpeed] (float dt) {
            if (!pea->getParent()) return;
            pea->setPositionX(pea->getPositionX() + peaSpeed * dt);

            auto* dir   = cocos2d::Director::getInstance();
            const auto vs = dir->getVisibleSize();
            const auto org = dir->getVisibleOrigin();
            if (pea->getPositionX() > org.x + vs.width + 40.f) {
                pea->removeFromParent();
                return;
            }

            if (target && target->getParent()) {
                   auto inset = [] (cocos2d::Rect r, float px, float py) {
                       r.origin.x += px; r.origin.y += py;
                       r.size.width  -= 2*px;
                       r.size.height -= 2*py;
                       return r;
                   };
                   auto peaRect    = inset(pea->getBoundingBox(),    pea->getContentSize().width  * 0.1f, pea->getContentSize().height * 0.30f);
                   auto zombieRect = inset(target->getBoundingBox(), target->getContentSize().width * 0.50f, target->getContentSize().height * 0.20f);

                   if (peaRect.intersectsRect(zombieRect)) {
                       target->takeDamage(20.f);
                       pea->removeFromParent();
                   }
               }
           }, "pea_step");
       }, 0.8f, "auto_fire_timer");
}

