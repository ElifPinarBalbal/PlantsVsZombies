/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __SECOND_SCENE_H__
#define __SECOND_SCENE_H__

#include  "cocos/2d/CCNode.h"
#include  "cocos/2d/CCSprite.h"
#include  "cocos/2d/CCScene.h"
#include "Zombies/ZombieBase.h"
#include <array>
#include  "Plants/PlantBase.h"
#include "Plants/PeaShooter.h"
#include "Controllers/ZombieController.h"

namespace gridSize
{
    static constexpr int GRID_COLS = 9;
    static constexpr int GRID_ROWS = 5;
    static constexpr int GRID_SIZE = GRID_COLS * GRID_ROWS;
}

namespace peaRetarget
{
    constexpr float PEA_RETARGET_LOOP_PERIOD = 0.001f;
}
class ZombieBase;
class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    void CreateUI(const cocos2d::Vec2& origin, const cocos2d::Size& visibleSize);

    void setupPeaRetargetLoop(float retargetPeriod = peaRetarget::PEA_RETARGET_LOOP_PERIOD);
    void reTargetAllPeaShooters();

    CREATE_FUNC(GameScene);

private:
    ZombieController* zombieController_ = nullptr;
    int mHoverIdx = -1;
    cocos2d::DrawNode* mHoverRect = nullptr;

    using TileArray = std::array<cocos2d::Sprite*, gridSize::GRID_SIZE>;
    TileArray mTiles{};
    cocos2d::Size mCellSize;
    cocos2d::Vec2 mGridOrigin;
    std::array<PlantBase*, gridSize::GRID_SIZE> mPlants{};   // nullptr = empty


    // convert row col index
    static constexpr  int index(int row, int col)
    {
        return row * gridSize::GRID_COLS + col;
    }

    void BuildGroundGrid(const cocos2d::Size& visibleSize, const cocos2d::Vec2& origin);
    cocos2d::Vec2 CellCenter(int col, int row) const;
    cocos2d::Sprite* checkTileAt(int row, int col) const;

    PlantBase *getNearestPlantAheadInRow(int row, float xMin) const;
    void updateZombieEating();

    void setupGridHover();
    int whichRowFromY (float positionY) const;
    bool isInTheSameRow(cocos2d::Node *nodeA, cocos2d::Node *nodeB) const;
    bool isInTheSameRowAndFront(cocos2d::Node *nodeA, cocos2d::Node *nodeB) const;
    ZombieBase* getNearestZombieInFront(PeaShooter* peashooter);

    bool cellOccupied(int row, int col) const {
        return mPlants[index(row, col)] != nullptr;
    }
    void markCell(int row, int col, PlantBase* p) {
        mPlants[index(row, col)] = p;
    }
    void unMarkCellForDeadPlant(PlantBase* plant);
};

#endif // __SECOND_SCENE_H__
