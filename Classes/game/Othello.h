//
//  OthelloManager.h
//  Othello
//
//  Created by Snow on 1/19/14.
//
//

#ifndef _OTHELLO_H_
#define _OTHELLO_H_

#include <thread>
#include <vector>
#include <stack>
#include <memory>
#include <atomic>
#include "../core/Board.h"
#include "../core/Player.h"
#include "../engine/Engine.h"

class Othello final {
private:
    std::atomic_bool _isGameShouldRun;
    Player _currentPlayer;
    std::shared_ptr<Board> _board;
    std::map<Player, std::shared_ptr<Engine>> _playerEngineMap;
    std::thread* _othelloThread;
    bool _showMoveTip;
    
private:
    void nextPlayer();
    void othelloThreadStart();

public:
    Othello();
    ~Othello();
    bool getIsGameRun();
    void startOthello();
    void endOthello();
    void setEngine(Player player, std::shared_ptr<Engine> enginePtr);
    const std::shared_ptr<Engine> getEngine(Player player);
    const Player getCurrentPlayer();
    const std::shared_ptr<Board> getBoard();
    PlayerScoreMap getPlayerScore();
    void setShouldShowMoveTip();
    bool getShouldShowMoveTip();
};

#endif
