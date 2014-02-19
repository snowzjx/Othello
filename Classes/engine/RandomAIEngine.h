//
//  AIEngine.h
//  Othello
//
//  Created by Snow on 1/22/14.
//
//

#ifndef _RANDOM_AI_EIGINE_H_
#define _RANDOM_AI_EIGINE_H_

#include "Engine.h"

#include <random>

class RandomAIEngine final: public Engine {
public:
    RandomAIEngine();
    ~RandomAIEngine();
    virtual std::function<bool()> getNextAction() override;
    virtual bool getComfirmUndoValue() override;
	virtual void start() override;
	virtual void stop() override;

private:
	std::default_random_engine _generator;
};

#endif
