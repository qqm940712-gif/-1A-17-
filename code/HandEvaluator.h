#ifndef HAND_EVALUATOR_H
#define HAND_EVALUATOR_H

#include "CardDeck.h"
#include <string>
#include <vector>
using namespace std;

class HandValue
{
public:
    int type;
    vector<int> ranks;

    HandValue(int t = 0, vector<int> r = {});
    bool operator>(const HandValue &other) const;
    bool operator==(const HandValue &other) const;
    string getTypeName() const;
};

class PokerHandEvaluator
{
private:
    static void sortDesc(vector<int> &nums);
    static void sortAsc(vector<int> &nums);
    static bool contains(vector<int> nums, int value);
    static vector<int> removeDuplicate(vector<int> nums);
    static int getStraightHigh(vector<int> ranks);

public:
    static HandValue evaluate(vector<Card> cards);
};

#endif
