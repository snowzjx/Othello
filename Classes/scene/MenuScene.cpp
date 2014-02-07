//
//  MenuScene.cpp
//  Othello
//
//  Created by Snow on 2/3/14.
//
//

#include "MenuScene.h"
#include "OthelloScene.h"
#include "../include/GraphicConfig.h"
#include "../game/GameMode.h"

USING_NS_CC;

Scene* MenuLayer::createScene() {
    auto scene = Scene::create();
    auto layer = MenuLayer::create();
    scene->addChild(layer);
    return scene;
}

bool MenuLayer::init() {
    if (!LayerColor::initWithColor(BACKGROUND_COLOR)) {
        return false;
    }
    
    this->_batchNode = SpriteBatchNode::create("menuscene.pvr.ccz");
    this->addChild(this->_batchNode, 5);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("menuscene.plist");
    
    auto winSize = Director::getInstance()->getWinSize();
    auto logoSprite = Sprite::createWithSpriteFrameName("OthelloLogo.png");
    logoSprite->setPosition(Point(winSize.width / 2, winSize.height / 2 + 40));
    this->_batchNode->addChild(logoSprite);
    
    this->_menu = Menu::create();
    this->_menu->setPosition(Point(winSize.width / 2, winSize.height / 2 - 50));
    this->addChild(this->_menu, 10);
    
    auto onePlayerLabel = LabelTTF::create("One Player Game", "Helvetica.ttf", 10);
    onePlayerLabel->setColor(Color3B::WHITE);
    auto onePlayerItem = MenuItemLabel::create(onePlayerLabel, CC_CALLBACK_1(MenuLayer::onePlayerCallBack, this));
    onePlayerItem->setPosition(Point(0, 0));
    this->_menu->addChild(onePlayerItem);
    
    auto twoPlayerLabel = LabelTTF::create("Two Player Game", "Helvetica.ttf", 10);
    twoPlayerLabel->setColor(Color3B::WHITE);
    auto twoPlayerItem = MenuItemLabel::create(twoPlayerLabel, CC_CALLBACK_1(MenuLayer::twoPlayerCallBack, this));
    twoPlayerItem->setPosition(Point(0, -25));
    this->_menu->addChild(twoPlayerItem);
    
    return true;
}

void MenuLayer::onePlayerCallBack(cocos2d::Object *pSender) {
    auto scene = OthelloLayer::createScene(GameMode::humanVsAi);
    Director::getInstance()->pushScene(scene);
}

void MenuLayer::twoPlayerCallBack(cocos2d::Object *pSender) {
    auto scene = OthelloLayer::createScene(GameMode::humanVsHuman);
    Director::getInstance()->pushScene(scene);

}