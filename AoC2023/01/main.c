#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

enum Status {
    SUCCESS,
    INPUT_ERROR,
    FILE_ERROR
};

int get_sum(FILE *file) {
    int c, sum, value, found_first;
    found_first = value = sum = 0;
    while (EOF != (c = getc(file))) {
        if (isdigit(c)) {
            value = c - '0';
            if (!found_first) {
                sum += value * 10;
                found_first = 1;
            }
        }
        if (c == '\n') {
            sum += value;
            value = found_first = 0;
        }
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
