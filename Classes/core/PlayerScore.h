//
//  PlayerScore.h
//  Othello
//
//  Created by Snow on 2/7/14.
//
//

#ifndef _PLAYER_SCORE_H_
#define _PLAYER_SCORE_H_

#include "Player.h"

struct PlayerScore final {
public:
    unsigned short blackPlayerScore;
    unsigned short whitePlayerScore;
    PlayerScore(const PlayerScore &playerScore) {
        this->blackPlayerScore = playerScore.blackPlayerScore;
        this->whitePlayerScore = playerScore.whitePlayerScore;
    }
    PlayerScore(unsigned short blackPlayerScore, unsigned short whitePlayerScore) {
        this->blackPlayerScore = blackPlayerScore;
        this->whitePlayerScore = whitePlayerScore;
    }
    unsigned short getScore(Player player) {
        switch(player) {
            case BlackPlayer:
                return blackPlayerScore;
            case WhitePlayer:
                return whitePlayerScore;
        }
    }
    unsigned short& operator[](Player player) {
        switch(player) {
            case BlackPlayer:
                return blackPlayerScore;
            case WhitePlayer:
                return whitePlayerScore;
        }
    }
};

#endif
