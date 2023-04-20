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
} tutor_profiles;

typedef struct {
    char user_id[50];
    char student_code[50];
} student_profiles;

/* Functions declaration */
// Menu functions
void home_menu();

void login_menu();

void available_sessions_menu();

void dashboard_menu(users session_user);

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

int read(char *filename, char lines[][1000], int *num_lines);

int user_code_parser(char *user_id);

sessions get_session(char *session_code);

enrolled_sessions *get_enrolled_session(char *user_id, int *num_sessions);

users get_user(char *user_id);

student_profiles get_student_profile(char *user_id);

tutor_profiles get_tutor_profile(char *user_id);

/* Main function */
int main() {
    setup();

    home_menu();

    return 0; // return 0 to indicate successful execution
}

/* Menus */
void home_menu() {
    title_printer("Welcome to APU Programming Cafe Management System!");
    printf("1. Login\n");
    printf("2. View available sessions\n");
    printf("0. Exit\n");

    while (1) {
        int option;
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

void login_menu() {
    title_printer("Login page");
    FILE *users_file = fopen("users.txt", "r");

    int authenticated = 0;
    int login_attempts = 0;

    const int max_login_attempts = 3;

    while (login_attempts < max_login_attempts) {
        rewind(users_file); // reset file pointer, since fscanf() will move the file pointer to the end of the file
        users user;

        char user_id[50];
        char password[50];

        printf("Please enter your User ID (TP number or Tutor code)  down below: \n");
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

void dashboard_menu(users session_user) {
    title_printer("Dashboard");
    printf("Welcome %s (%s) !\n", session_user.name, session_user.role);
    if (strcmp(session_user.role, "student") == 0) {
        student_dashboard_menu(session_user);
    } else if (strcmp(session_user.role, "tutor") == 0) {
        tutor_dashboard_menu(session_user);
    } else if (strcmp(session_user.role, "admin") == 0) {
        admin_dashboard_menu(session_user);
    } else {
        printf("Invalid user role. Please try again.\n");
    }
}

// Admin menus
void admin_dashboard_menu(users session_user) {
    printf("1. User operation\n");
    printf("2. Session operation.\n");
    printf("0. Logout\n");

    while (1) {
        int option;
        printf("Please select an option below: \n");
        if (scanf("%d", &option) != 1) {
            printf("Invalid input. Please enter a numeric integer.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option) {
            case 1:
                user_operation_menu(session_user);
                return;
            case 2:
                session_operation_menu(session_user);
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

// Admin menus -> User operation
void user_operation_menu(users session_user) {
    title_printer("User operation");
    printf("1. Add user.\n");
    printf("2. Delete user.\n");
    printf("3. View all user.\n");
    printf("4 .View user profile.\n");

    printf("0. Back to dashboard.\n");

    while (1) {
        int option;
        printf("Please select an option below: \n");
        if (scanf("%d", &option) != 1) {
            printf("Invalid input. Please enter a numeric integer.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option) {
            case 1:
                add_user_menu(session_user);
                return;
            case 2:
                delete_user_menu(session_user);
                return;
            case 3:
                view_all_user_menu(session_user);
                return;
            case 4:
                view_user_menu(session_user);
                return;
            case 0:
                dashboard_menu(session_user);
                return;
            default:
                printf("Invalid option. Please try again.\n");
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
            printf("User id must be exactly 6 characters long. Please try again.\n");
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

        FILE *student_profiles_file = fopen("student_profiles.txt", "a");

        fprintf(student_profiles_file, "%s;%s\n", student.user_id, student.student_code);

        fclose(student_profiles_file);
    }

    if (strcmp(user.role, "tutor") == 0) {
        tutor_profiles tutor;
        char tutor_code[50];

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

    printf("User added successfully!\n");
    session_operation_menu(session_user);
}

void delete_user_menu(users session_user) {
    title_printer("User operation - Delete user");

    char user_id[50];

    printf("Please enter the user id of the user you want to delete: \n");
    scanf("%s", user_id);

    int response = user_code_parser(user_id);

    if (response == 0) {
        printf("Invalid user code.\n");
        user_operation_menu(session_user);
    }

    users user = get_user(user_id);

    if (strcmp(user.user_id, "") == 0) {
        printf("User does not exist.\n");
        user_operation_menu(session_user);
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

            printf("User deleted successfully!\n");
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

            printf("User deleted successfully!\n");
        }
    }

    user_operation_menu(session_user);
}

void view_all_user_menu(users session_user) {
    title_printer("User operation - View user");

    char lines[1000][1000];
    int num_lines;
    char *file_name = "users.txt";

    int success = read(file_name, lines, &num_lines);

    if (success) {
        printf("User id;Name;Password;Email;Role\n");

        for (int i = 0; i < num_lines; i++) {
            printf("%s", lines[i]);
        }
    } else {
        printf("No users found.\n");
    }
    user_operation_menu(session_user);
}

void view_user_menu(users session_user) {
    title_printer("User operation - View user");

    char user_id[50];

    printf("Please enter the user code of the user you want to view: \n");
    scanf("%s", user_id);

    int response = user_code_parser(user_id);

    if (response == 0) {
        printf("Invalid user id.\n");
        user_operation_menu(session_user);
    }

    users user = get_user(user_id);

    if (strcmp(user.user_id, "") == 0) {
        printf("User does not exist.\n");
        user_operation_menu(session_user);
    } else {
        printf("User id: %s \n", user.user_id);
        printf("Name: %s \n", user.name);
        printf("Email: %s \n", user.email);
        printf("Role: %s \n", user.role);

        if (strcmp(user.role, "student") == 0) {
            student_profiles student = get_student_profile(user_id);
            printf("Student code: %s \n", student.student_code);
        } else if (strcmp(user.role, "tutor") == 0) {
            tutor_profiles tutor = get_tutor_profile(user_id);
            printf("Tutor code: %s \n", tutor.tutor_code);
            printf("Title: %s \n", tutor.title);
        }

        int num_sessions = 0;
        enrolled_sessions *sessions = get_enrolled_session(user_id, &num_sessions);

        printf("Sessions enrolled in: \n");
        for (int i = 0; i < num_sessions; i++) {
            printf("Line %d: %s %s %s %s\n", i + 1, sessions[i].session_code, sessions[i].user_id, sessions[i].name,
                   sessions[i].role);
        }
    }

    user_operation_menu(session_user);
}

// Admin menus -> Session operation
void session_operation_menu(users session_user) {
    title_printer("Session operation");

    printf("1. Add session.\n");
    printf("2. Delete session.\n");
    printf("3. View session.\n");
    printf("4. Enroll a user.\n");
    printf("5. Disenroll a user.\n");

    printf("0. Back to main menu.\n");

    while (1) {
        int option;
        printf("Please select an option below: \n");
        if (scanf("%d", &option) != 1) {
            printf("Invalid input. Please enter a numeric integer.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option) {
            case 1:
                add_session_menu(session_user);
                return; // exit the loop and return to the caller
            case 2:
                delete_session_menu(session_user);
                return;
            case 3:
                view_session_menu(session_user);
                return;
            case 4:
                enroll_user_menu(session_user);
                return;
            case 5:
                disenroll_user_menu(session_user);
                return;
            case 0:
                dashboard_menu(session_user);
                return;
            default:
                printf("Invalid option. Please try again.\n");
                break;
        }
    }
}

void add_session_menu(users session_user) {
    title_printer("Session operation - Add session");

    FILE *sessions_file = fopen("sessions.txt", "a");
    FILE *enrolled_sessions_file = fopen("enrolled_sessions.txt", "a");

    sessions session;

    printf("Please enter the session code: \n");
    scanf("%s", session.session_code);

    printf("Please enter the session name: \n");
    scanf("%s", session.title);

    printf("Please enter the session description: \n");
    scanf("%s", session.day);

    printf("Please enter the session start time: \n");
    scanf("%s", session.start_time);

    printf("Please enter the session location: \n");
    scanf("%s", session.location);

    printf("Enrolled a tutor into the session: \n");
    scanf("%s", session.tutor_code);

    int response = user_code_parser(session.tutor_code);

    if (response == 0) {
        printf("Invalid user code.\n");
        session_operation_menu(session_user);
    }

    char user_id[50];
    strcpy(user_id, session.tutor_code);

    int num_sessions = 0;
    enrolled_sessions *sessions = get_enrolled_session(user_id, &num_sessions);
    users tutor = get_user(user_id);

    if (num_sessions < 1 && strcmp(tutor.role, "tutor") == 0) {
        fprintf(sessions_file, "%s;%s;%s;%s;%s;%s;\n", session.session_code, session.title, session.day,
                session.start_time, session.location, session.tutor_code);
        fprintf(enrolled_sessions_file, "%s;%s;%s;%s;\n", session.session_code, user_id, tutor.name, tutor.role);

        printf("Session added successfully!\n");

    } else {
        printf("Tutor does not exist or has reached the maximum number of sessions.\n");
    }

    fclose(sessions_file);
    fclose(enrolled_sessions_file);

    session_operation_menu(session_user);
}

void delete_session_menu(users session_user) {
    title_printer("Session operation - Delete session");

    printf("Please enter the session code: \n");
    char session_code[50];
    scanf("%s", session_code);

    sessions session = get_session(session_code);

    if (strcmp(session.session_code, "") == 0) {
        printf("Session does not exist.\n");
        session_operation_menu(session_user);
    }

    FILE *sessions_file = fopen("sessions.txt", "r");
    FILE *sessions_temp_file = fopen("sessions_temp.txt", "w");

    char line[100];
    while (fgets(line, sizeof(line), sessions_file)) {
        if (strstr(line, session_code) == NULL) {
            fprintf(sessions_temp_file, "%s", line);
        }
    }

    fclose(sessions_file);
    fclose(sessions_temp_file);

    remove("sessions.txt");
    rename("sessions_temp.txt", "sessions.txt");

    printf("Session deleted successfully!\n");
    session_operation_menu(session_user);
}

void view_session_menu(users session_user) {
    title_printer("Session operation - View session");

    printf("Please enter the session code: \n");
    char session_code[50];
    scanf("%s", session_code);

    sessions session = get_session(session_code);

    if (strcmp(session.session_code, "") == 0) {
        printf("Session does not exist.\n");
        session_operation_menu(session_user);
    }

    printf("Session code: %s \n", session.session_code);
    printf("Session name: %s \n", session.title);
    printf("Session description: %s \n", session.day);
    printf("Session start time: %s \n", session.start_time);
    printf("Session location: %s \n", session.location);
    printf("Session tutor code: %s \n", session.tutor_code);

    int num_sessions = 0;
    enrolled_sessions *sessions = get_enrolled_session(session.session_code, &num_sessions);

    printf("Enrolled users: \n");
    for (int i = 0; i < num_sessions; i++) {
        printf("Line %d: %s %s %s %s\n", i + 1, sessions[i].session_code, sessions[i].user_id, sessions[i].name,
               sessions[i].role);
    }

    session_operation_menu(session_user);
}

void enroll_user_menu(users session_user) {
    title_printer("Session operation - Enroll user");

    FILE *enrolled_sessions_file = fopen("enrolled_sessions.txt", "a");

    printf("Please enter the session code: \n");
    char session_code[50];
    scanf("%s", session_code);

    sessions session = get_session(session_code);

    if (strcmp(session.session_code, "") == 0) {
        printf("Session does not exist.\n");
        session_operation_menu(session_user);
    }

    printf("Please enter the user code: \n");
    char user_id[50];
    scanf("%s", user_id);

    int response = user_code_parser(user_id);

    if (response == 0) {
        printf("Invalid user code.\n");
        session_operation_menu(session_user);
    }

    users user = get_user(user_id);

    if (strcmp(user.user_id, "") == 0) {
        printf("User does not exist.\n");
        session_operation_menu(session_user);
    }

    if (strcmp(user.role, "tutor") == 0) {
        printf("Tutor cannot enroll into a session.\n");
        session_operation_menu(session_user);
    }

    int num_sessions = 0;

    enrolled_sessions *sessions = get_enrolled_session(user_id, &num_sessions);

    for (int i = 0; i < num_sessions; i++) {
        if (strcmp(sessions[i].user_id, user_id) == 0) {
            printf("User is already enrolled in the session.\n");
            return;
        }
    }

    fprintf(enrolled_sessions_file, "%s;%s;%s;%s;\n", session_code, user_id, user.name, user.role);

    fclose(enrolled_sessions_file);

    printf("User enrolled successfully!\n");
    session_operation_menu(session_user);
}

void disenroll_user_menu(users session_user) {
    title_printer("Session operation - Disenroll user");

    printf("Please enter the session code: \n");
    char session_code[50];
    scanf("%s", session_code);

    sessions session = get_session(session_code);

    if (strcmp(session.session_code, "") == 0) {
        printf("Session does not exist.\n");
        session_operation_menu(session_user);
    }

    printf("Please enter the user id: \n");
    char user_id[50];
    scanf("%s", user_id);

    int response = user_code_parser(user_id);

    if (response == 0) {
        printf("Invalid user code.\n");
        session_operation_menu(session_user);
    }

    users user = get_user(user_id);

    if (strcmp(user.user_id, "") == 0) {
        printf("User does not exist.\n");
        session_operation_menu(session_user);
    }

    int num_sessions = 0;

    enrolled_sessions *sessions = get_enrolled_session(user_id, &num_sessions);

    if (num_sessions < 1) {
        printf("Enrolled session does not exist.\n");
    }

    FILE *enrolled_sessions_file = fopen("enrolled_sessions.txt", "r");
    FILE *enrolled_sessions_temp_file = fopen("enrolled_sessions_temp.txt", "w");

    char line[100];
    while (fgets(line, sizeof(line), enrolled_sessions_file)) {
        if (strstr(line, session_code) == NULL) {
            fprintf(enrolled_sessions_temp_file, "%s", line);
        }
    }

    fclose(enrolled_sessions_file);
    fclose(enrolled_sessions_temp_file);

    remove("enrolled_sessions.txt");
    rename("enrolled_sessions_temp.txt", "enrolled_sessions.txt");

    printf("User disenrolled successfully!\n");
    session_operation_menu(session_user);
}

// Tutor menus
void tutor_dashboard_menu(users session_user) {
    printf("1. View my sessions.\n");
    printf("2. View students enrolled in sessions.\n");

    while (1) {
        int option;
        printf("Please select an option below: \n");
        if (scanf("%d", &option) != 1) {
            printf("Invalid input. Please enter a numeric integer.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option) {
            case 1:
                view_my_sessions_menu(session_user);
                return;
            case 2:
                view_students_enrolled_in_sessions_menu(session_user);
                return;
            default:
                printf("Invalid choice.\n");
                break;
        }
    }
}

void view_students_enrolled_in_sessions_menu(users session_user) {
    title_printer("View students enrolled in sessions");

    int num_sessions = 0;
    enrolled_sessions *sessions = get_enrolled_session(session_user.user_id, &num_sessions);

    printf("Enrolled sessions: \n");
    for (int i = 0; i < num_sessions; i++) {
        if (strcmp(sessions[i].role, "student") == 0) {
            printf("Line %d: %s %s %s %s\n", i + 1, sessions[i].session_code, sessions[i].user_id, sessions[i].name,
                   sessions[i].role);
        }
    }

    dashboard_menu(session_user);
}

// Student menus
void student_dashboard_menu(users session_user) {
    printf("1. View my sessions.\n");
    printf("2. Enroll into session.\n");
    printf("0. Logout.\n");

    while (1) {
        int option;
        printf("Please select an option below: \n");
        if (scanf("%d", &option) != 1) {
            printf("Invalid input. Please enter a numeric integer.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option) {
            case 1:
                view_my_sessions_menu(session_user);
                return;
            case 2:
                enroll_into_session_menu(session_user);
                return;
            case 0:
                home_menu();
                return;
            default:
                printf("Invalid choice.\n");
                break;
        }
    }
}

void view_my_sessions_menu(users session_user) {
    title_printer("View my sessions");

    int num_sessions = 0;
    enrolled_sessions *sessions = get_enrolled_session(session_user.user_id, &num_sessions);

    printf("Enrolled sessions: \n");
    for (int i = 0; i < num_sessions; i++) {
        printf("Line %d: %s %s %s %s\n", i + 1, sessions[i].session_code, sessions[i].user_id, sessions[i].name,
               sessions[i].role);
    }

    dashboard_menu(session_user);
}

void enroll_into_session_menu(users session_user) {
    title_printer("Enroll into session");

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

    printf("Please enter the session code: \n");
    char session_code[50];
    scanf("%s", session_code);

    sessions session = get_session(session_code);

    if (strcmp(session.session_code, "") == 0) {
        printf("Session does not exist.\n");
        dashboard_menu(session_user);
    }

    int num_sessions = 0;

    enrolled_sessions *enrolled_session = get_enrolled_session(session_user.user_id, &num_sessions);

    for (int i = 0; i < num_sessions; i++) {
        if (strcmp(enrolled_session[i].session_code, session_code) == 0) {
            printf("You have already enrolled into this session.\n");
            dashboard_menu(session_user);
        }
    }

    FILE *enrolled_sessions_file = fopen("enrolled_sessions.txt", "a");

    fprintf(enrolled_sessions_file, "%s;%s;%s;%s;\n", session.session_code, session_user.user_id, session_user.name,
            session_user.role);

    fclose(enrolled_sessions_file);

    printf("You have successfully enrolled into %s.\n", session.title);

    dashboard_menu(session_user);
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

    char default_enrolled_sessions[5][4][50] = {
            {"PYP101", "265663", "Mary",   "tutor"},
            {"JAV102", "009650", "Peter",  "tutor"},
            {"CSC103", "544654", "James",  "tutor"},
            {"WEB104", "577001", "Johnny", "tutor"},
            {"CSP105", "683357", "David",  "tutor"},
    };

    char default_users[7][5][50] = {
            {"988955", "John",   "123456", "admin@apu.edu.my", "admin"},
            {"265663", "Mary",   "123456", "marry@apu.edu.my", "tutor"},
            {"009650", "Peter",  "123456", "peter@apu.edu.my", "tutor"},
            {"544654", "James",  "123456", "james@apu.edu.my", "tutor"},
            {"577001", "Johnny", "123456", "john@apu.edu.my",  "tutor"},
            {"683357", "David",  "123456", "david@apu.edu.my", "tutor"},
            {"072187", "Jammie", "123456", "jamie@apu.edu.my", "student"}
    };

    char default_tutor_profiles[5][3][50] = {
            {"265663", "T265663", "Python Programming"},
            {"009650", "T009650", "Java Programming"},
            {"544654", "T544654", "C Programming"},
            {"577001", "T577001", "Web Development"},
            {"683357", "T683357", "C Sharp Programming"}
    };

    char default_student_profiles[1][2][50] = {
            {"072187", "TP072187"}
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
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            fprintf(enrolled_sessions_file, "%s;", default_enrolled_sessions[i][j]);
        }
        fprintf(enrolled_sessions_file, "\n");
    }

    // users.txt
    for (int i = 0; i < 7; i++) {
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
    for (int i = 0; i < 1; i++) {
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

int user_code_parser(char *user_id) {
    int valid_user_id = 0;

    if ((user_id[0] == 'T' && user_id[1] == 'P') || (user_id[0] == 't' && user_id[1] == 'p') ||
        (user_id[0] == 't' && user_id[1] == 'P') || (user_id[0] == 'T' && user_id[1] == 'p')) {
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

    return valid_user_id;
}

sessions get_session(char *session_code) {
    sessions s;
    FILE *fp;
    char buffer[255];

    fp = fopen("sessions.txt", "r");
    if (fp == NULL) {
        printf("Error: failed to open sessions.txt\n");
        return s;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // remove the newline character from the buffer
        buffer[strcspn(buffer, "\n")] = 0;

        // split the buffer into fields using semicolon as delimiter
        char *session_code_field = strtok(buffer, ";");
        char *title_field = strtok(NULL, ";");
        char *day_field = strtok(NULL, ";");
        char *start_time_field = strtok(NULL, ";");
        char *location_field = strtok(NULL, ";");
        char *tutor_code_field = strtok(NULL, ";");

        // check if the session code matches the input
        if (strcmp(session_code_field, session_code) == 0) {
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

    // close the file if the input session code is not found
    fclose(fp);

    // return an empty session struct
    return s;
}

enrolled_sessions *get_enrolled_session(char *user_id, int *num_sessions) {
    enrolled_sessions *sessions = NULL;
    FILE *fp;
    char buffer[255];
    int count = 0;

    fp = fopen("enrolled_sessions.txt", "r");
    if (fp == NULL) {
        printf("Error: failed to open enrolled_sessions.txt\n");
        return sessions;
    }

    // count the number of sessions the user is enrolled in
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // remove the newline character from the buffer
        buffer[strcspn(buffer, "\n")] = 0;

        // split the buffer into fields using semicolon as delimiter
        char *session_code_field = strtok(buffer, ";");
        char *user_id_field = strtok(NULL, ";");
        char *name_field = strtok(NULL, ";");
        char *role_field = strtok(NULL, ";");

        // check if the user ID matches the input
        if (strcmp(user_id_field, user_id) == 0) {
            count++;
        }
    }

    // allocate memory for the sessions array
    sessions = (enrolled_sessions *) malloc(count * sizeof(enrolled_sessions));
    if (sessions == NULL) {
        printf("Error: failed to allocate memory\n");
        return sessions;
    }

    // reset the file pointer to the beginning of the file
    fseek(fp, 0, SEEK_SET);

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

        // check if the user ID matches the input
        if (strcmp(user_id_field, user_id) == 0) {
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
    char buffer[255];

    fp = fopen("users.txt", "r");
    if (fp == NULL) {
        printf("Error: failed to open users.txt\n");
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
    char buffer[255];

    fp = fopen("student_profiles.txt", "r");
    if (fp == NULL) {
        printf("Error: failed to open student_profiles.txt\n");
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
    char buffer[255];

    fp = fopen("tutor_profiles.txt", "r");
    if (fp == NULL) {
        printf("Error: failed to open tutor_profiles.txt\n");
        return tp;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // remove the newline character from the buffer
        buffer[strcspn(buffer, "\n")] = 0;

        // split the buffer into fields using semicolon as delimiter
        char *user_id_field = strtok(buffer, ";");
        char *tutor_code_field = strtok(NULL, ";");

        // check if the user ID matches the input
        if (strcmp(user_id_field, user_id) == 0) {
            // copy the fields into the tutor_profiles struct
            strcpy(tp.user_id, user_id_field);
            strcpy(tp.tutor_code, tutor_code_field);

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

