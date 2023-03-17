#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Functions declaration */
void home_menu();

void login_menu();

void setup();

/* Data structure */
typedef struct {
    char session_code[50];
    char title[50];
    char day[50];
    char start_time[50];
    char location[50];
    char tutor_code[50];
} sessions;

typedef struct {
    char session_code[50];
    char user_id[50];
    char name[50];
    char role[50];
} enrolled_sessions;

typedef struct {
    char user_id[50];
    char name[50];
    char password[50];
    char email[50];
    char role[50];
} users;

typedef struct {
    char user_id[50];
    char tutor_code[50];
    char title[50];
} tutor_profile;

typedef struct {
    char user_id[50];
    char student_code[50];
} student_profiles;

/* Main function */
int main() {
    setup();
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
void setup() {
    char default_sessions[5][6][50] = {
            {"PYP101", "Python Programming",  "Saturday", "9.00am", "C-01-01", "T265663"},
            {"JAV102", "Java Programming",    "Sunday",   "9.00am", "C-01-02", "T009650"},
            {"CSC103", "C Programming",       "Saturday", "2.00pm", "C-01-03", "T544654"},
            {"WEB104", "Web Development",     "Sunday",   "2.00pm", "C-01-04", "T577001"},
            {"CSP105", "C Sharp Programming", "Monday",   "7.00pm", "C-01-05", "T683357"}
    };

    char default_users[6][5][50] = {
            {"988955",  "John",   "123456", "admin@apu.edu.my", "admin"},
            {"265663 ", "Mary",   "123456", "marry@apu.edu.my", "tutor"},
            {"009650",  "Peter",  "123456", "peter@apu.edu.my", "tutor"},
            {"544654",  "James",  "123456", "james@apu.edu.my", "tutor"},
            {"577001",  "Johnny", "123456", "john@apu.edu.my",  "tutor"},
            {"683357",  "David",  "123456", "david@apu.edu.my", "tutor"}
    };

    char default_tutor_profiles[5][3][50] = {
            {"265663", "T265663", "Python Programming"},
            {"009650", "T009650", "Java Programming"},
            {"544654", "T544654", "C Programming"},
            {"577001", "T577001", "Web Development"},
            {"683357", "T683357", "C Sharp Programming"}
    };

    // create all the necessary files using pointer , each file is a table
    FILE *sessions_file = fopen("sessions.txt", "w");
    FILE *enrolled_sessions_file = fopen("enrolled_sessions.txt", "w");
    FILE *users_file = fopen("users.txt", "w");
    FILE *tutor_profile_file = fopen("tutor_profile.txt", "w");

    // Seed
    // sessions.txt
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            fprintf(sessions_file, "%s ", default_sessions[i][j]);
        }
        fprintf(sessions_file, "\n");
    }

    // users.txt
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 5; j++) {
            fprintf(users_file, "%s ", default_users[i][j]);
        }
        fprintf(users_file, "\n");
    }

    // tutor_profile.txt
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 2; j++) {
            fprintf(tutor_profile_file, "%s ", default_tutor_profiles[i][j]);
        }
        fprintf(tutor_profile_file, "\n");
    }

    fclose(sessions_file);
    fclose(enrolled_sessions_file);
    fclose(users_file);
    fclose(tutor_profile_file);
}

void create() {}





