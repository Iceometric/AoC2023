#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256

enum Status {
    SUCCESS,
    INPUT_ERROR,
    FILE_ERROR
};

int get_line(FILE *file, char *buffer) {
    char c;
    while (EOF != (c = getc(file))) {
        *buffer++ = c;
        if (c == '\n') break;
    }
    *buffer = '\0';
    return c == EOF;
}

int get_value_from_line(char buffer[BUFFER_SIZE]) {
    char *v, value_buffer[10];
    char *c = &buffer[0];
    int score = 0;
    int winning_numbers[30] = {0};
    int player_numbers[30] =  {0};
    int *current, value;
    current = winning_numbers;

    while (*c && *c != ':') c++;
    c++;
    while (*c) {
        while (isspace(*c)) c++;
        if (*c == '|') current = player_numbers;

        v = value_buffer;
        while (isdigit(*c)) *v++ = *c++;
        *v = '\0';
        value = atoi(value_buffer);
        
        if (value) {
            *current++ = value;
        }

        c++;
    }
    
    int *p = player_numbers;
    int *w = winning_numbers;
    while (*p) {
        w = winning_numbers;
        printf("NOW p:%d\n", *p);
        while (*w) {
            if (*w == *p) {
                printf("w:%d == p:%d\n", *w, *p);
                score *= 2;
                if (score == 0) score = 1;
            }
            w++;
        }
        p++;
    }

    return score;
}

int get_score(FILE *file) {
    int sum = 0;
    int end_of_file = 0;
    char buffer[BUFFER_SIZE] = {0};
    while(1) {
        end_of_file = get_line(file, buffer);
        if (end_of_file) break;
        sum += get_value_from_line(buffer);
    }
    return sum;
}

void run(void) {
    FILE *input = fopen("input.txt", "r");
    if (!input) exit(-FILE_ERROR);
    printf("The score of scratch cards is: %d\n", get_score(input));
    fclose(input);
}

int main(void) {
    run();
    return SUCCESS;
}
