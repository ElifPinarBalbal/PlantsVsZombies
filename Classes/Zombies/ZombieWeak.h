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
    const char* spriteFile() const override { return "zombie.png"; } // I think we can also skip this --> it will use the default one (for this, it's the same)
    void setupDefaults() override {
        hp_    = 40.f;    // let's say, weaker than base
        speed_ = 20.f;
        stopX_ = 100.f;
        setScale(0.85f);
    }

};

#endif //PLANTSVSZOMBIES_ZOMBIEWEAK_H