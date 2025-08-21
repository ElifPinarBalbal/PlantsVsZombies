//
// Created by Elif Balbal on 21.08.2025.
//

#include "ProjectileBase.h"
#include "cocos/base/CCDirector.h"

bool ProjectileBase::init() {
    if (!Sprite::initWithFile(spriteFile())) return false;
    setupDefaults();
    return true;
}

// moves the projectile
void ProjectileBase::start() {
    this->schedule([this] (float dt) {
        if (!this->getParent()) return;

        this->setPosition(this->getPosition() + velocity_ * dt);

        if (outOfBounds()) {this->removeFromParent(); return;}
        if (hitsTarget()) {
            onHit(target_);
        }
    }, "projectile_step");
}

bool ProjectileBase::outOfBounds() const {
    auto* dir = cocos2d::Director::getInstance();
    const auto visibleSize = dir->getVisibleSize();
    const auto origin = dir->getVisibleOrigin();
    const float extra = 40.f;
    const auto p = this->getPosition();
    return (p.x < origin.x - extra || p.x > origin.x + visibleSize.width + extra ||
            p.y < origin.y - extra || p.y > origin.y + visibleSize.height + extra);
}

static cocos2d::Rect inset(cocos2d::Rect r, float px, float py) {
    r.origin.x += px; r.origin.y += py;
    r.size.width  -= 2*px;
    r.size.height -= 2*py;
    return r;
}

bool ProjectileBase::hitsTarget() const {
    if (!target_ || !target_->getParent()) return false;

    auto peaRect    = inset(this->getBoundingBox(),
                            this->getContentSize().width  * 0.10f,
                            this->getContentSize().height * 0.30f);

    auto zRect      = inset(target_->getBoundingBox(),
                            target_->getContentSize().width  * 0.50f,
                            target_->getContentSize().height * 0.20f);

    return peaRect.intersectsRect(zRect);
}

void ProjectileBase::onHit(ZombieBase* z) {
    if (z) z->takeDamage(damage_);
    this->removeFromParent();
}