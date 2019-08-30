#ifndef __REWARD__H_
#define __REWARD__H_
#include <Board.hpp>
#include <memory>

class TTTReward {
 public:
    TTTReward(Player p) {
        _player = p;
    }
   Player GetPlayer() { return _player; }
   virtual int GetReward(std::string prev_state, Board &b ) = 0;
   Player _player;
};

class TTTEndReward: public TTTReward
{
 public:
   TTTEndReward(Player p): TTTReward(p) {};
   int GetReward(std::string prev_state, Board &b);

};

class TTTFancyReward: public TTTReward
{
 public:
   TTTFancyReward(Player p): TTTReward(p) {};
   int GetReward(std::string prev_state, Board &b);
};

typedef std::shared_ptr<TTTReward> TTTReward_h;
typedef std::shared_ptr<TTTEndReward> TTTEndReward_h;
typedef std::shared_ptr<TTTFancyReward> TTTFancyReward_h;

#endif
