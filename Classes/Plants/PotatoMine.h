//
// Created by Elif Balbal on 1.09.2025.
//

#ifndef PLANTSVSZOMBIES_POTATOMINE_H
#define PLANTSVSZOMBIES_POTATOMINE_H
class ZombieBase;

#include "PlantBase.h"

class PotatoMine : public PlantBase{
public:
    CREATE_FUNC(PotatoMine);
    void explode(class ZombieBase* target);

protected:
    const char* getImagePath() const override { return "potatoMine.png"; };
    void setupDefaults() override {
        setScale(0.65f);
        setHP(1000.f);   // to not be eaten until get exploded?
    }
};

#endif //PLANTSVSZOMBIES_POTATOMINE_H