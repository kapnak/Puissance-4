#include <stdio.h>
#include <time.h>
#include <windows.h>

#define Y_SIZE 5
#define X_SIZE 7

#define EMPTY 0
#define PLAYER_1 1
#define PLAYER_2 2

#define WHITE 15
#define GREY 7
#define RED 12
#define YELLOW 14

#define TRUE 1
#define FALSE 0


HANDLE hConsole;
int board[Y_SIZE][X_SIZE];
char name_player[1][255];

void start_game(boolean bot);
void initialize_board();
void print_board(boolean numbered);
boolean can_put_more(int x);
int put(int put_x, int player);
void unput(int unput_x);
int bot_choice();
int remove_element(int value, int array[], int size);

void clean();


int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    srand(time(NULL));
    setbuf(stdout, 0);

    while (TRUE) {
        clean();

        SetConsoleTextAttribute(hConsole, RED);
        printf("    ____  __  ________________ ___    _   ______________");
        SetConsoleTextAttribute(hConsole, YELLOW);
        printf("   __ __\n");
        SetConsoleTextAttribute(hConsole, RED);
        printf("   / __ \\/ / / /  _/ ___/ ___//   |  / | / / ____/ ____/");
        SetConsoleTextAttribute(hConsole, YELLOW);
        printf("  / // /\n");
        SetConsoleTextAttribute(hConsole, RED);
        printf("  / /_/ / / / // / \\__ \\\\__ \\/ /| | /  |/ / /   / __/   ");
        SetConsoleTextAttribute(hConsole, YELLOW);
        printf(" / // /_\n");
        SetConsoleTextAttribute(hConsole, RED);
        printf(" / ____/ /_/ // / ___/ /__/ / ___ |/ /|  / /___/ /___   ");
        SetConsoleTextAttribute(hConsole, YELLOW);
        printf("/__  __/\n");
        SetConsoleTextAttribute(hConsole, RED);
        printf("/_/    \\____/___//____/____/_/  |_/_/ |_/\\____/_____/   ");
        SetConsoleTextAttribute(hConsole, YELLOW);
        printf("  /_/   ");
        SetConsoleTextAttribute(hConsole, GREY);

        printf("\n\n"
               "                         ==== MENU ====                         \n"
               "                       1 - Jouer a deux                         \n"
               "                       2 - Jouer contre un bot                  \n"
               "                       3 - Quitter le jeu                       \n");

        int response = 0;

        while (response != 1 && response != 2 && response != 3) {
            printf("\n>");
            scanf("%d", &response);

            if (response == 1) {
                start_game(FALSE);
            } else if (response == 2) {
                start_game(TRUE);
            } else if (response == 3) {
                return 0;
            } else {
                SetConsoleTextAttribute(hConsole, RED);
                printf("[ERREUR] Choix invalide !\n");
                SetConsoleTextAttribute(hConsole, GREY);
            }
        }
    }
}


void start_game(boolean bot) {
        initialize_board();
        int current_player;
        int choice;
        boolean win;

        clean();
        SetConsoleTextAttribute(hConsole, RED);
        printf("\n                     ==== JOUEUR ROUGE ====                     \n");
        SetConsoleTextAttribute(hConsole, WHITE);
        printf("\n                            Pseudo ?                            \n\n>");
        SetConsoleTextAttribute(hConsole, GREY);
        scanf("%s", &name_player[0][255]);

        clean();
        SetConsoleTextAttribute(hConsole, YELLOW);
        printf("\n                     ==== JOUEUR JAUNE ====                     \n");
        SetConsoleTextAttribute(hConsole, WHITE);
        printf("\n                            Pseudo ?                            \n\n>");
        SetConsoleTextAttribute(hConsole, GREY);
        scanf("%s", &name_player[1][255]);


        do {
            clean();
            if (current_player == PLAYER_1) {
                current_player = PLAYER_2;
                SetConsoleTextAttribute(hConsole, YELLOW);
                printf("\n                     ==== JOUEUR JAUNE ====                     \n");
            } else {
                current_player = PLAYER_1;
                SetConsoleTextAttribute(hConsole, RED);
                printf("\n                     ==== JOUEUR ROUGE ====                     \n");
            }

            SetConsoleTextAttribute(hConsole, GREY);
            print_board(TRUE);
            int error = FALSE;

            do {
                if (current_player == PLAYER_2 && bot) {
                    choice = bot_choice();
                } else {
                    printf("\n>");
                    scanf("%d", &choice);
                }

                if (!can_put_more(choice)) {
                    error = TRUE;
                    SetConsoleTextAttribute(hConsole, RED);
                    printf("\n[ERREUR] La colonne %d est pleine.", choice);
                    SetConsoleTextAttribute(hConsole, GREY);
                } else if (choice >= X_SIZE){
                    error = TRUE;
                    SetConsoleTextAttribute(hConsole, RED);
                    printf("\n[ERREUR] La colonne %d n'existe pas.", choice);
                    SetConsoleTextAttribute(hConsole, GREY);
                } else {
                    error = FALSE;
                    win = put(choice, current_player);
                }
            } while (error);
        } while (!win);

        clean();

        if (current_player == 1)
            SetConsoleTextAttribute(hConsole, RED);
        else
            SetConsoleTextAttribute(hConsole, YELLOW);

        printf("          _    ______________________  ________  ______\n"
               "         | |  / /  _/ ____/_  __/ __ \\/  _/ __ \\/ ____/\n"
               "         | | / // // /     / / / / / // // /_/ / __/   \n"
               "         | |/ // // /___  / / / /_/ // // _, _/ /___   \n"
               "         |___/___/\\____/ /_/  \\____/___/_/ |_/_____/   \n");


        printf("\n               ==== Victoire du joueur %s ====               \n", name_player[current_player]);

        print_board(FALSE);
        SetConsoleTextAttribute(hConsole, WHITE);
        printf("\n");
        system("pause");
        SetConsoleTextAttribute(hConsole, GREY);
}


void clean() {
    system("cls");
    //printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}


void initialize_board() {
    for (int y=0; y<Y_SIZE; ++y) {
        for (int x=0; x<X_SIZE; ++x) {
            board[y][x] = EMPTY;
        }
    }
}


void print_board(boolean numbered) {
    SetConsoleTextAttribute(hConsole, GREY);

    printf("\n                 \xDA\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xBF\n");

    for (int y=0; y<Y_SIZE; ++y) {
        printf("                 \xB3");
        for (int x=0; x<X_SIZE; ++x) {
            switch (board[y][x]) {
                case EMPTY:
                    printf("   ");
                    break;
                case PLAYER_1:
                    SetConsoleTextAttribute(hConsole, RED);
                    printf(" O ");
                    SetConsoleTextAttribute(hConsole, GREY);
                    break;
                case PLAYER_2:
                    SetConsoleTextAttribute(hConsole, YELLOW);
                    printf(" O ");
                    SetConsoleTextAttribute(hConsole, GREY);
                    break;
            }
            printf("\xB3");
        }
        printf("\n                 \xC3\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xB4\n");
    }

    if (numbered) {
        SetConsoleTextAttribute(hConsole, WHITE);
        printf("                 ");
        for (int x=0; x<X_SIZE; ++x) {
            if (can_put_more(x))
                printf("  %d ", x);
            else
                printf("    ");
        }
        printf("\n");
        SetConsoleTextAttribute(hConsole, GREY);
    }
}


boolean can_put_more(int x) {
    if (board[0][x] == EMPTY)
        return TRUE;
    else
        return FALSE;
}



int put(int put_x, int player) {
    int put_y;

    // Get Y of new circle
    for (int y=Y_SIZE-1; y>=0; --y) {
        if (board[y][put_x] == EMPTY) {
            put_y = y;
            break;
        }
    }

    board[put_y][put_x] = player;

    int win = FALSE;
    int count = 0;

    // Check horizontal
    for (int x=0; x<X_SIZE; ++x) {
        if (board[put_y][x] == player) {
            count++;
            if (count >= 4)
                win = TRUE;
        } else
            count = 0;
    }

    // Check vertical
    count = 0;
    for (int y=0; y<Y_SIZE; ++y) {
        if (board[y][put_x] == player) {
            count++;
            if (count >= 4)
                win = TRUE;
        } else
            count = 0;
    }


    // Check diagonal left to right
    count = 0;
    int diagonal_y = (put_x - put_y)*-1 > 0 ? (put_x - put_y)*-1 : 0;
    int diagonal_x = put_x - put_y > 0 ? put_x - put_y : 0;

    while (diagonal_y < Y_SIZE && diagonal_x < X_SIZE) {
        if (board[diagonal_y][diagonal_x] == player) {
            count++;
            if (count >= 4)
                win = TRUE;
        } else
            count = 0;

        diagonal_y ++;
        diagonal_x ++;
    }


    // Check diagonal right to left
    count = 0;
    diagonal_y = put_x + put_y >= X_SIZE ? put_x + put_y - X_SIZE + 1 : 0;
    diagonal_x = put_x + put_y < X_SIZE ? put_x + put_y : X_SIZE - 1;

    while (diagonal_y < Y_SIZE && diagonal_x >= 0) {
        if (board[diagonal_y][diagonal_x] == player) {
            count++;
            if (count >= 4)
                win = TRUE;
        } else
            count = 0;

        diagonal_y ++;
        diagonal_x --;
    }

    return win;
}


void unput(int unput_x) {
    int unput_y = 0;

    for (int y=Y_SIZE-1; y>=0; --y) {
        if (board[y][unput_x] == EMPTY) {
            unput_y = y + 1;
            break;
        }
    }

    board[unput_y][unput_x] = EMPTY;
}


int remove_element(int value, int array[], int size) {
    int index = -1;
    for (int i=0; i<size; ++i) {
        if (array[i] == value) {
            index = i;
        }
    }

    if (index != -1) {
        for (int i=index; i<size-1; ++i) {
            array[i] = array[i + 1];
        }
        size--;
    }

    return size;
}


int bot_choice() {
    for (int x=0; x<X_SIZE; ++x) {
        if (can_put_more(x)) {
            if (put(x, PLAYER_2)) {
                unput(x);
                return x;
            }
            unput(x);
        }
    }

    for (int x=0; x<X_SIZE; ++x) {
        if (can_put_more(x)) {
            if (put(x, PLAYER_1)) {
                unput(x);
                return x;
            }
            unput(x);
        }
    }

    int possibilities[X_SIZE];
    int possibilities_size = X_SIZE;
    for (int i=0; i<X_SIZE; ++i) {
        possibilities[i] = i;
    }

    for (int x=0; x<X_SIZE; ++x) {
        if (can_put_more(x)) {
            put(x, PLAYER_2);
            for (int x2=0; x2<X_SIZE; ++x2) {
                if (can_put_more(x2)) {
                    if (put(x2, PLAYER_1)) {
                        printf("\nREMOVE : %d", x2);
                        possibilities_size = remove_element(x2, possibilities, possibilities_size);
                    }
                    unput(x2);
                }
            }
            unput(x);
        } else {
            possibilities_size = remove_element(x, possibilities, possibilities_size);
        }
    }


    if (possibilities_size != 0)
        return possibilities[rand() % possibilities_size];
    else
        return rand() % X_SIZE;
}
