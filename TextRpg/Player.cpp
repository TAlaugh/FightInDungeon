#include "Player.h"
#include <iostream>
#include "Game.h"
using namespace std;

Player::Player(int startX, int startY, Game& g) : _x(startX), _y(startY), _hp(200), _atk(25), _def(10), _inventory(_inventory), game(g)
{
    skill = make_shared<Skill>(0, "강타");
}

void Player::Move(char direction)
{
    int newX = _x;
    int newY = _y;

    switch (direction)
    {
    case 'w': 
        newY--;
        break;
    case 's': 
        newY++;
        break;
    case 'a': 
        newX--;
        break;
    case 'd': 
        newX++;
        break;
    }

    if (newX >= 0 && newX < Map_Width && newY >= 0 && 
        newY < Map_Height && game.map[newY][newX] != ' ') 
    {
        _x = newX;
        _y = newY;
    }
}

bool Player::isDead()
{
    return _hp <= 0;
}

void Player::takeDamage(int damage)
{
    if (_def <= damage)
        _hp -= (damage - _def);
    else if (_def > damage)
        _hp;
}

void Player::PrintPlayerInfo()
{
    cout << "■ Player hp : " << _hp << " Player atk : " << _atk << " Player def : " << _def << " " << endl;
    cout << "────────────────────────────────────────────────────────────" << endl;
}

void Player::ShowInventory()
{
    cout << endl;
    cout << "────────────────────────────────────────────────────────────" << endl;
    cout << "[사용 인벤토리]" << endl;
    for (int i = 0; i < _inventory.size(); i++)
    {
        cout << i << ". " << _inventory[i]->_name << endl;
    }
    cout << "────────────────────────────────────────────────────────────" << endl;
}

void Player::ShowEquipInventory()
{
    cout << endl;
    cout << "────────────────────────────────────────────────────────────" << endl;
    cout << "[장비 인벤토리]" << endl;
    for (int i = 0; i < _equipInventory.size(); i++)
    {
        cout << i << ". " << _equipInventory[i]->_name << endl;
    }
    cout << "────────────────────────────────────────────────────────────" << endl;
}

void Player::ShowEquipItem()
{
    cout << endl;
    cout << "────────────────────────────────────────────────────────────" << endl;
    cout << "[장착한 아이템]" << endl;
    for (int i = 0; i < _equip.size(); i++)
    {
        cout << i << ". " << _equip[i]->_name << endl;
    }
    cout << "────────────────────────────────────────────────────────────" << endl;
}

// 플레이어가 사용아이템 인벤토리로 가서 아이템 사용 시 실행되는 코드
void Player::UseItem(const int& number)
{
    if (number >= 0 && number < _inventory.size())
    {
        Item* item = _inventory[number];
        if (auto potion = dynamic_cast<Potion*>(item))
        {
            potion->HealHp(*this);
            cout << potion->_name << "을(를) 사용하여 " << potion->_addhp << " 회복했습니다." << endl;
        }
        else if (auto atkpotion = dynamic_cast<AtkPotion*>(item))
        {
            atkpotion->UpAtk(this);
            cout << atkpotion->_name << "을(를) 사용하여 스탯이 " << atkpotion->_addAtk << " 증가했습니다." << endl;
        }
        else if (auto bosspotion = dynamic_cast<BossPotion*>(item))
        {
            bosspotion->UpALL(this);
            cout << "강한 기운이 느껴진다." << endl;
            cout << "능력치가 크게 상승합니다." << endl;
        }
        else if (auto questitem = dynamic_cast<QuestItem*>(item))
        {
            cout << "사용할 수 없는 아이템입니다." << endl;
            return;
        }
        _inventory.erase(_inventory.begin() + number);  // 아이템 사용 후 삭제
        delete item;
    }
}

// 플레이어가 장착아이템 인벤토리로 가서 아이템 장착 시 실행되는 코드
void Player::EquipItem(const int& number)
{
    if (number >= 0 && number < _equipInventory.size())
    {
        Item* item = _equipInventory[number];
        if (item->_itemNum == ItemType::Weapon)
        {
            Sword* sword = dynamic_cast<Sword*>(item);
            if (sword)
            {
                if (EquipWeaponCheck() == false) // 웨폰아이템을 장착하지 않았다면
                {
                    sword->AddAtkStat(this);
                    cout << item->_name << " (+" << sword->_addAtkStat << " ATK) 을 장착했습니다." << endl;
                    _equipInventory.erase(_equipInventory.begin() + number); // 인벤토리에서 지우고
                    _equip.push_back(item); // 장착에 넣기
                }
                else if (EquipWeaponCheck() == true)
                {
                    cout << "이미 " << item->_name << " 아이템을 장착 중입니다." << endl;
                }
            }
        }
        else if (item->_itemNum == ItemType::Armor)
        {
            Armor* armor = dynamic_cast<Armor*>(item);
            if (armor)
            {
                if (EquipArmorCheck() == false)
                {
                    armor->AddDefStat(this);
                    cout << item->_name << " (+" << armor->_addDefStat << " DEF) 을 장착했습니다." << endl;
                    _equipInventory.erase(_equipInventory.begin() + number);
                    _equip.push_back(item);
                }
                else if (EquipArmorCheck() == true)
                {
                    cout << "이미 " << item->_name << " 아이템을 장착 중입니다." << endl;
                }
            }
        }
    }
}

void Player::AddItemtoInventory(Item* item)
{
    _inventory.push_back(item);
}

// 플레이어의 장착 아이템을 해제하는 코드
void Player::UnEquipItem(const int& number)
{
    if (number >= 0 && number < _equip.size())
    {
        Item* item = _equip[number];
        if (item->_itemNum == ItemType::Weapon)
        {
            Sword* sword = dynamic_cast<Sword*>(item);
            if (sword)
            {
                cout << item->_name << " 아이템을 장착 해제 하였습니다." << endl;
                this->_atk -= sword->_addAtkStat;
                _equip.erase(_equip.begin() + number); // 장착에서 지우고
                _equipInventory.push_back(item); // 인벤토리에 다시 넣기
            }
        }
        else if (item->_itemNum == ItemType::Armor)
        {
            Armor* armor = dynamic_cast<Armor*>(item);
            if (armor)
            {
                cout << item->_name << " 아이템을 장착 해제 하였습니다." << endl;
                this->_def -= armor->_addDefStat;
                _equip.erase(_equip.begin() + number);
                _equipInventory.push_back(item);
            }
        }
    }
}

// 장착 인벤토리에서 장착하기 전 플레이어가 이미 장착 중인지 체크
bool Player::EquipWeaponCheck()
{
    for (Item* equip : _equip)
    {
        if (equip->_itemNum == ItemType::Weapon)
            return true;
    }
    return false;
}

// 장착 인벤토리에서 장착하기 전 플레이어가 이미 장착 중인지 체크
bool Player::EquipArmorCheck()
{
    for (Item* equip : _equip)
    {
        if (equip->_itemNum == ItemType::Armor)
            return true;
    }
    return false;
}

void Player::useSkill()
{
    if (skill->SkillCool() == false) {
        cout << skill->_skillName << " 사용!" << endl;
        skill->ResetCoolTime();
    }
    else {
        cout << skill->_skillName << "의 쿨타임이 " << skill->_coolTime << "턴 남았습니다." << endl;
    }
}

bool Skill::SkillCool()
{
    if (_coolTime > 0) {
        return true;
    }
    return false;
}

void Skill::ResetCoolTime()
{
    _coolTime = 3;
}
