#include "game.h"
#include "env.h"
#include <sstream>
#include <string>

typedef env::Hand Hand;
typedef env::Contract Contract;
typedef env::Card Card;

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
		cardStream << "10";
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

bool game::playable(int color, Hand &h, Card c) {
	if (env::find(std::begin(h), std::end(h), c) != std::end(h)) {
		if (c[0] == color) return true;
		else {
			for (int i = 0; i < h.size(); i++) if (h[i][0] == color) return false;
			return true;
		}
	}

	return false;
}

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
