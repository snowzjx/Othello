//
//  Board.cpp
//  Othello
//
//  Created by Snow on 1/18/14.
//
//

#include "Board.h"

#include "../util/PlayerUtil.h"

Board::Board() {
    auto boardState = std::shared_ptr<Matrix<short>>(new Matrix<short>(BOARD_WIDTH, BOARD_HEIGHT));
    (*boardState)[BOARD_WIDTH / 2][BOARD_HEIGHT / 2] = Player::BlackPlayer;
    (*boardState)[BOARD_WIDTH / 2 - 1][BOARD_HEIGHT / 2 - 1] = Player::BlackPlayer;
    (*boardState)[BOARD_WIDTH / 2][BOARD_HEIGHT / 2 - 1] = Player::WhitePlayer;
    (*boardState)[BOARD_WIDTH / 2 - 1][BOARD_HEIGHT / 2] = Player::WhitePlayer;
    this->_boardStateStack.push(boardState);
    this->_playerScoreMap[Player::BlackPlayer] = 2;
    this->_playerScoreMap[Player::WhitePlayer] = 2;
}

std::shared_ptr<Matrix<short>> Board::getBoardState() {
    return this->_boardStateStack.top();
}

bool Board::popOneBoardState() {
    auto boardState = this->_boardStateStack.top();
    this->_boardStateStack.pop();
    if (this->_boardStateStack.empty()) {
        this->_boardStateStack.push(boardState);
        return false;
    } else {
        return true;
    }
}

bool Board::move(Player player, unsigned short x, unsigned short y) {
    std::shared_ptr<Matrix<short>> boardState = this->_boardStateStack.top()->clone();
    if (x < 0 || y < 0 || x > BOARD_WIDTH || y > BOARD_HEIGHT || (*boardState)[x][y] != NO_PLAYER) {
        return false;
    }
    std::vector<std::pair<short, short>> searchDirections;
    for(auto itr = DIRECTIONS.begin(); itr != DIRECTIONS.end(); ++itr) {
        auto direction = *itr;
        short searchX = x + std::get<0>(direction);
        short searchY = y + std::get<1>(direction);
        if (searchX >=0 && searchX <= BOARD_WIDTH - 1 && searchY >=0 && searchY <= BOARD_HEIGHT - 1 && (*boardState)[searchX][searchY] == -player) {
            searchDirections.push_back(*itr);
        }
    }
    if(searchDirections.size() == 0) {
        return false;
    }
    std::vector<std::pair<unsigned short, unsigned short>> path;
    bool moveValid = false;
    for (auto itr = std::begin(searchDirections); itr != std::end(searchDirections); ++itr) {
        auto direction = *itr;
        short searchX = x + std::get<0>(direction);
        short searchY = y + std::get<1>(direction);
        while (searchX >=0 && searchX <= BOARD_WIDTH - 1 && searchY >=0 && searchY <= BOARD_HEIGHT - 1){
            if ((*boardState)[searchX][searchY] == player) {
                this->_playerScoreMap[player] += path.size();
                this->_playerScoreMap[PlayerUtil::swapPlayer(player)] -= path.size();
                for (auto pathItr = std::begin(path); pathItr != std::end(path); ++pathItr) {
                    (*boardState)[std::get<0>(*pathItr)][std::get<1>(*pathItr)] = player;
                }
                moveValid = true;
                break;
            } else {
                path.push_back({searchX, searchY});
            }
            if ((*boardState)[searchX][searchY] == NO_PLAYER) {
                break;
            }
            searchX += std::get<0>(direction);
            searchY += std::get<1>(direction);
        }
        path.clear();
    }
    if (moveValid) {
        this->_playerScoreMap[player] ++;
        (*boardState)[x][y] = player;
    }
    this->_boardStateStack.push(boardState);
    return moveValid;
}

unsigned short Board::getPlayerScore(Player player) {
    return this->_playerScoreMap[player];
}