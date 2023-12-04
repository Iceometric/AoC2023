#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256
#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

enum Status {
    SUCCESS,
    INPUT_ERROR,
    FILE_ERROR
};

enum ReadFunction {
    TOP,
    READ,
    BOT,
    ROW_FUNCTION_COUNT
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

int set_null(char *buffer) {
    memset(buffer, '\0', BUFFER_SIZE);
    return 1;
}

int valid_char(char c) {
    return c != '.' && !isdigit(c);
}

int validate_value_with_start_end(char buffer[ROW_FUNCTION_COUNT][BUFFER_SIZE], int start, int end) {
    int is_valid = 0;

    for (int i = start;i <= end; ++i) {
        for (int j = 0; j < ROW_FUNCTION_COUNT; ++j) {
            if (buffer[j][i]) {
                is_valid = valid_char(buffer[j][i]);
                if (is_valid) goto exit_nested_loop;
            }
        }
    }
exit_nested_loop:
    return is_valid;
}

int get_value_from_line_array(char buffer[ROW_FUNCTION_COUNT][BUFFER_SIZE]) {
    char *v, value_buffer[10];
    char *c = &buffer[READ][0];
    int i = 0;
    int sum = 0;
    int is_digit_sequance = 0;
    int start, end;
    v = value_buffer;
    while (*c) {
        if (isdigit(*c)) {
            if (is_digit_sequance == 0) {
                start = i - 1 < 0 ? 0 : i - 1;
            }
            is_digit_sequance++;
            *v++ = *c;
        } else {
           *v = '\0';
           end = i;
           is_digit_sequance = 0;
           if (validate_value_with_start_end(buffer, start, end)) sum += atoi(value_buffer);
           v = value_buffer;
        }
        c++;
        i++;
    }
    return sum;
}

int get_sum(FILE *file) {
    int sum = 0;
    int end_of_file = 0;
    char buffer[ROW_FUNCTION_COUNT][BUFFER_SIZE] = {0};
    while(1) {
        memcpy(buffer[TOP], buffer[READ], BUFFER_SIZE);
        memcpy(buffer[READ], buffer[BOT], BUFFER_SIZE);
        end_of_file = !end_of_file ? get_line(file, &buffer[BOT][0])  : set_null(buffer[BOT]);
        sum += get_value_from_line_array(buffer);
        if (end_of_file) break;
    }
    return sum;
}

void run(void) {
    FILE *input = fopen("input.txt", "r");
    if (!input) exit(-FILE_ERROR);
    printf("The sum of the engineering schema is: %d\n", get_sum(input));
    fclose(input);
}

int main(void) {
    run();
    return SUCCESS;
}
