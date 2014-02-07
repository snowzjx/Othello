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
#include "../util/Singleton.h"

AIEngine::AIEngine() {
    
}

AIEngine::~AIEngine() {

}

std::function<bool()> AIEngine::getNextAction() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return [&]() -> bool {
        auto availPos = Singleton<Othello>::getInstance()->getPlayerAvailPos(this->_player);
        int size = availPos.size();
        short index = rand() % size;
        auto pos = availPos[index];
        unsigned short posX = std::get<0>(pos);
        unsigned short posY = std::get<1>(pos);
        return Singleton<Othello>::getInstance()->getBoard()->move(this->_player, posX, posY);
    };
}

bool AIEngine::getComfirmUndoValue() {
    return true;
}

void AIEngine::start() {
	srand(time(0));
}

void AIEngine::stop() {
	return;
}