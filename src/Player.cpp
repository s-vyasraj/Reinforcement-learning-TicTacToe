#include <Player.hpp>
#include <assert.h>

void
TTTGameInfo::UpdateGame(std::string &current_state, int played_row, int played_col) 
{
    std::string s = current_state;
    BoardPosition p = std::make_tuple(played_row, played_col);

    //TBD: check whether it exists
    game_moves[step] = s;
    
    played_move[s] = p;
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
    game_detail[game_count] = current_game;
    game_count++;  

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
    int total_rewards = 0;
    for (auto it=game_detail.begin(); it != game_detail.end(); ++it) {
        auto game = it->second;
        //std::cout << " GameNo: " << game->game_num << " Rewards: " << game->total_rewards << "\n";
        total_rewards += game->total_rewards;
    }
    std::cout << "Reward Expectation: " << total_rewards << " \n";
}

status
TTTPlayer::RLSearch(std::string search, unsigned int &row, unsigned int &col)
{
    bool got_best_move = false;
    int best_reward = -10000;
    BoardPosition p;

    std::cout << "Searching... \n";
    /* Search in each game - for best moves */
    for (auto it=game_detail.begin(); it != game_detail.end(); ++it) {
        auto game = it->second;
        /* Search for states and and associated reward */
        //std::cout << " Searching in game: " << it->first ;
        auto itr = game->reward_sequence.find(search);
        if (itr != game->reward_sequence.end()) {
            //std::cout << "Found with reward:" << itr->second << "\n";
            int reward_for_move = itr->second;
            if (reward_for_move > best_reward) {
                auto it2 = game->played_move.find(search);
                if (it2 != game->played_move.end()) {
                    p = it2->second;        /* Position */

                    //std::cout << "Found move \n";
                    got_best_move = true;
                    best_reward = reward_for_move;
                    row = std::get<0>(p);
                    col = std::get<1>(p);
                }else {
                    int i = 0;
                    for (auto itp = game->played_move.begin(); 
                            itp != game->played_move.end(); ++itp) {
                        std::cout << i++ << ": " << itp->first << "\n";
                    }
                    std::cout<<"Fatal: Game data error: " << it->second << "\n"; 
                    assert(0);
                }
            }
        }
    }
    if (got_best_move) {
        std::cout <<"Got move from Database \n";
        if(current_game) {
            int r = row; int c = col;
            current_game->UpdateGame(search, r, c);
        }
        return OK;
    }
    std::cout <<"Making random move\n";
    return GetNextRandomMove(row, col);
}

status
TTTPlayer::GetPersonMove(unsigned int &row, unsigned int &col)
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

    bool row_valid = false;
    while(1) {
        if (row_valid == false) {
            std::cout << "Enter row [0,1,2]: ";
            std::cin >> row;
            if ((row > 2)) {
                std::cout << "Enter a value between [0-2]\n";
                continue;
            }
            row_valid = true;
        }
        std::cout << "\nEnter column[0,1,2]: ";
        std::cin >> col;
        if ((col > 2)) {
            std::cout << "Enter a value between [0-2]\n";
            continue;
        }

        int pos = row*3+col;
        if(c[pos] == 'e') {    /* found an empty slot */
            int r = row; int c = col;
            if (current_game) {
                current_game->UpdateGame(board_state, r, c);
            }
            return OK;
        } else {
            std::cout << "\nSlot not empty - reenter\n";
            row_valid = false;
            continue;
        }
    }
    return FAIL;
}
