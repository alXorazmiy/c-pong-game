#include <ctype.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

pthread_t input_thread;
pthread_t game_thread;
const int eni = 80;
const int boyi = 25;
char ch;
int hisob_1 = 0, hisob_1_x = 30, hisob_1_y = 2;
int hisob_2 = 0, hisob_2_x = 50, hisob_2_y = 2;
int top_x = eni / 2 - 3, top_y = boyi / 2;
int oyinchi_1_y = boyi / 2;
int oyinchi_2_y = boyi / 2;
int ustun_control = 1;
int qator_control = 0;
int game_pauza = 1;

char get_keypress() {
    struct termios oldt, newt;
    char ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
void* read_input() {
    while (1) {
        char ch;
        ch = get_keypress();
        if (ch == 'a' && oyinchi_1_y > 2) {
            oyinchi_1_y--;
        } else if (ch == 'z' && oyinchi_1_y < boyi - 3) {
            oyinchi_1_y++;
        } else if (ch == 'k' && oyinchi_2_y > 2) {
            oyinchi_2_y--;
        } else if (ch == 'm' && oyinchi_2_y < boyi - 3) {
            oyinchi_2_y++;
        } else if (ch == ' ' && oyinchi_2_y < boyi - 3) {
            game_pauza = 1;
            top_x = eni / 2 - 3, top_y = boyi / 2;
        }
    }
    return NULL;
}

// Funksiya 2 - Doimiy ishni bajarish
void* game_logic() {
    while (1) {
        system("clear");

        printf("1-o'yinchi = %d          2-o'yinchi = %d\n", hisob_1, hisob_2);
        if (game_pauza == 0) {
            if (hisob_1 == 21) {
                printf("Birinchi o'yinchi g'alaba qozondi");
            }
            if (hisob_2 == 21) {
                printf("Ikkinchi o'yinchi g'alaba qozondi");
            }
        }
        printf("\n\n\n\n");

        for (int qator = 0; qator < boyi; qator++) {
            for (int ustun = 0; ustun < eni; ustun++) {
                if (ustun == 0 || ustun == eni - 1)
                    printf("│");
                else if (qator == 0 || qator == boyi - 1)
                    printf("─");
                else if (qator == top_y && ustun == top_x)
                    printf("●");
                // else if( ustun == eni / 2)
                //     printf("|");

                else if (qator >= oyinchi_1_y - 1 && qator <= oyinchi_1_y + 1 && ustun == 3)
                    printf("│");
                else if (qator >= oyinchi_2_y - 1 && qator <= oyinchi_2_y + 1 && ustun == 76)
                    printf("│");
                else {
                    printf(" ");
                }
            }
            printf("\n");
        }

        if (game_pauza == 0) {
        } else {
            if (top_x == 3) {
                if (top_y == oyinchi_1_y - 1 || top_y == oyinchi_1_y || top_y == oyinchi_1_y + 1) {
                    if (qator_control == 0 && ustun_control == 0) {
                        ustun_control = 1;
                        top_x++;
                        top_y--;
                    } else if (qator_control == 1 && ustun_control == 0) {
                        qator_control = 1;
                        ustun_control = 1;
                        top_x++;
                        top_y++;
                    }
                }
            } else if (top_x == 76) {
                if (top_y == oyinchi_2_y - 1 || top_y == oyinchi_2_y || top_y == oyinchi_2_y + 1) {
                    if (qator_control == 1 && ustun_control == 1) {
                        ustun_control = 0;
                        top_x++;
                        top_y--;
                    } else if (qator_control == 0 && ustun_control == 1) {
                        qator_control = 0;
                        ustun_control = 0;
                        top_x--;
                        top_y--;
                    }
                }
            }
            if (qator_control == 0 && ustun_control == 1) {
                if (top_y == 0) {
                    // game_pauza = 0;
                    qator_control = 1;
                    top_x++;
                    top_y++;

                } else if (top_x == 78) {
                    game_pauza = 0;
                    hisob_1++;
                    ustun_control = 0;
                    top_x++;
                    top_y--;
                } else {
                    top_x++;
                    top_y--;
                }
            } else if (qator_control == 1 && ustun_control == 1) {
                if (top_y == 24 && top_x != 78) {
                    qator_control = 0;
                    top_x++;
                    top_y--;
                } else if (top_x == 78) {
                    game_pauza = 0;
                    hisob_1++;
                    ustun_control = 0;
                    top_x--;
                    top_y++;
                } else {
                    top_x++;
                    top_y++;
                }
            } else if (qator_control == 1 && ustun_control == 0) {
                if (top_y == 24) {
                    qator_control = 0;
                } else if (top_x == 0) {
                    game_pauza = 0;
                    hisob_2++;
                    ustun_control = 1;
                    top_x++;
                    top_y++;
                } else {
                    top_x--;
                    top_y++;
                }
            } else if (qator_control == 0 && ustun_control == 0) {
                if (top_y == 0) {
                    qator_control = 1;
                    top_x--;
                    top_y++;
                } else if (top_x == 0) {
                    game_pauza = 0;
                    hisob_2++;
                    if (qator_control == 0) {
                        qator_control = 0;
                    } else {
                        qator_control = 0;
                    }
                    ustun_control = 1;
                    top_x++;
                    top_y++;
                } else {
                    top_x--;
                    top_y--;
                }
            }
        }
        usleep(100000);
    }
    return NULL;
}

int main() {
    printf("\n\n\n\n\n\n");
    pthread_create(&input_thread, NULL, read_input, NULL);
    pthread_create(&game_thread, NULL, game_logic, NULL);
    pthread_join(input_thread, NULL);
    pthread_join(game_thread, NULL);

    printf("\n\n\n\n\n\n");

    return 0;
}