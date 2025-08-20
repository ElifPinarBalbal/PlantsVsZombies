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
#include "CCDirector.h"
#include "SecondScene.h"
#include "2d/CCTransition.h"

USING_NS_CC;

#if CC_DEBUG == 0

#endif

namespace
{
    void LogUICreationError()
    {
#if CC_DEBUG == 1
        return;
#endif

        // ...
    }

    template <typename T, typename... Args>
    T* CreateCocosObject(Args... args)
    {
        auto* ret = T::create(std::forward<Args>(args)...);
        if (!ret) {

#if CC_DEBUG == 1
            // problem loading here
#endif

            return nullptr;
        }

        return ret;
    }
}

namespace zOrders
{
    constexpr int MENU_Z_ORDER = 1;
    constexpr int GO_BUTTON_Z_ORDER = 2;
    constexpr int MLABEL_Z_ORDER = 1;
}
using namespace cocos2d::ui;

Scene* HelloWorldScene::createScene()
{
    return HelloWorldScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorldScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto width = visibleSize.width / 2;
    auto height = visibleSize.height / 2;
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorldScene::menuCloseCallback, this));

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

    auto go_button = cocos2d::ui::Button::create("Red_button.png", "Red_button.png");
    go_button->setPosition(visibleSize/2);
    go_button->setTitleText("Go!");
    go_button->setTitleFontSize(35);
    go_button->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);    // to put the title at the center of the button
    go_button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
    {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
        // Use pushScene so we can pop back to this exact scene
        cocos2d::Director::getInstance()->pushScene(
            cocos2d::TransitionFade::create(0.3f, SecondScene::createScene())
        );
    }
    });

    this->addChild(go_button, zOrders::GO_BUTTON_Z_ORDER);

     mLabel = cocos2d::Label::createWithTTF("Let's Start!", "fonts/Marker Felt.ttf", 42);
    if (mLabel == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        mLabel->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - mLabel->getContentSize().height*10));

        // add the label as a child to this layer
        this->addChild(mLabel, zOrders::MLABEL_Z_ORDER);
    }
    return true;
}


void HelloWorldScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}


