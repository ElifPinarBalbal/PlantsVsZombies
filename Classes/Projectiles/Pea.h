//
// Created by Elif Balbal on 21.08.2025.
//

#ifndef PLANTSVSZOMBIES_PEA_H
#define PLANTSVSZOMBIES_PEA_H

#include "ProjectileBase.h"


class Pea : public ProjectileBase{
public:
    CREATE_FUNC(Pea);
    bool init() override {
        if (!ProjectileBase::init()) return false;
        return true;
    }

protected:
    const char* spriteFile() const override { return "pea.png"; }
    void setupDefaults() override {
        setScale(0.9f);
    }
};


#endif //PLANTSVSZOMBIES_PEA_H