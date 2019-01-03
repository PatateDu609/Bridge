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

std::vector<Card> what(Hand& h, int color) {
    std::vector<Card> possible;
	for (unsigned int i = 0; i < h.size(); i++)
		if (game::playable(color, h, h[i]))
			possible.push_back(h[i]);
    return possible;
}

int main() {
    unsigned int seed = (unsigned int) time(0);
    std::array<Hand, 4> hands = env::deal(seed);
    srand((unsigned int) time(0));
    std::ofstream file("test");
	std::string str;

	Winner winner;
	Card tmp;

    int color;

    if(file.is_open()) {
        file << seed << std::endl;
		std::cout << seed << std::endl;
        for(int j = 13, turn = 0; j > 0; j--) {
            for(int i = 0; i < 4; i++, turn++) {
				turn %= 4;
				std::cout << env::players[turn] << std::endl;
                if(i == 0) {
                    int d = rand() % j;
                    color = hands[turn][d][0];
					std::cout << env::showCards(hands[turn]);
                    file << game::translate(hands[turn][d]) << std::endl;
                    std::cout << game::translate(hands[turn][d]) << std::endl << std::endl;

					winner.id = 0;
					winner.c = hands[turn][d];

					hands[turn].erase(hands[turn].begin() + d);
                }
                else {
                    std::vector<Card> possible = what(hands[turn], color);
                    int d = rand() % possible.size();
					std::cout << env::showCards(hands[turn]);
					file << game::translate(possible[d]) << std::endl;
					std::cout << game::translate(possible[d]) << std::endl << std::endl;
					std::vector<Card>::iterator it = env::find(hands[turn].begin(), hands[turn].end(), possible[d]);
					hands[turn].erase(it);

					tmp = game::compare(color, -1, winner.c, possible[d]);
					str = game::translate(tmp);
 					if (tmp == possible[d]) winner.id = turn;
					winner.c = tmp;

					std::cout << env::players[winner.id] << " wins" << std::endl;

					if (i == 3) {
						turn = winner.id - 1;
					}
                }
				//std::cin.get();
            }
        }
    }
	return 0;
}
