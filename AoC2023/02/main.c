#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define BUFFER_SIZE 256
#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

enum Status {
    SUCCESS,
    INPUT_ERROR,
    FILE_ERROR
};

enum Colors {
    RED,
    GREEN,
    BLUE,
    COLOR_COUNT
};

int get_color(char *word) {
    char *COLORS[] = {
        "red",
        "green",
        "blue",
    };
    int is_equal = 0;
    char *color, *c;
    for (size_t i = 0; i < COLOR_COUNT; ++i) {
        color = COLORS[i];
        is_equal = 1;
        c = word;
        while (*color && is_equal) {
            is_equal = *color == *c;
            color++;
            c++;
        }
        
        if (is_equal) return i;
    }
    return -1;
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

int get_id(char *c) {
    char buffer[10];
    char *b = buffer;
    while (*c) {
        if (*c == ':') break;
        if (isdigit(*c)) *b++ = *c;
        c++;
    }
    return atoi(buffer);
}

int get_value_from_line(char *buffer) {
    char *v, value[10];
    char *c = buffer;
    int id, color, valid_id, int_value;
    int largest_red, largest_green, largest_blue;
    largest_red = largest_green = largest_blue = 0;

    id = get_id(c);

    while (*c && *c != ':') c++;
    c++;
    while (*c && id) {
        while (isspace(*c)) c++;

        v = value;
        while (isdigit(*c)) *v++ = *c++;
        *v = '\0';
        int_value = atoi(value);

        while (isspace(*c)) c++;

        color = get_color(c);

        switch(color) {
            case RED:
                valid_id = int_value <= MAX_RED;
                largest_red = largest_red < int_value ? int_value : largest_red;
                break;
            case GREEN:
                valid_id = int_value <= MAX_GREEN;
                largest_green = largest_green < int_value ? int_value : largest_green;
                break;
            case BLUE:
                valid_id = int_value <= MAX_BLUE;
                largest_blue = largest_blue < int_value ? int_value : largest_blue;
                break;
        }

        //if (!valid_id) break;

        while (!isspace(*c)) c++;

        c++;
    }

    printf("ID: %d red: %d, green: %d, blue: %d\n", id, largest_red, largest_green, largest_blue);
    //return id * valid_id;
    return largest_red * largest_green * largest_blue;
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
    //printf("ID sum is: %d\n", get_sum(input));
    printf("The power of cubes is: %d\n", get_sum(input));
    fclose(input);
}

int main(void) {
    run();
    return SUCCESS;
}
