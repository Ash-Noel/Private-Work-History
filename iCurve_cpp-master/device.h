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
#include <cmath>
#include <math.h>
#include "material.h"
using namespace std;

/*
 * Abstraction Function:
 *      A device is a representation of the thing being bent in the machine.
 *      A device has its own physical limitations and a vector (array) of materials that
 *      cover the relevant length of the device.
 *      The order of indexes in the materials array of the device object should correspond to the
 *      appearance of the material in the physical device.
 *      I.E: index 0: 1st material
 *           index 1: 2nd material
 *           etc.
 *
 * Representation Invariant:
 *      All objects != NULL
 *      The sum of all materials == the relevant length of the device
 *      All physical properties >= 0
 *      Device is made of at least 1 material.
 * */
class device
{	
public:
	//class constructor
	device(string model, int mats, float node, float curve_tol, vector<material> temp) {
		model_code = model;
		number_of_materials = mats;
		node_spacing = node;
		curve_tolerance = curve_tol;
		materials = temp;
		for (int i = 0; i < temp.size(); i++) {
			device_length += temp[i].get_length();
		}
	}

	//get functions for each value;
	string get_model() {return model_code;}

	int get_num_mats() {return number_of_materials;}

	float get_spacing() {return node_spacing;}

	float get_device_length() {return device_length;}

	float get_curve_tolerance() {return curve_tolerance;}

	vector<material> get_mats(){ return materials;}

private:
	string model_code;
	int number_of_materials;
	float node_spacing;
	float device_length;
	float curve_tolerance;
	vector<material> materials;
};
