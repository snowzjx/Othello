//
//  OthelloScene.cpp
//  Othello
//
//  Created by Snow on 1/22/14.
//
//

#include "OthelloScene.h"
#include "../AppDelegate.h"
#include "../engine/PlayerEngine.h"
#include "../engine/AIEngine.h"
#include "../include/GraphicConfig.h"
#include "../util/PointUtil.h"
#include "../util/AnimationUtil.h"
#include "../util/PlayerUtil.h"
#include "PopupLayer.h"

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
    if (!LayerColor::initWithColor(BACKGROUND_COLOR)) {
        return false;
    }
    
    this->_othello->setDelegate(this);
    
    Size winSize = Director::getInstance()->getWinSize();
    
    this->_piecesBatchNode = SpriteBatchNode::create("othelloscene.pvr.ccz");
    this->addChild(this->_piecesBatchNode, 5);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("othelloscene.plist");
    
    Sprite *board = Sprite::createWithSpriteFrameName("Board.png");
    board->setPosition(Point(winSize.width / 2, winSize.height / 2));
    this->_piecesBatchNode->addChild(board, boardZOrder);
    
    Sprite *notice = Sprite::createWithSpriteFrameName("Notice.png");
    notice->setAnchorPoint(Point(0.5, 1));
    notice->setPosition(Point(winSize.width / 2, winSize.height));
    this->_piecesBatchNode->addChild(notice, noticeZOrder);
    
    this->setGameMode(gameMode);
    this->_othello->startOthello();
    
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
        case GameMode::aiVsAi:
            bEngine = std::shared_ptr<Engine>(new AIEngine);
            wEngine = std::shared_ptr<Engine>(new AIEngine);
            break;
        default:
            break;
    }
    this->_othello->setEngine(Player::BlackPlayer, bEngine);
    this->_othello->setEngine(Player::WhitePlayer, wEngine);
}

void OthelloLayer::othelloGameDidFinish() {
    log("Game Did Finish!");
}

std::shared_ptr<Engine> OthelloLayer::createPlayerEngine() {
    std::shared_ptr<PlayerEngine> playerEngine = std::shared_ptr<PlayerEngine>(new PlayerEngine);
    this->_actionResponderSet.insert(playerEngine);
    return playerEngine;
}

void OthelloLayer::onEnter() {
    Layer::onEnter();
    this->listener = EventListenerTouchOneByOne::create();
    this->listener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
        this->scheduleOnce(schedule_selector(OthelloLayer::setLongPress), 0.3);
        return true;
    };
    this->listener->onTouchMoved = [&](Touch* touch, Event* event) {
        this->unschedule(schedule_selector(OthelloLayer::setLongPress));
    };
    this->listener->onTouchEnded = [&](Touch* touch, Event* event) {
        this->unschedule(schedule_selector(OthelloLayer::setLongPress));
        if (this->_isLongPress) {
            log("Long press event.");
            auto location = touch->getLocation();
            this->popupToolLayer(location);
            this->_isLongPress = false;
        } else {
            auto location = touch->getLocation();
            std::pair<short, short> boardPos = PointUtil::convertToBoardFromPoint(location);
            short x = std::get<0>(boardPos);
            short y = std::get<1>(boardPos);
            log("Touch event, x: %d, y: %d.", x, y);
            for (auto itr = std::begin(this->_actionResponderSet); itr != std::end(this->_actionResponderSet); ++itr) {
                if ((*itr)->respondToMoveAction(x, y)) {
                    break;
                }
            }
        }
    };
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(this->listener, this);
}

void OthelloLayer::onExit() {
	this->_eventDispatcher->removeEventListener(this->listener);
}

void OthelloLayer::update(float delta) {
	auto currentBoardState = this->_othello->getBoard()->getBoardState();
	if (this->_storedBoardState == nullptr || this->_storedBoardState != currentBoardState) {
		log("Othello board is updating ...");
        log("Waiting for player: %d", this->_othello->getCurrentPlayer());
        for (short i = 0; i < BOARD_WIDTH; i ++) {
            for (short j = 0; j < BOARD_HEIGHT; j++) {
                auto deltaValue = (*currentBoardState)[i][j] - (this->_storedBoardState == nullptr ? 0 : (*this->_storedBoardState)[i][j]);
                auto newStatus = (*currentBoardState)[i][j] == 1 ? PieceSpriteStatus::BlackPiece : PieceSpriteStatus::WhitePiece;
                if (abs(deltaValue) == 1) {
                    if ((*currentBoardState)[i][j] == 0) {
                        this->removePieceAt(i, j);
                    } else {
                        this->createPieceAt(i, j, newStatus);
                    }
                } else if (abs(deltaValue) == 2) {
                    this->changePieceStatusAt(i, j, newStatus);
                }
            }
        }
        this->_storedBoardState = currentBoardState;
	}
    for (auto itr = std::begin(this->_actionResponderSet); itr != std::end(this->_actionResponderSet); ++itr) {
        if ((*itr)->getStatus() == ActionResponderStatus::NEED_TO_ASK_FOR_USER_COMFIRM) {
            (*itr)->setStatus(ActionResponderStatus::WAITING_FOR_USER_COMFIRM_ACTION);
            this->popupUndoLayer();
        }
    }
}

void OthelloLayer::undoCallBack(cocos2d::Object *pSender) {
    this->hideToolLayer(this);
    for (auto itr = std::begin(this->_actionResponderSet); itr != std::end(this->_actionResponderSet); ++itr) {
        if ((*itr)->respondToUndoAction()) {
            break;
        }
    }
}

void OthelloLayer::undoComfirmCallBack(cocos2d::Object *pSender) {
    this->hidePopupUndoLayer();
    for (auto itr = std::begin(this->_actionResponderSet); itr != std::end(this->_actionResponderSet); ++itr) {
        if ((*itr)->respondToUndoComfirmAction(true)) {
            break;
        }
    }
}

void OthelloLayer::undoCancelCallBack(cocos2d::Object *pSender) {
    this->hidePopupUndoLayer();
    for (auto itr = std::begin(this->_actionResponderSet); itr != std::end(this->_actionResponderSet); ++itr) {
        if ((*itr)->respondToUndoComfirmAction(false)) {
            break;
        }
    }
}

void OthelloLayer::popupToolLayer(cocos2d::Point pos) {
    PopupLayer *popupLayer = PopupLayer::create();
    popupLayer->setBackgroundImage("TackBackPopupBG.png");
    popupLayer->setPosition(Point(pos));
    
    auto tackBackLabel = LabelTTF::create("Tack Back", "Helvetica.ttf", 8);
    tackBackLabel->setColor(FOREGROUND_COLOR);
    auto tackBackItem = MenuItemLabel::create(tackBackLabel, CC_CALLBACK_1(OthelloLayer::undoCallBack, this));
    popupLayer->addMenuItem(tackBackItem, Point(-25, 4));
    
    auto cancelLabel = LabelTTF::create("Cancel", "Helvetica.ttf", 8);
    cancelLabel->setColor(FOREGROUND_COLOR);
    auto cancelItem = MenuItemLabel::create(cancelLabel, CC_CALLBACK_1(OthelloLayer::hideToolLayer, this));
    popupLayer->addMenuItem(cancelItem, Point(30, 4));
    
    this->addChild(popupLayer, popupZOrder, toolPopupTag);

}

void OthelloLayer::hideToolLayer(cocos2d::Object *pSender) {
    this->removeChildByTag(toolPopupTag);
}

void OthelloLayer::popupUndoLayer() {
    Size winSize = Director::getInstance()->getWinSize();
    PopupLayer *popupLayer = PopupLayer::create();
    popupLayer->setBackgroundImage("PopupBG.png");
    popupLayer->setPosition(Point(winSize.width / 2, winSize.height / 2));
    
    auto titleLabel = LabelTTF::create("Agree with taking back ?", "Helvetica.ttf", 10);
    titleLabel->setColor(FOREGROUND_COLOR);
    popupLayer->setTitle(titleLabel);
    
    std::string player = PlayerUtil::swapPlayer(this->_othello->getCurrentPlayer()) == Player::BlackPlayer ? "black" : "white";
    std::string infoStr = "The " + player + " player wants to take back his previous move, do you agree ?";
    auto contextLabel = LabelTTF::create(infoStr, "Helvetica.ttf", 8, Size(150, 0), TextHAlignment::LEFT);
    contextLabel->setColor(Color3B::BLACK);
    popupLayer->setContext(contextLabel);
    
    auto okLabel = LabelTTF::create("Ok", "Helvetica.ttf", 10);
    okLabel->setColor(FOREGROUND_COLOR);
    auto okItem = MenuItemLabel::create(okLabel, CC_CALLBACK_1(OthelloLayer::undoComfirmCallBack, this));
    
    auto cancelLabel = LabelTTF::create("Cancel", "Helvetica.ttf", 10);
    cancelLabel->setColor(FOREGROUND_COLOR);
    auto cancelItem = MenuItemLabel::create(cancelLabel, CC_CALLBACK_1(OthelloLayer::undoCancelCallBack, this));
    
    popupLayer->addMenuItem(okItem, Point(-45, -42));
    popupLayer->addMenuItem(cancelItem, Point(45, -42));
    
    this->addChild(popupLayer, popupZOrder, undoPopupTag);
}

void OthelloLayer::hidePopupUndoLayer() {
    this->removeChildByTag(undoPopupTag);
}

void OthelloLayer::createPieceAt(short i, short j, PieceSpriteStatus status ) {
    PieceSprite *pieceSprite = PieceSprite::createWithPieceSpriteStatus(status);
    pieceSprite->setScale(0);
    pieceSprite->setPosition(PointUtil::convertToPointFromBoard(i, j));
    this->_pieceSpriteVector[i * BOARD_WIDTH + j] = pieceSprite;
    this->_piecesBatchNode->addChild(pieceSprite, pieceZOrder);
    pieceSprite->runAction(ScaleTo::create(0.2, 1));
}
void OthelloLayer::changePieceStatusAt(short i, short j, PieceSpriteStatus status) {
    this->_pieceSpriteVector[i * BOARD_WIDTH + j]->setStatus(status);
}

void OthelloLayer::removePieceAt(short i, short j) {
    auto piece = this->_pieceSpriteVector[i * BOARD_WIDTH + j];
    piece->removeFromParent();
}
void OthelloLayer::setLongPress(float delta) {
    this->_isLongPress = true;
}