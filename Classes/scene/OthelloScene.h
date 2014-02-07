//
//  OthelloScene.h
//  Othello
//
//  Created by Snow on 1/22/14.
//
//

#ifndef _OTHELLO_SCENE_H_
#define _OTHELLO_SCENE_H_

#include "cocos2d.h"

#include <memory>
#include <set>
#include <vector>
#include "../game/Othello.h"
#include "../engine/Engine.h"
#include "../util/Singleton.h"
#include "../core/Board.h"
#include "../core/Matrix.h"
#include "../game/GameMode.h"
#include "../control/ActionResponder.h"
#include "../sprite/PieceSprite.h"
#include "../sprite/PieceSpriteStatus.h"

class OthelloLayer: public cocos2d::LayerColor{
public:
    OthelloLayer();
    ~OthelloLayer();
    static cocos2d::Scene* createScene(GameMode gameMode);
    static OthelloLayer* create(GameMode gameMode);
    virtual bool init(GameMode gameMode);
    void setGameMode(GameMode gameMode);

private:
    std::shared_ptr<Engine> createPlayerEngine();
    virtual void onEnter() override;
	virtual void onExit() override;
    virtual void update(float delta) override;
    void undoCallBack(cocos2d::Object* pSender);
    void undoComfirmCallBack(cocos2d::Object* pSender);
    void undoCancelCallBack(cocos2d::Object* pSender);
    void popupToolLayer(cocos2d::Point pos);
    void hideToolLayer(cocos2d::Object* pSender);
    void pauseCallBack(cocos2d::Object* pSender);
    void pauseAbortCallBack(cocos2d::Object* pSender);
    void pauseResumeCallBack(cocos2d::Object* pSender);
    void popupUndoLayer();
    void hidePopupUndoLayer();
    void createPieceAt(short i, short j, PieceSpriteStatus status );
    void changePieceStatusAt(short i, short j, PieceSpriteStatus status);
    void removePieceAt(short i, short j);
    void createMoveTipAt(Player player, short i, short j);
    void clearMoveTip();
    void setLongPress(float delta);
    
private:
    std::shared_ptr<Matrix<short>> _storedBoardState = nullptr;
    std::set<std::shared_ptr<ActionResponder>> _actionResponderSet;
    cocos2d::SpriteBatchNode* _piecesBatchNode;
    std::vector<PieceSprite*> _pieceSpriteVector;
    std::vector<cocos2d::Sprite*> _moveTipSpriteVector;
    std::map<Player, cocos2d::LabelTTF*> _userScoreMap;
    cocos2d::EventListenerTouchOneByOne* _listener;
    bool _isLongPress = false;
};

#endif
