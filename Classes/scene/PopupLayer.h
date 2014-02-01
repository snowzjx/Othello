//
//  PopupLayer.h
//  Othello
//
//  Created by Snow on 2/1/14.
//
//

#ifndef _POPUP_LAYER_H_
#define _POPUP_LAYER_H_

#include "cocos2d.h"
#include <string>
#include <functional>

class PopupLayer: public cocos2d::Layer {
public:
    CREATE_FUNC(PopupLayer);
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    void setBackgroundImage(const std::string& backgroundImage);
    void setTitle(cocos2d::Label* titleLabel);
    void setContext(cocos2d::Label* contentLabel);
    void addMenuItem(cocos2d::MenuItem* menuItem, cocos2d::Point pos);
private:
    cocos2d::Menu *_memu;
};

#endif
