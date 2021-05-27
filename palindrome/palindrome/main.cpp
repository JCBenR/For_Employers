//
//  main.cpp
//  palindrome
//
//  Created by Justin Siebenhaar on 8/27/20.
//  Copyright © 2020 Justin Siebenhaar. All rights reserved.
//

#include <iostream>
#include <string>

using namespace std;

int main(int argc, const char * argv[]) {
    string wordToCheck;
    string backwards;
    cout<<"enter a word"<<endl;
    cin>>wordToCheck;
    for (int i=wordToCheck.size()-1; i<wordToCheck.size(); i--) {
        backwards += wordToCheck[i];
    }
    if (wordToCheck == backwards) {
        cout<<"it's a palindrome"<<endl;
    } else {
        cout<<"it's not a palindrome"<<endl;
    }
    cout<<backwards<<endl;
    
    return 0;
}
