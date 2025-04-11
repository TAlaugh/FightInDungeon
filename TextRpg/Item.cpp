#include "Item.h"
#include "Player.h"
#include <iostream>
using namespace std;

Item::Item()
{

}

Item* Item::GenerateItem()
{
	int randitem = rand() % 5;
	switch (randitem)
	{
	case 0:
		return new Potion();
	case 1:
		return new AtkPotion();
	case 2:
		return new Sword();
	case 3:
		return new Potion();
	case 4:
		return new Armor();
	default:
		break;
	}
}

Potion::Potion() : _addhp(20)
{
	_name = "Potion";
	_itemNum = ItemType::Consumable;
}

void Potion::HealHp(Player& player)
{
	player._hp += _addhp;
}

QuestItem::QuestItem() : _amount(0)
{
	_name = "QuestItem";
	_itemNum = ItemType::QuestItem;
}

AtkPotion::AtkPotion() : _addAtk(3)
{
	_name = "AtkPotion";
	_itemNum = ItemType::Consumable;
}

void AtkPotion::UpAtk(Player* player)
{
	player->_atk += _addAtk;
}

Sword::Sword() : _addAtkStat(10)
{
	_name = "Sword";
	_itemNum = ItemType::Weapon;
}

void Sword::AddAtkStat(Player* player)
{
	player->_atk += _addAtkStat;
}

Armor::Armor() : _addDefStat(5)
{
	_name = "Armor";
	_itemNum = ItemType::Armor;
}

void Armor::AddDefStat(Player* player)
{
	player->_def += _addDefStat;
}

BossPotion::BossPotion() : _addStat(70)
{
	_name = "BossPotion";
	_itemNum = ItemType::Consumable;
}

void BossPotion::UpALL(Player* player)
{
	player->_hp += _addStat;
	player->_def += (_addStat / 2);
	player->_atk += (_addStat / 2);
}