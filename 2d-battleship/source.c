#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define _CRT_SECURE_NO_WARNINGS 


#define MAX_ROWS 10
#define MAX_COLS 10
#define NUM_SHIPS 5
#define BOARD_SIZE 10
#define HIT '*'
#define MISS 'M'
#define WATER '~'

struct Ship {
    char symbol;
    int length;
    char* name;
};


char computer_board[MAX_ROWS][MAX_COLS];

// Function Prototypes 

void init_board(char board[][MAX_COLS], int num_rows, int num_cols);
void print_board(char board[][MAX_COLS], int num_rows, int num_cols);
void welcome_screen(void);
int gen_dir(void);
void gen_start_pt(int dir, int ship_length, int* row_start_ptr, int* col_start_ptr);
void place_ships(char board[][MAX_COLS]);
void random_place_ships(char board[][MAX_COLS]);
void menu_and_place_ships(char board[][MAX_COLS], struct Ship ships[]);
int check_shot(char board[][MAX_COLS], int row, int col);
void update_board(char board[][MAX_COLS], int row, int col, int is_hit);
int is_winner(char board[][MAX_COLS]);
int check_if_sunk_ship(char board[][MAX_COLS], char symbol);
void output_current_moves(FILE* log_file, int player, int row, int col, int is_hit, char ship_sunk);
void output_stats(FILE* log_file, int player1_hits, int player2_hits);
void game(char player_board[][MAX_COLS], char computer_board[][MAX_COLS], FILE* log_file);
void initialize_board(char board[][MAX_COLS], int rows, int cols, char character);
 

int main() {
    FILE* log_file = fopen("battleships.log", "w");
    
    srand(time(NULL));

    char player_board[MAX_ROWS][MAX_COLS];
    char computer_board[MAX_ROWS][MAX_COLS];

    init_board(player_board, MAX_ROWS, MAX_COLS);
    init_board(computer_board, MAX_ROWS, MAX_COLS);

    game(player_board, computer_board, log_file);

    fclose(log_file);
    return 0;
}


void menu_and_place_ships(char board[][MAX_COLS], struct Ship ships[]) {
    printf("Please select from the following menu:\n");
    printf("1. Enter positions of ships manually.\n");
    printf("2. Allow the program to randomly select positions of ships.\n");

    int choice;
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        init_board(board, MAX_ROWS, MAX_COLS); 
        place_ships(board); 
        break;
    case 2:
        init_board(board, MAX_ROWS, MAX_COLS); 
        random_place_ships(board); // Randomly place ships
        break;
    default:
        printf("Invalid choice!\n");
        break;
    }
}

void init_board(char board[][MAX_COLS], int num_rows, int num_cols) {
    for (int row_index = 0; row_index < num_rows; ++row_index) {
        for (int col_index = 0; col_index < num_cols; ++col_index) {
            board[row_index][col_index] = WATER;
        }
    }
}

void print_board(char board[][MAX_COLS], int num_rows, int num_cols) {
    printf("%5d%4d%4d%4d%4d%4d%4d%4d%4d%4d\n", 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    for (int row_index = 0; row_index < num_rows; ++row_index) {
        printf("%-2d", row_index);
        for (int col_index = 0; col_index < num_cols; ++col_index) {
            printf("|%2c ", board[row_index][col_index]);
        }
        putchar('|');
        putchar('\n');
    }
}

void welcome_screen(void) {
	printf("*****Welcome to battle ship*****\n\n");
	printf("Rules of the game: \n"
		"1. This is a two player game.\n"
		"2. Player 1 is you and Player2 is the computer\n"
		"3. You can either manually import your ships or have it randomzied \n"
		"4. You will each takes turns attacking. If you missed the ship it will displau '*' \n"
		"5. If it is a hit it will display 'M' \n\n");
	printf("Hit enter to start the game!");
	getchar();  
	system("cls");
}
int gen_dir(void) {
    return rand() % 2;
}

void gen_start_pt(int dir, int ship_length, int* row_start_ptr, int* col_start_ptr) {
    if (dir == 0) { 
        *row_start_ptr = rand() % MAX_ROWS;
        *col_start_ptr = rand() % (MAX_COLS - ship_length + 1);
    }
    else { 
        *row_start_ptr = rand() % (MAX_ROWS - ship_length + 1);
        *col_start_ptr = rand() % MAX_COLS;
    }
}
void place_ships(char board[][MAX_COLS]) {
    struct Ship ships[NUM_SHIPS] = {
        {'C', 5, "Carrier"},
        {'B', 4, "Battleship"},
        {'R', 3, "Cruiser"},
        {'S', 3, "Submarine"},
        {'D', 2, "Destroyer"}
    };

    for (int i = 0; i < NUM_SHIPS; i++) {
        int placed = 0;
        char ship_symbol = ships[i].symbol;
        int ship_length = ships[i].length;
        char* ship_name = ships[i].name;

        while (!placed) {
            printf("Please enter the %d cells to place the %s across:\n", ship_length, ship_name);

            int coords[5][2] = { 0 };

            for (int j = 0; j < ship_length; j++) {
                scanf("%d %d", &coords[j][0], &coords[j][1]);
            }
            int valid = 1;
            for (int j = 0; j < ship_length; j++) {
                int row = coords[j][0];
                int col = coords[j][1];
                if (row < 0 || row >= MAX_ROWS || col < 0 || col >= MAX_COLS ||
                    board[row][col] != WATER) {
                    valid = 0;
                    break;
                }
            }
            if (valid) {
                for (int j = 0; j < ship_length; j++) {
                    int row = coords[j][0];
                    int col = coords[j][1];
                    board[row][col] = ship_symbol;
                }
                placed = 1;
            }
            else {
                printf("Invalid coordinates or cells already occupied. Please re-enter.\n");
            }
        }
    }
}

void random_place_ships(char board[][MAX_COLS]) {
    struct Ship ships[NUM_SHIPS] = {
        {'C', 5, "Carrier"},
        {'B', 4, "Battleship"},
        {'R', 3, "Cruiser"},
        {'S', 3, "Submarine"},
        {'D', 2, "Destroyer"}
    };
 
    for (int i = 0; i < NUM_SHIPS; i++) {
        int placed = 0;
        while (!placed) {
            int dir = gen_dir();
            int row_start, col_start;
            gen_start_pt(dir, ships[i].length, &row_start, &col_start);
            int valid = 1;

            if (dir == 0) { 
                for (int j = 0; j < ships[i].length; j++) {
                    if (board[row_start][col_start + j] != WATER) {
                        valid = 0;
                        break;
                    }
                }
            }
            else {
                for (int j = 0; j < ships[i].length; j++) {
                    if (board[row_start + j][col_start] != WATER) {
                        valid = 0;
                        break;
                    }
                }
            }

            if (valid) {
                if (dir == 0) { 
                    for (int j = 0; j < ships[i].length; j++) {
                        board[row_start][col_start + j] = ships[i].symbol;
                    }
                }
                else { 
                    for (int j = 0; j < ships[i].length; j++) {
                        board[row_start + j][col_start] = ships[i].symbol;
                    }
                }
                placed = 1;
            }
        }
    }
}

int check_shot(char board[][MAX_COLS], int row, int col) {
    return (board[row][col] !=WATER && board[row][col] != HIT && board[row][col] != MISS);
}

void update_board(char board[][MAX_COLS], int row, int col, int is_hit) {
    board[row][col] = is_hit ? HIT : MISS;
}

int is_winner(char board[][MAX_COLS]) {
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            char cell = board[i][j];
            if (cell != WATER && cell != '*' && cell != 'M') {
                return 0; 
            }
        }
    }
    return 1; 
}
int check_if_sunk_ship(char board[][MAX_COLS], char symbol) {
    
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            if (board[i][j] == symbol) {
                return 0;
            }
        }
    }
    return 1;
}
void output_current_moves(FILE* log_file, int player, int row, int col, int is_hit, char ship_sunk) {
  

    if (is_hit) {
        fprintf(log_file, "Player %d hit a ship at %d,%d.\n", player, row, col);
        if (ship_sunk != WATER) {
            fprintf(log_file, "Player %d sunk the %c ship.\n", player, ship_sunk);

        }
    }
    else {
        fprintf(log_file, "Player %d missed at %d,%d.\n", player, row, col);
    }
}
void output_stats(FILE* log_file, int player1_hits, int player2_hits) {

        fprintf(log_file, "Player 1 hits: %d\n", player1_hits);
        fprintf(log_file, "Player 2 hits: %d\n", player2_hits);
  
}
void initialize_board(char board[][MAX_COLS], int rows, int cols, char character) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = character;
        }
    }
}
int select_who_starts(void) {
    return rand() % 2; // Returns 0 or 1
}
void game(char player_board[][MAX_COLS], char computer_board[][MAX_COLS], FILE* log_file) {


    int player1_hits = 0;
    int player2_hits = 0;
    int game_over = 0;



    welcome_screen();
    menu_and_place_ships(player_board, NUM_SHIPS);
    initialize_board(computer_board, MAX_ROWS, MAX_COLS, WATER);
    random_place_ships(computer_board[NUM_SHIPS]);

    char hidden_computer_board[MAX_ROWS][MAX_COLS];
    initialize_board(hidden_computer_board, MAX_ROWS, MAX_COLS, WATER);


    printf("Player 1's Board:\n");
    print_board(player_board, MAX_ROWS, MAX_COLS);
    system("pause");
    system("cls");

    int who_starts = select_who_starts();
    if (who_starts == 0) {
        printf("Player 1 has been randomly selected to start\n");
    }
    else {
        printf("Player 2 (computer) has been selected to start\n");
    }

    while (!game_over) {
        if (who_starts == 0 || player2_hits > 0) {
            printf("Player 2's Board (Computer's Board):\n");
            print_board(hidden_computer_board, MAX_ROWS, MAX_COLS);
            int valid_input = 0;

            int row, col;
            while (!valid_input) {
                printf("Enter your attack move (Row # Col #): ");
                if (scanf("%d %d", &row, &col) != 2 || (row < 0 || row >= MAX_ROWS) || (col < 0 || col >= MAX_COLS)) {
                    printf("Invalid coordinates or cell already occupied. Please enter valid coordinates.\n");
                }
                else {
                    valid_input = 1;
                }
            }
            int is_hit = check_shot(computer_board, row, col);

            if (is_hit) {
                player1_hits++;
                char ship_hit = computer_board[row][col];
                update_board(hidden_computer_board, row, col, 1); 

                if (check_if_sunk_ship(computer_board, ship_hit)) {
                    printf("Hit! You sunk the %c ship!\n", ship_hit);

                    output_current_moves(log_file, 1, row, col, 1, ship_hit);
                }
                else {
                    output_current_moves(log_file, 1, row, col, 1, 0);
                    printf("Hit!\n");
                }
            }
            else {
                output_current_moves(log_file, 1, row, col, 0, 0);
                printf("Missed.\n");
                hidden_computer_board[row][col] = 'M';

            }
            system("pause");
            system("cls");
            who_starts = 1;
        }
        if (who_starts == 1) {
            int computer_row, computer_col, computer_hit;

            do {
                computer_row = rand() % MAX_ROWS;
                computer_col = rand() % MAX_COLS;
            } while (player_board[computer_row][computer_col] == '*' || player_board[computer_row][computer_col] == 'M');

            computer_hit = check_shot(player_board, computer_row, computer_col);
            update_board(player_board, computer_row, computer_col, computer_hit);

            if (computer_hit) {
                player2_hits++;
                char comp_hit = player_board[computer_row][computer_col];
                if (check_if_sunk_ship(player_board, comp_hit)) {
                    printf("Computer hit and sunk the %c ship at %d,%d!\n", comp_hit, computer_row, computer_col);
                    output_current_moves(log_file, 2, computer_row, computer_col, 1, comp_hit);
                }
                else {
                    output_current_moves(log_file, 2, computer_row, computer_col, 1, 0);
                    printf("Computer hit at %d, %d!\n", computer_row, computer_col);
                }
            }
            else {
                output_current_moves(log_file, 2, computer_row, computer_col, 0, 0);
                printf("Computer missed at %d, %d.\n", computer_row, computer_col);
            }

            who_starts = 0;
        
            printf("Player 1's Board:\n");
            print_board(player_board, MAX_ROWS, MAX_COLS);
            system("pause");
            system("cls");
            who_starts = 0;
        }
        if (is_winner(computer_board) || is_winner(player_board)) {
            game_over = 1;
            output_stats(log_file, player1_hits, player2_hits);

        }
    }
}
