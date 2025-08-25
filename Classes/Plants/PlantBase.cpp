//
// Created by Elif Balbal on 19.08.2025.
//

#include "PlantBase.h"

#include "Zombies/ZombieBase.h"
#include "cocos/base/CCDirector.h"

bool PlantBase::init()
{
    if (!Sprite::initWithFile(getImagePath())) return false;
    setupDefaults();
    return true;
}


