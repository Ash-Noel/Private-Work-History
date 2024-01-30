/*
	Created by Ash Noel
	Last Updated : 6/21/2022
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
 * Abstraction Function:
 *      Every medical device will be made of various materials and below is a template for the
 *      construction of one.
 *      A material will be identified by its material code. It has the values as defined by
 *      the control variables template. These are constants as defined by the file the information
 *      is pulled from
 *
 *Representation Invariant:
 *  All items != NULL
 *  All physical properties >= 0
 *  mat_code != any other mat_code from any other material.
 *  hysteresis_factor is a well-defined mathematical expression that does not throw a muParser exception.
 * */
class material
{
public:
	//class constructor
	material(string code, float l,  float min, 
	float max, float hl, float ht, float ho, float hf) {
		mat_code = code;
		length = l;
		min_before_h = min; // need this for before hysteresis value. (see variable_analysis.h)
		max_before_h = max; // need this for before hysteresis value.  ^ is this what was included to find max hysteresis?
		max_after_h = max_before_h; // changed- was initially 'hysteresis_factor * max_before_h'
		heat_level = hl;
		heat_time = ht;
		heat_on_delay = ho;
		heat_off_delay = hf;
	}

	//get functions for a material
	string get_mat_code() {return mat_code;}

	float get_length() {return length;}

	float get_min_bend_before() {return min_before_h;} // need this for before hysteresis value. (see variable_analysis.h)

	float get_max_bend_before() {return max_before_h;} // need this for before hysteresis value. (see variable_analysis.h)
	
	float get_max_bend_after(){return max_after_h;}

	float get_heat_level() { return heat_level; }

	float get_heat_time() { return heat_time; }

	float get_heat_on_delay() { return heat_on_delay; }

	float get_heat_off_delay() { return heat_off_delay; }


private:
	string mat_code;
	float length;
	float min_before_h; // move ?
	float max_before_h; // move ? 
    float max_after_h;
	float heat_level;
	float heat_time;
	float heat_on_delay;
	float heat_off_delay;
};

