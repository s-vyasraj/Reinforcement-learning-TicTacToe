#ifndef __COMMON_HPP_
#define __COMMON_HPP_

typedef int status;

#define OK  0
#define FAIL 1

/* TBD: convert to class */
typedef enum {
	PLAYER_1,
	PLAYER_2,
	BOARD_3
}Player;

typedef enum {
    GameOn,
    Win,
    Lost,
    Tie,
    Invalid,
}Result_t;


#endif
