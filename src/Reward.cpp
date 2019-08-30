#include <iostream>
#include <Reward.hpp>
#include <assert.h>

int
TTTEndReward::GetReward(std::string prev_state, Board &b)
{
    int reward_win = 1;
    int reward_tie = 0;
    int reward_fail = -1;
    int otherwise   = 0;

    std::string current_state = b.GetCurrentBoardState();
    if (prev_state.compare(current_state) == 0) {
        std::cout <<" Fatal:: Previous and current states are same\n";
        assert(0);
    }

    if (b.IsGameEnd()) {
        switch(_player) {
            case PLAYER_1:
                if(b.HasPlayer1Won()) return reward_win;
                if(b.IsGameTie()) return reward_tie;
                return reward_fail;
                break;
            case PLAYER_2:
                if(b.HasPlayer2Won()) return reward_win;
                if(b.IsGameTie()) return reward_tie;
                return otherwise;
                break;
            default:
                assert(0);
                break;
        }
    }
    return otherwise;
}

int
TTTFancyReward::GetReward(std::string prev_state, Board &b)
{
    int no_of_moves = b.GetMoves();
    assert(no_of_moves <=9);
    int reward_win = (10 - no_of_moves);
    int reward_tie = 1;
    int reward_fail = (0 - no_of_moves);
    int otherwise   = 0;

    std::string current_state = b.GetCurrentBoardState();
    if (prev_state.compare(current_state) == 0) {
        std::cout <<" Fatal:: Previous and current states are same\n";
        assert(0);
    }

    if (b.IsGameEnd()) {
        switch(_player) {
            case PLAYER_1:
                if(b.HasPlayer1Won()) return reward_win;
                if(b.IsGameTie()) return reward_tie;
                return reward_fail;
                break;
            case PLAYER_2:
                if(b.HasPlayer1Won()) return reward_win;
                if(b.IsGameTie()) return reward_tie;
                break;
            default:
                assert(0);
                break;
        }
    }
    return otherwise;
}


