#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int getchar(void); // EOF = end-of-file
int getc(FILE *); // stdin, stdout, stderr
int fgetc(FILE *);

int putchar(int);
int putc(int, FILE *);
int fputc(int, FILE *);

printf()
scanf()

int main() {
    int a = 123;
    char buf[11];
    scanf("%10s", buf);
}
