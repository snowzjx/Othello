//
//  Engine.cpp
//  Othello
//
//  Created by Snow on 1/22/14.
//
//

#include "Engine.h"



void Engine::SetPlayer(Player player) {
    this->_player = player;
}

void Engine::SetOthello(std::shared_ptr<Othello> othello) {
    this->_othello = othello;
}