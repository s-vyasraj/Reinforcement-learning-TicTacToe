#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <iostream>
#include <string>
#include <common.hpp>

class Board {
    public:
        Board(char p1, char p2) {
			player1 = p1;
			player2 = p2;
            //std::cout << "Resetting board" << std::endl;
            Reset();
        }


        ~Board() {

        }

        void Reset() {
			winner_present = false;
			tie = false;
			moves = 0;
            next_player = PLAYER_1;
            for (int row=0; row<3; ++row) {
                for(int col=0; col<3; ++col) {
                    position[row][col] = 'e';
                }
            }
	    	ComputeState();
        }

        Result_t UpdateMove(unsigned int row, unsigned col, Player p);


        std::string GetCurrentBoardState() {
            return current_state;
        }
        void PrintBoard();

		bool HasPlayer1Won();
		bool HasPlayer2Won();
        bool IsGameTie() { if (IsGameEnd()) { if (tie) return true;} return false; }
        bool IsGameEnd() {
            if (winner_present || tie) {
                 return true;
            } else if (moves == 9) {
                 return true;
            }
            return false;
        }
        void GetResult(Player &p, Result_t &r);
        int GetMoves() { return moves; }


    private:

        char position[3][3];
        std::string current_state;

        bool winner_present;
        Player Winner;
		bool tie;
		int moves;

		char player1;
		char player2;
        Player next_player;

		void ComputeState();
		void CheckWinning();
};

#endif



