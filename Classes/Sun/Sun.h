//
// Created by Elif Balbal on 28.08.2025.
//

#ifndef PLANTSVSZOMBIES_SUN_H
#define PLANTSVSZOMBIES_SUN_H
#include "2d/CCSprite.h"


class Sun : public cocos2d::Sprite{
public:
    CREATE_FUNC(Sun);
    bool init() override;

    int sunAmount() const {return sunAmount_;}
    void startSunFalling(const cocos2d::Rect& area);

    // callback
    // GameScene will be notified that sun is collected
    using CollectCallBack = std::function<void(Sun*)>;
    void addOnSunCollected(CollectCallBack callback) { if (callback) collectedSunListeners_.push_back(std::move(callback)); }

private:
    void enableTouchToSun();
    void collectSunNow(); // animate, notify and remove

    int sunAmount_ = 20;
    std::vector<CollectCallBack> collectedSunListeners_;
};


#endif //PLANTSVSZOMBIES_SUN_H