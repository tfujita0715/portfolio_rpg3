#define NOMINMAX
#include "GameSystem.h"
#include <iostream>
#include <random>
#include <windows.h>
#include <mmsystem.h>
#include <algorithm> // min, max用
#pragma comment(lib, "winmm.lib")

using namespace std;

//音楽
void changeBgm(BgmType type) {
    static BgmType currentBgm = BGM_STOP;
    if (currentBgm == type && type != BGM_STOP) return;
    currentBgm = type;

    switch (type) {
    case BGM_FIELD:
        PlaySound(TEXT("sounds\\field.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        break;
    case BGM_BATTLE:
        PlaySound(TEXT("sounds\\battle.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        break;
    case BGM_STOP:
    default:
        PlaySound(NULL, 0, 0);
        break;
    }
}

void title(string word) {
    int num = word.length();
    for (int i = 0; i < num + 6; i++) cout << "+";
    cout << "\n" << "+++" << word << "+++" << endl;
    for (int i = 0; i < num + 6; i++) cout << "+";
    cout << "\n" << endl;
}

bool getContinuePlay() {
    string str = "";
    cout << "\nマップ選択に戻りますか?" << endl;
    while (true) {
        cout << "YES : NO" << endl;
        cin >> str;
        if (str == "YES" || str == "yes" || str == "Yes") return true;
        else if (str == "NO" || str == "no" || str == "No") return false;
        else cout << "エラー!もう一度入力してください\n" << endl;
    }
}

bool isDebug(string str) {
    return (str == "debug");
}

void systemMessage(int num) {
    switch (num) {
    case 1: cout << "勝利" << endl; break;
    case 2: cout << "敗北した。" << endl; break;
    case 3: cout << "逃げ切れた" << endl; break;
    case 10: cout << "冒険終わり" << endl; break;
    default: break;
    }
}

void endCledit(int endingType) {
    if (endingType == 0) cout << "異常終了" << endl;
    else if (endingType == 10) cout << "冒険終わり" << endl;
    else cout << "game over" << endl;
}

void plusMoney(int& playerMoney, int enemyMoneyMin, int enemyMoneyMax) {
    random_device rd;
    mt19937 randomMoney(rd());
    uniform_int_distribution<> distrib(enemyMoneyMin, enemyMoneyMax);
    int resultMoney = distrib(randomMoney);
    playerMoney += resultMoney;
    cout << resultMoney << "G を手に入れた！" << endl;
    cout << "所持金額：" << playerMoney << "G" << endl;
    cout << "\n" << endl;
}

void enterShop(Player& player) {
    system("cls");
    title("ショップ");
    cout << "いらっしゃいませ！" << endl;
    int choice = -1;
    while (choice != 0) {
        cout << "\n現在の所持金: " << player.money << "G" << endl;
        cout << "現在のHP: " << player.hp << "/" << player.maxHp << endl;
        cout << "1: 回復薬 (30G) / 2: 全回復 (100G) / 0: 出る > ";
        cin >> choice;

        if (choice == 0) {
            cout << "ありがとうございました！" << endl;
            continue;
        }

        int price = (choice == 1) ? 30 : 100;
        int heal = (choice == 1) ? 50 : 9999;

        if (player.money >= price) {
            if (player.hp == player.maxHp) cout << "HPは満タンです。" << endl;
            else {
                player.money -= price;
                int oldHp = player.hp;
                player.hp += heal;
                if (player.hp > player.maxHp) player.hp = player.maxHp;
                cout << (player.hp - oldHp) << " 回復しました。" << endl;
            }
        }
        else {
            cout << "お金が足りません！" << endl;
        }
    }
}

bool BattleManager::startBattle(Player& player, bool debug, int& endingType) {
    changeBgm(BGM_BATTLE);
    BattleState state;
    bool isHp = true;
    bool escape = false;
    bool defence = false;
    bool power = false;
    int guard = 0;
    int upTurn = 0;
    bool isGuardActive = false;

    random_device rd;
    mt19937 ene(rd());
    uniform_int_distribution<> distrib(1, 3);

    int enemyType = distrib(ene);
    if (debug) enemyType = 1;

    string eName; int eHp, eAtk, eExp, eLv, eMin, eMax;
    if (enemyType == 1) { 
        eName = "スライム"; 
        eHp = 20; 
        eAtk = 5; 
        eExp = 5; 
        eLv = 1; 
        eMin = 50; 
        eMax = 70; }
    else if (enemyType == 2) { 
        eName = "ゴーレム"; 
        eHp = 100; 
        eAtk = 20; 
        eExp = 20; 
        eLv = 5; 
        eMin = 150; 
        eMax = 200; }
    else { 
        eName = "ドラゴン"; 
        eHp = 150; 
        eAtk = 30; 
        eExp = 30; 
        eLv = 10; 
        eMin = 400; 
        eMax = 500; }

    Enemy enemy(eName, eHp, eAtk, eExp, eLv);
    player.passiveSkill(player.level);

    cout << enemy.name + "があらわれた！\n" << endl;

    while ((player.hp > 0) && (enemy.hp > 0)) {
        cout << "現在のHP -> " << player.name << ":" << player.hp << " / " << enemy.name << ":" << enemy.hp << endl;

        cout << "【" << player.name << "のターン】" << endl;
        player.chooseAction(enemy, state);

        if (defence && guard == 0) guard = 3;
        if (guard > 0) { defence = true; guard--; }
        if (power && upTurn == 0) { player.attack *= 2; upTurn = 3; }
        if (upTurn > 0) {
            power = true; upTurn--;
            if (upTurn == 0) { player.attack /= 2; cout << "Powerupの効果が切れた！" << endl; }
        }

        if (escape) { cout << "逃走成功" << endl; break; }
        player.getHp(isHp, player.hp, enemy.hp);
        if (!isHp) break;

        if (enemy.hp > 0) {
            cout << "【" + enemy.name + "のターン】" << endl;
            enemy.attackCharacter(player, enemy.attack, isGuardActive);
            enemy.getHp(isHp, player.hp, enemy.hp);
            if (!isHp) break;
        }
        cout << "\n" << endl;
    }

    bool continueGame = true;
    if (player.hp == 0) { endingType = 1; continueGame = false; }
    else if (enemy.hp == 0) {
        player.addExp(enemy.exp);
        plusMoney(player.money, eMin, eMax);
        continueGame = getContinuePlay();
        if (!continueGame) endingType = 10;
    }
    else if (escape) {
        continueGame = getContinuePlay();
        if (!continueGame) endingType = 10;
    }

    changeBgm(BGM_FIELD);
    return continueGame;
}