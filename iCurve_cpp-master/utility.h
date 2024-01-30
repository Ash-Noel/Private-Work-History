/*
	Created by Ash Noel
	Last Updated : 8/15/2022
	iCurve C++ GCode Writing Program
	Property of Armour Technologies
*/
#pragma once
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
#include <algorithm>
using namespace std;

void print_2d (vector<vector<float>> a2dvector) {
	for (int i = 0; i < a2dvector.size(); i++){
		for (int j = 0; j < a2dvector[i].size(); j++) {
			cout << setw(15) << a2dvector[i][j] << " , ";
		}
		cout << endl;
	}
}

void prnt(vector<float> avector) {
	for (int i = 0; i < avector.size(); i++){
		cout << setw(15) << avector[i];
	}
	cout << endl;
}



//Check if a rotation value is 0 and if so return False else return True
int check_rotate(float value) {
	return value != 0;
}

/*
* @param init_x/y/z: the coordinates in xyz plane of the first point
* @param final_x/y/z the coordinates in xyz plane of the second or final point
* @returns The output of the distance formula for two points defined by function arguments
*/
float calc_distance(float init_x, float final_x, float init_y, float final_y, float init_z, float final_z) {
	return sqrt(pow((final_x - init_x), 2) + pow((final_y - init_y), 2) + pow((final_z - init_z), 2));
}

float lin_alg_norm(vector<float> vector_a, vector<float> vector_b) {
    return sqrt(pow((vector_b[0] - vector_a[0]), 2) + pow((vector_b[1] - vector_a[1]), 2) + pow((vector_b[2] - vector_a[2]), 2));
}

vector<float> add_lin_alg(vector<float> vector_a, vector<float> vector_b) { // #make a point halfway between these points(c)
    vector<float> vector_c;
    for (int i = 0; i < vector_a.size(); i++) {
        vector_c.push_back((vector_a[i] + vector_b[i]) * .5);
    }
    return vector_c;
}

class xyz_point {
    public:
        vector<float> nextPoint;        // this is one of the results of the findNextPoint function
        int indx;                       // this is the index of the point that findNextPoint finds

		xyz_point(vector<float> c, int a) {
		nextPoint = c;
		indx = a;
	}

};

vector<float> copy_vector(xyz_point a) {		// hard copy what is in xyz_point vector a to vector b
	vector<float> b;
	for(int i=0; i < a.nextPoint.size(); i++) {
		b.push_back(a.nextPoint[i]);
	}
	return b;
}

//Need to implement the following two functions in c++. They are written in python right now. Do this before the return bends function in 
// the main() function. You want to feed the output of SpacedInterp to go into return_bends. There's comments marked by "#" that
// you can read to understand what it does line by line.

// findNextPoint(data, aindx, data[0], desiredLength, foundPoint) -> format used in spacedinterp

void findNextPoint(vector<vector<float>> &data, float &desiredLength, xyz_point& foundPoint) {
	//aindx = foundPoint.indx;				    			//aindx is the index number of the point a (same as foundPoint.indx)
	int bindx = foundPoint.indx + 1;        				//bindx is the index number of the point b
	vector<float> a = foundPoint.nextPoint;					//a is just the first point being used for comparison	{x1,y1,z1}
	vector<float> b = data[bindx];							//b is the second point being used for comparison		{x2,y2,z2}


	vector<float> base = copy_vector(foundPoint);

    while (true) {
        if (lin_alg_norm(base,a) < desiredLength && lin_alg_norm(base,b) < desiredLength) {		// are these two points within reasonable distance?
			// iterators (moves along the list of points )
			foundPoint.indx += 1;
            bindx += 1;

            if (bindx == data.size()) { // if bindx has reached the end of the list of points, a changes from base to the point before the last point on the list             
				// cout << "REACHED END" << endl;				// uncomment for error checking (this is if it doesnt reach the last point)
				foundPoint.indx = 0;						
				foundPoint.nextPoint = {};					 
				return;
		    }
			a = data[foundPoint.indx];				// a = data[aindx];
            b = data[bindx];     // b turns into the last point 

        }
        else if (   (lin_alg_norm(base,a) < desiredLength && lin_alg_norm(base,b) > desiredLength) || 
                    (lin_alg_norm(base,a) > desiredLength && lin_alg_norm(base,b) < desiredLength) ) {
					// cout << "Bisection starting\n\n" << endl;		//uncomment for error checking
                        while (true) {
                            vector<float> c = add_lin_alg(a,b);							// this is the next point
                            if (abs(lin_alg_norm(base,c) - desiredLength) < .0001) {					
								foundPoint.nextPoint = c;          							 //changes value  globally to store the next point
								// cout << "Next point is: ";					// uncomment this to see the next point
								// prnt(c);

                                return;			 //c,aindx; -> return the c point and the index of where this happened
                            }

                            if (lin_alg_norm(base,a) < desiredLength && lin_alg_norm(base,c) > desiredLength) {
                                b = c; // makes C the new B 
                            }
                            else if (lin_alg_norm(base,c) < desiredLength && lin_alg_norm(base,b) > desiredLength) {
                                a = c; // makes C the new A
                            }

                        }
                }
    }

}

std::vector<float> reverse(std::vector<float> list_a) { // function that reverses lists/vectors. list_a is the original vector
	std::vector<float> list_b;						// reversed list/vector

	for(int i = list_a.size() -1 ; i >= 0 ; i--) {
		list_b.push_back(list_a[i]);
	}

	list_a = list_b;
    return list_a;
    
}

std::vector<float> slice( vector<vector<float>>& twodvector) {	// function that slices 2 dimensional lists/vectors the way numpy does (i.e,; list[:,1])
	// cout << "data size = " << twodvector.size() << endl;
	vector<float> distances;
	for(int i = 0; i < twodvector.size(); i++ ) {
		// cout << twodvector[i][1] << endl;
		distances.push_back(twodvector[i][1]);
	}
	// cout << "split size = " << distances.size() << endl;


	return distances;

}

// IMPORTANT NOTE: vector<float>& nextPoint AND int& point_indx HAVE BEEN ADDED TO CHANGE THE VALUES GLOBALLY
// ADD THIS TO FUNCTIONS THAT USE findNextPoint 
vector<vector<float>> SpacedInterp(vector<vector<float>> data, float interpolation) { 
	xyz_point foundPoint (data[0], 0);	// parameters = (nextPoint, aindx) -> the nextPoint is some point {x,y,z}, aindx is its index. Both initialized as the base (first index of data) and
	
	vector<float> slice_data = slice(data); // data[:,1] <- 2d vector storing points (format { {x,y,z},{x1}{y1}{z1},...,{xn}{yn}{zn} } )
	float max = *max_element(slice_data.begin(), slice_data.end()); //max(data[:, 1])

	int cap = .03 * max;
    float constant = 0.1;
	float desiredLength = interpolation;
	vector<vector<float>> pointHistory; // 2d vector storing each point initialized with 'np.vstack((pointHistory, nextPoint)) from numpy

	int loop = data.size();
	for (int i = 0; i < loop; i++) {
		if (foundPoint.nextPoint.size() == 0) {
			break;
		}
		else {
			pointHistory.push_back(foundPoint.nextPoint);
			findNextPoint(data, desiredLength, foundPoint);	//nextPoint, aindx = findNextPoint(data, 0, data[0], desiredLength, angles);         

		}
	}

	// cout << "\n" << pointHistory.size() << endl;			// error checking -> check the size (should be 151 for D5cm half spline kmtest 1 ) 
	return pointHistory;

}




/*
	@param prev_x/y/z the first point's coordinates
	@param curr_x/y/z the current point's cordinates
	@param next_x/y/z the sequential point's coordinates
	@param flag: unused left over from old code can be removed.
	@returns angle: the 2D rotation angle in radians required to get to the next point
*/
float bend_angle(int flag, float prev_x, float prev_y, float prev_z, float curr_x, float curr_y, float curr_z, float next_x, float next_y, float next_z) {
	float a = calc_distance(curr_x, next_x, curr_y, next_y, curr_z, next_z);
	float b = calc_distance(prev_x, next_x, prev_y, next_y, prev_z, next_z);
	float c = calc_distance(curr_x, prev_x, curr_y, prev_y, curr_z, prev_z);
	float direction = (prev_x - curr_x) * (next_y - curr_y) - (prev_y - curr_y) * (next_x - curr_x);
	float angle = 0.0;
	if (direction > 0) {
		if ((pow(a, 2) + pow(c, 2) - pow(b, 2)) / (2.0 * a * c) > 1) {
			angle = M_PI - M_PI / 2;
		}
		else {
			angle = M_PI - acos((pow(a, 2) + pow(c, 2) - pow(b, 2)) / (2.0 * a * c));
		}
	}
	else {
		if ((pow(a, 2) + pow(c, 2) - pow(b, 2)) / (2.0 * a * c) > 1) {
			angle = -(M_PI - M_PI / 2);
		}
		else {
			angle = -(M_PI - acos((pow(a, 2) + pow(c, 2) - pow(b, 2)) / (2.0 * a * c)));
		}

	}

	if (isnan(angle)) {angle = 0;}

	if (angle == -0) {angle = 0;}

	return angle;
}

/*
* @param flag: the value to check in order to run the calculation.
* @param curr_x/y/z the coordinates of the point to determine the 3D rotation from
* @returns angle: the value of the rotation in radians needed to take the XY plane to be
*				 inline with the curr point
*/
float rotate_angle(int flag, float curr_x, float curr_y, float curr_z) {
	if (!flag) {
		return 0;
	}

	else {
		float angle = asin(curr_z / (sqrt(pow(curr_x, 2) + pow(curr_y, 2) + pow(curr_z, 2))));
		return angle;
	}

}

/*
	@param points: the list of points (x,y,z) interpolated by the desired index distance
	@return final: the preliminary table of instructions of (index distance, theta, beta)
					to be sent to later post-processing.
*/
vector<vector<float>> return_bends(vector<vector<float>>& points) {
	vector<vector<float>> data(points.begin() + 1, points.end());

	vector<int> rotate_flag(points.size(), 0);
	vector<float> r(points.size(), 0);
	vector<float> theta(points.size(), 0);
	vector<float> beta(points.size(), 0);

	for (int i = 0; i < data.size() - 1; i++) {

		if (i == 0) {
			theta[i] = 0;
			beta[i] = 0;
			rotate_flag[i] = check_rotate(data[i][2]);
			r[i] = calc_distance(data[i][0], data[i + 1][0], data[i][1], data[i + 1][1], data[i][2], data[i + 1][2]);
			continue;
		}
		float prev_x = data[i - 1][0];
		float prev_y = data[i - 1][1];
		float prev_z = data[i - 1][2];
		float curr_x = data[i][0];
		float curr_y = data[i][1];
		float curr_z = data[i][2];
		float next_x = data[i + 1][0];
		float next_y = data[i + 1][1];
		float next_z = data[i + 1][2];

		rotate_flag[i] = check_rotate(data[i][2]);
		r[i] = calc_distance(data[i][0], data[i + 1][0], data[i][1], data[i + 1][1], data[i][2], data[i + 1][2]);

		theta[i] = bend_angle(rotate_flag[i], prev_x, prev_y, prev_z, curr_x, curr_y, curr_z, next_x, next_y, next_z);
		beta[i] = rotate_angle(rotate_flag[i], curr_x, curr_y, curr_z);
	}
	vector<vector<float>> final;

	for (int i = 0; i < theta.size(); i++) {
		float temp_theta = theta[i] * 180 / M_PI;
		float temp_beta = beta[i] * 180 / M_PI;
		vector<float> temp{ r[i], temp_theta, temp_beta };
		final.push_back(temp);
	}
	return final;
}

// python uses D5cm_Half_Spline_KMTest1_Printed to test the files