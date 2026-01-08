#pragma once
#include "Characters.h" // Playerなどを知る必要がある

//BGMタイプ
enum BgmType {
    BGM_STOP,
    BGM_FIELD,
    BGM_BATTLE
};

//バトル管理
class BattleManager {
public:
    BattleManager() {}
    bool startBattle(Player& player, bool debug, int& endingType);
};

//共通関数
void changeBgm(BgmType type);
void title(string word);
bool getContinuePlay();
bool isDebug(string str);
void systemMessage(int num);
void endCledit(int endingType);
void plusMoney(int& playerMoney, int enemyMoneyMin, int enemyMoneyMax);
void enterShop(Player& player);