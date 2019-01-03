#include "game.h"
#include "env.h"
#include <sstream>
#include <string>
#include <iostream>
#include <array>
#include <ctime>

typedef env::Hand Hand;
typedef env::Contract Contract;
typedef env::Card Card;
typedef bidding::BidResult BidResult;

//Vérifie s'il y a un contrat
bool bidding::isContract(std::string test) {
	bool contract = 0, lvl = 0;
	char contractC = test[0], lvlC = test[test.size()-1];

	switch (contractC) {
	case 'S': case 'H': case 'D': case 'C': case 'N':
		contract = 1;
		break;
	}

	if((int) lvlC < '8' && (int) lvlC > '0') lvl = 1;

	if(lvl && contract) return true;

	return false;
}

//Traduit un vecteur en string
std::string bidding::translate(Contract& c, bool owner) {
	std::ostringstream oss;
	switch(c[0]) {
	case -1:
		oss << "NT";
		break;
	default:
		oss << env::color[c[0]];
	}

	oss << c[1];

	if (owner) oss << env::players[c[2]][0];
	return oss.str();
}

//Traduit un string en contrat
Contract bidding::translate(std::string command, int owner) {
	Contract c;
	c[2] = owner;
	switch(command[0]) {
	case 'N':
		c[0] = -1;
		break;
	case 'S':
		c[0] = 0;
		break;
	case 'H':
		c[0] = 1;
		break;
	case 'D':
		c[0] = 2;
		break;
	case 'C':
		c[0] = 3;
		break;
	}

	switch(command[command.size() - 1]) {
	case '1':
		c[1] = 1;
		break;
	case '2':
		c[1] = 2;
		break;
	case '3':
		c[1] = 3;
		break;
	case '4':
		c[1] = 4;
		break;
	case '5':
		c[1] = 5;
		break;
	case '6':
		c[1] = 6;
		break;
	case '7':
		c[1] = 7;
		break;
	}

	return c;
}

//Compare les contrats
Contract bidding::compare(Contract const& a, Contract const& b) {
	int x = a[0] - b[0], y = a[1] - b[1];
	Contract c, def = { -10, -10, -10 };
	if (y > 0) {
		c = a;
	}
	else if (y == 0) {
		if (x == 0) {
			c = def;
		}
		else if (x < 0) {
			c = def;
		}
		else {
			c = b;
		}
	}
	else {
		c = b;
	}

	return c;
}

//Réalise les enchères jusqu'a un contrat valide
BidResult bidding::doBidding(std::array<Hand, 4>& hands, int turn) {
	Contract c, win, temp, def = { -10, -10, -10 };
	std::string command;
	int nbPass = 0, b = 0, dead = 0;
	BidResult r, defr;
	bool ctr = false;

	defr.c = { -10, -10, -10 };
	defr.dead = -1;
	defr.turn = -1;

	std::cout << "Bidding" << std::endl << std::endl;
	do {
		for (int i = 0; i < 4; i++, ++turn, ++b) {
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
							else {
								std::cout << "The contract you typed is invalid." << std::endl;
							}
						}
					}
				}
				else {
					std::cout << std::endl;
					std::cout << env::players[turn] << " plays" << std::endl << env::showCards(hands[turn]);
				}
			} while (true);
		}

		if (nbPass == 4 && !ctr) {
			std::cout << std::endl << std::endl << std::endl << "no one has played, the cards will be dealt again" << std::endl << std::endl;
			hands = env::deal((unsigned int) time(0));
			nbPass = 0;
		}
	} while (true);
}

//vérifier que la carte est valable
bool game::isCard(std::string test) {
	bool col = 0, pow = 0;
	char powC = test[1], colC = test[0];
	switch (colC) {
	case 'S': case 'H': case 'D': case 'C':
		col = 1;
		break;
	}

	switch (powC) {
	case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case 'T': case 'J': case 'Q': case 'K': case 'A':
		pow = 1;
		break;
	}

	if (pow && col) return true;

	return false;
}

std::string game::translate(Card c) {
	std::ostringstream cardStream;

	cardStream << env::color[c[0]];
	switch (c[1]) {
	case 8:
		cardStream << "T";
		break;
	case 11:
		cardStream << "K";
		break;
	case 12:
		cardStream << "A";
		break;
	default:
		cardStream << env::p[c[1]];
	}
	return cardStream.str();
}

//Traduit la couleur et la puissance d'une carte en une une paire de nombres
Card game::translate(char colC, char powC) {
	int col, pow;
	switch (colC) {
	case 'S':
		col = 0;
		break;
	case 'H':
		col = 1;
		break;
	case 'D':
		col = 2;
		break;
	case 'C':
		col = 3;
		break;
	default:
		col = -1;
	}

	switch (powC) {
	case '2':
		pow = 0;
		break;
	case '3':
		pow = 1;
		break;
	case '4':
		pow = 2;
		break;
	case '5':
		pow = 3;
		break;
	case '6':
		pow = 4;
		break;
	case '7':
		pow = 5;
		break;
	case '8':
		pow = 6;
		break;
	case '9':
		pow = 7;
		break;
	case 'T':
		pow = 8;
		break;
	case 'J':
		pow = 9;
		break;
	case 'Q':
		pow = 10;
		break;
	case 'K':
		pow = 11;
		break;
	case 'A':
		pow = 12;
		break;
	default:
		pow = -1;
	}

	return { col, pow };
}

//Compare deux cartes et retourne la gagnante en fonction du contrat et de la couleur
Card game::compare(int color, int contract, Card const& a, Card const& b) {
	Card c;
	int x = a[0] - b[0], y = a[1] - b[1];
	bool ac = (a[0] == color),
		bc = (b[0] == color),
		acontract = (a[0] == contract),
		bcontract = (b[0] == contract);

	if (x == 0) c = (y > 0) ? a : b;
	else if (bc && !ac) c = a;
	else if (ac && !bc) {
		if (!bcontract) c = a; // B se défausse
		else c = b; // B coupe
	}
	return c;
}

//Vérifie qu'une carte est jouable
bool game::playable(int color, Hand &h, Card c) {
	if (env::find(std::begin(h), std::end(h), c) != std::end(h)) {
		if (c[0] == color) return true;
		else {
			for (unsigned int i = 0; i < h.size(); i++) if (h[i][0] == color) return false;
			return true;
		}
	}

	return false;
}

//Affichage des mains des joueurs
std::string game::show(std::string command, std::array<Hand, 4> hands, int dead, bool open, int turn) {
	std::ostringstream oss;
	if(open) {
		std::istringstream s(command);
		std::string b, c;
		s >> b >> c;
		switch (c[0]) {
		case 'n': case 'N':
			oss << env::showCards(hands[0]);
			break;
		case 'e': case 'E':
			oss << env::showCards(hands[1]);
			break;
		case 's': case 'S':
			oss << env::showCards(hands[2]);
			break;
		case 'w': case 'W':
			oss << env::showCards(hands[3]);
			break;
		default:
			oss << env::showCards(hands[turn]);
		}
	}
	else oss << env::showCards(hands[dead]);
	return oss.str();
}

int game::play(std::array<env::Hand, 4>& hands, bidding::BidResult& r) {
	std::string command;
	BidResult defr;
	char col, pow;	//carte du joueur
	Card cardT, defcard = { -1, -1 }, w;
	std::array<Card, 4> playedCards;
	int color, turn = 1, fp = turn, dead;
	Winner winner;
	std::array<int, 2> score = { 0, 0 };
	Contract win;

	defr.c = { -10, -10, -10 };
	defr.dead = -1;
	defr.turn = -1;

	if (r == defr) return -1;
	win = r.c;
	turn = r.turn;
	dead = r.dead;
	turn++;

	std::cout << std::endl << std::endl << "Game with " << bidding::translate(win) << std::endl << std::endl;
	for (int l = 0; l < 13; l++) {
		env::help(0);
		for (int i = 0; i < 4; i++, ++turn) {
			turn %= 4;

			std::cout << env::players[turn] << " plays";
			if (turn == dead) std::cout << " (played by " << env::players[dead - 2] << ")";
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
	return (score[0] > score[1]);
}
