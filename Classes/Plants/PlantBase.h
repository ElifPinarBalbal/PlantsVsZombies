//
// Created by Elif Balbal on 19.08.2025.
//

#ifndef PLANTSVSZOMBIES_PLANTBASE_H
#define PLANTSVSZOMBIES_PLANTBASE_H
#include "2d/CCSprite.h"

namespace zOrders
{
    constexpr int PEA_Z_ORDER = 8;
}
class PlantBase : public cocos2d::Sprite{
public:
    CREATE_FUNC(PlantBase);
    bool init() override;
    void setHP(float hp)            { hp_ = hp; }
    float hp() const                { return hp_; }

protected:
    virtual const char* getImagePath() const { return "peaShooter.png"; } 
    virtual void setupDefaults() {
        hp_    = 100.f;
        setScale(0.17f);
    }

    float hp_       =   100.f;
    float maxHp_    =   100.f;
    bool dead_      =   false;

};
#endif //PLANTSVSZOMBIES_PLANTBASE_H