//
// Created by Elif Balbal on 20.08.2025.
//

#ifndef PLANTSVSZOMBIES_PEASHOOTER_H
#define PLANTSVSZOMBIES_PEASHOOTER_H

#include "PlantBase.h"
#include "Projectiles/ProjectileBase.h"
#include "Zombies/ZombieBase.h"

class PeaShooter : public PlantBase {
public:
    CREATE_FUNC(PeaShooter);
    void startAutoFire(class ZombieBase* target, cocos2d::Node* world, int pea_Z_Order = zOrders::PEA_Z_ORDER);
    ZombieBase* currentTarget_ = nullptr;

    void setDamage(float dmg)     { damage_ = dmg; }
    float getDamage() const     { return damage_; }

protected:
    const char* getImagePath() const override { return "peaShooter.png"; };
    void setupDefaults() override {
        setScale(0.17f);
        damage_ = 20.f;
    }

    virtual ProjectileBase* createProjectile();

private:
    float damage_ = 20.f;
};

#endif //PLANTSVSZOMBIES_PEASHOOTER_H