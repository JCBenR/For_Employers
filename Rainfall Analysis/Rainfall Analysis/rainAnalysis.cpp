//
//  rainAnalysis.cpp
//  Rainfall Analysis
//
//  Created by Justin Siebenhaar on 9/3/20.
//  Copyright © 2020 Justin Siebenhaar. All rights reserved.
//

#include "rainAnalysis.hpp"
#include <iostream>

using namespace std;

string Months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

void printData(vector<RainMonthData>& rain){
    for(RainMonthData i: rain){
           cout<<i.month<<","<<i.year<<","<<i.precip<<endl;
       }
}

string getTitle(){
    string city;    
    return city;
};

int dataPoints(vector<RainMonthData>& rain){
    int totalDataPoints = 0;
    for (int i=0; i<rain.size(); i++) {
        totalDataPoints++;
    }
    return totalDataPoints;
}

double overallAvg(vector<RainMonthData>& rain){
    int totalDP = dataPoints(rain);
    double totalRF = 0;
    for(RainMonthData i: rain){
        totalRF += i.precip;
    }
    double total = totalRF / totalDP;
    return total;
}

vector<RainMonthData> months(vector<RainMonthData>& rain, string month){
    vector<RainMonthData> forMonth;
    for(RainMonthData i: rain){
        if(i.month == month){
            forMonth.push_back(i);
        }
    }return forMonth;
}

vector<RainJustMonthData> monthAvg(vector<RainMonthData>& rain){
    vector<RainJustMonthData> justMonthData;
    for (int i=0; i<12; i++) {
        RainJustMonthData getTheMonthData;
    string thisMonth = Months[i];
    double totalRF = 0;
    int totalRecords = 0;
    double avgForMonth = 0;
        for(RainMonthData z : months(rain, thisMonth)){
            totalRF += z.precip;
            totalRecords++;
        } avgForMonth = totalRF / totalRecords;
        getTheMonthData.month = thisMonth;
        getTheMonthData.precip = avgForMonth;
        justMonthData.push_back(getTheMonthData);
    } return justMonthData;
}

void swap(RainMonthData& value1, RainMonthData& value2){
    RainMonthData temp = value1;
    value1 = value2;
    value2 = temp;
}

int smallestIndex(vector<RainMonthData>& rain, int startIdx){
    
    int minIdx = startIdx;
    
    for(int index = startIdx + 1; index < rain.size(); ++index){
        if (rain[index].precip < rain[minIdx].precip)
            minIdx = index;
    }
    
    return minIdx;
}

void selectionSortHand(vector<RainMonthData>& rain){
    int minIndex;
    for (int index = 0; index < rain.size(); ++index) {
        minIndex = smallestIndex(rain, index);
        if (rain[minIndex].precip != rain[index].precip) {
            swap(rain[minIndex], rain[index]);
        }
    }
}

void printPrecip(vector<RainMonthData>& rain){
    for(RainMonthData i: rain){
           cout<<i.precip<<" ";
       }
}

//not used
void printWettest(vector<RainMonthData>& rain){
    selectionSortHand(rain);
    vector<RainMonthData> wettest;
    vector<RainMonthData> dryest;
    for (int i =0; i<4; i++) {
        dryest.push_back(rain[i]);
    }
    for (int i = rain.size()-1; i>rain.size()-5; i--) {
        wettest.push_back(rain[i]);
    }
}

vector<double> dry(vector<RainMonthData>& rain){
    selectionSortHand(rain);
    vector<double> mostDry;
    for (int i =0; i<4; i++) {
        mostDry.push_back(rain[i].precip);
    } return mostDry;
}

vector<double> wet(vector<RainMonthData>& rain){
    selectionSortHand(rain);
    vector<double> mostWet;
    for (int i = rain.size()-1; i>rain.size()-5; i--) {
        mostWet.push_back(rain[i].precip);
    }
    return mostWet;
}

RainMonthData findMiddle(vector<RainMonthData>& rain){
    selectionSortHand(rain);
    float median;
    size_t size = rain.size();
    if (size % 2 == 0)
      {
          median = (float)size / 2;
      }
      else
      {
        median = (float)size/2;
      }
    return rain[median];
    }
