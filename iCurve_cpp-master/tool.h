/*
	Created by Ash Noel
	Last Updated : 6/26/2022
	iCurve C++ GCode Writing Program
	Property of Armour Technologies
*/
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <cmath>
#include <math.h>
using namespace std;

/*
 * Abstraction Function:
 *     
 *     The tool file currently stores information of the name of the tool. 
 * */

 class tool
{	
public:
    tool(vector<vector<string>>&input) {
        Tool_code = input[0][1];
    }

// get functions for tool
string get_tool() {return Tool_code;}

private:
    string Tool_code;
};