#include "CardDeck.h"
#include <cstdlib>
using namespace std;

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
// 把牌面數字轉成玩家看得懂的字串。

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
// 把花色轉成符號

string cardToString(Card card)
{
    return rankToString(card.rank) + suitToString(card.suit);
}
// 把一張牌轉成完整表示

Deck::Deck()
{
    index = 0;
}
// 牌堆建構子

void Deck::reset()
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
// 重新建立完整52張牌

void Deck::shuffle()
{
    for (int i = 0; i < cards.size(); i++)
    {
        int j = rand() % cards.size();
        Card temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }
}

Card Deck::draw()
{
    Card card = cards[index];
    index++;
    return card;
}
// 依序抽取牌張