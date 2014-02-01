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
    return true;
}

void PopupLayer::onEnter() {
    log("PopupLayer onEnter...");
    Layer::onEnter();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
        return true;
    };
    this->_eventDispatcher->addEventListenerWithFixedPriority(listener, -128);
}

void PopupLayer::onExit() {
    this->_eventDispatcher->removeAllEventListeners();
}

void PopupLayer::setBackgroundImage(const std::string &backgroundImage) {
    Sprite *backgroundSprite = Sprite::createWithSpriteFrameName(backgroundImage);
    this->addChild(backgroundSprite, 0);
}

void PopupLayer::setTitle(const std::string &title) {
    
}

void PopupLayer::setContext(const std::string &context) {
    
}

void PopupLayer::addButton(const std::string &normalImage, const std::string &selectedImage, const std::string &title, std::function<bool (cocos2d::Object *)>) {
    
}

