#include <stdio.h>
#include <stdlib.h>
#include "../include/mystrfunctions.h"
#include "../include/myfilefunctions.h"

int main() {
    printf("--- Testing String Functions ---\n");

    char dest[100];

    printf("Length of 'hello': %d\n", mystrlen("hello"));

    mystrcpy(dest, "Copied string");
    printf("mystrcpy result: %s\n", dest);

    mystrncpy(dest, "TruncatedText", 5);
    printf("mystrncpy result (5 chars): %s\n", dest);

    mystrcpy(dest, "Hello ");
    mystrcat(dest, "World");
    printf("mystrcat result: %s\n", dest);

    printf("\n--- Testing File Functions ---\n");

    FILE* fp = fopen("src/main.c", "r");
    if (fp != NULL) {
        int lines, words, chars;
        wordCount(fp, &lines, &words, &chars);
        printf("main.c -> Lines: %d, Words: %d, Chars: %d\n", lines, words, chars);
        fclose(fp);
    }

    fp = fopen("src/main.c", "r");
    if (fp != NULL) {
        char** matches;
        int found = mygrep(fp, "printf", &matches);
        printf("Lines containing 'printf': %d\n", found);
        for (int i = 0; i < found; i++) {
            printf("  %s", matches[i]);
            free(matches[i]);
        }
        free(matches);
        fclose(fp);
    }

    return 0;
}
