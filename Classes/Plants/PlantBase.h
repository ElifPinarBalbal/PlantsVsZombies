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

protected:
    virtual const char* getImagePath() const { return "peaShooter.png"; } // default image
    virtual void setupDefaults() {
        setScale(0.2f);
    }
};
#endif //PLANTSVSZOMBIES_PLANTBASE_H