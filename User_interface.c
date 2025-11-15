#include "User_interface.h"
#include "librairy.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
    #include <conio.h>
    #define getch_portable() _getch()
#else
    #include <termios.h>
    #include <unistd.h>

    int getch_portable(void) {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif

char* passWord_dynamic(void) {
    char* password = malloc(SIZE);
    if (!password) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    int length = 0;
    int capacity = SIZE;
    char ch;

    password[0] = '\0';

    while (1) {
        ch = getch_portable();

        if (ch == '\n' || ch == '\r') {
            printf("\n");
            break;
        }

        int is_backspace = 0;
#ifdef _WIN32
        if (ch == 8) is_backspace = 1;
#else
        if (ch == 127) is_backspace = 1;
#endif

        if (is_backspace) {
            if (length > 0) {
                length--;
                password[length] = '\0';
                printf("\b \b");
                fflush(stdout);
            }
        }
        else if (ch >= 32 && ch <= 126) {
            if (length >= capacity - 1) {
                capacity *= 2;
                char* new_password = realloc(password, capacity);
                if (!new_password) {
                    free(password);
                    return NULL;
                }
                password = new_password;
            }

            password[length] = ch;
            length++;
            password[length] = '\0';
            printf("*");
            fflush(stdout);
        }
    }

    return password;
}

int menu_1() {
    printf("1. Connection\n2. Create account\n3. log out\n");
    int choice;
    scanf("%d", &choice);
    while (choice < 1 || choice > 3) {
        printf("Please enter a number between 1 and 3\n");
        scanf("%d", &choice);
    }
    return choice;
}

void create_acc_menu() {
    printf("\t\t~~~ CREATING ACCOUNT~~~\n");
    char username[SIZE];
    printf("Enter your username: ");
    while (getchar() != '\n'){};
    scanf("%s", username);
    while (getchar() != '\n'){};
    printf("Enter your password: ");
    char* password = passWord_dynamic();
    if (!password) {
        fprintf(stderr, "Error allocating memory for password string\n");
        exit(EXIT_FAILURE);
    }
    printf("Your age : ");
    int age = 0;
    scanf("%d", &age);
        while (getchar() != '\n'){};
    createAccount(username, password, age);
    free(password);
}

void login_menu() {
    printf("\t\t~~~LOGIN~~~\n");
    char username[SIZE];
    printf("Please enter your username: ");
    scanf("%s", username);
    while (getchar() != '\n'){};
    printf("Please enter your password: ");
    char* password = passWord_dynamic();

    int age = 0;
    printf("Your age : ");
    scanf("%d", &age);
    while (getchar() != '\n'){};
    login(username, password, age);
    free(password);
}

int menu_2() {
    printf("1. To take a book\n2. To give a book\n3. log out\n");
    int choice;
    scanf("%d", &choice);
        while (getchar() != '\n'){};
    while (choice < 1 || choice > 3) {
        printf("Please enter a number between 1 and 3\n");
        scanf("%d", &choice);
        while (getchar() != '\n'){};
    }
    return choice;
}

void take_a_book(Book* book) {
    printf("You are going to take a book ");
    char b_title[SIZE];
    printf("title of book : ");
    scanf("%s", b_title);
    Book* new_book = get_book_by_title(book, b_title);
    if (new_book) {
        printf("You are taking the book : %s \n", new_book->title);
        printf("From the author : %s\n ", new_book->author);
        printf("Numbers of pages : %d \n", new_book->pages);
        printf("Rating : %d stars\n", new_book->rating);
    }
    printf("SEE YOU SOON\n");
}

void give_a_book(Book* book) {
    printf("Welcome to give back the book, we hope it's was a plaisure\n");
    char b_title[SIZE];
    printf("title of book : ");
    scanf("%s", b_title);
    Book* new_book = get_book_by_title(book, b_title);
    if (!new_book) {
        printf("Sorry but it's not our book\n");
        return;
    }
    printf("We are taking back the book : %s\n", new_book->title);
    printf("From the author : %s\n ", new_book->author);
    printf("Numbers of pages : %d \n", new_book->pages);
    printf("Rating : %d stars\n", new_book->rating);
    printf("See you soon for a other book\n");
}

void run_application() {
    int choice = 0, next = -1;
    Book* librairy = create_book();
    add_book(librairy);
    int running = 1;
    while (running) {
        choice = menu_1();
        switch (choice) {
            case 1:
                login_menu();
                next = 2;
                running = 0;
                break;
            case 2:
                create_acc_menu();
                break;
            case 3:
                running = 0;
                break;
            default:
                printf("Invalid Choice\n");
                break;
        }
    }
    running = 1;
    if (next == 2) {
        while (running) {
            choice = menu_2();
            switch (choice) {
                case 1:
                    take_a_book(librairy);
                    break;
                case 2:
                    give_a_book(librairy);
                    break;
                case 3:
                    running = 0;
                    break;
                default:
                    printf("There are something wrong please restart the app\n");
                    break;
            }
        }
    }
    free(librairy);
}