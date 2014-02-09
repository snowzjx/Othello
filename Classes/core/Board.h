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
#include "Move.h"
#include "PlayerScoreInfo.h"

static const unsigned short BOARD_WIDTH = 8;
static const unsigned short BOARD_HEIGHT = 8;
static const std::vector<std::pair<short, short>> DIRECTIONS = {{-1,0},{1,0},{0,-1},{0,1},{-1,-1},{1,1},{-1,1},{1,-1}};

class Board final {
private:
    std::stack<std::shared_ptr<Matrix<short>>> _boardStateStack;
    std::stack<PlayerScoreInfo> _playerScoreInfoStack;
    std::stack<Move> _moveStack;
    std::map<Player, std::vector<std::pair<unsigned short, unsigned short>>> _cachedAvailPosMap;
public:
    
    // Constructor.
    Board();
    
    // Destructor.
    ~Board();
    
    // Get the board status matrix.
    std::shared_ptr<Matrix<short>> getBoardState();
    
    // Move.
    bool move(Move move);
    
    // Move.
    bool move(Player player, unsigned short x, unsigned short y);
    
    // Get previous move.
    Move getPreviousMove();
    
    // Tack back one move.
    bool tackBackOneMove();
    
    // Get whether the board has move to be taken back.
    bool canTackBackOneMove();
    
    // Get the available positions.
    std::vector<std::pair<unsigned short, unsigned short>> getAvailPos(Player player);
    
    // Get the player score info.
    const PlayerScoreInfo getPlayerScoreInfo();
};

#endif
