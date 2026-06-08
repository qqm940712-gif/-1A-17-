#include "Player.h"
#include <iostream>
using namespace std;

Participant::Participant(string n)
{
    name = n;
}

Participant::~Participant()
{
}

string Participant::getName() const
{
    return name;
}

Player::Player(string n, int startingChips) : Participant(n)
{
    chips = startingChips;
    folded = false;
    currentBet = 0;
}

void Player::resetForNewHand()
{
    holeCards.clear();
    folded = false;
    currentBet = 0;
}

void Player::resetBet()
{
    currentBet = 0;
}

void Player::addHoleCard(Card card)
{
    holeCards.push_back(card);
}

int Player::payChips(int amount)
{
    if (amount > chips)
    {
        amount = chips;
    }

    chips -= amount;
    currentBet += amount;
    return amount;
}

void Player::winChips(int amount)
{
    chips += amount;
}

void Player::fold()
{
    folded = true;
}

int Player::getChips() const
{
    return chips;
}

int Player::getCurrentBet() const
{
    return currentBet;
}

bool Player::isFolded() const
{
    return folded;
}

bool Player::canPlay() const
{
    return chips > 0;
}

vector<Card> Player::getHoleCards() const
{
    return holeCards;
}

void Player::showHoleCards() const
{
    cout << name << " 的手牌：";

    for (int i = 0; i < holeCards.size(); i++)
    {
        cout << cardToString(holeCards[i]) << " ";
    }

    cout << endl;
}
