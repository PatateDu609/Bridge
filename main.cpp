#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <list>

typedef std::vector<int> Pack;
typedef std::vector<int> Hand;

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
	for (int i = 0; i < 13; i++) c[h[i] / 13].push_back(p[h[i] % 13]);
	
	for (int i = 0; i < 4; i++) {
		c[i].sort();
		c[i].reverse();

		std::list<char>::iterator searchS = std::find(c[i].begin(), c[i].end(), 'S');
		std::list<char>::iterator searchR = std::find(c[i].begin(), c[i].end(), 'R');

		if (searchS != c[i].end()) *searchS = 'A';
		if (searchR != c[i].end()) *searchR = 'K';
	}

	for (int i = 0; i < 4; i++) {
		c[i].push_front(':');
		c[i].push_front(color[i]);
	}
	for (int i = 0; i < 4; i++) {
		for (char cha : c[i]) {
			if(cha == 'I') std::cout << " " << 10;
			else std::cout << " " << cha;
		}
		std::cout << std::endl;
	}
}

int main() {
	std::srand(std::time(0));
	Pack pack, pack1;
	Hand hands[4];

	//Remplis le paquet
	for (int i = 0; i < 52; i++) pack.push_back(i);
	//Mélange
	for (int i = 51; i >= 0; i--) {
		std::vector<int>::iterator it = pack.begin();
		int j = (i != 0) ? (rand() % i) : 0;
		pack1.push_back(pack.at(j));
		pack.erase(it + j);
	}
	pack = pack1;
	pack1.clear();

	//Distribue
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			hands[i].push_back(pack.back());
			pack.pop_back();
		}
	}
	pack.clear();
	
	showCards(hands[2]);

	system("PAUSE");
	return 0;
}
