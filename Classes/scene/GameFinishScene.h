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
#include <map>
#include "../core/Player.h"

class GameFinishLayer: public cocos2d::LayerColor {
public:
    static cocos2d::Scene* createScene(const std::map<Player, unsigned short>& playerScoreMap);
    static GameFinishLayer* create(const std::map<Player, unsigned short>& playerScoreMap);
    bool init(const std::map<Player, unsigned short>& playerScoreMap);
    void restartClickCallBack(cocos2d::Object* pSender);
    void quitClickCallback(cocos2d::Object* pSender);
};

#endif
