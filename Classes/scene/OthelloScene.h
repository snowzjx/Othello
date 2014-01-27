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
#include "../game/Othello.h"
#include "../engine/Engine.h"
#include "../util/Singleton.h"
#include "../core/Board.h"
#include "../core/Matrix.h"
#include "../game/GameMode.h"
#include "../control/ActionResponder.h"

class OthelloLayer: public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene(GameMode gameMode);
    static OthelloLayer* create(GameMode gameMode);
    virtual bool init(GameMode gameMode);
    void setGameMode(GameMode gameMode);

private:
    std::shared_ptr<Engine> createPlayerEngine();
    virtual void onEnter() override;
    virtual void update(float delta) override;
    void undoCallBack(Object* pSender);
    void undoComfirmCallBack(Object *pSender);
    void undoCancelCallBack(Object *pSender);
    
private:
    std::shared_ptr<Othello> _othello = std::shared_ptr<Othello>(Singleton<Othello>::instance());
    std::shared_ptr<Matrix<short>> _storedBoardState = nullptr;
    std::set<std::shared_ptr<ActionResponder>> _actionResponderSet;
    cocos2d::SpriteBatchNode _piecesNode;
};

#endif