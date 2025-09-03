//
// Created by Elif Balbal on 25.08.2025.
//

#ifndef PLANTSVSZOMBIES_ZOMBIECONTROLLER_H
#define PLANTSVSZOMBIES_ZOMBIECONTROLLER_H
#include "Zombies/ZombieBase.h"
#include <vector>

class ZombieController : public cocos2d::Node{
public:
    CREATE_FUNC(ZombieController);
    virtual bool init() override;

    void spawnZombies(int countOfZombies, float sceneWidth, float tileHeight, int numOfRows);
    void removeZombie(ZombieBase* zombie);

    const std::vector<ZombieBase*>& getZombies() const { return zombies_; }

    void setLosePositionX(float losePosX);
    void setOnAnyZombieReachedHouse(std::function<void()> callback);

private:
    std::vector<ZombieBase*> zombies_;
    int countOfDeadZombies_;        // this will be the score

    float losePosX_ = -1000.f;
    std::function<void()> onAnyZombieReachedHouse_;
};
#endif //PLANTSVSZOMBIES_ZOMBIECONTROLLER_H