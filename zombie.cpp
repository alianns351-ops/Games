#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

const int width = 50;
const int height = 20;

void goToXY(int x, int y) {
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info = {100, FALSE};
    SetConsoleCursorInfo(consoleHandle, &info);
}

// Zombie Class
class Zombie {
public:
    int x, y;
    Zombie() {
        // Randomly spawn at edges
        int edge = rand() % 4;
        if (edge == 0) { x = 0; y = rand() % height; }         // Left
        else if (edge == 1) { x = width - 1; y = rand() % height; } // Right
        else if (edge == 2) { x = rand() % width; y = 0; }         // Top
        else { x = rand() % width; y = height - 1; }              // Bottom
    }

    void MoveTowards(int px, int py) {
        // Basic AI: Move one step closer to player
        if (x < px) x++;
        else if (x > px) x--;
        
        if (y < py) y++;
        else if (y > py) y--;
    }
};

class ZombieGame {
private:
    int playerX, playerY, score;
    vector<Zombie> zombies;
    bool gameOver;
    int frameCount;

public:
    ZombieGame() {
        srand((unsigned)time(0));
        hideCursor();
        Reset();
    }

    void Reset() {
        playerX = width / 2;
        playerY = height / 2;
        score = 0;
        gameOver = false;
        frameCount = 0;
        zombies.clear();
        zombies.push_back(Zombie()); // Start with 1 zombie
        system("cls");
    }

    void Draw() {
        goToXY(0, 0);
        for (int i = 0; i < width + 2; i++) cout << "#";
        cout << endl;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (j == 0) cout << "#";

                if (i == playerY && j == playerX) cout << "@"; // Player
                else {
                    bool printed = false;
                    for (auto& z : zombies) {
                        if (z.x == j && z.y == i) {
                            cout << "Z"; // Zombie
                            printed = true;
                            break;
                        }
                    }
                    if (!printed) cout << " ";
                }

                if (j == width - 1) cout << "#";
            }
            cout << endl;
        }

        for (int i = 0; i < width + 2; i++) cout << "#";
        cout << "\n\n SURVIVAL SCORE: " << score << "   " << endl;
        cout << " ZOMBIES ACTIVE: " << zombies.size() << "   " << endl;
        
        if (gameOver) {
            cout << "\n !!! BITTEN !!! GAME OVER !!!" << endl;
            cout << " Press 'R' to Restart or 'Q' to Quit." << endl;
        }
    }

    void Input() {
        if (GetAsyncKeyState(VK_UP) && playerY > 0) playerY--;
        if (GetAsyncKeyState(VK_DOWN) && playerY < height - 1) playerY++;
        if (GetAsyncKeyState(VK_LEFT) && playerX > 0) playerX--;
        if (GetAsyncKeyState(VK_RIGHT) && playerX < width - 1) playerX++;
        if (GetAsyncKeyState('Q')) gameOver = true;
    }

    void Logic() {
        if (gameOver) return;

        frameCount++;
        score++;

        // Zombies move every 2nd frame (to make it fair)
        if (frameCount % 2 == 0) {
            for (auto& z : zombies) {
                z.MoveTowards(playerX, playerY);
                // Check Collision with Player
                if (z.x == playerX && z.y == playerY) {
                    gameOver = true;
                }
            }
        }

        // Spawn new zombie every 50 frames
        if (frameCount % 50 == 0) {
            zombies.push_back(Zombie());
        }
    }

    void Run() {
        while (true) {
            while (!gameOver) {
                Draw();
                Input();
                Logic();
                Sleep(50); // Fast gameplay
            }

            Draw();
            while (gameOver) {
                if (GetAsyncKeyState('R')) { Reset(); break; }
                if (GetAsyncKeyState('Q')) return;
                Sleep(100);
            }
        }
    }
};

int main() {
    system("mode con: cols=65 lines=35");
    ZombieGame game;
    game.Run();
    return 0;
}