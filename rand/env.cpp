#include "env.h"
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>

typedef env::Card Card;
typedef env::Hand Hand;

//Tableau des couleurs des cartes
char env::color[4] = {
		'S',
		'H',
		'D',
		'C'
};

//Tableau des emplacements des joueurs
std::array<std::string, 4> env::players = {
	   "North",
	   "East",
	   "South",
	   "West"
};

//Tableau des puissances des cartes
char env::p[13] = {
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

//Fonctionnement de la commande help
void env::help(int w) {
	if (w == 0) {
		std::cout << "Enter your card value (colour then level) to end your turn\n";
		std::cout << "Type game to see the cards played by the others\n";
		std::cout << "Type contract to see the current contract\n";
		std::cout << "Type score to see the current score\n";
		std::cout << "Type show to see the hand in argument (ex : show n or show north to see the north's hand).\n";
	}
	else if(w == 1){
		std::cout << "Enter your contract (color then tricks) to end your turn\n";
		std::cout << "Type winner to see what is the current best contract\n";
		std::cout << "Type pass to end your turn without choosing a contract\n";
	}

	std::cout << "Type stop to stop the program\n";
	std::cout << std::endl;
}


//Vérifie si une liste de caractères contient un certain caractère
std::list<char>::iterator env::find(std::list<char>::iterator start, std::list<char>::iterator end, const char& value) {
	for (; start != end; ++start) {
		if (*start == value) return start;
	}
	return end;
}

//Vérifie si une main contient une certaine carte
std::vector<Card>::iterator env::find(std::vector<Card>::iterator start, std::vector<Card>::iterator end, const Card& value) {
	for (; start != end; ++start) {
		if (*start == value) return start;
	}
	return end;
}

//Distribue les cartes
std::array<Hand, 4> env::deal(unsigned int seed) {
	srand(seed);
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

//Adapte l'affichage (classe les cartes par ordre de puissance) et affiche
std::string env::showCards(const Hand &h) {
	std::ostringstream oss;
	std::list<char> c[4];
	for (unsigned int i = 0; i < h.size(); i++) c[h[i][0]].push_back(p[h[i][1]]);

	for (int i = 0; i < 4; i++) {
		c[i].sort();
		c[i].reverse();

		std::list<char>::iterator searchS = env::find(c[i].begin(), c[i].end(), 'S');
		std::list<char>::iterator searchR = env::find(c[i].begin(), c[i].end(), 'R');

		if (searchS != c[i].end()) *searchS = 'A';
		if (searchR != c[i].end()) *searchR = 'K';
	}

	for (int i = 0; i < 4; i++) {
		c[i].push_front(':');
		c[i].push_front(color[i]);
	}
	for (int i = 0; i < 4; i++) {
		for (char cha : c[i]) {
			if (cha == 'I') oss << 'T';
			else oss << cha;

			oss << " ";
		}
		oss << std::endl;
	}

	return oss.str();
}
