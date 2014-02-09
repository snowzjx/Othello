//
//  PlayerScoreMap.cpp
//  Othello
//
//  Created by Snow on 2/9/14.
//
//

#include "PlayerScoreInfo.h"

#include <assert.h>

PlayerScoreInfo::PlayerScoreInfo(const PlayerScoreInfo &playerScore) {
    this->blackPlayerScore = playerScore.blackPlayerScore;
    this->whitePlayerScore = playerScore.whitePlayerScore;
}
PlayerScoreInfo::PlayerScoreInfo(unsigned short blackPlayerScore, unsigned short whitePlayerScore) {
    this->blackPlayerScore = blackPlayerScore;
    this->whitePlayerScore = whitePlayerScore;
}
unsigned short PlayerScoreInfo::getScore(Player player) {
    assert(player != Player::NoPlayer);
    if (player == Player::BlackPlayer) {
        return blackPlayerScore;
    } else {
        return whitePlayerScore;
    }
}
unsigned short& PlayerScoreInfo::operator[](Player player) {
    assert(player != Player::NoPlayer);
    if (player == Player::BlackPlayer) {
        return blackPlayerScore;
    } else {
        return whitePlayerScore;
    }
}
