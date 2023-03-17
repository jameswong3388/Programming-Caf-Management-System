#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Functions declaration */
void home_menu();
void login_menu();

/* Main function */
int main() {
    home_menu();

    return 0;
}

/* Menus */
void home_menu() {

    #define OPTION_LOGIN 1
    #define OPTION_EXIT 0

    int option;

    printf("------------------------------------------------------\n");
    printf("| Welcome to APU Programming Cafe Management System! |\n");
    printf("------------------------------------------------------\n");
    printf("1. Login\n");
    printf("0. Exit\n");

    while (1) {
        printf("Please select an option below: \n");
        if (scanf("%d", &option) != 1) {
            printf("Invalid input. Please enter a numeric integer.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option) {
            case OPTION_LOGIN:
                login_menu();
                return; // exit the loop and return to the caller
            case OPTION_EXIT:
                printf("Thank you for using APU Programming Cafe Management System!\n");
                return; // exit the loop and return to the caller
            default:
                printf("Invalid option. Please try again.\n");
                break;
        }
    }
}

void login_menu() {

}


/* APIs - implements create, read, update, delete to the database on .txt files */
void create() {
    FILE *fp;
}





