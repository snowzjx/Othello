//
//  PointUtil.h
//  Othello
//
//  Created by Snow on 1/29/14.
//
//

#ifndef _POINT_UTIL_H_
#define _POINT_UTIL_H_

#include <set>
#include "cocos2d.h"
#include "../include/GraphicConfig.h"

class PointUtil {
public:
    static cocos2d::Point convertToPointFromBoard(short x, short y) {
        float px = cocos2d::Director::getInstance()->getWinSize().width / 2.0 - boardEdge / 2.0 + boardDeltaMain / 2.0 + (boardDeltaMain + boardDeltaGap) * (float)x;
        float py = cocos2d::Director::getInstance()->getWinSize().height / 2.0 - boardEdge / 2.0 + boardDeltaMain / 2.0 + (boardDeltaMain + boardDeltaGap) * (float)y;
        return cocos2d::Point(px, py);
    }
    static std::pair<short, short> convertToBoardFromPoint(cocos2d::Point point) {
        short x = (point.x - cocos2d::Director::getInstance()->getWinSize().width / 2.0 + boardEdge / 2.0) / (boardDeltaMain + boardDeltaGap);
        short y = (point.y - cocos2d::Director::getInstance()->getWinSize().height / 2.0 + boardEdge / 2.0) / (boardDeltaMain + boardDeltaGap);
        return {x, y};
    }
};


#endif
