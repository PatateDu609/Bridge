#include "env.h"
#include "game.h"
#include <iostream>
#include <array>

int main() {
	std::array<env::Hand, 4> hands = env::deal();

	std::cout << "BRIDGE" << std::endl << std::endl << std::endl;
	bidding::BidResult r = bidding::doBidding(hands, 0);
	int result = game::play(hands, r);

	if(result == 1 || result == 0) std::cout << "Winner : ";
	if (result == 1) std::cout << "Team East/West" << std::endl;
	else if (result == 0) std::cout << "Team North/South" << std::endl;

	return 0;
}
