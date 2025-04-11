#pragma once

class Player;
class Monster;

class Battle
{
public:
    static void SetCursorPosition(int x, int y);
    static void StartBattle(Player& player, Monster& monster);
    static void PlayerBehavior(Player& player, Monster& monster);
    static void BattleDraw(int a);
    static void BossBattleDraw(int b);
};