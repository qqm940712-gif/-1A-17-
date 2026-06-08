#include "HandEvaluator.h"
using namespace std;

HandValue::HandValue(int t, vector<int> r)
{
    type = t;
    ranks = r;
}
// 用來存一副牌的結果
// type 代表牌型大小
// ranks 則是用來比較同牌型大小的數字

bool HandValue::operator>(const HandValue &other) const
{
    if (type != other.type)
    {
        return type > other.type;
    }

    for (int i = 0; i < ranks.size() && i < other.ranks.size(); i++)
    {
        if (ranks[i] != other.ranks[i])
        {
            return ranks[i] > other.ranks[i];
        }
    }

    return false;
}
// 比較兩手牌誰比較大

bool HandValue::operator==(const HandValue &other) const
{
    if (type != other.type || ranks.size() != other.ranks.size())
    {
        return false;
    }

    for (int i = 0; i < ranks.size(); i++)
    {
        if (ranks[i] != other.ranks[i])
        {
            return false;
        }
    }

    return true;
}
// 判斷兩手牌是否完全一樣大

string HandValue::getTypeName() const
{
    switch (type)
    {
    case 8:
        return "同花順";
    case 7:
        return "鐵支";
    case 6:
        return "葫蘆";
    case 5:
        return "同花";
    case 4:
        return "順子";
    case 3:
        return "三條";
    case 2:
        return "兩對";
    case 1:
        return "一對";
    default:
        return "高牌";
    }
}
// 把數字牌型轉成中文名稱

void PokerHandEvaluator::sortDesc(vector<int> &nums)
{
    for (int i = 0; i < nums.size() - 1; i++)
    {
        for (int j = 0; j < nums.size() - i - 1; j++)
        {
            if (nums[j] < nums[j + 1])
            {
                int temp = nums[j];
                nums[j] = nums[j + 1];
                nums[j + 1] = temp;
            }
        }
    }
}
// 排序順序

void PokerHandEvaluator::sortAsc(vector<int> &nums)
{
    for (int i = 0; i < nums.size() - 1; i++)
    {
        for (int j = 0; j < nums.size() - i - 1; j++)
        {
            if (nums[j] > nums[j + 1])
            {
                int temp = nums[j];
                nums[j] = nums[j + 1];
                nums[j + 1] = temp;
            }
        }
    }
}

bool PokerHandEvaluator::contains(vector<int> nums, int value)
{
    for (int i = 0; i < nums.size(); i++)
    {
        if (nums[i] == value)
        {
            return true;
        }
    }

    return false;
}

vector<int> PokerHandEvaluator::removeDuplicate(vector<int> nums)
{
    vector<int> result;

    for (int i = 0; i < nums.size(); i++)
    {
        if (!contains(result, nums[i]))
        {
            result.push_back(nums[i]);
        }
    }

    return result;
}

int PokerHandEvaluator::getStraightHigh(vector<int> ranks)
{
    ranks = removeDuplicate(ranks);
    sortAsc(ranks);

    if (contains(ranks, 14))
    {
        vector<int> newRanks;
        newRanks.push_back(1);

        for (int i = 0; i < ranks.size(); i++)
        {
            newRanks.push_back(ranks[i]);
        }

        ranks = newRanks;
    }

    int count = 1;
    int bestHigh = 0;

    for (int i = 1; i < ranks.size(); i++)
    {
        if (ranks[i] == ranks[i - 1] + 1)
        {
            count++;

            if (count >= 5)
            {
                bestHigh = ranks[i];
            }
        }
        else
        {
            count = 1;
        }
    }

    return bestHigh;
}

HandValue PokerHandEvaluator::evaluate(vector<Card> cards)
{
    vector<int> rankCount(15, 0);
    vector<vector<int>> suitCards(4);

    for (int i = 0; i < cards.size(); i++)
    {
        rankCount[cards[i].rank]++;
        suitCards[cards[i].suit].push_back(cards[i].rank);
    }

    for (int s = 0; s < 4; s++)
    {
        if (suitCards[s].size() >= 5)
        {
            int high = getStraightHigh(suitCards[s]);
            if (high > 0)
            {
                return HandValue(8, {high});
            }
        }
    }

    for (int r = 14; r >= 2; r--)
    {
        if (rankCount[r] == 4)
        {
            int kicker = 0;

            for (int k = 14; k >= 2; k--)
            {
                if (k != r && rankCount[k] > 0)
                {
                    kicker = k;
                    break;
                }
            }

            return HandValue(7, {r, kicker});
        }
    }

    int three = 0;
    int pair = 0;

    for (int r = 14; r >= 2; r--)
    {
        if (rankCount[r] >= 3 && three == 0)
        {
            three = r;
        }
        else if (rankCount[r] >= 2 && pair == 0)
        {
            pair = r;
        }
    }

    if (three != 0 && pair != 0)
    {
        return HandValue(6, {three, pair});
    }

    for (int s = 0; s < 4; s++)
    {
        if (suitCards[s].size() >= 5)
        {
            sortDesc(suitCards[s]);

            vector<int> topFive;
            for (int i = 0; i < 5; i++)
            {
                topFive.push_back(suitCards[s][i]);
            }

            return HandValue(5, topFive);
        }
    }

    vector<int> allRanks;

    for (int i = 0; i < cards.size(); i++)
    {
        allRanks.push_back(cards[i].rank);
    }

    int straightHigh = getStraightHigh(allRanks);
    if (straightHigh > 0)
    {
        return HandValue(4, {straightHigh});
    }

    for (int r = 14; r >= 2; r--)
    {
        if (rankCount[r] == 3)
        {
            vector<int> result;
            result.push_back(r);

            for (int k = 14; k >= 2 && result.size() < 3; k--)
            {
                if (k != r && rankCount[k] > 0)
                {
                    result.push_back(k);
                }
            }

            return HandValue(3, result);
        }
    }

    vector<int> pairs;

    for (int r = 14; r >= 2; r--)
    {
        if (rankCount[r] >= 2)
        {
            pairs.push_back(r);
        }
    }

    if (pairs.size() >= 2)
    {
        int kicker = 0;

        for (int k = 14; k >= 2; k--)
        {
            if (k != pairs[0] && k != pairs[1] && rankCount[k] > 0)
            {
                kicker = k;
                break;
            }
        }

        return HandValue(2, {pairs[0], pairs[1], kicker});
    }

    if (pairs.size() == 1)
    {
        vector<int> result;
        result.push_back(pairs[0]);

        for (int k = 14; k >= 2 && result.size() < 4; k--)
        {
            if (k != pairs[0] && rankCount[k] > 0)
            {
                result.push_back(k);
            }
        }

        return HandValue(1, result);
    }

    vector<int> highCards;

    for (int r = 14; r >= 2 && highCards.size() < 5; r--)
    {
        if (rankCount[r] > 0)
        {
            highCards.push_back(r);
        }
    }

    return HandValue(0, highCards);
}
