#include "env.h"
#include "game.h"
#include <iostream>

typedef env::Card Card;
typedef env::Contract Contract;
typedef env::Hand Hand;
typedef game::Winner Winner;

struct BidResult {
	Contract c;
	int dead;
	int turn;
};

BidResult doBidding(std::array<Hand, 4>& hands) {
	Contract c, win, temp, def = { -10, -10, -10 };
	std::string command;
	int turn, nbPass, b = 0, dead;
	BidResult r, defr;
	bool ctr = false;

	defr.c = def;
	defr.dead = -1;
	defr.turn = -1;

	std::cout << "Bidding" << std::endl << std::endl;
	do {
		for(int i = 0; i < 4; i++, ++turn, ++b) {
			turn %= 4;
			env::help(1);
			std::cout << env::players[turn] << " plays" << std::endl << env::showCards(hands[turn]);

			do {
				std::cout << "Enter command" << std::endl;
				std::getline(std::cin, command);
				if (command == "stop") return defr;
				else if (command == "pass") {
					++nbPass;
					if ((nbPass == 3 && ctr)) {
							r.turn = turn;
							r.c = win;
							r.dead = turn + 2;
							return r;
					}
					break;
				}
				else if (command == "winner" && b > 0) {
					std::cout << bidding::translate(win, true) << std::endl;
				}
				else if (bidding::isContract(command)) {
					c = bidding::translate(command, turn);
					if (command == "NT7") {
						r.turn = turn;
						r.c = c;
						r.dead = turn + 2;
						return r;
					}
					if (!ctr) {
						win = c;
						ctr = 1;
						nbPass = 0;
						break;
					}
					else {
						temp = bidding::compare(win, c);
						if (temp != def) {
							if (win != temp) {
								win = temp;
								nbPass = 0;
								break;
							}
						}
					}
				}
				else {
					std::cout << std::endl;
					std::cout << env::players[turn] << " plays" << std::endl << env::showCards(hands[turn]);
				}
			} while(true);
		}

		if (nbPass == 4 && !ctr) {
			std::cout << std::endl << std::endl << std::endl << "no one has played, the cards will be dealt again" << std::endl << std::endl;
			hands = env::deal();
			nbPass = 0;
		}
	} while(true);
}

int main() {
	std::array<Hand, 4> hands = env::deal();
	std::string command;
	char col, pow;	//carte du joueur
	Card cardT, defcard = { -1, -1 }, w;
	std::array<Card, 4> playedCards;
	int color, turn = 1, fp = turn, nbPass = 0, b = 0, dead;
	Winner winner;
	std::array<int, 2> score = { 0, 0 };
	Contract c, win, temp, def = { -10, -10, -10 };
	BidResult r;

	std::cout << "BRIDGE" << std::endl << std::endl << std::endl;

	r = doBidding(hands);
	win = r.c;
	turn = r.turn;
	dead = r.dead;

	game:
	dead = turn + 2;
	turn++;

	std::cout << std::endl << std::endl << "Game with " << bidding::translate(win) << std::endl << std::endl;
	for (int l = 0; l < 13; l++) {
		env::help(0);
		for (int i = 0; i < 4; i++, ++turn) {
			turn %= 4;

			std::cout << env::players[turn] << " plays";
			if(turn == dead) std::cout << " (played by " << env::players[dead - 2] << ")";
			std::cout << std::endl << env::showCards(hands[turn]);

			do {//lecture de la commande et affectation aux variables
				std::cout << "Enter command" << std::endl;
				std::getline(std::cin, command);
				if (command == "stop") {
					return(0);
				}
				else if (command == "contract") {
					std::cout << bidding::translate(win) << std::endl;
				}
				else if (command == "game" && i > 0) {
					for (int j = 0, jp = fp - 1; j < i; j++, jp++) {
						jp %= 4;
						std::cout << env::players[jp] << " : " << game::translate(playedCards[jp]) << std::endl;
					}
				}
				else if (command == "score")
					std::cout << "Team North/South : " << score[0] << std::endl << "Team East/West : " << score[1] << std::endl;
				else if (command.find("show") != std::string::npos) {
					std::cout << game::show(command, hands, dead);
				}
				else if (game::isCard(command)) {
					col = command[0];
					pow = command[1];
					cardT = game::translate(col, pow);

					if (cardT == defcard) std::cout << "The card you entered isn't existing" << std::endl;
					else {
						playedCards[turn] = cardT;
						if (i == 0) color = playedCards[turn][0];
						if (!game::playable(color, hands[turn], cardT)) std::cout << "The card you entered isn't in your hand, or you can't discard" << std::endl;
						else {
							hands[turn].erase(env::find(std::begin(hands[turn]), std::end(hands[turn]), cardT));
							break;
						}
					}
				}
				else {
					std::cout << std::endl;
					std::cout << env::players[turn] << " plays" << std::endl << env::showCards(hands[turn]);
				}
			} while (true);

			// Comparaison des cartes et détermine le vainqueur du pli
			if (i == 0) {
				winner.c = playedCards[turn];
				winner.id = turn;
			}
			else {
				w = game::compare(color, win[0], winner.c, playedCards[turn]);
				if (w != winner.c) winner.id = turn;
				winner.c = w;

				if (i == 3) {
					++score[winner.id % 2];
					turn = winner.id - 1;
					fp = turn + 2;
					for (int j = 0; j < 4; ++j) playedCards[j] = defcard;
				}
			}
			std::cout << std::endl;
		}
	}
	return 0;
}
