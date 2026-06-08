#include "TexasHoldemGame.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

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
