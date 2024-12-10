#include <iostream>
#include <ctime>
#include <conio.h>
#include <cstdlib>
#include <windows.h>

using namespace std;

class cPlayer {
public:
    int x, y;
    cPlayer(int width) {
        x = width / 2;
        y = 0;
    }
};

class cZombie {
private:
    bool* zombies;
    bool right;
public:
    cZombie(int width) {
        zombies = new bool[width];
        for (int i = 0; i < width; i++)
            zombies[i] = false; // No zombies at the start
        right = rand() % 2; // Random direction (left or right)
    }

    ~cZombie() {
        delete[] zombies;
    }

    void Move(int width) {
        if (right) {
            for (int i = width - 1; i > 0; i--)
                zombies[i] = zombies[i - 1];
            zombies[0] = rand() % 10 == 1; // Random zombie at the start
        } else {
            for (int i = 0; i < width - 1; i++)
                zombies[i] = zombies[i + 1];
            zombies[width - 1] = rand() % 10 == 1; // Random zombie at the end
        }
    }

    bool CheckPos(int pos) {
        return zombies[pos];
    }

    void ChangeDirection() {
        right = !right;
    }
};

class cGame {
private:
    bool quit;
    int numberOfLanes;
    int width;
    int score;
    cPlayer* player;
    cZombie** area;
    int speed; // Variable speed for difficulty
    int zombieSpawnRate; // Spawn rate for zombies

    void DrawBorder() {
        for (int i = 0; i < width + 2; i++) cout << "#";
        cout << endl;

        for (int i = 0; i < numberOfLanes; i++) {
            cout << "#";
            for (int j = 0; j < width; j++) {
                if (player->x == j && player->y == i)
                    cout << "P";
                else if (area[i]->CheckPos(j))
                    cout << "Z";
                else
                    cout << " ";
            }
            cout << "#";
            cout << endl;
        }

        for (int i = 0; i < width + 2; i++) cout << "#";
        cout << endl;
    }

public:
    cGame(int w = 20, int h = 10, int sp = 150, int spawnRate = 10) {
        numberOfLanes = h;
        width = w;
        quit = false;
        score = 0;
        speed = sp;
        zombieSpawnRate = spawnRate;
        area = new cZombie * [numberOfLanes];
        for (int i = 0; i < numberOfLanes; i++)
            area[i] = new cZombie(width);
        player = new cPlayer(width);

        // Set background color to green
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
    }

    ~cGame() {
        delete player;
        for (int i = 0; i < numberOfLanes; i++)
            delete area[i];
        delete[] area;
    }

    void Input() {
        if (_kbhit()) {
            char current = _getch();
            if (current == 'a' && player->x > 0) player->x--; // Move left
            if (current == 'd' && player->x < width - 1) player->x++; // Move right
            if (current == 'w' && player->y > 0) player->y--; // Move up
            if (current == 's' && player->y < numberOfLanes - 1) player->y++; // Move down
            if (current == 'q') quit = true; // Quit game
        }
    }

    void Logic() {
        for (int i = 1; i < numberOfLanes - 1; i++) {
            if (rand() % zombieSpawnRate == 1)
                area[i]->Move(width);
            if (area[i]->CheckPos(player->x) && player->y == i) { // Collision with zombie
                quit = true;
            }
        }
        if (player->y == numberOfLanes - 1) { // Player reaches safe zone
            score++;
            player->y = 0; // Reset player to start
            cout << "\x07"; // Sound alert
            area[rand() % numberOfLanes]->ChangeDirection(); // Change direction of a random lane
        }
    }

    void Draw() {
        COORD cursorPos = { 0, 0 };
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hConsole, cursorPos);

        DrawBorder();
        cout << "Score: " << score << endl;
    }

    void Run() {
        while (!quit) {
            Input();
            Draw();
            Logic();
            Sleep(speed); // Adjust speed for difficulty
        }
        cout << "Game Over! Your score: " << score << endl;
    }
};

class Help {
public:
    void DisplayHelp() {
        cout << "Welcome to the Zombie Game!\n";
        cout << "Controls:\n";
        cout << "  W - Move Up\n";
        cout << "  S - Move Down\n";
        cout << "  A - Move Left\n";
        cout << "  D - Move Right\n";
        cout << "  Q - Quit Game\n";
        cout << "Objective:\n";
        cout << "  Avoid the zombies (Z) and reach the safe zone at the bottom.\n";
        cout << "  Each successful attempt increases your score.\n";
    }
};

void PrintGameLogo() {
    cout << "______               _  ______            \n";
    cout << "|  _  \\             | | | ___ \\           \n";
    cout << "| | | |___  __ _  __| | | |_/ /   _ _ __  \n";
    cout << "| | | / _ \\/ _` |/ _` | |    / | | | '_ \\ \n";
    cout << "| |/ /  __/ (_| | (_| | | |\\ \\ |_| | | | | \n";
    cout << "|___/ \\___|\\__,_|\\__,_| \\_| \\_\\__,_|_| |_| \n";

}

int main() {
    srand(time(0)); // Seed for random numbers

    char choice;
    do {
        system("cls");
        PrintGameLogo();

        cout << "\n\n\n1. Play Game\n";
        cout << "2. How to Play\n";
        cout << "3. Quit\n";
        cout << "Enter your choice: ";
        int menuChoice;
        cin >> menuChoice;

        system("cls");

        if (menuChoice == 1) {
            cout << "Select Difficulty:\n";
            cout << "1. Easy\n";
            cout << "2. Medium\n";
            cout << "3. Hard\n";
            cout << "Enter your choice: ";
            int difficulty;
            cin >> difficulty;

            int speed = 150;
            int spawnRate = 10;

            if (difficulty == 1) { // Easy
                speed = 80;        // Slightly faster updates
                spawnRate = 10;    // Zombies appear more frequently
            } else if (difficulty == 2) { // Medium
                speed = 40;        // Faster updates
                spawnRate = 7;     // Zombies appear even more frequently
            } else if (difficulty == 3) { // Hard
                speed = 20;        // Very fast updates
                spawnRate = 3;     // Constant zombie threats
            }

            // Updated screen size to 50x20
            cGame game(50, 20, speed, spawnRate);
            game.Run();
        } else if (menuChoice == 2) {
            Help help;
            help.DisplayHelp();
            cout << "\nPress any key to return to the menu...";
            _getch();
        } else if (menuChoice == 3) {
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }

        cout << "\nDo you want to return to the main menu? (y/n): ";
        cin >> choice;

    } while (choice == 'y' || choice == 'Y');

    return 0;
}


