//
//  main.cpp
//  Rainfall Analysis
//
//  Created by Justin Siebenhaar on 9/3/20.
//  Copyright © 2020 Justin Siebenhaar. All rights reserved.
//

#include "rainAnalysis.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

string city;

vector<RainMonthData> raindrops;

int main(int argc, const char * argv[]) {

string Months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    
    ifstream rain("macon.txt");
    
    string city;
    string month;
    int year;
    double precip;
    
    string line;
    
    getline(rain, line);
    city = line;
    cout<<"the city is: "<<line<<endl;
    
        RainMonthData temp;
        while (rain>>month>>year>>precip) {
            temp.month = month;
            temp.year = year;
            temp.precip = precip;
            raindrops.push_back(temp);
    }
    rain.close();

    // open a file in write mode.
    ofstream RainJDS;
    RainJDS.open("jdsrain.txt");

    // WRITE DATA TO FILE
    
    //overall average
    RainJDS << "The overall average rainfall amount is "<<overallAvg(raindrops) <<" inches." << endl;
    
    //every month average
    for(RainJustMonthData i : monthAvg(raindrops)){
        RainJDS << "The average rainfall amount for "<<i.month<<" is "<<i.precip<<" inches."<<endl;
    };
    
    //four wettest months
    RainJDS << "The rain amounts (in inches) of the four wettest months are: ";
    for(double i: wet(raindrops)){
        RainJDS<<i;};
    RainJDS<<endl;
    
    //four driest months
    RainJDS << "The rain amounts (in inches) of the four driest months are: ";
    for(double i: dry(raindrops)){
        RainJDS<<i;};
    RainJDS<<endl;
    
    //median
    RainJDS << "The median months is: "<<findMiddle(raindrops).month<<" "<<findMiddle(raindrops).year<<" "<<findMiddle(raindrops).precip<<"."<<endl;

    // close the opened file.
    RainJDS.close();
    
    cout<<"The overall average rainfall amount is "<<overallAvg(raindrops) <<" inches."<<endl;

    //TEST FUNCTIONS
//    monthAvg(raindrops);
//    selectionSortHand(raindrops);
//    printData(raindrops);
//    printWettest(raindrops);
//    findMiddle(raindrops);
    return 0;
}


    
    
    
    

