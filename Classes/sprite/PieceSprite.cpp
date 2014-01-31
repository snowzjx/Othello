//
//  PieceSprite.cpp
//  Othello
//
//  Created by Snow on 1/29/14.
//
//

#include "PieceSprite.h"

#include <string>
#include "../util/AnimationUtil.h"

USING_NS_CC;

PieceSprite* PieceSprite::createWithPieceSpriteStatus(PieceSpriteStatus status) {
    std::string frameName;
    switch (status) {
        case PieceSpriteStatus::BlackPiece:
            frameName = "Piece_0.png";
            break;
        case PieceSpriteStatus::WhitePiece:
            frameName = "Piece_7.png";
            break;
    }
    PieceSprite *pieceSprite = new PieceSprite();
    if (pieceSprite && pieceSprite->initWithSpriteFrameName(frameName)) {
        pieceSprite->autorelease();
        pieceSprite->_status = status;
        pieceSprite->initAnimation();
        return pieceSprite;
    }
    CC_SAFE_DELETE(pieceSprite);
    return NULL;
}

void PieceSprite::initAnimation() {
    this->_animFromBlackToWhite = AnimationCache::getInstance()->getAnimation("animFromBlackToWhite");
    if (this->_animFromBlackToWhite == nullptr) {
        this->_animFromBlackToWhite = AnimationUtil::animationFromPlist("fromBlackToWhite.plist", 0.02);
        AnimationCache::getInstance()->addAnimation(this->_animFromBlackToWhite, "animFromBlackToWhite");
    }
    this->_animFromWhiteToBlack = AnimationCache::getInstance()->getAnimation("animFromWhiteToBlack");
    if (this->_animFromWhiteToBlack == nullptr) {
        this->_animFromWhiteToBlack = AnimationUtil::animationFromPlist("fromWhiteToBlack.plist", 0.02);
        AnimationCache::getInstance()->addAnimation(this->_animFromWhiteToBlack, "animFromWhiteToBlack");
    }
}

void PieceSprite::setStatus(PieceSpriteStatus targetStatus) {
    if (targetStatus == PieceSpriteStatus::BlackPiece && this->_status == PieceSpriteStatus::WhitePiece) {
        auto animate = Animate::create(this->_animFromWhiteToBlack);
        this->runAction(animate);
    }
    if (targetStatus == PieceSpriteStatus::WhitePiece && this->_status == PieceSpriteStatus::BlackPiece) {
        auto animate = Animate::create(this->_animFromBlackToWhite);
        this->runAction(animate);
    }
    this->_status = targetStatus;
}