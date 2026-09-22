#include "../include/myfilefunctions.h"
#include <stdlib.h>
#include <string.h>

int wordCount(FILE* file, int* lines, int* words, int* chars) {
    if (file == NULL) return -1;

    *lines = 0;
    *words = 0;
    *chars = 0;

    int c;
    int in_word = 0;

    while ((c = fgetc(file)) != EOF) {
        (*chars)++;
        if (c == '\n') {
            (*lines)++;
        }
        if (c == ' ' || c == '\n' || c == '\t') {
            in_word = 0;
        } else if (in_word == 0) {
            in_word = 1;
            (*words)++;
        }
    }
    return 0;
}

int mygrep(FILE* fp, const char* search_str, char*** matches) {
    if (fp == NULL) return -1;

    char buffer[1024];
    int count = 0;
    int capacity = 10;

    *matches = malloc(capacity * sizeof(char*));

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strstr(buffer, search_str) != NULL) {
            if (count >= capacity) {
                capacity *= 2;
                *matches = realloc(*matches, capacity * sizeof(char*));
            }
            (*matches)[count] = malloc(strlen(buffer) + 1);
            strcpy((*matches)[count], buffer);
            count++;
        }
    }
    return count;
}
