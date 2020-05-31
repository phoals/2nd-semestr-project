#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct books{
    char* num;
    char* author;
    char* title;
    char* qf;
    char* qt;
};

struct users{
    char* login;
    char* pass;
    char* stud;
    char* lib;
};

struct students {
    char* id;
    char* name;
    char* surname;
    char* fname;
    char* faculty;
    char* spec;
};

int quantity(FILE*);

char* scan(int*, FILE*);

struct books* input(struct books*, FILE*);

void show(struct books*, int);

char* scan_console();

int check(struct books*, int, char*);

void input_file(int, FILE*, struct books*);

void delete(FILE*, struct books*, int);

void refresh(FILE*, struct books*, int);

void search(FILE*, struct books*, int);

int page_proofs(char*);

int char_to_int(char*);

void shell(int , int*);

char* int_to_char(int);

void sort(FILE *, struct books*, int);

int lib_moderator(FILE*);

/*****************Students**********************/

struct students* input_students(struct students*, FILE*);

void show_s(struct students*, int);

int check_s(struct students*, int, char*);

void input_file_s(int, FILE*, struct students*);

void delete_s(struct students* , int );

void refresh_s(FILE* fp, struct students* stud, int n);

void update_s(FILE*, struct students*, int);

void edit_s(struct students*, int, FILE*);

int stud_moderator(FILE*);
/*****************Users*************************/
struct users* input_users(struct users*, FILE*);

int check_users_lg(struct users*, int, char*);

int check_users_ps(struct users*, int, char*);

int index(struct users* , int, char*);

int mode(char*, char*, struct users*, int);

/*****************Admin***********************/

int admin(FILE*, FILE*);

int main() {
    FILE *fusers = fopen("users.csv", "r");
    FILE *file = fopen("books.csv", "a+");
    FILE *studfile = fopen("students.csv", "a+");
    int digit_users = quantity(fusers);
    struct users* user = malloc(1 * sizeof(struct users));
    user = input_users(user, fusers);
    printf("Login:");
    char* log = scan_console();
    printf("Password:");
    char *psw = scan_console();
    int var = mode(log, psw, user, digit_users);
    switch (var) {
        case 0:
            printf("Invalid login or password");
        break;
        case 1:
            return(admin(file, studfile));
        case 2:
            return (stud_moderator(studfile));
        case 3:
            return(lib_moderator(file));
    }
}


int quantity(FILE* fp){
    int n = 1;
    char c = fgetc(fp);
    while (c != EOF){
        if (c == '\n') n++;
        c = fgetc(fp);
    }
    rewind(fp);
    return(n);
}

struct books* input(struct books* book, FILE* fp)
{
    int n = 0;
    int checker = 0;
    while (checker != 1)
    {
        book[n].num = scan(&checker, fp);
        book[n].author = scan(&checker, fp);
        book[n].title = scan(&checker, fp);
        book[n].qf = scan(&checker, fp);
        book[n].qt = scan(&checker, fp);
        n++;
        book = realloc(book, (n + 1) * sizeof(struct books));
    }
    return(book);
}

char* scan(int* checker, FILE* fp)
{
    int i = 0;
    char c;
    char* str = malloc(1 * sizeof(char));
    while ((c = getc(fp)) != ';' && c != '\n' && c != EOF)
    {
        *(str + i) = c;
        i++;
        str = realloc(str, (i + 1) * sizeof(char));
    }
    *(str + i) = '\0';
    if (c == EOF)
        *checker = 1;
    return(str);
}

void show(struct books* book, int n)
{
    int a;
    int j = 0;
    printf("+--------------+-------------------+-----------------+--------------+--------------+\n");
    printf("|     ISBN     |       Author      |      Title      |     Free     |     Taken    |\n");
    printf("+--------------+-------------------+-----------------+--------------+--------------+\n");
    for (int i = 0; i < n; ++i)
    {
        printf("+--------------+-------------------+-----------------+--------------+--------------+\n");
        a = 14 - page_proofs(book[i].num);
        printf("|%s",book[i].num);
        for (j = 0; j < a; ++j) {
            printf(" ");
        }
        printf("|%s", book[i].author);
        a = 19 - page_proofs(book[i].author);
        for (j = 0; j < a; ++j) {
            printf(" ");
        }
        printf("|%s", book[i].title);
        a = 17 - page_proofs(book[i].title);
        for (j = 0; j < a; ++j) {
            printf(" ");
        }
        printf("|%s", book[i].qf);
        a = 14 - page_proofs(book[i].qf);
        for (j = 0; j < a; ++j) {
            printf(" ");
        }
        printf("|%s", book[i].qt);
        a = 14 - page_proofs(book[i].qt);
        for (j = 0; j < a; ++j) {
            printf(" ");
        }
        printf("|\n");
        printf("+--------------+-------------------+-----------------+--------------+--------------+\n");
    }
}

char* scan_console()
{
    int i = 0;
    char c;
    char* str = malloc(1 * sizeof(char));
    while ((c = getchar()) != ';' && c != '\n' && c != EOF)
    {
        *(str + i) = c;
        i++;
        str = realloc(str, (i + 1) * sizeof(char));
    }
    *(str + i) = '\0';
    return (str);
}

int check(struct books* book, int n, char* str){
    int checker = 0;
    for (int i = 0; i < n; ++i) {
        if (strcmp(book[i].num, str) == 0) checker = 1;
    }
    return checker;
}

void input_file(int digit, FILE* fp, struct books* book){
    printf("Input book info:\n");
    book = realloc(book, (digit + 1) * sizeof(struct books));
    book[digit].num = scan_console();
    if (check(book, digit - 1, book[digit].num) == 0) {
        book[digit].author = scan_console();
        book[digit].title = scan_console();
        book[digit].qf = scan_console();
        book[digit].qt = scan_console();
        fprintf(fp, "\n%s;%s;%s;%s;%s", book[digit].num, book[digit].author, book[digit].title, book[digit].qf,
                book[digit].qt);
        printf("Book successfully added\n");
    } else {
        printf("Book with this ISBN already exist\n");
    }
}

void delete(FILE* fp, struct books* book, int n){
    printf("\nEnter an ISBN code of a book you want to delete:\n");
    int i = 0;
    int index;
    char c;
    char* str = malloc(1 * sizeof(char));
    while ((c = getchar()) != '\n')
    {
        *(str + i) = c;
        i++;
        str = realloc(str, (i + 1) * sizeof(char));
    }
    *(str + i) = '\0';
    for (i = 0; i < n; i++) {
        if (strcmp (book[i].num, str) == 0) index = i;
    }
    for (i = index; i < n - 1; ++i) {
        book[i].num = book[i + 1].num;
        book[i].author = book[i + 1].author;
        book[i].title = book[i + 1].title;
        book[i].qf = book[i + 1].qf;
        book[i].qt = book[i + 1].qt;
    }
    book[n].num = NULL;
    book[n].author = NULL;
    book[n].title = NULL;
    book[n].qf = NULL;
    book[n].qt = NULL;
}

void refresh(FILE* fp, struct books* book, int n){
    book = realloc(book, (n) * sizeof(struct books));
    for (int i = 0; i < n - 1; ++i) {
        fprintf(fp, "%s;%s;%s;%s;%s\n", book[i].num, book[i].author, book[i].title, book[i].qf, book[i].qt);
    }
    fprintf(fp, "%s;%s;%s;%s;%s", book[n - 1].num, book[n - 1].author, book[n - 1].title, book[n - 1].qf, book[n - 1].qt);
    printf("Book successfully deleted\n");
}

void search(FILE* fp, struct books* book, int n) {
    printf("\nEnter an ISBN code of a book you want to find:\n");
    int i = 0;
    int index = -1;
    char c;
    char* str = malloc(1 * sizeof(char));
    while ((c = getchar()) != '\n')
    {
        *(str + i) = c;
        i++;
        str = realloc(str, (i + 1) * sizeof(char));
    }
    *(str + i) = '\0';
    for (i = 0; i < n; i++) {
        if (strcmp (book[i].num, str) == 0) index = i;
    }
    if (index == -1) printf("There is no book with this parameters\n");
    else {
        printf("Here is your book :\n");
        printf("%s %s %s %s %s\n", book[index].num, book[index].author, book[index].title, book[index].qf, book[index].qt);
    }
}

int page_proofs(char* str){
    char c = str[0];
    int i = 1;
    while(c != '\0'){
        i++;
        c = str[i];
    }
    return(i);
}

int char_to_int(char* str){
    int digit = 0;
    int i = 0;
    while (str[i] != '\0') {
        digit = digit * 10;
        digit = digit + (str[i] - '0');
        i++;
    }
    return digit;
}

void shell(int n, int* mass)
{
    int i, j, step;
    int tmp;
    for (step = n / 2; step > 0; step /= 2)
        for (i = step; i < n; i++)
        {
            tmp = mass[i];
            for (j = i; j >= step; j -= step)
            {
                if (tmp < mass[j - step])
                    mass[j] = mass[j - step];
                else
                    break;
            }
            mass[j] = tmp;
        }
}

char* int_to_char(int digit){
    int n = 0;
    int* d = malloc(1 * sizeof(int));
    char* str = malloc(1 * sizeof(char));
    while (digit> 0) {
        n++;
        d[n - 1] = digit % 10;
        d = realloc(d, (n + 1) * sizeof(int));
        digit = digit / 10;
    }
    str = realloc(str, n * sizeof(char));
    for (int i = 0; i < n; ++i) {
        str[i] = d[n - i - 1] + '0';
    }
    str[n] = '\0';
    return(str);
}

void sort(FILE *fp, struct books* book, int n){
    char* s;
    int i = 0;
    int k = 0;
    int* num = malloc(n * sizeof(int));
    for (i = 0; i < n; ++i) {
        num[i] = char_to_int(book[i].num);
    }
    shell(n, num);
    for (i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (strcmp(int_to_char(num[i]), book[j].num) == 0) {
               s = book[k].num;
               book[k].num = book[j].num;
               book[j].num = s;
               s = book[k].author;
               book[k].author = book[j].author;
               book[j].author = s;
               s = book[k].title;
               book[k].title = book[j].title;
               book[j].title = s;
               s = book[k].qf;
               book[k].qf = book[j].qf;
               book[j].qf = s;
               s = book[k].qt;
               book[k].qt = book[j].qt;
               book[j].qt = s;
            }
        }
        k++;
    }
}

int lib_moderator(FILE* file) {
    printf("\nU entered as a library moderator\n\n");
    int v;
    printf("Choose an option :\n");
    printf("1. Show all books\n");
    printf("2. Add a book\n");
    printf("3. Delete book\n");
    printf("4. Find a book\n");
    printf("5. Exit\n");
    int digit;
    struct books *book = malloc(1 * sizeof(struct books));
    digit = quantity(file);
    book = input(book, file);
    scanf("%d", &v);
    while (1) {
        switch (v) {
            case 1:
                v = 0;
                sort(file, book, digit);
                show(book, digit);
                scanf("%d", &v);
                break;
            case 2:
                v = 0;
                getchar();
                input_file(digit + 1, file, book);
                scanf("%d", &v);
                break;
            case 3:
                v = 0;
                getchar();
                delete(file, book, digit);
                FILE *f = fopen("books.csv", "w");
                refresh(f, book, digit - 1);
                fclose(file);
                scanf("%d", &v);
                break;
            case 4:
                v = 0;
                getchar();
                search(file, book, digit);
                scanf("%d", &v);
                break;
            case 5:
                for (int i = 0; i < digit; ++i) {
                    free(book[i].num);
                    free(book[i].author);
                    free(book[i].title);
                    free(book[i].qf);
                    free(book[i].qt );
                }
                free(book);
                fclose(file);
                return 0;
        }
    }
}
/***************************Users*********************************/
struct users* input_users(struct users* user, FILE* fp)
{
    int n = 0;
    int checker = 0;
    while (checker != 1)
    {
        user[n].login = scan(&checker, fp);
        user[n].pass = scan(&checker, fp);
        user[n].stud = scan(&checker, fp);
        user[n].lib = scan(&checker, fp);
        n++;
        user = realloc(user, (n + 1) * sizeof(struct books));
    }
    return(user);
}

int check_users_lg(struct users* user, int n, char* str){
    int checker = 0;
    for (int i = 0; i < n; ++i) {
        if (strcmp(user[i].login, str) == 0) checker = 1;
    }
    return checker;
}

int check_users_ps(struct users* user, int n, char* str){
    int checker = 0;
    for (int i = 0; i < n; ++i) {
        if (strcmp(user[i].pass, str) == 0) checker = 1;
    }
    return checker;
}

int index(struct users* user, int n, char* str){
    int ind = -1;
    for (int i = 0; i < n; ++i) {
        if (strcmp(user[i].login, str) == 0) ind = i;
    }
    return ind;
}

int mode(char* login, char* password, struct users* user, int n) {
    int m;
    if (check_users_lg(user, n, login) == 1 && check_users_ps(user, n, password) == 1) {
        int i = index(user, n, login);
        int stud = char_to_int(user[i].stud);
        int lib = char_to_int(user[i].lib);
        if (stud == 1 && lib == 1) m = 1;
        if (stud == 1 && lib == 0) m = 2;
        if (stud == 0 && lib == 1) m = 3;
    } else m = 0;
    return m;
}

/***********************Students*********************************/

struct students* input_students(struct students* stud, FILE* fp) {
    int i = 0;
    int checker = 0;
    while (checker != 1)
    {
        stud[i].id = scan(&checker, fp);
        stud[i].name = scan(&checker, fp);
        stud[i].surname = scan(&checker, fp);
        stud[i].fname = scan(&checker, fp);
        stud[i].faculty = scan(&checker, fp);
        stud[i].spec = scan(&checker, fp);
        i++;
        stud = realloc(stud, (i + 1) * sizeof(struct students));
    }
    return(stud);
}

void show_s(struct students* stud, int n)
{
    int a;
    int j;
    printf("+----------+--------------+--------------+--------------+---------+------------------------+\n");
    printf("|    ID    |     Name     |    Surname   | Fathers name | Faculty |        Specialty       |\n");
    printf("+----------+--------------+--------------+--------------+---------+------------------------+\n");
    for (int i = 0; i < n; ++i)
    {
        printf("+----------+--------------+--------------+--------------+---------+------------------------+\n");
        a = 10 - page_proofs(stud[i].id);
        printf("|%s",stud[i].id);
        for (j = 0; j < a; ++j) {
            printf(" ");
        }
        printf("|%s", stud[i].name);
        a = 14 - page_proofs(stud[i].name);
        for (j = 0; j < a; ++j) {
            printf(" ");
        }
        printf("|%s", stud[i].surname);
        a = 14 - page_proofs(stud[i].surname);
        for (j = 0; j < a; ++j) {
            printf(" ");
        }
        printf("|%s", stud[i].fname);
        a = 14 - page_proofs(stud[i].fname);
        for (j = 0; j < a; ++j) {
            printf(" ");
        }
        printf("|%s", stud[i].faculty);
        a = 9 - page_proofs(stud[i].faculty);
        for (j = 0; j < a; ++j) {
            printf(" ");
        }
        printf("|%s", stud[i].spec);
        a = 24 - page_proofs(stud[i].spec);
        for (j = 0; j < a; ++j) {
            printf(" ");
        }
        printf("|\n");
        printf("+----------+--------------+--------------+--------------+---------+------------------------+\n");
    }
}

int check_s(struct students* stud, int n, char* str){
    int checker = 0;
    for (int i = 0; i < n; ++i) {
        if (strcmp(stud[i].id, str) == 0) checker = 1;
    }
    return checker;
}

void input_file_s(int digit, FILE* fp, struct students* stud){
    printf("Input student info:\n");
    stud = realloc(stud, (digit + 1) * sizeof(struct students));
    stud[digit].id = scan_console();
    if (check_s(stud, digit - 1, stud[digit].id) == 0) {
        stud[digit].name = scan_console();
        stud[digit].surname = scan_console();
        stud[digit].fname = scan_console();
        stud[digit].faculty = scan_console();
        stud[digit].spec = scan_console();
        fprintf(fp, "\n%s;%s;%s;%s;%s;%s", stud[digit].id, stud[digit].name, stud[digit].surname, stud[digit].fname,
                stud[digit].faculty, stud[digit].spec);
        printf("Student successfully added\n");
    } else {
        printf("Student with this ID already exists\n");
    }
}

void delete_s(struct students* stud, int n){
    printf("\nEnter student's ID you want to delete:\n");
    int i;
    int index;
    char* str = scan_console();
    for (i = 0; i < n; i++) {
        if (strcmp (stud[i].id, str) == 0) index = i;
    }
    for (i = index; i < n - 1; ++i) {
        stud[i].id = stud[i + 1].id;
        stud[i].name = stud[i + 1].name;
        stud[i].surname = stud[i + 1].surname;
        stud[i].fname = stud[i + 1].fname;
        stud[i].faculty = stud[i + 1].faculty;
        stud[i].spec = stud[i + 1].spec;
    }
    stud[n].id = NULL;
    stud[n].name = NULL;
    stud[n].surname = NULL;
    stud[n].fname = NULL;
    stud[n].faculty = NULL;
    stud[n].spec = NULL;
}

void refresh_s(FILE* fp, struct students* stud, int n){
    stud = realloc(stud, (n) * sizeof(struct students));
    for (int i = 0; i < n - 1; ++i) {
        fprintf(fp, "%s;%s;%s;%s;%s;%s\n", stud[i].id, stud[i].name, stud[i].surname, stud[i].fname, stud[i].faculty, stud[i].spec);
    }
    fprintf(fp, "%s;%s;%s;%s;%s;%s", stud[n - 1].id, stud[n - 1].name, stud[n - 1].surname, stud[n - 1].fname, stud[n - 1].faculty, stud[n - 1].spec);
    printf("Students successfully deleted\n");
}

void update_s(FILE* fp, struct students* stud, int n){
    for (int i = 0; i < n - 1; ++i) {
        fprintf(fp, "%s;%s;%s;%s;%s;%s\n", stud[i].id, stud[i].name, stud[i].surname, stud[i].fname, stud[i].faculty, stud[i].spec);
    }
    fprintf(fp, "%s;%s;%s;%s;%s;%s", stud[n - 1].id, stud[n - 1].name, stud[n - 1].surname, stud[n - 1].fname, stud[n - 1].faculty, stud[n - 1].spec);
}

void edit_s(struct students* stud, int n, FILE* file) {
    int i;
    int v;
    int index = -1;
    printf("Type ID of a student u want to edit\n");
    char* str = scan_console();
    for (i = 0; i < n; i++) {
        if (strcmp (stud[i].id, str) == 0) index = i;
    }
    str = NULL;
    if (index == -1) printf("There is not any student with this ID");
    else {
        fclose(file);
        FILE* refile= fopen("students.csv", "w");
        printf("What do u want to change ?\n");
        printf("1. ID\n");
        printf("2. Name\n");
        printf("3. Surname\n");
        printf("4. Father's name\n");
        printf("5. Faculty\n");
        printf("6. Specialty\n");
        scanf("%d", &v);
        switch (v) {
            case 1:
                getchar();
                printf("Type new ID:");
                str = scan_console();
                stud[index].id = str;
                update_s(refile, stud, n );
                printf("\nInfo successfully edited\n");
                break;
            case 2:
                getchar();
                printf("Type new name:");
                str = scan_console();
                stud[index].name = str;
                update_s(refile, stud, n);
                printf("\nInfo successfully edited\n");
                break;
            case 3:
                getchar();
                printf("Type new surname:");
                str = scan_console();
                stud[index].surname = str;
                update_s(refile, stud, n);
                printf("\nInfo successfully edited\n");
                break;
            case 4:
                getchar();
                printf("Type new father's name:");
                str = scan_console();
                stud[index].fname = str;
                update_s(refile, stud, n);
                printf("\nInfo successfully edited\n");
                break;
            case 5:
                getchar();
                printf("Type new faculty:");
                str = scan_console();
                stud[index].faculty = str;
                update_s(refile, stud, n);
                printf("\nInfo successfully edited\n");
                break;
            case 6:
                getchar();
                printf("Type new specialty:");
                str = scan_console();
                stud[index].spec = str;
                update_s(refile, stud, n);
                printf("\nInfo successfully edited\n");
                break;
        }
    }
}

int stud_moderator(FILE*file) {
    printf("\nU entered as a student moderator\n\n");
    int v;
    printf("Choose an option :\n");
    printf("1. Show all students\n");
    printf("2. Add a student\n");
    printf("3. Delete student\n");
    printf("4. Edit student's data\n");
    printf("5. Exit\n");
    int digit;
    struct students *stud = malloc(1 * sizeof(struct students));
    digit = quantity(file);
    stud = input_students(stud, file);
    scanf("%d", &v);
    while (1) {
        switch (v) {
            case 1:
                v = 0;
                show_s(stud, digit);
                scanf("%d", &v);
                break;
            case 2:
                v = 0;
                getchar();
                input_file_s(digit + 1, file, stud);
                scanf("%d", &v);
                break;
            case 3:
                v = 0;
                getchar();
                delete_s(stud, digit);
                FILE *f = fopen("students.csv", "w");
                refresh_s(f, stud, digit - 1);
                fclose(file);
                scanf("%d", &v);
                break;
            case 4:
                v = 0;
                getchar();
                edit_s(stud, digit, file);
                scanf("%d", &v);
                break;
            case 5:
                for (int i = 0; i < digit; ++i) {
                    free(stud[i].id);
                    free(stud[i].name);
                    free(stud[i].surname);
                    free(stud[i].fname);
                    free(stud[i].faculty);
                    free(stud[i].spec);
                }
                free(stud);
                fclose(file);
                return 0;
        }
    }
}

/*************************Admin****************************/

int admin (FILE* lib_file, FILE* stud_file) {
    printf("\nU entered as an admin\n\n");
    int v;
    printf("Choose an option :\n");
    printf("1. Show all students\n");
    printf("2. Add a student\n");
    printf("3. Delete student\n");
    printf("4. Edit student's data\n");
    printf("5. Show all books\n");
    printf("6. Add a book\n");
    printf("7. Delete book\n");
    printf("8. Find a book\n");
    printf("9. Exit\n");
    int digit;
    struct students *stud = malloc(1 * sizeof(struct students));
    digit = quantity(stud_file);
    stud = input_students(stud, stud_file);
    int digit_l;
    struct books *book = malloc(1 * sizeof(struct books));
    digit_l = quantity(lib_file);
    book = input(book, lib_file);
    scanf("%d", &v);
    while (1) {
        switch (v) {
            case 1:
                v = 0;
                show_s(stud, digit);
                scanf("%d", &v);
                break;
            case 2:
                v = 0;
                getchar();
                input_file_s(digit + 1, stud_file, stud);
                scanf("%d", &v);
                break;
            case 3:
                v = 0;
                getchar();
                delete_s(stud, digit);
                FILE *f = fopen("students.csv", "w");
                refresh_s(f, stud, digit - 1);
                fclose(stud_file);
                scanf("%d", &v);
                break;
            case 4:
                v = 0;
                getchar();
                edit_s(stud, digit, stud_file);
                scanf("%d", &v);
                break;
            case 5:
                v = 0;
                sort(lib_file, book, digit_l);
                show(book, digit_l);
                scanf("%d", &v);
                break;
            case 6:
                v = 0;
                getchar();
                input_file(digit_l + 1, lib_file, book);
                scanf("%d", &v);
                break;
            case 7:
                v = 0;
                getchar();
                delete(lib_file, book, digit_l);
                FILE *fp = fopen("books.csv", "w");
                refresh(fp, book, digit_l - 1);
                fclose(lib_file);
                scanf("%d", &v);
                break;
            case 8:
                v = 0;
                getchar();
                search(lib_file, book, digit_l);
                scanf("%d", &v);
                break;
            case 9:
                for (int i = 0; i < digit_l; ++i) {
                    free(book[i].num);
                    free(book[i].author);
                    free(book[i].title);
                    free(book[i].qf);
                    free(book[i].qt );
                }
                free(book);
                fclose(lib_file);
                return 0;
        }
    }
}
