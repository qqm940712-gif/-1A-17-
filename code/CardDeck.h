#ifndef CARD_DECK_H
#define CARD_DECK_H

#include <string>
#include <vector>
using namespace std;

enum Suit
{
    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES
};

struct Card
{
    int rank; // 2~14, 11=J, 12=Q, 13=K, 14=A
    Suit suit;
};

string rankToString(int rank);
string suitToString(Suit suit);
string cardToString(Card card);

class Deck
{
private:
    vector<Card> cards;
    int index;

public:
    Deck();
    void reset();
    void shuffle();
    Card draw();
};

#endif
