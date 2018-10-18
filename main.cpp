#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <list>
#include <array>

typedef std::array<int, 2> Card;
typedef std::vector<Card> Pack;
typedef std::vector<Card> Hand;

Card compare(int color, int contract, Card const& a, Card const& b) {
	Card c;
	int x = a[0] - b[0], y = a[1] - b[1];
	if (x == 0) {
		c = (y > 0) ? a : b;
	}
	else {
		bool ac = a[0] == color,
			bc = b[0] == color,
			acontract = a[0] == contract,
			bcontract = b[0] == contract;

		if (contract != color) {
			if (!ac && !bc && !acontract && !bcontract) c = { -10, -10 };
			else if (ac && !bc && !bcontract) c = a; // cas où le joueur A pose une carte de la couleur du pli et le joueur B se défausse d'une carte
			else if (ac && bcontract) c = b; // cas où le joueur A pose une carte de la couleur du pli mais que le joueur B coupe avec une carte d'atout
			else if (acontract && bcontract) c = (y > 0) ? a : b; // cas où les deux joueurs coupent avec une carte d'atout, on compare la puissance des cartes
		}
		else if (contract == color || contract == 5) {
			if (!ac && !bc) c = { -10, -10 };
			else if (ac && !bc) c = a; // cas où B se défausse mais que A a joué dans la couleur du pli (qui peut aussi être celle du contrat si jeu avec atout)
			else if (ac && bc) c = (y > 0) ? a : b; // cas où A et B jouent dans la couleur du pli
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

void showCards(const Hand &h) {
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
			if(cha == 'I') std::cout << 10;
			else std::cout << cha;

			std::cout << " ";
		}
		std::cout << std::endl;
	}
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
		'10',
		'J',
		'Q',
		'K',
		'A'
	};

	std::cout << std::endl << std::endl << "joueur 1 : " << color[a[0]] << p[a[1]] << std::endl;
	std::cout << "joueur 2 : " << color[b[0]] << p[b[1]] << std::endl;

	Card c = compare(2, 2, a, b);
	if (c == a) std::cout << "joueur 1" << std::endl;
	else if (c == b) std::cout << "joueur 2" << std::endl;
	else std::cout << "Les 2 se sont defausses" << std::endl;

	system("PAUSE");
	return 0;
}
