//
// Created by Elif Balbal on 19.08.2025.
//

#ifndef PLANTSVSZOMBIES_PEASHOOTER_H
#define PLANTSVSZOMBIES_PEASHOOTER_H
#include "2d/CCSprite.h"

class PeaShooter : cocos2d::Sprite{
public:
    CREATE_FUNC(PeaShooter);
    bool init() override;
    void update(float dt) override;
};
#endif //PLANTSVSZOMBIES_PEASHOOTER_H