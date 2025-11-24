#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

// Player Class

class Player {
private:
    string name;
    int win;
    int loss;
    string lastHistory;

public:
    Player() {}

    Player(string n, int w, int l, string h = "No Match Played Yet!") {
        name = n;
        win = w;
        loss = l;
        lastHistory = h;
    }

    string getName() { return name; }
    int getWin() { return win; }
    int getLoss() { return loss; }
    string getHistory() { return lastHistory; }

    void winGame() {
        win++;
        lastHistory = "WON the last match!";
    }

    void loseGame() {
        loss++;
        lastHistory = "LOST the last match!";
    }

    void clearHistory() {
        lastHistory = "History Cleared!";
    }

    // Show full profile with history

    void showProfile() {
        cout << name << " -> WIN = " << win << ", LOSS = " << loss << endl;
        cout << "Last Game: " << lastHistory << endl;
    }

    // Show only WIN/LOSS (used in Choose Player menu)

    void showScoreOnly() {
        cout << name << " -> WIN = " << win << ", LOSS = " << loss << endl;
    }

    // For file save/load

    string toFileString() {
        return name + " " + to_string(win) + " " + to_string(loss) + " " + lastHistory;
    }

    void fromFileString(string line) {
        int firstSpace = line.find(' ');
        int secondSpace = line.find(' ', firstSpace + 1);
        int thirdSpace = line.find(' ', secondSpace + 1);

        name = line.substr(0, firstSpace);
        win = stoi(line.substr(firstSpace + 1, secondSpace - firstSpace - 1));
        loss = stoi(line.substr(secondSpace + 1, thirdSpace - secondSpace - 1));
        lastHistory = line.substr(thirdSpace + 1);
    }
};

// Game Class

class Game {
private:
    vector<Player> players;

public:
    Game() {
        loadFromFile();
        if (players.empty()) {   // Initialize 5 players if file not exist
            players.push_back(Player("Shadon", 8, 4));
            players.push_back(Player("Alif", 11, 6));
            players.push_back(Player("Dhiren", 5, 9));
            players.push_back(Player("Nahid", 7, 10));
            players.push_back(Player("Nafiz", 13, 6));
            saveToFile();
        }
    }

    // Save players data to file

    void saveToFile() {
        ofstream fout("players.txt");
        for (int i = 0; i < players.size(); i++) {
            fout << players[i].toFileString() << endl;
        }
        fout.close();
    }

    // Load players data from file

    void loadFromFile() {
        ifstream fin("players.txt");
        if (!fin) return;

        players.clear();
        string line;
        while (getline(fin, line)) {
            Player p;
            p.fromFileString(line);
            players.push_back(p);
        }
        fin.close();
    }

    // Display main menu

    void mainMenu() {
        while (true) {
            system("CLS"); // Clear screen every time menu opens
            cout << "  Welcome to RPS Challenge Game\n";
            cout << endl;
            cout << "1. Choose the Player\n";
            cout << endl;
            cout << "2. Profile & History\n";
            cout << endl;
            cout << "3. Exit\n";
            cout << endl;
            cout << "Please Enter Your Choice Number: ";

            int op;
            cin >> op;

            switch (op) {
                case 1: choosePlayer(); break;
                case 2: showProfiles(); break;
                case 3: exit(0);
                default: cout << "Invalid Option!\n";
            }
        }
    }

    // Let user choose a player to play

void choosePlayer() {
    system("CLS");
    cout << "\n Player's Profile \n\n";

    for (int i = 0; i < players.size(); i++) {  // Show only WIN/LOSS score
        cout << i + 1 << ". ";
        players[i].showScoreOnly();
        cout << endl;
    }
    cout << "6. Go to the Menu\n";   // <-- Added this line
    cout << endl;
    cout << "Choose a player as you like or 6 to go menu: ";
    int p;
    cin >> p;

    if (p == 6) {       // <-- If user chooses option 6
        return;         // Simply go back to menu
    }

    if (p < 1 || p > players.size()) {
        cout << "Invalid Player!\n";
        return;
    }

    startGame(p - 1);
}

    // profiles and history

    void showProfiles() {
        system("CLS");
        cout << "\n Profiles & History \n\n";

        for (int i = 0; i < players.size(); i++) {
            cout << endl;
            cout << i + 1 << ". ";
            players[i].showProfile();
            cout << endl;
        }

        cout << "Do you want to Clear the history? (C/c to clear, or any key to go menu): ";
        char ch;
        cin >> ch;

        if (ch == 'c' || ch == 'C') {
            clearHistory();
        }
    }

    // Clear history of selected player

    void clearHistory() {
        cout << "Please Enter The Player Number: ";
        int n;
        cin >> n;

        if (n >= 1 && n <= players.size()) {
            players[n - 1].clearHistory();
            saveToFile();
            cout << "History Cleared Successfully!\n";
        } else {
            cout << "Invalid Player Number!\n";
        }
    }

    // Generate random choice for computer

    int getComputerChoice() {
        return rand() % 3 + 1; // 1=Rock, 2=Paper, 3=Scissors
    }

    // Convert choice number to name

    string choiceName(int n) {
        if (n == 1) return "Rock";
        if (n == 2) return "Paper";
        return "Scissors";
    }

    // Start the actual game loop

    void startGame(int index) {
        srand(time(0));

        while (true) {
            system("CLS");
            cout << "\n Game Start \n\n";
            cout << "Rock = 1, Paper = 2, Scissors = 3\n";
            cout << "Pause = P/p , Resume = R/r\n\n";
            cout << endl;
            cout << "Enter your choice: ";

            char x;
            cin >> x;

            // Pause & Resume
            if (x == 'P' || x == 'p') {
                cout << "Game Paused! Press R/r to resume...\n";
                while (true) {
                    cin >> x;
                    if (x == 'R' || x == 'r') break;
                }
                continue;
            }

            int playerChoice = x - '0';
            if (playerChoice < 1 || playerChoice > 3) {
                cout << "Invalid Choice!\n";
                continue;
            }

            int comp = getComputerChoice();

            cout << "You chose: " << choiceName(playerChoice) << endl;
             cout << endl;
            cout << "Computer chose: " << choiceName(comp) << endl;

            if (playerChoice == comp) cout << "DRAW!\n";
            else if ((playerChoice == 1 && comp == 3) ||
                     (playerChoice == 2 && comp == 1) ||
                     (playerChoice == 3 && comp == 2)) {
                players[index].winGame();
                 cout << endl;
                cout << "YOU WIN!\n";
            } else {
                players[index].loseGame();
                 cout << endl;
                cout << "YOU LOSS!\n";
            }

            saveToFile(); // Save after every game

            cout << "\nGAME OVER \n";
            cout << endl;
            cout << "\nSCORE UPDATED \n";
            cout << endl;
            players[index].showProfile();

            cout << "\n1. Go to the Menu\n\n2. Start New Game\n\n3. Exit The Game\n";
            cout << endl;
            cout << "Please Enter Your Choice Number :";
            int op;
            cin >> op;

            if (op == 1) return;
            if (op == 3) exit(0);
        }
    }
};

// Main Function

int main() {
    Game game;           // Create Game object
    game.mainMenu();     // Start the main menu
    return 0;
}
