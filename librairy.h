#ifndef BIBLIOTHEQUE_UNIVERSITAIRE_LIBRAIRY_H
#define BIBLIOTHEQUE_UNIVERSITAIRE_LIBRAIRY_H

#define SIZE 50
#define PASSWORD_SIZE 20
#define BOOK_SIZE 100
#define USER_SIZE 100

typedef struct Book Book;
struct Book {
    char title[SIZE];
    char author[SIZE];
    int pages;
    int rating;
};

typedef struct User User;
struct User {
    char* username;
    char* password;
    int age;
};

Book* create_book();
void add_book(Book* book);
User* create_user();
char* hash_password(const char* password);
Book* get_book_by_title(Book* book, const char* title);
void createAccount(char* username, const char* password, int age);
int login(const char* username, const char* password, int age);

#endif