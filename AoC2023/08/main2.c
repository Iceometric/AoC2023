#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256 + 32
#define LINE_COUNT 1024
#define OFFSET_DATA 2
#define OFFSET_LEFT 7
#define OFFSET_RIGHT 12
#define LAST_CHAR_KEY 2

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

void get_destination_ends_with(Destination *source, char search, Destination **destination) {
    size_t count = 0;
    for (size_t i = 0; i < LINE_COUNT; ++i) {
        if (source[i].key[LAST_CHAR_KEY] == search) {
            destination[count++] = &source[i];
        }
    }
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

void print_destination(Destination *d) {
    printf("%s(%p) (%p, %p)\n", d->key, (void *)d, (void*)d->l, (void*)d->r);
}

void set_directions(int directions[BUFFER_SIZE], int count, char *buffer) {
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

int destination_in_list(Destination *p, Destination **source) {
    int value = 0;
    if (p == NULL) return value;
    size_t i = 0;
    while (source[i]) {
        value = (p == source[i]);
        if (value) break;
        ++i;
    }
    return value;
}

int all_equal(unsigned long long *moves, size_t count) {
    int value = 0;
    unsigned long long previous = moves[0];
    for (size_t i = 1; i < count; ++i) {
        value = (moves[i] == previous);
        previous = moves[i];
        if (value) break;
    }

    return value;
}

unsigned long long get_max(unsigned long long *values, size_t count) {
    unsigned long long highest = 0;
    for (size_t i = 0; i < count; ++i)
        highest = highest < values[i] ? values[i] : highest;

    return highest;
}

int all_modulus_zero(unsigned long long *values, size_t count, unsigned long long modulus) {
    int value;
    for (size_t i = 0; i < count; ++i) {
        value = modulus % values[i] == 0;
        if (!value) break;
    }

    return value;
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

    Destination *starts[count];
    get_destination_ends_with(destinations, 'A', starts);
    Destination *ends[count];
    get_destination_ends_with(destinations, 'Z', ends);

    unsigned long long moves[count];
    memset(&moves, '\0', sizeof(unsigned long long) * count);

    int dir_index;
    Destination *d;
    unsigned long long steps;
    size_t move_count = 0;

    
    for (size_t i = 0; i < count; ++i) {
        if (starts[i] == NULL) break;
        d = starts[i];
        dir_index   = 0;
        steps       = 0;

        while (!destination_in_list(d, ends)) {
            if (!(dir_index < dir_count)) dir_index = 0;
            switch(directions[dir_index]) {
                case LEFT:  d = d->l; break;
                case RIGHT: d = d->r; break;
            }
            dir_index++;
            steps++;
        }
        printf("Reached destination at %llu steps\n", steps);
        print_destination(d);
        moves[move_count++] = steps;
    }
    unsigned long long max = get_max(moves, move_count);
    unsigned long long current = max;
    
    while (!all_modulus_zero(moves, move_count, current))
        current += max;

    return current;
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
