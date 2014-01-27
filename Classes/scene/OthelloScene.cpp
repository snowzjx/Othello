//
//  OthelloScene.cpp
//  Othello
//
//  Created by Snow on 1/22/14.
//
//

#include "OthelloScene.h"
#include "../engine/PlayerEngine.h"
#include "../engine/AIEngine.h"

USING_NS_CC;

Scene* OthelloLayer::createScene(GameMode gameMode) {
    auto scene = Scene::create();
    auto layer = OthelloLayer::create(gameMode);
    scene->addChild(layer);
    return scene;
}

OthelloLayer* OthelloLayer::create(GameMode gameMode) {
    auto layer = new OthelloLayer();
    if (layer && layer->init(gameMode)) {
        layer->autorelease();
        return layer;
    } else {
        delete layer;
        layer = NULL;
        return NULL;
    }
}

bool OthelloLayer::init(GameMode gameMode) {
    if (!Layer::init()) {
        return false;
    }
    
    this->setGameMode(gameMode);
    this->_othello->startOthello();
        
    // ------------- Test -------------
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    auto closeItem = MenuItemImage::create("CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(OthelloLayer::undoCallBack, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                 origin.y + closeItem->getContentSize().height/2));
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    // --------------------------------
    this->scheduleUpdate();
    return true;
}

void OthelloLayer::setGameMode(GameMode gameMode) {
    std::shared_ptr<Engine> bEngine = nullptr;
    std::shared_ptr<Engine> wEngine = nullptr;
    switch (gameMode) {
        case GameMode::humanVsHuman:
            bEngine = this->createPlayerEngine();
            wEngine = this->createPlayerEngine();
            break;
        case GameMode::humanVsAi:
            bEngine = this->createPlayerEngine();
            wEngine = std::shared_ptr<Engine>(new AIEngine);
            break;
        case GameMode::aiVsHuman:
            bEngine = std::shared_ptr<Engine>(new AIEngine);
            wEngine = this->createPlayerEngine();
            break;
        default:
            break;
    }
    this->_othello->setEngine(Player::BlackPlayer, bEngine);
    this->_othello->setEngine(Player::WhitePlayer, wEngine);
}

std::shared_ptr<Engine> OthelloLayer::createPlayerEngine() {
    std::shared_ptr<PlayerEngine> playerEngine = std::shared_ptr<PlayerEngine>(new PlayerEngine);
    this->_actionResponderSet.insert(playerEngine);
    return playerEngine;
}

void OthelloLayer::onEnter() {
    Layer::onEnter();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
        return true;
    };
    listener->onTouchEnded = [&](Touch* touch, Event* event) {
        auto location = touch->getLocation();
        float touchX = location.x;
        float touchY = location.y;
        for (auto itr = std::begin(this->_actionResponderSet); itr != std::end(this->_actionResponderSet); ++itr) {
            if ((*itr)->respondToMoveAction(touchX, touchY)) {
                break;
            }
        }
    };
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void OthelloLayer::update(float delta) {
	auto currentBoardState = this->_othello->getBoard()->getBoardState();
	if (this->_storedBoardState == nullptr || this->_storedBoardState != currentBoardState) {
		log("Othello board is updating ...");
		this->_storedBoardState = currentBoardState;
	}
    for (auto itr = std::begin(this->_actionResponderSet); itr != std::end(this->_actionResponderSet); ++itr) {
        if ((*itr)->getStatus() == ActionResponderStatus::NEED_TO_ASK_FOR_USER_COMFIRM) {
            (*itr)->setStatus(ActionResponderStatus::WAITING_FOR_USER_COMFIRM_ACTION);
            //TEST CODE BEGIN
            Size visibleSize = Director::getInstance()->getVisibleSize();
            Point origin = Director::getInstance()->getVisibleOrigin();
            auto closeItem = MenuItemImage::create("CloseNormal.png",
                                                   "CloseSelected.png",
                                                   CC_CALLBACK_1(OthelloLayer::undoComfirmCallBack, this));
            closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                         origin.y + 100 + closeItem->getContentSize().height/2));
            auto menu = Menu::create(closeItem, NULL);
            menu->setPosition(Point::ZERO);
            this->addChild(menu, 1);
            //TEST CODE END
        }
    }
}

void OthelloLayer::undoCallBack(cocos2d::Object *pSender) {
    for (auto itr = std::begin(this->_actionResponderSet); itr != std::end(this->_actionResponderSet); ++itr) {
        if ((*itr)->respondToUndoAction()) {
            break;
        }
    }
}

void OthelloLayer::undoComfirmCallBack(cocos2d::Object *pSender) {
    for (auto itr = std::begin(this->_actionResponderSet); itr != std::end(this->_actionResponderSet); ++itr) {
        if ((*itr)->respondToUndoComfirmAction(true)) {
            break;
        }
    }
}

void OthelloLayer::undoCancelCallBack(cocos2d::Object *pSender) {
    for (auto itr = std::begin(this->_actionResponderSet); itr != std::end(this->_actionResponderSet); ++itr) {
        if ((*itr)->respondToUndoComfirmAction(false)) {
            break;
        }
    }
}