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
    void setTitle(const std::string& title);
    void setContext(const std::string& context);
    void addButton(const std::string& normalImage, const std::string& selectedImage, const std::string& title, std::function<bool(cocos2d::Object* pSender)>);
};

#endif
