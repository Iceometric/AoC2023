#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256 + 32
#define LINE_COUNT 1024
#define OFFSET_DATA 2
#define OFFSET_LEFT 7
#define OFFSET_RIGHT 12

#define KEY_SIZE 3

enum Status {
    SUCCESS,
    INPUT_ERROR,
    FILE_ERROR
};

enum Direction {
    LEFT = 1,
    RIGHT
};

typedef struct Destination {
    char key[KEY_SIZE + 1];
    struct Destination *l;
    struct Destination *r;
} Destination;

int get_lines(FILE *file, char buffer[LINE_COUNT][BUFFER_SIZE]) {
    char c;
    int i, line;
    i = line = 0;
    while (EOF != (c = getc(file))) {
        buffer[line][i++] = c;
        if (c == '\n') {
            buffer[line][i] = '\0';
            i = 0;
            ++line;
        }
    }
    return line;
}

Destination *get_destination(Destination *d, char *search) {
    for (size_t i = 0; i < LINE_COUNT; ++i) {
        if (strcmp(d[i].key, search) == 0) {
            return &d[i];
        }
    }
    exit(-INPUT_ERROR);
}

void populateDestinationsFromBuffer(Destination *d, char buffer[LINE_COUNT][BUFFER_SIZE], size_t count) {
    // set keys
    for (size_t i = 0; i < count; ++i) {
        for (size_t j = 0; j < KEY_SIZE; ++j) {
            d[i].key[j] = buffer[i + OFFSET_DATA][j];
        }
        d[i].key[KEY_SIZE] = '\0';
    }

    // pair pointers
    char left[KEY_SIZE + 1];
    char right[KEY_SIZE + 1];
    for (size_t i = 0; i < count; ++i) {
        for (size_t j = 0; j < KEY_SIZE; ++j) {
            left[j] = buffer[i + OFFSET_DATA][j + OFFSET_LEFT];
            right[j] = buffer[i + OFFSET_DATA][j + OFFSET_RIGHT];
        }
        left[KEY_SIZE] = '\0';
        right[KEY_SIZE] = '\0';
        d[i].l = get_destination(d, left);
        d[i].r = get_destination(d, right);
    }
}

void set_directions(int directions[BUFFER_SIZE], int count, char *buffer) {
    // size_t i = 0;
    int is_left = 0;
    for (size_t i = 0; i < count; ++i) {
        is_left = buffer[i] == 'L';
        directions[i] = is_left ? LEFT : RIGHT;
    }
}

int direction_count(char *str) {
    int i = 0;
    while (str[i] != '\n') ++i;
    return i;
}

unsigned long long get_movement_count(FILE *file) {
    char buffer[LINE_COUNT][BUFFER_SIZE];
    int count = get_lines(file, buffer);

    int dir_count = direction_count(&buffer[0][0]);
    int directions[dir_count];
    memset(&directions, '\0', sizeof(int) * dir_count);
    set_directions(directions, dir_count, &buffer[0][0]);

    Destination destinations[count];
    memset(&destinations, '\0', sizeof(Destination) * count);

    populateDestinationsFromBuffer(destinations, buffer, count);

    unsigned long long sum = 0;
    int dir_index = 0;
    Destination *d = get_destination(destinations, "AAA");
    while (strcmp(d->key, "ZZZ") != 0) {
        if (!(dir_index < dir_count)) dir_index = 0;
        switch(directions[dir_index]) {
            case LEFT:  d = d->l; break;
            case RIGHT: d = d->r; break;
        }
        sum++;
        dir_index++;
    }
    return sum;
}

void run(void) {
    FILE *input = fopen("input.txt", "r");
    if (!input) exit(-FILE_ERROR);
    printf("The movement count is: %llu\n", get_movement_count(input));
    fclose(input);
}

int main(void) {
    run();
    return SUCCESS;
}
