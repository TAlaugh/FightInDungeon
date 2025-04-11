#include <iostream>
#include <ctime>
#include <cstdlib>
#include "Windows.h"
#include "Game.h"
#include "Loby.h"
using namespace std;

void HideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

int main() {
    HideCursor();
    system("mode con: cols=62 lines=40");

    srand(unsigned int(time(0)));

    Game game;
    Loby loby;

    loby.PrintStory(1);
    loby.PrintStory(2);
    loby.PrintStory(3);
    
    loby.PrintLoby();
    loby.InputLoby();

    system("cls");

    while (game.GameOver == false) {
        game.Run();
    }

    return 0;
}