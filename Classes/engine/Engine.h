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
    std::weak_ptr<Othello> _othello;
public:
    void SetPlayer(Player player);
    void SetOthello(std::shared_ptr<Othello> othello);
    virtual std::function<bool()> getNextAction() = 0;
    virtual bool getComfirmUndoValue() = 0;
};

#endif