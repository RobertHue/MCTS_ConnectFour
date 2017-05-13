
#include "GamePanel.h"

int GamePanel::getNumOfFreeFields() const {
    return this->numOfFreeFields;
}

Position GamePanel::getPositionOfLastPlacedToken() const {
    return this->positionOfLastPlacedToken;
}

vector<vector<int>> GamePanel::getGameData() const {
    return gameData;
}

void GamePanel::setTurnPlayer(Player turnPlayer) {
    this->turnPlayer = turnPlayer;
}

Player GamePanel::getTurnPlayer() const {
    return this->turnPlayer;
}

Player GamePanel::getOtherPlayer() const {
    return this->otherPlayer;
}

void GamePanel::nextTurn() {
    if (turnPlayer == PLAYER_1) {
        turnPlayer = PLAYER_2;
        otherPlayer = PLAYER_1;
    } else {
        turnPlayer = PLAYER_1;
        otherPlayer = PLAYER_2;
    }
}

/*
Allgemein:
        gibt den Spieler (der zuvor ein Token platziert hat) zurck, der kurz davor ist, eine 4er-Reihe zu vervollstndigen.
        gibt den Spieler (der zuvor ein Token platziert hat) zurck, der eine 3er-Reihe besitzt, die durch Platzieren eines Tokens vervollstndigt werden kann

Falls es keine solche 3er-Reihe gibt, wird NONE zurckgegeben

Beispiele:
        "X,X,X,O" => NONE;  "X,X,X,_" oder "X,_,X,X" oder "O,X,X,_,X" => Player X
        "_,_,X,_,X,_,X,_,X,X"
 */
Player GamePanel::isAboutToWin() {
    bool closeFreeField = false;
    int x_placed = positionOfLastPlacedToken.x;
    int y_placed = positionOfLastPlacedToken.y;

    // wer hat dieses Token berhaupt platziert
    Player tokenPlacedByPlayer, otherPlayer;
    if (gameData[x_placed][y_placed] == PLAYER_1) {
        tokenPlacedByPlayer = PLAYER_1;
        otherPlayer = PLAYER_2;
    } else {
        tokenPlacedByPlayer = PLAYER_2;
        otherPlayer = PLAYER_1;
    }

    // immer mitz�hlen wieviel tokens in einer Reihe:
    int num_of_tokens_in_row = 0;


    int offsets[][2] = {
        { -3, 0},
        { -2, 0},
        { -1, 0},
        { 0, 0},
        { 1, 0},
        { 2, 0},
        { 3, 0}, // horizontal
        { 0, -3},
        { 0, -2},
        { 0, -1},
        { 0, 0},
        { 0, 1},
        { 0, 2},
        { 0, 3}, // vertical
        { -3, -3},
        { -2, -2},
        { -1, -1},
        { 0, 0},
        { 1, 1},
        { 2, 2},
        { 3, 3}, // diagonal1
        { -3, 3},
        { -2, 2},
        { -1, 1},
        { 0, 0},
        { 1, -1},
        { 2, -2},
        { 3, -3} // diagonal2
    };

    // berprfe vom platzierten Spielstein aus alle 8 Richtungen, und zhle die Anzahl der Spielsteine in einer Reihe
    for (size_t i = 0; i < ((sizeof (offsets) / sizeof (int)) / 2); ++i) {
        // neue Zeile in der offset_Matrix => neue berprfung der Anzahl der Token in einer Reihe
        if ((i % 7) == 0) {
            num_of_tokens_in_row = 0;
            closeFreeField = false;
        }
        int x_check = x_placed + offsets[i][0];
        int y_check = y_placed + offsets[i][1];

        // Plausi-Check (Grenzwerte des GamePanels erreicht):
        if (x_check < 0 || x_check >= MAX_X) continue;
        if (y_check < 0 || y_check >= MAX_Y) continue;

        if (this->gameData[x_check][y_check] == tokenPlacedByPlayer) {
            ++num_of_tokens_in_row;
        } else if (this->gameData[x_check][y_check] == FREE_FIELD) {
            // falls zum zweiten mal ein freies Feld kam, setzte die zahl zur�ck
            if (closeFreeField == true) {
                num_of_tokens_in_row = 0;
                continue;
            }
            closeFreeField = true;
        } else if (this->gameData[x_check][y_check] == otherPlayer) { // auf Position befindet sich ein gegnerischer Spielstein (Token)
            num_of_tokens_in_row = 0;
            closeFreeField = false;
            continue;
        }

        // hat der turnPlayer eine 3er-Reihe (wird nur �berpr�ft falls ein FREE_FIELD vorkam!)
        if (closeFreeField == true && num_of_tokens_in_row >= 3)
            return turnPlayer;
    }


    return NONE; // der turnPlayer hat keine 3er-Reihe mit offenem Feld
}



// gibt den Spieler, der gewonnen hat zuck;
// gibt NONE (0) zur�ck falls unentschieden oder gewinner noch nicht feststeht

Player GamePanel::hasSomeoneWon() {
    // todo nicht alle diagonalen werden berprft
    // evtl besser: dort wo spielstein platziert wurde 3 spielsteine in alle richtungen prfen
    // Player hasWon;
    int x_placed = positionOfLastPlacedToken.x;
    int y_placed = positionOfLastPlacedToken.y;

    // immer mitzhlen wieviel tokens in einer Reihe:
    int num_of_tokens_p1_in_row = 0;
    int num_of_tokens_p2_in_row = 0;


    int offsets[][2] = {

        { -3, 0},
        { -2, 0},
        { -1, 0},
        { 0, 0},
        { 1, 0},
        { 2, 0},
        { 3, 0}, // horizontal
        { 0, -3},
        { 0, -2},
        { 0, -1},
        { 0, 0},
        { 0, 1},
        { 0, 2},
        { 0, 3}, // vertical
        { -3, -3},
        { -2, -2},
        { -1, -1},
        { 0, 0},
        { 1, 1},
        { 2, 2},
        { 3, 3}, // diagonal1
        { -3, 3},
        { -2, 2},
        { -1, 1},
        { 0, 0},
        { 1, -1},
        { 2, -2},
        { 3, -3} // diagonal2
    };
    // cout << sizeof(offsets) << endl;

    // berprfe vom platzierten Spielstein aus alle 8 Richtungen, und zhle die Anzahl der Spielsteine in einer Reihe
    for (size_t i = 0; i < ((sizeof (offsets) / sizeof (int)) / 2); ++i) {
        // neue Zeile in der offset_Matrix => neue berprfung der Anzahl der Token in einer Reihe
        if ((i % 7) == 0) {
            num_of_tokens_p1_in_row = 0;
            num_of_tokens_p2_in_row = 0;
        }
        int x_check = x_placed + offsets[i][0];
        int y_check = y_placed + offsets[i][1];

        // Plausi-Check (Grenzwerte des GamePanels erreicht):
        if (x_check < 0 || x_check >= MAX_X) continue;
        if (y_check < 0 || y_check >= MAX_Y) continue;


        if (this->gameData[x_check][y_check] == PLAYER_1) {
            num_of_tokens_p2_in_row = 0;
            ++num_of_tokens_p1_in_row;
        } else if (this->gameData[x_check][y_check] == PLAYER_2) {
            num_of_tokens_p1_in_row = 0;
            ++num_of_tokens_p2_in_row;
        } else if (this->gameData[x_check][y_check] == FREE_FIELD) {
            num_of_tokens_p1_in_row = 0;
            num_of_tokens_p2_in_row = 0;
        }

        // hat wer gewonnen?
        if (num_of_tokens_p1_in_row >= 4) return PLAYER_1;
        else if (num_of_tokens_p2_in_row >= 4) return PLAYER_2;
    }


    return NONE; // niemand hat bis jetzt gewonnen!
}

void GamePanel::drawGamePanelOnConsole(vector<vector<int>> gameData, int MAX_X, int MAX_Y) {
    // system("cls");
    cout << "Connect-Four (" << MAX_Y << "x" << MAX_X << "):\n\n" << endl;
    cout << "  ";
    for (int x = 0; x < MAX_X; ++x) {
        cout << " " << x;
    }
    cout << "\n  ";


    for (int x = 0; x < MAX_X; ++x) {
        cout << "--";
    }
    cout << "-\n";


    for (int y = 0; y < MAX_Y; ++y) {
        cout << y << " ";
        for (int x = 0; x < MAX_X; ++x) {
            cout << "|";

            switch (gameData[x][y]) {
                case FREE_FIELD:
                    cout << " "; // freier Platz
                    break;
                case PLAYER_1:
                    cout << "O";
                    break;
                case PLAYER_2:
                    cout << "X";
                    break;
                default:
                    cout << "E"; // Error...
            }
        }
        // zwischenzeile fr Trennlinie
        cout << "|\n  ";
        for (int x = 0; x < MAX_X; ++x) {
            cout << "--";
        }
        cout << "-\n";
    }
    cout << endl;
}

int GamePanel::insertTokenIntoColumn(int column) {
    int row;
    for (row = MAX_Y - 1; row >= 0; --row) {

        if (gameData[column][row] == FREE_FIELD) {
            // fge Token in freier gefundener Zelle ein:
            gameData[column][row] = this->turnPlayer;
            // Merke die Position des zuletzt platzierten Tokens
            positionOfLastPlacedToken.x = column;
            positionOfLastPlacedToken.y = row;
            --numOfFreeFields;

            // wechsle TurnPlayer im Fall des gltigen Zuges
            nextTurn();
            return 0;
        }
    }
    // Token konnte der Spalte nicht hinzugefgt werden. Bitte nochmal whlen!
    // Grund:	Column ist bereits voll!!
    return -1;
}

int GamePanel::getMAX_X() const {
    return MAX_X;
}

int GamePanel::getMAX_Y() const {
    return MAX_Y;
}
