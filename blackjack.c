#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define num_cards 52
#define upper_numer_of_cards 10


char *card_names[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};
char *card_suits[] = {"H", "D", "C", "S"};
int card_values[num_cards];


int deck[num_cards];
int player_hand[upper_numer_of_cards];
int dealer_hand[upper_numer_of_cards];


int deck_index = 0;
int numer_player_cards = 0;
int numer_dealer_cards = 0;



void create_deck() {
    int deck_position;
    for (deck_position = 0; deck_position < num_cards; deck_position++) {
        int value_at_deck_position = deck_position % 13;
        int suit_at_deck_position = deck_position / 13;

        
        if (value_at_deck_position == 0) {
            card_values[deck_position] = 11;
        }    
        else if (value_at_deck_position >= 10) {
            card_values[deck_position] = 10;
        }    
        else {
            card_values[deck_position] = value_at_deck_position + 1;
        }
        
        deck[deck_position] = deck_position;
    }
}



void shuffle_deck() {
    srand(time(0)); 
    int old_card_position;
    int new_card_position;
    int temporary_position;
    
    for (old_card_position = num_cards - 1; old_card_position > 0; old_card_position--) {
        new_card_position = rand() % (old_card_position + 1);
        temporary_position = deck[old_card_position];
        deck[old_card_position] = deck[new_card_position];
        deck[new_card_position] = temporary_position;

    }
}



void print_hand(int *hand, int numer_of_cards) {
    int item;
    for (item = 0; item < numer_of_cards; item++) {
        int card_pointer = hand[item];
        printf("%s %s", card_names[card_pointer % 13], card_suits[card_pointer / 13]);
        if (item < numer_of_cards - 1) {
            printf(", ");
            
        }
    }
    printf(" || ");
}




void deal_start_cards() {
    player_hand[0] = deck[deck_index++];
    player_hand[1] = deck[deck_index++];
    dealer_hand[0] = deck[deck_index++];
    dealer_hand[1] = deck[deck_index++];
    numer_player_cards = 2;
    numer_dealer_cards = 2;
}



int calculate_hand_value(int *hand, int numer_of_cards) {
    int total_hand_value = 0;
    int aces = 0;
    int card_item;

    
    for (card_item = 0; card_item < numer_of_cards; card_item++) {
        int card_pointer = hand[card_item];
        total_hand_value += hand[card_item];
        if (card_values[card_pointer] == 11) {
            aces++;
        }
    }

    
    while (total_hand_value > 21 && aces > 0) {
        total_hand_value -= 10;
        aces--;
    }

    
    return total_hand_value;
}



int new_calculate_hand_value(int *hand, int numer_of_cards) {
    int total_hand_value = 0;
    int aces = 0;
    int card_item;

    
    for (card_item = 0; card_item < numer_of_cards - 1; card_item++) {
        int card_pointer = hand[card_item];
        total_hand_value += hand[card_item];
        if (hand[card_pointer] == 11) {
            aces++;
        }
    }

    
    while (total_hand_value > 21 && aces > 0) {
        total_hand_value -= 10;
        aces--;
    }

    
    return total_hand_value;
}



void hit(int *hand, int *numer_of_cards) {
    hand[*numer_of_cards] = deck[deck_index++];
    (*numer_of_cards)++;
}




void game_start() {
    printf("\n\n\n\nNEW GAME\n");
    printf("---------------------------------------");

    create_deck();
    shuffle_deck();
    deal_start_cards();

    int player_hand_value = calculate_hand_value(player_hand, numer_player_cards);
    printf("\nYour Hand: ");
    print_hand(player_hand, numer_player_cards);
    printf("Total: %d\n", player_hand_value);
    printf("---------------------------------------");

    int new_dealer_hand_value = new_calculate_hand_value(dealer_hand, numer_dealer_cards);
    printf("\nDealer's Hand: ");
    print_hand(player_hand, numer_dealer_cards);
    printf("Total: %d\n", new_dealer_hand_value);
    printf("---------------------------------------\n");


    
    char player_responce;
    while (player_hand_value <= 21) {
        printf("\nWould you like to (H)it, (S)tand, or (Q)uit? ");
        scanf(" %c", &player_responce);

        if (player_responce == 'h' || player_responce == 'H') {
            hit(player_hand, &numer_player_cards);
            player_hand_value = calculate_hand_value(player_hand, numer_player_cards);
            printf("---------------------------------------\n");
            printf("Your Hand: ");
            print_hand(player_hand, numer_player_cards);
            printf("Total: %d\n", player_hand_value);
            printf("---------------------------------------\n");
        } 
            
        else if (player_responce == 'q' || player_responce == 'Q') {
            exit(0);
        } 
            
        else if (player_responce == 's' || player_responce == 'S') {
            break;
        } 
            
        else {
            printf("Please enter either 'H' or 'S'.\n");
        }
        
    }



    
    int dealer_hand_value = calculate_hand_value(dealer_hand, numer_dealer_cards);
    while (dealer_hand_value < 17) {
        hit(dealer_hand, &numer_dealer_cards);
        dealer_hand_value = calculate_hand_value(dealer_hand, numer_dealer_cards);
    }


    
    printf("\nDealer's Hand:\n");
    printf("---------------------------------------\n");
    print_hand(dealer_hand, numer_dealer_cards);
    printf("Total: %d\n", dealer_hand_value);
    printf("---------------------------------------\n");


    
    if (player_hand_value > 21) {
        printf("You busted. The dealer won.\n");
        system("./blackjack.c");
    }
        
    else if (dealer_hand_value > 21) {
        printf("Dealer busts! You win.\n");
        system("./blackjack.c");
    } 
        
    else if (player_hand_value > dealer_hand_value) {
        printf("You had the greater hand value! You win!\n");
        system("./blackjack.c");
    } 
        
    else {
        printf("The dealer's hand value was greater than (or equal to) your own. You lose.\n");
        system("./blackjack.c");
    }
    
}




int main() {
    while (1) {
        game_start();
    }
    return 0;
}
