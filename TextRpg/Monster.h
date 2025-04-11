#pragma once

enum class MonsterType
{
    Monster = 1,
    BossMonster = 2
};

const int Max_Monster = 4;
const int Min_Monster = 2;

class Monster
{
public:
    Monster(int X, int Y);
    virtual ~Monster() {}

    bool isDead();
    void takeDamage(int damage);
    void PrintMonsterInfo();

    bool operator==(const Monster& other) const;

public:
    int _x, _y;
    int _hp;
    int _atk;
    int _def;
    MonsterType _monsterType;
};

class BossMonster : public Monster
{
public:
    BossMonster(int x, int y);
    virtual ~BossMonster() {}

};