//
//  OthelloManager.cpp
//  Othello
//
//  Created by Snow on 1/19/14.
//
//

#include "Othello.h"

#include <thread>
#include "../util/PlayerUtil.h"

Othello::Othello() {
	this->_isGameShouldRun = false;
    this->_playStack = std::shared_ptr<std::stack<Player>>(new std::stack<Player>());
}

void Othello::setDelegate(std::shared_ptr<OthelloDelegate> delegate) {
	this->_delegate = delegate;
}

void Othello::setEngine(Player player, std::shared_ptr<Engine> enginePtr) {
    enginePtr->setPlayer(player);
    enginePtr->setOthello(this->shared_from_this());
    this->_playerEngineMap[player] = enginePtr;
}

const std::shared_ptr<Engine> Othello::getEngine(Player player) {
    return this->_playerEngineMap[player];
}

const std::shared_ptr<Board> Othello::getBoard() {
    return this->_board;
}

const std::vector<std::pair<unsigned short, unsigned short>>& Othello::getPlayerAvailPos(Player player) {
    return this->_playAvailPosMap[player];
}

const Player Othello::getCurrentPlayer() {
    return this->_playStack->top();
}

const std::shared_ptr<std::stack<Player>> Othello::getPlayerStack() {
    return this->_playStack;
}

void Othello::startOthello() {
	this->_board = std::shared_ptr<Board>(new Board);
    this->_playStack->push(Player::BlackPlayer);
	this->_playerEngineMap[Player::BlackPlayer]->start();
	this->_playerEngineMap[Player::WhitePlayer]->start();
    this->_isGameShouldRun = true;
    this->_othelloThread = std::thread(&Othello::othelloThreadStart, this);
}

void Othello::endOthello() {
    this->_isGameShouldRun = false;
	this->_playerEngineMap[Player::BlackPlayer]->stop();
	this->_playerEngineMap[Player::WhitePlayer]->stop();
	this->_othelloThread.join();
}

void Othello::updateAvailPos(Player player) {
    std::vector<std::pair<unsigned short, unsigned short>> availPos;
    std::vector<std::pair<unsigned short, unsigned short>> enemyPos;
    Matrix<short> boardStatus = *(this->_board->getBoardState());
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
                if (posX >= 0 && posX <= BOARD_WIDTH - 1 && posY >= 0 && posY <= BOARD_HEIGHT - 1 && boardStatus[posX][posY] == NO_PLAYER) {
                    short searchX = posX;
                    short searchY = posY;
                    while (searchX >= 0 && searchX <= BOARD_WIDTH - 1 && searchY >= 0 && searchY <= BOARD_HEIGHT - 1) {
                        searchX -= std::get<0>(*directionItr);
                        searchY -= std::get<1>(*directionItr);
                        if (boardStatus[searchX][searchY] == player) {
                            availPos.push_back({posX, posY});
                        }
                        if (boardStatus[searchX][searchY] == NO_PLAYER) {
                            break;
                        }
                    }
                }
            }
        }
    }
    this->_playAvailPosMap[player] = availPos;
}

void Othello::nextPlayer() {
    this->_playStack->push(PlayerUtil::swapPlayer(this->getCurrentPlayer()));
}

void Othello::othelloThreadStart() {
    while (_isGameShouldRun) {
        auto currentPlayer = this->getCurrentPlayer();
        this->updateAvailPos(currentPlayer);
        if (this->_playAvailPosMap[currentPlayer].size() == 0) {
            Player enemyPlayer = PlayerUtil::swapPlayer(currentPlayer);
            this->updateAvailPos(enemyPlayer);
            if (this->_playAvailPosMap[enemyPlayer].size() == 0) {
                break;
            } else {
                this->nextPlayer();
            }
        } else {
            auto action = this->_playerEngineMap[currentPlayer]->getNextAction();
            if (action()) {
                this->nextPlayer();
            }
        }
    }
	if (this->_delegate.lock()) {
		this->_delegate.lock()->othelloGameDidFinish();
	}
}

Othello::~Othello() {
	this->endOthello();
}


