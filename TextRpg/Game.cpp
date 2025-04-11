#include "Game.h"
#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include <conio.h>
#include "Player.h"
#include "Monster.h"
#include "Battle.h"
#include "Item.h"
#include "Windows.h"
using namespace std;

Game::Game() : player(2, 3, *this), boss(0, 0)
{
	GenerateMonster();
	GenerateWall();
}

void Game::SetCursorPosition(int x, int y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Game::GenerateMap()
{
	// 맵을 생성.
	for (int i = 0; i < Map_Height; ++i)
	{
		for (int j = 0; j < Map_Width; ++j)
		{
			map[i][j] = '.';
		}
	}

	// 보스 몬스터가 생성된 곳에 B
	if (bossSpawn == true)
	{
		map[boss._y][boss._x] = 'B';
	}

	// 저장된 벽 위치에 벽을 추가
	for (const auto& wall : _walls)
	{
		map[wall.first][wall.second] = ' ';
	}

	// 몬스터가 생성된 위치에 M
	for (const auto& monster : monsters)
	{
		map[monster._y][monster._x] = 'M';
	}

	// 플레이어 생성된 위치에 P
	map[player._y][player._x] = 'P';

	// 맵 출력
	for (int i = 0; i < Map_Height; ++i)
	{
		for (int j = 0; j < Map_Width; ++j)
		{
			cout << map[i][j] << " ";
		}
		cout << endl;
	}

}

void Game::GenerateWall()
{
	_walls.clear();
	int wallCount = 15;
	bool wallPositionValid = false;

	while (_walls.size() < wallCount)
	{
		for (int i = 0; i < wallCount; i++)
		{
			int _wx = rand() % Map_Width;
			int _wy = rand() % Map_Height;

			wallPositionValid = true;

			if (player._x == _wx && player._y == _wy)
			{
				wallPositionValid = false;
			}
			else if (boss._x == _wx && boss._y == _wy)
			{
				wallPositionValid = false;
			}
			else
			{
				for (auto& monster : monsters)
				{
					if (monster._x == _wx && monster._y == _wy)
					{
						wallPositionValid = false;
						break;
					}
				}
			}

			if (wallPositionValid)
			{
				_walls.push_back(pair<int, int>(_wy, _wx));
			}
		}
	}
}

void Game::GenerateMonster()
{
	// 랜덤 위치의 2 ~ 4마리의 몬스터 생성
	monsters.clear();
	int monsterCount = (rand() % (Max_Monster - Min_Monster + 1)) + Min_Monster;
	bool monsterPositionValid = false;

	while (!monsterPositionValid)
	{
		for (int i = 0; i < monsterCount; i++)
		{
			int _mx = rand() % Map_Width;
			int _my = rand() % Map_Height;
			monsters.push_back(Monster(_mx, _my));
		}
		
		monsterPositionValid = true;

		for (auto it = monsters.begin(); it != monsters.end(); ++it)
		{
			if (player._x == it->_x && player._y == it->_y)
			{
				monsterPositionValid = false;
				break;
			}
		}

		/*for (auto monster : monsters)
		{
			if (player._x == monster._x && player._y == monster._y)
			{
				monsterPositionValid = false;
				break;
			}
		}*/
	}
	
	// 랜덤 위치의 보스 생성
	if (bossSpawn == true)
	{
		bool bossPositionValid = false;

		while (!bossPositionValid)
		{
			int _bx = rand() % Map_Width;
			int _by = rand() % Map_Height;
			boss = BossMonster(_bx, _by);

			bossPositionValid = true;

			for (auto it = monsters.begin(); it != monsters.end(); ++it)
			{
				if (boss._x == it->_x && boss._y == it->_y)
				{
					bossPositionValid = false;
					break;
				}
			}
		}
	}
}

void Game::HandleInventory()
{
	int itemnumber;
	bool validInput = false;

	player.ShowInventory();
	player.PrintPlayerInfo();

	if (player._inventory.size() != 0)
	{
		while (!validInput)
		{
			cout << "아이템 사용 번호를 눌러주세요 : ";
			if (cin >> itemnumber)
			{
				validInput = true;
			}
			else
			{
				cout << "잘못된 입력입니다."<< endl;
				cin.clear();
				cin.ignore(INT_MAX, '\n');
			}
		}
		player.UseItem(itemnumber);
		cout << "────────────────────────────────────────────────────────────" << endl;
		cout << "아이템 사용 후 플레이어 스탯" << endl;
		player.PrintPlayerInfo();
		cout << endl;
		cout << "--- Enter를 눌러 화면 창을 닫아주세요 ---" << endl;
	}
	else
	{
		cout << "인벤토리에 아이템이 없습니다." << endl;
		cout << endl;
		cout << "--- Enter를 눌러 화면 창을 닫아주세요 ---" << endl;
	}
}
 
void Game::HandleEquipInventory()
{
	int itemnumber;
	bool validInput = false;

	player.ShowEquipInventory();
	player.PrintPlayerInfo();

	if (player._equipInventory.size() != 0)
	{
		while (!validInput)
		{
			cout << "아이템 장착 번호를 눌러주세요 : ";
			if (cin >> itemnumber)
			{
				validInput = true;
			}
			else
			{
				cout << "잘못된 입력입니다. 숫자를 입력해주세요." << endl;
				cin.clear();
				cin.ignore(INT_MAX, '\n');
			}
		}
		player.EquipItem(itemnumber);
		cout << "────────────────────────────────────────────────────────────" << endl;
		cout << "아이템 사용 후 플레이어 스탯" << endl;
		player.PrintPlayerInfo();
		cout << endl;
		cout << "--- Enter를 눌러 화면 창을 닫아주세요 ---" << endl;
	}
	else
	{
		cout << "인벤토리에 아이템이 없습니다." << endl;
		cout << endl;
		cout << "--- Enter를 눌러 화면 창을 닫아주세요 ---" << endl;
	}
}

void Game::HandleEquipment()
{
	player.ShowEquipItem();
	player.PrintPlayerInfo();
	if (player._equip.size() != 0)
	{
		int itemnumber;
		bool validInput = false;

		while (!validInput)
		{
			cout << "장착 해제 할 아이템을 골라주세요 : " << endl;
			if (cin >> itemnumber)
			{
				validInput = true;
			}
			else
			{
				cout << "잘못된 입력입니다. 숫자를 입력해주세요." << endl;
				cin.clear();
				cin.ignore(INT_MAX, '\n');
			}
		}

		player.UnEquipItem(itemnumber);
		cout << "────────────────────────────────────────────────────────────" << endl;
		cout << "장착 해제 후 플레이어 스탯" << endl;
		player.PrintPlayerInfo();
		cout << endl;
		cout << "--- Enter를 눌러 화면 창을 닫아주세요 ---" << endl;
	}
	else
	{
		cout << "장착 중인 아이템이 없습니다." << endl;
		cout << endl;
		cout << "--- Enter를 눌러 화면 창을 닫아주세요 ---" << endl;
	}
}

void Game::HandleInput()
{
	cout << "(w/a/s/d) 이동 / (e) 장착 아이템 " << endl;
	cout << "(i) 사용 인벤토리 / (o) 장비 인벤토리" << endl;

	auto now = chrono::steady_clock::now();

	if (now - lastInputTime >= inputDelay)
	{
		lastInputTime = now; // 마지막 입력 시간을 갱신

		if (_kbhit())
		{
			char input = _getch();

			switch (input)
			{
			case 'w':
				player.Move('w');
				break;
			case 's':
				player.Move('s');
				break;
			case 'a':
				player.Move('a');
				break;
			case 'd':
				player.Move('d');
				break;
			case 'i':
				HandleInventory();
				break;
			case 'o':
				HandleEquipInventory();
				break;
			case 'e':
				HandleEquipment();
				break;
			case 13:
				system("cls");
				break;
			default:
				break;
			}
		}
	}
	
	// 몬스터의 위치와 플레이어의 위치가 같으면 싸움 시작
	for (auto& monster : monsters)
	{
		if (player._x == monster._x && player._y == monster._y)
		{
			Battle::StartBattle(player, monster);
			if (player.isDead())
			{
				GameOver = true;
				return;
			}
			// 아이템 종류에 따라서 다른 인벤토리로 들어감.
			else if (player._hp > 0)
			{
				Item* item = Item::GenerateItem();
				if (item->_itemNum == ItemType::Consumable)
				{
					cout << "* " << item->_name << " * 사용 아이템 획득" << endl;
					// player._inventory.push_back(item);
					player.AddItemtoInventory(item);
				}
				else if (item->_itemNum == ItemType::QuestItem)
				{
					cout << "* " << item->_name << " * 퀘스트 아이템 획득" << endl;
				}
				else if (item->_itemNum == ItemType::Weapon || item->_itemNum == ItemType::Armor)
				{
					cout << "* " << item->_name << " * 장비 아이템 획득" << endl;
					player._equipInventory.push_back(item);
				}
			}
			monsters.erase(remove(monsters.begin(), monsters.end(), monster), monsters.end());
			this_thread::sleep_for(chrono::seconds(2));
			system("cls");
			break;
		}
	}

	if (questitem._amount == 2 && player._x == boss._x && player._y == boss._y)
	{
		Battle::StartBattle(player, boss);  // 보스 몬스터와의 전투
		if (player.isDead())
		{
			GameOver = true;
			return;
		}
		else if (player._hp > 0)
		{
			cout << "* 수상한 포션 *을 얻었습니다." << endl;
			Item* item = new BossPotion();
			player.AddItemtoInventory(item);
			boss._x = -1;
			boss._y = -1;
			bossSpawn = false;
			this_thread::sleep_for(chrono::seconds(2));
			system("cls");
		}

	}

	// 몬스터를 다 잡았으면 다시 생성하고 questitem을 하나 인벤토리에 추가 함.
	if (monsters.empty() && bossSpawn == false)
	{
		Item* questtem = new QuestItem();
		player.AddItemtoInventory(questtem);
		questitem._amount++;
		QuestClear();

		if (questitem._amount == 2)
		{
			bossSpawn = true;
		}

		GenerateMonster();
		GenerateWall();
	}
}

void Game::Run()
{
	while (!GameOver) {
		SetCursorPosition(0, 0);
		GenerateMap();
		SetCursorPosition(0, Map_Height);
		HandleInput();
	}
}

void Game::QuestClear()
{
	if (questitem._amount >= 3)
	{
		cout << "****** QuestClear *******" << endl;
		GameOver = true;
	}
}