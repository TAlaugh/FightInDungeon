#pragma once
#include "Item.h"
#include <memory>
#include <vector>

class Game;
class Skill;

class Player
{
public:
    Player(int startX, int startY, Game& g);
    virtual ~Player()
    {
        for (Item* item : _inventory)
        {
            delete item;  // _inventory 내의 Item 객체들을 해제
        }
        for (Item* item : _equipInventory)
        {
            delete item;
        }
        for (Item* item : _equip)
        {
            delete item;
        }
    }

    void Move(char direction);
    bool isDead();
    void takeDamage(int damage);
    void PrintPlayerInfo();
    void ShowInventory();
    void ShowEquipInventory();
    void ShowEquipItem();
    void UseItem(const int& number);
    void EquipItem(const int& number);
    void AddItemtoInventory(Item* item);
    void UnEquipItem(const int& number);
    bool EquipWeaponCheck();
    bool EquipArmorCheck();
    void useSkill();

public:
    int _x, _y;
    int _hp;
    int _atk;
    int _def;
    vector<Item*> _inventory;
    vector<Item*> _equipInventory;
    vector<Item*> _equip;
    Game& game;
    shared_ptr<Skill> skill;
};

class Skill
{
public:
    Skill(int cooltime, string skillname) : _coolTime(cooltime), _skillName(skillname) {}
    ~Skill() {}

    bool SkillCool();
    void ResetCoolTime();

public:
    int _coolTime = 2;
    string _skillName = "강타";
};