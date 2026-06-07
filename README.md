<img width="734" height="746" alt="截圖 2026-06-07 下午2 49 23" src="https://github.com/user-attachments/assets/0df33a45-3266-4f1a-9ef6-d24c037a1da8" />
系班級：資工系1A

組別：第17組

成員：梁宜生、劉裕耀、吳玟萱

小專題題目：德州撲克遊戲

【規則及玩法】
本程式是一款文字版德州撲克遊戲。遊戲開始時，玩家需輸入人數、小盲、大盲與起始籌碼。

每位玩家會拿到兩張手牌，並依序經過翻牌前、Flop、Turn、River 四個下注階段。玩家可以選擇跟注、加注、過牌或棄牌。

最後若剩下一位玩家未棄牌，該玩家直接贏得底池；若進入攤牌階段，程式會根據玩家手牌與五張公牌組合成最佳牌型，判斷同花順、鐵支、葫蘆、同花、順子等牌型，並由牌型最大者獲勝。

【程式介紹】
本程式使用 C++ 製作，包含牌組建立、洗牌、發牌、下注、盲注、牌型判斷與勝負結算等功能。

遊戲以文字方式進行，適合多人輪流操作，模擬德州撲克的基本流程。
<img width="734" height="746" alt="截圖 2026-06-07 下午2 49 23" src="https://github.com/user-attachments/assets/102f44af-9698-4d9f-91d8-b5c77e4aca54" />

【程式如何安裝】

1. 安裝 C++ 編譯器（例如 Dev-C++、Code::Blocks 或 Visual Studio）。
2. 將 Holdem.cpp 匯入專案。
3. 編譯並執行程式。
4. 依照畫面提示輸入玩家資訊即可開始遊戲。

第一步：輸入人數

<img width="858" height="494" alt="截圖 2026-06-07 下午2 39 45" src="https://github.com/user-attachments/assets/94eb4aa5-82f8-44bc-b2e6-c36ab6c67762" />

第二步：輸入小盲

<img width="850" height="494" alt="截圖 2026-06-07 下午2 42 12" src="https://github.com/user-attachments/assets/08636950-d4a1-494f-aead-02af445f53a7" />

第三步驟：輸入大盲

<img width="846" height="488" alt="截圖 2026-06-07 下午2 43 20" src="https://github.com/user-attachments/assets/814f23e1-827c-4b8a-890e-10ff8100190e" />

第四步驟：輸入每個人起始點數

<img width="849" height="487" alt="截圖 2026-06-07 下午2 44 16" src="https://github.com/user-attachments/assets/c3050a46-1870-4b18-99ed-aaf318691727" />

第五步驟：輸入個玩家的名稱

<img width="844" height="486" alt="截圖 2026-06-07 下午2 44 51" src="https://github.com/user-attachments/assets/75e6b05f-541b-4598-891f-e9796015338c" />

第六步驟：開始遊玩直到每個人當過大盲時才結束

<img width="832" height="487" alt="截圖 2026-06-07 下午2 45 33" src="https://github.com/user-attachments/assets/6537b170-b245-4528-91ac-c292c9114246" />







【分工資訊】

梁宜生：程式設計與牌型判斷、第一次報告、最終報告 、all

劉裕耀：遊戲流程與測試

吳玟萱：文件撰寫
