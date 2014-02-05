//
//  PopupLayer.cpp
//  Othello
//
//  Created by Snow on 2/1/14.
//
//

#include "PopupLayer.h"

USING_NS_CC;

bool PopupLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    this->_memu = Menu::create();
    this->_memu->setPosition(Point(0, 0));
    this->addChild(this->_memu, 1);
    return true;
}

void PopupLayer::onEnter() {
    Layer::onEnter();
    this->listener = EventListenerTouchOneByOne::create();
    this->listener->setSwallowTouches(true);
    this->listener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
        return true;
    };
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(this->listener, this);
}

void PopupLayer::onExit() {
    Layer::onExit();
    this->_eventDispatcher->removeEventListener(this->listener);
}

void PopupLayer::setBackgroundImage(const std::string &backgroundImage) {
    Sprite *backgroundSprite = Sprite::createWithSpriteFrameName(backgroundImage);
    this->addChild(backgroundSprite, 0);
}

void PopupLayer::setTitle(cocos2d::LabelTTF *titleLabel) {
    titleLabel->setAnchorPoint(Point(0.5, 0.5));
    titleLabel->setPosition(Point(0, 35));
    this->addChild(titleLabel, 1);
}

void PopupLayer::setContext(cocos2d::LabelTTF *contentLabel) {
    contentLabel->setAnchorPoint(Point(0.5, 0.5));
    contentLabel->setPosition(Point(0, 0));
    this->addChild(contentLabel, 1);
}

void PopupLayer::addMenuItem(cocos2d::MenuItem *menuItem, cocos2d::Point pos) {
    menuItem->setPosition(pos);
    this->_memu->addChild(menuItem, 1);
}

