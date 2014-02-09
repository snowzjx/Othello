//
//  GameFinishScene.h
//  Othello
//
//  Created by Snow on 2/5/14.
//
//

#ifndef _GAME_FINISH_SCENE_H_
#define _GAME_FINISH_SCENE_H_

#include "cocos2d.h"

#include <map>
#include <string>
#include "../core/Player.h"
#include "../core/PlayerScoreMap.h"

class GameFinishLayer: public cocos2d::LayerColor {
public:
    static cocos2d::Scene* createScene(PlayerScoreMap playerScore);
    static GameFinishLayer* create(PlayerScoreMap playerScore);
    bool init(PlayerScoreMap playerScoreMap);
    void restartClickCallBack(cocos2d::Object* pSender);
    void quitClickCallback(cocos2d::Object* pSender);
};

#endif
