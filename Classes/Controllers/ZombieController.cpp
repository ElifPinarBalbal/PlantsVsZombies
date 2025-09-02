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
    constexpr float SPACING_BETWEEN_ZOMBIES_AT_THE_SAME_ROW = 150.f;
}

bool ZombieController::init() {
    return true;
}

void ZombieController::spawnZombies(int countOfZombies, float sceneWidth, float tileHeight, int numOfRows) {
    std::unordered_map<int, int> zombiesPerRow;   // store how many zombies are there in a row-basis

    for (int i = 0; i < countOfZombies; ++i) {
        auto * zombie = ZombieWeak::create();
        int row = rand() % numOfRows;           // randomly select the row to come

        // When there is more than 1 zombie in the same row, they init at the same position - so I added spacing between them
        int zombieCountInARow = zombiesPerRow[row] ++;
        float xPos = sceneWidth + spawnConstants::X_POS_OFFSET + (zombieCountInARow * spawnConstants::SPACING_BETWEEN_ZOMBIES_AT_THE_SAME_ROW);
        float yPos = tileHeight * (4-row) + tileHeight - spawnConstants::Y_POS_OFFSET;         // I made (4-row) to count the rows from the top

        zombie->setPosition({xPos, yPos});
        zombie->setRow(row);
        cocos2d::log("Row number: %d", zombie->getRow());
        this->addChild(zombie);
        zombies_.push_back(zombie);             // put the pointer at the end of the vector
    }
    cocos2d::log("-----\n");
}

void ZombieController::removeZombie(ZombieBase* zombie) {
    this->removeChild(zombie);
    zombies_.erase(std::remove(zombies_.begin(), zombies_.end(), zombie), zombies_.end());   // delete from the vector itself
}
