//
//  AlphaBetaSearchAIEngine.h
//  Othello
//
//  Created by Snow on 2/10/14.
//
//

#ifndef _ALPHA_BETA_SEARCH_ENGINE_H_
#define _ALPHA_BETA_SEARCH_ENGINE_H_

#include "Engine.h"
#include "../core/Board.h"
#include "../core/Move.h"

class AlphaBetaSearchAIEngine final: public Engine {
public:
    AlphaBetaSearchAIEngine();
    ~AlphaBetaSearchAIEngine();
    virtual std::function<bool()> getNextAction() override;
    virtual bool getComfirmUndoValue() override;
	virtual void start() override;
	virtual void stop() override;
    
    Move alphaBetaSearch(Player player, Board& board);
    int alphaBetaSearchDriver(Player player, Board& board, int alpha, int beta, int depth);
    static int getMobility(Player player, Board& board);
    static int getStability(Player player, Board& board);
    static int getEvalValue(Player player, Board& board);
    
private:
    Move _bestMove;
};

#endif
