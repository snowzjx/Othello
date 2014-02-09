//
//  GameFinishScene.cpp
//  Othello
//
//  Created by Snow on 2/5/14.
//
//

#include "GameFinishScene.h"

#include "../include/GraphicConfig.h"
#include "MenuScene.h"

USING_NS_CC;

cocos2d::Scene* GameFinishLayer::createScene(PlayerScoreMap playerScore) {
    auto scene = Scene::create();
    auto layer = GameFinishLayer::create(playerScore);
    scene->addChild(layer);
    return scene;
}

GameFinishLayer* GameFinishLayer::create(PlayerScoreMap playerScore) {
    auto layer = new GameFinishLayer();
    if (layer && layer->init(playerScore)) {
        layer->autorelease();
        return layer;
    } else {
        delete layer;
        layer = NULL;
        return NULL;
    }
}

bool GameFinishLayer::init(PlayerScoreMap playerScore) {
    short blackScore = playerScore[Player::BlackPlayer];
    short whiteScore = playerScore[Player::WhitePlayer];
    Color3B foreGroundColor;
    Color4B backGroundColor;
    std::string infoString;
    std::string spriteFrameName;
    if (blackScore > whiteScore) {
        foreGroundColor = FOREGROUND_COLOR;
        backGroundColor = Color4B::WHITE;
        infoString = "Black Player Win!";
        spriteFrameName = "BlackWinner.png";
    } else {
        foreGroundColor = Color3B::WHITE;
        backGroundColor = BACKGROUND_COLOR;
        infoString = "White Player Win!";
        spriteFrameName = "WhiteWinner.png";
    }
    if (!LayerColor::initWithColor(backGroundColor)) {
        return false;
    }
    auto winSize = Director::getInstance()->getWinSize();
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("finishscene.plist");
    auto winnerLogo = Sprite::createWithSpriteFrameName(spriteFrameName);
    winnerLogo->setPosition(Point(winSize.width / 2, winSize.height / 2 + 50));
    this->addChild(winnerLogo, 1);
    auto label = LabelTTF::create(infoString, "Helvetica.ttf", 16);
    label->setPosition(Point(winSize.width / 2, winSize.height / 2 - 30));
    label->setColor(foreGroundColor);
    this->addChild(label, 1);
    
    auto menu = Menu::create();
    menu->setPosition(Point(winSize.width / 2, winSize.height / 2 - 70));
    this->addChild(menu, 1);
    auto restartLabel = LabelTTF::create("Restart", "Helvetica.ttf", 12);
    restartLabel->setColor(foreGroundColor);
    auto restartItem = MenuItemLabel::create(restartLabel, CC_CALLBACK_1(GameFinishLayer::restartClickCallBack, this));
    restartItem->setPosition(-80, 0);
    menu->addChild(restartItem);
    
    auto quitLabel = LabelTTF::create("Quit", "Helvetica.ttf", 12);
    quitLabel->setColor(foreGroundColor);
    auto quitItem = MenuItemLabel::create(quitLabel, CC_CALLBACK_1(GameFinishLayer::quitClickCallback, this));
    quitItem->setPosition(80, 0);
    menu->addChild(quitItem);
    
    return true;
}

void GameFinishLayer::restartClickCallBack(cocos2d::Object *pSender) {
    Director::getInstance()->popScene();
}

void GameFinishLayer::quitClickCallback(cocos2d::Object *pSender) {
    Director::getInstance()->popToRootScene();
}