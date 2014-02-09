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
    this->_playerScoreMapStack.push(PlayerScoreMap(2, 2));
}

Board::~Board() {
    
}

std::shared_ptr<Matrix<short>> Board::getBoardState() {
    return this->_boardStateStack.top();
}

bool Board::tackBackOneMove() {
    if (this->_boardStateStack.size() > 1) {
        this->_boardStateStack.pop();
        this->_playerScoreMapStack.pop();
        this->_moveStack.pop();
        this->_cachedAvailPosMap.clear();
        return true;
    } else {
        return false;
    }
}

bool Board::canTackBackOneMove() {
    return this->_moveStack.size() > 0;
}

bool Board::move(Player player, unsigned short x, unsigned short y) {
    return this->move(Move(player, x, y));
}

bool Board::move(Move move) {
    auto x = move.x;
    auto y = move.y;
    auto player = move.player;
    if (x < 0 || y < 0 || x > BOARD_WIDTH || y > BOARD_HEIGHT || (*this->getBoardState())[x][y] != Player::NoPlayer) {
        return false;
    }
    std::vector<std::pair<short, short>> searchDirections;
    for(auto itr = DIRECTIONS.begin(); itr != DIRECTIONS.end(); ++itr) {
        auto direction = *itr;
        short searchX = x + std::get<0>(direction);
        short searchY = y + std::get<1>(direction);
        if (searchX >=0 && searchX <= BOARD_WIDTH - 1 && searchY >=0 && searchY <= BOARD_HEIGHT - 1 && (*this->getBoardState())[searchX][searchY] == -player) {
            searchDirections.push_back(*itr);
        }
    }
    if(searchDirections.size() == 0) {
        return false;
    }
    std::shared_ptr<Matrix<short>> boardState = this->_boardStateStack.top()->clone();
    std::vector<std::pair<unsigned short, unsigned short>> path;
    PlayerScoreMap playerScore(this->getPlayerScoreMap());
    bool moveValid = false;
    for (auto itr = std::begin(searchDirections); itr != std::end(searchDirections); ++itr) {
        auto direction = *itr;
        short searchX = x + std::get<0>(direction);
        short searchY = y + std::get<1>(direction);
        while (searchX >=0 && searchX <= BOARD_WIDTH - 1 && searchY >=0 && searchY <= BOARD_HEIGHT - 1){
            if ((*boardState)[searchX][searchY] == player) {
                playerScore[player] += path.size();
                playerScore[PlayerUtil::swapPlayer(player)] -= path.size();
                for (auto pathItr = std::begin(path); pathItr != std::end(path); ++pathItr) {
                    (*boardState)[std::get<0>(*pathItr)][std::get<1>(*pathItr)] = player;
                }
                moveValid = true;
                break;
            } else {
                path.push_back({searchX, searchY});
            }
            if ((*boardState)[searchX][searchY] == Player::NoPlayer) {
                break;
            }
            searchX += std::get<0>(direction);
            searchY += std::get<1>(direction);
        }
        path.clear();
    }
    if (moveValid) {
        (*boardState)[x][y] = player;
        playerScore[player] ++;
		this->_boardStateStack.push(boardState);
        this->_playerScoreMapStack.push(playerScore);
        this->_moveStack.push(move);
        this->_cachedAvailPosMap.clear();
    }
    return moveValid;
}

Move Board::getPreviousMove() {
    return this->_moveStack.top();
}

std::vector<std::pair<unsigned short, unsigned short>> Board::getAvailPos(Player player) {
    auto itr = this->_cachedAvailPosMap.find(player);
    if (itr != this->_cachedAvailPosMap.end()) {
        return itr->second;
    }
    std::vector<std::pair<unsigned short, unsigned short>> availPos;
    std::vector<std::pair<unsigned short, unsigned short>> enemyPos;
    auto boardStatus = *this->getBoardState();
    for (short i = 0; i < BOARD_WIDTH; i++) {
        for (short j = 0; j < BOARD_HEIGHT; j++) {
            if (boardStatus[i][j] == -player) {
                enemyPos.push_back({i, j});
            }
        }
    }
    if (enemyPos.size() != 0) {
        for (auto enemyPosItr = enemyPos.begin(); enemyPosItr != enemyPos.end(); enemyPosItr++) {
            for (auto directionItr = DIRECTIONS.begin(); directionItr != DIRECTIONS.end(); directionItr++) {
                short posX = std::get<0>(*enemyPosItr) + std::get<0>(*directionItr);
                short posY = std::get<1>(*enemyPosItr) + std::get<1>(*directionItr);
                if (posX >= 0 && posX <= BOARD_WIDTH - 1 && posY >= 0 && posY <= BOARD_HEIGHT - 1 && boardStatus[posX][posY] == Player::NoPlayer) {
                    short searchX = posX - std::get<0>(*directionItr);
                    short searchY = posY - std::get<1>(*directionItr);
                    while (searchX >= 0 && searchX <= BOARD_WIDTH - 1 && searchY >= 0 && searchY <= BOARD_HEIGHT - 1) {
                        if (boardStatus[searchX][searchY] == player) {
                            availPos.push_back({posX, posY});
                        }
                        if (boardStatus[searchX][searchY] == Player::NoPlayer) {
                            break;
                        }
                        searchX -= std::get<0>(*directionItr);
                        searchY -= std::get<1>(*directionItr);
                    }
                }
            }
        }
    }
    this->_cachedAvailPosMap[player] = availPos;
    return availPos;
}

const PlayerScoreMap Board::getPlayerScoreMap() {
    return this->_playerScoreMapStack.top();
}