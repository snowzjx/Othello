//
//  Board.h
//  Othello
//
//  Created by Snow on 1/18/14.
//
//

#ifndef _BOARD_H_
#define _BOARD_H_

#include <vector>
#include <map>
#include <stack>
#include <memory>
#include "Matrix.h"
#include "Player.h"
#include "PlayerScore.h"

static const unsigned short BOARD_WIDTH = 8;
static const unsigned short BOARD_HEIGHT = 8;
static const std::vector<std::pair<short, short>> DIRECTIONS = {{-1,0},{1,0},{0,-1},{0,1},{-1,-1},{1,1},{-1,1},{1,-1}};
static const unsigned short NO_PLAYER = 0;

class Board final {
private:
    std::stack<std::shared_ptr<Matrix<short>>> _boardStateStack;
    std::stack<PlayerScore> _playerScoreStack;
public:
    Board();
    std::shared_ptr<Matrix<short>> getBoardState();
    bool tackBackOneMove();
    bool move(Player player, unsigned short x, unsigned short y);
    const PlayerScore getPlayerScore();
};

#endif
