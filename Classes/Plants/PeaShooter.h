//
// Created by Elif Balbal on 20.08.2025.
//

#ifndef PLANTSVSZOMBIES_PEASHOOTER_H
#define PLANTSVSZOMBIES_PEASHOOTER_H

#include "PlantBase.h"

class PeaShooter : public PlantBase {
public:
    CREATE_FUNC(PeaShooter);

protected:
    const char* spriteFile() const override { return "peaShooter.png"; };
    void setupDefaults() override {
        setScale(0.2f);
    }

};

#endif //PLANTSVSZOMBIES_PEASHOOTER_H