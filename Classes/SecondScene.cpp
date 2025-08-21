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
#include "HelloWorldScene.h"
#include "cocos/2d/CCMenu.h"
#include "cocos/2d/CCMenuItem.h"
#include "cocos/2d/CCLabel.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"
#include  "SecondScene.h"

#include "CCEventDispatcher.h"
#include "CCEventListenerMouse.h"
#include "Zombies/ZombieWeak.h"
#include "Plants/PeaShooter.h"

#include "2d/CCActionInterval.h"
#include "cocos/2d/CCAnimation.h"

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
}
Scene* SecondScene::createScene()
{
    return SecondScene::create();
}

// Print useful error message instead of segfault when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
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

cocos2d::Sprite* CreatePeaShooterSprite (const cocos2d::Size& visibleSize) {
    auto* const peaShooter = cocos2d::Sprite::create("peaShooter.png");
    if (!peaShooter) {
        problemLoading("peaShooter.png");
        return nullptr;
    }

    peaShooter->setPosition({
        210.f, 270.f
    });
    peaShooter->setScale(0.2f);
    return peaShooter;
}

cocos2d::Sprite* CreatePeaSprite (const cocos2d::Size& visibleSize) {
    auto* const pea = cocos2d::Sprite::create("pea.png");
    if (!pea) {
        problemLoading("pea.png");
        return nullptr;
    }

    pea->setScale(1.0f);
    return pea;
}

// Grid Layout
void SecondScene::BuildGroundGrid(const Size& visibleSize, const Vec2& origin)
{
    const float fieldW = grid::fieldWidth(visibleSize);
    const float fieldH = grid::fieldHeight(visibleSize);

    mCellSize   = Size(fieldW / GRID_COLS, fieldH / GRID_ROWS);
    mGridOrigin = Vec2(origin.x + grid::MARGIN_L, origin.y + grid::MARGIN_B);

    for (int row = 0; row < GRID_ROWS; ++row)
    {
        for (int col = 0; col < GRID_COLS; ++col)
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
            mTiles[index(row, col)] = tile;                         // store for later (hover, highlight, etc.)
        }
    }
}

// this function coverts the grid coordinate into actual pixel positions
Vec2 SecondScene::CellCenter(int col, int row) const
{
    col = max(0, min(GRID_COLS - 1, col));
    row = max(0, min(GRID_ROWS - 1, row));
    return Vec2(
        mGridOrigin.x + mCellSize.width  * (col + 0.5f),
        mGridOrigin.y + mCellSize.height * (row + 0.5f)
    );
}

void SecondScene::setupGridHover()
{
    mHoverRect = cocos2d::DrawNode::create();
    this->addChild(mHoverRect, zOrders::HOVERED_TILE_Z_ORDER); // above tiles

    auto listener = cocos2d::EventListenerMouse::create();
    listener->onMouseMove = [this](cocos2d::EventMouse* event)
    {
        cocos2d::Vec2 pos = event->getLocationInView();
        auto vs = cocos2d::Director::getInstance()->getVisibleSize();
        auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

        // Convert mouse pos into grid col/row
        float relX = pos.x - mGridOrigin.x;
        float relY = pos.y - mGridOrigin.y;

        int col = (int)(relX / mCellSize.width);
        int row = (int)(relY / mCellSize.height);

        if (col < 0 || col >= GRID_COLS || row < 0 || row >= GRID_ROWS)
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

            // whitish semi-transparent highlight
            cocos2d::Color4F hoverColor(1.f, 1.f, 1.f, 0.35f);
            mHoverRect->drawSolidPoly(rect, 4, hoverColor);
        }
    };
    listener->onMouseDown = [this](cocos2d::EventMouse* event) {
        if (event->getMouseButton() != cocos2d::EventMouse::MouseButton::BUTTON_LEFT)
            return;

        if (mHoverIdx <= 0) return;

        int row = mHoverIdx / GRID_COLS;
        int col = mHoverIdx % GRID_COLS;

        // To not place 2 peaShooters on the same tile
        if (tileAt(row, col)->getChildByName("PeaShooter")) return;

        auto* peaShooter = PeaShooter::create();
        if (!peaShooter) return;

        peaShooter->setPosition(CellCenter(col, row));
        peaShooter->setName("PeaShooter");

        this->addChild(peaShooter, zOrders::PEASHOOTER_Z_ORDER);
        peaShooter->startAutoFire(mZombie, this, zOrders::PEA_Z_ORDER);

    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void SecondScene::CreateUI(const cocos2d::Vec2& origin, const cocos2d::Size& visibleSize)
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

    if (auto* const peaShooter = CreatePeaShooterSprite(visibleSize)) {
        mPeaShooterSprite = peaShooter;
        this->addChild(peaShooter, zOrders::PEASHOOTER_Z_ORDER);
    }

    if (auto* const pea = CreatePeaSprite(visibleSize)) {
        mPeaSprite = pea;
        this->addChild(pea, zOrders::PEA_Z_ORDER);
    }

    auto* zombie = ZombieWeak::create();
    zombie->setPosition(650.f, 300.f);  // start position
    this->addChild(zombie, zOrders::ZOMBIE_Z_ORDER);
    mZombie = zombie;

    if (mPeaShooterSprite && mZombie) {
        // shoot the zombie with pea periodically
        this->schedule([=](float) {    // starts a timer
            // create pea
            auto pea = CreatePeaSprite(Director::getInstance()->getVisibleSize());  // create pea only if there are both peaShooter and zombie
            if (!pea) return;

            const cocos2d::Vec2 muzzleOffset(40.f, 20.f);  // to make the pea occur from the "mouth" of the peaShooter
            const cocos2d::Vec2 startPos = mPeaShooterSprite->getPosition() + muzzleOffset;
            pea->setPosition(startPos);
            this->addChild(pea, zOrders::PEA_Z_ORDER);

            const float peaSpeed = 300.0f;

            pea->schedule([this, pea, peaSpeed](float dt) {
                if (!pea->getParent()) return;

                pea->setPositionX(pea->getPositionX() + peaSpeed * dt); // move the pea

                // off-screen situations
                auto* dir    = cocos2d::Director::getInstance();
                const auto visibleSize     = dir->getVisibleSize();
                const auto origin = dir->getVisibleOrigin();
                if (pea->getPositionX() > origin.x + visibleSize.width + 40.f) {
                    pea->removeFromParent();
                    return;
                }
                // collision situation !
                if (mZombie && mZombie->getParent()) {
                    auto peaRect = pea->getBoundingBox();
                    auto zombieRect = mZombie->getBoundingBox();

                    auto inset = [] (cocos2d::Rect rect, float px, float py) {
                        rect.origin.x += px; rect.origin.y += py;
                        rect.size.width -= 2*px;
                        rect.size.height -= 2*py;
                        return rect;
                    };

                    peaRect = inset(peaRect,peaRect.size.width * 0.1f,    peaRect.size.height * 0.30f);
                    zombieRect = inset(zombieRect, zombieRect.size.width * 0.50f, zombieRect.size.height * 0.20f);

                    if (peaRect.intersectsRect(zombieRect)) {
                        mZombie->takeDamage(20.f);
                        cocos2d::log("Zombie HP: %.1f", mZombie->hp());  // It correctly decreases the hp of zombie
                        pea->removeFromParent();
                        return;
                    }
            }
            },"pea_step");
        }, 0.8f, "pea_fire_timer");  // per 0.8 seconds
    }
}

// on "init" you need to initialize your instance
bool SecondScene::init()
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

        // add it at z-order 0 so it stays behind everything else
        this->addChild(background, zOrders::BACKGROUND_Z_ORDER);
    }
    BuildGroundGrid(visibleSize, origin);
    setupGridHover();

    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(SecondScene::menuCloseCallback, this));

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
    return true;
}

void SecondScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

