//
//  PlayerUtil.h
//  Othello
//
//  Created by Snow on 1/22/14.
//
//

#ifndef _PLAYER_UTIL_H_
#define _PLAYER_UTIL_H_

#include "../core/Player.h"

class PlayerUtil {
public:
    static Player swapPlayer(Player player) {
        return player == Player::BlackPlayer? WhitePlayer: BlackPlayer;
    }
};

#endif
