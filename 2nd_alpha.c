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

int stud_moderator(FILE*);

struct students* input_students(struct students*, FILE*);

void stud_sort(FILE *, struct students* , int );

void show_s(struct students*, int);

int check_s(struct students*, int, char*);

void input_file_s(int, FILE*, struct students*);

void delete_s(FILE* , struct students* , int );

void refresh_s(FILE* fp, struct students* stud, int n);

void edit_s();
/*****************Users*************************/
struct users* input_users(struct users*, FILE*);

int check_users_lg(struct users*, int, char*);

int check_users_ps(struct users*, int, char*);

int index(struct users* , int, char*);

int mode(char*, char*, struct users*, int);

int main() {
    FILE *fusers = fopen("users.csv", "r");
    int digit_users = quantity(fusers);
    struct users* user = malloc(1 * sizeof(struct users));
    user = input_users(user, fusers);
    FILE *file = fopen("books.csv", "a+");
    FILE *studfile = fopen("students.csv", "a+");
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
            printf("\nU entered as an admin\n");
        break;
        case 2:
            return(stud_moderator(studfile));
        break;
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

int stud_moderator(FILE*file) {
    printf("\nU entered as a student moderator\n\n");
    int v;
    printf("Choose an option :\n");
    printf("1. Show all students\n");
    printf("2. Add a student\n");
    printf("3. Delete student\n");
    printf("4. Edit student\n");
    printf("5. Exit\n");
    int digit;
    struct students *stud = malloc(1 * sizeof(struct books));
    digit = quantity(file);
    stud = input_students(stud, file);
    scanf("%d", &v);
    while (1) {
        switch (v) {
            case 1:
                v = 0;
                stud_sort(file, stud, digit);
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
                delete_s(file, stud, digit);
                FILE *f = fopen("students.csv", "w");
                refresh_s(f, stud, digit - 1);
                fclose(file);
                scanf("%d", &v);
                break;
            case 4:
                v = 0;
                getchar();
                edit_s(file, stud, digit);
                scanf("%d", &v);
                break;
            case 5:
                for (int i = 0; i < digit; ++i) {
                    free(stud[i].id);
                    free(stud[i].name);
                    free(stud[i].surname);
                    free(stud[i].fname);
                    free(stud[i].faculty );
                    free(stud[i].spec );
                }
                free(stud);
                fclose(file);
                return 0;
        }
    }
}

struct students* input_students(struct students* stud, FILE* fp) {
    int n = 0;
    int checker = 0;
    while (checker != 1)
    {
        stud[n].id = scan(&checker, fp);
        stud[n].name = scan(&checker, fp);
        stud[n].surname = scan(&checker, fp);
        stud[n].fname = scan(&checker, fp);
        stud[n].faculty = scan(&checker, fp);
        stud[n].spec = scan(&checker, fp);
        n++;
        stud = realloc(stud, (n + 1) * sizeof(struct books));
    }
    return(stud);
}

void show_s(struct students* stud, int n)
{
    int a;
    int j = 0;
    printf("+--------------+-------------------+-----------------+-------------------+--------------+--------------------------------------------------+\n");
    printf("|      ID      |        Name       |     Surname     |    Fathers name   |    Faculty   |                    Specialty                     |\n");
    printf("+--------------+-------------------+-----------------+-------------------+--------------+--------------------------------------------------+\n");
    for (int i = 0; i < n; ++i)
    {
        printf("+--------------+-------------------+-----------------+-------------------+--------------+--------------------------------------------------+\n");
        a = 14 - page_proofs(stud[i].id);
        printf("|%s",stud[i].id);
        for (j = 0; j < a; ++j) {
            printf(" ");
        }
        printf("|%s", stud[i].name);
        a = 19 - page_proofs(stud[i].name);
        for (j = 0; j < a; ++j) {
            printf(" ");
        }
        printf("|%s", stud[i].surname);
        a = 17 - page_proofs(stud[i].surname);
        for (j = 0; j < a; ++j) {
            printf(" ");
        }
        printf("|%s", stud[i].fname);
        a = 19 - page_proofs(stud[i].fname);
        for (j = 0; j < a; ++j) {
            printf(" ");
        }
        printf("|%s", stud[i].faculty);
        a = 14 - page_proofs(stud[i].faculty);
        for (j = 0; j < a; ++j) {
            printf(" ");
        }
        printf("|%s", stud[i].spec);
        a = 50 - page_proofs(stud[i].spec);
        for (j = 0; j < a; ++j) {
            printf(" ");
        }
        printf("|\n");
        printf("+--------------+-------------------+-----------------+-------------------+--------------+--------------------------------------------------+\n");
    }
}

void stud_sort(FILE *fp, struct students* stud, int n){
    char* s;
    int i = 0;
    int k = 0;
    int* num = malloc(n * sizeof(int));
    for (i = 0; i < n; ++i) {
        num[i] = char_to_int(stud[i].id);
    }
    shell(n, num);
    for (i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (strcmp(int_to_char(num[i]), stud[j].id) == 0) {
                s = stud[k].id;
                stud[k].id = stud[j].id;
                stud[j].id = s;
                s = stud[k].name;
                stud[k].name = stud[j].name;
                stud[j].name = s;
                s = stud[k].surname;
                stud[k].surname = stud[j].surname;
                stud[j].surname = s;
                s = stud[k].fname;
                stud[k].fname = stud[j].fname;
                stud[j].fname = s;
                s = stud[k].faculty;
                stud[k].faculty = stud[j].faculty;
                stud[j].faculty = s;
                s = stud[k].spec;
                stud[k].spec = stud[j].spec;
                stud[j].spec = s;
            }
        }
        k++;
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
        printf("Student with this ID already exist\n");
    }
}

void delete_s(FILE* fp, struct students* stud, int n){
    printf("\nEnter student's ID you want to delete:\n");
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
    stud = realloc(stud, (n) * sizeof(struct books));
    for (int i = 0; i < n - 1; ++i) {
        fprintf(fp, "%s;%s;%s;%s;%s;%s\n", stud[i].id, stud[i].name, stud[i].surname, stud[i].fname, stud[i].faculty, stud[i].spec);
    }
    fprintf(fp, "%s;%s;%s;%s;%s;%s", stud[n - 1].id, stud[n - 1].name, stud[n - 1].surname, stud[n - 1].fname, stud[n - 1].faculty, stud[n - 1].spec);
    printf("Students successfully deleted\n");
}

void edit_s() {
    /****/
}
