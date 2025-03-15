#include <iostream>
#include <math.h>
#include <string>


struct Point
{
    int x, y;
};

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

#define NO_COMMUNICATES 0
#define COMMUNICATES 1

#define WHITE 3
#define BLACK 4

enum Direction
{
    DOWN,
    UP,
    RIGHT,
    LEFT,
    RIGHT_DOWN,
    LEFT_UP
};


class Game {
private:
    int* board;
    int s, k, gw, gb;
    int size;
    int amountW, amountB;
    int turn;
    bool is_empty;

    void instantiatePieces() {
        board[size + 1] = WHITE;
        board[size + s] = BLACK;
        board[s * size + 1] = BLACK;
        board[s * size + size - 2] = WHITE;
        board[size * (size - 2) + s] = WHITE;
        board[size * (size - 2) + size - 2] = BLACK;
        amountW = gw - 3;
        amountB = gb - 3;
    }

    void drawDigit(int x) {
        if (board[x] == BLACK)
            std::cout << "B ";
        else if (board[x] == WHITE)
            std::cout << "W ";
        else
            std::cout << "_ ";
    }

    void getDigit(int x, char a) {
        if (a == '_') board[x] = 0;
        else if (a == 'W') board[x] = WHITE;
        else if (a == 'B') board[x] = BLACK;
    }

public:
    Game(int s, int k, int gw, int gb) : s(s), k(k), gw(gw), gb(gb) {
        size = (s * 2 + 1); //s * 2 - 1 array size without borders
        turn = WHITE;
        board = new int[size * size];
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (j > s + i || j < i - s)
                    board[i * size + j] = 1;
                else if (j == s + i ||
                    j == i - s ||
                    j == 0 ||
                    i == 0 ||
                    j == size - 1 ||
                    i == size - 1)
                    board[i * size + j] = 2;
                else
                    board[i * size + j] = 0;
            }
        }
        instantiatePieces();
        is_empty = false;
    }

    Game(Game& copy) {
        s = copy.s;
        k = copy.k;
        gw = copy.gw;
        gb = copy.gb;
        size = copy.size;
        amountW = copy.amountW;
        amountB = copy.amountB;
        turn = copy.turn;
        is_empty = copy.is_empty;
        board = new int[size * size];
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                board[i * size + j] = copy.board[i * size + j];
            }
        }

    }

    ~Game() {
        delete[] board;
    }

    void createNew(int news, int newk, int newgw, int newgb, int newamountW, int newamountB, char newturn) {
        s = news;
        k = newk;
        gw = newgw;
        gb = newgb;
        amountW = newamountW;
        amountB = newamountB;
        if (newturn == 'W') turn = WHITE;
        else turn = BLACK;
        size = (s * 2 + 1); //s * 2 - 1 array size without borders
        delete[] board;
        board = new int[size * size];
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (j > s + i || j < i - s)
                    board[i * size + j] = 1;
                else if (j == s + i ||
                    j == i - s ||
                    j == 0 ||
                    i == 0 ||
                    j == size - 1 ||
                    i == size - 1)
                    board[i * size + j] = 2;
                else
                    board[i * size + j] = 0;
            }
        }
        is_empty = false;
    }

    //draws game board
    void Draw() {
        if (is_empty) {
            std::cout << "EMPTY_BOARD\n";
            return;
        }
        std::cout << s << " " << k << " " << gw << " " << gb << "\n";
        std::cout << amountW << " " << amountB << " ";
        if (turn == WHITE) std::cout << "W";
        else std::cout << "B";
        for (int i = 0; i < size; i++) {
            for (int j = 0; (j < s - i && i <= s) || (j < i - s && i > s); j++) {
                std::cout << " ";
            }
            for (int j = 0; j < size; j++) {
                if (!(j > s + i || j < i - s || j == s + i || j == i - s ||
                    j == 0 || i == 0 || j == size - 1 || i == size - 1)) {
                    drawDigit(i * size + j);
                }
            }
            std::cout << std::endl;
        }
    }
    
    //draws full encoded array
    void DrawFull() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                std::cout << board[i * size + j];
            }
            std::cout << std::endl;
        }
    }

    //Draws encoded board with borders
    void DrawWithBorder() {
        for (int i = 0; i < size; i++) {
            if (i <= s) {
                for (int k = i; k < s; k++) {
                    std::cout << " ";
                }
                for (int j = 0; j < size; j++) {
                    if (j <= s + i) {
                        std::cout << board[i * size + j] << " ";
                    }
                }
            }
            else {
                for (int k = size - i; k <= s; k++) {
                    std::cout << " ";
                }
                for (int j = 0; j < size; j++) {
                    if (j + s >= i) {
                        std::cout << board[i * size + j] << " ";
                    }
                }
            }
            std::cout << std::endl;
        }
    }

    int horizontalRowsCheck() {
        int amount = 0;
        int Wpieces = 0;
        int Bpieces = 0;
        //for each horizontal line
        for (int i = 1; i < size - 1; i++) {
            int start = max(i - s + 1, 1);
            Bpieces = 0;
            Wpieces = 0;
            //iterate throught whole line
            for (int j = start; j < min(i + s, size - 1); j++) {
                if (board[i * size + j] == WHITE)
                {
                    Wpieces++;
                    Bpieces = 0;
                }
                else if (board[i * size + j] == BLACK) {
                    Bpieces++;
                    Wpieces = 0;
                }
                else {
                    Bpieces = 0;
                    Wpieces = 0;
                }
                if (Wpieces == k || Bpieces == k) {
                    amount++;
                    break;
                }
            }
        }
        return amount;
    }

    int verticalRowsCheck() {
        int amount = 0;
        int Wpieces = 0;
        int Bpieces = 0;
        //for each vertical line
        for (int j = 1; j < size - 1; j++) {
            int start = max(j - s + 1, 1);
            Bpieces = 0;
            Wpieces = 0;
            //iterate throught whole line
            for (int i = start; i < min(j + s, size - 1); i++)
            {
                if (board[i * size + j] == WHITE)
                {
                    Wpieces++;
                    Bpieces = 0;
                }
                else if (board[i * size + j] == BLACK) {
                    Bpieces++;
                    Wpieces = 0;
                }
                else {
                    Bpieces = 0;
                    Wpieces = 0;
                }
                if (Wpieces == k || Bpieces == k) {
                    amount++;
                    break;
                }
            }
        }
        return amount;
    }

    int diagonalRowsCheck() {
        int amount = 0;
        int Wpieces = 0;
        int Bpieces = 0;
        //for each diagonal
        for (int d = 1; d < size - 1; d++) {
            int ile = 0;
            //go to the top left
            while (board[(d - ile - 1) * size + (s - ile - 1)] != 2) {
                ile++;
            }
            Bpieces = 0;
            Wpieces = 0;
            //until bottom right is hit
            while (board[(d - ile) * size + (s - ile)] != 2) {
                if (board[(d - ile) * size + s - ile] == WHITE)
                {
                    Wpieces++;
                    Bpieces = 0;
                }
                else if (board[(d - ile) * size + s - ile] == BLACK) {
                    Bpieces++;
                    Wpieces = 0;
                }
                else {
                    Bpieces = 0;
                    Wpieces = 0;
                }
                if (Wpieces == k || Bpieces == k) {
                    amount++;
                    break;
                }
                ile--;
            }
        }
        return amount;
    }

    //checks the number of completed rows on the board
    int checkRows() {
        int amount = 0;
        amount += horizontalRowsCheck();
        amount += verticalRowsCheck();
        amount += diagonalRowsCheck();
        return amount;
    }


    void loadBoard(Game& currentGame) {
        char a;
        a = getchar(); //new line
        int expectedW = currentGame.gw; //at the end must be equal to amountW
        int expectedB = currentGame.gb; //at the end must be equal to amountB
        bool goodLineLength = true;

        bool endRead;//end of line

        for (int i = 0; i < size; i++) {
            for (int j = 0; (j < s - i && i <= s && i > 0) || (j < i - s && i > s && i < size - 1); j++) {
                a = getchar();
            }
            endRead = false;
            for (int j = 0; j < size; j++) {
                if (!(j > s + i || j < i - s || j == s + i || j == i - s ||
                    j == 0 || i == 0 || j == size - 1 || i == size - 1)) {
                    if (endRead == false) {
                        a = getchar();
                        if (a == 'W') expectedW--;
                        else if (a == 'B') expectedB--;
                        else if (a != '_') {
                            goodLineLength = false;
                            break;
                        }
                        getDigit(i * size + j, a);
                        a = getchar();
                        if (a == '\n') {
                            endRead = true;
                        }
                    }
                    else {
                        goodLineLength = false;//line ended to quickly
                        break;
                    }
                }
            }
            while (a != '\n') {
                if (a != '\n' && a != ' ') {
                    goodLineLength = false;//too much characters in one line
                }
                a = getchar();
                if (a == EOF) break;
            }
        }

        int rows = 0;
        rows = currentGame.checkRows();
        is_empty = true;
        if (rows == 1) {
            printf("ERROR_FOUND_%d_ROW_OF_LENGTH_K\n", rows);
        }
        else if (rows > 1) {
            printf("ERROR_FOUND_%d_ROWS_OF_LENGTH_K\n", rows);
        }
        else if (goodLineLength == false) {
            std::cout << "WRONG_BOARD_ROW_LENGTH\n";
        }
        else if (expectedW < amountW) {
            std::cout << "WRONG_WHITE_PAWNS_NUMBER\n";
        }
        else if (expectedB < amountB) {
            std::cout << "WRONG_BLACK_PAWNS_NUMBER\n";
        }
        else {
            std::cout << "BOARD_STATE_OK\n";
            is_empty = false;
        }
    }

    //converts string into a 2D Vector
    Point covertChords(std::string point) {
        Point result;
        result.x = point[0] - 97;
        result.y = min(s + result.x, size - 1) - (point[1] - 49);
        return result;
    }

    //endoces direction from 2 points into an integer
    int checkDirection(Point& fromChords, Point& toChords) {
        if (fromChords.y + 1 == toChords.y && fromChords.x == toChords.x) {
            return DOWN;
        }
        else if (fromChords.y - 1 == toChords.y && fromChords.x == toChords.x) {
            return UP;
        }
        else if (fromChords.y == toChords.y && fromChords.x + 1 == toChords.x) {
            return RIGHT;
        }
        else if (fromChords.y == toChords.y && fromChords.x - 1 == toChords.x) {
            return LEFT;
        }
        else if (fromChords.y + 1 == toChords.y && fromChords.x + 1 == toChords.x) {
            return RIGHT_DOWN;
        }
        else if (fromChords.y - 1 == toChords.y && fromChords.x - 1 == toChords.x) {
            return LEFT_UP;
        }
        else {
            return -1;
        }
    }


    //checks if move starting point is valid
    bool startFieldBad(Point from) {
        if (board[(from.y * size) + from.x] == 2) {
            return false;
        }
        else {
            return true;
        }
    }

    //checks if move destination is valid
    bool endFieldBad(Point to) {
        if (board[(to.y * size) + to.x] == 2) {
            return true;
        }
        else {
            return false;
        }
    }

    //translates encoded direction into a 2D vector
    Point translateDirection(int direction) {
        Point move;
        if (direction == DOWN) {
            move.x = 0;
            move.y = 1;
        }
        else if (direction == UP) {
            move.x = 0;
            move.y = -1;
        }
        else if (direction == RIGHT) {
            move.x = 1;
            move.y = 0;

        }
        else if (direction == LEFT) {
            move.x = -1;
            move.y = 0;
        }
        else if (direction == RIGHT_DOWN) {
            move.x = 1;
            move.y = 1;
        }
        else if (direction == LEFT_UP) {
            move.x = -1;
            move.y = -1;
        }
        else {
            move.x = 0;
            move.y = 0;
            throw _EXCEPTION_;
        }
        return move;
    }

    //checks if the row is full
    bool rowBad(Point fromChords, Point toChords, int direction) {
        Point next = toChords;
        Point move;
        bool ifBad = true;
        move = translateDirection(direction);
        while (board[next.y * size + next.x] != 2) {
            if (board[next.y * size + next.x] == 0) {
                ifBad = false;
            }
            next.x += move.x;
            next.y += move.y;
        }

        return ifBad;
    }

    void horizontalRemove() {
        int Wpieces = 0;
        int Bpieces = 0;
        //for each horizontal line
        for (int i = 1; i < size - 1; i++) {
            int start = max(i - s + 1, 1);
            Bpieces = 0;
            Wpieces = 0;
            //untile edge of the board is hit
            for (int j = start; j < min(i + s, size - 1); j++) {
                if (board[i * size + j] == WHITE)
                {
                    Wpieces++;
                    Bpieces = 0;
                }
                else if (board[i * size + j] == BLACK) {
                    Bpieces++;
                    Wpieces = 0;
                }
                else {
                    Bpieces = 0;
                    Wpieces = 0;
                }
                //removing row
                if (Wpieces == k || Bpieces == k) {
                    int offset = 0;
                    //go to the end of the row
                    while(board[i * size + j + offset] != 2 && board[i * size + j + offset] != 0) {
                        offset -= 1;
                    }
                    offset += 1;
                    //iterate through whole row
                    while (board[i * size + j + offset] != 2 && board[i * size + j + offset] != 0) {
                        if (Wpieces == k) {
                            if (board[i * size + j + offset] == WHITE)
                                amountW++;
                        }
                        else {
                            if (board[i * size + j + offset] == BLACK)
                                amountB++;
                        }
                        board[i * size + j + offset] = 0;
                        offset += 1;
                    }
                    Wpieces = 0;
                    Bpieces = 0;
                }
            }
        }
    }

    void verticalRemove() {
        int Wpieces = 0;
        int Bpieces = 0;
        //for each vertical line
        for (int j = 1; j < size - 1; j++) {
            int start = max(j - s + 1, 1);
            Bpieces = 0;
            Wpieces = 0;
            //untile edge of the board is hit
            for (int i = start; i < min(j + s, size - 1); i++)
            {
                if (board[i * size + j] == WHITE)
                {
                    Wpieces++;
                    Bpieces = 0;
                }
                else if (board[i * size + j] == BLACK) {
                    Bpieces++;
                    Wpieces = 0;
                }
                else {
                    Bpieces = 0;
                    Wpieces = 0;
                }
                //removing row
                if (Wpieces == k || Bpieces == k) {
                    int offset = 0;
                    //go to the end of the row
                    while (board[(i + offset) * size + j] != 2 && board[(i + offset) * size + j] != 0) {
                        offset += 1;
                    }
                    offset -= 1;
                    //iterate through whole row
                    while (board[(i + offset) * size + j] != 2 && board[(i + offset) * size + j] != 0) {
                        if (Wpieces == k) {
                            if (board[(i + offset) * size + j] == WHITE)
                                amountW++;
                        }
                        else {
                            if (board[(i + offset) * size + j] == BLACK)
                                amountB++;
                        }
                        board[(i + offset) * size + j] = 0;
                        offset -= 1;
                    }
                    Wpieces = 0;
                    Bpieces = 0;
                }
            }
        }
    }

    void diagonalRemove() {
        int amount = 0;
        int Wpieces = 0;
        int Bpieces = 0;
        //for each diagonal
        for (int d = 1; d < size - 1; d++) {
            int ile = 0;
            //go to the top left
            while (board[(d - ile - 1) * size + (s - ile - 1)] != 2) {
                ile++;
            }
            Bpieces = 0;
            Wpieces = 0;
            //untile bottom right is hit
            while (board[(d - ile) * size + (s - ile)] != 2) {
                if (board[(d - ile) * size + s - ile] == WHITE)
                {
                    Wpieces++;
                    Bpieces = 0;
                }
                else if (board[(d - ile) * size + s - ile] == BLACK) {
                    Bpieces++;
                    Wpieces = 0;
                }
                else {
                    Bpieces = 0;
                    Wpieces = 0;
                }
                //removing row
                if (Wpieces == k || Bpieces == k) {
                    int offset = 0;
                    //go to the end of the row
                    while (board[(d - ile + offset) * size + (s - ile + offset)] != 2 && board[(d - ile + offset) * size + (s - ile + offset)] != 0) {
                        offset += 1;
                    }
                    offset -= 1;
                    //iterate through whole row
                    while (board[(d - ile + offset) * size + (s - ile + offset)] != 2 && board[(d - ile + offset) * size + (s - ile + offset)] != 0){
                        if (Wpieces == k) {
                            if(board[(d - ile + offset) * size + (s - ile + offset)] == WHITE)
                            amountW++;
                        }
                        else {
                            if (board[(d - ile + offset) * size + (s - ile + offset)] == BLACK)
                            amountB++;
                        }
                        board[(d - ile + offset) * size + (s - ile + offset)] = 0;
                        offset -= 1;
                    }
                    Wpieces = 0;
                    Bpieces = 0;
                }
                ile--;
            }
        }
    }

    void removeRows() {
        horizontalRemove();
        verticalRemove();
        diagonalRemove();
    }

    //moves pieces on the board and updates amount of spare pieces
    void executeMove(Point fromChords, Point toChords, int direction) {
        Point move = translateDirection(direction);
        Point current = toChords;
        if (board[current.y * size + current.x] == 0) {
            board[current.y * size + current.x] = turn;
        }
        else {
            bool end = false;
            int tmpColor;
            int setColor = turn;
            while (!end) {
                tmpColor = board[current.y * size + current.x];
                board[current.y * size + current.x] = setColor;
                setColor = tmpColor;
                current.x += move.x;
                current.y += move.y;
                if (tmpColor == 0)
                    end = true;
            }
        }
        if (turn == WHITE) {
            amountW--;
            turn = BLACK;
        }
        else {
            amountB--;
            turn = WHITE;
        }
    }


    //checks is provided indexes are correct and executes move
    bool checkIndex(std::string& move, int communicates = COMMUNICATES) {
        std::string from = "";
        std::string to = "";
        bool first = true;
        for (int i = 0; i < move.size(); i++) {
            if (first) {
                if (move[i] != '-')
                    from.push_back(move[i]);
                else
                    first = false;
            }
            else {
                to.push_back(move[i]);
            }
        }
        Point fromChords;
        Point toChords;
        fromChords = covertChords(from);
        toChords = covertChords(to);

        bool outOfBounds = false;
        std::string outOfBoundsStr = "";

        if (fromChords.y < size && fromChords.y >= 0 && fromChords.x < size && fromChords.x >= 0) {
            if (board[fromChords.y * size + fromChords.x] == 1) {
                outOfBounds = true;
                outOfBoundsStr = from;
            }
        }
        else {
            outOfBoundsStr = from;
            outOfBounds = true;
        }

        if (!outOfBounds) {
            if (toChords.y < size && toChords.y >= 0 && toChords.x < size && toChords.x >= 0) {
                if (board[toChords.y * size + toChords.x] == 1) {
                    outOfBounds = true;
                    outOfBoundsStr = to;
                }
            }
            else {
                outOfBoundsStr = to;
                outOfBounds = true;
            }
        }

        bool checkFields = false;
        int direction;
        if (outOfBounds)
            std::cout << "BAD_MOVE_" << outOfBoundsStr << "_IS_WRONG_INDEX\n";
        else {
            direction = checkDirection(fromChords, toChords);
            if (direction == -1) {
                std::cout << "UNKNOWN_MOVE_DIRECTION\n";
            }
            else {
                checkFields = true;
            }
        }

        bool checkRow = false;
        if (checkFields) {
            if (!startFieldBad(fromChords)) {
                if (!endFieldBad(toChords)) {
                    checkRow = true;
                }
                else {
                    std::cout << "BAD_MOVE_" << to << "_IS_WRONG_DESTINATION_FIELD\n";
                }
            }
            else {
                std::cout << "BAD_MOVE_" << from << "_IS_WRONG_STARTING_FIELD\n";
            }
        }

        if (checkRow) {
            if (!rowBad(fromChords, toChords, direction)) {
                if(communicates == COMMUNICATES)
                    std::cout << "MOVE_COMMITTED\n";
                executeMove(fromChords, toChords, direction);
                return true;
            }
            else {
                std::cout << "BAD_MOVE_ROW_IS_FULL\n";
            }
        }

        return false;
    }


    //checks validity of rows that are specified to be deleted
    bool checkDeletionRows(std::string& color, std::string& first_index, std::string& last_index) {
        int intColor;
        if (color[0] == 'w') intColor = WHITE;
        else if (color[0] == 'b') intColor = BLACK;
        else {
            std::cout << "COLOR: " << color << "\n";
        }
        bool good = true;
        int direction;
        Point a = covertChords(first_index);
        Point b = covertChords(last_index);

        //checking index
        if (a.x >= size || b.x >= size || a.x < 0 || b.x < 0 || board[a.y * size + a.x] == 2 || board[b.y * size + b.x] == 2 || board[a.y * size + a.x] == 1 || board[b.y * size + b.x] == 1) {
            good = false;
        }
        if (good) {
            if (a.x == b.x) {
                if (a.y > b.y) {
                    direction = DOWN;
                }
                else if (a.y < b.y) {
                    direction = UP;
                }
                else {
                    good = false;
                }
            }
            else if (a.y == b.y) {
                if (a.x > b.x) {
                    direction = LEFT;
                }
                else if (a.x < b.x) {
                    direction = RIGHT;
                }
                else {
                    good = false;
                }
            }
            else if ((a.x - b.x) == (a.y - b.y)) {
                if (a.x > b.x) {
                    direction = RIGHT_DOWN;
                }
                else if (a.x < b.x) {
                    direction = LEFT_UP;
                }
                else {
                    good = false;
                }
            }
            else {
                good = false;
            }
        }

        if (direction == LEFT || direction == RIGHT) {
            if (((a.x - b.x) < 3 && (a.x - b.x) >= 0) || ((a.x - b.x) > -3 && (a.x - b.x) <= 0)) {
                good = false;
            }
        }
        else{
            if (((a.y - b.y) < 3 && (a.y - b.y) >= 0) || ((a.y - b.y) > -3 && (a.y - b.y) <= 0)) {
                good = false;
            }
        }

        //checking colour
        if (good) {
            Point move = translateDirection(direction);
            Point current = a;
            int Wamount = 0;
            int Bamount = 0;
            if (board[current.y * size + current.x] == WHITE)
                Wamount++;
            else if (board[current.y * size + current.x] == BLACK)
                Bamount++;
            while(true){
                current.x -= move.x;
                current.y -= move.y;
                if (board[current.y * size + current.x] == WHITE)
                    Wamount++;
                else if (board[current.y * size + current.x] == BLACK)
                    Bamount++;
                if (current.x == b.x && current.y == b.y)
                    break;
            } 

            if ((Wamount >= 4 && intColor == WHITE) || (Bamount >= 4 && intColor == BLACK)){
                return true;
            }
            else {
                std::cout << "WRONG_COLOR_OF_CHOSEN_ROW\n";
                return false;
            }

        }
        else {
            std::cout << "WRONG_INDEX_OF_CHOSEN_ROW\n";
            return false;
        }
    }


    int setDirection(Point& a, Point& b) {
        int direction = -1;
        if (a.x == b.x) {
            if (a.y > b.y) {
                direction = DOWN;
            }
            else if (a.y < b.y) {
                direction = UP;
            }
        }
        else if (a.y == b.y) {
            if (a.x > b.x) {
                direction = LEFT;
            }
            else if (a.x < b.x) {
                direction = RIGHT;
            }
        }
        else if ((a.x - b.x) == (a.y - b.y)) {
            if (a.x > b.x) {
                direction = RIGHT_DOWN;
            }
            else if (a.x < b.x) {
                direction = LEFT_UP;
            }
        }
        return direction;
    }


    //removes specified row
    void deleteRow(std::string& color, std::string& first_index,std::string& last_index) {
        Point a = covertChords(first_index);
        Point b = covertChords(last_index);
        int direction;

        direction = setDirection(a,b);

        Point move = translateDirection(direction);
        Point current = a;
        while (true) {

            if (board[current.y * size + current.x] == WHITE) {
                if (color[0] == 'w') {
                    amountW++;
                }
                board[current.y * size + current.x] = 0;
            }
            else if (board[current.y * size + current.x] == BLACK) {
                if (color[0] == 'b') {
                    amountB++;
                }
                board[current.y * size + current.x] = 0;
            }
            current.x -= move.x;
            current.y -= move.y;
            if (board[current.y * size + current.x] == 0 || board[current.y * size + current.x] == 2)
                break;
        }
        current = a;
        while (true) {

            if (board[current.y * size + current.x] == WHITE) {
                if (color[0] == 'w') {
                    amountW++;
                }
                board[current.y * size + current.x] = 0;
            }
            else if (board[current.y * size + current.x] == BLACK) {
                if (color[0] == 'b') {
                    amountB++;
                }
                board[current.y * size + current.x] = 0;
            }
            current.x += move.x;
            current.y += move.y;
            if (board[current.y * size + current.x] == 0 || board[current.y * size + current.x] == 2)
                break;
        }
    }


    //checks amount of words sepatated by space in a given string
    int wordAmount(std::string& bufor) {
        bool word = false;
        int counter = 0;
        for (int i = 0; i < bufor.size(); i++) {
            if (bufor[i] != ' ' && word == false) {
                word = true;
                counter++;
            }
            if (bufor[i] == ' ' && word == true) {
                word = false;
            }
        }
        return counter;
    }


    //parses DO_MOVE input and executes move
    void DoMove()
    {
        bool index;
        bool word = false;
        int counter = 0;
        std::string bufor;
        std::string move;
        char a = getchar();
        //reading input to bufor
        while (a != '\n') {
            bufor.push_back(a);
            a = getchar();
        }

        counter = wordAmount(bufor);

        if (counter == 1) {
            for (int i = 0; i < bufor.size(); i++) {
                if (bufor[i] != ' ') {
                    move.push_back(bufor[i]);
                }
            }
            index = checkIndex(move);
            if(index) removeRows();
        }
        else if (counter == 4) {
            counter = 0;
            std::string color;
            std::string first_index;
            std::string last_index;
            //dividing bufor into separate words
            for (int i = 0; i < bufor.size(); i++) {
                if (bufor[i] != ' ' && word == false) {
                    word = true;
                    counter++;
                }
                if (bufor[i] == ' ' && word == true) {
                    word = false;
                }
                if (word == true) {
                    if (counter == 1)
                        move.push_back(bufor[i]);
                    else if (counter == 2)
                        color.push_back(bufor[i]);
                    else if (counter == 3)
                        first_index.push_back(bufor[i]);
                    else if (counter == 4)
                        last_index.push_back(bufor[i]);
                }
            }
            //executing move
            Game* test = new Game(*this);
            index = test->checkIndex(move, NO_COMMUNICATES);
            if (index) {
                bool r = test->checkDeletionRows(color, first_index, last_index);
                if (r) {
                    checkIndex(move);
                    deleteRow(color,first_index, last_index);
                }

            }
            delete test;
        }
    }
};

class Listener {
    std::string str;

    void loadGameBoard(Game& currentGame) {
        int s, k, gw, gb, w, b;
        char turn;
        std::cin >> s >> k >> gw >> gb >> w >> b >> turn;
        currentGame.createNew(s, k, gw, gb, w, b, turn);
        currentGame.loadBoard(currentGame);
    }

public:
    Listener() {};
    void Listen(Game currentGame) {
        while (std::cin) {
            std::cin >> str;
            if (std::cin) {
                if (str == "LOAD_GAME_BOARD")
                    loadGameBoard(currentGame);
                else if (str == "PRINT_GAME_BOARD")
                    currentGame.Draw();
                else if (str == "DO_MOVE")
                    currentGame.DoMove();
                else if (str == "P2") {
                    currentGame.DrawFull();
                }
            }
        }
    }
};

int main()
{
    Game* currentGame = new Game(4, 4, 15, 15);
    Listener listener;
    listener.Listen(*currentGame);
}
