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

int get_value_from_line(char buffer[BUFFER_SIZE], int *additional_cards) {
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
        
        if (value) *current++ = value;

        c++;
    }
    
    do {
        int *a = additional_cards + 1;
        int *p = player_numbers;
        int *w = winning_numbers;
        while (*p) {
            w = winning_numbers;
            while (*w) {
                if (*w == *p) {
                    *a++ += 1;
                }
                w++;
            }
            p++;
        }
        score++;
    } while(additional_cards[0]--);

    int *old = additional_cards;
    int *new = additional_cards + 1;
    while (*new) *old++ = *new++;
    *new = 0;
    
    return score;
}

int get_score(FILE *file) {
    int sum = 0;
    int end_of_file = 0;
    char buffer[BUFFER_SIZE] = {0};
    int additional_cards[50] =  {0};
    int i = 0;
    while(1) {
        printf("%d\n",i++);
        end_of_file = get_line(file, buffer);
        if (end_of_file) break;
        sum += get_value_from_line(buffer, additional_cards);
    }
    return sum - 1;
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
