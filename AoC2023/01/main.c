#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define BUFFER_SIZE 256

enum Status {
    SUCCESS,
    INPUT_ERROR,
    FILE_ERROR
};

int get_digit_word(char *word) {
    char *NUMBERS[] = {
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine"
    };
    int is_equal = 0;
    char *digit, *c;
    for (size_t i = 0; i < 9; ++i) {
        digit = NUMBERS[i];
        is_equal = 1;
        c = word;
        while (*digit && is_equal) {
            is_equal = *digit == *c;
            digit++;
            c++;
        }
        
        if (is_equal) return (i + 1);
    }
    return 0;
}

int get_line(FILE *file, char *buffer) {
    char c;
    while (EOF != (c = getc(file))) {
        *buffer++ = c;
        if (c == '\n') break;
    }
    *buffer = '\0';
    return c == EOF;
}

int get_value_from_line(char *buffer) {
    char *c = buffer;
    int value, word_value, found_first, sum;
    found_first = sum = 0;
    while (*c) {
        if ((word_value = get_digit_word(c))) {
            value = word_value;
            if (!found_first) {
                sum += value * 10;
                found_first = 1;
            }
        }
        if (isdigit(*c)) {
            value = *c - '0';
            if (!found_first) {
                sum += value * 10;
                found_first = 1;
            }
        }
        if (*c == '\n') {
            sum += value;
            value = found_first = 0;
        }
        c++;
    }
    return sum;
}

int get_sum(FILE *file) {
    int sum, end_of_file;
    char buffer[BUFFER_SIZE];
    sum = 0;
    while(1) {
        end_of_file = get_line(file, buffer);
        sum += get_value_from_line(buffer);
        if (end_of_file) break;
    }
    return sum;
}

void run(void) {
    FILE *input = fopen("input.txt", "r");
    if (!input) exit(-FILE_ERROR);
    printf("Calibration value is: %d\n", get_sum(input));
    fclose(input);
}

int main(void) {
    run();
    return SUCCESS;
}
