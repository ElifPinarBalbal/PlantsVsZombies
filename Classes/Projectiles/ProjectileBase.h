//
// Created by Elif Balbal on 19.08.2025.
//

#ifndef PLANTSVSZOMBIES_PROJECTILEBASE_H
#define PLANTSVSZOMBIES_PROJECTILEBASE_H
#include "2d/CCSprite.h"
#include "base/CCVector.h"
#include "Zombies/ZombieBase.h"

class ProjectileBase : public cocos2d::Sprite{
public:
    CREATE_FUNC(ProjectileBase);

    bool init() override;

    void setVelocity(const cocos2d::Vec2& v) { velocity_ = v; }
    void giveDamage(float d) { damage_ = d; }
    void setTarget(ZombieBase* z) { target_ = z; }

    void startShootingProjectile();

protected:  // can be overriden by subclasses-only
    virtual const char* spriteFile() const { return "pea.png"; } // default image
    virtual void setupDefaults() {
        setScale(1.f);
    }
    virtual void onProjectileHitZombie(ZombieBase* z);

private:
    cocos2d::Vec2 velocity_         {   300.f, 0.f  };
    float damage_                   {   20.f    };
    ZombieBase* target_             {   nullptr };

    bool IsPeaOutOfBounds() const;
    bool IsHittingTarget() const;

};


#endif //PLANTSVSZOMBIES_PROJECTILEBASE_H