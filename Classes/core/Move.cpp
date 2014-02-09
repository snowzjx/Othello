//
//  Move.cpp
//  Othello
//
//  Created by Snow on 2/9/14.
//
//

#include "Move.h"

Move::Move(Player player, unsigned short x, unsigned short y) {
    this->player = player;
    this->x = x;
    this->y = y;
}