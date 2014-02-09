//
//  Move.h
//  Othello
//
//  Created by Snow on 2/9/14.
//
//

#ifndef _MOVE_H_
#define _MOVE_H_

#include "Player.h"

class Move {
public:
    Player player;
    unsigned short x;
    unsigned short y;
    Move(Player player, unsigned short x, unsigned short y);
};


#endif
