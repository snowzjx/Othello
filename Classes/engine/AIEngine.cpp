//
//  AIEngine.cpp
//  Othello
//
//  Created by Snow on 1/22/14.
//
//

#include "AIEngine.h"

#include <cstdlib>
#include <time.h>
#include "../game/Othello.h"

AIEngine::AIEngine() {
    srand(time(0));
}

std::function<bool()> AIEngine::getNextAction() {
    return [&]() -> bool {
        auto availPos = this->_othello.lock()->getPlayerAvailPos(this->_player);
        int size = availPos.size();
        short index = rand() % size;
        auto pos = availPos[index];
        unsigned short posX = std::get<0>(pos);
        unsigned short posY = std::get<1>(pos);
        return this->_othello.lock()->getBoard()->move(this->_player, posX, posY);
    };
}

bool AIEngine::getComfirmUndoValue() {
    return true;
}