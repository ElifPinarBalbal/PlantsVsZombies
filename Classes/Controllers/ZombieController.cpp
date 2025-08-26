//
// Created by Elif Balbal on 25.08.2025.
//
#include "ZombieController.h"

#include "Plants/PlantBase.h"
#include "Zombies/ZombieBase.h"
#include "Zombies/ZombieWeak.h"

bool ZombieController::init() {
    return true;
}

void ZombieController::spawnZombies(int countOfZombies, float sceneWidth, float tileHeight, int numOfRows) {
    for (int i = 0; i < countOfZombies; ++i) {
        auto * zombie = ZombieWeak::create();
        int row = rand() % numOfRows;           // randomly select the row to come

        float xPos = sceneWidth + 50.f;
        float yPos = tileHeight * row + tileHeight - 20.f;

        zombie->setPosition({xPos, yPos});
        this->addChild(zombie);
        zombies_.push_back(zombie);             // put the pointer at the end of the vector
    }
}

void ZombieController::removeZombie(ZombieBase* zombie) {
    this->removeChild(zombie);
    zombies_.erase(std::remove(zombies_.begin(), zombies_.end(), zombie), zombies_.end());   // delete from the vector itself
}
