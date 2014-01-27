//
//  Engine.cpp
//  Othello
//
//  Created by Snow on 1/22/14.
//
//

#include "Engine.h"



void Engine::setPlayer(Player player) {
    this->_player = player;
}

void Engine::setOthello(std::shared_ptr<Othello> othello) {
    this->_othello = othello;
}