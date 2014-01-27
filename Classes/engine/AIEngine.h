//
//  AIEngine.h
//  Othello
//
//  Created by Snow on 1/22/14.
//
//

#ifndef _AI_EIGINE_H_
#define _AI_EIGINE_H_

#include "Engine.h"

class AIEngine: public Engine {
public:
    AIEngine();    
    virtual std::function<bool()> getNextAction() override;
    virtual bool getComfirmUndoValue() override;
};

#endif
