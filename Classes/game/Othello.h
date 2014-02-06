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

class Othello final: public std::enable_shared_from_this<Othello> {
private:
    std::atomic_bool _isGameShouldRun;
    std::shared_ptr<Board> _board;
    std::map<Player, std::shared_ptr<Engine>> _playerEngineMap;
    std::map<Player, std::vector<std::pair<unsigned short, unsigned short>>> _playAvailPosMap;
    std::shared_ptr<std::stack<Player>> _playStack;
    std::thread* _othelloThread;
    bool _showMoveTip;
    
private:
    void updateAvailPos(Player player);
    void nextPlayer();
    void othelloThreadStart();

public:
    Othello();
    void setEngine(Player player, std::shared_ptr<Engine> enginePtr);
    const Player getCurrentPlayer();
    const std::shared_ptr<Engine> getEngine(Player player);
    const std::shared_ptr<Board> getBoard();
    const std::vector<std::pair<unsigned short, unsigned short>>& getPlayerAvailPos(Player player);
    const std::shared_ptr<std::stack<Player>> getPlayerStack();
    const short getPlayerScore(Player player);
    const std::map<Player, unsigned short>& getPlayerScoreMap();
    void setShouldShowMoveTip();
    bool getShouldShowMoveTip();
    bool getIsGameRun();
    void startOthello();
    void endOthello();
	~Othello();
};

#endif
