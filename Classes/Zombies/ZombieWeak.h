//
// Created by Elif Balbal on 20.08.2025.
//

#ifndef PLANTSVSZOMBIES_ZOMBIEWEAK_H
#define PLANTSVSZOMBIES_ZOMBIEWEAK_H

#include "ZombieBase.h"
class ZombieWeak : public ZombieBase {
public:
    CREATE_FUNC(ZombieWeak);

protected:
    const char* getImagePath() const override { return "zombie.png"; }
    void setupDefaults() override {
        hp_    = 100.f;
        speed_ = 20.f;
        stopX_ = 100.f;
        setScale(0.85f);
    }

};

#endif //PLANTSVSZOMBIES_ZOMBIEWEAK_H