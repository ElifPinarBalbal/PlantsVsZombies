//
// Created by Elif Balbal on 19.08.2025.
//

#ifndef PLANTSVSZOMBIES_ZOMBIE_H
#define PLANTSVSZOMBIES_ZOMBIE_H
#include <utility>

#include "2d/CCSprite.h"

class ZombieBase : public cocos2d::Sprite{
public:
    CREATE_FUNC(ZombieBase);

    bool init() override;
    void update(float dt) override;
    void setHP(float hp)            { hp_ = hp; }
    float hp() const                { return hp_; }
    void takeDamage(float dmg);
    bool isDead()                   {return dead_;}
    void zombieDie();

    // death callback that invokes at the end
    using DeathCallback = std::function<void(ZombieBase*)>;
    void setOnDeath(DeathCallback callback) {onDeath_ = std::move(callback);}

    // movement
    void setSpeed(float s)          {speed_ = s;}
    void setStopX(float x)          {stopX_ = x;}

    int getRow() const;
    void setRow(int row);

protected:
    virtual const char* getImagePath() const { return "zombie.png"; }
    virtual void setupDefaults() {
        hp_    = 50.f;
        speed_ = 20.f;
        stopX_ = 100.f;
        setScale(0.85f);
    }

    float maxHp_    =   100.f;
    float hp_       =   100.f;
    float speed_    =   20.f;
    float stopX_    =   100.f;
    bool dead_      =   false;
    int row_        =   -1;

    DeathCallback onDeath_{nullptr};
};

#endif //PLANTSVSZOMBIES_ZOMBIE_H