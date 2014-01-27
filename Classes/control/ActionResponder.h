//
//  TouchResponder.h
//  Othello
//
//  Created by Snow on 1/24/14.
//
//

#ifndef _ACTION_RESPONDER_H_
#define _ACTION_RESPONDER_H_

#include <atomic>
#include "ActionResponderStatus.h"

class ActionResponder {
protected:
    std::atomic<ActionResponderStatus> _status;
public:
    ActionResponder() {
        this->_status = ActionResponderStatus::BUSY;
    }
    ActionResponderStatus getStatus() {
        return this->_status;
    }
    void setStatus(ActionResponderStatus status) {
        this->_status = status;
    }
    virtual bool respondToMoveAction(unsigned short posX, unsigned short posY) = 0;
    virtual bool respondToUndoAction() = 0;
    virtual bool respondToUndoComfirmAction(bool value) = 0;
};

#endif
