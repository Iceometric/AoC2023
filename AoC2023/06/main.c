#include <stdio.h>
#include <stdlib.h>

// #define RACE_COUNT 4
#define RACE_COUNT 1

enum Status {
    SUCCESS,
    INPUT_ERROR,
    FILE_ERROR
};

int get_race_sum(void) {

    // int time[RACE_COUNT] = { 44, 80, 65, 72 };
    // int distance[RACE_COUNT] = { 208, 1581, 1050, 1102 };
    long long time[RACE_COUNT] = { 44806572  };
    long long distance[RACE_COUNT] = { 208158110501102 };

    long long win_count = 0;
    long long sum = 1;

    long long t, d, result;
    for (size_t i = 0; i < RACE_COUNT; ++i) {
        t = time[i];
        d = distance[i];
        win_count = 0;
        
        for (long long press = 1; press < t; ++press) {
            result = (t - press) * press;
            if(result > d) {
                ++win_count;
                if (win_count % 100000 == 0) printf("%lld\n", win_count);
            }
        }

        sum *= win_count;
    }
    return sum;
}

void run(void) {
    printf("The race sum: %d\n", get_race_sum());
}

int main(void) {
    run();
    return SUCCESS;
}
