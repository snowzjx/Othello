//
//  PlayerScore.h
//  Othello
//
//  Created by Snow on 2/7/14.
//
//

#ifndef _PLAYER_SCORE_INFO_H_
#define _PLAYER_SCORE_INFO_H_

#include "Player.h"

class PlayerScoreInfo final {
private:
    unsigned short blackPlayerScore;
    unsigned short whitePlayerScore;
public:
    PlayerScoreInfo(const PlayerScoreInfo &playerScore);
    PlayerScoreInfo(unsigned short blackPlayerScore, unsigned short whitePlayerScore);
    unsigned short getScore(Player player);
    unsigned short& operator[](Player player);
};

#endif
