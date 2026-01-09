#include "Characters.h"
#include <random>
#include <iostream>
#include <algorithm> //min, max用

using namespace std;

//--- Characterの実装 ---
Character::Character(string name, int hp, int attack, int exp, int level) {
    this->name = name;
    this->hp = hp;
    this->attack = attack;
    this->exp = exp;
    this->level = level;
}

void title(string word); 

void Character::getHp(bool& ishp, int playerhp, int enemyhp) {
    string result = "";
    if (playerhp == 0) {
        ishp = false;
        result = "敗北";
        cout << "\n+++" << result << "+++" << endl;
    }
    else if (enemyhp == 0) {
        ishp = false;
        result = "勝利";
        cout << "\n+++" << result << "+++" << endl;
    }
}

void Character::attackCharacter(Character& target, int damage, bool def) {
    int critical = 0;
    random_device rd;
    mt19937 judge(rd());
    uniform_int_distribution<> distrib(1, 100);
    critical = distrib(judge);
    if (critical <= 5) {
        damage *= 2;
        cout << "クリティカル!" << endl;
    }
    if (def == true) {
        damage -= (damage / 2);
    }
    target.hp -= damage;
    if (target.hp <= 0) target.hp = 0;
    cout << damage << "のダメージ!" << endl;
}

Player::Player(string name, int initialHp, int attack, int exp, int level, int initialNextLevelExp, int money)
    : Character(name, initialHp, attack, exp, level) {
    this->nextLevelExp = initialNextLevelExp;
    this->maxHp = initialHp;
    this->money = money;
}

void Player::chooseAction(Character& target, BattleState& state) {
    string Action;
    int selectSkill = 0;
    bool error = false;
    state.isDefending = false;

    do {
        cout << "[Attack] 相手にダメージを与える" << endl;
        cout << "[Defence] 3ターン相手からのダメージを減らす" << endl;
        cout << "[Skill] 2種類のスキル" << endl;
        cout << "[Escape] 戦闘を中断して逃げる" << endl;
        cin >> Action;
        system("cls");

        if (Action == "Attack" || Action == "attack") {
            attackCharacter(target, this->attack, false);
            break;
        }
        else if (Action == "Defence" || Action == "defence") {
            state.isDefending = true;
            cout << "ガードした！" << endl;
            break;
        }
        else if (Action == "Skill" || Action == "skill") {
            selectSkill = chooseSkill();
            if (selectSkill == 1) { //Magic
                int total = (this->maxHp * 0.3);
                int oldHp = this->hp;
                this->hp += total;
                if (this->hp >= this->maxHp) this->hp = this->maxHp;
                cout << (this->hp - oldHp) << " 回復した" << endl;
                break;
            }
            else if (selectSkill == 2) { //Powerup
                state.isPowerUp = true;
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
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> distrib(1, 2);
            int rdEscape = distrib(gen);
            if (rdEscape == 1) {
                state.isEscape = true;
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

void Player::levelUp() {
    while (this->exp >= this->nextLevelExp) {
        this->level++;
        this->maxHp += 10;
        this->hp += 10;
        if (this->hp > this->maxHp) this->hp = this->maxHp;
        this->attack += 2;
        this->exp -= this->nextLevelExp;
        this->nextLevelExp = (int)(this->nextLevelExp * 1.5);

        cout << "\n++++++++++++++++++++++++++++++" << endl;
        cout << this->name << "はLV." << this->level << " に上がった！" << endl;
        cout << "++++++++++++++++++++++++++++++\n" << endl;
    }
}

void Player::addExp(int enemyExp) {
    cout << this->name << "は " << enemyExp << " の経験値を獲得した！" << endl;
    this->exp += enemyExp;
    if (this->exp >= this->nextLevelExp) {
        levelUp();
    }
    else {
        cout << "次のレベルまであと " << (this->nextLevelExp - this->exp) << " の経験値" << endl;
    }
}

void Player::passiveSkill(int hoge) {}

int Player::chooseSkill() {
    string ability;
    cout << "スキルを選ぼう" << endl;
    cout << "[Magic] 自分のHPを回復する" << endl;
    cout << "[Powerup] 次のターンから2ターン攻撃力を2倍" << endl;
    cout << "[Back] ひとつ前の選択に戻る" << endl;
    cin >> ability;
    if (ability == "Magic" || ability == "magic") return 1;
    else if (ability == "Powerup" || ability == "powerup") return 2;
    else if (ability == "Back" || ability == "back") return 0;

    cout << "エラー! もう一度入力してください\n" << endl;
    return -1;
}

Enemy::Enemy(string name, int hp, int attack, int exp, int level)
    : Character(name, hp, attack, exp, level) {
}