//
// Created by Elif Balbal on 19.08.2025.
//

#ifndef PLANTSVSZOMBIES_ZOMBIE_H
#define PLANTSVSZOMBIES_ZOMBIE_H
#include "2d/CCSprite.h"


class Zombie : public cocos2d::Sprite{
public:
    CREATE_FUNC(Zombie);

    bool init() override;
    void update(float dt) override;
    void setHP(float hp)            {   hp_ = hp;   }
    float hp() const                 {   return hp_;    }
    void takeDamage(float dmg)      {  hp_ -= dmg; if(hp_ <= 0)removeFromParent();   }

    // movement
    void setSpeed(float s)          {speed_ = s;}
    void setStopX(float x)          {stopX_ = x;}

private:
    float hp_ =     50.f;
    float speed_ =  20.f;
    float stopX_ =  100.f;

};


#endif //PLANTSVSZOMBIES_ZOMBIE_H