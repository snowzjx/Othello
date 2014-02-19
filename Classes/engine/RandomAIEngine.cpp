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

#include "../util/Singleton.h"
#include "../game/Othello.h"

RandomAIEngine::RandomAIEngine() {

}

RandomAIEngine::~RandomAIEngine() {

}

std::function<bool()> RandomAIEngine::getNextAction() {
    return [this]() -> bool {
        auto availPos = Singleton<Othello>::getInstance()->getBoard()->getAvailPos(this->_player);
        int size = availPos.size();
		auto dist = std::uniform_int_distribution<int>(0, size - 1);
		int index = dist(this->_generator);
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
	this->_generator = std::default_random_engine(time(0));
}

void RandomAIEngine::stop() {
	return;
}