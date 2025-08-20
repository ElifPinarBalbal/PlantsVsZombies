//
// Created by Elif Balbal on 19.08.2025.
//

#ifndef PLANTSVSZOMBIES_ZOMBIE_H
#define PLANTSVSZOMBIES_ZOMBIE_H
#include "2d/CCSprite.h"

class ZombieBase : public cocos2d::Sprite{
public:
    CREATE_FUNC(ZombieBase);

    bool init() override;
    void update(float dt) override;
    void setHP(float hp)            { hp_ = hp; }
    float hp() const                { return hp_; }
    void takeDamage(float dmg)      { hp_ -= dmg; }
    bool isDead() const             { if (hp_ <= 0.f) return true; }

    // movement
    void setSpeed(float s)          {speed_ = s;}
    void setStopX(float x)          {stopX_ = x;}

protected:  // can be overriden by subclasses-only
    virtual const char* spriteFile() const { return "zombie.png"; } // default image
    virtual void setupDefaults() {
        hp_    = 50.f;
        speed_ = 20.f;
        stopX_ = 100.f;
        setScale(0.85f);
    }

    float hp_ =     50.f;
    float speed_ =  20.f;
    float stopX_ =  100.f;

};


#endif //PLANTSVSZOMBIES_ZOMBIE_H