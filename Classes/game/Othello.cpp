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
    this->_showMoveTip = false;
    this->_othelloThread = nullptr;
}

void Othello::setEngine(Player player, std::shared_ptr<Engine> enginePtr) {
    enginePtr->setPlayer(player);
    this->_playerEngineMap[player] = enginePtr;
}

const std::shared_ptr<Engine> Othello::getEngine(Player player) {
    return this->_playerEngineMap[player];
}

const std::shared_ptr<Board> Othello::getBoard() {
    return this->_board;
}

const Player Othello::getCurrentPlayer() {
    return this->_currentPlayer;
}

void Othello::setShouldShowMoveTip() {
    this->_showMoveTip = true;
}

bool Othello::getShouldShowMoveTip() {
    return this->_showMoveTip;
}

bool Othello::getIsGameRun() {
    return this->_isGameShouldRun;
}

void Othello::startOthello() {
	this->_board = std::shared_ptr<Board>(new Board);
    this->_currentPlayer = Player::BlackPlayer;
	this->_playerEngineMap[Player::BlackPlayer]->start();
	this->_playerEngineMap[Player::WhitePlayer]->start();
    this->_isGameShouldRun = true;
    this->_othelloThread = new std::thread(&Othello::othelloThreadStart, this);
}

void Othello::endOthello() {
	this->_isGameShouldRun = false;
    this->_playerEngineMap[Player::BlackPlayer]->stop();
    this->_playerEngineMap[Player::WhitePlayer]->stop();
}

void Othello::nextPlayer() {
    this->_currentPlayer = PlayerUtil::swapPlayer(this->getCurrentPlayer());
}

void Othello::othelloThreadStart() {
    while (_isGameShouldRun) {
        auto currentPlayer = this->getCurrentPlayer();
        if (this->getBoard()->getAvailPos(currentPlayer).size() == 0) {
            Player enemyPlayer = PlayerUtil::swapPlayer(currentPlayer);
            if (this->getBoard()->getAvailPos(enemyPlayer).size() == 0) {
                this->endOthello();
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
}

Othello::~Othello() {
    this->endOthello();
    if (this->_othelloThread && this->_othelloThread->joinable()) {
		this->_othelloThread->join();
        delete _othelloThread;
	}
}


