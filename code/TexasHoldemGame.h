#ifndef TEXAS_HOLDEM_GAME_H
#define TEXAS_HOLDEM_GAME_H

#include "CardDeck.h"
#include "HandEvaluator.h"
#include "Player.h"
#include <string>
#include <vector>
using namespace std;

class TexasHoldemGame
{
private:
    vector<Player> players;
    vector<Card> communityCards;
    Deck deck;
    int smallBlind;
    int bigBlind;
    int pot;

    int readInt(string message, int minValue);
    int activePlayerCount();
    int firstActivePlayer();
    int nextIndex(int index);
    int getDealerIndex(int bigBlindIndex);
    int getSmallBlindIndex(int bigBlindIndex);
    void showCommunityCards();
    void showTable();
    void pressEnter();
    void hideScreen();
    void postBlind(int playerIndex, int amount);
    void dealHoleCards();
    void burnCard();
    void dealFlop();
    void dealTurnOrRiver();
    void setNeedsAction(vector<bool> &needsAction, int exceptIndex);
    bool hasPlayerNeedAction(vector<bool> needsAction);
    void bettingRound(int startIndex, int currentHighestBet);
    void resetBets();
    void awardOnlyActivePlayer();
    void showdown();
    void playOneHand(int round, int bigBlindIndex);

public:
    TexasHoldemGame(int sb, int bb);
    void addPlayer(string name, int startingChips);
    void start();
};

#endif
