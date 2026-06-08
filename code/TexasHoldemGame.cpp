#include "TexasHoldemGame.h"
#include <iostream>
#include <limits>
using namespace std;

TexasHoldemGame::TexasHoldemGame(int sb, int bb)
{
    smallBlind = sb;
    bigBlind = bb;
    pot = 0;
}
// 設立大小盲

void TexasHoldemGame::addPlayer(string name, int startingChips)
{
    players.push_back(Player(name, startingChips));
}
// 新增玩家

int TexasHoldemGame::readInt(string message, int minValue)
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

int TexasHoldemGame::activePlayerCount()
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
// 計算還在局內的人數

int TexasHoldemGame::firstActivePlayer()
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

int TexasHoldemGame::nextIndex(int index)
{
    return (index + 1) % players.size();
}

int TexasHoldemGame::getDealerIndex(int bigBlindIndex)
{
    if (players.size() == 2)
    {
        return nextIndex(bigBlindIndex);
    }

    return (bigBlindIndex - 2 + players.size()) % players.size();
}

int TexasHoldemGame::getSmallBlindIndex(int bigBlindIndex)
{
    if (players.size() == 2)
    {
        return getDealerIndex(bigBlindIndex);
    }

    return (bigBlindIndex - 1 + players.size()) % players.size();
}

void TexasHoldemGame::showCommunityCards()
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

void TexasHoldemGame::showTable()
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

void TexasHoldemGame::pressEnter()
{
    cout << "按 Enter 繼續...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void TexasHoldemGame::hideScreen()
{
    for (int i = 0; i < 25; i++)
    {
        cout << endl;
    }
}

void TexasHoldemGame::postBlind(int playerIndex, int amount)
{
    int paid = players[playerIndex].payChips(amount);
    pot += paid;

    cout << players[playerIndex].getName()
         << " 下注盲注 " << paid << endl;
}

void TexasHoldemGame::dealHoleCards()
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

void TexasHoldemGame::burnCard()
{
    deck.draw();
}

void TexasHoldemGame::dealFlop()
{
    burnCard();
    communityCards.push_back(deck.draw());
    communityCards.push_back(deck.draw());
    communityCards.push_back(deck.draw());
}

void TexasHoldemGame::dealTurnOrRiver()
{
    burnCard();
    communityCards.push_back(deck.draw());
}

void TexasHoldemGame::setNeedsAction(vector<bool> &needsAction, int exceptIndex)
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

bool TexasHoldemGame::hasPlayerNeedAction(vector<bool> needsAction)
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

void TexasHoldemGame::bettingRound(int startIndex, int currentHighestBet)
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

void TexasHoldemGame::resetBets()
{
    for (int i = 0; i < players.size(); i++)
    {
        players[i].resetBet();
    }
}

void TexasHoldemGame::awardOnlyActivePlayer()
{
    int winner = firstActivePlayer();

    if (winner != -1)
    {
        players[winner].winChips(pot);
        cout << players[winner].getName()
             << " 贏得底池 " << pot << endl;
    }
}

void TexasHoldemGame::showdown()
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

void TexasHoldemGame::playOneHand(int round, int bigBlindIndex)
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

void TexasHoldemGame::start()
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
