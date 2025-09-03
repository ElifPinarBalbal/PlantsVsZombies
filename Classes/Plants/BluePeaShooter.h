//
// Created by Elif Balbal on 2.09.2025.
//

#ifndef PLANTSVSZOMBIES_BLUEPEASHOOTER_H
#define PLANTSVSZOMBIES_BLUEPEASHOOTER_H
#include "PeaShooter.h"
#include "Projectiles/BluePea.h"

class BluePeaShooter : public PeaShooter {
public:
    CREATE_FUNC(BluePeaShooter);
protected:
    const char* getImagePath() const override { return "peaShooterBlue.png"; }
    void setupDefaults() override {
        setScale(0.25f);
        setDamage(40.f);   // stronger
    }

    ProjectileBase* createProjectile() override{
        return BluePea::create();
    }
};
#endif //PLANTSVSZOMBIES_BLUEPEASHOOTER_H