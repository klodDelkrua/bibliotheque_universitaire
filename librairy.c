#include "librairy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Book * create_book() {
    Book* book = (Book*)malloc(BOOK_SIZE * sizeof(Book));
    if (!book) {
        fprintf(stderr, "Error allocating memory for book");
        exit(EXIT_FAILURE);
    }
    return book;
}

void add_book(Book *book) {
    FILE *f = fopen("/home/lcdelcroix/CLionProjects/ProjectC/bibliotheque_universitaire/Book.txt", "r");
    if (!f) {
        fprintf(stderr, "Error opening Book.txt");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    while (fscanf(f, "%s %s %d %d", book[i].title, book[i].author, &book[i].pages, &book[i].rating) == 4) {
        if (i == BOOK_SIZE) {
            break;
        }
        i++;
    }
    fclose(f);
}

User * create_user() {
    User* user = (User*)malloc(USER_SIZE * sizeof(User));
    if (!user) {
        fprintf(stderr, "Error allocating memory for user");
        exit(EXIT_FAILURE);
    }
    return user;
}

char * hash_password(const char *password) {
    char *hashed = malloc(strlen(password) + 8);
    if (!hashed) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    strcpy(hashed, "HASHED_");
    strcat(hashed, password);
    return hashed;
}

Book * get_book_by_title(Book *book, const char *title) {
    for (int i = 0; i < BOOK_SIZE; i++) {
        if (strcmp(book[i].title, title) == 0) {
            return &book[i];
        }
    }
    printf("We didn't find the book\n");
    return NULL;
}

void createAccount(char *username, const char *password, const int age) {
    FILE *file = fopen("/home/lcdelcroix/CLionProjects/ProjectC/bibliotheque_universitaire/account.txt", "w");
    if (!file) {
        fprintf(stderr, "Error opening file account.txt\n");
        exit(EXIT_FAILURE);
    }
    char *hashed = hash_password(password);
    fprintf(file, "name : %s\n", username);
    fprintf(file, "password : %s\n", hashed);
    fprintf(file, "age : %d\n", age);
    fclose(file);
    free(hashed);
}

int login(const char *username, const char *password, const int age) {
    FILE *file = fopen("/home/lcdelcroix/CLionProjects/ProjectC/bibliotheque_universitaire/account.txt", "r");
    if (!file) {
        fprintf(stderr, "Error opening file account.txt\n");
        return 0;
    }

    char user_file[100];
    char stored_hash[100];
    int age_file;

    if (fscanf(file, "name : %99s\n", user_file) != 1 ||
        fscanf(file, "password : %99s\n", stored_hash) != 1 ||
        fscanf(file, "age : %d\n", &age_file) != 1) {
        fclose(file);
        fprintf(stderr, "Error reading account file\n");
        return 0;
    }

    fclose(file);

    char *input_hash = hash_password(password);
    if (!input_hash) {
        return 0;
    }

    int user_ok = (strcmp(username, user_file) == 0);
    int age_ok = (age == age_file);
    int pass_ok = (strcmp(input_hash, stored_hash) == 0);

    free(input_hash);

    if (user_ok && age_ok && pass_ok) {
        printf("\t\t~~~WELCOME %s to your account\n", username);
        return 1;
    } else {
        fprintf(stderr, "Wrong username or password\n");
        return 0;
    }
}