//
//  PlayerEngine.h
//  Othello
//
//  Created by Snow on 1/22/14.
//
//

#ifndef _PLAYER_ENGINE_H_
#define _PLAYER_ENGINE_H_

#include "Engine.h"

#include <atomic>
#include <mutex>
#include <condition_variable>
#include "../control/ActionResponder.h"

class PlayerEngine: public Engine, public ActionResponder {
private:
    std::atomic_bool _isActionValueSet;
    std::mutex _mtx;
    std::condition_variable _condVar;
    std::function<bool()> _action;
    bool _isComfirmUndo;
    
public:
    PlayerEngine();
    virtual std::function<bool()> getNextAction() override;
    virtual bool getComfirmUndoValue() override;
    virtual bool respondToMoveAction(unsigned short posX, unsigned short posY) override;
    virtual bool respondToUndoAction() override;
    virtual bool respondToUndoComfirmAction(bool value) override;
};

#endif
