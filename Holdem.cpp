#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
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
    int rank; // 2~14，11=J，12=Q，13=K，14=A
    Suit suit;
};

string rankToString(int rank)
{
    if (rank == 11)
    {
        return "J";
    }
    if (rank == 12)
    {
        return "Q";
    }
    if (rank == 13)
    {
        return "K";
    }
    if (rank == 14)
    {
        return "A";
    }
    return to_string(rank);
}

string suitToString(Suit suit)
{
    if (suit == CLUBS)
    {
        return "♣";
    }
    if (suit == DIAMONDS)
    {
        return "♦";
    }
    if (suit == HEARTS)
    {
        return "♥";
    }
    return "♠";
}

string cardToString(Card card)
{
    return rankToString(card.rank) + suitToString(card.suit);
}

class HandValue
{
public:
    int type;
    vector<int> ranks;

    HandValue(int t = 0, vector<int> r = {})
    {
        type = t;
        ranks = r;
    }

    bool operator>(const HandValue &other) const
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

    bool operator==(const HandValue &other) const
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

    string getTypeName() const
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
};

class PokerHandEvaluator
{
private:
    static void sortDesc(vector<int> &nums)
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

    static void sortAsc(vector<int> &nums)
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

    static bool contains(vector<int> nums, int value)
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

    static vector<int> removeDuplicate(vector<int> nums)
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

    static int getStraightHigh(vector<int> ranks)
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

public:
    static HandValue evaluate(vector<Card> cards)
    {
        vector<int> rankCount(15, 0);
        vector<vector<int> > suitCards(4);

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
};

class Deck
{
private:
    vector<Card> cards;
    int index;

public:
    Deck()
    {
        index = 0;
    }

    void reset()
    {
        cards.clear();
        index = 0;

        for (int s = 0; s < 4; s++)
        {
            for (int r = 2; r <= 14; r++)
            {
                Card card;
                card.rank = r;
                card.suit = (Suit)s;
                cards.push_back(card);
            }
        }
    }

    void shuffle()
    {
        for (int i = 0; i < cards.size(); i++)
        {
            int j = rand() % cards.size();
            Card temp = cards[i];
            cards[i] = cards[j];
            cards[j] = temp;
        }
    }

    Card draw()
    {
        Card card = cards[index];
        index++;
        return card;
    }
};

class Player
{
private:
    string name;
    int chips;
    vector<Card> holeCards;
    bool folded;
    int currentBet;

public:
    Player(string n = "", int startingChips = 0)
    {
        name = n;
        chips = startingChips;
        folded = false;
        currentBet = 0;
    }

    void resetForNewHand()
    {
        holeCards.clear();
        folded = false;
        currentBet = 0;
    }

    void resetBet()
    {
        currentBet = 0;
    }

    void addHoleCard(Card card)
    {
        holeCards.push_back(card);
    }

    int payChips(int amount)
    {
        if (amount > chips)
        {
            amount = chips;
        }

        chips -= amount;
        currentBet += amount;
        return amount;
    }

    void winChips(int amount)
    {
        chips += amount;
    }

    void fold()
    {
        folded = true;
    }

    string getName() const
    {
        return name;
    }

    int getChips() const
    {
        return chips;
    }

    int getCurrentBet() const
    {
        return currentBet;
    }

    bool isFolded() const
    {
        return folded;
    }

    bool canPlay() const
    {
        return chips > 0;
    }

    vector<Card> getHoleCards() const
    {
        return holeCards;
    }

    void showHoleCards() const
    {
        cout << name << " 的手牌：";
        for (int i = 0; i < holeCards.size(); i++)
        {
            cout << cardToString(holeCards[i]) << " ";
        }
        cout << endl;
    }
};

class TexasHoldemGame
{
private:
    vector<Player> players;
    vector<Card> communityCards;
    Deck deck;
    int smallBlind;
    int bigBlind;
    int pot;

    int readInt(string message, int minValue)
    {
        int value;

        while (true)
        {
            cout << message;
            cin >> value;

            if (!cin.fail() && value >= minValue)
            {
                return value;
            }

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "輸入錯誤，請重新輸入。" << endl;
        }
    }

    int activePlayerCount()
    {
        int count = 0;

        for (int i = 0; i < players.size(); i++)
        {
            if (!players[i].isFolded() && players[i].getHoleCards().size() == 2)
            {
                count++;
            }
        }

        return count;
    }

    int firstActivePlayer()
    {
        for (int i = 0; i < players.size(); i++)
        {
            if (!players[i].isFolded() && players[i].getHoleCards().size() == 2)
            {
                return i;
            }
        }

        return -1;
    }

    int nextIndex(int index)
    {
        return (index + 1) % players.size();
    }

    int getDealerIndex(int bigBlindIndex)
    {
        if (players.size() == 2)
        {
            return nextIndex(bigBlindIndex);
        }

        return (bigBlindIndex - 2 + players.size()) % players.size();
    }

    int getSmallBlindIndex(int bigBlindIndex)
    {
        if (players.size() == 2)
        {
            return getDealerIndex(bigBlindIndex);
        }

        return (bigBlindIndex - 1 + players.size()) % players.size();
    }

    void showCommunityCards()
    {
        cout << "公牌：";

        if (communityCards.size() == 0)
        {
            cout << "尚未發出";
        }
        else
        {
            for (int i = 0; i < communityCards.size(); i++)
            {
                cout << cardToString(communityCards[i]) << " ";
            }
        }

        cout << endl;
    }

    void showTable()
    {
        cout << "\n目前底池：" << pot << endl;
        showCommunityCards();

        for (int i = 0; i < players.size(); i++)
        {
            cout << players[i].getName()
                 << " 籌碼：" << players[i].getChips();

            if (players[i].isFolded())
            {
                cout << "（已棄牌）";
            }

            cout << endl;
        }
    }

    void pressEnter()
    {
        cout << "按 Enter 繼續...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    void hideScreen()
    {
        for (int i = 0; i < 25; i++)
        {
            cout << endl;
        }
    }

    void postBlind(int playerIndex, int amount)
    {
        int paid = players[playerIndex].payChips(amount);
        pot += paid;

        cout << players[playerIndex].getName()
             << " 下注盲注 " << paid << endl;
    }

    void dealHoleCards()
    {
        for (int round = 0; round < 2; round++)
        {
            for (int i = 0; i < players.size(); i++)
            {
                if (players[i].canPlay())
                {
                    players[i].addHoleCard(deck.draw());
                }
            }
        }
    }

    void burnCard()
    {
        deck.draw();
    }

    void dealFlop()
    {
        burnCard();
        communityCards.push_back(deck.draw());
        communityCards.push_back(deck.draw());
        communityCards.push_back(deck.draw());
    }

    void dealTurnOrRiver()
    {
        burnCard();
        communityCards.push_back(deck.draw());
    }

    void setNeedsAction(vector<bool> &needsAction, int exceptIndex)
    {
        for (int i = 0; i < players.size(); i++)
        {
            needsAction[i] = false;

            if (i != exceptIndex && !players[i].isFolded() && players[i].canPlay())
            {
                needsAction[i] = true;
            }
        }
    }

    bool hasPlayerNeedAction(vector<bool> needsAction)
    {
        for (int i = 0; i < needsAction.size(); i++)
        {
            if (needsAction[i])
            {
                return true;
            }
        }

        return false;
    }

    void bettingRound(int startIndex, int currentHighestBet)
    {
        vector<bool> needsAction(players.size(), false);
        setNeedsAction(needsAction, -1);

        int index = startIndex;

        while (activePlayerCount() > 1 && hasPlayerNeedAction(needsAction))
        {
            if (!needsAction[index] || players[index].isFolded() || !players[index].canPlay())
            {
                index = nextIndex(index);
                continue;
            }

            showTable();
            cout << "\n輪到 " << players[index].getName() << endl;
            cout << "請只讓 " << players[index].getName() << " 看手牌。" << endl;
            players[index].showHoleCards();

            int callAmount = currentHighestBet - players[index].getCurrentBet();

            if (callAmount <= 0)
            {
                cout << "可選動作：1. 過牌  2. 加注  3. 棄牌" << endl;
                int action = readInt("請選擇：", 1);

                while (action < 1 || action > 3)
                {
                    action = readInt("請輸入 1~3：", 1);
                }

                if (action == 1)
                {
                    cout << players[index].getName() << " 過牌。" << endl;
                    needsAction[index] = false;
                }
                else if (action == 2)
                {
                    int maxBet = players[index].getCurrentBet() + players[index].getChips();
                    int newBet = readInt("請輸入加注後的總下注額：", currentHighestBet + bigBlind);

                    while (newBet > maxBet)
                    {
                        cout << "你的籌碼不夠，最多只能下注到 " << maxBet << endl;
                        newBet = readInt("請重新輸入總下注額：", currentHighestBet + bigBlind);
                    }

                    int payAmount = newBet - players[index].getCurrentBet();
                    pot += players[index].payChips(payAmount);
                    currentHighestBet = newBet;

                    cout << players[index].getName()
                         << " 加注到 " << currentHighestBet << endl;

                    setNeedsAction(needsAction, index);
                }
                else
                {
                    players[index].fold();
                    cout << players[index].getName() << " 棄牌。" << endl;
                    needsAction[index] = false;

                    if (activePlayerCount() == 1)
                    {
                        return;
                    }
                }
            }
            else
            {
                cout << "目前需要跟注：" << callAmount << endl;
                cout << "可選動作：1. 跟注  2. 加注  3. 棄牌" << endl;
                int action = readInt("請選擇：", 1);

                while (action < 1 || action > 3)
                {
                    action = readInt("請輸入 1~3：", 1);
                }

                if (action == 1)
                {
                    int paid = players[index].payChips(callAmount);
                    pot += paid;
                    cout << players[index].getName() << " 跟注 " << paid << endl;
                    needsAction[index] = false;
                }
                else if (action == 2)
                {
                    int maxBet = players[index].getCurrentBet() + players[index].getChips();
                    int newBet = readInt("請輸入加注後的總下注額：", currentHighestBet + bigBlind);

                    while (newBet > maxBet)
                    {
                        cout << "你的籌碼不夠，最多只能下注到 " << maxBet << endl;
                        newBet = readInt("請重新輸入總下注額：", currentHighestBet + bigBlind);
                    }

                    int payAmount = newBet - players[index].getCurrentBet();
                    pot += players[index].payChips(payAmount);
                    currentHighestBet = newBet;

                    cout << players[index].getName()
                         << " 加注到 " << currentHighestBet << endl;

                    setNeedsAction(needsAction, index);
                }
                else
                {
                    players[index].fold();
                    cout << players[index].getName() << " 棄牌。" << endl;
                    needsAction[index] = false;

                    if (activePlayerCount() == 1)
                    {
                        return;
                    }
                }
            }

            pressEnter();
            hideScreen();
            index = nextIndex(index);
        }
    }

    void resetBets()
    {
        for (int i = 0; i < players.size(); i++)
        {
            players[i].resetBet();
        }
    }

    void awardOnlyActivePlayer()
    {
        int winner = firstActivePlayer();

        if (winner != -1)
        {
            players[winner].winChips(pot);
            cout << players[winner].getName()
                 << " 贏得底池 " << pot << endl;
        }
    }

    void showdown()
    {
        vector<int> winners;
        HandValue bestHand;
        bool first = true;

        cout << "\n攤牌！" << endl;
        showCommunityCards();

        for (int i = 0; i < players.size(); i++)
        {
            if (players[i].isFolded() || players[i].getHoleCards().size() != 2)
            {
                continue;
            }

            vector<Card> sevenCards = players[i].getHoleCards();

            for (int j = 0; j < communityCards.size(); j++)
            {
                sevenCards.push_back(communityCards[j]);
            }

            HandValue value = PokerHandEvaluator::evaluate(sevenCards);

            players[i].showHoleCards();
            cout << "牌型：" << value.getTypeName() << endl;

            if (first || value > bestHand)
            {
                winners.clear();
                winners.push_back(i);
                bestHand = value;
                first = false;
            }
            else if (value == bestHand)
            {
                winners.push_back(i);
            }
        }

        int share = pot / winners.size();
        int remainder = pot % winners.size();

        for (int i = 0; i < winners.size(); i++)
        {
            int amount = share;

            if (i == 0)
            {
                amount += remainder;
            }

            players[winners[i]].winChips(amount);
            cout << players[winners[i]].getName()
                 << " 贏得 " << amount << endl;
        }
    }

    void playOneHand(int round, int bigBlindIndex)
    {
        int dealerIndex = getDealerIndex(bigBlindIndex);
        int smallBlindIndex = getSmallBlindIndex(bigBlindIndex);

        communityCards.clear();
        pot = 0;

        for (int i = 0; i < players.size(); i++)
        {
            players[i].resetForNewHand();
        }

        deck.reset();
        deck.shuffle();

        cout << "\n==============================" << endl;
        cout << "第 " << round << " 局" << endl;
        cout << "莊家 Button：" << players[dealerIndex].getName() << endl;
        cout << "小盲 SB：" << players[smallBlindIndex].getName() << endl;
        cout << "大盲 BB：" << players[bigBlindIndex].getName() << endl;
        cout << "==============================" << endl;

        postBlind(smallBlindIndex, smallBlind);
        postBlind(bigBlindIndex, bigBlind);
        dealHoleCards();

        cout << "\n翻牌前下注" << endl;
        bettingRound(nextIndex(bigBlindIndex), bigBlind);

        if (activePlayerCount() == 1)
        {
            awardOnlyActivePlayer();
            return;
        }

        resetBets();
        dealFlop();
        cout << "\nFlop 翻牌" << endl;
        bettingRound(nextIndex(dealerIndex), 0);

        if (activePlayerCount() == 1)
        {
            awardOnlyActivePlayer();
            return;
        }

        resetBets();
        dealTurnOrRiver();
        cout << "\nTurn 轉牌" << endl;
        bettingRound(nextIndex(dealerIndex), 0);

        if (activePlayerCount() == 1)
        {
            awardOnlyActivePlayer();
            return;
        }

        resetBets();
        dealTurnOrRiver();
        cout << "\nRiver 河牌" << endl;
        bettingRound(nextIndex(dealerIndex), 0);

        if (activePlayerCount() == 1)
        {
            awardOnlyActivePlayer();
            return;
        }

        showdown();
    }

public:
    TexasHoldemGame(int sb, int bb)
    {
        smallBlind = sb;
        bigBlind = bb;
        pot = 0;
    }

    void addPlayer(string name, int startingChips)
    {
        players.push_back(Player(name, startingChips));
    }

    void start()
    {
        for (int i = 0; i < players.size(); i++)
        {
            playOneHand(i + 1, i);
        }

        cout << "\n遊戲結束：每位玩家都當過一次大盲。" << endl;
        cout << "\n最後籌碼：" << endl;

        for (int i = 0; i < players.size(); i++)
        {
            cout << players[i].getName()
                 << "：" << players[i].getChips() << endl;
        }
    }
};

int main()
{
    srand(time(0));

    int playerCount;
    int smallBlind;
    int bigBlind;
    int startingChips;

    cout << "請輸入玩家人數：";
    cin >> playerCount;

    while (playerCount < 2)
    {
        cout << "玩家人數至少要 2 人，請重新輸入：";
        cin >> playerCount;
    }

    cout << "請輸入小盲金額：";
    cin >> smallBlind;

    cout << "請輸入大盲金額：";
    cin >> bigBlind;

    cout << "請輸入每位玩家起始籌碼：";
    cin >> startingChips;

    while (startingChips < bigBlind * 10)
    {
        cout << "起始籌碼建議至少是大盲的 10 倍，請重新輸入：";
        cin >> startingChips;
    }

    TexasHoldemGame game(smallBlind, bigBlind);

    for (int i = 0; i < playerCount; i++)
    {
        string name;
        cout << "請輸入第 " << i + 1 << " 位玩家名稱：";
        cin >> name;
        game.addPlayer(name, startingChips);
    }

    game.start();

    return 0;
}
