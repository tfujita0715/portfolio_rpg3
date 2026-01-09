#pragma once
#include <string>
#include <iostream>

using namespace std;

//戦闘中の状態フラグ
struct BattleState {
    bool isEscape = false;    //逃走フラグ
    bool isDefending = false; //防御フラグ
    bool isPowerUp = false;   //攻撃力バフフラグ
    int guardTurn = 0;        //残防御
    int powerTurn = 0;        //残パワーアップ
};


class Character {
public:
    string name;
    int hp;
    int attack;
    int exp;
    int level;

    Character(string name, int hp, int attack, int exp, int level);
    void getHp(bool& ishp, int playerhp, int enemyhp);
    void attackCharacter(Character& target, int damage, bool def);
};


class Player : public Character {
public:
    int nextLevelExp;
    int maxHp;
    int money;

    Player(string name, int initialHp, int attack, int exp, int level, int initialNextLevelExp, int money);
    void chooseAction(Character& target, BattleState& state);
    void levelUp();
    void addExp(int enemyExp);
    void passiveSkill(int hoge);
    int chooseSkill();
};

class Enemy : public Character {
public:
    Enemy(string name, int hp, int attack, int exp, int level);
};