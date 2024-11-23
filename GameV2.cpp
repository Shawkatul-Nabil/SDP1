#include <iostream>
#include <ctime>
#include <conio.h>
#include <cstdlib>
#include <windows.h>  // Include this header for color manipulation
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
    bool *zombies;
    bool right;
public:
    cZombie(int width) {
        zombies = new bool[width];
        for (int i = 0; i < width; i++)
            zombies[i] = false;  // No zombies at the start
        right = rand() % 2;  // Random direction (left or right)
    }

    ~cZombie() {
        delete[] zombies;
    }

    void Move(int width) {
        if (right) {
            if (rand() % 10 == 1)
                zombies[0] = true;  // Add a new zombie
            else
                zombies[0] = false;  // No zombie
            // Shift zombies to the right
            for (int i = width - 1; i > 0; i--)
                zombies[i] = zombies[i - 1];
            zombies[0] = false;  // Empty space at the beginning
        }
        else {
            if (rand() % 10 == 1)
                zombies[width - 1] = true;  // Add a new zombie
            else
                zombies[width - 1] = false;  // No zombie
            // Shift zombies to the left
            for (int i = 0; i < width - 1; i++)
                zombies[i] = zombies[i + 1];
            zombies[width - 1] = false;  // Empty space at the end
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
    cPlayer *player;
    cZombie **area;
public:
    cGame(int w = 20, int h = 10) {
        numberOfLanes = h;
        width = w;
        quit = false;
        area = new cZombie*[numberOfLanes];
        for (int i = 0; i < numberOfLanes; i++)
            area[i] = new cZombie(width);
        player = new cPlayer(width);
    }

    ~cGame() {
        delete player;
        for (int i = 0; i < numberOfLanes; i++) {
            delete area[i];
        }
        delete[] area;
    }

    void SetBackgroundColor(int color) {
        // Set console text attributes (background color and text color)
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }

    void Draw() {
        system("cls");  // Clear screen

        // Set background color to green and foreground color to white
        SetBackgroundColor(BACKGROUND_GREEN | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        // Draw top border
        for (int i = 0; i < width + 2; i++) {
            cout << "#";  // Top border
        }
        cout << endl;

        // Draw the game area with left and right borders
        for (int i = 0; i < numberOfLanes; i++) {
            cout << "#";  // Left border
            for (int j = 0; j < width; j++) {
                if (i == 0 && j == 0) cout << "Start";  // Start zone on the left side only
                else if (area[i]->CheckPos(j) && i != 0 && i != numberOfLanes - 1)
                    cout << "Z";  // Zombie
                else if (player->x == j && player->y == i)
                    cout << "P";  // Player (Survivor)
                else
                    cout << " ";  // Empty space
            }
            cout << "#";  // Right border
            cout << endl;
        }

        // Draw bottom border
        for (int i = 0; i < width + 2; i++) {
            cout << "#";  // Bottom border
        }
        cout << endl;

        cout << "Score: " << score << endl;
    }

    void Input() {
        if (_kbhit()) {
            char current = _getch();
            if (current == 'a') player->x--;  // Move left
            if (current == 'd') player->x++;  // Move right
            if (current == 'w') player->y--;  // Move up
            if (current == 's') player->y++;  // Move down
            if (current == 'q') quit = true;  // Quit game
        }
    }

    void Logic() {
        for (int i = 1; i < numberOfLanes - 1; i++) {
            if (rand() % 10 == 1)
                area[i]->Move(width);
            if (area[i]->CheckPos(player->x) && player->y == i)  // Player collided with zombie
                quit = true;
        }
        if (player->y == numberOfLanes - 1) {  // Reached safe zone
            score++;
            player->y = 0;  // Reset player to the start
            cout << "\x07";  // Sound (alert)
            area[rand() % numberOfLanes]->ChangeDirection();  // Change direction of a random zombie lane
        }
    }

    void Run() {
        while (!quit) {
            Input();
            Draw();
            Logic();
        }
    }
};

class Help {
public:
    void get_help(void) {
        cout << "W key for up, S key for down, A key for left, D key for right, Q to quit.\n";
        cout << "Avoid the zombies, reach the safe zone to survive!";
    }
};

void printGameLogo() {
    cout << " ______   _______  _______  ______       _______           _       \n";
    cout << "(  __  \\ (  ____ \\(  ___  )(  __  \\     (  ____ )|\\     /|( (    /|\n";
    cout << "| (  \\  )| (    \\/| (   ) || (  \\  )    | (    )|| )   ( ||  \\  ( |\n";
    cout << "| |   ) || (__    | (___) || |   ) |    | (____)|| |   | ||   \\ | |\n";
    cout << "| |   | ||  __)   |  ___  || |   | |    |     __)| |   | || (\\ \\) |\n";
    cout << "| |   ) || (      | (   ) || |   ) |    | (\\ (   | |   | || | \\   |\n";
    cout << "| (__/  )| (____/\\| )   ( || (__/  )    | ) \\ \\__| (___) || )  \\  |\n";
    cout << "(______/ (_______/|/     \\|(______/     |/   \\__/(_______)|/    )_)\n";
}

int main() {
    srand(time(NULL));  // Seed random number generator

    // Print the game logo once
    printGameLogo();

    char op;
    int ch;
    do {

        cout << "Menu:\n";
        cout << "1. Play Game\n";
        cout << "2. Help/How to play\n";
        cout << "3. Quit Game\n";
        cin >> ch;
        system("cls");

        if (ch == 1) {
            int choice;

            cout << "Select Difficulty Level:\n";
            cout << "1. Easy\n";
            cout << "2. Medium\n";
            cout << "3. Hard\n";
            cout << "Enter your choice (1-3): ";
            cin >> choice;
            system("cls");

            switch (choice) {
            case 1: {
                cGame game(50, 25);
                game.Run();
                cout << "Game over!" << endl;
            }
                break;
            case 2: {
                cGame game(50, 25);
                game.Run();
                cout << "Game over!" << endl;
            }
                break;
            case 3: {
                cGame game(50, 25);
                game.Run();
                cout << "Game over!" << endl;
            }
                break;
            }
        }

        if (ch == 2) {
            Help obj;
            obj.get_help();
        }

        if (ch == 3) {
            return 0;
        }

        cout << "Continue? (y/n): ";
        cin >> op;

    } while (op == 'y');
    return 0;
}


