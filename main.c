#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Macros */
#define MAX_USER_ID_LENGTH 7
#define MAX_SESSION_CODE_LENGTH 7
#define MAX_USER_CODE_LENGTH 9
#define MAX_STRING_LENGTH 128

/* Data structure */
typedef struct {
    char session_code[MAX_SESSION_CODE_LENGTH];
    char title[MAX_STRING_LENGTH];
    char day[MAX_STRING_LENGTH];
    char start_time[MAX_STRING_LENGTH];
    char location[MAX_STRING_LENGTH];
    char tutor_code[MAX_USER_CODE_LENGTH];
} sessions;

typedef struct {
    char session_code[MAX_SESSION_CODE_LENGTH];
    char user_id[MAX_USER_ID_LENGTH];
    char name[MAX_STRING_LENGTH];
    char role[MAX_STRING_LENGTH];
} enrolled_sessions;

typedef struct {
    char user_id[MAX_USER_ID_LENGTH];
    char name[MAX_STRING_LENGTH];
    char password[MAX_STRING_LENGTH];
    char email[MAX_STRING_LENGTH];
    char role[MAX_STRING_LENGTH];
} users;

typedef struct {
    char user_id[MAX_USER_ID_LENGTH];
    char tutor_code[MAX_USER_CODE_LENGTH];
    char title[MAX_STRING_LENGTH];
} tutor_profiles;

typedef struct {
    char user_id[MAX_USER_ID_LENGTH];
    char student_code[MAX_USER_CODE_LENGTH];
} student_profiles;

/* Functions declaration */
// Menu functions
void home_menu();

int login_menu();

void available_sessions_menu();

// Menu functions -> Admin
void admin_dashboard_menu(users session_user);

// Menu functions -> Admin -> User management
void user_operation_menu(users session_user);

void add_user_menu(users session_user);

void delete_user_menu(users session_user);

void view_all_user_menu(users session_user);

void view_user_menu(users session_user);

// Menu functions -> Admin -> Session management
void session_operation_menu(users session_user);

void add_session_menu(users session_user);

void delete_session_menu(users session_user);

void view_session_menu(users session_user);

void enroll_user_menu(users session_user);

void disenroll_user_menu(users session_user);

// Menu functions -> Tutor
void tutor_dashboard_menu(users session_user);

void view_my_sessions_menu(users session_user);

void view_students_enrolled_in_sessions_menu(users session_user);

// Menu functions -> Student
void student_dashboard_menu(users session_user);

void enroll_into_session_menu(users session_user);

// API functions
void setup();

void title_printer(char *title);

void dash_printer(int num);

char **read(char *filename, int *num_lines);

int user_code_parser(char *user_code);

sessions get_session(char *filter_field, char *filter_value);

enrolled_sessions *get_enrolled_sessions(char *filter_field, char *filter_value, int *num_sessions);

users get_user(char *user_id);

student_profiles get_student_profile(char *user_id);

tutor_profiles get_tutor_profile(char *user_id);

/* Main function */
int main() {
    setup();

    home_menu();

    return 0;
}

/* Menus */
void home_menu() {
    int flag = 1;

    while (flag) {
        title_printer("Welcome to APU Programming Cafe Management System!");
        printf("1. Login\n");
        printf("2. View available sessions\n");
        printf("0. Exit\n");

        int option;
        printf("Please select an option below: \n");
        // If the scanf returns 1, indicating that the input successfully converts to an integer.
        if (scanf("%d", &option) != 1) {
            printf("[SERVER WARNING] Invalid input. Please enter a numeric integer.\n");
            // Clear the input buffer, including the newline character, so that the next input will not be skipped.
            while (getchar() != '\n');
            continue;
        }

        int response = 0;

        switch (option) {
            case 1:
                response = login_menu();
                if (response == 0) {
                    flag = 0;
                }
                break;
            case 2:
                available_sessions_menu();
                break;
            case 0:
                flag = 0;
                printf("[SERVER INFO] Thank you for using APU Programming Cafe Management System!\n");
                break;
            default:
                printf("[SERVER WARNING] Invalid option. Please try again.\n");
        }
    }
}

void available_sessions_menu() {
    title_printer("Available sessions");
    int num_lines;
    char **lines = read("sessions.txt", &num_lines);

    sessions session;

    if (lines != NULL) {
        dash_printer(98);
        printf("| %-3s | %-12s | %-25s | %-8s | %-10s | %-8s | %-10s |\n",
               "No.", "Session Code", "Title", "Day", "Start Time", "Location", "Tutor Code");
        dash_printer(98);
        for (int i = 0; i < num_lines; i++) {
            // split the line into tokens using strtok() then store them into the session struct
            char *session_code = strtok(lines[i], ";");
            char *title = strtok(NULL, ";");
            char *day = strtok(NULL, ";");
            char *start_time = strtok(NULL, ";");
            char *location = strtok(NULL, ";");
            char *tutor_code = strtok(NULL, ";");

            strcpy(session.session_code, session_code);
            strcpy(session.title, title);
            strcpy(session.day, day);
            strcpy(session.start_time, start_time);
            strcpy(session.location, location);
            strcpy(session.tutor_code, tutor_code);

            printf("| %-3d | %-12s | %-25s | %-8s | %-10s | %-8s | %-10s |\n",
                   i + 1, session.session_code, session.title, session.day, session.start_time, session.location,
                   session.tutor_code);
        }
        dash_printer(98);
        // free memory allocated for lines
        for (int i = 0; i < num_lines; i++) {
            free(lines[i]);
        }
        free(lines);
    }
}

int login_menu() {
    title_printer("Login page");
    FILE *users_file = fopen("users.txt", "r");

    int authenticated = 0;
    int login_attempts = 0;

    const int max_login_attempts = 3;

    while (login_attempts < max_login_attempts) {
        rewind(users_file); // reset file pointer, since fscanf() will move the file pointer to the end of the file
        users user;

        char user_id[MAX_USER_CODE_LENGTH];
        char password[MAX_STRING_LENGTH];

        printf("Please enter your User Code (TP number or Tutor code) down below: \n");
        scanf("%s", user_id);
        printf("Please enter your password down below: \n");
        scanf("%s", password);

        int response = user_code_parser(user_id);

        if (response) {
            // use while loop to read the file line by line
            while (fscanf(users_file, "%[^;];%[^;];%[^;];%[^;];%[^;];\n", user.user_id, user.name, user.password,
                          user.email, user.role) != EOF) {
                if (strcmp(user.user_id, user_id) == 0 && strcmp(user.password, password) == 0) {
                    authenticated = 1;
                    break;
                }
            }
        }

        if (authenticated) {
            fclose(users_file);
            printf("[SERVER INFO] Login successful %s (%s) !\n", user.name, user.role);
            if (strcmp(user.role, "admin") == 0) {
                admin_dashboard_menu(user);
            } else if (strcmp(user.role, "tutor") == 0) {
                tutor_dashboard_menu(user);
            } else if (strcmp(user.role, "student") == 0) {
                student_dashboard_menu(user);
            }
            break;
        } else {
            login_attempts++;
            printf("[SERVER WARNING] Login id or password is incorrect, %d attempts left. Please try again.\n",
                   (3 - login_attempts));
        }
    }

    fclose(users_file);
    printf("[SERVER ERROR] Max login attempts reached. Exiting program.\n");
    return authenticated;
}

// Admin menus
void admin_dashboard_menu(users session_user) {
    int flag = 1;
    while (flag) {
        title_printer("Dashboard");
        printf("Welcome %s (%s) !\n", session_user.name, session_user.role);
        printf("1. User operation\n");
        printf("2. Session operation.\n");
        printf("0. Logout\n");
        int option;
        printf("Please select an option below: \n");
        if (scanf("%d", &option) != 1) {
            printf("[SERVER WARNING] Invalid input. Please enter a numeric integer.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option) {
            case 1:
                user_operation_menu(session_user);
                break;
            case 2:
                session_operation_menu(session_user);
                break;
            case 0:
                flag = 0;
                break;
            default:
                printf("[SERVER WARNING] Invalid option. Please try again.\n");
                break;
        }
    }
}

// Admin menus -> User operation
void user_operation_menu(users session_user) {
    int flag = 1;
    while (flag) {
        title_printer("User operation");
        printf("1. Add user.\n");
        printf("2. Delete user.\n");
        printf("3. View all user.\n");
        printf("4 .View user profile.\n");
        printf("0. Back to dashboard.\n");
        int option;
        printf("Please select an option below: \n");
        if (scanf("%d", &option) != 1) {
            printf("[SERVER WARNING] Invalid input. Please enter a numeric integer.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option) {
            case 1:
                add_user_menu(session_user);
                break;
            case 2:
                delete_user_menu(session_user);
                break;
            case 3:
                view_all_user_menu(session_user);
                break;
            case 4:
                view_user_menu(session_user);
                break;
            case 0:
                flag = 0;
                break;
            default:
                printf("[SERVER WARNING] Invalid option. Please try again.\n");
                break;
        }
    }
}

void add_user_menu(users session_user) {
    users user;
    title_printer("User operation - Add user");
    while (1) {
        printf("Please enter the user's role (student or tutor) down below: \n");
        scanf("%s", user.role);
        if (strcmp(user.role, "student") == 0 || strcmp(user.role, "tutor") == 0) {
            break;
        } else {
            printf("[SERVER ERROR] Invalid role. Please try again.\n");
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
            printf("[SERVER WARNING] User id must be exactly 6 characters long. Please try again.\n");
        }
    }

    while (1) {
        printf("Please enter the user's password down below: \n");
        scanf("%s", user.password);
        if (strlen(user.password) >= 8) {
            break;
        } else {
            printf("[SERVER WARNING] Password must be at least 8 characters long. Please try again.\n");
        }
    }

    printf("Please enter the user's email down below: \n");
    scanf("%s", user.email);


    if (strcmp(user.role, "student") == 0) {
        char student_code[MAX_USER_CODE_LENGTH];
        student_profiles student;

        strcpy(student.user_id,
               user.user_id); // copy user.user_id to student.user_id since u cant assign string to char array

        strcpy(student_code, "TP"); // copy the string "TP" to student_code
        strcat(student_code, user.user_id); // concatenate user.user_id to student_code
        strcpy(student.student_code, student_code); // copy the concatenated string to student.student_code

        FILE *student_profiles_file = fopen("student_profiles.txt", "a");

        fprintf(student_profiles_file, "%s;%s\n", student.user_id, student.student_code);

        fclose(student_profiles_file);
    }

    if (strcmp(user.role, "tutor") == 0) {
        tutor_profiles tutor;
        char tutor_code[MAX_USER_CODE_LENGTH];

        printf("Please enter the tutor's title down below: \n");
        scanf("%s", tutor.title);

        strcpy(tutor.user_id, user.user_id);

        strcpy(tutor_code, "T");
        strcat(tutor_code, user.user_id);
        strcpy(tutor.tutor_code, tutor_code);

        FILE *tutor_profiles_file = fopen("tutor_profiles.txt", "a");

        fprintf(tutor_profiles_file, "%s;%s;%s;\n", tutor.user_id, tutor.tutor_code, tutor.title);

        fclose(tutor_profiles_file);
    }

    FILE *users_file = fopen("users.txt", "a");

    fprintf(users_file, "%s;%s;%s;%s;%s;\n", user.user_id, user.name, user.password, user.email, user.role);

    fclose(users_file);

    printf("[SERVER INFO] User added successfully!\n");
}

void delete_user_menu(users session_user) {
    title_printer("User operation - Delete user");

    char user_id[MAX_USER_CODE_LENGTH];

    printf("Please enter the user code of the user you want to delete: \n");
    scanf("%s", user_id);

    int response = user_code_parser(user_id);

    if (response == 0) {
        printf("[SERVER WARNING] Invalid user code.\n");
        return;
    }

    users user = get_user(user_id);

    if (strcmp(user.user_id, "") == 0) {
        printf("[SERVER ERROR] User does not exist.\n");
        return;
    } else {
        FILE *users_temp_file = fopen("users_temp.txt", "w");
        FILE *users_file = fopen("users.txt", "r");

        int is_student = 0;
        int is_tutor = 0;

        while (fscanf(users_file, "%[^;];%[^;];%[^;];%[^;];%[^;];\n", user.user_id, user.name, user.password,
                      user.email, user.role) != EOF) {
            if (strcmp(user.user_id, user_id) != 0) {
                fprintf(users_temp_file, "%s;%s;%s;%s;%s;\n", user.user_id, user.name, user.password, user.email,
                        user.role);
            } else {
                if (strcmp(user.role, "student") == 0) {
                    is_student = 1;
                } else if (strcmp(user.role, "tutor") == 0) {
                    is_tutor = 1;
                }
            }
        }

        fclose(users_file);
        fclose(users_temp_file);

        remove("users.txt");
        rename("users_temp.txt", "users.txt");

        if (is_student == 1) {
            // delete student profile
            FILE *student_profiles_file = fopen("student_profiles.txt", "r");
            FILE *student_profiles_temp_file = fopen("student_profiles_temp.txt", "w");

            student_profiles student;

            while (fscanf(student_profiles_file, "%[^;];%[^;];\n", student.user_id, student.student_code) != EOF) {
                if (strcmp(student.user_id, user_id) != 0) {
                    fprintf(student_profiles_temp_file, "%s;%s;\n", student.user_id, student.student_code);
                }
            }

            fclose(student_profiles_file);
            fclose(student_profiles_temp_file);

            remove("student_profiles.txt");
            rename("student_profiles_temp.txt", "student_profiles.txt");

            printf("[SERVER INFO] User deleted successfully!\n");
        }

        if (is_tutor == 1) {
            // delete tutor profile
            FILE *tutor_profiles_file = fopen("tutor_profiles.txt", "r");
            FILE *tutor_profiles_temp_file = fopen("tutor_profiles_temp.txt", "w");

            tutor_profiles tutor;

            while (fscanf(tutor_profiles_file, "%[^;];%[^;];%[^;];\n", tutor.user_id, tutor.tutor_code, tutor.title) !=
                   EOF) {
                if (strcmp(tutor.user_id, user_id) != 0) {
                    fprintf(tutor_profiles_temp_file, "%s;%s;%s;\n", tutor.user_id, tutor.tutor_code, tutor.title);
                }
            }

            fclose(tutor_profiles_file);
            fclose(tutor_profiles_temp_file);

            remove("tutor_profiles.txt");
            rename("tutor_profiles_temp.txt", "tutor_profiles.txt");

            printf("[SERVER INFO] User deleted successfully!\n");
        }
    }
}

void view_all_user_menu(users session_user) {
    title_printer("User operation - View user");

    int num_lines;
    char **lines = read("users.txt", &num_lines);

    users user;

    if (lines != NULL) {
        dash_printer(91);
        printf("| %-3s | %-7s | %-15s | %-20s | %-20s | %-7s |\n",
               "No.", "User ID", "Name", "Password", "Email", "Role");
        dash_printer(91);
        for (int i = 0; i < num_lines; i++) {
            // split the line into tokens using strtok() then store them into the session struct
            char *user_id = strtok(lines[i], ";");
            char *name = strtok(NULL, ";");
            char *password = strtok(NULL, ";");
            char *email = strtok(NULL, ";");
            char *role = strtok(NULL, ";");

            strcpy(user.user_id, user_id);
            strcpy(user.name, name);
            strcpy(user.password, password);
            strcpy(user.email, email);
            strcpy(user.role, role);

            printf("| %-3d | %-7s | %-15s | %-20s | %-20s | %-7s |\n",
                   i + 1, user.user_id, user.name, user.password, user.email, user.role);
        }
        dash_printer(91);
        // free memory allocated for lines
        for (int i = 0; i < num_lines; i++) {
            free(lines[i]);
        }
        free(lines);
    }
}

void view_user_menu(users session_user) {
    title_printer("User operation - View user");

    char user_id[MAX_USER_CODE_LENGTH];

    printf("Please enter the user code of the user you want to view: \n");
    scanf("%s", user_id);

    int response = user_code_parser(user_id);

    if (response == 0) {
        printf("[SERVER WARNING] Invalid user code.\n");
        return;
    }

    users user = get_user(user_id);

    if (strcmp(user.user_id, "") == 0) {
        printf("[SERVER ERROR] User does not exist.\n");
        return;
    } else {
        printf("## User Details ##\n");
        dash_printer(85);
        printf("| %-7s | %-15s | %-20s | %-20s | %-7s |\n",
               "User ID", "Name", "Password", "Email", "Role");
        dash_printer(85);
        printf("| %-7s | %-15s | %-20s | %-20s | %-7s |\n",
               user.user_id, user.name, user.password, user.email, user.role);
        dash_printer(85);
        printf("\n");

        printf("## User Profile ##\n");
        if (strcmp(user.role, "student") == 0) {
            dash_printer(16);
            student_profiles student = get_student_profile(user_id);
            printf("| %-12s |\n", "Student Code");
            dash_printer(16);
            printf("| %-12s |\n", student.student_code);
            dash_printer(16);
        } else if (strcmp(user.role, "tutor") == 0) {
            tutor_profiles tutor = get_tutor_profile(user_id);
            dash_printer(50);
            printf("| %-10s | %-33s |\n", "Tutor Code", "Title");
            dash_printer(50);
            printf("| %-10s | %-33s |\n", tutor.tutor_code, tutor.title);
            dash_printer(50);
        }
        printf("\n");

        int num_sessions = 0;
        enrolled_sessions *sessions = get_enrolled_sessions("user_id", user_id, &num_sessions);

        printf("## Enrolled Sessions ##\n");
        dash_printer(55);
        printf("| %-3s | %-7s | %-7s | %-15s | %-7s |\n",
               "No.", "Session", "User ID", "Name", "Role");
        dash_printer(55);

        if (num_sessions > 0) {
            for (int i = 0; i < num_sessions; i++) {
                printf("| %-3d | %-7s | %-7s | %-15s | %-7s |\n",
                       i + 1, sessions[i].session_code, sessions[i].user_id, sessions[i].name, sessions[i].role);
            }
        } else {
            printf("| %-51s |\n", "No enrolled session.");
        }
        dash_printer(55);

        free(sessions); // free the memory allocated to the array when it is no longer needed
    }
}

// Admin menus -> Session operation
void session_operation_menu(users session_user) {
    int flag = 1;
    while (flag) {
        title_printer("Session operation");

        printf("1. Add session.\n");
        printf("2. Delete session.\n");
        printf("3. View session.\n");
        printf("4. Enroll a user.\n");
        printf("5. Disenroll a user.\n");
        printf("0. Back to main menu.\n");
        int option;
        printf("Please select an option below: \n");
        if (scanf("%d", &option) != 1) {
            printf("[SERVER WARNING] Invalid input. Please enter a numeric integer.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option) {
            case 1:
                add_session_menu(session_user);
                break; // exit the loop and return to the caller
            case 2:
                delete_session_menu(session_user);
                break;
            case 3:
                view_session_menu(session_user);
                break;
            case 4:
                enroll_user_menu(session_user);
                break;
            case 5:
                disenroll_user_menu(session_user);
                break;
            case 0:
                flag = 0;
                break;
            default:
                printf("[SERVER WARNING] Invalid option. Please try again.\n");
                break;
        }
    }
}

void add_session_menu(users session_user) {
    title_printer("Session operation - Add session");

    FILE *sessions_file = fopen("sessions.txt", "a");
    FILE *enrolled_sessions_file = fopen("enrolled_sessions.txt", "a");

    sessions session;

    while (1) {
        printf("Please enter the session code: \n");
        scanf("%s", session.session_code);

        if (strlen(session.session_code) == 6) {
            break;
        } else {
            printf("[SERVER WARNING] Session code must be 6 characters long.\n");
        }
    }

    printf("Please enter the session name: \n");
    scanf("%s", session.title);

    printf("Please enter the session day: \n");
    scanf("%s", session.day);

    printf("Please enter the session start time: \n");
    scanf("%s", session.start_time);

    printf("Please enter the session location: \n");
    scanf("%s", session.location);

    printf("Enrolled a tutor into the session: \n");
    scanf("%s", session.tutor_code);

    int response = user_code_parser(session.tutor_code);

    if (response == 0) {
        printf("[SERVER WARNING] Invalid user code.\n");
        return;
    }

    char user_id[MAX_USER_CODE_LENGTH];
    strcpy(user_id, session.tutor_code);

    int num_sessions = 0;
    users tutor = get_user(user_id);
    enrolled_sessions *sessions = get_enrolled_sessions("user_id", user_id, &num_sessions);

    if (num_sessions < 1 && strcmp(tutor.role, "tutor") == 0) {
        fprintf(sessions_file, "%s;%s;%s;%s;%s;%s;\n", session.session_code, session.title, session.day,
                session.start_time, session.location, session.tutor_code);
        fprintf(enrolled_sessions_file, "%s;%s;%s;%s;\n", session.session_code, user_id, tutor.name, tutor.role);

        printf("[SERVER INFO] Session added successfully!\n");

    } else {
        printf("[SERVER ERROR] Tutor does not exist or has reached the maximum number of sessions.\n");
    }

    fclose(sessions_file);
    fclose(enrolled_sessions_file);

    free(sessions);
}

void delete_session_menu(users session_user) {
    title_printer("Session operation - Delete session");

    printf("Please enter the session code: \n");
    char session_code[MAX_SESSION_CODE_LENGTH];
    scanf("%s", session_code);

    sessions session = get_session("session_code", session_code);

    if (strcmp(session.session_code, "") == 0) {
        printf("[SERVER ERROR] Session does not exist.\n");
        return;
    }

    FILE *sessions_file = fopen("sessions.txt", "r");
    FILE *sessions_temp_file = fopen("sessions_temp.txt", "w");
    FILE *enrolled_sessions_file = fopen("enrolled_sessions.txt", "r");
    FILE *enrolled_sessions_temp_file = fopen("enrolled_sessions_temp.txt", "w");

    char line[MAX_STRING_LENGTH];
    while (fgets(line, sizeof(line), sessions_file)) {
        if (strstr(line, session_code) == NULL) {
            fprintf(sessions_temp_file, "%s", line);
        }
    }

    while (fgets(line, sizeof(line), enrolled_sessions_file)) {
        if (strstr(line, session_code) == NULL) {
            fprintf(enrolled_sessions_temp_file, "%s", line);
        }
    }

    fclose(sessions_file);
    fclose(sessions_temp_file);
    fclose(enrolled_sessions_file);
    fclose(enrolled_sessions_temp_file);

    remove("sessions.txt");
    rename("sessions_temp.txt", "sessions.txt");

    remove("enrolled_sessions.txt");
    rename("enrolled_sessions_temp.txt", "enrolled_sessions.txt");

    printf("[SERVER INFO] Session deleted successfully!\n");
}

void view_session_menu(users session_user) {
    title_printer("Session operation - View session");

    printf("Please enter the session code: \n");
    char session_code[MAX_SESSION_CODE_LENGTH];
    scanf("%s", session_code);

    sessions session = get_session("session_code", session_code);

    if (strcmp(session.session_code, "") == 0) {
        printf("[SERVER ERROR] Session does not exist.\n");
        return;
    }

    printf("## Session Details ##\n");
    dash_printer(92);
    printf("| %-12s | %-25s | %-8s | %-10s | %-8s | %-10s |\n",
           "Session Code", "Title", "Day", "Start Time", "Location", "Tutor Code");
    dash_printer(92);
    printf("| %-12s | %-25s | %-8s | %-10s | %-8s | %-10s |\n", session.session_code, session.title, session.day,
           session.start_time, session.location, session.tutor_code);
    dash_printer(92);
    printf("\n");

    int num_sessions = 0;
    enrolled_sessions *sessions = get_enrolled_sessions("session_code", session.session_code, &num_sessions);

    printf("## Enrolled Users ##\n");
    dash_printer(55);
    printf("| %-3s | %-7s | %-7s | %-15s | %-7s |\n",
           "No.", "Session", "User ID", "Name", "Role");
    dash_printer(55);

    if (num_sessions > 0) {
        for (int i = 0; i < num_sessions; i++) {
            printf("| %-3d | %-7s | %-7s | %-15s | %-7s |\n",
                   i + 1, sessions[i].session_code, sessions[i].user_id, sessions[i].name, sessions[i].role);
        }
    } else {
        printf("| %-51s |\n", "No enrolled user.");
    }
    dash_printer(55);

    free(sessions);
}

void enroll_user_menu(users session_user) {
    title_printer("Session operation - Enroll user");

    FILE *enrolled_sessions_file = fopen("enrolled_sessions.txt", "a");

    printf("Please enter the session code: \n");
    char session_code[MAX_SESSION_CODE_LENGTH];
    scanf("%s", session_code);

    sessions session = get_session("session_code", session_code);

    if (strcmp(session.session_code, "") == 0) {
        printf("[SERVER ERROR] Session does not exist.\n");
        return;
    }

    printf("Please enter the user code: \n");
    char user_id[MAX_USER_CODE_LENGTH];
    scanf("%s", user_id);

    int response = user_code_parser(user_id);

    if (response == 0) {
        printf("[SERVER WARNING] Invalid user code.\n");
        return;
    }

    users user = get_user(user_id);

    if (strcmp(user.user_id, "") == 0) {
        printf("[SERVER ERROR] User does not exist.\n");
        return;
    }

    if (strcmp(user.role, "tutor") == 0) {
        int num_sessions = 0;
        enrolled_sessions *user_sessions = get_enrolled_sessions("user_id", user_id, &num_sessions);

        if (num_sessions == 1) {
            printf("[SERVER ERROR] A tutor cannot enroll in more than one session.\n");
            free(user_sessions);
            return;
        }
    }

    int num_sessions = 0;

    enrolled_sessions *sessions = get_enrolled_sessions("session_code", session_code, &num_sessions);

    for (int i = 0; i < num_sessions; i++) {
        if (strcmp(sessions[i].user_id, user_id) == 0) {
            printf("[SERVER ERROR] User is already enrolled in the session.\n");
            return;
        }
    }

    free(sessions);

    fprintf(enrolled_sessions_file, "%s;%s;%s;%s;\n", session_code, user_id, user.name, user.role);

    fclose(enrolled_sessions_file);

    printf("[SERVER INFO] User enrolled successfully!\n");
}

void disenroll_user_menu(users session_user) {
    title_printer("Session operation - Disenroll user");

    printf("Please enter the session code: \n");
    char session_code[MAX_SESSION_CODE_LENGTH];
    scanf("%s", session_code);

    sessions session = get_session("session_code", session_code);

    if (strcmp(session.session_code, "") == 0) {
        printf("[SERVER ERROR] Invalid session code.\n");
        return;
    }

    printf("Please enter the user code: \n");
    char user_id[MAX_USER_CODE_LENGTH];
    scanf("%s", user_id);

    int response = user_code_parser(user_id);

    if (response == 0) {
        printf("[SERVER WARNING] Invalid user code.\n");
        return;
    }

    users user = get_user(user_id);

    if (strcmp(user.user_id, "") == 0) {
        printf("[SERVER ERROR] User does not exist.\n");
        return;
    }

    if (strcmp(user.role, "tutor") == 0) {
        printf("[SERVER ERROR] Tutor cannot be dis-enroll.\n");
        return;
    }

    int num_sessions = 0;
    enrolled_sessions *sessions = get_enrolled_sessions("session_code", session_code, &num_sessions);

    int flag = 0;

    for (int i = 0; i < num_sessions; i++) {
        if (strcmp(sessions[i].user_id, user_id) == 0) {
            flag = 1;
            break;
        }
    }

    if (flag == 0) {
        printf("[SERVER ERROR] Enrolled session does not exist.\n");
        free(sessions);
        return;
    }

    free(sessions);

    FILE *enrolled_sessions_file = fopen("enrolled_sessions.txt", "r");
    FILE *enrolled_sessions_temp_file = fopen("enrolled_sessions_temp.txt", "w");

    enrolled_sessions e_session;
    printf("%s\n", session_code);
    printf("%s\n", user_id);

    while (fscanf(enrolled_sessions_file, "%[^;];%[^;];%[^;];%[^;];\n", e_session.session_code, e_session.user_id, e_session.name, e_session.role) != EOF) {
        if (strcmp(e_session.session_code, session_code) == 0 && strcmp(e_session.user_id, user_id) == 0) {
            continue;
        }

        fprintf(enrolled_sessions_temp_file, "%s;%s;%s;%s;\n", e_session.session_code, e_session.user_id, e_session.name, e_session.role);
    }

    fclose(enrolled_sessions_file);
    fclose(enrolled_sessions_temp_file);

    remove("enrolled_sessions.txt");
    rename("enrolled_sessions_temp.txt", "enrolled_sessions.txt");

    printf("[SERVER INFO] User dis-enrolled successfully!\n");
}

// Tutor menus
void tutor_dashboard_menu(users session_user) {
    int flag = 1;
    while (flag) {
        title_printer("Dashboard");
        printf("Welcome %s (%s) !\n", session_user.name, session_user.role);
        printf("1. View my sessions.\n");
        printf("2. View students enrolled in sessions.\n");
        printf("0. Logout.\n");
        int option;
        printf("Please select an option below: \n");
        if (scanf("%d", &option) != 1) {
            printf("[SERVER WARNING] Invalid input. Please enter a numeric integer.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option) {
            case 1:
                view_my_sessions_menu(session_user);
                break;
            case 2:
                view_students_enrolled_in_sessions_menu(session_user);
                break;
            case 0:
                flag = 0;
                break;
            default:
                printf("[SERVER WARNING] Invalid choice.\n");
                break;
        }
    }
}

void view_students_enrolled_in_sessions_menu(users session_user) {
    title_printer("View students enrolled in sessions");

    char tutor_code[MAX_USER_CODE_LENGTH] = "T";
    strcat(tutor_code, session_user.user_id);

    sessions session = get_session("tutor_code", tutor_code);

    int num_sessions = 0;
    enrolled_sessions *enrolled_sessions = get_enrolled_sessions("session_code", session.session_code, &num_sessions);

    printf("## Enrolled Students ##\n");
    dash_printer(55);
    printf("| %-3s | %-7s | %-7s | %-15s | %-7s |\n",
           "No.", "Session", "User ID", "Name", "Role");
    dash_printer(55);

    if (num_sessions > 0) {
        for (int i = 0; i < num_sessions; i++) {
            if (strcmp(enrolled_sessions[i].role, "student") == 0) {
                printf("| %-3d | %-7s | %-7s | %-15s | %-7s |\n",
                       i, enrolled_sessions[i].session_code, enrolled_sessions[i].user_id,
                       enrolled_sessions[i].name,
                       enrolled_sessions[i].role);
            }
        }
    } else {
        printf("| %-51s |\n", "No enrolled student.");
    }
    dash_printer(55);

    free(enrolled_sessions);
}

// Student menus
void student_dashboard_menu(users session_user) {
    int flag = 1;
    while (flag) {
        title_printer("Dashboard");
        printf("Welcome %s (%s) !\n", session_user.name, session_user.role);
        printf("1. View my sessions.\n");
        printf("2. Enroll into session.\n");
        printf("0. Logout.\n");
        int option;
        printf("Please select an option below: \n");
        if (scanf("%d", &option) != 1) {
            printf("[SERVER WARNING] Invalid input. Please enter a numeric integer.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option) {
            case 1:
                view_my_sessions_menu(session_user);
                break;
            case 2:
                enroll_into_session_menu(session_user);
                break;
            case 0:
                flag = 0;
                break;
            default:
                printf("[SERVER WARNING] Invalid choice.\n");
                break;
        }
    }
}

void view_my_sessions_menu(users session_user) {
    title_printer("View my sessions");

    int num_sessions = 0;
    enrolled_sessions *sessions = get_enrolled_sessions("user_id", session_user.user_id, &num_sessions);

    printf("## Enrolled Sessions ##\n");
    dash_printer(55);
    printf("| %-3s | %-7s | %-7s | %-15s | %-7s |\n",
           "No.", "Session", "User ID", "Name", "Role");
    dash_printer(55);

    if (num_sessions > 0) {
        for (int i = 0; i < num_sessions; i++) {
            printf("| %-3d | %-7s | %-7s | %-15s | %-7s |\n",
                   i + 1, sessions[i].session_code, sessions[i].user_id,
                   sessions[i].name,
                   sessions[i].role);

        }
    } else {
        printf("| %-51s |\n", "No enrolled session.");
    }
    dash_printer(55);

    free(sessions);
}

void enroll_into_session_menu(users session_user) {
    title_printer("Enroll into session");

    int num_lines;
    char **lines = read("sessions.txt", &num_lines);

    sessions available_session;

    if (lines != NULL) {
        dash_printer(98);
        printf("| %-3s | %-12s | %-25s | %-8s | %-10s | %-8s | %-10s |\n",
               "No.", "Session Code", "Title", "Day", "Start Time", "Location", "Tutor Code");
        dash_printer(98);
        for (int i = 0; i < num_lines; i++) {
            // split the line into tokens using strtok() then store them into the session struct
            char *session_code = strtok(lines[i], ";");
            char *title = strtok(NULL, ";");
            char *day = strtok(NULL, ";");
            char *start_time = strtok(NULL, ";");
            char *location = strtok(NULL, ";");
            char *tutor_code = strtok(NULL, ";");

            strcpy(available_session.session_code, session_code);
            strcpy(available_session.title, title);
            strcpy(available_session.day, day);
            strcpy(available_session.start_time, start_time);
            strcpy(available_session.location, location);
            strcpy(available_session.tutor_code, tutor_code);

            printf("| %-3d | %-12s | %-25s | %-8s | %-10s | %-8s | %-10s |\n",
                   i + 1, available_session.session_code, available_session.title, available_session.day,
                   available_session.start_time, available_session.location, available_session.tutor_code);
        }
        dash_printer(98);
        // free memory allocated for lines
        for (int i = 0; i < num_lines; i++) {
            free(lines[i]);
        }
        free(lines);
    }

    printf("Please enter the session code: \n");
    char session_code[MAX_SESSION_CODE_LENGTH];
    scanf("%s", session_code);

    sessions session = get_session("session_code", session_code);

    if (strcmp(session.session_code, "") == 0) {
        printf("[SERVER ERROR] Session does not exist.\n");
        return;
    }

    int num_sessions = 0;

    enrolled_sessions *enrolled_session = get_enrolled_sessions("session_code", session_user.user_id, &num_sessions);

    for (int i = 0; i < num_sessions; i++) {
        if (strcmp(enrolled_session[i].session_code, session_code) == 0) {
            printf("[SERVER ERROR] You have already enrolled into this session.\n");
            return;
        }
    }

    free(enrolled_session);

    FILE *enrolled_sessions_file = fopen("enrolled_sessions.txt", "a");

    fprintf(enrolled_sessions_file, "%s;%s;%s;%s;\n", session.session_code, session_user.user_id, session_user.name,
            session_user.role);

    fclose(enrolled_sessions_file);

    printf("[SERVER INFO] You have successfully enrolled into %s.\n", session.title);
}

/* APIs */
void setup() {
    // create all the necessary files using pointer , each file is a table
    FILE *sessions_file = fopen("sessions.txt", "w");
    FILE *enrolled_sessions_file = fopen("enrolled_sessions.txt", "w");
    FILE *users_file = fopen("users.txt", "w");
    FILE *tutor_profiles_file = fopen("tutor_profiles.txt", "w");
    FILE *student_profiles_file = fopen("student_profiles.txt", "w");

    char default_sessions[5][6][50] = {
            {"PYP101", "Python Programming",  "Saturday", "9.00am", "C-01-01", "T265663"},
            {"JAV102", "Java Programming",    "Sunday",   "9.00am", "C-01-02", "T009650"},
            {"CSC103", "C Programming",       "Saturday", "2.00pm", "C-01-03", "T544654"},
            {"WEB104", "Web Development",     "Sunday",   "2.00pm", "C-01-04", "T577001"},
            {"CSP105", "C Sharp Programming", "Monday",   "7.00pm", "C-01-05", "T683357"}
    };

    char default_enrolled_sessions[6][4][50] = {
            {"PYP101", "265663", "Mary",   "tutor"},
            {"JAV102", "009650", "Peter",  "tutor"},
            {"CSC103", "544654", "James",  "tutor"},
            {"WEB104", "577001", "Johnny", "tutor"},
            {"CSP105", "683357", "David",  "tutor"},
            {"CSP105", "072187", "Jammie", "student"}
    };

    char default_users[8][5][50] = {
            {"123456", "John",   "123456", "admin@apu.edu.my", "admin"},
            {"265663", "Mary",   "123456", "marry@apu.edu.my", "tutor"},
            {"009650", "Peter",  "123456", "peter@apu.edu.my", "tutor"},
            {"544654", "James",  "123456", "james@apu.edu.my", "tutor"},
            {"577001", "Johnny", "123456", "john@apu.edu.my",  "tutor"},
            {"683357", "David",  "123456", "david@apu.edu.my", "tutor"},
            {"072187", "Jammie", "123456", "jamie@apu.edu.my", "student"},
            {"073188", "Jin", "123456", "jin@apu.edu.my", "student"}
    };

    char default_tutor_profiles[5][3][50] = {
            {"265663", "T265663", "Python Programming Tutor"},
            {"009650", "T009650", "Java Programming Tutor"},
            {"544654", "T544654", "C Programming Tutor"},
            {"577001", "T577001", "Web Development Tutor"},
            {"683357", "T683357", "C Sharp Programming Tutor"}
    };

    char default_student_profiles[2][2][50] = {
            {"072187", "TP072187"},
            {"073188", "TP073188"}
    };

    // Seed
    // sessions.txt
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            fprintf(sessions_file, "%s;", default_sessions[i][j]);
        }
        fprintf(sessions_file, "\n");
    }

    // enrolled_sessions.txt
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            fprintf(enrolled_sessions_file, "%s;", default_enrolled_sessions[i][j]);
        }
        fprintf(enrolled_sessions_file, "\n");
    }

    // users.txt
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 5; j++) {
            fprintf(users_file, "%s;", default_users[i][j]);
        }
        fprintf(users_file, "\n");
    }

    // tutor_profiles.txt
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            fprintf(tutor_profiles_file, "%s;", default_tutor_profiles[i][j]);
        }
        fprintf(tutor_profiles_file, "\n");
    }

    // student_profiles.txt
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            fprintf(student_profiles_file, "%s;", default_student_profiles[i][j]);
        }
        fprintf(student_profiles_file, "\n");
    }

    fclose(sessions_file);
    fclose(enrolled_sessions_file);
    fclose(users_file);
    fclose(tutor_profiles_file);
    fclose(student_profiles_file);
}

char **read(char *filename, int *num_lines) {
    char **lines = NULL;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("[SERVER ERROR] Failed to open file '%s'\n", filename);
        return NULL;
    }

    // count the number of lines in the file
    int count = 0;
    char c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            count++;
        }
    }
    rewind(fp); // reset file pointer to beginning of file

    // allocate memory for the lines array
    lines = (char **) malloc(sizeof(char *) * count);
    if (lines == NULL) {
        printf("[SERVER ERROR] Failed to allocate memory for lines array\n");
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        lines[i] = (char *) malloc(sizeof(char) * MAX_STRING_LENGTH);
    }

    // read each line from the file and store it in the lines array
    int i = 0;
    while (fgets(lines[i], MAX_STRING_LENGTH, fp) != NULL) {
        i++;
    }

    fclose(fp);
    *num_lines = count;
    return lines;
}

void title_printer(char *title) {
    // size_t guaranteed to be big enough to contain the size of the biggest object the host system can handle.
    size_t str_len = strlen(title); // strlen returned unsigned long long, so we need to use size_t
    size_t line_len = 4 + str_len;  // total line length is 4 + length of the string

    // print the top line
    dash_printer(line_len);

    // print the string line
    printf("| %s |\n", title);

    // print the bottom line
    dash_printer(line_len);
}

void dash_printer(int num) {
    for (int i = 0; i < num; i++) {
        printf("-");
    }
    printf("\n");
}

int user_code_parser(char *user_code) {
    int valid_user_id = 0;

    if ((user_code[0] == 'T' && user_code[1] == 'P') || (user_code[0] == 't' && user_code[1] == 'p') ||
        (user_code[0] == 't' && user_code[1] == 'P') || (user_code[0] == 'T' && user_code[1] == 'p')) {
        valid_user_id = 1;
        for (int i = 0; i < MAX_USER_CODE_LENGTH; i++) {
            user_code[i] = user_code[i + 2];
        }
    } else if ((user_code[0] == 'T' && user_code[1] == 'U') || (user_code[0] == 't' && user_code[1] == 'u') ||
               (user_code[0] == 't' && user_code[1] == 'U') || (user_code[0] == 'T' && user_code[1] == 'u')) {
        valid_user_id = 1;
        for (int i = 0; i < MAX_USER_CODE_LENGTH; i++) {
            user_code[i] = user_code[i + 2];
        }
    } else if ((user_code[0] == 'A' && user_code[1] == 'D') || (user_code[0] == 'a' && user_code[1] == 'd') ||
               (user_code[0] == 'a' && user_code[1] == 'D') || (user_code[0] == 'A' && user_code[1] == 'd')) {
        valid_user_id = 1;
        for (int i = 0; i < MAX_USER_CODE_LENGTH; i++) {
            user_code[i] = user_code[i + 2];
        }
    }

    return valid_user_id;
}

sessions get_session(char *filter_field, char *filter_value) {
    sessions s;
    FILE *fp;
    char buffer[MAX_STRING_LENGTH];

    fp = fopen("sessions.txt", "r");
    if (fp == NULL) {
        printf("[SERVER ERROR] failed to open sessions.txt\n");
        return s;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // remove the newline character from the buffer
        buffer[strcspn(buffer, "\n")] = 0;

        // split the buffer into fields using semicolon as delimiter
        char *session_code_field = strtok(buffer, ";");
        // use strtok(NULL, ";") to continue tokenising where the previous call left off
        char *title_field = strtok(NULL, ";");
        char *day_field = strtok(NULL, ";");
        char *start_time_field = strtok(NULL, ";");
        char *location_field = strtok(NULL, ";");
        char *tutor_code_field = strtok(NULL, ";");

        // check if the filter field matches the input
        if (strcmp(filter_field, "session_code") == 0 && strcmp(session_code_field, filter_value) == 0) {
            // copy the fields into the session struct
            strcpy(s.session_code, session_code_field);
            strcpy(s.title, title_field);
            strcpy(s.day, day_field);
            strcpy(s.start_time, start_time_field);
            strcpy(s.location, location_field);
            strcpy(s.tutor_code, tutor_code_field);

            // close the file and return the session struct
            fclose(fp);
            return s;
        } else if (strcmp(filter_field, "tutor_code") == 0 && strcmp(tutor_code_field, filter_value) == 0) {
            // copy the fields into the session struct
            strcpy(s.session_code, session_code_field);
            strcpy(s.title, title_field);
            strcpy(s.day, day_field);
            strcpy(s.start_time, start_time_field);
            strcpy(s.location, location_field);
            strcpy(s.tutor_code, tutor_code_field);

            // close the file and return the session struct
            fclose(fp);
            return s;
        }
    }

    // close the file if the input filter is not matched
    fclose(fp);

    // return an empty session struct
    return s;
}

enrolled_sessions *get_enrolled_sessions(char *filter_field, char *filter_value, int *num_sessions) {
    enrolled_sessions *sessions = NULL;
    FILE *fp;
    char buffer[MAX_STRING_LENGTH];
    int count = 0;

    fp = fopen("enrolled_sessions.txt", "r");
    if (fp == NULL) {
        printf("[SERVER ERROR] failed to open enrolled_sessions.txt\n");
        return sessions;
    }

    // count the number of sessions matching the filter
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // remove the newline character from the buffer
        buffer[strcspn(buffer, "\n")] = 0;

        // split the buffer into fields using semicolon as delimiter
        char *session_code_field = strtok(buffer, ";");
        char *user_id_field = strtok(NULL, ";");
        char *name_field = strtok(NULL, ";");
        char *role_field = strtok(NULL, ";");

        // check if the filter field matches the input
        if (strcmp(filter_field, "session_code") == 0 && strcmp(session_code_field, filter_value) == 0) {
            count++;
        } else if (strcmp(filter_field, "user_id") == 0 && strcmp(user_id_field, filter_value) == 0) {
            count++;
        }
    }

    // allocate memory for the sessions array
    sessions = (enrolled_sessions *) malloc(count * sizeof(enrolled_sessions));
    if (sessions == NULL) {
        printf("[SERVER ERROR] failed to allocate memory for sessions array\n");
        return sessions;
    }

    // reset the file pointer to the beginning of the file
    rewind(fp);

    // read the enrolled session information into the sessions array
    int i = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // remove the newline character from the buffer
        buffer[strcspn(buffer, "\n")] = 0;

        // split the buffer into fields using semicolon as delimiter
        char *session_code_field = strtok(buffer, ";");
        char *user_id_field = strtok(NULL, ";");
        char *name_field = strtok(NULL, ";");
        char *role_field = strtok(NULL, ";");

        // check if the filter field matches the input
        if (strcmp(filter_field, "session_code") == 0 && strcmp(session_code_field, filter_value) == 0) {
            // copy the fields into the sessions struct
            strcpy(sessions[i].session_code, session_code_field);
            strcpy(sessions[i].user_id, user_id_field);
            strcpy(sessions[i].name, name_field);
            strcpy(sessions[i].role, role_field);

            // increment the counter
            i++;
        } else if (strcmp(filter_field, "user_id") == 0 && strcmp(user_id_field, filter_value) == 0) {
            // copy the fields into the sessions struct
            strcpy(sessions[i].session_code, session_code_field);
            strcpy(sessions[i].user_id, user_id_field);
            strcpy(sessions[i].name, name_field);
            strcpy(sessions[i].role, role_field);

            // increment the counter
            i++;
        }
    }

    // set the number of sessions found
    *num_sessions = count;

    // close the file and return the sessions array
    fclose(fp);
    return sessions;
}


users get_user(char *user_id) {
    users u;
    FILE *fp;
    char buffer[MAX_STRING_LENGTH];

    fp = fopen("users.txt", "r");
    if (fp == NULL) {
        printf("[SERVER ERROR] failed to open users.txt\n");
        return u;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // remove the newline character from the buffer
        buffer[strcspn(buffer, "\n")] = 0;

        // split the buffer into fields using semicolon as delimiter
        char *user_id_field = strtok(buffer, ";");
        char *name_field = strtok(NULL, ";");
        char *password_field = strtok(NULL, ";");
        char *email_field = strtok(NULL, ";");
        char *role_field = strtok(NULL, ";");

        // check if the user ID matches the input
        if (strcmp(user_id_field, user_id) == 0) {
            // copy the fields into the user struct
            strcpy(u.user_id, user_id_field);
            strcpy(u.name, name_field);
            strcpy(u.password, password_field);
            strcpy(u.email, email_field);
            strcpy(u.role, role_field);

            // close the file and return the user struct
            fclose(fp);
            return u;
        }
    }

    // close the file if the input user ID is not found
    fclose(fp);

    // return an empty user struct
    return u;
}

student_profiles get_student_profile(char *user_id) {
    student_profiles sp;
    FILE *fp;
    char buffer[MAX_STRING_LENGTH];

    fp = fopen("student_profiles.txt", "r");
    if (fp == NULL) {
        printf("[SERVER ERROR] failed to open student_profiles.txt\n");
        return sp;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // remove the newline character from the buffer
        buffer[strcspn(buffer, "\n")] = 0;

        // split the buffer into fields using semicolon as delimiter
        char *user_id_field = strtok(buffer, ";");
        char *student_code_field = strtok(NULL, ";");

        // check if the user ID matches the input
        if (strcmp(user_id_field, user_id) == 0) {
            // copy the fields into the student_profiles struct
            strcpy(sp.user_id, user_id_field);
            strcpy(sp.student_code, student_code_field);

            // close the file and return the student_profiles struct
            fclose(fp);
            return sp;
        }
    }

    // close the file if the input user ID is not found
    fclose(fp);

    // return an empty student_profiles struct
    return sp;
}

tutor_profiles get_tutor_profile(char *user_id) {
    tutor_profiles tp;
    FILE *fp;
    char buffer[MAX_STRING_LENGTH];

    fp = fopen("tutor_profiles.txt", "r");
    if (fp == NULL) {
        printf("[SERVER ERROR] failed to open tutor_profiles.txt\n");
        return tp;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // remove the newline character from the buffer
        buffer[strcspn(buffer, "\n")] = 0;

        // split the buffer into fields using semicolon as delimiter
        char *user_id_field = strtok(buffer, ";");
        char *tutor_code_field = strtok(NULL, ";");
        char *title_field = strtok(NULL, ";");

        // check if the user ID matches the input
        if (strcmp(user_id_field, user_id) == 0) {
            // copy the fields into the tutor_profiles struct
            strcpy(tp.user_id, user_id_field);
            strcpy(tp.tutor_code, tutor_code_field);
            strcpy(tp.title, title_field);

            // close the file and return the tutor_profiles struct
            fclose(fp);
            return tp;
        }
    }

    // close the file if the input user ID is not found
    fclose(fp);

    // return an empty tutor_profiles struct
    return tp;
}
