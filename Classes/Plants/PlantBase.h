//
// Created by Elif Balbal on 19.08.2025.
//

#ifndef PLANTSVSZOMBIES_PEASHOOTER_H
#define PLANTSVSZOMBIES_PEASHOOTER_H
#include "2d/CCSprite.h"

class PlantBase :public cocos2d::Sprite{
public:
    CREATE_FUNC(PlantBase);

    bool init() override;

protected:
    virtual const char* spriteFile() const { return "peaShooter.png"; } // default image
    virtual void setupDefaults() {
        setScale(0.2f);
    }

};
#endif //PLANTSVSZOMBIES_PEASHOOTER_H