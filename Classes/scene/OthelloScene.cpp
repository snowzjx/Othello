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
    
    this->setGameMode(gameMode);
    this->_othello->startOthello();
    
    //TODO: Test code here
    this->popupUndoLayer();
    
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
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
        return true;
    };
    listener->onTouchEnded = [&](Touch* touch, Event* event) {
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
    };
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
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
                    this->createPieceAt(i, j, newStatus);
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
            //TODO: add pop over layer
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

void OthelloLayer::popupUndoLayer() {
    Size winSize = Director::getInstance()->getWinSize();
    PopupLayer *popupLayer = PopupLayer::create();
    popupLayer->setBackgroundImage("PopupBG.png");
    popupLayer->setPosition(Point(winSize.width / 2, winSize.height / 2));
    this->addChild(popupLayer, popupZOrder);
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