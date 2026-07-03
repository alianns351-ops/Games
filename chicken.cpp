#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

const int width = 30;
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

struct Entity {
    int x, y;
    bool active;
};

class ChickenShooter {
private:
    Entity player;
    vector<Entity> bullets;
    vector<Entity> chickens;
    int score;
    bool gameOver;

public:
    ChickenShooter() {
        srand((unsigned)time(0));
        hideCursor();
        Reset();
    }

    void Reset() {
        player = {width / 2, height - 1, true};
        bullets.clear();
        chickens.clear();
        score = 0;
        gameOver = false;
        system("cls");
        system("color 0B"); // Light Blue color for space feel
    }

    void SpawnChicken() {
        if (rand() % 10 == 0) { // Probability of spawn
            chickens.push_back({rand() % width, 0, true});
        }
    }

    void Draw() {
        goToXY(0, 0);
        for (int i = 0; i < width + 2; i++) cout << "-";
        cout << endl;

        for (int i = 0; i < height; i++) {
            cout << "|";
            for (int j = 0; j < width; j++) {
                bool printed = false;

                // Draw Player
                if (i == player.y && j == player.x) {
                    cout << "🚀"; 
                    printed = true;
                }
                
                // Draw Bullets
                if (!printed) {
                    for (auto& b : bullets) {
                        if (b.active && b.x == j && b.y == i) {
                            cout << "🔥"; 
                            printed = true;
                            break;
                        }
                    }
                }

                // Draw Chickens
                if (!printed) {
                    for (auto& c : chickens) {
                        if (c.active && c.x == j && c.y == i) {
                            cout << "🐔";
                            printed = true;
                            break;
                        }
                    }
                }

                if (!printed) cout << "  "; // Double space for emojis
            }
            cout << "|" << endl;
        }

        for (int i = 0; i < width + 2; i++) cout << "-";
        cout << "\n SCORE: " << score << " | [Arrows: Move] [Space: Shoot]   " << endl;

        if (gameOver) {
            cout << "\n !!! MISSION FAILED !!! 💥" << endl;
            cout << " Press 'R' to Restart or 'Q' to Quit." << endl;
        }
    }

    void Input() {
        if (GetAsyncKeyState(VK_LEFT) && player.x > 0) player.x--;
        if (GetAsyncKeyState(VK_RIGHT) && player.x < width - 1) player.x++;
        if (GetAsyncKeyState(VK_SPACE)) {
            static DWORD lastShot = 0;
            if (GetTickCount() - lastShot > 300) { // Fire rate limit
                bullets.push_back({player.x, player.y - 1, true});
                lastShot = GetTickCount();
            }
        }
        if (GetAsyncKeyState('Q')) gameOver = true;
    }

    void Logic() {
        if (gameOver) return;

        // Move Bullets
        for (auto& b : bullets) {
            if (b.active) {
                b.y--;
                if (b.y < 0) b.active = false;
            }
        }

        // Move Chickens
        static int moveCounter = 0;
        if (moveCounter++ % 3 == 0) { // Chickens move slower than bullets
            for (auto& c : chickens) {
                if (c.active) {
                    c.y++;
                    if (c.y >= height) gameOver = true; // Chicken reached ground
                }
            }
        }

        // Collision Detection
        for (auto& b : bullets) {
            if (!b.active) continue;
            for (auto& c : chickens) {
                if (!c.active) continue;
                if (b.x == c.x && b.y == c.y) {
                    b.active = false;
                    c.active = false;
                    score += 50;
                }
            }
        }

        SpawnChicken();
    }

    void Run() {
        while (true) {
            while (!gameOver) {
                Draw();
                Input();
                Logic();
                Sleep(30); 
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
    // Windows terminal settings for Emoji support
    SetConsoleOutputCP(CP_UTF8);
    system("mode con: cols=70 lines=30");
    
    ChickenShooter game;
    game.Run();
    return 0;
}