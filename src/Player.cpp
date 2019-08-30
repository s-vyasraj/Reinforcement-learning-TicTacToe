#include <Player.hpp>
#include <assert.h>

void
TTTGameInfo::UpdateGame(std::string &current_state, int played_row, int played_col) 
{
    BoardPosition p = std::make_tuple(played_row, played_col);

    //TBD: check whether it exists
    game_moves[step] = current_state;
    
    played_move[current_state] = p;
    game_state = current_state.c_str();
    step++;
}

void
TTTGameInfo::UpdateReward(int reward)
{
    if (game_state.empty()) {
        std::cout << "FATAL: Game_state not updated \n";
        assert(0);
    } 
    
    total_rewards += reward;
    reward_sequence[game_state] = reward;
}

void
TTTPlayer::StartNewGame (bool starting_first)
{
    if (is_prev_game_completed == false) {
        std::cout << "FATAL: Previous game not yet completed Game: " << game_count << "\n";
        assert(0);
    }
    waiting_for_board_state_update = true;
    current_game = std::make_shared<TTTGameInfo>(game_count);
    game_detail[game_count] = current_game;
    game_count++;  
}

status
TTTPlayer::GetNextRandomMove(unsigned int &row, unsigned int &col)
{
    if (waiting_for_board_state_update == true) {
        std::cout << "FATAL: Board_state not yet updated \n";
        assert(0);
    }

    size_t n = std::count(board_state.begin(), board_state.end(), 'e');
    const char *c = board_state.c_str();
    if (!n) {
        return FAIL;
    }
    int random_val = rand() % n;


    row = 0; col = 0;
    int count = 0;
    for (row=0; row<3; ++row){
        for (col=0; col<3; ++col){
            int pos = row*3+col;
            if(c[pos] == 'e') {    /* found an empty slot */
                if (count == random_val) {
                    int r = row; int c = col;
                    if (current_game) {
                        current_game->UpdateGame(board_state, r, c);
                    }
                    return OK; 
                }
                count++;
            }
        }
    }

    return  FAIL;
}

void
TTTPlayer::UpdateResultCloseGame(Player &winner, Result_t &result)
{
    switch(result) {
        case Win:
            if (winner == player_type) {
                win_count++; 
            } else {
                lost_count++;
            }
            break;
        case Tie:
            tie_count++; break;
        default:
            std::cout << "Invalid win/lost/tie result\n";
            assert(0);
    }
    is_prev_game_completed = true;
}

void
TTTPlayer::PrintStats()
{
    int total_games = win_count + lost_count + tie_count;
    if(player_type == PLAYER_1) {
        std::cout << "\n**Player1 Stats:\n";
    } else {
        std::cout << "\n**Player2 Stats:\n";
    }
    std::cout << " Win: " << win_count;
    std::cout << " Lost: " << lost_count;
    std::cout << " Tie: " << tie_count << std::endl;
    std::cout << " P(W) = " << float(win_count)/total_games;
    std::cout << " P(L) = " << float(lost_count)/total_games;
    std::cout << " P(T) = " << float(Tie)/total_games;
    std::cout << "\n";
}


void
TTTPlayer::RLAnalyze()
{
    std::cout << "Size: " << game_detail.size() << "\n";
    int total_rewards = 0;
    for (auto it=game_detail.begin(); it != game_detail.end(); ++it) {
        auto game = it->second;
        //std::cout << " GameNo: " << game->game_num << " Rewards: " << game->total_rewards << "\n";
        total_rewards += game->total_rewards;
    }
    std::cout << "Reward Expectation: " << total_rewards << " \n";
}

void
TTTPlayer::RLLearn()
{

}
