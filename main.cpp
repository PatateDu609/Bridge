#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>

typedef std::vector<int> Pack;
typedef std::vector<int> Hand;
typedef std::vector<std::string> PBN;

PBN init() {
	PBN pbn;
	std::string color[4] = {
		"S",
		"H",
		"D",
		"C"
	};

	std::string p[13] = {
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"10",
		"J",
		"Q",
		"K",
		"A"
	};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			pbn.push_back(color[i] + p[j]);
		}
	}

	return pbn;
}

int main() {
	std::srand(std::time(0));
	Pack pack, pack1;
	Hand hands[4];
	PBN pbn = init();


	for (int i = 0; i < 52; i++) {
		pack.push_back(i);
	}

	for (auto i = 51; i >= 0; i--) {
		int j;
		std::vector<int>::iterator it = pack.begin();
		if (i != 0) j = rand() % i;
		else j = 0;
		pack1.push_back(pack.at(j));
		pack.erase(it + j);
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			hands[i].push_back(pack1.back());
			pack1.pop_back();
		}
	}

	for (int i = 0; i < 13; i++) {
		std::cout << pbn[hands[1][i]] << std::endl;
	}

	system("PAUSE");
	return 0;
}