#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <list>
#include <array>
#include <string>
#include <sstream>

typedef std::array<int, 2> Card;
typedef std::vector<Card> Pack;
typedef std::vector<Card> Hand;
typedef std::array<int, 3> Contract;

struct Winner {
	Card c;
	int id;
};

//Définition cartes
char color[4] = {
		'S',
		'H',
		'D',
		'C'
};

char p[13] = {
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'I',
	'J',
	'Q',
	'R',
	'S'
};

std::array<std::string, 4> players = {
	"North",
	"East",
	"South",
	"West"
};

bool isCard(std::string test) {
	bool col = 0, pow = 0;
	char powC = test[1], colC = test[0];
	switch (colC) {
	case 'S': case 'H': case 'D': case 'C':
		col = 1;
		break;
	}

	switch (powC) {
	case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '1': case 'J': case 'Q': case 'K': case 'A':
		pow = 1;
		break;
	}

	if (pow && col) return true;
	
	return false;
}

bool isContract(std::string test) {
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

std::string translate(Card c) {
	std::ostringstream cardStream;

	cardStream << color[c[0]];
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
		cardStream << p[c[1]];
	}
	return cardStream.str();
}

Card translate(char colC, char powC) {
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
	case '1':
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

std::string translateC(Contract c, bool owner = true) {
	std::ostringstream oss;
	switch(c[0]) {
	case -1:
		oss << "NT";
		break;
	default:
		oss << color[c[0]];
	}
	
	oss << c[1];

	if (owner) oss << players[c[2]][0];
	return oss.str();
}

Contract translateC(std::string command, int owner) {
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

Card compare(int color, int contract, Card const& a, Card const& b) {
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

Contract compareC (Contract const& a, Contract const& b) {
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

std::list<char>::iterator find(std::list<char>::iterator start, std::list<char>::iterator end, const char& value) {
	for (; start != end; ++start) {
		if (*start == value) return start;
	}
	return end;
}

std::vector<Card>::iterator find(std::vector<Card>::iterator start, std::vector<Card>::iterator end, const Card& value) {
	for (; start != end; ++start) {
		if (*start == value) return start;
	}
	return end;
}

void showCards(const Hand &h) {
	//Adapte l'affichage (classe les cartes par ordre de puissance) et affiche
	std::list<char> c[4];
	for (int i = 0; i < h.size(); i++) c[h[i][0]].push_back(p[h[i][1]]);

	for (int i = 0; i < 4; i++) {
		c[i].sort();
		c[i].reverse();

		std::list<char>::iterator searchS = find(c[i].begin(), c[i].end(), 'S');
		std::list<char>::iterator searchR = find(c[i].begin(), c[i].end(), 'R');

		if (searchS != c[i].end()) *searchS = 'A';
		if (searchR != c[i].end()) *searchR = 'K';
	}

	for (int i = 0; i < 4; i++) {
		c[i].push_front(':');
		c[i].push_front(color[i]);
	}
	for (int i = 0; i < 4; i++) {
		for (char cha : c[i]) {
			if (cha == 'I') std::cout << 10;
			else std::cout << cha;

			std::cout << " ";
		}
		std::cout << std::endl;
	}
}
bool playable(int color, Hand h, Card c) {
	if (find(std::begin(h), std::end(h), c) != std::end(h)) {
		if (c[0] == color) return true;
		else {
			for (int i = 0; i < h.size(); i++) if (h[i][0] == color) return false;
			return true;
		}
	}

	return false;
}

std::array<Hand, 4> deal() {
	std::srand(std::time(0));
	Pack pack, pack1;
	std::array<Hand, 4> hands;
	Card c;

	//Remplis le paquet
	for (int i = 0; i < 52; i++) {
		c[0] = i / 13;
		c[1] = i % 13;
		pack.push_back(c);
	}
	//Mélange
	for (int i = 51; i >= 0; i--) {
		std::vector<Card>::iterator it = pack.begin();
		int j = (i != 0) ? (rand() % i) : 0;
		pack1.push_back(pack.at(j));
		pack.erase(it + j);
	}
	pack = pack1;

	//Distribue
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			hands[i].push_back(pack.back());
			pack.pop_back();
		}
	}
	return hands;
}

void help(int w) {
	if (w == 0) {
		std::cout << "Enter your card value (colour then level) to end your turn\n";
		std::cout << "Type game to see the cards played by the others\n";
		std::cout << "Type contract to see the current contract\n";
		std::cout << "Type score to see the current score\n";
	}
	else if(w == 1){
		std::cout << "Enter your contract (color then tricks) to end your turn\n";
		std::cout << "Type winner to see what is the current best contract\n";
		std::cout << "Type pass to end your turn without choosing a contract\n";
	}

	std::cout << "Type show to see the hand in argument (ex : show n or show north to see the north's hand).\n";
	std::cout << "Type stop to stop the program\n";
	std::cout << std::endl;
}

int main() {
	std::array<Hand, 4> hands = deal();
	std::string command;
	char col, pow;	//carte du joueur
	Card cardT, defcard = { -1, -1 }, w;
	std::array<Card, 4> playedCards;
	int color, turn = 1, fp = turn, nbPass = 0, b = 0;
	bool ctr = false;
	Winner winner;
	std::array<int, 2> score = { 0, 0 };
	Contract c, win, temp, def = { -10, -10, -10 };

	std::cout << "BRIDGE" << std::endl << std::endl << std::endl;
	
	std::cout << "Bidding" << std::endl << std::endl;
	do {
		for(int i = 0; i < 4; i++, ++turn, ++b) {
			turn %= 4;
			help(1);
			std::cout << players[turn] << " plays" << std::endl;
			showCards(hands[turn]);
			
			do {
				std::cout << "Enter command" << std::endl;
				std::getline(std::cin, command);
				if (command == "stop") return(0);
				else if (command == "pass") {
					++nbPass;
					if ((nbPass == 3 && ctr)) goto game;
					break;
				}
				else if (command == "winner" && b > 0) {
					std::cout << translateC(win, true) << std::endl;
				}
				else if (command.find("show") != std::string::npos) {
					std::istringstream s(command);
					std::string b, c;
					s >> b >> c;
					switch (c[0]) {
					case 'n': case 'N':
						showCards(hands[0]);
						break;
					case 'e': case 'E':
						showCards(hands[1]);
						break;
					case 's': case 'S':
						showCards(hands[2]);
						break;
					case 'w': case 'W':
						showCards(hands[3]);
						break;
					default:
						showCards(hands[turn]);
					}
				}
				else if (isContract(command)) {
					c = translateC(command, turn);
					if (command == "NT7") {
						win = c;
						goto game;
					}
					if (!ctr) {
						win = c;
						ctr = 1;
						nbPass = 0;
						break;
					}
					else {
						temp = compareC(win, c);
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
					std::cout << players[turn] << " plays" << std::endl;
					showCards(hands[turn]);
				}
			} while(true);
		}

		if (nbPass == 4 && !ctr) {
			std::cout << std::endl << std::endl << std::endl << "no one has played, the cards will be dealt again" << std::endl << std::endl;
			hands = deal();
			nbPass = 0;
		}
	} while(true);
	
	game:

	std::cout << std::endl << std::endl << "Game with " << translateC(win) << std::endl << std::endl;
	for (int l = 0; l < 13; l++) {
		help(0);
		for (int i = 0; i < 4; i++, ++turn) {
			turn %= 4;
			std::cout << players[turn] << " plays" << std::endl;
			showCards(hands[turn]);

			do {//lecture de la commande et affectation aux variables
				std::cout << "Enter command" << std::endl;
				std::getline(std::cin, command);
				if (command == "stop") {
					return(0);
				}
				else if (command == "contract") {
					std::cout << translateC(win) << std::endl;
				}
				else if (command == "game" && i > 0) {
					for (int j = 0, jp = fp - 1; j < i; j++, jp++) {
						jp %= 4;
						std::cout << players[jp] << " : " << translate(playedCards[jp]) << std::endl;
					}
				}
				else if (command == "score")
					std::cout << "Team North/South : " << score[0] << std::endl << "Team East/West : " << score[1] << std::endl;
				else if (command.find("show") != std::string::npos) {
					std::istringstream s(command);
					std::string b, c;
					s >> b >> c;
					switch (c[0]) {
					case 'n': case 'N':
						showCards(hands[0]);
						break;
					case 'e': case 'E':
						showCards(hands[1]);
						break;
					case 's': case 'S':
						showCards(hands[2]);
						break;
					case 'w': case 'W':
						showCards(hands[3]);
						break;
					default:
						showCards(hands[turn]);
					}
				}
				else if (isCard(command)) {
					col = command[0];
					pow = command[1];
					cardT = translate(col, pow);

					if (cardT == defcard) std::cout << "The card you entered isn't existing" << std::endl;
					else {
						playedCards[turn] = cardT;
						if (i == 0) color = playedCards[turn][0];
						if (!playable(color, hands[turn], cardT)) std::cout << "The card you entered isn't in your hand, or you can't discard" << std::endl;
						else {
							hands[turn].erase(find(std::begin(hands[turn]), std::end(hands[turn]), cardT));
							break;
						}
					}
				}
				else {
					std::cout << std::endl;
					std::cout << players[turn] << " plays" << std::endl;
					showCards(hands[turn]);
				}
			} while (true);

			// Comparaison des cartes et détermine le vainqueur du pli
			if (i == 0) {
				winner.c = playedCards[turn];
				winner.id = turn;
			}
			else {
				w = compare(color, win[0], winner.c, playedCards[turn]);
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
