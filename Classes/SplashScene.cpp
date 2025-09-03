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
#include "GameScene.h"

#include <cocos/2d/CCLayer.h>
#include <cocos/2d/CCAction.h>
#include <cocos/2d/CCTransition.h>

#include "CCDirector.h"
#include "ccUTF8.h"
#include "2d/CCActionInterval.h"

using namespace cocos2d;

namespace {
    constexpr float FADE_IN_DUR  = 0.5f;
    const std::string LOGO_PATH  = "sunFlower.png";

    inline Color4F bgFill()   { return Color4F(1, 1, 1, 0.10f); }
    inline Color4F barFill()  { return Color4F(0.29f, 0.78f, 0.45f, 1.0f); }
    inline Color4F barEdge()  { return Color4F(1, 1, 1, 0.35f); }
}

SplashScene* SplashScene::createScene()
{
    return SplashScene::create();
}

bool SplashScene::init()
{
    if (!Scene::init()) return false;
    addChild(LayerColor::create(Color4B(10, 10, 14, 255)), -1);
    RunSplash();
    return true;
}

void SplashScene::RunSplash()
{
    const auto visibleSize = _director->getVisibleSize();

    mSplashLogo = Sprite::create(LOGO_PATH);
    if (!mSplashLogo)
    {
        CCLOG("[SplashScene] Missing logo at %s", LOGO_PATH.c_str());
        GoNextScene();
        return;
    }

    mSplashLogo->setOpacity(0);
    mSplashLogo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    mSplashLogo->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.60f));
    addChild(mSplashLogo);

    const float maxFactor = 0.80f;
    const float maxWidth  = visibleSize.width  * maxFactor;
    const float maxHeight = visibleSize.height * maxFactor;
    const Size  bb        = mSplashLogo->getBoundingBox().size;
    const float logoScale = std::min(1.0f, std::min(maxWidth / bb.width, maxHeight / bb.height));
    mSplashLogo->setScale(logoScale);

    mSplashLogo->runAction(FadeIn::create(FADE_IN_DUR));

    CreateProgressBar();
    schedule(CC_SCHEDULE_SELECTOR(SplashScene::UpdateProgressBar));
}

void SplashScene::CreateProgressBar()
{
    const auto visibleSize = _director->getVisibleSize();

    const float width  = std::min(visibleSize.width * 0.70f, 640.0f);
    const float height = std::max(visibleSize.height * 0.018f, 16.0f);
    mBarSize = Size(width, height);

    mBarRootNode = Node::create();
    mBarRootNode->setContentSize(mBarSize);
    mBarRootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    mBarRootNode->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.38f));
    addChild(mBarRootNode);

    mBarBackgroundNode = DrawNode::create();
    const Vec2 bl(0, 0), tr(mBarSize.width, mBarSize.height);
    mBarBackgroundNode->drawSolidRect(bl, tr, bgFill());
    mBarBackgroundNode->drawRect(bl, tr, barEdge());
    mBarRootNode->addChild(mBarBackgroundNode);

    mBarFillNode = DrawNode::create();
    mBarRootNode->addChild(mBarFillNode);

    mPercentLabel = Label::createWithSystemFont("Loading 0%", "Arial", std::max(18.0f, height * 0.9f));
    mPercentLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    mPercentLabel->setPosition(Vec2(mBarSize.width * 0.5f, mBarSize.height * 0.5f));
    mPercentLabel->setTextColor(Color4B(255,255,255,230));
    mBarRootNode->addChild(mPercentLabel, 2);
}

void SplashScene::UpdateProgressBar(float dt)
{
    if (mFinished) { unschedule(CC_SCHEDULE_SELECTOR(SplashScene::UpdateProgressBar)); return; }

    mElapsedSec += dt;
    float t = mElapsedSec / mDurationSeconds;
    t = std::min(t, 1.0f);

    mBarFillNode->clear();
    const float w = mBarSize.width * t;
    if (w > 0.0f)
        mBarFillNode->drawSolidRect(Vec2(0, 0), Vec2(w, mBarSize.height), barFill());

    const int pct = static_cast<int>(roundf(t * 100.0f));
    mPercentLabel->setString(StringUtils::format("Loading %d%%", pct));

    if (t >= 1.0f)
    {
        unschedule(CC_SCHEDULE_SELECTOR(SplashScene::UpdateProgressBar));
        GoNextScene();
    }
}

void SplashScene::GoNextScene()
{
    if (mFinished) return;
    mFinished = true;

    if (mSplashLogo) mSplashLogo->stopAllActions();

    auto next = GameScene::createScene();
    auto transition = TransitionFade::create(0.35f, next, Color3B::BLACK);
    _director->replaceScene(transition);
}
