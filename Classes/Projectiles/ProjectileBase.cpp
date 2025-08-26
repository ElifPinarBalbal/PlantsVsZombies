//
// Created by Elif Balbal on 21.08.2025.
//

#include "ProjectileBase.h"
#include "cocos/base/CCDirector.h"
#include "cocos/math/CCGeometry.h"

bool ProjectileBase::init() {
    if (!Sprite::initWithFile(spriteFile())) return false;
    setupDefaults();
    return true;
}

static cocos2d::Rect rectInWorld(const cocos2d::Node* n)
{
    cocos2d::Rect r = n->getBoundingBox();
    const cocos2d::Node* p = n->getParent();
    if (!p) return r;

    cocos2d::Vec2 bl = r.origin;
    cocos2d::Vec2 br = r.origin + cocos2d::Vec2(r.size.width, 0.f);
    cocos2d::Vec2 tl = r.origin + cocos2d::Vec2(0.f, r.size.height);
    cocos2d::Vec2 tr = r.origin + cocos2d::Vec2(r.size.width, r.size.height);

    bl = p->convertToWorldSpace(bl);
    br = p->convertToWorldSpace(br);
    tl = p->convertToWorldSpace(tl);
    tr = p->convertToWorldSpace(tr);

    float minx = std::min(std::min(bl.x, br.x), std::min(tl.x, tr.x));
    float maxx = std::max(std::max(bl.x, br.x), std::max(tl.x, tr.x));
    float miny = std::min(std::min(bl.y, br.y), std::min(tl.y, tr.y));
    float maxy = std::max(std::max(bl.y, br.y), std::max(tl.y, tr.y));

    return {{minx, miny}, {maxx - minx, maxy - miny}};
}

void ProjectileBase::startShootingProjectile() {
    this->schedule([this] (float dt) {
        if (!this->getParent()) return;

        this->setPosition(this->getPosition() + velocity_ * dt);

        if (IsPeaOutOfBounds()) {this->removeFromParent(); return;}
        if (IsHittingTarget()) {
            onProjectileHitZombie(target_);
        }
    }, "projectile_step");
}

bool ProjectileBase::IsPeaOutOfBounds() const {
    auto* dir = cocos2d::Director::getInstance();
    const auto visibleSize = dir->getVisibleSize();
    const auto origin = dir->getVisibleOrigin();
    const float extra = 40.f;
    const auto p = this->getPosition();
    return (p.x < origin.x - extra || p.x > origin.x + visibleSize.width + extra ||
            p.y < origin.y - extra || p.y > origin.y + visibleSize.height + extra);
}

static cocos2d::Rect addInsetToBoundingBoxes(cocos2d::Rect r, float px, float py) {
    r.origin.x += px; r.origin.y += py;
    r.size.width  -= 2*px;
    r.size.height -= 2*py;
    return r;
}

// calculates collision using worldPositions
bool ProjectileBase::IsHittingTarget() const {
    if (!target_ || !target_->getParent() || target_->isDead()) return false;

    auto peaRect = rectInWorld(this);
    auto targetRect = rectInWorld(target_);

    peaRect    = addInsetToBoundingBoxes(peaRect,
                            peaRect.size.width  * 0.10f,
                            peaRect.size.height * 0.30f);

    targetRect      = addInsetToBoundingBoxes(targetRect,
                            targetRect.size.width  * 0.50f,
                            targetRect.size.height * 0.20f);

    return peaRect.intersectsRect(targetRect);
}

void ProjectileBase::onProjectileHitZombie(ZombieBase* zombie) {
    if (zombie) {
        zombie->takeDamage(damage_);
    }
    this->removeFromParent();
}