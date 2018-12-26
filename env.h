#ifndef ENV
#define ENV

#include <array>
#include <vector>
#include <list>
#include <string>

namespace env {
    typedef std::array<int, 2> Card;
    typedef std::vector<Card> Pack;
    typedef std::vector<Card> Hand;
    typedef std::array<int, 3> Contract;

    void help(int w);
    std::list<char>::iterator find(std::list<char>::iterator, std::list<char>::iterator, const char&);
    std::vector<Card>::iterator find(std::vector<Card>::iterator, std::vector<Card>::iterator, const Card&);
    std::string showCards(const Hand &);
    std::array<Hand, 4> deal(unsigned int);

    struct Winner {
    	Card c;
    	int id;
    };

    extern char color[4];

    extern char p[13];

	extern std::array<std::string, 4> players;
}

#endif
