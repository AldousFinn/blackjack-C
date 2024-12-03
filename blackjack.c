#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CARDS 52
#define MAX_HAND_SIZE 10

struct Card {
    char name[20];
    char suit[10];
    int value;
};

struct Card deck[MAX_CARDS];
struct Card player_hand[MAX_HAND_SIZE];
struct Card dealer_hand[MAX_HAND_SIZE];

int deck_index = 0;
int player_hand_size = 0;
int dealer_hand_size = 0;

void initialize_deck() {
    char *values[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};
    char *suits[] = {"H", "D", "C", "S"};
    
    for (int i = 0; i < MAX_CARDS; i++) {
        int value_idx = i % 13;
        int suit_idx = i / 13;
        snprintf(deck[i].name, sizeof(deck[i].name), "%s", values[value_idx]);
        snprintf(deck[i].suit, sizeof(deck[i].suit), "%s", suits[suit_idx]);
        if (value_idx == 0) {
            deck[i].value = 11; // Ace
        } else if (value_idx >= 10) {
            deck[i].value = 10; // Face cards (Jack, Queen, King)
        } else {
            deck[i].value = value_idx + 1; // Number cards (2-10)
        }
    }
}

void shuffle_deck() {
    srand(time(0)); 

    for (int i = MAX_CARDS - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        struct Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void print_hand(struct Card *hand, int hand_size) {
    for (int i = 0; i < hand_size; i++) {
        printf("%s %s", hand[i].name, hand[i].suit);
        if (i < hand_size - 1) {
            printf(", ");
        }
    }
    printf(" || ");
}

void deal_initial_cards() {
    player_hand[0] = deck[deck_index++];
    player_hand[1] = deck[deck_index++];
    dealer_hand[0] = deck[deck_index++];
    dealer_hand[1] = deck[deck_index++];
    player_hand_size = 2;
    dealer_hand_size = 2;
}

int calculate_hand_value(struct Card *hand, int hand_size) {
    int total = 0;
    int aces = 0;
    for (int i = 0; i < hand_size; i++) {
        total += hand[i].value;
        if (hand[i].value == 11) {
            aces++;
        }
    }
    while (total > 21 && aces > 0) {
        total -= 10;
        aces--;
    }
    return total;
}

void hit(struct Card *hand, int *hand_size) {
    hand[*hand_size] = deck[deck_index++];
    (*hand_size)++;
}

void game_start() {
    printf("\nNEW GAME\n");
    printf("---------------------------------------");

    initialize_deck();
    shuffle_deck();
    deal_initial_cards();

    int player_hand_value = calculate_hand_value(player_hand, player_hand_size);
    printf("\nYour Hand: ");
    print_hand(player_hand, player_hand_size);
    printf("Total: %d\n", player_hand_value);
    printf("---------------------------------------");

    printf("\nDealer's Hand: ");
    printf("%s %s [HIDDEN CARD]\n", dealer_hand[0].name, dealer_hand[0].suit);
    printf("---------------------------------------\n");

    char action;
    while (player_hand_value <= 21) {
        printf("\nWould you like to (H)it, (S)tand, or (Q)uit? ");
        scanf(" %c", &action);

        if (action == 'h' || action == 'H') {
            hit(player_hand, &player_hand_size);
            player_hand_value = calculate_hand_value(player_hand, player_hand_size);
            printf("---------------------------------------\n");
            printf("Your Hand: ");
            print_hand(player_hand, player_hand_size);
            printf("Total: %d\n", player_hand_value);
            printf("---------------------------------------\n");
        } else if (action == 'q' || action == 'Q') {
            exit(0);
        } else if (action == 's' || action == 'S') {
            break;
        } else {
            printf("Please enter either 'H' or 'S'.\n");
        }
    }

    int dealer_hand_value = calculate_hand_value(dealer_hand, dealer_hand_size);
    while (dealer_hand_value < 17) {
        hit(dealer_hand, &dealer_hand_size);
        dealer_hand_value = calculate_hand_value(dealer_hand, dealer_hand_size);
    }

    printf("\nDealer's Hand:\n");
    printf("---------------------------------------\n");
    print_hand(dealer_hand, dealer_hand_size);
    printf("Total: %d\n", dealer_hand_value);
    printf("---------------------------------------\n");

    if (player_hand_value > 21) {
        printf("You busted. The dealer won.\n");
    } else if (dealer_hand_value > 21) {
        printf("Dealer busts! You win.\n");
    } else if (player_hand_value > dealer_hand_value) {
        printf("You had the greater hand value! You win!\n");
    } else {
        printf("The dealer's hand value was greater than yours. You lose.\n");
    }
}

int main() {
    while (1) {
        game_start();
    }
    return 0;
}
