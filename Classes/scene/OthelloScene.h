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
#include "../game/OthelloDelegate.h"
#include "../control/ActionResponder.h"
#include "../sprite/PieceSprite.h"
#include "../sprite/PieceSpriteStatus.h"

class OthelloLayer: public cocos2d::LayerColor, public OthelloDelegate {
public:
    static cocos2d::Scene* createScene(GameMode gameMode);
    static OthelloLayer* create(GameMode gameMode);
    virtual bool init(GameMode gameMode);
    void setGameMode(GameMode gameMode);
    virtual void othelloGameDidFinish() override;

private:
    std::shared_ptr<Engine> createPlayerEngine();
    virtual void onEnter() override;
    virtual void update(float delta) override;
    void undoCallBack(Object* pSender);
    void undoComfirmCallBack(Object *pSender);
    void undoCancelCallBack(Object *pSender);
    void createPieceAt(short i, short j, PieceSpriteStatus status );
    void changePieceStatusAt(short i, short j, PieceSpriteStatus status);
    
private:
    std::shared_ptr<Othello> _othello = std::shared_ptr<Othello>(Singleton<Othello>::getInstance());
    std::shared_ptr<Matrix<short>> _storedBoardState = nullptr;
    std::set<std::shared_ptr<ActionResponder>> _actionResponderSet;
    cocos2d::SpriteBatchNode* _piecesBatchNode;
    std::vector<PieceSprite*> _pieceSpriteVector = std::vector<PieceSprite*>(BOARD_WIDTH * BOARD_HEIGHT);
};

#endif
