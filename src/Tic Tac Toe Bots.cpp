#include <cstdio>
#include <cstring>
using namespace std;

// Strategy-based tic-tac-toe player
class Tic_Tac_Toe_Player_1 {

    // The Tic-Tac-Toe board is represented by a 9-element vector
    // 2 represents a blank square
    // 3 represents an X
    // 5 represents an O
    int board[9];

    // Squares representing winning combinations
    int win_comb[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6},
                        {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

    // Attempts to capture the center square {4} if it is blank
    // otherwise any blank non-corner square {1, 3, 5, 7}
    int make2() {
        if (board[4] == 2)
            return 4;
        for (int i = 1; i <= 7; i += 2)
            if (board[i] == 2)
                return i;
    }

    // Return -1 if the player p cannot win on his next move
    // otherwise it returns the number of square that constitutes
    // a winning move
    int posswin(char p) {
        int i, j, prod;

        switch (p)  {
            case 'X':
                for (i = 0; i < 8; i++) {
                    for (j = 0, prod = 1; j < 3; j++)
                        prod *= board[win_comb[i][j]];
                    if (prod == 18) {   // 3 x 3 x 2 represents X can win on his next move
                        for (j = 0; j < 3; j++)
                            if (board[win_comb[i][j]] == 2)
                                return win_comb[i][j];   // return the blank square
                    }
                }
                break;
            case 'O':
                for (i = 0; i < 8; i++) {
                    for (j = 0, prod = 1; j < 3; j++)
                        prod *= board[win_comb[i][j]];
                    if (prod == 50) {   // 5 x 5 x 2 represents O can win on his next move
                        for (j = 0; j < 3; j++)
                            if (board[win_comb[i][j]] == 2)
                                return win_comb[i][j];   // return the blank square
                    }
                }
                break;
        }
        return -1;
    }

    // Returns any blank square
    int go_blank()  {
        for (int i = 0; i <= 8; i++)
            if (board[i] == 2)
                return i;
    }

public:

    // Given a board configuration and the turn number, it returns
    // the square number it wants to make a move on
    int play(int grid[9], int turn)    {
        memcpy(board, grid, 9 * sizeof (int));
        int move;

        // Playing strategy
        switch (turn)   {
            case 1:     // Capture upper-left corner
                return 0;
            case 2:     // if center is blank, capture it, otherwise capture the upper-left corner
                return (board[4] == 2) ? 4 : 0;
            case 3:     // if lower-right corner is blank, capture it, otherwise capture the upper-right corner
                return (board[8] == 2) ? 8 : 2;
            case 4:
                move = posswin('X');
                return (move != -1) ? move : make2();
            case 5:
                move = posswin('X');
                if (move != -1) return move;
                move = posswin('O');
                if (move != -1) return move;
                return (board[6] == 2) ? 6 : 2;
            case 6:
                move = posswin('O');
                if (move != -1) return move;
                move = posswin('X');
                return (move != -1) ? move : make2();
            case 7:
                move = posswin('X');
                if (move != -1) return move;
                move = posswin('O');
                return (move != -1) ? move : go_blank();
            case 8:
                move = posswin('O');
                if (move != -1) return move;
                move = posswin('X');
                return (move != -1) ? move : go_blank();
            case 9:
                move = posswin('X');
                if (move != -1) return move;
                move = posswin('O');
                return (move != -1) ? move : go_blank();
        }

        // invalid turn
        return -1;
    }

};

// Heuristic-based tic-tac-toe player
class Tic_Tac_Toe_Player_2 {
    // Squares representing winning combinations
    int win_comb[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6},
                        {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

    // Heuristic_Array[i][j] gives the utility value for player 'X' if some
    // row, column or diagonal has i 'X' markers and j 'O' markers
    // Similarly, it gives the utility value for player 'O' if some
    // row, column or diagonal has i 'O' markers and j 'X' markers
    int Heuristic_Array[4][4] = {
            {     0,   -10,  -100, -1000 },
            {    10,     0,     0,     0 },
            {   100,     0,     0,     0 },
            {  1000,     0,     0,     0 }};

    // Returns the utility value of the given position for the given player
    int evaluatePosition(int board[9], char p)    {
        int player, opponent, sum = 0, i, j, piece;

        for (i = 0; i < 8; i++)  {
            player = opponent = 0;
            for (j = 0; j < 3; j++)  {
                piece = board[win_comb[i][j]];
                if ((piece == 3 && p == 'X') || (piece == 5 && p == 'O'))
                    player++;
                else if (piece != 2)
                    opponent++;
            }
            sum += Heuristic_Array[player][opponent];
        }
        return sum;
    }

public:
    // Given a board configuration and the turn number, it returns
    // the square number it wants to make a move on
    int play(int board[9], int turn)   {
        int i, k, heuristic = -10000, utility, best = 0, worst, tmp;
        char player = (turn & 1) ? 'X' : 'O';
        char opponent = (turn & 1) ? 'O' : 'X';

        for(k = 0; k < 9; k++)  {
            if(board[k] == 2) { // found a blank square
                board[k] = (turn & 1) ? 3 : 5;  // try playing this move
                utility = evaluatePosition(board, player);

                worst = -10000;     // find the worst your opponent could do
                for (i = 0; i < 9; i++)  {
                    if(board[i] == 2) { // simulate a move by opponent
                        board[i] = (turn & 1) ? 5 : 3;
                        tmp = evaluatePosition(board, opponent);
                        if(tmp > worst)
                            worst = tmp;
                        board[i] = 2;
                    }
                }

                // opponent had no legal move
                if(worst == -10000)
                    worst = 0;

                utility -= worst;
                if(utility > heuristic) {
                    heuristic = utility;
                    best = k;
                }
                board[k] = 2;
            }
        }

        return best;
    }
};

// Plays a game between the two Tic_Tac_Toe_Players
class Judge {
    int board[9];

    // Squares representing winning combinations
    int win_comb[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6},
                        {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

    // Returns true if player 'p' has won, false otherwise
    bool check_win(char p)    {
        int i, j, prod;
        for (i = 0; i < 8; i++) {
            for (j = 0, prod = 1; j < 3; j++)
                prod *= board[win_comb[i][j]];
            if ((prod == 27 && p == 'X') || (prod == 125 && p == 'O'))
                return true;
        }
        return false;
    }

    // Display board on the console
    void print_board()  {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                switch (board[i * 3 + j])   {
                    case 2: printf("-"); break;
                    case 3: printf("X"); break;
                    case 5: printf("O"); break;
                }
            }
            printf("\n");
        }
        printf("\n");
    }

public:
    void play_game()    {
        Tic_Tac_Toe_Player_1 X;
        Tic_Tac_Toe_Player_2 O;
        int move, i, turn;
        char player;

        // make all squares blank
        for (i = 0; i < 9; i++)
            board[i] = 2;

        for (turn = 1; turn <= 9; turn++)   {
            player = (turn & 1) ? 'X' : 'O';
            move = (turn & 1) ? X.play(board, turn) : O.play(board, turn);

            // check if the move is valid
            if (board[move] != 2)   {
                printf("Invalid move %d by player %c.\n", move, player);
                return;
            }

            // make the move
            board[move] = (turn & 1) ? 3 : 5;
            print_board();

            // check if the move wins the game
            if (check_win(player))  {
                printf("Game Over. Player %c won.\n", player);
                return;
            }
        }
        printf("Game Tied.\n");
    }
};

int main()  {
    Judge judge;
    judge.play_game();

    return 0;
}