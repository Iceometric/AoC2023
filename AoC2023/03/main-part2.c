#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

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
    return c == '*';
}

int get_digit_sequence_as_int(char *row, int index) {
    char *v, buffer[10];
    char *c = &row[index];
    v = buffer;
    while (isdigit(*c) && c >= row) c--;
    c++;
    while (*c && isdigit(*c)) *v++ = *c++;
    *v = '\0';
    return atoi(buffer);
}

int *get_adjacent_value_for_row(char row[BUFFER_SIZE], int index, int *values) {
    int is_digit_sequance = 0;
    int *current = values;
    for (int i = -1; i < 2; ++i) {
        if (index + i < 0) continue;
        if (isdigit(row[index + i])) {
            if (is_digit_sequance == 0) {
                *current++ = get_digit_sequence_as_int(row, index + i);
            }
            is_digit_sequance++;
        } else {
            is_digit_sequance = 0;
        }
    }
    return current;
}

int *populate_buffer_from_index(char buffer[ROW_FUNCTION_COUNT][BUFFER_SIZE], int index, int *values) {
    int *current = get_adjacent_value_for_row(&buffer[TOP][0], index, values);
    current = get_adjacent_value_for_row(&buffer[READ][0], index, current);
    current = get_adjacent_value_for_row(&buffer[BOT][0], index, current);
    return current;
}

int get_value_from_line_array(char buffer[ROW_FUNCTION_COUNT][BUFFER_SIZE]) {
    char *read_row = &buffer[READ][0];
    int i = 0;
    int iteration_sum;
    int sum = 0;
    int *v, *end;
    int value_buffer[10] = {0};
    while (*read_row) {
        if (valid_char(*read_row)) {
            iteration_sum = 0;
            v = value_buffer;
            memset(value_buffer, '\0', sizeof(int) * 10);
            end = populate_buffer_from_index(buffer, i, value_buffer);
            if (end - value_buffer > 1) {
                while (*v) {
                    iteration_sum = max(iteration_sum, 1) * *v++;
                }
            }
            sum += iteration_sum;
        }
        read_row++;
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
        end_of_file = !end_of_file ? get_line(file, &buffer[BOT][0]) : set_null(buffer[BOT]);
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
