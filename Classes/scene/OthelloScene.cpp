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
#include "../util/StringPatch.h"
#include "PopupLayer.h"
#include "GameFinishScene.h"

USING_NS_CC;

OthelloLayer::OthelloLayer() {
    
}

OthelloLayer::~OthelloLayer() {
    log("~OthelloLayer ...");
}

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
    
    this->_pieceSpriteVector = std::vector<PieceSprite*>(BOARD_WIDTH * BOARD_HEIGHT);
    
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
    
    LabelTTF *blackPlayerScore = LabelTTF::create("0", "Helvetica.ttf", 10);
    LabelTTF *whitePlayerScore = LabelTTF::create("0", "Helvetica.ttf", 10);
    
    blackPlayerScore->setColor(FOREGROUND_COLOR);
    whitePlayerScore->setColor(FOREGROUND_COLOR);
    
    blackPlayerScore->setPosition(Point(winSize.width / 2 - 15, winSize.height - 10));
    whitePlayerScore->setPosition(Point(winSize.width / 2 + 35, winSize.height - 10));
    
    this->_userScoreMap[Player::BlackPlayer] = blackPlayerScore;
    this->_userScoreMap[Player::WhitePlayer] = whitePlayerScore;
    this->addChild(blackPlayerScore, noticeZOrder + 1);
    this->addChild(whitePlayerScore, noticeZOrder + 1);
    
    auto menu = Menu::create();
    menu->setPosition(winSize.width - 10, winSize.height - 10);
    this->addChild(menu, noticeZOrder);
    
    auto pauseBtnSprite = Sprite::createWithSpriteFrameName("PauseButton.png");
    auto pauseBtn = MenuItemSprite::create(pauseBtnSprite, pauseBtnSprite, CC_CALLBACK_1(OthelloLayer::pauseCallBack, this));
    menu->addChild(pauseBtn);
    
    this->setGameMode(gameMode);
    
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
    Singleton<Othello>::getInstance()->setEngine(Player::BlackPlayer, bEngine);
    Singleton<Othello>::getInstance()->setEngine(Player::WhitePlayer, wEngine);
}

std::shared_ptr<Engine> OthelloLayer::createPlayerEngine() {
    std::shared_ptr<PlayerEngine> playerEngine = std::shared_ptr<PlayerEngine>(new PlayerEngine);
    this->_actionResponderSet.insert(playerEngine);
    return playerEngine;
}

void OthelloLayer::onEnter() {
    Layer::onEnter();
    this->_listener = EventListenerTouchOneByOne::create();
    this->_listener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
        this->scheduleOnce(schedule_selector(OthelloLayer::setLongPress), 0.3);
        return true;
    };
    this->_listener->onTouchMoved = [&](Touch* touch, Event* event) {
        this->unschedule(schedule_selector(OthelloLayer::setLongPress));
    };
    this->_listener->onTouchEnded = [&](Touch* touch, Event* event) {
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
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(this->_listener, this);
    Singleton<Othello>::getInstance()->startOthello();
}

void OthelloLayer::onExit() {
    Layer::onExit();
	this->_eventDispatcher->removeEventListener(this->_listener);
    Singleton<Othello>::getInstance()->endOthello();
}

void OthelloLayer::update(float delta) {
    if (!Singleton<Othello>::getInstance()->getIsGameRun()) {
        log("Game Did Finish!");
        auto scene = GameFinishLayer::createScene(Singleton<Othello>::getInstance()->getPlayerScore());
        Director::getInstance()->pushScene(scene);
    }
	auto currentBoardState = Singleton<Othello>::getInstance()->getBoard()->getBoardState();
	if (this->_storedBoardState == nullptr || this->_storedBoardState != currentBoardState) {
		log("Othello board is updating ...");
        log("Waiting for player: %d", Singleton<Othello>::getInstance()->getCurrentPlayer());
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
        this->_userScoreMap[Player::BlackPlayer]->setString(string_patch::to_string(Singleton<Othello>::getInstance()->getPlayerScore()[Player::BlackPlayer]));
        this->_userScoreMap[Player::WhitePlayer]->setString(string_patch::to_string(Singleton<Othello>::getInstance()->getPlayerScore()[Player::WhitePlayer]));
        if (Singleton<Othello>::getInstance()->getShouldShowMoveTip()) {
            log("Showing move tip ...");
            auto currentPlayer = Singleton<Othello>::getInstance()->getCurrentPlayer();
            auto availPos = Singleton<Othello>::getInstance()->getPlayerAvailPos(currentPlayer);
            if (availPos.size() > 0) {
                this->clearMoveTip();
                for (auto itr = std::begin(availPos); itr != std::end(availPos); ++itr) {
                    this->createMoveTipAt(currentPlayer, std::get<0>(*itr), std::get<1>(*itr));
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

void OthelloLayer::pauseCallBack(cocos2d::Object *pSender) {
    auto size = Director::getInstance()->getWinSize();
    auto popupLayer = PopupLayer::create();
    popupLayer->setBackgroundImage("PopupBG.png");
    popupLayer->setPosition(size.width / 2, size.height / 2);
    auto titleLabel = LabelTTF::create("Game Pause", "Helvetica.ttf", 14);
    titleLabel->setColor(FOREGROUND_COLOR);
    popupLayer->setTitle(titleLabel);
    
    auto resumeLabel = LabelTTF::create("Resume", "Helvetica.ttf", 10);
    resumeLabel->setColor(FOREGROUND_COLOR);
    auto resumeItem = MenuItemLabel::create(resumeLabel, CC_CALLBACK_1(OthelloLayer::pauseResumeCallBack, this));
    popupLayer->addMenuItem(resumeItem, Point(-45, -42));
    
    auto abortLabel = LabelTTF::create("Abort", "Helvetica.ttf", 10);
    abortLabel->setColor(FOREGROUND_COLOR);
    auto abortItem = MenuItemLabel::create(abortLabel, CC_CALLBACK_1(OthelloLayer::pauseAbortCallBack, this));
    popupLayer->addMenuItem(abortItem, Point(45, -42));
    
    this->addChild(popupLayer, popupZOrder, pausePopupTag);
}

void OthelloLayer::pauseResumeCallBack(cocos2d::Object *pSender) {
    this->removeChildByTag(pausePopupTag);
}

void OthelloLayer::pauseAbortCallBack(cocos2d::Object *pSender) {
    this->removeChildByTag(pausePopupTag);
    Singleton<Othello>::getInstance()->endOthello();
}

void OthelloLayer::popupToolLayer(cocos2d::Point pos) {
    auto popupLayer = PopupLayer::create();
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
    auto popupLayer = PopupLayer::create();
    popupLayer->setBackgroundImage("PopupBG.png");
    popupLayer->setPosition(Point(winSize.width / 2, winSize.height / 2));
    
    auto titleLabel = LabelTTF::create("Agree with taking back ?", "Helvetica.ttf", 10);
    titleLabel->setColor(FOREGROUND_COLOR);
    popupLayer->setTitle(titleLabel);
    
    std::string player = PlayerUtil::swapPlayer(Singleton<Othello>::getInstance()->getCurrentPlayer()) == Player::BlackPlayer ? "black" : "white";
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

void OthelloLayer::createMoveTipAt(Player player, short i, short j) {
    std::string frameName = player == Player::BlackPlayer ? "TipBlack.png" : "TipWhite.png";
    auto tipSprite = Sprite::createWithSpriteFrameName(frameName);
    tipSprite->setScale(0);
    tipSprite->setPosition(PointUtil::convertToPointFromBoard(i, j));
    this->_moveTipSpriteVector.push_back(tipSprite);
    this->_piecesBatchNode->addChild(tipSprite, pieceZOrder);
    tipSprite->runAction(ScaleTo::create(0.2, 1));
}

void OthelloLayer::clearMoveTip() {
    if (this->_moveTipSpriteVector.size() > 0) {
        for (auto itr = std::begin(this->_moveTipSpriteVector); itr != std::end(this->_moveTipSpriteVector); ++itr) {
            (*itr)->runAction(ScaleTo::create(0.2, 0));
            (*itr)->removeFromParent();
        }
        this->_moveTipSpriteVector.clear();
    }
}

void OthelloLayer::setLongPress(float delta) {
    this->_isLongPress = true;
}