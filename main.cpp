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

std::vector<Card> what(Hand h, int color) {
    std::vector<Card> possible;
    for(int i = 0; i < h.size(); i++)
        if(game::playable(color, h, h[i]))
            possible.push_back(h[i]);

    return possible;
}

int main() {
    unsigned int seed = (unsigned int) time(0);
    std::array<Hand, 4> hands = env::deal(seed);
    srand((unsigned int) time(0));
    std::ofstream file("test");

    int color;

    if(file.is_open()) {
        file << seed << std::endl;
        for(int j = 13; j > 0; j--) {
            for(int i = 0; i < 4; i++) {
                if(j == 13) {
                    int d = rand() % j;
                    color = hands[i][d][0];
                    file << game::translate(hands[i][d]) << std::endl;
                    hands[i].erase(hands[i].begin() + d);
                }
                else {
                    std::vector<Card> possible = what(hands[i], color);
                    int d = rand() % possible.size();
                    file << game::translate(hands[i][d]) << std::endl;
                    hands[i].erase(hands[i].begin() + d);
                }
            }
        }
    }

    return 0;
}
