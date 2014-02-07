//
//  GraphicConfig.h
//  Othello
//
//  Created by Snow on 1/29/14.
//
//

#ifndef _GRAPHIC_CONFIG_H_
#define _GRAPHIC_CONFIG_H_

#include "cocos2d.h"

static const cocos2d::Color4B BACKGROUND_COLOR = cocos2d::Color4B(25, 110, 245, 255);
static const cocos2d::Color3B FOREGROUND_COLOR = cocos2d::Color3B(25, 110, 245);
static const int boardZOrder = 0;
static const float boardEdge = 240.0;
static const float boardDeltaMain = boardEdge * 135.0/ 1150.0;
static const float boardDeltaGap = boardEdge * 10.0 / 1150.0;
static const int pieceZOrder = 5;
static const int noticeZOrder = 10;
static const int popupZOrder = 15;
static const int undoPopupTag = 101;
static const int toolPopupTag = 102;
static const int pausePopupTag = 103;

#endif
