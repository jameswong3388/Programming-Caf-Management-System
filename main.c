#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* Functions declaration */
// Menu functions
void home_menu();

void login_menu();

void dashboard_menu(users user);

// Menu functions -> Admin
void admin_dashboard_menu(users user);

// Menu functions -> Admin -> User management
void user_operation_menu(users user);

void add_user_menu();

void update_user_menu();

void delete_user_menu();

void view_user_menu();

// Menu functions -> Admin -> Session management
void session_operation_menu();

// Menu functions -> Tutor
void tutor_dashboard_menu(users user);

// Menu functions -> Student
void student_dashboard_menu(users user);

void available_sessions_menu();

// API functions
void setup();

void title_printer(char *title);

int insert(char *filename, void *data, size_t size);

int read(char *filename, char lines[][1000], int *num_lines);

/* Main function */
int main() {
    setup();

    home_menu();

    return 0; // return 0 to indicate successful execution
}

/* Menus */
void home_menu() {
    int option;

    title_printer("Welcome to APU Programming Cafe Management System!");
    printf("1. Login\n");
    printf("2. View available sessions\n");
    printf("0. Exit\n");

    while (1) {
        printf("Please select an option below: \n");
        if (scanf("%d", &option) != 1) {
            printf("Invalid input. Please enter a numeric integer.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option) {
            case 1:
                login_menu();
                return; // exit the loop and return to the caller
            case 2:
                available_sessions_menu();
                return;
            case 0:
                printf("Thank you for using APU Programming Cafe Management System!\n");
                return;
            default:
                printf("Invalid option. Please try again.\n");
                break;
        }
    }
}

void login_menu() {
    title_printer("Login page");
    FILE *users_file = fopen("users.txt", "r");

    int authenticated = 0;
    int login_attempts = 0;

    const int max_login_attempts = 3;

    while (login_attempts < max_login_attempts) {
        rewind(users_file); // reset file pointer, since fscanf() will move the file pointer to the end of the file
        users user;

        int valid_user_id = 0;
        char user_id[50];
        char password[50];

        printf("Please enter your User ID (TP number or Tutor code)  down below: \n");
        scanf("%s", user_id);
        printf("Please enter your password down below: \n");
        scanf("%s", password);

        if ((user_id[0] == 'T' && user_id[1] == 'P') || (user_id[0] == 't' && user_id[1] == 'p')) {
            valid_user_id = 1;
            for (int i = 0; i < 50; i++) {
                user_id[i] = user_id[i + 2];
            }
        } else if ((user_id[0] == 'T') || (user_id[0] == 't')) {
            valid_user_id = 1;
            for (int i = 0; i < 50; i++) {
                user_id[i] = user_id[i + 1];
            }
        } else if ((user_id[0] == 'A') || (user_id[0] == 'a')) {
            valid_user_id = 1;
            for (int i = 0; i < 50; i++) {
                user_id[i] = user_id[i + 1];
            }
        }

        if (valid_user_id) {
            // use while loop to read the file line by line
            while (fscanf(users_file, "%s %s %s %s %s \n", user.user_id, user.name, user.password,
                          user.email, user.role) != EOF) {
                if (strcmp(user.user_id, user_id) == 0 && strcmp(user.password, password) == 0) {
                    authenticated = 1;
                    break;
                }
            }
        }

        if (authenticated) {
            fclose(users_file);
            printf("Login successful %s (%s) !\n", user.name, user.role);
            dashboard_menu(user);
            break;
        } else {
            login_attempts++;
            printf("Login id or password is incorrect, %d attempts left. Please try again.\n", (3 - login_attempts));
        }
    }

    if (authenticated == 0) {
        printf("Max login attempts reached. Exiting program.\n");
    }

    fclose(users_file);
}

void dashboard_menu(users user) {
    title_printer("Dashboard");
    printf("Welcome %s (%s) !\n", user.name, user.role);
    if (strcmp(user.role, "student") == 0) {
        student_dashboard_menu(user);
    } else if (strcmp(user.role, "tutor") == 0) {
        tutor_dashboard_menu(user);
    } else if (strcmp(user.role, "admin") == 0) {
        admin_dashboard_menu(user);
    } else {
        printf("Invalid user role. Please try again.\n");
    }
}

// Admin menus
void admin_dashboard_menu(users user) {
    int option;

    printf("1. User operation\n");
    printf("2. Session operation.\n");
    printf("0. Logout\n");


    while (1) {
        printf("Please select an option below: \n");
        if (scanf("%d", &option) != 1) {
            printf("Invalid input. Please enter a numeric integer.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option) {
            case 1:
                user_operation_menu(user);
                return;
            case 2:
                session_operation_menu();
                return;
            case 0:
                home_menu();
                return;
            default:
                printf("Invalid option. Please try again.\n");
                break;
        }
    }
}

void user_operation_menu(users user) {
    int option;
    title_printer("User operation");
    printf("1. Add user.\n");
    printf("2. Delete user.\n");
    printf("3. Update user.\n");
    printf("4. View user.\n");
    printf("0. Back to dashboard.\n");

    while (1) {
        printf("Please select an option below: \n");
        if (scanf("%d", &option) != 1) {
            printf("Invalid input. Please enter a numeric integer.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option) {
            case 1:
                add_user_menu();
                return;
            case 2:
                delete_user_menu();
                return;
            case 3:
                update_user_menu();
                return;
            case 4:
                view_user_menu();
                return;
            case 0:
                dashboard_menu(user);
                return;
            default:
                printf("Invalid option. Please try again.\n");
                break;
        }
    }


}

void add_user_menu() {
    users user;
    title_printer("User operation - Add user");
    while (1) {
        printf("Please enter the user's role (student or tutor) down below: \n");
        scanf("%s", user.role);
        if (strcmp(user.role, "student") == 0 || strcmp(user.role, "tutor") == 0) {
            break;
        } else {
            printf("Invalid role. Please try again.\n");
        }
    }

    printf("Please enter the user's name down below: \n");
    scanf("%s", user.name);

    while (1) {
        printf("Please enter the user's user id down below: \n");
        scanf("%s", user.user_id);
        if (strlen(user.user_id) == 6) {
            break;
        } else {
            printf("User id must be at least 8 characters long. Please try again.\n");
        }
    }

    while (1) {
        printf("Please enter the user's password down below: \n");
        scanf("%s", user.password);
        if (strlen(user.password) >= 8) {
            break;
        } else {
            printf("Password must be at least 8 characters long. Please try again.\n");
        }
    }

    printf("Please enter the user's email down below: \n");
    scanf("%s", user.email);


    if (strcmp(user.role, "student") == 0) {
        char student_code[50];
        student_profiles student;

        strcpy(student.user_id,
               user.user_id); // copy user.user_id to student.user_id since u cant assign string to char array

        strcpy(student_code, "TP"); // copy the string "TP" to student_code
        strcat(student_code, user.user_id); // concatenate user.user_id to student_code
        strcpy(student.student_code, student_code); // copy the concatenated string to student.student_code

        FILE *student_profile_file = fopen("student_profile.txt", "a");

        fprintf(student_profile_file, "%s;%s\n", student.user_id, student.student_code);

        fclose(student_profile_file);
    }

    if (strcmp(user.role, "tutor") == 0) {
        tutor_profile tutor;
        char tutor_code[50];

        printf("Please enter the tutor's title down below: \n");
        scanf("%s", tutor.title);

        strcpy(tutor.user_id, user.user_id);

        strcpy(tutor_code, "T");
        strcat(tutor_code, user.user_id);
        strcpy(tutor.tutor_code, tutor_code);

        FILE *tutor_profile_file = fopen("tutor_profile.txt", "a");

        fprintf(tutor_profile_file, "%s %s %s \n", tutor.user_id, tutor.tutor_code, tutor.title);

        fclose(tutor_profile_file);
    }

    FILE *users_file = fopen("users.txt", "a");

    fprintf(users_file, "%s %s %s %s %s \n", user.user_id, user.name, user.password, user.email, user.role);

    fclose(users_file);

    printf("User added successfully!\n");
}

void delete_user_menu() {
    title_printer("User operation - Delete user");
}

void update_user_menu() {
    title_printer("User operation - Update user");
}

void view_user_menu() {
    title_printer("User operation - View user");
}

void session_operation_menu() {}

// Tutor menus
void tutor_dashboard_menu(users user) {
    printf("1. View sessions.\n");
    printf("2. View students enrolled in sessions.\n");
}

// Student menus
void student_dashboard_menu(users user) {
    printf("1. View my sessions.\n");
}

void available_sessions_menu() {
    title_printer("Available sessions");

    char lines[1000][1000];
    int num_lines;

    char *file_name = "sessions.txt";
    int success = read(file_name, lines, &num_lines);

    if (!success) {
        printf("Error reading file %s\n", file_name);
        return;
    } else {
        for (int i = 0; i < num_lines; i++) {
            printf("Line %d: %s", i + 1, lines[i]);
        }
    }

    home_menu();
}

/* APIs */
void setup() {
    // create all the necessary files using pointer , each file is a table
    FILE *sessions_file = fopen("sessions.txt", "w");
    FILE *enrolled_sessions_file = fopen("enrolled_sessions.txt", "w");
    FILE *users_file = fopen("users.txt", "w");
    FILE *tutor_profile_file = fopen("tutor_profile.txt", "w");
    FILE *student_profile_file = fopen("student_profile.txt", "w");

    char default_sessions[5][6][50] = {
            {"PYP101", "Python Programming",  "Saturday", "9.00am", "C-01-01", "T265663"},
            {"JAV102", "Java Programming",    "Sunday",   "9.00am", "C-01-02", "T009650"},
            {"CSC103", "C Programming",       "Saturday", "2.00pm", "C-01-03", "T544654"},
            {"WEB104", "Web Development",     "Sunday",   "2.00pm", "C-01-04", "T577001"},
            {"CSP105", "C Sharp Programming", "Monday",   "7.00pm", "C-01-05", "T683357"}
    };

    char default_users[6][5][50] = {
            {"988955", "John",   "123456", "admin@apu.edu.my", "admin"},
            {"265663", "Mary",   "123456", "marry@apu.edu.my", "tutor"},
            {"009650", "Peter",  "123456", "peter@apu.edu.my", "tutor"},
            {"544654", "James",  "123456", "james@apu.edu.my", "tutor"},
            {"577001", "Johnny", "123456", "john@apu.edu.my",  "tutor"},
            {"683357", "David",  "123456", "david@apu.edu.my", "tutor"}
    };

    char default_tutor_profiles[5][3][50] = {
            {"265663", "T265663", "Python Programming"},
            {"009650", "T009650", "Java Programming"},
            {"544654", "T544654", "C Programming"},
            {"577001", "T577001", "Web Development"},
            {"683357", "T683357", "C Sharp Programming"}
    };

    // Seed
    // sessions.txt
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            fprintf(sessions_file, "%s ", default_sessions[i][j]);
        }
        fprintf(sessions_file, "\n");
    }

    // users.txt
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 5; j++) {
            fprintf(users_file, "%s ", default_users[i][j]);
        }
        fprintf(users_file, "\n");
    }

    // tutor_profile.txt
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            fprintf(tutor_profile_file, "%s ", default_tutor_profiles[i][j]);
        }
        fprintf(tutor_profile_file, "\n");
    }

    fclose(sessions_file);
    fclose(enrolled_sessions_file);
    fclose(users_file);
    fclose(tutor_profile_file);
}

int insert(char *filename, void *data, size_t size) {
    /*  Usages
     *  users user = {"981255", "John", "123456", "admin@apu.edu.my admin"};
     *  int response = insert("users.txt", (void *) &user, sizeof(users));
     *  printf("response: %d", response);
     * */

    FILE *fp = fopen(filename, "a");

    if (fp == NULL) {
        printf("Error: could not open file %s\n", filename);
        return 0;
    }

    fwrite(data, size, 1, fp);
    fprintf(fp, "\n");

    fclose(fp);
    return 1;
}

int read(char *filename, char lines[][1000], int *num_lines) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: could not open file %s\n", filename);
        return 0;
    }

    char line[1000];
    int i = 0;
    while (fgets(line, 1000, fp) != NULL) {
        strcpy(lines[i], line);
        i++;
        if (i == 1000) {
            printf("Error: maximum number of lines exceeded\n");
            break;
        }
    }
    *num_lines = i;

    fclose(fp);
    return 1;
}

void title_printer(char *title) {
    // size_t guaranteed to be big enough to contain the size of the biggest object the host system can handle.
    size_t str_len = strlen(title); // strlen returned unsigned long long, so we need to use size_t
    size_t line_len = 4 + str_len;  // total line length is 4 + length of the string

    // print the top line
    for (int i = 0; i < line_len; i++) {
        printf("-");
    }
    printf("\n");

    // print the string line
    printf("| %s |\n", title);

    // print the bottom line
    for (int i = 0; i < line_len; i++) {
        printf("-");
    }
    printf("\n");
}
