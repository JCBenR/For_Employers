//
//  rainAnalysis.hpp
//  Rainfall Analysis
//
//  Created by Justin Siebenhaar on 9/3/20.
//  Copyright © 2020 Justin Siebenhaar. All rights reserved.
//

#ifndef rainAnalysis_hpp
#define rainAnalysis_hpp

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;
//struct declarations
struct RainMonthData { string month; int year; double precip;};
struct RainJustMonthData { string month; double precip;};

//vector declarations
vector<double> dry(vector<RainMonthData>& rain);
vector<double> wet(vector<RainMonthData>& rain);
vector<RainMonthData> months(vector<RainMonthData>& rain, string month);
vector<RainJustMonthData> monthAvg(vector<RainMonthData>& rain);

//FUNCTION DECLARATIONS
void printData(vector<RainMonthData>& rain);
int dataPoints(vector<RainMonthData>& rain);
double overallAvg(vector<RainMonthData>& rain);
void printPrecip(vector<RainMonthData>& rain);
void printWettest(vector<RainMonthData>& rain);

//sort functions
void swap(RainMonthData& value1, RainMonthData& value2);
int smallestIndex(vector<RainMonthData>& rain, int startIdx);
void selectionSortHand(vector<RainMonthData>& rain);

//median
RainMonthData findMiddle(vector<RainMonthData>& rain);
#endif /* rainAnalysis_hpp */
