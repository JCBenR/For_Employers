//
//  main.cpp
//  Poker
//
//  Created by Justin Siebenhaar on 9/1/20.
//  Copyright © 2020 Justin Siebenhaar. All rights reserved.
//

#include <iostream>
#include <vector>
#include "cards.hpp"

using namespace std;



int main(int argc, const char * argv[]) {
    //TEST DECKS
    srand(time(NULL));
    Deck flush = {
        {{10, "ten", "Spades"}, {3, "three", "Spades"}, {4, "four", "Spades"}, {5, "five", "Spades"}, {6, "six", "Spades"}}
    };
    
    Deck straight = {
        {{7, "ten", "Spades"}, {3, "three", "Spades"}, {4, "four", "Spades"}, {5, "five", "Spades"}, {6, "six", "Spades"}}
    };
    
    Deck royalFlush = {
        {{10, "ten", "Spades"}, {11, "three", "Spades"}, {12, "four", "Spades"}, {13, "five", "Spades"}, {14, "six", "Spades"}}
    };
    
    Deck fullHouse = { {{11, "ten", "Spades"}, {11, "three", "Spades"}, {12, "four", "Spades"}, {12, "five", "Spades"}, {12, "six", "Spades"}}};
    //we create a deck called myDeck, of type Deck.
    Deck myDeck;
    //we pass mydeck into the get deck function
    getDeck(myDeck);
    int flushNum=0;
    int straightNum=0;
    int straightFlushNum=0;
    int royalFlushNum=0;
    int fullHouseNum=0;
    int badHandNum = 0;
    int iterations = 1;
    for (int i=0;i<iterations;i++)
    {
        shuffle(myDeck);
        Deck newHand = hand(myDeck);
        
        
        if (isRoyalFlush(newHand)) {
            royalFlushNum+=1;
        }
        else if (isStraightFlush(newHand)){
            straightFlushNum+=1;
        }
        else if (isFullHouse(newHand)){
            fullHouseNum+=1;
        }
        else if (isFlush(newHand)) {
            flushNum+=1;
        }
        else if (isStraight(newHand)){
            straightNum+=1;
            //printDeck(newHand);
            //std::cout<<endl;
        }
        else
        {
            badHandNum+=1;
        }
        
//        cout<<"smallest index: "<<smallestIndex(newHand, 0)<<endl;
        selectionSortHand(newHand);
        printDeck(newHand);
        cout<<isStraight2(newHand)<<endl;
        cout<<isStraight(newHand)<<endl;
//        cout<<isStraight(straight)<<endl;
//        cout<<isStraight2(straight)<<endl;
//        printDeck(straight);
    }
    
    
    std::cout<< "Royal Flushes: " << royalFlushNum << "/" << iterations << endl << "Straight Flushes: " << straightFlushNum << "/" << iterations << endl << "Full Houses: " << fullHouseNum << "/" << iterations << endl << "Flushes: " << flushNum << "/" << iterations << endl << "Straights: " << straightNum << "/" << iterations << endl << "Bad Hands: " << badHandNum << "/" << iterations << endl;
    
    
    
    
    

    return 0;
}


