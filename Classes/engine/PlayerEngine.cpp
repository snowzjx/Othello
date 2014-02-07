//
//  PlayerEngine.cpp
//  Othello
//
//  Created by Snow on 1/22/14.
//
//

#include "PlayerEngine.h"

#include <thread>
#include <cstdlib>
#include "../game/Othello.h"
#include "../util/Singleton.h"
#include "../util/PlayerUtil.h"

PlayerEngine::PlayerEngine() {
    
}

PlayerEngine::~PlayerEngine() {
    
}

std::function<bool()> PlayerEngine::getNextAction() {
    this->_status = ActionResponderStatus::WAITING_FOR_CONTROL_ACTION;
    std::unique_lock<std::mutex> mLock(_mtx);
    while (!_isActionValueSet) {
        this->_condVar.wait(mLock);
    }
    this->_status = ActionResponderStatus::BUSY;
    this->_isActionValueSet = false;
    return this->_action;
}

bool PlayerEngine::getComfirmUndoValue() {
    this->_status = ActionResponderStatus::NEED_TO_ASK_FOR_USER_COMFIRM;
    std::unique_lock<std::mutex> mLock(_mtx);
    while (!_isActionValueSet) {
        this->_condVar.wait(mLock);
    }
    this->_status = ActionResponderStatus::BUSY;
    this->_isActionValueSet = false;
    return this->_isComfirmUndo;
}

bool PlayerEngine::respondToMoveAction(unsigned short posX, unsigned short posY) {
    if (!(this->_status == ActionResponderStatus::WAITING_FOR_CONTROL_ACTION)) {
        return false;
    }
    this->_action = [this, posX, posY]() -> bool {
        return Singleton<Othello>::getInstance()->getBoard()->move(this->_player, posX, posY);
    };
    this->_isActionValueSet = true;
    this->_condVar.notify_all();
    return true;
}

bool PlayerEngine::respondToUndoAction() {
    if (!(this->_status == ActionResponderStatus::WAITING_FOR_CONTROL_ACTION)) {
        return false;
    }
    this->_action = [this]() -> bool {
        auto playStack = Singleton<Othello>::getInstance()->getPlayerStack();
        if (playStack->size() > 2) {
            playStack->pop();
            if (playStack->top() == this->_player) {
                Singleton<Othello>::getInstance()->getBoard()->tackBackOneMove();
            } else {
                if (Singleton<Othello>::getInstance()->getEngine(PlayerUtil::swapPlayer(this->_player))->getComfirmUndoValue()) {
                    short count = 0;
                    while (playStack->top() != this->_player) {
                        playStack->pop();
                        count++;
                    }
                    for (short i = 0; i <= count; i++) {
                        Singleton<Othello>::getInstance()->getBoard()->tackBackOneMove();
                    }
                } else {
                    playStack->push(this->_player);
                }
            }
        }
        return false;
    };
    this->_isActionValueSet = true;
    this->_condVar.notify_all();
    return true;
}

bool PlayerEngine::respondToUndoComfirmAction(bool value) {
    if (!(this->_status == ActionResponderStatus::WAITING_FOR_USER_COMFIRM_ACTION)) {
        return false;
    }
    this->_isComfirmUndo = value;
    this->_isActionValueSet = true;
    this->_condVar.notify_all();
    return true;
}

void PlayerEngine::start() {
	this->_isActionValueSet = false;
}

void PlayerEngine::stop() {
	this->_isActionValueSet = true;
	this->_action = []()->bool {return false;};
	this->_isComfirmUndo = false;
	this->_condVar.notify_all();
}