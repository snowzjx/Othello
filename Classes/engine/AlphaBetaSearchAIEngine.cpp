//
//  AlphaBetaSearchAIEngine.cpp
//  Othello
//
//  Created by Snow on 2/10/14.
//
//

#include "AlphaBetaSearchAIEngine.h"

#include <vector>
#include <set>
#include <cstdlib>
#include "../util/Singleton.h"
#include "../util/PlayerUtil.h"
#include "../game/Othello.h"

const static std::set<std::pair<unsigned short, unsigned short>> CORNERS = {{0, 0}, {0, BOARD_HEIGHT - 1}, {BOARD_WIDTH - 1, 0}, {BOARD_WIDTH - 1, BOARD_HEIGHT - 1}};
const static int k_MOVES_WEIGHT = 5;
const static int k_FRONTIERS_SQUARES_WEIGHT = 1;
const static int k_CORNER_WEIGHT = 200;
const static int k_STABLE_PIECE_WEIGHT = 10;
const static int k_X_SQUARE_WEIGHT = 200;
const static int k_C_SQUARE_WEIGHT = 150;
const static int SEARCH_MAX_DEPTH = 4;
const static int POSITIVE_INFINITY = 1000000;
const static int NEGATIVE_INFINITY = -1000000;

AlphaBetaSearchAIEngine::AlphaBetaSearchAIEngine() {
    
}

AlphaBetaSearchAIEngine::~AlphaBetaSearchAIEngine() {
    
}

std::function<bool()> AlphaBetaSearchAIEngine::getNextAction() {
    auto board = Singleton<Othello>::getInstance()->getBoard();
    auto aiBoard = new Board(*board);
    auto move = this->alphaBetaSearch(this->_player, *aiBoard);
    return [move]()->bool {
        return Singleton<Othello>::getInstance()->getBoard()->move(move);
    };
}

Move AlphaBetaSearchAIEngine::alphaBetaSearch(Player player, Board &board) {
    this->alphaBetaSearchDriver(player, board, NEGATIVE_INFINITY, POSITIVE_INFINITY, SEARCH_MAX_DEPTH);
    return this->_bestMove;
}

int AlphaBetaSearchAIEngine::alphaBetaSearchDriver(Player player, Board &board, int alpha, int beta, int depth) {
    if (depth <= 0) {
        return AlphaBetaSearchAIEngine::getEvalValue(player, board) - AlphaBetaSearchAIEngine::getEvalValue(PlayerUtil::swapPlayer(player), board);
    }
    auto availPos = board.getAvailPos(player);
    if (availPos.size() == 0) {
        return AlphaBetaSearchAIEngine::getEvalValue(player, board) - AlphaBetaSearchAIEngine::getEvalValue(PlayerUtil::swapPlayer(player), board);
    }
    for (auto itr = std::begin(availPos); itr != std::end(availPos); ++itr) {
        Move move(player, std::get<0>(*itr), std::get<1>(*itr));
        board.move(move);
        int evalValue = - this->alphaBetaSearchDriver(PlayerUtil::swapPlayer(player), board, -beta, -alpha, depth - 1);
        board.tackBackOneMove();
        if (evalValue >= beta) {
            return beta;
        }
        if (evalValue > alpha) {
            alpha = evalValue;
            if (depth == SEARCH_MAX_DEPTH) {
                this->_bestMove = move;
            }
        }
    }
    return alpha;
}

bool AlphaBetaSearchAIEngine::getComfirmUndoValue() {
    return true;
}

void AlphaBetaSearchAIEngine::start() {

}

void AlphaBetaSearchAIEngine:: stop() {
    
}

int AlphaBetaSearchAIEngine::getMobility(Player player, Board &board) {
    int moveCount =  board.getAvailPos(player).size();
    std::vector<std::pair<unsigned short, unsigned short>> playerPosVector;
    for (auto i = 0; i < BOARD_WIDTH; i++) {
        for (auto j = 0; j < BOARD_HEIGHT; j++) {
            if ((*board.getBoardState())[i][j] == player) {
                playerPosVector.push_back({i, j});
            }
        }
    }
    int frontierSuqareCount = 0;
    for (auto itr = std::begin(playerPosVector); itr != std::end(playerPosVector); ++itr) {
        for (auto dItr = std::begin(DIRECTIONS); dItr != std::end(DIRECTIONS); ++dItr) {
            auto x = std::get<0>(*itr) + std::get<0>(*dItr);
            auto y = std::get<1>(*itr) + std::get<1>(*dItr);
            if (x > 0 && x < BOARD_WIDTH && y > 0 && y < BOARD_HEIGHT && (*board.getBoardState())[x][y] == player) {
                ++ frontierSuqareCount;
            }
        }
    }
    return moveCount * k_MOVES_WEIGHT - frontierSuqareCount * k_FRONTIERS_SQUARES_WEIGHT;
}

int AlphaBetaSearchAIEngine::getStability(Player player, Board &board) {
    int cornerCount = 0;
    int stableCount = 0;
    int negXSquareCount = 0;
    int negCSquareCount = 0;
    for (auto itr = std::begin(CORNERS); itr != std::end(CORNERS); ++itr) {
        auto x = std::get<0>(*itr);
        auto y = std::get<0>(*itr);
        if ((*board.getBoardState())[x][y] == player) {
            ++ cornerCount;
            std::set<std::pair<unsigned short, unsigned short>> searchDirection;
            searchDirection.insert({0, (BOARD_HEIGHT / 2 - y) / abs(BOARD_HEIGHT / 2 - y)});
            searchDirection.insert({(BOARD_WIDTH / 2 - x) / abs(BOARD_WIDTH / 2 - x), 0});
            for (auto searchDItr = std::begin(searchDirection); searchDItr != std::end(searchDirection); ++searchDItr) {
                auto searchX = x + std::get<0>(*searchDItr);
                auto searchY = y + std::get<1>(*searchDItr);
                while (searchX > 0 && searchX < BOARD_WIDTH && searchY > 0 && searchY < BOARD_HEIGHT && (*board.getBoardState())[searchX][searchY] == player) {
                    ++ stableCount;
                    searchX += std::get<0>(*searchDItr);
                    searchY += std::get<1>(*searchDItr);
                }
            }
        } else {
            auto searchX = x - ((BOARD_WIDTH / 2 - x) / abs(BOARD_WIDTH / 2 - x));
            auto searchY = y - ((BOARD_HEIGHT / 2 - y) / abs(BOARD_HEIGHT / 2 - y));
            if ((*board.getBoardState())[searchX][searchY] == player) {
                ++ negXSquareCount;
            }
            searchX = x;
            searchY = y - ((BOARD_HEIGHT / 2 - y) / abs(BOARD_HEIGHT / 2 - y));
            if ((*board.getBoardState())[searchX][searchY] == player) {
                ++ negCSquareCount;
            }
            searchX = x - ((BOARD_WIDTH / 2 - x) / abs(BOARD_WIDTH / 2 - x));
            searchY = y;
            if ((*board.getBoardState())[searchX][searchY] == player) {
                ++ negCSquareCount;
            }
        }
    }
    return cornerCount * k_CORNER_WEIGHT + stableCount * k_STABLE_PIECE_WEIGHT - negXSquareCount * k_X_SQUARE_WEIGHT - negCSquareCount * k_C_SQUARE_WEIGHT;
}

int AlphaBetaSearchAIEngine::getEvalValue(Player player, Board &board) {
    return AlphaBetaSearchAIEngine::getMobility(player, board) + AlphaBetaSearchAIEngine::getStability(player, board);
}