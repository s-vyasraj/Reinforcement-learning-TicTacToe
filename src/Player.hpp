#ifndef __PLAYER__H_
#define __PLAYER__H_

#include <iostream>
#include <common.hpp>
#include <algorithm>
#include <stdlib.h>
#include <map>
#include <string>
#include <memory>
#include <Reward.hpp>

typedef std::tuple<int, int>  BoardPosition;

class TTTGameInfo {
 public:
    TTTGameInfo(int g) { step = 0; game_num = g; total_rewards = 0;}
    void UpdateGame(std::string &current_state, int row, int col);
    void UpdateReward(int reward);

    int step;
    int game_num;
    int total_rewards;
    std::string game_state;
    std::map<int, std::string> game_moves;  /* move number and current_state */
    std::map<std::string, BoardPosition> played_move;   /*current_state, row, col */
    std::map<std::string, int> reward_sequence;   /*current_state, row, col */
};
typedef std::shared_ptr<TTTGameInfo> TTTGameInfo_h;

class TTTPlayer 
{
 public:
    TTTPlayer(char c, Player p) {
        token = c;
        player_type = p;
        game_count = win_count = lost_count = tie_count = 0;
        is_prev_game_completed = true;
        waiting_for_board_state_update = true;
        current_game = nullptr;
    }

    void StartNewGame(bool starting_first);
    void UpdateBoardStatus(std::string &s) {
         board_state = s;
         waiting_for_board_state_update = false;
    }

    status GetNextRandomMove(unsigned int &row, unsigned int &col);
    void   UpdateReward(int i) { current_game->UpdateReward(i); };

    char GetChar() { return token; }
    Player GetPlayerType() { return player_type; }

    void UpdateResultCloseGame(Player &p, Result_t &r);
    void PrintStats();

    void RLAnalyze();
    void RLLearn();

 private:
    std::string  board_state;
    char token;
    Player player_type;
    bool is_prev_game_completed;
    bool waiting_for_board_state_update;

    int game_count;
    int win_count;
    int lost_count;
    int tie_count;

    std::map<int, TTTGameInfo_h> game_detail;
    TTTGameInfo_h current_game;
};


#endif
