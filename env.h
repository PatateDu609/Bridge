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
    std::list<char>::iterator find(std::list<char>::iterator start, std::list<char>::iterator end, const char& value);
    std::vector<Card>::iterator find(std::vector<Card>::iterator start, std::vector<Card>::iterator end, const Card& value);
    std::string showCards(const Hand &h);
    std::array<Hand, 4> deal();

    struct Winner {
    	Card c;
    	int id;
    };

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
}

#endif
