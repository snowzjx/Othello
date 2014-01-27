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
#include "../util/PlayerUtil.h"

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
    this->_action = [&]() -> bool {
        auto availPos = this->_othello.lock()->getPlayerAvailPos(this->_player);
        int size = availPos.size();
        short index = rand() % size;
        auto pos = availPos[index];
        unsigned short _posX = std::get<0>(pos);
        unsigned short _posY = std::get<1>(pos);
        return this->_othello.lock()->getBoard()->move(this->_player, _posX, _posY);
    };
    this->_isActionValueSet = true;
    this->_condVar.notify_all();
    return true;
}

bool PlayerEngine::respondToUndoAction() {
    if (!(this->_status == ActionResponderStatus::WAITING_FOR_CONTROL_ACTION)) {
        return false;
    }
    this->_action = [&]() -> bool {
        auto playStack = this->_othello.lock()->getPlayerStack();
        if (playStack->size() > 2) {
            playStack->pop();
            if (playStack->top() == this->_player) {
                this->_othello.lock()->getBoard()->popOneBoardState();
            } else {
                if (this->_othello.lock()->getEngine(PlayerUtil::swapPlayer(this->_player))->getComfirmUndoValue()) {
                    short count = 0;
                    while (playStack->top() != this->_player) {
                        playStack->pop();
                        count++;
                    }
                    for (short i = 0; i <= count; i++) {
                        this->_othello.lock()->getBoard()->popOneBoardState();
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