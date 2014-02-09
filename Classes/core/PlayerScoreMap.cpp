//
//  PlayerScoreMap.cpp
//  Othello
//
//  Created by Snow on 2/9/14.
//
//

#include "PlayerScoreMap.h"

#include <assert.h>

PlayerScoreMap::PlayerScoreMap(const PlayerScoreMap &playerScore) {
    this->blackPlayerScore = playerScore.blackPlayerScore;
    this->whitePlayerScore = playerScore.whitePlayerScore;
}
PlayerScoreMap::PlayerScoreMap(unsigned short blackPlayerScore, unsigned short whitePlayerScore) {
    this->blackPlayerScore = blackPlayerScore;
    this->whitePlayerScore = whitePlayerScore;
}
unsigned short PlayerScoreMap::getScore(Player player) {
    assert(player != Player::NoPlayer);
    if (player == Player::BlackPlayer) {
        return blackPlayerScore;
    } else {
        return whitePlayerScore;
    }
}
unsigned short& PlayerScoreMap::operator[](Player player) {
    assert(player != Player::NoPlayer);
    if (player == Player::BlackPlayer) {
        return blackPlayerScore;
    } else {
        return whitePlayerScore;
    }
}
