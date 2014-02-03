//
//  MenuScene.h
//  Othello
//
//  Created by Snow on 2/3/14.
//
//

#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "cocos2d.h"

class MenuLayer: public cocos2d::LayerColor {
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(MenuLayer);
    virtual bool init() override;
    void onePlayerCallBack(cocos2d::Object* pSender);
    void twoPlayerCallBack(cocos2d::Object* pSender);
private:
    cocos2d::SpriteBatchNode *_batchNode;
    cocos2d::Menu *_menu;
};

#endif
