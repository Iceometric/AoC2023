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
    while (EOF != (c = getc(file))) {
        if (isalpha(c) || c == '-') {
            while (!isdigit(c)) c = getc(file);
        }
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
    unsigned long long start, end, result;
    while (buffer[i][0]) {
        memset(values, '\0', sizeof(unsigned long long) * INDEX_TYPE_COUNT);
        if (get_properties(&buffer[i++][0], values)) continue;

        start = values[DESTINATION_RANGE_START];
        end = values[DESTINATION_RANGE_START] + values[RANGE_LENGTH];

        if (index >= start && index < end) {
            result = values[SOURCE_RANGE_START] + (index - values[DESTINATION_RANGE_START]);
            return result;
        }
    }
    return index;
}

int exists_in_seeds(unsigned long long index, unsigned long long seeds[SEED_COUNT]) {
    unsigned long long start, end;
    
    size_t i = 0;
    while(seeds[i]) {
        start = seeds[i++];
        end = seeds[i++] + start;

        if (index >= start && index <= end) {
            printf("index: %llu", index);
            return 1;
        }
    }
    return 0;
}

unsigned long long get_lowest_location(FILE *file) {
    char buffer[MAP_TYPE_COUNT][BUFFER_COUNT][BUFFER_SIZE] = {0};
    unsigned long long seeds[SEED_COUNT] = {0};

    unsigned long long index, i;

    get_seeds(file, seeds);
    get_maps(file, buffer);
    i = 0;


    while (1) {
        if (i % 10000 == 0) printf("%lld\n", i);
        index = i;
        for (int map = HUMIDITY_TO_LOCATION; map >= 0; --map) {
            index = get_index_from_map(index, buffer[map]);
        }
        if (exists_in_seeds(index, seeds)) return i;

        i++;
    }
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
