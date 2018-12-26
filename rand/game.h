#ifndef GAME
#define GAME

#include "env.h"

#include <string>
#include <array>

namespace bidding {
	struct BidResult {
		env::Contract c;
		int dead;
		int turn;

		bool operator==(const BidResult& a) {
			return (a.c == c) && (dead == a.dead) && (turn == a.turn);
		}
	};
    bool isContract(std::string);
    std::string translate(env::Contract&, bool owner = true);
    env::Contract translate(std::string, int);
    env::Contract compare(env::Contract const&, env::Contract const&);
	BidResult doBidding(std::array<env::Hand, 4>&, int);
}

namespace game {
    struct Winner {
    	env::Card c;
    	int id;
    };

    bool isCard(std::string);
    std::string translate(env::Card);
    env::Card translate(char, char);
    env::Card compare(int, int, env::Card const&, env::Card const&);
    bool playable(int, env::Hand&, env::Card);
    std::string show(std::string, std::array<env::Hand, 4>, int, bool open = 0, int turn = -1);
	int play(std::array<env::Hand, 4>&, bidding::BidResult&);
}

#endif
