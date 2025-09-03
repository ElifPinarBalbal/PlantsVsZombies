//
// Created by Elif Balbal on 3.09.2025.
//

#ifndef PLANTSVSZOMBIES_BLUEPEA_H
#define PLANTSVSZOMBIES_BLUEPEA_H
#include "ProjectileBase.h"

class BluePea : public ProjectileBase {
public:
    CREATE_FUNC(BluePea);
    bool init() override {
        if (!ProjectileBase::init()) return false;
        return true;
    }

protected:
    const char* getImagePath() const override { return "peaBlue.png"; }
    void setupDefaults() override {
        setScale(0.13f);
    }
};
#endif //PLANTSVSZOMBIES_BLUEPEA_H