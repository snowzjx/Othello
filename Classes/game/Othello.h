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
    
    // Constructor.
    Othello();
    
    // Desturctor.
    ~Othello();
    
    // Get whether the Othello is running or not.
    bool getIsGameRun();
    
    // Start Othello game, the game will run on a background thread.
    void startOthello();
    
    // Stop Othello game.
    void endOthello();
    
    // Set the player engine.
    void setEngine(Player player, std::shared_ptr<Engine> enginePtr);
    
    // Get the player engine.
    const std::shared_ptr<Engine> getEngine(Player player);
    
    // Get the current player.
    const Player getCurrentPlayer();
    
    // Get the Othello board.
    const std::shared_ptr<Board> getBoard();
    
    // Set whether should show the move tip.
    void setShouldShowMoveTip();
    
    // Get whether should show the move tip.
    bool getShouldShowMoveTip();
};

#endif
