#include "env.h"
#include "game.h"
#include <iostream>
#include <array>
#include <ctime>

//Lance la partie et affiche l'équipe gagnante
int main() {
	std::array<env::Hand, 4> hands = env::deal((unsigned int) time(0));

	std::cout << "BRIDGE" << std::endl << std::endl << std::endl;
	bidding::BidResult r = bidding::doBidding(hands, 0);
	/*bidding::BidResult r;
	r.c = { -1, 5, 3 };
	r.turn = 3;
	r.dead = 1;*/
	int result = game::play(hands, r);

	if(result == 1 || result == 0) std::cout << "Winner : ";
	if (result == 1) std::cout << "Team East/West" << std::endl;
	else if (result == 0) std::cout << "Team North/South" << std::endl;

	return 0;
}
