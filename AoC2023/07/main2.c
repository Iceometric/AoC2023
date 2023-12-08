#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 32
#define LINE_COUNT 1100

#define HAND_SIZE 5
#define CARD_TYPE_COUNT 13

enum Status {
    SUCCESS,
    INPUT_ERROR,
    FILE_ERROR
};

/** Lower the index the stronger the hand */
typedef enum HandType {
    FIVE_OF_A_KIND = 0,
    FOUR_OF_A_KIND,
    FULL_HOUSE,
    THREE_OF_A_KIND,
    TWO_PAIR,
    ONE_PAIR,
    HIGH_CARD
} HandType;

typedef struct Card {
    char hand[HAND_SIZE + 1];
    HandType type;
    int bid;
} Card;

/** Lower the index the stronger the card */
const char CARDS[] = { 'A', 'K', 'Q', 'T', '9', '8', '7', '6', '5', '4', '3', '2', 'J' };
// const char CARDS[] = { 'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2' };

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

size_t get_card_index(char c) {
    for (size_t i = 0; i < CARD_TYPE_COUNT; ++i) {
        if (c == CARDS[i]) return i;
    }
    return CARD_TYPE_COUNT;
}

void move_J_to_most_beneficial(int map[CARD_TYPE_COUNT]) {
    int j = get_card_index('J');
    int highest = 0;
    int highest_index = 0;

    for (size_t i = 0; i < j; ++i) {
        if (map[i] > highest) {
            highest = map[i];
            highest_index = i;
        }
    }
    map[highest_index] += map[j];
    map[j] = 0;
}

void set_hand_type(Card *card) {
    int map[CARD_TYPE_COUNT] = {0};
    for (size_t i = 0; i < HAND_SIZE; ++i)
        map[get_card_index(card->hand[i])]++;

    move_J_to_most_beneficial(map);

    int count[5] = {0};
    for (size_t i = 0; i < CARD_TYPE_COUNT; ++i)
        switch(map[i]) {
            case 0: break;
            case 1: count[0]++; break;
            case 2: count[1]++; break;
            case 3: count[2]++; break;
            case 4: count[3]++; break;
            case 5: count[4]++; break;
        }

    if (count[4] == 1) {
        card->type = FIVE_OF_A_KIND;
        return;
    }
    if (count[3] == 1) {
        card->type = FOUR_OF_A_KIND;
        return;
    }
    if (count[2] == 1 && count[1] == 1) {
        card->type = FULL_HOUSE;
        return;
    }
    if (count[2] == 1) {
        card->type = THREE_OF_A_KIND;
        return;
    }
    if (count[1] == 2) {
        card->type = TWO_PAIR;
        return;
    }
    if (count[1] == 1) {
        card->type = ONE_PAIR;
        return;
    }

    card->type = HIGH_CARD;
}

void get_hand_data(char *c, Card *card) {
    for (size_t i = 0; i < HAND_SIZE; ++i) {
        card->hand[i] = c[i];
        if (i == HAND_SIZE - 1) card->hand[++i] = '\0';
    }
    card->bid = atoi(&c[HAND_SIZE + 1]);
    set_hand_type(card);
}

void print_card_data(Card *card) {
    printf("%s | bid: %04d | type: %d\n", card->hand, card->bid, card->type);
}

int lowest_hand_first(Card *c1, Card *c2) {
    int value;
    for (size_t i = 0; i < HAND_SIZE; ++i) {
        value = get_card_index(c2->hand[i]) - get_card_index(c1->hand[i]);
        if (value != 0) break;
    }
    return value;
}

int lowest_value_first(const void *a, const void *b) {
    Card *c1 = (Card*)a;
    Card *c2 = (Card*)b;

    int value = c2->type - c1->type;
    if (value == 0) value = lowest_hand_first(c1, c2);

    return value;
}

int get_hands_sum(FILE *file) {
    char buffer[LINE_COUNT][BUFFER_SIZE];
    int count = get_lines(file, buffer);

    Card cards[count];

    for (size_t i = 0; i < count; ++i)
        get_hand_data(&buffer[i][0], &cards[i]);

    qsort(&cards, count, sizeof(Card), lowest_value_first);

    int sum = 0;
    for (size_t i = 0; i < count; ++i)
        sum += cards[i].bid * (i + 1);

    return sum;
}

void run(void) {
    FILE *input = fopen("input.txt", "r");
    if (!input) exit(-FILE_ERROR);
    printf("The hands sum is: %d\n", get_hands_sum(input));
    fclose(input);
}

int main(void) {
    run();
    return SUCCESS;
}
