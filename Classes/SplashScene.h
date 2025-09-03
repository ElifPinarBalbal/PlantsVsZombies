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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#pragma once
#include "cocos/2d/CCScene.h"
#include "cocos/2d/CCLabel.h"
#include "cocos/2d/CCDrawNode.h"
#include "cocos/2d/CCSprite.h"

class SplashScene : public cocos2d::Scene
{
public:
    static SplashScene* createScene();
    CREATE_FUNC(SplashScene);

    bool init() override;

private:
    // flow
    void RunSplash();
    void CreateProgressBar();
    void UpdateProgressBar(float dt);
    void GoNextScene();
    cocos2d::Sprite*  mSplashLogo   = nullptr;

    cocos2d::Node*      mBarRootNode      = nullptr;
    cocos2d::DrawNode*  mBarBackgroundNode= nullptr;
    cocos2d::DrawNode*  mBarFillNode      = nullptr;
    cocos2d::Label*     mPercentLabel     = nullptr;
    cocos2d::Size       mBarSize;

    // timing
    float mElapsedSec      = 0.f;
    float mDurationSeconds = 1.8f;
    bool  mFinished        = false;
};


#endif // __HELLOWORLD_SCENE_H__
