#include "Monster.h"
#include <iostream>
using namespace std;

Monster::Monster(int X, int Y) : _x(X), _y(Y), _hp(0), _atk(0), _def(0)
{
    const int Max_Hp = 70;
    const int Min_Hp = 40;
    _hp = (rand() % (Max_Hp - Min_Hp) + 1) + Min_Hp;

    const int Max_Def = 5;
    const int Min_Def = 0;
    _def = (rand() % (Max_Def - Min_Def) + 1) + Min_Def;

    const int Max_Damage = 25;
    const int Min_Damage = 15;
    _atk = (rand() % (Max_Damage - Min_Damage) + 1) + Min_Damage;

    _monsterType = MonsterType::Monster;
}

bool Monster::isDead()
{
    return _hp <= 0;
}

void Monster::takeDamage(int damage)
{
    if (_def <= damage)
        _hp -= (damage - _def);
    else if (_def > damage)
        _hp;
}

void Monster::PrintMonsterInfo()
{
    if (isDead())
        _hp = 0;
    cout << "■ Monster hp : " << _hp << " Monster atk : " << _atk << " Monster def : " << _def << endl;
    cout << "────────────────────────────────────────────────────────────" << endl;
}

bool Monster::operator==(const Monster& other) const
{
    return (_x == other._x && _y == other._y);
}

BossMonster::BossMonster(int x, int y) : Monster(x, y)
{
    _hp = 150;
    _atk = 30;
    _def = 7;
    _monsterType = MonsterType::BossMonster;
}