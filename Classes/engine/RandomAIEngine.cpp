//
//  AIEngine.cpp
//  Othello
//
//  Created by Snow on 1/22/14.
//
//

#include "RandomAIEngine.h"

#include <cstdlib>
#include <time.h>
#include "../game/Othello.h"
#include "../util/Singleton.h"

RandomAIEngine::RandomAIEngine() {
    
}

RandomAIEngine::~RandomAIEngine() {

}

std::function<bool()> RandomAIEngine::getNextAction() {
    return [&]() -> bool {
        auto availPos = Singleton<Othello>::getInstance()->getBoard()->getAvailPos(this->_player);
        int size = availPos.size();
        short index = rand() % size;
        auto pos = availPos[index];
        unsigned short posX = std::get<0>(pos);
        unsigned short posY = std::get<1>(pos);
        return Singleton<Othello>::getInstance()->getBoard()->move(this->_player, posX, posY);
    };
}

bool RandomAIEngine::getComfirmUndoValue() {
    return true;
}

void RandomAIEngine::start() {
	srand(time(0));
}

void RandomAIEngine::stop() {
	return;
}