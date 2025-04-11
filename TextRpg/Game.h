#pragma once
#include "Player.h"
#include "Monster.h"
#include "Item.h"
#include <vector>
#include <chrono>
using namespace std;

const int Map_Height = 9;
const int Map_Width = 15;

class Game
{
public:
    Game();
    virtual ~Game() {};

    void SetCursorPosition(int x, int y);
    void GenerateMap();
    void GenerateWall();
    void GenerateMonster();
    void HandleInventory();
    void HandleEquipInventory();
    void HandleEquipment();
    void HandleInput();
    void Run();
    void QuestClear();

public:
    char map[Map_Height][Map_Width];
    vector<pair<int, int>> _walls;
    Player player;
    vector<Monster> monsters;
    QuestItem questitem;
    BossMonster boss;
    bool GameOver = false;
    bool bossSpawn = false;

private:
    chrono::steady_clock::time_point lastInputTime;
    const chrono::milliseconds inputDelay = chrono::milliseconds(70);

};
