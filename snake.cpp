#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

// Screen Settings
const int width = 40;
const int height = 25;

// Function to move cursor without clearing screen (Prevents Flickering)
void goToXY(int x, int y) {
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Function to hide the blinking cursor
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info = {100, FALSE};
    SetConsoleCursorInfo(consoleHandle, &info);
}

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

class NokiaSnake {
private:
    int x, y, fruitX, fruitY, score;
    int tailX[1000], tailY[1000]; // Supporting long snake
    int nTail;
    Direction dir;
    bool gameOver;

public:
    NokiaSnake() {
        srand((unsigned)time(0));
        hideCursor();
        Reset();
    }

    void Reset() {
        gameOver = false;
        dir = STOP;
        x = width / 2;
        y = height / 2;
        score = 0;
        nTail = 0;
        GenerateFruit();
        system("cls"); // Clear once at start
    }

    void GenerateFruit() {
        fruitX = rand() % width;
        fruitY = rand() % height;
    }

    void Draw() {
        goToXY(0, 0);
        for (int i = 0; i < width + 2; i++) cout << "#";
        cout << endl;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (j == 0) cout << "#";

                if (i == y && j == x) cout << "O"; // Head
                else if (i == fruitY && j == fruitX) cout << "*"; // Food
                else {
                    bool printTail = false;
                    for (int k = 0; k < nTail; k++) {
                        if (tailX[k] == j && tailY[k] == i) {
                            cout << "o"; // Tail
                            printTail = true;
                            break;
                        }
                    }
                    if (!printTail) cout << " ";
                }
                if (j == width - 1) cout << "#";
            }
            cout << endl;
        }

        for (int i = 0; i < width + 2; i++) cout << "#";
        cout << "\n\n SCORE: " << score << "   " << endl;
        
        if (dir == STOP && !gameOver) 
            cout << " PRESS ANY ARROW KEY TO START...             " << endl;
        else if (gameOver) 
            cout << " !!! GAME OVER !!! Press 'R' to Replay, 'Q' to Quit " << endl;
        else 
            cout << " [Arrow Keys: Move] | [Q: Quit Game]         " << endl;
    }

    void Input() {
        if (GetAsyncKeyState(VK_UP) && dir != DOWN) dir = UP;
        else if (GetAsyncKeyState(VK_DOWN) && dir != UP) dir = DOWN;
        else if (GetAsyncKeyState(VK_LEFT) && dir != RIGHT) dir = LEFT;
        else if (GetAsyncKeyState(VK_RIGHT) && dir != LEFT) dir = RIGHT;
        else if (GetAsyncKeyState('Q')) gameOver = true;
    }

    void Logic() {
        if (dir == STOP) return;

        // Tail movement
        int prevX = tailX[0], prevY = tailY[0], p2X, p2Y;
        tailX[0] = x; tailY[0] = y;
        for (int i = 1; i < nTail; i++) {
            p2X = tailX[i]; p2Y = tailY[i];
            tailX[i] = prevX; tailY[i] = prevY;
            prevX = p2X; prevY = p2Y;
        }

        switch (dir) {
            case LEFT:  x--; break;
            case RIGHT: x++; break;
            case UP:    y--; break;
            case DOWN:  y++; break;
            default: break;
        }

        // Hit Walls
        if (x >= width || x < 0 || y >= height || y < 0) gameOver = true;

        // Hit Self
        for (int i = 0; i < nTail; i++)
            if (tailX[i] == x && tailY[i] == y) gameOver = true;

        // Eat Food
        if (x == fruitX && y == fruitY) {
            score += 10;
            nTail++;
            GenerateFruit();
        }
    }

    void Run() {
        while (true) {
            while (!gameOver) {
                Draw();
                
                // Speed Control: 100ms Total Delay
                // Input 10 baar check hoga (fast response ke liye)
                for(int i = 0; i < 10; i++) {
                    Input(); 
                    Sleep(10); 
                }
                
                Logic();
            }
            
            Draw(); // Show Game Over state
            
            // Replay Logic
            while (gameOver) {
                if (GetAsyncKeyState('R')) {
                    Reset();
                    break;
                }
                if (GetAsyncKeyState('Q')) return;
                Sleep(50);
            }
        }
    }
};

int main() {
    // Window size adjustment
    system("mode con: cols=60 lines=40");
    
    NokiaSnake game;
    game.Run();
    
    return 0;
}