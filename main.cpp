#include <iostream>
#include <array>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "env.h"
#include "game.h"

typedef env::Pack Pack;
typedef env::Hand Hand;
typedef env::Card Card;
typedef game::Winner Winner;

std::vector<Card> what(Hand h, int color) {
    std::vector<Card> possible;
	for (unsigned int i = 0; i < h.size(); i++) {
		if (game::playable(color, h, h[i]))
			possible.push_back(h[i]);
	}
    return possible;
}

int main() {
    unsigned int seed = (unsigned int) time(0);
    std::array<Hand, 4> hands = env::deal(seed);
    srand((unsigned int) time(0));
    std::ofstream file("test");

	Winner winner;
	Card tmp;

    int color;

    if(file.is_open()) {
        file << seed << std::endl;
        for(int j = 13, turn = 0; j > 0; j--) {
            for(int i = 0; i < 4; i++, turn++) {
				turn %= 4;
                if(i == 0) {
                    int d = rand() % j;
                    color = hands[turn][d][0];
                    file << game::translate(hands[turn][d]) << std::endl;

					winner.id = 0;
					winner.c = hands[turn][d];

					hands[turn].erase(hands[turn].begin() + d);
                }
                else {
                    std::vector<Card> possible = what(hands[turn], color);
                    int d = rand() % possible.size();
                    file << game::translate(possible[d]) << std::endl;
					std::vector<Card>::iterator it = env::find(hands[turn].begin(), hands[turn].end(), possible[d]);
					hands[turn].erase(it);

					tmp = game::compare(color, -1, winner.c, possible[d]);
					if (tmp == possible[d]) winner.id = turn;
					winner.c = tmp;
					if (i == 3) {
						turn = winner.id - 1;
					}
                }
            }
        }
    }

    return 0;
}
