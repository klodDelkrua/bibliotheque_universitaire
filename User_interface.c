//
// Created by lcdelcroix on 15/11/2025.
//

#include "User_interface.h"
#include "librairy.h"
#include <stdlib.h>
#include <stdio.h>

// Fonction portable pour getch()
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

// ----- Fonction de saisie du mot de passe -----
// Version alternative avec allocation dynamique (plus flexible)
char* passWord_dynamic(void) {
    char* password = malloc(SIZE*sizeof(char)); // Allocation initiale
    if (!password) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    int length = 0;
    int capacity = 256;
    char ch;

    password[0] = '\0';

    while (1) {
        ch = getch_portable();

        // Fin de saisie
        if (ch == '\n' || ch == '\r') {
            printf("\n");
            break;
        }

        // Backspace
        bool is_backspace = false;
#ifdef _WIN32
        if (ch == 8) is_backspace = true;
#else
        if (ch == 127) is_backspace = true;
#endif

        if (is_backspace) {
            if (length > 0) {
                length--;
                password[length] = '\0';
                printf("\b \b");
                fflush(stdout);
            }
        }
        // Caractères normaux
        else if (ch >= 32 && ch <= 126) {
            // Vérifier si on a besoin de plus de mémoire
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
    fflush(stdin);
    return choice;
}

void create_acc_menu() {
    printf("\t\t~~~ CREATING ACCOUNT~~~\n");
    char* username = (char*)malloc(SIZE*sizeof(char));
    if (!username) {
        fprintf(stderr, "Error allocating memory for username string\n");
        exit(EXIT_FAILURE);
    }
    char* password = (char*)malloc(SIZE*sizeof(char));
    if (!password) {
        fprintf(stderr, "Error allocating memory for password string\n");
        exit(EXIT_FAILURE);
    }
    printf("Enter your username: ");
    fflush(stdin);
    fgets(username, SIZE, stdin);
    printf("Enter your password: ");
    password = passWord_dynamic();
    if (!password) {
        fprintf(stderr, "Error allocating memory for password string\n");
        exit(EXIT_FAILURE);
    }
    printf("Your age : ");
    int age = 0;
    scanf("%d", &age);
    fflush(stdin);
    createAccount(username, password, age);
}

void login_menu() {
    printf("\t\t~~~LOGIN~~~\n");
    char* username = (char*)malloc(SIZE*sizeof(char));
    char* password = (char*)malloc(SIZE*sizeof(char));
    if (!username || !password) {
        fprintf(stderr, "Error allocating memory for username string\n");
        exit(EXIT_FAILURE);
    }
    int age = 0;
    printf("Please enter your username: ");
    fgets(username, SIZE, stdin);
    printf("Please enter your password: ");
    password = passWord_dynamic();
    printf("Your age : ");
    scanf("%d", &age);
    fflush(stdin);
    login(username, password, age);
}
int menu_2() {
        printf("1. To take a book\n2. To give a book\n3. log out\n");
        int choice ;
        scanf("%d", &choice);
        fflush(stdin);
        while (choice < 1 || choice > 3) {
            printf("Please enter a number between 1 and 4\n");
            scanf("%d", &choice);
        }
        return choice;
}

void take_a_book(Book* book) {
    printf("You are going to take a book ");
    char* b_title = (char*)malloc(SIZE*sizeof(char));
    if (!b_title) {
        fprintf(stderr, "Error allocating memory for b_title\n");
        exit(EXIT_FAILURE);
    }
    printf("title of book : ");
    fgets(b_title, SIZE, stdin);
    Book* new_book = get_book_by_title(book,b_title);
    if (new_book) {
        printf("You are taking the book : %s \n",new_book->title);
        printf("From the author : %s\n ",new_book->author);
        printf("Numbers of pages : %d \n",new_book->pages);
        printf("Rating : %d stars\n",new_book->rating);
    }
    printf("SEE YOU SOON\n");
}

void give_a_book(Book* book) {
    printf("Welcome to give back the book, we hope it's was a plaisure\n");
    char* b_title = (char*)malloc(SIZE*sizeof(char));
    Book* new_book = get_book_by_title(book,b_title);
    if (!new_book) {
        printf("Sorry but it's not our book\n");
        return;
    }
    printf("We are taking back the book : %s\n",new_book->title);
    printf("From the author : %s\n ",new_book->author);
    printf("Numbers of pages : %d \n",new_book->pages);
    printf("Rating : %d stars\n",new_book->rating);
    printf("See you soon for a other book\n");
}

void run_application() {
    int choice = 0,next = -1;
    Book* librairy = create_book();
    add_book(librairy);
    bool running = true;
    while (running) {
        choice = menu_1();
        switch (choice) {
            case 1:
                create_acc_menu();
                break;
            case 2:
                login_menu();
                next = 2;
                running = false;
                break;
            case 3:
                running = false;
                break;
            default:
                printf("Invalid Choice\n");
                break;
        }
    }
    running = true;
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
                    running = false;
                    break;
                default:
                    printf("There are something wrong please restart the app\n");
                    break;
            }
        }
    }
}

