#ifndef PLAYER_H
#define PLAYER_H

#include "CardDeck.h"
#include <string>
#include <vector>
using namespace std;

class Participant
{
protected:
    string name;

public:
    Participant(string n = "");
    virtual ~Participant();
    virtual string getName() const;
    virtual void showHoleCards() const = 0;
};

class Player : public Participant
{
private:
    int chips;
    vector<Card> holeCards;
    bool folded;
    int currentBet;

public:
    Player(string n = "", int startingChips = 0);
    void resetForNewHand();
    void resetBet();
    void addHoleCard(Card card);
    int payChips(int amount);
    void winChips(int amount);
    void fold();
    int getChips() const;
    int getCurrentBet() const;
    bool isFolded() const;
    bool canPlay() const;
    vector<Card> getHoleCards() const;
    void showHoleCards() const;
};

#endif
