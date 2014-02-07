//
//  Engine.h
//  Othello
//
//  Created by Snow on 1/22/14.
//
//

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <memory>
#include <functional>
#include "../core/Board.h"
#include "../core/Player.h"

class Othello;

class Engine {
protected:
    Player _player;
public:
    void setPlayer(Player player);
    virtual std::function<bool()> getNextAction() = 0;
    virtual bool getComfirmUndoValue() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
};

#endif