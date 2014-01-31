//
//  PieceSprite.h
//  Othello
//
//  Created by Snow on 1/29/14.
//
//

#ifndef _PIECE_SPRITE_H_
#define _PIECE_SPRITE_H_

#include "cocos2d.h"
#include "PieceSpriteStatus.h"

class PieceSprite: public cocos2d::Sprite {
private:
    PieceSpriteStatus _status;
    cocos2d::Animation *_animFromBlackToWhite = nullptr;
    cocos2d::Animation *_animFromWhiteToBlack = nullptr;
public:
    static PieceSprite* createWithPieceSpriteStatus(PieceSpriteStatus status);
    void initAnimation();
    void setStatus(PieceSpriteStatus targetStatus);
};

#endif
