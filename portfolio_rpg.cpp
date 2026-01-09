#include <iostream>
#include <limits>
#include <string>
#include "Characters.h"
#include "GameSystem.h"

using namespace std;

int main() {
    changeBgm(BGM_FIELD);

    bool debug = false;
    title("RPG");
    cout << "Press Enter to start..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string Playername;
    cout << "プレイヤーの名前を入力してください" << endl;
    cin >> Playername;
    debug = isDebug(Playername);
    system("cls");

    //Player生成
    Player Player(Playername, 100, 10, 0, 1, 10, 100);

    bool gameflag = true;
    int endingType = 0;

    //ゲームループ
    while (gameflag) {
        cout << "------------------------------------------------" << endl;
        cout << Player.name << " | LV: " << Player.level << " | HP: " << Player.hp << "/" << Player.maxHp << " | G: " << Player.money << endl;
        cout << "------------------------------------------------" << endl;
        cout << "どこへ行きますか？\n1: 森 (戦闘)\n2: ショップ (回復)\n0: 冒険を終了する\n> ";

        int mapChoice;
        cin >> mapChoice;

        if (mapChoice == 1) {
            BattleManager battleManager;
            gameflag = battleManager.startBattle(Player, debug, endingType);
            if (!gameflag) changeBgm(BGM_STOP);
        }
        else if (mapChoice == 2) {
            enterShop(Player);
        }
        else if (mapChoice == 0) {
            gameflag = false;
            endingType = 10;
        }
        else {
            cout << "正しい番号を入力してください。" << endl;
        }
    }

    if (!gameflag) endCredit(endingType);

    changeBgm(BGM_STOP);
    system("pause");
    return 0;
}