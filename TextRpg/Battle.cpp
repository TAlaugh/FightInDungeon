#include "Battle.h"
#include "Player.h"
#include "Monster.h"
#include <iostream>
#include <thread>
#include <conio.h>
#include <Windows.h>
#include <chrono>
using namespace std;

void Battle::SetCursorPosition(int x, int y)
{
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Battle::StartBattle(Player& player, Monster& monster)
{
    system("cls");

    while (player._hp > 0 && monster._hp > 0)
    {
        PlayerBehavior(player, monster);
    }
}

void Battle::PlayerBehavior(Player& player, Monster& monster)
{
    // 플레이어 스킬 쿨타임 감소
    if (player.skill->SkillCool() == true) {
        player.skill->_coolTime--;
    }

    // 플레이어 행동
    if (monster._monsterType == MonsterType::Monster)
    {
        BattleDraw(1);
    }
    else if (monster._monsterType == MonsterType::BossMonster)
    {
        BossBattleDraw(1);
    }

    int input;
    int selectedOption = 1;
    bool validinput = false;

    while (!validinput)
    {
        SetCursorPosition(0, 15);
        player.PrintPlayerInfo();
        monster.PrintMonsterInfo();

        cout << "■■■■■■■■■■■■■■■■" << endl;
        for (int i = 1; i <= 3; ++i)
        {
            if (i == selectedOption) {
                cout << "\033[30m";
                cout << "\033[47m";
            }
            if (i == 1) {
                cout << "■ 공격                       ■" << endl;
            }
            else if (i == 2) {
                cout << "■ 아이템 사용                ■" << endl;
            }
            else {
                cout << "■ 강타 사용 | " << player.skill->_coolTime << "턴 후 가능    ■" << endl;
            }
            if (i == selectedOption) {
                cout << "\033[0m";
            }
        }
        cout << "■■■■■■■■■■■■■■■■" << endl;

        input = _getch();

        if (input == 'w' || input == 'W') {
            if (selectedOption > 1) {
                selectedOption--;
            }
        }
        else if (input == 's' || input == 'S') {
            if (selectedOption < 3) {
                selectedOption++;
            }
        }
        else if (input == 13)
        {
            validinput = true;

            if (selectedOption == 1)
            {
                system("cls");
                if (monster._monsterType == MonsterType::Monster)
                {
                    BattleDraw(2);
                }
                else if (monster._monsterType == MonsterType::BossMonster)
                {
                    BossBattleDraw(2);
                }
                cout << "플레이어 공격! Monster HP : " << monster._hp << " ( - " << player._atk - monster._def << " ) " << endl;
                monster.takeDamage(player._atk);
                if (monster.isDead())
                {
                    monster._hp = 0;
                }
                cout << "Monster Hp : " << monster._hp << endl;
            }
            else if (selectedOption == 2)
            {
                bool valid_inventory = false;
                int iteminput;

                player.ShowInventory();
                cout << "어떤 아이템을 사용할까?" << endl;
                cout << " > ";
                cin >> iteminput;

                while (!valid_inventory)
                {
                    if (iteminput + 1 > player._inventory.size() || iteminput < 0)
                    {
                        cout << "존재하지 않는 아이템입니다." << endl;
                        validinput = false;
                        break;
                    }
                    else
                    {
                        player.UseItem(iteminput);
                        player.PrintPlayerInfo();
                        valid_inventory = true;
                    }
                }
            }
            else if (selectedOption == 3)
            {
                if (player.skill->SkillCool() == false)  // 쿨타임이 0일 때만 스킬 사용
                {
                    system("cls");
                    if (monster._monsterType == MonsterType::Monster)
                    {
                        BattleDraw(6);
                    }
                    else if (monster._monsterType == MonsterType::BossMonster)
                    {
                        BossBattleDraw(5);
                    }
                    player.useSkill();  // 스킬 사용 처리
                    cout << "플레이어 공격! Monster HP : " << monster._hp << " ( - " << player._atk * 1.5 - monster._def << " ) " << endl;
                    monster.takeDamage(player._atk * 1.5);
                    if (monster.isDead())
                    {
                        monster._hp = 0;
                    }
                    cout << "Monster Hp : " << monster._hp << endl;
                }
                else
                {
                    player.useSkill();
                }
            }
        }
    }

    this_thread::sleep_for(chrono::seconds(1));
    system("cls");

    if (monster.isDead())
    {
        if (monster._monsterType == MonsterType::Monster)
        {
            BattleDraw(4);
            cout << "몬스터 처치!" << endl;
            return;
        }
        else if (monster._monsterType == MonsterType::BossMonster)
        {
            BattleDraw(4);
            cout << "던전의 주인을 처치했습니다!" << endl;
            return;
        }
    }

    // 플레이어의 행동과 관계없는 몬스터 행동
    int monsterdamage = monster._atk - player._def;
    if (monsterdamage < 0)
        monsterdamage = 0;

    if (monster._monsterType == MonsterType::Monster)
    {
        BattleDraw(3);
        cout << "몬스터 공격! Player HP : " << player._hp << " ( - " << monsterdamage << " ) " << endl;
        player.takeDamage(monster._atk);
        if (player.isDead())
        {
            player._hp = 0;
        }
        cout << "Player Hp : " << player._hp << endl;

        if (player._hp <= 0)
        {
            this_thread::sleep_for(chrono::seconds(1));
            system("cls");

            BattleDraw(5);
            cout << "플레이어가 사망했습니다. Game over" << endl;
            return;
        }
    }
    else if (monster._monsterType == MonsterType::BossMonster)
    {
        BossBattleDraw(3);
        cout << "몬스터 공격! Player HP : " << player._hp << " ( - " << monsterdamage << " ) " << endl;
        player.takeDamage(monster._atk);
        if (player.isDead())
        {
            player._hp = 0;
        }
        cout << "Player Hp : " << player._hp << endl;

        if (player._hp <= 0)
        {
            this_thread::sleep_for(chrono::seconds(1));
            system("cls");

            BossBattleDraw(4);
            cout << "플레이어가 사망했습니다. Game over" << endl;
            return;
        }
    }
    this_thread::sleep_for(chrono::seconds(1));
    system("cls");
}

void Battle::BattleDraw(int a)
{
    if (a == 1)
    {
        cout << endl;
        cout << "                       ,,:  " << endl;
        cout << "                     .-,~;~ " << endl;
        cout << "                     : ,:!! " << endl;
        cout << "                     --;:~~ " << endl;
        cout << "                    --.~-   " << endl;
        cout << "      :$@@!                 " << endl;
        cout << "     ~#@@@;:    //ㅣ        " << endl;
        cout << "     :=!=:.   / / ㅣ        " << endl;
        cout << "      ;@: . /ㅣ/  /         " << endl;
        cout << "     .~,~  ＼/ ＼/          " << endl;
        cout << "    ~!~-=;~~/＼/            " << endl;
        cout << "   --~~~-:;-                " << endl;
        cout << "   ~,-;~,~.                 " << endl;
        cout << "────────────────────────────────────────────────────────────" << endl;
    }
    else if (a == 2)
    {
        for (int i = 0; i < 3; i++) {
            cout << "\033[H";
            if (i % 2 == 0) {
                cout << "\033[31m";
            }
            else {
                cout << "\033[0m";
            }

            cout << endl;
            cout << "                   ＼ ,,:   " << endl;
            cout << "                   .＼-,~;~ " << endl;
            cout << "                   : ＼,:!! " << endl;
            cout << "                   --;:＼~~ " << endl;
            cout << "                  --.~- ＼  " << endl;
            cout << "\033[0m";
            cout << "      :$@@!                 " << endl;
            cout << "     ~#@@@;:    //ㅣ        " << endl;
            cout << "     :=!=:.   / / ㅣ        " << endl;
            cout << "      ;@: . /ㅣ/  /         " << endl;
            cout << "     .~,~  ＼/ ＼/          " << endl;
            cout << "    ~!~-=;~~/＼/            " << endl;
            cout << "   --~~~-:;-                " << endl;
            cout << "   ~,-;~,~.                 " << endl;
            cout << "────────────────────────────────────────────────────────────" << endl;
            this_thread::sleep_for(chrono::milliseconds(200));
        }
    }
    else if (a == 3)
    {
        cout << endl;
        cout << "                            " << endl;
        cout << "                     ,,:    " << endl;
        cout << "                   .-,~;~   " << endl;
        cout << "                   : ,:!!   " << endl;
        cout << "                   --;:~~   " << endl;
        cout << "                  --.~-     " << endl;
        cout << "\033[33m";
        cout << "      :$@@!                 " << endl;
        cout << "    ＼  ~#@@@;:    //ㅣ     " << endl;
        cout << "     ＼:=!=:.   / /  ㅣ     " << endl;
        cout << "   ＼  ;＼@: . /ㅣ/  /      " << endl;
        cout << "    ＼.~,~＼  ＼/ ＼/       " << endl;
        cout << "   ~!~＼-=;~＼~/＼/         " << endl;
        cout << "   --~~~＼-:;-＼            " << endl;
        cout << "   ~,-;~,~＼.  ＼           " << endl;
        cout << "           ＼   ＼          " << endl;
        cout << "\033[0m";
        cout << "────────────────────────────────────────────────────────────" << endl;
    }
    else if (a == 4)
    {
        cout << endl;
        cout << endl;
        cout << endl;
        cout << "                            " << endl;
        cout << "                            " << endl;
        cout << "                            " << endl;
        cout << "      :$@@!                 " << endl;
        cout << "     ~#@@@;:    //ㅣ        " << endl;
        cout << "     :=!=:.   / / ㅣ        " << endl;
        cout << "      ;@: . /ㅣ/  /         " << endl;
        cout << "     .~,~  ＼/ ＼/          " << endl;
        cout << "    ~!~-=;~~/＼/            " << endl;
        cout << "   --~~~-:;-                " << endl;
        cout << "   ~,-;~,~.                 " << endl;
        cout << "────────────────────────────────────────────────────────────" << endl;
    }
    else if (a == 5)
    {
        cout << endl;
        cout << "                       ,,:  " << endl;
        cout << "                     .-,~;~ " << endl;
        cout << "                     : ,:!! " << endl;
        cout << "                     --;:~~ " << endl;
        cout << "                    --.~-   " << endl;
        cout << "                            " << endl;
        cout << "                           " << endl;
        cout << "                           " << endl;
        cout << "                           " << endl;
        cout << "                          " << endl;
        cout << "                           " << endl;
        cout << "   --~~~-:;-                " << endl;
        cout << "   ~,-;~,~.                 " << endl;
        cout << "────────────────────────────────────────────────────────────" << endl;
    }
    else if (a == 6)
    {
        for (int i = 0; i < 5; i++) {
            cout << "\033[H";
            if (i % 2 == 0) {
                cout << "\033[31m";
            }
            else {
                cout << "\033[0m";
            }
            cout << endl;
            cout << "                    ,,: 『／  " << endl;
            cout << "                   .＼-『／,~;~ " << endl;
            cout << "                   : 『／ㄱ＼,:!! " << endl;
            cout << "                   --『／『／;:＼~ " << endl;
            cout << "                  --.~『／- ＼  " << endl;
            cout << "\033[0m";
            cout << "      :$@@!                 " << endl;
            cout << "     ~#@@@;:    //ㅣ        " << endl;
            cout << "     :=!=:.   / / ㅣ        " << endl;
            cout << "      ;@: . /ㅣ/  /         " << endl;
            cout << "     .~,~  ＼/ ＼/          " << endl;
            cout << "    ~!~-=;~~/＼/            " << endl;
            cout << "   --~~~-:;-                " << endl;
            cout << "   ~,-;~,~.                 " << endl;
            cout << "────────────────────────────────────────────────────────────" << endl;
            this_thread::sleep_for(chrono::milliseconds(200));
        }
    }
}

void Battle::BossBattleDraw(int b)
{
    if (b == 1)
    {
        cout << "                         / _._  \\ // " << endl;
        cout << "                        //_`&gt; /  \\" << endl;
        cout << "                       / \" \"\\/  \  " << endl;
        cout << "                       | /_.(  |\   | " << endl;
        cout << "                        \__/ \| '__/  " << endl;
        cout << "                          =/ =/  _)   " << endl;
        cout << "      :$@@!               "  "        " << endl;
        cout << "     ~#@@@;:    //ㅣ        " << endl;
        cout << "     :=!=:.   / / ㅣ        " << endl;
        cout << "      ;@: . /ㅣ/  /         " << endl;
        cout << "     .~,~  ＼/ ＼/          " << endl;
        cout << "    ~!~-=;~~/＼/            " << endl;
        cout << "   --~~~-:;-                " << endl;
        cout << "   ~,-;~,~.                 " << endl;
        cout << "────────────────────────────────────────────────────────────" << endl;
    }
    else if (b == 2)
    {
        for (int i = 0; i < 3; i++) {
            cout << "\033[H";
            if (i % 2 == 0) {
                cout << "\033[31m";
            }
            else {
                cout << "\033[0m";
            }
            cout << "                        ∥ / _∥./_ ∥ /\\ // " << endl;
            cout << "                        ∥//_`∥/&gt∥/; /  \\" << endl;
            cout << "                       /∥ \" /\"\\///  \  " << endl;
            cout << "                       |∥ /_/.∥(  /∥|\   | " << endl;
            cout << "                        ∥\_/_/∥ \/|∥ '__/  " << endl;
            cout << "                        ∥ / =/∥ /=/∥  _)   " << endl;
            cout << "\033[0m";
            cout << "     :$@@!                   "  "          " << endl;
            cout << "    ~#@@@;:    //ㅣ        " << endl;
            cout << "    :=!=:.   / / ㅣ        " << endl;
            cout << "     ;@: . /ㅣ/  /         " << endl;
            cout << "    .~,~  ＼/ ＼/          " << endl;
            cout << "   ~!~-=;~~/＼/            " << endl;
            cout << "  --~~~-:;-                " << endl;
            cout << "  ~,-;~,~.                 " << endl;
            cout << "────────────────────────────────────────────────────────────" << endl;
            this_thread::sleep_for(chrono::milliseconds(200));
        }
    }

    else if (b == 3)
    {
        cout << "               / _._  \\ // " << endl;
        cout << "              //_`&gt; /  \\" << endl;
        cout << "             / \" \"\\/  \  " << endl;
        cout << "             | /_.(  |\   | " << endl;
        cout << "              \__/ \| '__/  " << endl;
        cout << "                =/ =/  _)   " << endl;
        cout << "\033[33m";
        cout << "     ://$@@!//      "  "        " << endl;
        cout << "    ~//#@@@//;:    //ㅣ        " << endl;
        cout << "    //:=!=//:. //  / / ㅣ        " << endl;
        cout << "     ∥;@//: .// /ㅣ/  /         " << endl;
        cout << "    .~∥//,~ // ＼/ ＼/          " << endl;
        cout << "   ~!//~∥-=//;~~/＼/            " << endl;
        cout << "  --//~~~∥//-:;-                " << endl;
        cout << "  ~//,-;//~∥,~.                 " << endl;
        cout << "\033[0m";
        cout << "────────────────────────────────────────────────────────────" << endl;
    }

    else if (b == 4)
    {
        cout << "               / _._  \\ // " << endl;
        cout << "              //_`&gt; /  \\" << endl;
        cout << "             / \" \"\\/  \  " << endl;
        cout << "             | /_.(  |\   | " << endl;
        cout << "              \__/ \| '__/  " << endl;
        cout << "                =/ =/  _)   " << endl;
        cout << "                "  "        " << endl;
        cout << "                  " << endl;
        cout << "                  " << endl;
        cout << "                  " << endl;
        cout << "                  " << endl;
        cout << "   ~!~-\=;~\~/＼  " << endl;
        cout << "    ~~~-~~~-      " << endl;
        cout << "   ~,-\;~,~\.     " << endl;
        cout << "────────────────────────────────────────────────────────────" << endl;
    }
    else if (b == 5)
    {
        for (int i = 0; i < 5; i++) {
            cout << "\033[H";
            if (i % 2 == 0) {
                cout << "\033[31m";
            }
            else {
                cout << "\033[0m";
            }
            cout << "                       / ／_.×_  \\ ／ " << endl;
            cout << "                      //_『／`&∇gt; /『／ " << endl;
            cout << "                     / \" ／『 ㄱ『／ " << endl;
            cout << "                     | /_.『／( 〃 |／\  " << endl;
            cout << "                      \__/ 『ㄱ／／『／\| '  " << endl;
            cout << "                        =/ =/ ㄱ _／)   " << endl;
            cout << "\033[0m";
            cout << "      :$@@!               "  "        " << endl;
            cout << "     ~#@@@;:    //ㅣ        " << endl;
            cout << "     :=!=:.   / / ㅣ        " << endl;
            cout << "      ;@: . /ㅣ/  /         " << endl;
            cout << "     .~,~  ＼/ ＼/          " << endl;
            cout << "    ~!~-=;~~/＼/            " << endl;
            cout << "   --~~~-:;-                " << endl;
            cout << "   ~,-;~,~.                 " << endl;
            cout << "────────────────────────────────────────────────────────────" << endl;
            this_thread::sleep_for(chrono::milliseconds(200));
        }
    }
}
