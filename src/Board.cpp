#include <iostream>
#include <Board.hpp>
#include <assert.h>

using namespace std;

void
Board::GetResult(Player &p, Result_t &r)
{
    if (winner_present) {
        p = Winner;
        r = Win;
    } else if (tie) {
        r = Tie;
    } else {
        r = Invalid;
    }
}

void
Board::PrintBoard()
{
	for (int row = 0; row <3; ++row) {
		cout << endl << "| ";
		for (int col = 0; col <3; ++col) {
		    int pos = row*3+col;
		    cout << current_state.at(pos)<< " | " ;
		}
		cout <<endl << "|---|---|---|\n";
	}

    if (!moves) {
        cout << "Game is yet to start\n";
    }

	if (HasPlayer1Won()) {
		cout << "Player1 won in moves: " << moves << "\n";
	} else if(HasPlayer2Won()) {
		cout << "Player2 won in moves: " << moves << "\n";
	} else if (tie) {
		cout << "Game is tie in moves: " << moves << "\n";
	} else {
		cout << "Middle of Game\n";
    }
	return;
}

Result_t
Board::UpdateMove(unsigned int row, unsigned col, Player p) {
    if (winner_present || tie) {
        std::cout << "Winner already declared";
        return Invalid;
    }

    if(next_player != p){
        std::cout << "Fatal.. expecting move from "; 
        if (p == PLAYER_1) { std::cout << "player2\n"; } else {std::cout << "player1\n";}
        assert(0);
    }
        

    if (position[row][col] == 'e') {
		if (p == PLAYER_1) {
        	position[row][col]=player1;
            next_player = PLAYER_2;
		} else if (p == PLAYER_2)  {
        	position[row][col]=player2;
            next_player = PLAYER_1;
		}
    } else {
        std::cout << "Fatal.. position already taken by ";
        std::cout << position[row][col] << "row: "<< row<< "col: " << col << std::endl;
		assert(0);
    }
	moves++;
    ComputeState();
	CheckWinning();
    if (tie) {
        return Tie;
    } else if (winner_present) {
        return Win;
    } else {
        return GameOn;
    }
}

void
Board::ComputeState() {
    current_state.clear();
    for (int row=0; row<3; ++row) {
        for(int col=0; col<3; ++col) {
            current_state += position[row][col];
        }
    }
}

bool 
Board::HasPlayer1Won()
{
	if(winner_present && (Winner == PLAYER_1))
		return true;
	return false;
}

bool 
Board::HasPlayer2Won()
{
	if(winner_present && (Winner == PLAYER_2))
		return true;
	return false;
}

void
Board::CheckWinning()
{
	winner_present = false;

	/* check row */
	for (int row = 0; row < 3; ++ row) {
		if (position[row][0] == position[row][1]) {
			if (position[row][1] == position[row][2]) {
				winner_present = true;
				if (position[row][2] == player1) {
					Winner = PLAYER_1;
				} else if (position[row][2] == player2) {
					Winner = PLAYER_2;
				} else {
					Winner = BOARD_3;
					winner_present = false;
				}
			}
		}
	}

	/* check column*/
	for (int col = 0; col < 3; ++col) {
		if (position[0][col] == position[1][col]) {
			if (position[1][col] == position[2][col]) {
				winner_present = true;
				if (position[2][col] == player1) {
					Winner = PLAYER_1;
				} else if (position[2][col] == player2) {
					Winner = PLAYER_2;
				} else {
					Winner = BOARD_3;
					winner_present = false;
				}
			}
		}
	}


	/* check diagnoal */
	if (position[0][0] == position[1][1]) {
		if(position[2][2] == position[1][1]) {
			winner_present = true;
			if (position[1][1] == player1) {
				Winner = PLAYER_1;
			} else if (position[2][2] == player2) {
				Winner = PLAYER_2;
			} else {
				Winner = BOARD_3;
				winner_present = false;
			}
		}
	}

	if (position[0][2] == position[1][1]) {
		if(position[2][0] == position[1][1]) {
		    winner_present = true;
			if (position[1][1] == player1) {
				Winner = PLAYER_1;
			} else if (position[1][1] == player2) {
				Winner = PLAYER_2;
			} else {
				Winner = BOARD_3;
				winner_present = false;
			}
		}
	}

	if (moves == 9) {
		if (winner_present == false) {
			tie = true;
		}
	}

	return;
}


