//
// Created by Elif Balbal on 19.08.2025.
//

#include "PlantBase.h"

bool PlantBase::init()
{
    if (!Sprite::initWithFile(spriteFile())) return false;
    setupDefaults();
    return true;
}
