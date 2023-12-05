#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 256
#define BUFFER_COUNT 100
#define SEED_COUNT 30

enum Status {
    SUCCESS,
    INPUT_ERROR,
    FILE_ERROR
};

enum IndexType {
    DESTINATION_RANGE_START = 0,
    SOURCE_RANGE_START,
    RANGE_LENGTH,
    INDEX_TYPE_COUNT
};

enum MapType {
    SEED_TO_SOIL = 0,
    SOIL_TO_FERTILIZER,
    FERTILIZER_TO_WATER,
    WATER_TO_LIGHT,
    LIGHT_TO_TEMPERATURE,
    TEMPERATURE_TO_HUMIDIY,
    HUMIDITY_TO_LOCATION,
    MAP_TYPE_COUNT
};

int get_seeds(FILE *file, unsigned long long seeds[SEED_COUNT]) {
    char c;
    unsigned long long val = 0;
    int seed = 0;
    char *v, value[30];
    v = value;
    while (EOF != (c = getc(file))) {
        if (isdigit(c)) {
            *v++ = c;
        } else {
            *v = '\0';
            v = value;
            if (strlen(value) > 0) {
                if (strstr(value, "-")) {
                    printf("OH NEJ NEJ\n");
                }
                val = atoll(value);
                seeds[seed++] = val;
            }
        }
        if (c == '\n') break;
    }
    return seed > 0;
}

void get_maps(FILE *file, char buffer[MAP_TYPE_COUNT][BUFFER_COUNT][BUFFER_SIZE]) {
    char c, prev;
    size_t map, count, i;
    map = count = i = 0;
    prev = '\0';
    int is_map_name = 0;
    while (EOF != (c = getc(file))) {
        if (isalpha(c) || c == '-') {
            while (!isdigit(c)) c = getc(file);
        }
        // printf("m:%zu,c:%zu,i:%zu - %c\n", map, count, i, c);
        buffer[map][count][i++] = c;
        if (c == '\n') {
            ++count;
            i = 0;
        }
        if (c == '\n' && prev == '\n') {
            ++map;
            count = i = 0;
        }
        prev = c;
    }
}

int get_properties(char *c, unsigned long long values[INDEX_TYPE_COUNT]) {
    char *v, value[40];
    v = value;
    int type = 0;
    while(*c) {
        if (isdigit(*c)) {
            *v++ = *c;
        } else {
            *v = '\0';
            v = value;
            if (strlen(value) > 0) {
                values[type++] = atoll(value);
            }
        }
        c++;
    }
    return values[0] == 0 && values[1] == 0 && values[2] == 0;
}


unsigned long long get_index_from_map(unsigned long long index, char buffer[BUFFER_COUNT][BUFFER_SIZE]) {

    int i = 0;
    unsigned long long values[INDEX_TYPE_COUNT];
    printf("index: %llu\n", index);
    while (buffer[i][0]) {
        memset(values, '\0', sizeof(unsigned long long) * INDEX_TYPE_COUNT);
        if (get_properties(&buffer[i++][0], values)) continue;

        for (size_t test = 0; test < INDEX_TYPE_COUNT; ++test) {
            printf("%llu ", values[test]);
        }
        printf("\n");

        if (index >= values[SOURCE_RANGE_START] 
            && index < values[SOURCE_RANGE_START] + values[RANGE_LENGTH]) {
            unsigned long long start = values[SOURCE_RANGE_START];
            unsigned long long end = values[SOURCE_RANGE_START] + values[RANGE_LENGTH];
            printf("%llu between %llu - %llu\n", index, start, end);
            unsigned long long test = values[DESTINATION_RANGE_START] + (index - values[SOURCE_RANGE_START]);
            printf("new index = %llu\n", test);
            return values[DESTINATION_RANGE_START] + (index - values[SOURCE_RANGE_START]);
        }

        printf("\n");
    }
    // for (size_t test = 0; test < INDEX_TYPE_COUNT; ++test) {
    //     printf("%llu ", values[test]);
    // }

    //while (*c) {

    //}
    return index;
}

unsigned long long get_lowest_location(FILE *file) {
    char buffer[MAP_TYPE_COUNT][BUFFER_COUNT][BUFFER_SIZE] = {0};
    unsigned long long seeds[SEED_COUNT] = {0};
    int i, got_first_value;
    unsigned long long lowest_location, index;
    index = got_first_value = i = 0;

    get_seeds(file, seeds);
    get_maps(file, buffer);
    while (seeds[i]) {
        printf("At seed: %llu\n", seeds[i]);
        index = seeds[i];
        for (int map = SEED_TO_SOIL; map < MAP_TYPE_COUNT; ++map) {
            index = get_index_from_map(index, buffer[map]);
        }
        if (!got_first_value) {
            lowest_location = index;
            got_first_value = 1;
        }
        lowest_location = lowest_location < index ? lowest_location : index;
        i++;
    }
    return lowest_location;
}

void run(void) {
    FILE *input = fopen("input.txt", "r");
    if (!input) exit(-FILE_ERROR);
    printf("The lowest location is: %llu\n", get_lowest_location(input));
    fclose(input);
}

int main(void) {
    run();
    return SUCCESS;
}
