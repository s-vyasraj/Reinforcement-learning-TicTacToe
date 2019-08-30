#include <iostream>
#include <Board.hpp>
#include <Player.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <memory>
#include <Reward.hpp>

TTTReward_h Reward = nullptr;

static
status PlayGame(TTTPlayer &t1, TTTPlayer &t2, Result_t &result, Player &winner)
{
	Board b(t1.GetChar(), t2.GetChar());
    unsigned int row, col;
    std::string s1, s2;

    t1.StartNewGame(true);
    t2.StartNewGame(false);
    for (int i=0; ; ++i) {
        /* Player 1 */
        if ((i != 0) && (Reward->GetPlayer() == t1.GetPlayerType())) {
            int rval = Reward->GetReward(s1, b);
            t1.UpdateReward(rval);
        }
        s1 = b.GetCurrentBoardState();
        t1.UpdateBoardStatus(s1);
        if (t1.GetNextRandomMove(row, col) == OK) {
	        b.UpdateMove(row,col, t1.GetPlayerType());
        }
        if (b.IsGameEnd()) break;

        /* Player 2 */
        if ((i != 0)  && (Reward->GetPlayer() == t2.GetPlayerType())) {
            int rval = Reward->GetReward(s1, b);
            t2.UpdateReward(rval);
        }
        s2 = b.GetCurrentBoardState();
        t2.UpdateBoardStatus(s2);
        if(t2.GetNextRandomMove(row, col) == OK) {
	        b.UpdateMove(row, col, t2.GetPlayerType());
        }
            

        if (b.IsGameEnd()) break;
    }
    int rval = Reward->GetReward(s1, b);
    if (Reward->GetPlayer() == t1.GetPlayerType()) {
        t1.UpdateReward(rval);
    } else {
        t2.UpdateReward(rval);
    }
    
    b.GetResult(winner, result);
    t1.UpdateResultCloseGame(winner, result);
    t2.UpdateResultCloseGame(winner, result);

    return OK;
}

int 
main()
{
    TTTPlayer t1('o',PLAYER_1), t2('x',PLAYER_2);

    Reward = std::make_shared<TTTEndReward>(t2.GetPlayerType());

    srand(time(NULL));

#define RUN_COUNT 100    
    for(int count =0; count < RUN_COUNT; ++count) {
            Player winner;
            Result_t result;
            if (PlayGame(t1, t2, result, winner) == OK) {
                if (result == Win) {
                    //std::cout << "Game ended Winner: ";
                    switch (winner) { 
                        case PLAYER_1:
                            std::cout << t1.GetChar(); break;
                        case PLAYER_2:
                            std::cout << t2.GetChar(); break;
                        default: 
                            std::cout << "XXXX";
                            assert(0); break;
                    }
                } else if (result == Tie) {
                    //std::cout << "Game ended Tie: ";
                    std::cout << "-";
                } else {
                    std::cout << "Fatal error: ";
                    assert(0);
                }
            } else {
                assert(0);
            }
    }

    t1.PrintStats();
    t2.PrintStats();
    t2.RLAnalyze();
    
    return 0;
}




