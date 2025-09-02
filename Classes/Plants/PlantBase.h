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
    void setHP(float hp)            { currentHp_ = hp; }
    float getHP() const                { return currentHp_; }

    void takeDamage(float dmg);
    bool isDead() const { return isDead_; }
    void plantDie();

    using DeathCallback = std::function<void(PlantBase*)>;
    void setOnDeath(DeathCallback callback);

protected:
    virtual const char* getImagePath() const { return "peaShooter.png"; } 
    virtual void setupDefaults() {
        currentHp_    = 100.f;
        setScale(0.17f);
    }

    float currentHp_        =   100.f;
    float maxHp_            =   100.f;
    bool isDead_            =   false;

    DeathCallback onDeath_{nullptr};
    std::vector<DeathCallback> deathListeners_;  // I added this listener to notify that a plant is dead to other classes
};
#endif //PLANTSVSZOMBIES_PLANTBASE_H