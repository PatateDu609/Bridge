#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <list>
#include <array>

typedef std::array<int, 2> Card;
typedef std::vector<Card> Pack;
typedef std::vector<Card> Hand;

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

Card translate(std::string c) {
	int col, pow;
	switch (c[0]) {
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
	
	switch (c[1]) {
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

	return {col, pow};
}

Card compare(int color, int contract, Card const& a, Card const& b) {
	Card c;
	int x = a[0] - b[0], y = a[1] - b[1];
	bool ac = (a[0] == color),
		bc = (b[0] == color),
		acontract = (a[0] == contract),
		bcontract = (b[0] == contract);

	if (x == 0 && ac) {
		if (a[1] == 0 || b[1] == 0) { //règle le cas de l'as
			if (a[1] == 0) c = a;
			if (b[1] == 0) c = b;
		}
		else c = (y > 0) ? a : b;
	}
	else if (bc && !ac) {
		if (!acontract) c = b;
		else c = a;
	}
	else if (ac && !bc) {
		if (!bcontract) c = a;
		else c = b;
	}
	else if (x == 0 && !ac) {
		if (acontract) {
			if (a[1] == 0 || b[1] == 0) { //règle le cas de l'as
				if (a[1] == 0) c = a;
				if (b[1] == 0) c = b;
			}
			else c = (y > 0) ? a : b;
		}
	}

	return c;
}

std::list<char>::iterator find(std::list<char>::iterator start, std::list<char>::iterator end, const char& value) {
	for (; start != end; ++start) {
		if (*start == value) return start;
	}
	return end;
}

std::vector<char>::iterator find(std::vector<char>::iterator start, std::vector<char>::iterator end, const Card& value) {
	for (; start != end; ++start) {
		if (*start == value) return start;
	}
	return end;
}

void showCards(const Hand &h) {
	//Adapte l'affichage (classe les cartes par ordre de puissance) et affiche
	std::list<char> c[4];
	for (int i = 0; i < 13; i++) c[h[i][0]].push_back(p[h[i][1]]);

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
bool playable(Hand h, Card c) {
	return find(h[0], h[12], c) != h.end();
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

int main() {
	std::array<Hand, 4> hands = deal();
	std::cout << "joueur 1" << std::endl << std::endl;
	showCards(hands[2]);
	std::cout << std::endl;
	std::cout << "joueur 2" << std::endl << std::endl;
	showCards(hands[3]);

	Card a = hands[3][2], b = hands[2][8];

	std::cout << std::endl << std::endl << "joueur 1 : " << color[a[0]] << p[a[1]] << std::endl;
	std::cout << "joueur 2 : " << color[b[0]] << p[b[1]] << std::endl;

	Card c = compare(2, 2, a, b);
	if (c == a) std::cout << "joueur 1" << std::endl;
	else if (c == b) std::cout << "joueur 2" << std::endl;
	else std::cout << "Les 2 se sont defausses" << std::endl;

	system("PAUSE");
	return 0;
}
