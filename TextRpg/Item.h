#pragma once
#include <iostream>
using namespace std;

class Player;

enum class ItemType
{
	Item = 0,
	Consumable = 1,
	QuestItem = 2,
	Weapon = 3,
	Armor = 4
};

class Item
{
public:
	Item();
	virtual ~Item() {}

	static Item* GenerateItem();

public:
	ItemType _itemNum = ItemType::Item;
	string _name = "Item";
};

class Potion : public Item
{
public:
	Potion();
	virtual ~Potion() {}

	void HealHp(Player& player);

public:
	int _addhp;
};

class AtkPotion : public Item
{
public:
	AtkPotion();
	virtual ~AtkPotion() {}

	void UpAtk(Player* player);

public:
	int _addAtk;
};

class BossPotion : public Item
{
public:
	BossPotion();
	virtual ~BossPotion() {}

	void UpALL(Player* player);

public:
	int _addStat;
};

class QuestItem : public Item
{
public:
	QuestItem();
	virtual ~QuestItem() {}

public:
	int _amount;
};

class Sword : public Item
{
public:
	Sword();
	virtual ~Sword() {}

	void AddAtkStat(Player* player);

public:
	int _addAtkStat;
};

class Armor : public Item
{
public:
	Armor();
	virtual ~Armor() {}

	void AddDefStat(Player* player);

public:
	int _addDefStat;
};