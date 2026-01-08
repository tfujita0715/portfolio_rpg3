#pragma once
#include <string>
#include <iostream>

using namespace std;

class Character {
public:
    string name;
    int hp;
    int attack;
    int exp;
    int level;

    Character(string name, int hp, int attack, int exp, int level);
    void getHp(bool& ishp, int playerhp, int enemyhp);
    void attackCharacter(int& hp, int damage, bool def);
};

class Player : public Character {
public:
    int nextLevelExp;
    int maxHp;
    int money;

    Player(string name, int initialHp, int attack, int exp, int level, int initialNextLevelExp, int money);
    void chooseAction(int& playerhp, int& enemyhp, int attack, bool& escape, bool& def, bool& power);
    void levelUp();
    void addExp(int enemyExp);
    void passiveSkill(int hoge);
    int chooseSkill();
};

class Enemy : public Character {
public:
    Enemy(string name, int hp, int attack, int exp, int level);
};