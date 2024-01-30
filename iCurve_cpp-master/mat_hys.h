/*
	Created by Ash Noel
	Last Updated : 8/15/2022
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
using namespace std;

/*
	Note: This class is mainly being utilized in 'variable_analysis.h' in the apply_hysteresis function.
	The apply hysteresis function from this class and the apply_hysteresis function in 
	'variable_analysis.h' are different, they work together to apply hysteresis depending on the material.
	The 'Hystersis Check' file on the shared Google Drive folder should have a spreadsheet containing how the formula works. 


*/


class mat_hys
{
public:
	//class constructor
	mat_hys(vector<vector<string>>&input) {
		Hysteresis_code = input[0][1];
		for (int i = 1; i < input[1].size(); i++){
			Bend_angle_before_hys.push_back(stof(input[1][i]));
		}

		for (int i = 1; i < input[2].size(); i++){
			Bend_angle_after_hys.push_back(stof(input[2][i]));
		}
		
	}


	//get functions for a material's hysteresis
	string get_hys_code() {return Hysteresis_code;}
    vector<float> get_before_hys() {return Bend_angle_before_hys;}
    vector<float> get_after_hys() {return Bend_angle_after_hys;}

	vector<float> find_slopes() {         // param = (bend angles (before), bend angles (after), empty container for slopes)
		vector<float> b = Bend_angle_before_hys;
		vector<float> a = Bend_angle_after_hys;
		vector<float> slopes;
		for (int i=1; i < b.size()+1; i++) {
			if (i == b.size()) {					// if this is the value, dont subtrace anything 
				slopes.push_back( a[i-1]/b[i-1]);
			}
			else {
				slopes.push_back((a[i] - a[i-1]) / (b[i] - b[i-1]) );
			}
		}
		return slopes;
	}

	vector<float> find_intercepts(vector<float>& slopes) {
		vector<float> intercepts;
		vector<float> b = Bend_angle_before_hys;
		vector<float> a = Bend_angle_after_hys;

		for (int i=0; i < slopes.size(); i++) {
			float slope_x = (b[i] * slopes[i]);                
			float intercept = (a[i] - slope_x);

			if (round(intercept + 0.001111111) == 0) {			
				intercept = 0.0;
			}
			// cout << a[i] << " - " << slope_x << " = " << intercept << endl << endl;     //uncommment to view intercept calculations
			intercepts.push_back(intercept);
		}
		return intercepts;

	}

	void apply_hysteresis(vector<float>& p, vector<float> slopes, vector<float> intercepts) {      // we are rewriting ONE point (p) 
		// update: p will be a single point
		vector<float> b = Bend_angle_before_hys;

			p[1] = abs(p[1]);
			bool flag = true;
			int j = 0;

			while(flag) {
				if (p[1] > b[j] && j != b.size()-1) {               //  iterate if the theta value is larger than the bend angle its comparing
					j++;
				}
				else {

				// uncomment to view parameters
					if (j == 0) {											// if the theta value is not greater than the first bend point, use the first slope & intercept
						p[1] = intercepts[j] + p[1] * slopes[j];
						// cout << p[1] << " => " << b[j] << endl;
					}

					else {														// when theta is less in between two bend angles, use the slope/intercept of the lesser beng angle
						p[1] = intercepts[j-1] + p[1] * slopes[j-1];
						// cout << p[1] << " < " << b[j] << endl;
					}

					flag = false;
				}
			}   
			// cout << endl;

	}

private:
	string Hysteresis_code;
	vector<float> Bend_angle_after_hys;
	vector<float> Bend_angle_before_hys;

};