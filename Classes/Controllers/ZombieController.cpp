//
// Created by Elif Balbal on 25.08.2025.
//
#include "ZombieController.h"

#include "Plants/PlantBase.h"
#include "Zombies/ZombieBase.h"
#include "Zombies/ZombieWeak.h"

namespace spawnConstants
{
    constexpr float X_POS_OFFSET = 50.f;
    constexpr float Y_POS_OFFSET = 20.f;
}

bool ZombieController::init() {
    return true;
}

void ZombieController::spawnZombies(int countOfZombies, float sceneWidth, float tileHeight, int numOfRows) {
    for (int i = 0; i < countOfZombies; ++i) {
        auto * zombie = ZombieWeak::create();
        int row = rand() % numOfRows;           // randomly select the row to come

        float xPos = sceneWidth + spawnConstants::X_POS_OFFSET;
        float yPos = tileHeight * row + tileHeight - spawnConstants::Y_POS_OFFSET;

        zombie->setPosition({xPos, yPos});
        this->addChild(zombie);
        zombies_.push_back(zombie);             // put the pointer at the end of the vector
    }
}

void ZombieController::removeZombie(ZombieBase* zombie) {
    this->removeChild(zombie);
    zombies_.erase(std::remove(zombies_.begin(), zombies_.end(), zombie), zombies_.end());   // delete from the vector itself
}
