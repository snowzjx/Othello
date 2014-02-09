//
//  PlayerScore.h
//  Othello
//
//  Created by Snow on 2/7/14.
//
//

#ifndef _PLAYER_SCORE_MAP_H_
#define _PLAYER_SCORE_MAP_H_

#include "Player.h"

class PlayerScoreMap final {
private:
    unsigned short blackPlayerScore;
    unsigned short whitePlayerScore;
public:
    PlayerScoreMap(const PlayerScoreMap &playerScore);
    PlayerScoreMap(unsigned short blackPlayerScore, unsigned short whitePlayerScore);
    unsigned short getScore(Player player);
    unsigned short& operator[](Player player);
};

#endif
