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

class BattleManager 
{
public:
    BattleManager() {}
    bool startBattle(Player& player, bool debug, int& endingType) 
    {
        //bgm変更
        changeBgm(BGM_BATTLE);
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

    //debug
    bool debug = false;
    //タイトル
    string title = "RPG";
    totle(title);
    cout << "Press Enter to start..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');


    string Playername;

    //名前入力
    cout << "プレイヤーの名前を入力してください" << endl;
    cin >> Playername;
    //debugモード.debugと打つとスライム固定にします。
    debug = isDebug(Playername);
    system("cls");
    int initialHp = 100;
    int initialAttack = 10;
    int initialNextLevelExp = 10;//次のレベルまでの経験値
    // Player(名前, 初期HP, 初期攻撃力, 初期EXP, 初期レベル, 次のEXP, 初期所持金)
    Player Player(Playername, initialHp, initialAttack, 0, 1, initialNextLevelExp, 100);
    int enemyType = 0;


    //変数を定義
    bool gameflag = true; //gameを終了するか否かの選択
    bool escape = false;
    bool isHp = true;
    bool defence = false;
    bool power = false;
    int level = 0;
    float levelCorrection = 1.0;//レベルに応じて敵に補正をかけるための変数
    int guard = 0;
    int upTurn = 0;
    int endingType = 0;//呼び出すエンディングの種類、0の時は異常終了


    //ゲームスタート (マップ選択ループ)
    while (gameflag == true) {

        //system("cls");
        cout << "------------------------------------------------" << endl;
        cout << Player.name << " | LV: " << Player.level << " | HP: " << Player.hp << "/" << Player.maxHp << " | G: " << Player.money << endl;
        cout << "------------------------------------------------" << endl;
        cout << "どこへ行きますか？" << endl;
        cout << "1: 森 (戦闘)" << endl;
        cout << "2: ショップ (回復)" << endl;
        cout << "0: 冒険を終了する" << endl;
        cout << "> ";

        int mapChoice;
        cin >> mapChoice;

        if (mapChoice == 1) {
            // --- 戦闘処理 ---
            // 戦闘BGMに変更
            changeBgm(BGM_BATTLE);

            //flag reset
            isHp = true;
            escape = false;
            // "sound.wav" というWAVファイルを非同期で再生する
            //PlaySound(TEXT("sound.wav"), NULL, SND_FILENAME | SND_ASYNC);

            string enemyName = "";
            int enemyHp = 0;
            int enemyAttack = 0;
            int enemyExp = 0;
            int enemyLevel = 0;
            int enemyMoneyMin = 0;
            int enemyMoneyMax = 0;

            // 1. 乱数のシード設定
            random_device rd;
            // 2. 乱数エンジンを初期化
            mt19937 ene(rd());
            // 3. 分布を定義
            uniform_int_distribution<> distrib(1, 3);
            enemyType = distrib(ene);

            //string debugStr = debug? "true" : "false";cout << debugStr << endl;

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
            //cout << enemyName << endl;

            Enemy Enemy(enemyName, enemyHp, enemyAttack, enemyExp, enemyLevel);
            //レベルアップ時に戦闘が始まったときに回復するパッシブいれたい、、、、関数だけ置いておく
            Player.passiveSkill(level);
            //continue処理

            cout << Enemy.name + "があらわれた！\n" << endl;
            cout << "～戦闘開始～" << endl;

            while ((Player.hp > 0) && (Enemy.hp > 0)) {
                cout << "現在のHP" << endl;
                cout << Player.name << "のHP:" << Player.hp << "/" << Player.maxHp << endl;
                cout << Enemy.name << "のHP:" << Enemy.hp << "\n" << endl;

                //Playerのターン
                cout << "【" << Player.name << "のターン】" << endl;

                cout << "行動を選ぼう" << endl;
                //行動の選択
                Player.chooseAction(Player.hp, Enemy.hp, Player.attack, escape, defence, power);

                //Defenceを選択した場合
                if (defence == true && guard == 0) {
                    guard = 3;
                    //cout << guard << endl;
                }
                if (guard != 0) {
                    defence = true;
                    guard -= 1;
                    //cout << guard << endl;
                }
                //Powerupを選択した場合
                if (power == true && upTurn == 0) {
                    Player.attack *= 2;
                    upTurn = 3;
                    //cout << upTurn << endl;
                }
                if (upTurn != 0) {
                    power = true;
                    upTurn -= 1;
                    //cout << upTurn << endl;
                    if (upTurn == 0) {
                        Player.attack /= 2;
                        cout << "Powerupの効果が切れた！" << endl;
                    }
                }
                //Escapeを選択した場合
                if (escape == true) {
                    cout << "逃走成功" << endl;
                    break;
                }
                //HPの判断
                Player.getHp(isHp, Player.hp, Enemy.hp);
                if (isHp == false) {
                    break;
                }

                //Enemyのターン
                if (Enemy.hp > 0) {
                    cout << "【" + Enemy.name + "のターン" + "】" << endl;

                    //Enemyの攻撃
                    cout << Enemy.name << "の攻撃!" << endl;
                    Enemy.attackCharacter(Player.hp, Enemy.attack, defence);
                    //HPの判断
                    Enemy.getHp(isHp, Player.hp, Enemy.hp);
                    if (isHp == false) {
                        break;
                    }
                }
                power = false;

                cout << "\n" << endl;
            }//戦闘ループここまで

            // --- 戦闘後処理 ---
            if (Player.hp == 0) {// 敗北
                gameflag = false;
                endingType = 1;//gameover
            }
            else if (Enemy.hp == 0) {// 勝利
                Player.addExp(Enemy.exp); // 経験値
                plusMoney(Player.money, enemyMoneyMin, enemyMoneyMax);
                gameflag = getContinuePlay();
                if (gameflag == false) {
                    endingType = 10;
                }
            }
            else if (escape == true) {// 逃走経験値なし
                gameflag = getContinuePlay();
                if (gameflag == false) {
                    endingType = 10;
                }
            }

            // マップに戻るか、ゲーム終了かによってBGM制御
            if (gameflag) {
                changeBgm(BGM_FIELD);
            }
            else {
                changeBgm(BGM_STOP);
            }
        }
        else if (mapChoice == 2) {
            // --- ショップ処理 ---
            enterShop(Player);
            // ショップから戻ってきたらループの先頭（マップ選択）に戻る
        }
        else if (mapChoice == 0) {
            // --- ゲーム終了 ---
            gameflag = false;
            endingType = 10;
        }
        else {
            cout << "正しい番号を入力してください。" << endl;
        }

    }//game loop end

    if (gameflag == false) {
        endCledit(endingType);
    }

    // 終了時に音を止める
    changeBgm(BGM_STOP);

    system("pause");
    //system("pause > NUL");
    return 0;
}