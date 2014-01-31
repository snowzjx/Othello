//
//  AnimationUtil.h
//  Othello
//
//  Created by Snow on 1/29/14.
//
//

#ifndef _ANIMATION_UTIL_H_
#define _ANIMATION_UTIL_H_

#include "cocos2d.h"
#include <string>

class AnimationUtil {
public:
    static cocos2d::Animation* animationFromPlist(std::string animPlist, float delay) {
        cocos2d::ValueVector animImages = cocos2d::FileUtils::getInstance()->getValueVectorFromFile(animPlist);
        cocos2d::Vector<cocos2d::SpriteFrame* > animFrames;
        for (int i = 0; i < animImages.size(); i++) {
            auto animImage = animImages[i].asString();
            animFrames.pushBack(cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(animImage));
        }
        auto animation = cocos2d::Animation::createWithSpriteFrames(animFrames);
        animation->setDelayPerUnit(delay);
        return animation;
    }
};


#endif
