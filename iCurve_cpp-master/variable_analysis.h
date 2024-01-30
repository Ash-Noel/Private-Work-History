/*
    Created by Ash Noel
    Last Updated : 6/21/2022
    iCurve C++ GCode Writing Program
    Property of Armour Technologies
*/

#ifndef SOURCE_CPP_VARIABLE_ANALYSIS_H
#define SOURCE_CPP_VARIABLE_ANALYSIS_H
#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <cmath>
#include <math.h>
#include <typeinfo>
#include "utility.h"
#include "material.h"
#include "mat_hys.h"
#include "device.h"
#include "machine.h"
#include "tool.h"
using namespace std;

/*
	@param points: the table of instructions with columns (index, theta, beta)
	@param threshold: the minimum angle to start rotating
	@modifies points
	@effects points: by removing values (replace with 0) for any angle that is under the minimum
    @returns points (by nature of pass by reference)
*/
//Takes a line from the csv data and splits it into a vector of strings seperated by that comma
std::vector<std::string> ssplit(std::string strToSplit, char delimeter){
	std::stringstream ss(strToSplit);
	std::string item;
	std::vector<std::string> splittedStrings;
	while (std::getline(ss, item, delimeter))
	{
		splittedStrings.push_back(item);
	}
	return splittedStrings;
}

//Takes a csv and loads the data into a 2D Table of strings
vector<vector<string>> ggather_file(string filename) {
	vector<string> lines;
	string line;
	vector<vector<string>> input;
	ifstream input_file(filename);
	if (!input_file.is_open()) {
		cerr << "Could not open the file - '"
			<< filename << "'" << endl;
	}
	while (getline(input_file, line)) {
		lines.push_back(line);
	}
	input_file.close();
	for (int i = 0; i < lines.size(); i++) {
		vector<string> temp;
		vector<string> xyz = ssplit(lines[i], ',');
		for (int j = 0; j < xyz.size(); j++) {
			temp.push_back(xyz[j]);
		}
		input.push_back(temp);
	}
	return input;
}

void threshold_pass(vector<vector<float>>& points, float threshold) {
    for (int i = 0; i < points.size(); i++) {
        for (int j = 1; j < points[i].size(); j++) {
            if (fabs(points[i][j]) < threshold) {
                points[i][j] = 0.0;
            }
        }
    }
}

/*
	@param points: the table of instructions with columns (index, theta, beta)
    @param dev: The device that provides the max angle before hysteresis and the min angle before hystersis
	@modifies points
	@effects points: As defined in step 3. Check if a theta is larger
					than the min acceptable. If not add to next row.
	@effects points: As defined in step 3. If an angle is larger than our maximum possible
					break it up into multiple rows of instructions with a remainder. And
					scale the index distances to the ratio of the threshold to the limit.
					Do the same for the bend angle.
    @returns points (by nature of pass by reference)
*/
void before_hysteresis(vector<vector<float>>& points, device& dev){
    int size = points.size();
    int i = 0;
    int j = 0;
    float distance_traversed = 0;
    material m = dev.get_mats()[j];
    while (i < size + 1) {

        distance_traversed += points[i][0];
        cout << "i = " << i << endl;

        if (distance_traversed >= m.get_length()) {

            distance_traversed = distance_traversed - m.get_length();
            
            j += 1;
            // cout << "\nj = " << j << "|| material vector ?? = " << m.size() << endl;
            cout << typeid(m).name() << endl;

            m = dev.get_mats()[j]; // < vector 
            cout << "test reach 1 " << endl;                
            cout << "\n DISTANCE TRAVERSED: " << distance_traversed << endl;
        }

        if (fabs(points[i][1])>=m.get_min_bend_before()) {
            cout << "Iteration STARTS HERE:" << endl;
            cout << "\n First case A check: \n" << endl;
            cout << fabs(points[i][1]) << " >= " << m.get_min_bend_before() << endl;

            cout << "\n Second case: " << fabs(points[i][1]) << " > " << m.get_max_bend_before() << endl;

            cout << "\n Third case: " << points[i][1] << " < 0" << endl; 
            if (fabs(points[i][1]) > m.get_max_bend_before()) {
                cout << "\n Second case A check" << endl;
                if (points[i][1] < 0) {
                    cout << "\n Third case A check" << endl;

                    float og_theta = points[i][1];
                    float og_dist = points[i][0];
                    float og_beta = points[i][2];

                    float new_theta = points[i][1] + m.get_max_bend_before();                        // updating theta to: theta + max bend angle before hysteresis
                    float new_beta = points[i][2] * fabs((m.get_max_bend_before() / og_theta));     // updates beta to: beta * abs( max bend angle before / old theta)
                    points[i][1] = m.get_max_bend_before()*-1;                                     // CHANGES theta to: -(max bend angle before hysteresis)
                    points[i][0] = fabs(og_dist * (m.get_max_bend_before() / og_theta));            // CHANGES distance to: abs(distance * (max bend angle before hys / old theta))
                    points[i][2] = new_beta;                                                     // CHANGES beta to updated version of beta
                    distance_traversed -= og_dist - points[i][0];                           // keeps track of device length so we know what material to switch to

                    vector<float> temp = { og_dist - (points[i][0]),new_theta,og_beta - (new_beta) }; // temp = {original distance - new distance, new theta, orig beta - new beta}
                    cout << "theta is < 0: " << new_theta << endl;
                    points.insert(points.begin() + i + 1, temp);    // inserts the updated version of a single point after the point its looking at ( see practice file )

                    size += 1;
                }

                else {
                    
                    cout << "\n third case B check" << endl;
                    float og_theta = points[i][1];
                    float og_dist = points[i][0];
                    float og_beta = points[i][2];

                    float new_theta = points[i][1] - m.get_max_bend_before();
                    float new_beta = points[i][2] * fabs((m.get_max_bend_before() / og_theta));
                    points[i][1] = m.get_max_bend_before();
                    points[i][0] = fabs(og_dist * (m.get_max_bend_before() / og_theta));
                    points[i][2] = new_beta;
                    distance_traversed -= og_dist - points[i][0];

                    vector<float> temp = { og_dist - (points[i][0]),new_theta,og_beta - (new_beta) };
                    points.insert(points.begin() + i + 1, temp);
                    cout << "theta !< 0: " << new_theta << endl;

                    size += 1;
                }
            }
        }
        else {

            cout << "\n First case B check" << endl;
            if (i == size) { // once we reach the end
                cout << "case 3: "<< i << ": THETA= " << points[i][1] << endl;
                return;
            }
            else {  // turns theta into 0
                cout << "theta turns into 0. Theta before= " << points[i][1] << endl;
                cout << "size = " << size << endl;
                cout << "i = " << i << endl;
                points[i + 1][1] = points[i][1] + points[i + 1][1];
                
                points[i][1] = 0;
                
                // cout << "\ni at the end == " + i << endl;
            }
            cout << "???" << endl;
        }
        i += 1;
        // cout << "\ni at the end == " + i << endl;
    }
    
}

/*
 * @param points: the table of instructions that will be used to write Gcode
 * @param dev: the device object itself which contains the physical information of the device
 * @modifies points
 * @effects points: by scaling the theta and beta angles by the hysteresis value gathered 
                    from the materials on the device.
 * @returns points (by nature of pass by reference)
 * */
mat_hys gathering_input_material_hys(machine mac, device d, tool t, material mat) {

	string mat_hys_name = "materials/H_" + mac.get_machine_code() + "_" + d.get_model() + "_" + t.get_tool() + "_" + mat.get_mat_code() + ".csv";	// figure out how to get material ID
	
	vector<vector<string>> hys_data = ggather_file(mat_hys_name);
	mat_hys h(hys_data);

	// cout << "Material hysteresis file: " << mat_hys_name << endl;                // check if the file matches the correct file 

	return h;
	
}

void apply_hysteresis(vector<vector<float>>& points,machine mac, device& dev, tool t) {
    int size = points.size();
    int i = 0;
    int j = 0;
    float distance_traversed = 0;
    material m = dev.get_mats()[j];
    mat_hys h1 = gathering_input_material_hys(mac,dev,t,m);
    while (i < size) {
        distance_traversed += points[i][0]; // advancement 
        // cout << "distance: " << distance_traversed << endl;
        vector<float> slopes = h1.find_slopes();
	    vector<float> intercepts = h1.find_intercepts(slopes);
        h1.apply_hysteresis(points[i],slopes,intercepts);   

        // cout << "theta: " << points[i][1] << endl;
        if (distance_traversed >= m.get_length()) {
            distance_traversed = distance_traversed - m.get_length();
            j += 1;
            m = dev.get_mats()[j];
            h1 = gathering_input_material_hys(mac,dev,t,m);     // changes depending on the material 
        }

        i += 1;
    }
}


//positive and 0 returns false
//negative returns true
bool polarity(float x){
    return (x*-1) > x;
}

/*
 * @param distance_traversed: When going row by row in the table of instructions, this value keeps tracks of the
 *                             distance crossed to determine where in the device we are.
 * @param push_limit: The maximum length a machine is able to index a device forward.
 * @param m: The current material being considered and providing the requisite constraints.
 * @param first: the first row of instructions to be considered for combinations
 * @param second: the subsequent row of instructions directly after the first.
 * @effects: If the polarity switches between rows, if the push limit or max bend angle defined by the material
 *            are exceeded, or if a combination would result in a crossing the length of the material, return false.
 *            Else return true.
 * @returns boolean that defines whether it has passed or not.
 * */
bool combine(float distance_traversed, float push_limit, material& m , vector<float>& first,vector<float>& second){
    if(polarity(first[1]) != polarity(second[1]) || polarity(first[2]) == polarity(second[2])){
        if(polarity(first[1]) != polarity(second[1])){
            if(first[1] == 0 || second[1] == 0){
            }
            else{
                return false;
            }
        }
        if(polarity(first[2]) != polarity(second[2])){
            if(first[2] == 0 || second[2] == 0){
            }
            else{
                return false;
            }
        }
    }
    if((first[0]+second[0] > push_limit)){
        if(fabs(first[0]+second[0]-push_limit)<0.001){
            //cout<<"here"<<endl;
        }
        else {
            //cout<< (distance_traversed+second[0] - m.get_length())<<endl;
            return false;
        }

    }
    if(fabs(first[1] +second[1]) > m.get_max_bend_before()){
        return false;
    }
    if (distance_traversed + second[0] > m.get_length()){
        if(fabs(distance_traversed +second[0] -m.get_length())<0.001){
            //cout<<"here"<<endl;
        }
        else {
            //cout<< (distance_traversed+second[0] - m.get_length())<<endl;
            return false;
        }
    }
    return true;
}
/*
	@param points: the table of instructions with columns (index, theta, beta)
	@param dev: the device object itself which contains the physical information of the device
    @param machine: the machine object that provides the physical information of the machine
	@modifies points
	@effects points: As described in step 5 this is the step which combines instructions unless certain
                    tests fails. Those tests are described in the above function. If it passes then
                    it combines the rows togethers.
    @returns points (by nature of pass by reference)
*/
void condense(vector<vector<float>>& points, device dev, machine mac1) {
    int size = points.size();

    int i = 0;

    while (i < size) {
        if (points[i][0] == 0.0 && points[i][1] == 0.0 && points[i][2] == 0.0) {
            points.erase(points.begin() + i);
            i -= 1;
            size -= 1;
        }
        i += 1;
    }

    i = 0;
    size = points.size();
    int j = 0;
    material m = dev.get_mats()[j];
    float distance_traversed = points[0][0];
    while (i < size){
        distance_traversed += points[i][0];

        if(distance_traversed>m.get_length()){
            distance_traversed-= m.get_length();
            j+=1;
            m = dev.get_mats()[j];
            i+=1;
            continue;
        }
        if(i== size-1){
            break;
        }
        if(combine(distance_traversed,mac1.get_max_advance(),m,points[i],points[i+1])){
            points[i+1][0] += points[i][0]; //adds index together
            points[i+1][1] += points[i][1]; //adds thetas together
            points[i+1][2] += points[i][2]; //adds betas together
            distance_traversed -= points[i][0];

            points.erase(points.begin() + i);
            i -= 1;
            size -= 1;
        }
        i+=1;
    }
}

/*
    @param points : The Table of Instructions
    @modifies points
    @effects points : If theta or beta is larger than 180 in any
                        rotation, finds the equivalent reference angle <180.
    @returns points (by nature of pass by reference)
*/
void flip_180(vector<vector<float>>& points){
    for(int i = 0; i < points.size(); i++){
        if(fabs(points[i][2]) > 180){
            if(points[i][2]<0){
                points[i][2] +=360;
            }
            else{
                points[i][2] -= 360;
            }
        }
    }
}

/*
	@param points: the table of instructions with columns (index, theta, beta)
	@param rot_thresh: the maximum rotation (3D) that a machine can do
	@modifies points
	@effects points: by adding new rows of instructions based on the thresholds and other machine specifics
    @returns points (by nature of pass by reference)
*/
void splitting(vector<vector<float>>& points, float rot_thresh) {
    int size = points.size();

    int i = 0;
    while (i < size) {
        float test = points[i][2];

        if (test != 0.000 && points[i][1] != 0.00) {
            points[i][2] = 0.00;
            vector<vector<float>>::iterator it = points.begin();
            vector<float> new_row = { 0.0,0.0,test };
            points.insert(it + i, new_row);
            i += 1;
            size += 1;
        }
        i += 1;
    }

    size = points.size();
    i = 0;
    while (i < size) {
        if (fabs(points[i][2]) - rot_thresh>0.001) {

            if (points[i][2] < 0) {
                vector<float> temp = { 0.00,0.00,points[i][2] + rot_thresh };
                points[i][2] = rot_thresh*-1;
                points.insert(points.begin() + i+1, temp);

                size += 1;
            }
            else {
                vector<float> temp = { 0.00,0.00,points[i][2] - rot_thresh };
                points[i][2] = rot_thresh;
                points.insert(points.begin() + i+1, temp);

                size += 1;
            }
        }
        i += 1;
    }
}

#endif //SOURCE_CPP_VARIABLE_ANALYSIS_H
