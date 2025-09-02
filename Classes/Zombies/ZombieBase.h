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
    void zombieTakeDamage(float dmg);
    bool isDead()                   {return dead_;}
    void zombieDie();

    // death callback that invokes at the end
    using DeathCallback = std::function<void(ZombieBase*)>;
    void setOnDeath(DeathCallback callback) {onDeath_ = std::move(callback);}

    // another callback for update score label after zombie died
    using DeathCallbackScore = std::function<void(ZombieBase*)>;
    void addOnDeath(DeathCallbackScore callback);

    // movement
    void setSpeed(float s)          {speed_ = s;}
    void setStopX(float x)          {stopX_ = x;}
    float getStopX() const          { return stopX_; }

    int getRow() const;
    void zombieSetRow(int row);

    void zombieStartEating(class PlantBase* plant);
    void zombieStopEating();
    bool isEating() const { return eatingTarget_ != nullptr; }

protected:
    virtual const char* getImagePath() const { return "zombie.png"; }
    virtual void setupDefaults() {
        hp_         = 100.f;
        speed_      = 20.f;
        stopX_      = 100.f;
        setScale(0.75f);
    }
    PlantBase* eatingTarget_    =   nullptr;
    float maxHp_                =   100.f;
    float hp_                   =   100.f;
    float speed_                =   20.f;
    float stopX_                =   100.f;
    float biteInterval_         =   0.8f;
    float biteDamage_           =   20.f;
    float savedStopX_           =   0.f;
    int row_                    =   -1;
    bool dead_                  =   false;

    DeathCallback onDeath_{nullptr};
    std::vector<DeathCallbackScore> deathListeners_;       // notify scene for score update
};

#endif //PLANTSVSZOMBIES_ZOMBIE_H