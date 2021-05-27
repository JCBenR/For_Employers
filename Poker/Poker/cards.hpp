//
//  cards.hpp
//  Poker
//
//  Created by Justin Siebenhaar on 9/1/20.
//  Copyright © 2020 Justin Siebenhaar. All rights reserved.
//

#ifndef cards_hpp
#define cards_hpp

#include <stdio.h>
#include <iostream>
#include <vector>


//each card has a rank, suit, and these two values for max nums
struct Card {
    int rankNum;
    std::string rank;
    std::string suit;
    int num_suits = 4;
    int num_ranks = 13;
};

//each deck is made up of a vector of cards, each item being the type of Card. it also has a max size of 52.
struct Deck {
    std::vector<Card> cards;
    int maxSize = 52;
};

//declaring our functions using references.
void getDeck(Deck&);
void printDeck(Deck&);
void shuffle(Deck& deck);
Deck hand(Deck& deck);
void printHand(Deck& deck);
bool isFlush(Deck& deck);
int findMin(Deck& deck);
bool Contains(Deck& deck, int lookFor);
//bool compareCards (Card a, Card b);
bool isStraight(Deck& deck);
bool isStraight2(Deck& deck);
bool isStraightFlush(Deck& deck);
bool isRoyalFlush(Deck& deck);
bool isFullHouse(Deck& deck);

//HELPER FUNCTIONS
void swap(Card& value1, Card& value2);
int smallestIndex(Deck& deck, int startIdx);
void selectionSortHand(Deck& deck);
#endif /* cards_hpp */
