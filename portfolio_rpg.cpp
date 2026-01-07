#define NOMINMAX//windows.hよりも前に置くこと、マクロの無効化ができる

#include <iostream>
#include <string>
#include <random>
#include <algorithm> // min, max 関数用

#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <limits>//::max()

using namespace std;

//プロトタイプ宣言
void plusMoney(int& playerMoney, int enemyMoneyMin, int enemyMoneyMax);
bool getContinuePlay();

// --- BGM管理用 ---
enum BgmType {
    BGM_STOP,
    BGM_FIELD,
    BGM_BATTLE
};

// BGMを変更する関数 (soundsフォルダを参照)
void changeBgm(BgmType type) {
    static BgmType currentBgm = BGM_STOP;

    // 同じ曲が流れている場合は何もしない
    if (currentBgm == type && type != BGM_STOP) {
        return;
    }

    currentBgm = type;

    switch (type) {
    case BGM_FIELD:
        // soundsフォルダの field.wav を再生
        PlaySound(TEXT("sounds\\field.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        break;
    case BGM_BATTLE:
        // soundsフォルダの battle.wav を再生
        PlaySound(TEXT("sounds\\battle.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        break;
    case BGM_STOP:
    default:
        // 停止
        PlaySound(NULL, 0, 0);
        break;
    }
}
// ----------------

void totle(string word) {
    int num = word.length();
    for (int i = 0; i < num + 6; i++) {
        cout << "+";
    }
    cout << "\n" << "+++" << word << "+++" << endl;
    for (int i = 0; i < num + 6; i++) {
        cout << "+";
    }
    cout << "\n" << endl;
};

class Character
{
public:
    string name;
    int hp;
    int attack;
    int exp;
    int level;

    Character(string name, int hp, int attack, int exp, int level)
    {
        this->name = name;
        this->hp = hp;
        this->attack = attack;
        this->exp = exp;
        this->level = level;
    }

    void getHp(bool& ishp, int playerhp, int enemyhp) {
        string result = "";
        if (playerhp == 0) {
            ishp = false;
            //system("cls");
            result = "敗北";
            totle(result);
        }
        else if (enemyhp == 0) {
            //system("cls");
            ishp = false;
            result = "勝利";
            totle(result);
        }
    }

    void attackCharacter(int& hp, int damage, bool def) {
        //クリティカル判定
        int critical = 0;
        random_device rd;
        mt19937 judge(rd());
        uniform_int_distribution<> distrib(1, 100);
        critical = distrib(judge);
        if (critical <= 5) {
            damage *= 2;
            cout << "クリティカル!" << endl;
        }
        //defence判定
        if (def == true) {
            damage -= (damage / 2);
            //cout << "defence" << endl;
            //cout << damage << endl;
        }
        //ダメージを与える
        hp -= damage;
        if (hp <= 0) {
            hp = 0;
        }
        cout << damage << "のダメージ!" << endl;

        def = false;
    }
};

class Player :public Character
{
public:
    int nextLevelExp;
    int maxHp; // 最大HP
    int money; // 所持金

    Player(string name, int initialHp, int attack, int exp, int level, int initialNextLevelExp, int money)
        : Character(name, initialHp, attack, exp, level)
    {
        this->nextLevelExp = initialNextLevelExp;
        this->maxHp = initialHp; // 最大HPを初期化
        this->money = money; // 所持金を初期化
    }

    void chooseAction(int& playerhp, int& enemyhp, int attack, bool& escape, bool& def, bool& power)
    {
        string Action;
        int selectSkill;
        bool error = false;
        def = false;

        //string str0 = def ? "true" : "false";
        //cout << str0 << endl;

        do {
            cout << "[Attack] 相手にダメージを与える" << endl;
            cout << "[Defence] 3ターン相手からのダメージを減らす" << endl;
            cout << "[Skill] 2種類のスキル" << endl;
            cout << "[Escape] 戦闘を中断して逃げる" << endl;
            cin >> Action;
            system("cls");
            if (Action == "Attack" || Action == "attack") {
                //string str1 = def ? "true" : "false";
                //cout << str1 << endl;

                attackCharacter(enemyhp, attack, def);
                break;
            }
            else if (Action == "Defence" || Action == "defence") {
                def = true;
                cout << "ガードした！" << endl;
                break;
            }
            else if (Action == "Skill" || Action == "skill") { // バグ修正 (|| "skill" -> || Action == "skill")
                selectSkill = chooseSkill();
                if (selectSkill == 1) { //Magic
                    int total = (this->maxHp * 0.3); // 最大HP基準で回復
                    int oldHp = playerhp;
                    playerhp += total;
                    if (playerhp >= this->maxHp) { // 最大HPを超えない
                        playerhp = this->maxHp;
                    }
                    cout << (playerhp - oldHp) << " 回復した" << endl; // 実際の回復量を表示
                    break;
                }
                else if (selectSkill == 2) { //Powerup
                    power = true;
                    cout << "攻撃力が上がった！" << endl;
                    break;
                }
                else if (selectSkill == 0) { //Back
                    cout << "選択し直そう\n" << endl;
                }
                else {
                    cout << "エラー! もう一度入力してください\n" << endl;
                }
            }
            else if (Action == "Escape" || Action == "escape") {
                int rdEscape = 0;
                // 1. 乱数のシード設定
                random_device rd;
                // 2. 乱数エンジンを初期化
                mt19937 gen(rd());
                // 3. 分布を定義 (1から100) -> 1から2に変更
                uniform_int_distribution<> distrib(1, 2);
                rdEscape = distrib(gen);
                if (rdEscape == 1) {
                    escape = true;
                    cout << "うまく逃げ切れた！" << endl;
                }
                else {
                    cout << "逃げきれなかった！" << endl;
                }
                break;
            }
            else {
                cout << "エラー! もう一度入力してください\n" << endl;
            }
        } while (error == false || selectSkill != 0);
    }
    // レベルアップ処理
    void levelUp() {
        while (this->exp >= this->nextLevelExp) {
            this->level++;
            this->maxHp += 10; // 最大HPも増加
            this->hp += 10; // HPを10増加

            // HPが最大HPを超えないように調整
            if (this->hp > this->maxHp) {
                this->hp = this->maxHp;
            }

            this->attack += 2;
            this->exp -= this->nextLevelExp; // 現在の経験値から必要経験値を引く
            this->nextLevelExp = (int)(this->nextLevelExp * 1.5);
            cout << "\n++++++++++++++++++++++++++++++" << endl;
            cout << this->name << "はLV." << this->level << " に上がった！" << endl;
            cout << "++++++++++++++++++++++++++++++\n" << endl;
            cout << "最大HPが 10 増加した。（現在: " << this->maxHp << "）" << endl;
            cout << "攻撃力が 2 増加した。（現在: " << this->attack << "）" << endl;

            // レベルアップ後もまだ経験値が足りていればループ継続
            if (this->exp >= this->nextLevelExp) {
                cout << "次のレベルまであと " << (this->nextLevelExp - this->exp) << " の経験値" << endl;
            }
            else {
                cout << "次のレベルまであと " << (this->nextLevelExp - this->exp) << " の経験値" << endl;
            }
        }
    }
    // 経験値加算処理
    void addExp(int enemyExp)
    {
        cout << this->name << "は " << enemyExp << " の経験値を獲得した！" << endl;
        this->exp += enemyExp;

        if (this->exp >= this->nextLevelExp) {
            levelUp();
        }
        else {
            cout << "次のレベルまであと " << (this->nextLevelExp - this->exp) << " の経験値" << endl;
        }
    }
    void passiveSkill(int hoge) {}

    int chooseSkill() {
        string ability;
        cout << "スキルを選ぼう" << endl;
        cout << "[Magic] 自分のHPを回復する" << endl;
        cout << "[Powerup] 次のターンから2ターン攻撃力を2倍" << endl;
        cout << "[Back] ひとつ前の選択に戻る" << endl;//可能ならひとつ前の選択に戻れるBackを追加
        cin >> ability;
        if (ability == "Magic" || ability == "magic") {
            return 1;
        }
        else if (ability == "Powerup" || ability == "powerup") {
            return 2;
        }
        else if (ability == "Back" || ability == "back") {
            return 0;
        }
        else {
            cout << "エラー! もう一度入力してください\n" << endl;
        }
        return -1; // エラーケース
    }
};

class Enemy :public Character
{
public:
    Enemy(string name, int hp, int attack, int exp, int level)
        : Character(name, hp, attack, exp, level)
    {
    }
};


class BattleManager
{
public:
    BattleManager() {}

    bool startBattle(Player& player, bool debug, int& endingType)
    {
        //戦闘BGM
        changeBgm(BGM_BATTLE);

        //フラグリセット
        bool isHp = true;
        bool escape = false;
        bool defence = false;
        bool power = false;
        int guard = 0;
        int upTurn = 0;

        //敵生成
        string enemyName = "";
        int enemyHp = 0;
        int enemyAttack = 0;
        int enemyExp = 0;
        int enemyLevel = 0;
        int enemyMoneyMin = 0;
        int enemyMoneyMax = 0;

        // 敵タイプ
        int enemyType = 0;
        random_device rd;
        mt19937 ene(rd());
        uniform_int_distribution<> distrib(1, 3);
        enemyType = distrib(ene);

        if (debug == true) {
            enemyType = 1;
            cout << "debug mode" << endl;
        }

        if (enemyType == 1) {
            enemyName = "スライム";
            enemyHp = 20;
            enemyAttack = 5;
            enemyExp = 5;
            enemyLevel = 1;
            enemyMoneyMin = 50;
            enemyMoneyMax = 70;
        }
        else if (enemyType == 2) {
            enemyName = "ゴーレム";
            enemyHp = 100;
            enemyAttack = 20;
            enemyExp = 20;
            enemyLevel = 5;
            enemyMoneyMin = 150;
            enemyMoneyMax = 200;
        }
        else {
            enemyName = "ドラゴン";
            enemyHp = 150;
            enemyAttack = 30;
            enemyExp = 30;
            enemyLevel = 10;
            enemyMoneyMin = 400;
            enemyMoneyMax = 500;
        }

        Enemy enemy(enemyName, enemyHp, enemyAttack, enemyExp, enemyLevel);

        //パッシブスキル（仮）
        player.passiveSkill(player.level);

        cout << enemy.name + "があらわれた！\n" << endl;
        cout << "～戦闘開始～" << endl;

        //--- 戦闘ループ ---
        while ((player.hp > 0) && (enemy.hp > 0)) {
            cout << "現在のHP" << endl;
            cout << player.name << "のHP:" << player.hp << "/" << player.maxHp << endl;
            cout << enemy.name << "のHP:" << enemy.hp << "\n" << endl;

            //Playerのターン
            cout << "【" << player.name << "のターン】" << endl;
            cout << "行動を選ぼう" << endl;

            //行動選択
            player.chooseAction(player.hp, enemy.hp, player.attack, escape, defence, power);

            //Defence処理
            if (defence == true && guard == 0) {
                guard = 3;
            }
            if (guard != 0) {
                defence = true;
                guard -= 1;
            }
            //Powerup処理
            if (power == true && upTurn == 0) {
                player.attack *= 2;
                upTurn = 3;
            }
            if (upTurn != 0) {
                power = true;
                upTurn -= 1;
                if (upTurn == 0) {
                    player.attack /= 2;
                    cout << "Powerupの効果が切れた！" << endl;
                }
            }
            //Escape処理
            if (escape == true) {
                cout << "逃走成功" << endl;
                break;
            }
            //HP判定
            player.getHp(isHp, player.hp, enemy.hp);
            if (isHp == false) break;

            //Enemyのターン
            if (enemy.hp > 0) {
                cout << "【" + enemy.name + "のターン" + "】" << endl;
                cout << enemy.name << "の攻撃!" << endl;
                enemy.attackCharacter(player.hp, enemy.attack, defence);

                //HP判定
                enemy.getHp(isHp, player.hp, enemy.hp);
                if (isHp == false) break;
            }
            power = false;
            cout << "\n" << endl;
        } //戦闘ループ終了

        //--- 戦闘後処理 ---
        bool gameResult = true; //ゲーム継続フラグ

        if (player.hp == 0) { //敗北
            gameResult = false;
            endingType = 1; //gameover
        }
        else if (enemy.hp == 0) { //勝利
            player.addExp(enemy.exp);
            plusMoney(player.money, enemyMoneyMin, enemyMoneyMax);

            //マップに戻るか確認
            if (getContinuePlay() == false) {
                endingType = 10;
                gameResult = false;
            }
        }
        else if (escape == true) { //逃走
            //マップに戻るか確認
            if (getContinuePlay() == false) {
                endingType = 10;
                gameResult = false;
            }
        }

        //BGM制御
        if (gameResult) {
            changeBgm(BGM_FIELD);
        }
        else {
            changeBgm(BGM_STOP);
        }

        return gameResult;
    }
};

//入力を受け取って続けるか否かboolを返す。
bool getContinuePlay() {
    string str = "";
    //system("cls");
    cout << "\nマップ選択に戻りますか?" << endl;

    while (true) {
        cout << "YES : NO" << endl;
        cin >> str;

        if (str == "YES" || str == "yes" || str == "Yes") {
            return true;
        }
        else if (str == "NO" || str == "no" || str == "No") {
            return false;
        }
        else {
            cout << "エラー!もう一度入力してください\n" << endl;
        }
    }
}

bool isDebug(string str) {
    if (str == "debug")
    {
        return true;
    }
    return false;
}
//システムメッセージを表示する関数
void systemMessage(int num) {
    switch (num)
    {
    case 0:
        cout << "error不正な呼び出しです" << endl;
        break;
    case 1:
        cout << "勝利" << endl;
        break;
    case 2:
        cout << "敗北した。" << endl;
        //getContinuePlay();
        break;
    case 3:
        cout << "逃げ切れた" << endl;
        break;
    case 4:
        cout << "game over" << endl;
        break;
    case 10:
        cout << "冒険終わり" << endl;
        break;
    default:
        break;
    }
}

void endCledit(int endingType) {
    int ending = endingType;
    switch (ending)
    {
    case 0:
        cout << "異常終了" << endl;
        break;
    case 1:
        cout << "game over" << endl;
        break;
    case 2:
        cout << "game over" << endl;
        break;
    case 3:
        cout << "game over" << endl;
        break;
    case 4:
        cout << "game over" << endl;
        break;
    case 10:
        cout << "冒険終わり" << endl;
        break;
    default:
        break;
    }
}

void plusMoney(int& playerMoney, int enemyMoneyMin, int enemyMoneyMax) {
    int resultMoney;
    random_device rd;
    mt19937 randomMoney(rd());
    uniform_int_distribution<> distrib(enemyMoneyMin, enemyMoneyMax);
    resultMoney = distrib(randomMoney);
    playerMoney += resultMoney;
    cout << resultMoney << "G を手に入れた！" << endl;
    cout << "所持金額：" << playerMoney << "G" << endl;
    cout << "\n" << endl;
}

void enterShop(Player& player) {
    system("cls");
    totle("ショップ");
    cout << "いらっしゃいませ！" << endl;

    int choice = -1;
    while (choice != 0) {
        cout << "\n現在の所持金: " << player.money << "G" << endl;
        cout << "現在のHP: " << player.hp << "/" << player.maxHp << endl;
        cout << "\n何を購入しますか？" << endl;
        cout << "1: 回復薬 (HP 50 回復) - 30G" << endl;
        cout << "2: すごい回復薬 (HP 全回復) - 100G" << endl;
        cout << "0: 店を出る" << endl;
        cout << "> ";

        cin >> choice;

        int price = 0;
        int recoveryAmount = 0;
        bool fullHeal = false;

        switch (choice) {
        case 1:
            price = 30;
            recoveryAmount = 50;
            break;
        case 2:
            price = 100;
            fullHeal = true;
            break;
        case 0:
            cout << "ありがとうございました！" << endl;
            //system("cls");
            continue; // ループを抜ける
        default:
            cout << "その商品は取り扱っていません。" << endl;

            //system("cls"); // 無効な入力の場合も画面をクリアして再表示
            totle("ショップ");
            cout << "いらっしゃいませ！" << endl;
            continue; // ループの先頭に戻る
        }

        if (player.money >= price) {
            if (player.hp == player.maxHp) {
                cout << "HPはすでに満タンです。" << endl;
            }
            else {
                player.money -= price;

                if (fullHeal) {
                    recoveryAmount = player.maxHp - player.hp;
                    player.hp = player.maxHp;
                }
                else {
                    int actualRecovery = min(recoveryAmount, player.maxHp - player.hp);
                    player.hp += actualRecovery;
                    recoveryAmount = actualRecovery;
                }

                cout << recoveryAmount << " HP回復しました。" << endl;
                cout << "現在のHP: " << player.hp << "/" << player.maxHp << endl;
                cout << "残りの所持金: " << player.money << "G" << endl;
            }
        }
        else {
            cout << "お金が足りません！" << endl;
        }

    }
}


int main()
{
    // マップ(フィールド)BGMを再生
    changeBgm(BGM_FIELD);

    // debug
    bool debug = false;
    // タイトル
    string title = "RPG";
    totle(title);
    cout << "Press Enter to start..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');


    string Playername;

    // 名前入力
    cout << "プレイヤーの名前を入力してください" << endl;
    cin >> Playername;

    // debugモード判定
    debug = isDebug(Playername);
    system("cls");

    int initialHp = 100;
    int initialAttack = 10;
    int initialNextLevelExp = 10; // 次のレベルまでの経験値

    // Playerオブジェクトの作成 (変数名を player に変更)
    Player player(Playername, initialHp, initialAttack, 0, 1, initialNextLevelExp, 100);

    // バトルマネージャーの作成
    BattleManager battleManager;

    // 変数を定義
    bool gameflag = true; // gameを終了するか否かの選択
    int endingType = 0;   // 呼び出すエンディングの種類

    // ゲームスタート (マップ選択ループ)
    while (gameflag == true) {

        // system("cls");
        cout << "------------------------------------------------" << endl;
        cout << player.name << " | LV: " << player.level << " | HP: " << player.hp << "/" << player.maxHp << " | G: " << player.money << endl;
        cout << "------------------------------------------------" << endl;
        cout << "どこへ行きますか？" << endl;
        cout << "1: 森 (戦闘)" << endl;
        cout << "2: ショップ (回復)" << endl;
        cout << "0: 冒険を終了する" << endl;
        cout << "> ";

        int mapChoice;
        cin >> mapChoice;

        if (mapChoice == 1) {
            // --- 戦闘処理 (BattleManagerに委譲) ---
            // startBattleが false を返したらゲーム終了 (敗北 or 逃走後の終了)
            gameflag = battleManager.startBattle(player, debug, endingType);
        }
        else if (mapChoice == 2) {
            // --- ショップ処理 ---
            enterShop(player);
        }
        else if (mapChoice == 0) {
            // --- ゲーム終了 ---
            gameflag = false;
            endingType = 10;
        }
        else {
            cout << "正しい番号を入力してください。" << endl;
        }

    }// game loop end

    if (gameflag == false) {
        endCledit(endingType);
    }

    // 終了時に音を止める
    changeBgm(BGM_STOP);

    system("pause");
    // system("pause > NUL");
    return 0;
}