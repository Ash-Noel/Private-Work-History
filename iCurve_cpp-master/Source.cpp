/*
	Created by Ash Noel
	Last Updated : 8/17/2022
	iCurve C++ GCode Writing Program
	Property of Armour Technologies
*/
#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <time.h>
#include "utility.h"
#include "material.h"
#include "mat_hys.h"
#include "device.h"
#include "variable_analysis.h"
#include "write_gcode.h"
#include "machine.h"
#include "tool.h"
using namespace std;


//Takes a line from the csv data and splits it into a vector of strings seperated by that comma
std::vector<std::string> split(std::string strToSplit, char delimeter){
	std::stringstream ss(strToSplit);
	std::string item;
	std::vector<std::string> splittedStrings;
	while (std::getline(ss, item, delimeter))
	{
		splittedStrings.push_back(item);
	}
	return splittedStrings;
}

//Output each element of the points array to a csv file 
void write_csv(vector<vector<float>>& points, string filename) {
	ofstream myfile;
	myfile.open(filename);
	for (int i = 0; i < points.size(); i++) {
		for (int j = 0; j < points[i].size(); j++) {
			myfile << to_string(points[i][j]) << ',';
		}
		myfile << '\n';
	}
	myfile.close();
}

//Output a vector of strings to a file.
void write(vector<string> &lines, string filename) {
	ofstream myfile;
	myfile.open(filename, std::ofstream::trunc);
	for (int i = 0; i < lines.size(); i++) {
		myfile << lines[i];
	}
	myfile.close();
}

//same as write_csv but just for cout
void console_out(vector<vector<float>>& points) {
	for (int i = 0; i < points.size(); i++) {
		for (int j = 0; j < points[i].size(); j++) {
			cout << to_string(points[i][j]) << ',';
		}
		cout << '\n';
	}
}

//Takes a csv and loads the data into a 2D Table of strings
vector<vector<string>> gather_file(string filename) {
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
		vector<string> xyz = split(lines[i], ',');
		for (int j = 0; j < xyz.size(); j++) {
			temp.push_back(xyz[j]);
		}
		input.push_back(temp);
	}
	return input;
}

//Runs cin requests and creates a device object
//device files should have what materials they are made up of and those
//are pulled from the materials folder.
device gathering_input_device() {
	string device_name;

	cout << "Please enter the device that is being bent: " << endl;
	cin >> device_name;

	device_name = "devices/" + device_name + ".csv";
	vector<vector<string>> device_data = gather_file(device_name);
	vector<string> mat_info;
	
	for (int i = 0; i < device_data.size(); i++) {
		if (device_data[i][0] == "number_of_materials") {
			mat_info = device_data[i];
			break;
		}
	}
	vector<material>temp_list_mats;
	for (int i = 2; i < mat_info.size(); i+=2) {
		vector<vector<string>> temp_data;
		string mat_name = "materials/" + mat_info[i] + ".csv";
		temp_data = gather_file(mat_name);
		material m(temp_data[0][1],stof(mat_info[i+1]), stof(temp_data[1][1]), 
		stof(temp_data[2][1]), stof(temp_data[3][1]), stof(temp_data[4][1]),
		stof(temp_data[5][1]), stof(temp_data[6][1]));
		temp_list_mats.push_back(m);

/* uncomment to view the parameters of the material class 
		cout << temp_data[0][1] << endl; //  code num
		cout << mat_info[i+1] << endl; 	 //  mat length
		cout << temp_data[1][1] << endl; //  min bend
		
		cout << temp_data[2][1] << endl; // max bend
		cout << temp_data[3][1] << endl; // heat level
		cout << temp_data[4][1] << endl; // heat time

		cout << temp_data[5][1] << endl; // heat delay
		cout << temp_data[6][1] << endl; // heat off delay
*/


// uncomment to view the material file being read
		// cout << endl;
		// for (int i = 0; i < temp_data.size(); i++) {
		// 	for (int j = 0; j < temp_data[i].size(); j++) {
		// 		cout << temp_data[i][j] << " ";
		// 	}
		// 	cout << endl;
		// }
	
	}
	device d1(device_data[0][1], stoi(device_data[1][1]), stof(device_data[2][1]),stof(device_data[3][1]), temp_list_mats);

/* 	uncomment to view the device class parameters 
	cout << "DEVICE DATA: " << endl;
	cout << device_data[0][1] << endl; // AbCd
	cout << device_data[1][1] << endl; // num of materials
	cout << device_data[2][1] << endl; // node spacing
	cout << device_data[3][1] << endl; // curve tol
*/
	return d1;
}

tool gathering_input_tool() {
	string tool_name;
	cout << "Please enter the tool being used: " << endl;
	cin >> tool_name;

	tool_name = "tool/" + tool_name + ".csv";
	vector<vector<string>> tool_data = gather_file(tool_name);
	tool t1(tool_data);
	
	cout << "The device's assigned tool is " + t1.get_tool() + ". Please ensure that this tool is attached." <<endl;
	return t1;
}

//Creates a machine object by loading in the data
//requested by cin and reading from that file.
machine gathering_input_machine() {
	string machine_name;								// @ machine_name = a file being read that stores the machine information (data gets transfered to machine.h)

	cout << "Please enter the machine that is being used: " << endl;
	cin >> machine_name;

	machine_name = "machines/" + machine_name + ".csv";			// @ searches through a folder with different machine names 

	vector<vector<string>> machine_data = gather_file(machine_name);

	machine m(machine_data);

	return m;
}

mat_hys gathering_input_material_hys(machine mac, device d, tool t) {
	material mat = d.get_mats()[0];		// this is the first material that appears in the device

	string mat_hys_name = "materials/H_" + mac.get_machine_code() + "_" + d.get_model() + "_" + t.get_tool() + "_" + mat.get_mat_code() + ".csv";	// figure out how to get material ID
	
	vector<vector<string>> hys_data = gather_file(mat_hys_name);
	mat_hys h(hys_data);

	// vector<float> g = h.get_before_hys(); 		// test
	// cout << "Material hysteresis file: " << mat_hys_name << endl;

	return h;
	
}

//The main Program. Runs everything.
int main(int argc, char* argv[]) {

	/*Parsing (Will not be permanent)*/
	string filename(argv[1]);
	vector<string> lines;
	string line;
	vector<vector<float>> points;

	ifstream input_file(filename);
	if (!input_file.is_open()) {
		cerr << "Could not open the file - '"
			<< filename << "'" << endl;
		return EXIT_FAILURE;
	}

	while (getline(input_file, line)) {
		lines.push_back(line);
	}
	input_file.close();

	for (int i = 0; i < lines.size(); i++) {
		vector<float> temp;
		vector<string> xyz = split(lines[i], ',');
		for (int j = 0; j < xyz.size(); j++) {
			temp.push_back(stof(xyz[j]));
		}
		points.push_back(temp);
	}
	/*End of parsing*/

	//Scopic attaches everything below this line (and the #includes above)
	//to their code in order to have a direct pipeline from user curve
	//to written GCode.

	//This is where the medical person would indicate what device they are bending
	//And what machine they are operating on.
	
	device d1 = gathering_input_device();									// @ FUNCTION ABOVE -> takes all the information from a device found in the device																	// folder ( the user inputs this when the program is being ran). also see 'material.h'
	tool t1 = gathering_input_tool();
	machine mac1 = gathering_input_machine();								// @ FUNCTION ABOVE -> does the same as gathering_input_device, but for the machine. see 'machine.h'
	mat_hys h1 = gathering_input_material_hys(mac1, d1, t1);			// Will this depend on the output of: the machine, device, and material ID? Yes
	// cout << "\nInput: " << endl;
	// console_out(points);

	// vector<float> slopes = h1.find_slopes();
	// vector<float> intercepts = h1.find_intercepts(slopes);

	
	// points = SpacedInterp(points, 1.000); 							// @ interpolates each point
	// cout << "\nPOINT SIZE: " << points.size() << endl;			// @ uncomment to see the amount of points that exist & all the points
	// cout << "\n Interpolated: " << endl;
	// print_2d(points);

	//takes in raw curve points and returns a basic Table of Instructions
	points = return_bends(points);											// @ FUNCTION IN 'utility.h'. 
	// cout << "\n return_bends= takes in raw curve points and returns a basic Table of Instructions: " << endl;
	// console_out(points);													// @ uncomment to see the initial points

	//(Step 2)Removes Theta and Beta values that are below a threshold
	threshold_pass(points, d1.get_curve_tolerance()); 						// @ FUNCTION IN 'device.h' -> information from a given file
    // cout << "\n threshold_pass= Removes Theta and Beta values that are below a threshold: " << endl;
	// console_out(points);


	//(Step 3)Adds Theta values together row by row until above a minimum. 
	//If theta is too large splits them into multiple rows of instructions

	cout << "\n Before Hysteresis check (Breaks at this function)" << endl; 
	before_hysteresis(points, d1); 											// @ FUNCTION IN 'variable_analysis.h'
	cout << "\n before hysteresis= Adds Theta values together row by row until above a minimum. " << endl;
	console_out(points);

	//(Step 5) Based on a given set of rules defined in the combine function
	//Combines rows by adding values together to try to lower amount of instructions
	condense(points, d1, mac1);												// @ FUNCTION IN 'variable_analysis.h'
    cout << "\n condense= Based on a given set of rules defined in the combine function " << endl;
	// console_out(points);

	//Simple function that find equivalent negative or positive angles should and angle be >180

	// cout << "\n Flip error test" << endl;
	flip_180(points);														// @ FUNCTION IN 'variable_analysis.h'
    cout << "\n flip 180= find equivalent negative or positive angles should and angle be >180" << endl;
	// console_out(points);

	//(Step 4) Changes theta values by the hysteris function of a material.
	apply_hysteresis(points, mac1,d1,t1);										// @ FUNCTION IN 'variable_analysis.h' *updated
	cout << "\n apply hysteresis: " << endl;
	// console_out(points);

	
	//(Step 5) If beta is too large split the instructions to multiple iterations of the maximum bend possible.
	splitting(points, mac1.get_max_3D());									// @ FUNCTION IN 'variable_analysis.h'
	cout << "\n splitting= If beta is too large split the instructions to multiple iterations of the maximum bend possible " << endl;
	// console_out(points);

	//Uncomment the line below to see the Table of Instructions after all the post processing
	// console_out(points);

	/// uncomment vvvvvvvvvvv ********
	vector<string> gcode = return_gcode(points, d1, mac1);					// @ FUNCTION IN 'write_gcode.h'
	
	write(gcode, "gcode_output.txt");

    
    //TODO: review duet3D notation. Update write_gcode.h file.

	/* 
	Writeup:
		Any updates to the last write up with regards of code
		Homing of the duet3D point to the webpage and any information
		Further elaborating on the code

	Mechanical Engineer:
		Brief yet descriptive overview of the code and files

		Suggestions for future potential roadblocks
	*/
        
	return EXIT_SUCCESS;
}