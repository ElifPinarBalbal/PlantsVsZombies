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
#include "SplashScene.h"
#include "cocos/2d/CCMenu.h"
#include "cocos/2d/CCMenuItem.h"
#include "cocos/2d/CCLabel.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"
#include  "GameScene.h"

#include "AppDelegate.h"
#include "2d/CCDrawNode.h"

#include <cocos/base/CCEventListenerTouch.h>

#include "CCEventDispatcher.h"
#include "CCEventListenerMouse.h"
#include "Zombies/ZombieWeak.h"
#include "Plants/PeaShooter.h"
#include "Plants/PotatoMine.h"
#include "Controllers/ZombieController.h"
#include "Controllers/PlantController.h"
#include "Controllers/SunController.h"

#include "2d/CCActionInterval.h"
#include "Plants/BluePeaShooter.h"
#include "ui/UIButton.h"
#include "UIComponents/PlantsMenu.h"

USING_NS_CC;

using namespace cocos2d::ui;
using namespace std;
using cocos2d::Sprite;
using cocos2d::Vec2;
using cocos2d::Size;

namespace button
{
    constexpr float BACK_BUTTON_MARGIN_X       = 20.f;
    constexpr float BACK_BUTTON_MARGIN_Y       = 20.f;
    constexpr float BACK_BUTTON_RATIO_POS_X    = 0.3f;
    constexpr float BACK_BUTTON_RATIO_POS_Y    = 0.3f;
}

namespace grid
{
    constexpr float MARGIN_L                    = 185.f;
    constexpr float MARGIN_R                    = 22.f;
    constexpr float MARGIN_B                    = 20.f;
    constexpr float MARGIN_T                    = 50.f;

    inline float fieldWidth (const cocos2d::Size& vs)  { return vs.width  - (MARGIN_L + MARGIN_R); }
    inline float fieldHeight (const cocos2d::Size& vs)  { return vs.height - (MARGIN_T + MARGIN_B); }
}

namespace zOrders
{
    constexpr int TILE_Z_ORDER = 0;
    constexpr int BACK_BUTTON_Z_ORDER = 10;
    constexpr int PEASHOOTER_Z_ORDER = 5;
    //constexpr int PEA_Z_ORDER = 8;
    constexpr int ZOMBIE_Z_ORDER = 5;
    constexpr int BACKGROUND_Z_ORDER = 0;
    constexpr int MENU_Z_ORDER = 1;
    constexpr int HOVERED_TILE_Z_ORDER = 50;
    constexpr int M_SCORE_HUD_Z_ORDER = 1000;
    constexpr int M_SCORE_BOX_GEOMETRY_Z_ORDER = 0;
    constexpr int M_SCORE_LABEL_Z_ORDER = 1;
    constexpr int M_SUN_HUD_Z_ORDER = 1000;
    constexpr int M_SUN_BOX_GEOMETRY_Z_ORDER = 0;
    constexpr int M_SUN_LABEL_Z_ORDER = 1;
    constexpr int SUN_CONTROLLER_Z_ORDER = 20;
    constexpr int PLANTS_MENU_Z_ORDER = 1000;
    constexpr int BACK_COLOR_LAYER_Z_ORDER = 5000;
    constexpr int M_DRAG_GHOST_Z_ORDER = 3000;
}

namespace
{
    constexpr int TAG_SCORE_HOOKED = 0x51A;
}

namespace HUD_paddings
{
    constexpr int PADDING_X = 16.f;
    constexpr int PADDING_Y = 10.f;
}
Scene* GameScene::createScene()
{
    return GameScene::create();
}

// Print useful error message instead of segfault when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
}

// Back Button create function
cocos2d::ui::Button* CreateBackButton(const cocos2d::Vec2& origin, const cocos2d::Size& visibleSize)
{
    auto* const backButton = cocos2d::ui::Button::create("Red_button.png", "Red_button.png");
    if (!backButton)
    {
        problemLoading("Red_button.png");
        return nullptr;
    }

    const cocos2d::Size& backButtonContentSize = backButton->getContentSize();

    backButton->setScale(0.6f);
    backButton->setTitleText("");
    backButton->setTitleFontSize(48);
    backButton->setTitleAlignment(cocos2d::TextHAlignment::CENTER,
                                  cocos2d::TextVAlignment::CENTER);

    backButton->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_RIGHT);

    const float marginX = button::BACK_BUTTON_MARGIN_X;;
    const float marginY = button::BACK_BUTTON_MARGIN_Y;;
    backButton->setPosition({
        origin.x + visibleSize.width  - marginX,
        origin.y +                      marginY
    });
    return backButton;
}

// Grid Layout
void GameScene::BuildGroundGrid(const Size& visibleSize, const Vec2& origin)
{
    const float fieldW = grid::fieldWidth(visibleSize);
    const float fieldH = grid::fieldHeight(visibleSize);

    mCellSize   = Size(fieldW / gridSize::GRID_COLS, fieldH / gridSize::GRID_ROWS);
    mGridOrigin = Vec2(origin.x + grid::MARGIN_L, origin.y + grid::MARGIN_B);

    for (int row = 0; row < gridSize::GRID_ROWS; ++row)
    {
        for (int col = 0; col < gridSize::GRID_COLS; ++col)
        {
            const bool light = ((row + col) % 2 == 0);
            const char* file = light ? "ground_light.png" : "ground_dark.png";

            auto* tile = Sprite::create(file);
            if (!tile) { problemLoading(file); continue; }

            // scale to cell size
            const Size tex = tile->getContentSize();
            tile->setScaleX(mCellSize.width  / tex.width);
            tile->setScaleY(mCellSize.height / tex.height);

            // center of this cell
            const float colX = mGridOrigin.x + mCellSize.width  * (col + 0.5f);
            const float colY = mGridOrigin.y + mCellSize.height * (row + 0.5f);
            tile->setPosition(Vec2(colX, colY));

            this->addChild(tile, zOrders::TILE_Z_ORDER);            // behind everything
            mTiles[index(row, col)] = tile;
        }
    }
}

// this function converts the grid coordinate into actual pixel positions
Vec2 GameScene::CellCenter(int col, int row) const
{
    col = max(0, min(gridSize::GRID_COLS - 1, col));
    row = max(0, min(gridSize::GRID_ROWS - 1, row));
    return Vec2(
        mGridOrigin.x + mCellSize.width  * (col + 0.5f),
        mGridOrigin.y + mCellSize.height * (row + 0.5f)
    );
}

cocos2d::Sprite* GameScene::checkTileAt(int row, int col) const
{
    if (row < 0 || row >= gridSize::GRID_ROWS || col < 0 || col >= gridSize::GRID_COLS) return nullptr;
    return mTiles.at(index(row, col));
}

PlantBase* GameScene::getNearestPlantAheadInRow(int row, float xMin) const
{
    if (auto* plantController = this->getChildByName<PlantController*>("PlantController")) {
        return plantController->getNearestPlantInRow(row, xMin);
    }
    return nullptr;
}

void GameScene::updateZombieEating()
{
    const float STAND_OFF     = 50.f;                // where zombie will stop according to the plant
    const float DETECT_REACH  = STAND_OFF + 5.f;      // start eating a bit early - so do not overlap

    for (auto* zombie : zombieController_->getZombies()) {
        if (!zombie || !zombie->getParent() || zombie->isDead()) continue;

        const int row = whichRowFromY(zombie->getPositionY());
        auto* plant = getNearestPlantAheadInRow(row, zombie->getPositionX());

        if (!plant) {
            if (zombie->isEating()) zombie->zombieStopEating();
            continue;
        }

        if (auto* potatoMine = dynamic_cast<PotatoMine*>(plant)) {
            const float dx = zombie->getPositionX() - potatoMine->getPositionX();
            if (dx <= 12.f) {
                potatoMine->explode(zombie);
                continue;                    // both will die
            }
            if (zombie->isEating()) zombie->zombieStopEating();
            continue;
        }

        const float distanceX = zombie->getPositionX() - plant->getPositionX();
        if (!zombie->isEating()) {
            if (distanceX >= STAND_OFF && distanceX <= DETECT_REACH) {
                zombie->zombieStartEating(plant);
            }
        } else {
        }
    }
}

/*
bool OnUserTouch(cocos2d::Touch* touch, cocos2d::Event* event)
{
    return false;
}

void OnUserTouchMove(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!IsPointInsideBoard())
        remove

    HighlightCell(x)
}*/


void GameScene::setupGridHover()
{
    mHoverRect = cocos2d::DrawNode::create();
    this->addChild(mHoverRect, zOrders::HOVERED_TILE_Z_ORDER);
/*
    auto* touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = std::bind(&OnUserTouch, std::placeholders::_1, std::placeholders::_2);
    touchListener->onTouchMoved = std::bind(&OnUserTouchMove, std::placeholders::_1, std::placeholders::_2);
*/

    auto* listener = cocos2d::EventListenerMouse::create();
    listener->onMouseMove = [this](cocos2d::EventMouse* event)
    {
        if (isGameEnded_) return;
        cocos2d::Vec2 pos = event->getLocationInView();
        auto vs = cocos2d::Director::getInstance()->getVisibleSize();
        auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

        // Convert mouse pos into grid col/row
        float relX = pos.x - mGridOrigin.x;
        float relY = pos.y - mGridOrigin.y;

        int col = (int)(relX / mCellSize.width);
        int row = (int)(relY / mCellSize.height);

        if (col < 0 || col >= gridSize::GRID_COLS || row < 0 || row >= gridSize::GRID_ROWS)
        {
            mHoverRect->clear();
            return;
        }

        int idx = index(row, col);
        if (idx != mHoverIdx)
        {
            mHoverIdx = idx;
            mHoverRect->clear();

            cocos2d::Vec2 center = CellCenter(col, row);
            float w = mCellSize.width;
            float h = mCellSize.height;

            cocos2d::Vec2 rect[4] = {
                {center.x - w/2, center.y - h/2},
                {center.x + w/2, center.y - h/2},
                {center.x + w/2, center.y + h/2},
                {center.x - w/2, center.y + h/2}
            };

            // white-ish semi-transparent highlight
            cocos2d::Color4F hoverColor(1.f, 1.f, 1.f, 0.35f);
            mHoverRect->drawSolidPoly(rect, 4, hoverColor);
        }
    };

    listener->onMouseDown = [this](cocos2d::EventMouse* event) {
        if (isGameEnded_) return;
        if (mDragging) return;
        if (event->getMouseButton() != cocos2d::EventMouse::MouseButton::BUTTON_LEFT)
            return;

        Vec2 mousePosition = event->getLocation(); // I need to see the mouse position

        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        //auto visibleOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
        const float gridLeft = 200.0f;
        const float gridRight = gridLeft + gridSize::GRID_COLS * grid::fieldWidth(visibleSize);
        const float gridBottom = 0.0f;
        const float gridTop = gridBottom + gridSize::GRID_ROWS * grid::fieldHeight(visibleSize);

        if (mousePosition.x < gridLeft || mousePosition.x > gridRight ||
            mousePosition.y < gridBottom || mousePosition.y > gridTop) {
            log("Clicked outside grid bounds");
            return;
            }

        if (mHoverIdx < 0) return;
        int row = mHoverIdx / gridSize::GRID_COLS;
        int col = mHoverIdx % gridSize::GRID_COLS;

        // Check mechanism for outside of the grid
        if (row < 0 || row >= gridSize::GRID_ROWS || col < 0 || col >= gridSize::GRID_COLS) {
            log("Clicked outside grid.");
            return;
        }

        // To not place 2 peaShooters on the same tile
        if (cellOccupied(row, col))
        {
            log("Burası dolu !");
            return;
        }

        if (!plantsMenu_ || !plantsMenu_->hasPlantSelection()) {
            CCLOG("No plant selected from menu");
            return;
        }

        const int selectedPlantCost = plantsMenu_->selectedPlantCost();
        const PlantType plantToPlace = plantsMenu_->selectedPlantType();

        if (!spendSun(selectedPlantCost)) {
            if (mSunHUD) {
                mSunHUD->runAction(Sequence::create(
                    cocos2d::TintTo::create(0.05f, 255,120,120),
                    cocos2d::TintTo::create(0.15f, 255,255,255),
                    nullptr));
            }
            return;
        }

        switch (plantToPlace) {
            case PlantType::PeaShooter: {
                auto* peaShooter = PeaShooter::create();
                if (!peaShooter) return;

                peaShooter->setPosition(CellCenter(col, row));
                peaShooter->setName("PeaShooter");
                this->addChild(peaShooter, zOrders::PEASHOOTER_Z_ORDER);

                if (auto* plantController = this->getChildByName<PlantController*>("PlantController"))
                    plantController->addPlant(peaShooter, row);

                markCell(row, col, peaShooter);

                peaShooter->setOnDeath([this](PlantBase* p){
                    this->unMarkCellForDeadPlant(p);
                });

                // start shooting immediately if a zombie is already in front
                for (auto* zombie : zombieController_->getZombies()) {
                    if (isInTheSameRowAndFront(peaShooter, zombie) &&
                        whichRowFromY(zombie->getPositionY()) != -1) {
                        peaShooter->startAutoFire(zombie, this, zOrders::PEA_Z_ORDER);
                        break;
                        }
                }
            } break;

            case PlantType::BluePeaShooter: {
                auto* bluePeaShooter = BluePeaShooter::create();
                if (!bluePeaShooter) return;

                bluePeaShooter->setPosition(CellCenter(col, row));
                bluePeaShooter->setName("BluePeaShooter");
                this->addChild(bluePeaShooter, zOrders::PEASHOOTER_Z_ORDER);

                if (auto* plantController = this->getChildByName<PlantController*>("PlantController"))
                    plantController->addPlant(bluePeaShooter, row);

                markCell(row, col, bluePeaShooter);
                bluePeaShooter->setOnDeath([this](PlantBase* p){ this->unMarkCellForDeadPlant(p); });

                for (auto* z : zombieController_->getZombies()) {
                    if (isInTheSameRowAndFront(bluePeaShooter, z) && whichRowFromY(z->getPositionY()) != -1) {
                        bluePeaShooter->startAutoFire(z, this, zOrders::PEA_Z_ORDER);
                        break;
                    }
                }
            } break;

            case PlantType::PotatoMine: {
                auto* potatoMine = PotatoMine::create();
                if (!potatoMine) return;

                potatoMine->setPosition(CellCenter(col, row));
                potatoMine->setName("PotatoMine");
                this->addChild(potatoMine, zOrders::PEASHOOTER_Z_ORDER);

                if (auto* pc = this->getChildByName<PlantController*>("PlantController"))
                    pc->addPlant(potatoMine, row);

                markCell(row, col, potatoMine);

                potatoMine->setOnDeath([this](PlantBase* p){
                    this->unMarkCellForDeadPlant(p);
                });
            } break;
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


// row number finder
int GameScene::whichRowFromY (float positionY) const
{
    int row = static_cast<int>((positionY - mGridOrigin.y) / mCellSize.height);
    if (row < 0 || row >= gridSize::GRID_ROWS) return -1;
    return row;
}

bool GameScene::isInTheSameRow(cocos2d::Node* nodeA, cocos2d::Node* nodeB) const
{
    return (whichRowFromY(nodeA->getPositionY()) == whichRowFromY(nodeB->getPositionY()));
}

bool GameScene::isInTheSameRowAndFront(cocos2d::Node *plant, cocos2d::Node *zombie) const
{
    if (isInTheSameRow(plant, zombie))
        {
        if (plant->getPositionX() <= zombie->getPositionX()) {
            return true;
        }
        return false;
    }
    return false;
}

void GameScene::unMarkCellForDeadPlant(PlantBase* plant)
{
    if (!plant) return;
    for (int row = 0; row < gridSize::GRID_ROWS; ++row) {
        for (int col = 0; col < gridSize::GRID_COLS; ++col) {
            const int tile = index(row, col);
            if (mPlants[tile] == plant) {
                mPlants[tile] = nullptr;
                return;
            }
        }
    }
}

ZombieBase* GameScene::getNearestZombieInFront(PeaShooter* peashooter)
{
    ZombieBase* nearestZombie = nullptr;
    for (auto* zombie : zombieController_->getZombies()) {
        if (!zombie || zombie->isDead()) continue;
        if (!isInTheSameRowAndFront(peashooter, zombie)) continue;

        if (!nearestZombie ||
            zombie->getPositionX() < nearestZombie->getPositionX()) {
            nearestZombie = zombie;
            }
    }
    return nearestZombie;
}

void GameScene::CreateUI(const cocos2d::Vec2& origin, const cocos2d::Size& visibleSize)
{
    if (auto* const backButton = CreateBackButton(origin, visibleSize))
    {
        backButton->addTouchEventListener([&](cocos2d::Ref*, ui::Widget::TouchEventType touchEventType) {
            if (touchEventType == ui::Widget::TouchEventType::ENDED) {
                cocos2d::Director::getInstance()->popScene();
            }
        });
        this->addChild(backButton, zOrders::BACK_BUTTON_Z_ORDER);
    }

    zombieController_ = ZombieController::create();
    this->addChild(zombieController_, zOrders::ZOMBIE_Z_ORDER);

    // if zombie passes this X position, game ends and player loses
    const float losePosX = mGridOrigin.x - 20.f;
    zombieController_->setLosePositionX(losePosX);

    zombieController_->setOnAnyZombieReachedHouse([this]{
        if (!isGameEnded_) onLose();
    });

    int numOfZombies = 5;
    zombieController_->spawnZombies(
        numOfZombies,
        Director::getInstance()->getVisibleSize().width,
        mCellSize.height,
        gridSize::GRID_ROWS
    );
}

void GameScene::reTargetAllPeaShooters()
{
    auto* plantController = this->getChildByName<PlantController*>("PlantController");
    if (!plantController) return;

    for (int row = 0; row < gridSize::GRID_ROWS; ++row) {
        const auto& plants = plantController->getPlantsInRow(row);
        for (auto* plant : plants) {
            auto* peaShooter = dynamic_cast<PeaShooter*>(plant);
            if (!peaShooter || !peaShooter->getParent()) continue;

            auto* target = peaShooter->currentTarget_;
            const bool needRetarget =
                (!target) || target->isDead() || !target->getParent() ||
                (target->getPositionX() < peaShooter->getPositionX());

            if (needRetarget) {
                if (auto* next = getNearestZombieInFront(peaShooter)) {
                    peaShooter->startAutoFire(next, this, zOrders::PEA_Z_ORDER);
                }
            }
        }
    }
}

void GameScene::setupPeaRetargetLoop(float retargetPeriod)
{
    this->schedule([this](float){
        this->reTargetAllPeaShooters();
    }, retargetPeriod, "retarget_tick");
}

void GameScene::updateScoreBoxGeometry()
{
    const float paddingX = HUD_paddings::PADDING_X;
    const float paddingY = HUD_paddings::PADDING_Y;

    const Size labelSize = mScoreLabel->getContentSize();
    const Size box(labelSize.width + paddingX * 2.f, labelSize.height + paddingY * 2.f);

    mScoreLabel->setPosition(Vec2(paddingX, box.height * 0.5f));
    mScoreBoxGeometry->clear();  // redraw in each change

    const Vec2 bl(0.f, 0.f);
    const Vec2 tr(box.width, box.height);

    //semi-transparent - thin border
    mScoreBoxGeometry->drawSolidRect(bl, tr, Color4F(0.f, 0.f, 0.f, 0.45f));
    mScoreBoxGeometry->drawRect(bl, tr, Color4F(1.f, 1.f, 1.f, 0.9f));

    mScoreHUD->setContentSize(box);
}

void GameScene::updateSunBoxGeometry()
{
    const float paddingX = HUD_paddings::PADDING_X;
    const float paddingY = HUD_paddings::PADDING_Y;

    const Size labelSize = mSunLabel->getContentSize();
    const Size box(labelSize.width + paddingX*2.f, labelSize.height + paddingY*2.f);

    mSunLabel->setPosition({paddingX, box.height * 0.5f});
    mSunBoxGeometry->clear();
    mSunBoxGeometry->drawSolidRect({0,0}, {box.width, box.height}, Color4F(0.f,0.f,0.f,0.45f));
    mSunBoxGeometry->drawRect({0,0}, {box.width, box.height}, Color4F(1.f,1.f,1.f,0.9f));
    mSunHUD->setContentSize(box);
}

void GameScene::initScoreUI() {
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto visibleOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();

    mScoreHUD = Node::create();
    mScoreHUD->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    mScoreHUD->setPosition(visibleOrigin + Vec2(18.f, visibleSize.height - 14.f));
    this->addChild(mScoreHUD, zOrders::M_SCORE_HUD_Z_ORDER);

    mScoreBoxGeometry = DrawNode::create();
    mScoreHUD->addChild(mScoreBoxGeometry, zOrders::M_SCORE_BOX_GEOMETRY_Z_ORDER);

    mScoreLabel = Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", 28);
    mScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    mScoreHUD->addChild(mScoreLabel, zOrders::M_SCORE_LABEL_Z_ORDER);
    updateScoreBoxGeometry();
}

void GameScene::initSunUI()
{
    using namespace cocos2d;
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

    mSunHUD = Node::create();
    mSunHUD->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    mSunHUD->setPosition(visibleOrigin + Vec2(18.f, visibleSize.height - 64.f));
    addChild(mSunHUD, zOrders::M_SUN_HUD_Z_ORDER);

    mSunBoxGeometry = DrawNode::create();
    mSunHUD->addChild(mSunBoxGeometry, zOrders::M_SUN_BOX_GEOMETRY_Z_ORDER);

    mSunLabel = Label::createWithTTF("Sun: 40", "fonts/Marker Felt.ttf", 26);
    mSunLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    mSunHUD->addChild(mSunLabel, zOrders::M_SUN_LABEL_Z_ORDER);

    updateSunBoxGeometry();
    updateSunBoxGeometry();
}

void GameScene::addSun(int sunAmount)
{
    mSun += sunAmount;
    if (mSunLabel) { mSunLabel->setString("Sun: " + std::to_string(mSun)); updateSunBoxGeometry(); }
    if (plantsMenu_) plantsMenu_->setSunAmount(mSun);
}

bool GameScene::spendSun(int sunAmount)
{
    if (mSun < sunAmount) return false;
    mSun -= sunAmount;
    if (mSunLabel) { mSunLabel->setString("Sun: " + std::to_string(mSun)); updateSunBoxGeometry(); }
    if (plantsMenu_) plantsMenu_->setSunAmount(mSun);
    return true;
}

void GameScene::hookScoreToNewZombies() {
    for (auto* zombie : zombieController_->getZombies()) {
        if (!zombie || !zombie->getParent() || zombie->isDead()) continue;
        if (zombie->getTag() == TAG_SCORE_HOOKED) continue;
        zombie->setTag(TAG_SCORE_HOOKED);

        zombie->addOnDeath([this](ZombieBase*) {
            ++mScore;
            ++numberOfKills_;
            if (mScoreLabel) {
                mScoreLabel->setString("Score: " + std::to_string(mScore));
                updateScoreBoxGeometry();
                if (!isGameEnded_ && numberOfKills_ >= numberOfTargets_) onWin();
            }
        });
    }
}

void GameScene::onWin()
{
    showLevelEndPopup(true);
}

void GameScene::onLose()
{
    showLevelEndPopup(false);
}

void GameScene::showLevelEndPopup(bool didWin)
{
    isGameEnded_ = true;

    auto* backColorLayer = cocos2d::LayerColor::create(cocos2d::Color4B(0,0,0,160));
    backColorLayer->setContentSize(Director::getInstance()->getVisibleSize());
    this->addChild(backColorLayer, zOrders::BACK_COLOR_LAYER_Z_ORDER);

    auto* panel = cocos2d::Node::create();
    panel->setContentSize({420.f, 220.f});
    panel->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    panel->setPosition(Director::getInstance()->getVisibleSize()/2);
    backColorLayer->addChild(panel);

    auto* popUpBox = cocos2d::DrawNode::create();
    panel->addChild(popUpBox);
    auto s = panel->getContentSize();
    popUpBox->drawSolidRect({0,0}, {s.width, s.height}, Color4F(cocos2d::Color4B(222, 184, 135, 255)));
    popUpBox->drawRect({0,0}, {s.width, s.height}, cocos2d::Color4F(1,1,1,0.9f));

    auto* popUpTitle = cocos2d::Label::createWithTTF(
        didWin ? "Congratulations!" : "Game Over",
        "fonts/Marker Felt.ttf", 40);
    popUpTitle->setPosition({s.width*0.5f, s.height*0.7f});
    panel->addChild(popUpTitle);

    auto* replayLabel = cocos2d::Label::createWithTTF("Replay", "fonts/Marker Felt.ttf", 32);
    auto* replayItem  = cocos2d::MenuItemLabel::create(replayLabel, [this](Ref*){
        auto* popUpScene = GameScene::createScene();
        Director::getInstance()->replaceScene(popUpScene);
    });
    auto* menu = cocos2d::Menu::create(replayItem, nullptr);
    menu->setPosition({s.width*0.5f, s.height*0.3f});
    panel->addChild(menu);
}


bool GameScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("mapp.png");

    if (background) {
        background->setPosition(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height / 2);

        // scale it to fit the screen size if needed
        float scaleX = visibleSize.width  / background->getContentSize().width;
        float scaleY = visibleSize.height / background->getContentSize().height;
        background->setScaleX(scaleX);
        background->setScaleY(scaleY);

        this->addChild(background, zOrders::BACKGROUND_Z_ORDER);
    }
    BuildGroundGrid(visibleSize, origin);
    setupGridHover();

    initScoreUI();
    schedule([this](float){ hookScoreToNewZombies(); }, 0.25f, "score_hook_tick");

    plantsMenu_ = PlantsMenu::create();
    addChild(plantsMenu_, zOrders::PLANTS_MENU_Z_ORDER);
    const auto visSize = Director::getInstance()->getVisibleSize();
    const auto visOrigin = Director::getInstance()->getVisibleOrigin();
    plantsMenu_->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    plantsMenu_->setPosition(visOrigin + Vec2(18.f, visSize.height - 70.f));

    auto* peaCard = plantsMenu_->addSlot(PlantType::PeaShooter, 20, "peaShooter.png", 0.20f);
    peaCard->setPlantCardSelected(true);

    plantsMenu_->addSlot(PlantType::BluePeaShooter, 40, "peaShooterBlue.png", 0.30f);

    plantsMenu_->addSlot(PlantType::PotatoMine, 30, "potatoMine.png", 0.75f);
    plantsMenu_->setSunAmount(mSun);

    plantsMenu_->onCardDragBegan = [this](PlantType type, PlantCard* card)
    {
        if (isGameEnded_) return;
        if (!card || !card->getPlantCardEnabled()) return;
        mDragging = true;
        mDragType = type;
        mDragCost = card->getPlantCardCost();

        // make a ghost sprite that follows the cursor
        mDragGhost = cocos2d::Sprite::create(card->getPlantCardImagePath());
        mDragGhost->setScale(card->getPlantCardImageScale());
        mDragGhost->setOpacity(180);
        this->addChild(mDragGhost, zOrders::M_DRAG_GHOST_Z_ORDER);
    };

    auto* dragMove = cocos2d::EventListenerMouse::create();
    dragMove->onMouseMove = [this](cocos2d::EventMouse* e){
        if (isGameEnded_) return;
        if (!mDragging || !mDragGhost) return;
        const cocos2d::Vec2 pos = e->getLocation();
        mDragGhost->setPosition(pos);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(dragMove, this);

    // After adding dragMove listener in GameScene::init()
auto* dragUp = cocos2d::EventListenerMouse::create();
dragUp->onMouseUp = [this](cocos2d::EventMouse* e){
    if (isGameEnded_) return;
    if (!mDragging) return;
    mDragging = false;

    auto cleanupGhost = [this](){
        if (mDragGhost) { mDragGhost->removeFromParent(); mDragGhost = nullptr; }
    };
    const cocos2d::Vec2 pos = e->getLocation();

    // compute grid col/row (same logic as onMouseMove in setupGridHover)
    int col = static_cast<int>((pos.x - mGridOrigin.x) / mCellSize.width);
    int row = static_cast<int>((pos.y - mGridOrigin.y) / mCellSize.height);

    // validate drop
    if (col < 0 || col >= gridSize::GRID_COLS ||
        row < 0 || row >= gridSize::GRID_ROWS ||
        cellOccupied(row, col)) {
        cleanupGhost();
        return;
    }

    if (!spendSun(mDragCost)) {
        if (mSunHUD) {
            mSunHUD->runAction(cocos2d::Sequence::create(
                cocos2d::TintTo::create(0.05f, 255,120,120),
                cocos2d::TintTo::create(0.15f, 255,255,255),
                nullptr));
        }
        cleanupGhost();
        return;
    }

    switch (mDragType) {
        case PlantType::PeaShooter: {
            auto* pea = PeaShooter::create();
            if (!pea) { cleanupGhost(); return; }
            pea->setPosition(CellCenter(col, row));
            pea->setName("PeaShooter");
            this->addChild(pea, zOrders::PEASHOOTER_Z_ORDER);

            if (auto* pc = this->getChildByName<PlantController*>("PlantController"))
                pc->addPlant(pea, row);

            markCell(row, col, pea);
            pea->setOnDeath([this](PlantBase* p){ this->unMarkCellForDeadPlant(p); });

            // optional: shoot immediately if a zombie is already in front
            for (auto* z : zombieController_->getZombies()) {
                if (isInTheSameRowAndFront(pea, z) &&
                    whichRowFromY(z->getPositionY()) != -1) {
                    pea->startAutoFire(z, this, zOrders::PEA_Z_ORDER);
                    break;
                }
            }
        } break;

        case PlantType::BluePeaShooter: {
            auto* blue = BluePeaShooter::create();
            if (!blue) { cleanupGhost(); return; }
            blue->setPosition(CellCenter(col, row));
            blue->setName("BluePeaShooter");
            this->addChild(blue, zOrders::PEASHOOTER_Z_ORDER);

            if (auto* pc = this->getChildByName<PlantController*>("PlantController"))
                pc->addPlant(blue, row);

            markCell(row, col, blue);
            blue->setOnDeath([this](PlantBase* p){ this->unMarkCellForDeadPlant(p); });

            for (auto* z : zombieController_->getZombies()) {
                if (isInTheSameRowAndFront(blue, z) &&
                    whichRowFromY(z->getPositionY()) != -1) {
                    blue->startAutoFire(z, this, zOrders::PEA_Z_ORDER);
                    break;
                    }
            }
        } break;


        case PlantType::PotatoMine: {
            auto* mine = PotatoMine::create();
            if (!mine) { cleanupGhost(); return; }
            mine->setPosition(CellCenter(col, row));
            mine->setName("PotatoMine");
            this->addChild(mine, zOrders::PEASHOOTER_Z_ORDER);

            if (auto* pc = this->getChildByName<PlantController*>("PlantController"))
                pc->addPlant(mine, row);

            markCell(row, col, mine);
            mine->setOnDeath([this](PlantBase* p){ this->unMarkCellForDeadPlant(p); });
        } break;
    }

    cleanupGhost();
};
_eventDispatcher->addEventListenerWithSceneGraphPriority(dragUp, this);




    initSunUI();

    sunController_ = SunController::create();
    addChild(sunController_,zOrders::SUN_CONTROLLER_Z_ORDER);

    const cocos2d::Rect fallArea(
    mGridOrigin.x,
    mGridOrigin.y,
    mCellSize.width  * gridSize::GRID_COLS,
    mCellSize.height * gridSize::GRID_ROWS);
    sunController_->setFallArea(fallArea);

    this->schedule([this](float){
        // Iterate current children of sun controller and attach listener once
        for (auto* node : sunController_->getChildren()) {
            auto* sun = dynamic_cast<Sun*>(node);
            if (!sun || sun->getTag() == 0xC0FE) continue;
            sun->setTag(0xC0FE);
            sun->addOnSunCollected([this](Sun* whichSun){
                this->addSun(whichSun->sunAmount());  // +10
            });
        }
    }, 0.2f, "sun_hook_tick");

    auto* plantController = PlantController::create();
    plantController->setName("PlantController");
    this->addChild(plantController, zOrders::PEASHOOTER_Z_ORDER);


    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameScene::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, zOrders::MENU_Z_ORDER);

    CreateUI(origin, visibleSize);
    setupPeaRetargetLoop();
    this->schedule([this](float){ this->updateZombieEating(); }, 0.1f, "zombie_bite_loop");
    return true;
}

void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}