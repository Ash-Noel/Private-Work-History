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
#include <math.h>
#include <cmath>
using namespace std;
/*
	Abstraction Function:
		A Machine is very similar to a Device. In that it has physical properties that
		are all packaged into a machine class to be stored in a variable.
		A Machine has all the physical properties shown in the private class. This is
		subject to change.
		The order of the Motor variables in that vector should match this basic definition. Subject to change.
		
		Index 0 = Bending Pin Rotation
		Index 1 = Advancement
		Index 2 = 3D Rotation
		Index 3 = Front Gripper
		Index 4 = Rear Gripper
		Index 5 = Bending Pin Up/Down


	Representation Invariant:
		All values != NULL
		All Physical properties >= 0
		All GCode specific properties do not break the bounds of the machine/controller
		(size of motors vector) motors.size() == # of motors on device
		
*/

class machine
{
public:
	//class constructor
	machine(vector<vector<string>>&input) {
		Machine_Model_Code = input[0][1];
		for (int i = 1; i < input[1].size(); i++) {
			input[1][i].erase(std::remove(input[1][i].begin(), input[1][i].end(), '\n'), input[1][i].end());
			motors.push_back(input[1][i]);
		}
		Bending_Pin_Microsteps = stof(input[2][1]);			// omitted from MMC- will be calculated
		Bending_Pin_Steps_Per_Mm = stof(input[3][1]);			// omitted from MMC- will be calculated
		Bending_Pin_Step_Angle = stof(input[4][1]);
		Bending_Pin_Home_Angle = input[5][1];				// TBD VALUE
		Bending_Pin_Load_Angle = stof(input[6][1]);
		Bending_Pin_Run_Angle = stof(input[7][1]);
		Bending_Pin_Reduction_Ratio = stof(input[8][1]);			
		Bending_Pin_Home_Position = stof(input[9][1]);
		Bending_Pin_End_Position = stof(input[10][1]);
		
		
		Front_Gripper_Microsteps = stof(input[11][1]);
		Front_Gripper_Steps_Per_Mm = stof(input[12][1]);
		Front_Gripper_Step_Angle = stof(input[13][1]);
		Front_Gripper_Home_Angle = stof(input[14][1]);
		Front_Gripper_Home_Gap = stof(input[15][1]);
		Front_Gripper_Load_Gap = stof(input[16][1]);
		Front_Gripper_Run_Gap = stof(input[17][1]);
		Front_Gripper_Cam_Eccentricity = stof(input[18][1]);
		Front_Gripper_Home_Position = stof(input[19][1]);
		Front_Gripper_End_Position = stof(input[20][1]);
		
		Rear_Gripper_Microsteps = stof(input[21][1]);
		Rear_Gripper_Steps_Per_Mm = stof(input[22][1]);
		Rear_Gripper_Step_Angle = stof(input[23][1]);			// omitted from MMC- will be calculated
		Rear_Gripper_Home_Angle = stof(input[24][1]);
		Rear_Gripper_Home_Gap = stof(input[25][1]);
		Rear_Gripper_Load_Gap = stof(input[26][1]);
		Rear_Gripper_Run_Gap = stof(input[27][1]);
		Rear_Gripper_Cam_Eccentricity = stof(input[28][1]);
		Rear_Gripper_Home_Position = stof(input[29][1]);
		Rear_Gripper_End_Position = stof(input[30][1]);

		Advancement_Microsteps = stof(input[31][1]);
		Advancement_Steps_Per_Mm = stof(input[32][1]);
		Advancement_Step_Angle = stof(input[33][1]);
		Advancement_Home_Angle = stof(input[34][1]);
		Advancement_Home_Gap = stof(input[35][1]);
		Advancement_Load_Gap = stof(input[36][1]);
		Advancement_Run_Gap	= stof(input[37][1]);
		Advancement_Cam_Eccentricity = stof(input[38][1]);
		Advancement_Home_Position = stof(input[39][1]);
		Advancement_End_Position = stof(input[40][1]);
		Max_advancement_distance = stof(input[41][1]);


		// add placeholder values to TBD values
		Home_3D_Rotation_Position = input[42][1]; 			// TBD  also changed names since c++ variables cannot start with a number
		Run_3D_Rotation_Position = input[43][1];			// TBD
		Reduction_3D_Rotation_Ratio	= input[44][1];			// TBD
		Max_3D_Rotation_Angle = stof(input[45][1]);				// TBD leaving as 15 for now since TBD will break it 
		
		Vertical_Microsteps = stof(input[46][1]);
		Vertical_Steps_Per_Mm = stof(input[47][1]);
		Vertical_Step_Angle = stof(input[48][1]);
		Vertical_Home_Angle = input[49][1]; // TBD VALUE
		Vertical_Load_Angle = stof(input[50][1]);
		Vertical_Run_Angle = stof(input[51][1]);
		Vertical_Reduction_Ratio = stof(input[52][1]);
		Vertical_Home_Position = input[53][1]; // TBD VALUE
		Vertical_End_Position = input[54][1]; // TBD VALUE
		Vertical_Home_Offset = stof(input[55][1]);
		Vertical_Drop_Angle = stof(input[56][1]);
		Bending_Pin_Home_Offset = stof(input[57][1]);
		
	}

	//get functions
	string get_machine_code() { return Machine_Model_Code;}
	vector<string> get_motors() { return motors; }

// bending pin getters 
	float get_pin_microsteps() { return Bending_Pin_Microsteps; }
	float get_pin_steps_per_mm() { return Bending_Pin_Steps_Per_Mm; }
	float get_pin_step_angle() { return Bending_Pin_Step_Angle; }
	string get_pin_home_angle() { return Bending_Pin_Home_Angle; }			// TBD VALUE
	float get_pin_load_angle() { return Bending_Pin_Load_Angle; }
	float get_pin_run_angle() { return Bending_Pin_Run_Angle; } // may have to change(?)
	float get_pin_reduction_ratio() { return Bending_Pin_Reduction_Ratio; }	
	float get_pin_home_position() { return Bending_Pin_Home_Position; }
	float get_pin_end_position() { return Bending_Pin_End_Position; }
	float get_pin_home_offset() {return Bending_Pin_Home_Offset; }
	float get_pin_load_position() {
		return (Bending_Pin_Reduction_Ratio*Bending_Pin_Load_Angle/((Bending_Pin_Steps_Per_Mm * Bending_Pin_Step_Angle)/Bending_Pin_Microsteps));
	}
	float get_pin_run_position() {
		return (Bending_Pin_Reduction_Ratio*Bending_Pin_Run_Angle/((Bending_Pin_Steps_Per_Mm * Bending_Pin_Step_Angle)/Bending_Pin_Microsteps));
	}
	float get_pin_target_position(float theta) { // * does this spit out pos values? may change 1/29
		return (Bending_Pin_Reduction_Ratio * (90-theta)/(Bending_Pin_Steps_Per_Mm * Bending_Pin_Step_Angle/Bending_Pin_Microsteps));
	}
	float get_pin_home_offset_position() {
		return (Bending_Pin_Reduction_Ratio*-Bending_Pin_Home_Offset/(Bending_Pin_Steps_Per_Mm*Bending_Pin_Step_Angle/Bending_Pin_Microsteps));
	}
// front gripper getters 
	float get_front_microsteps() { return Front_Gripper_Microsteps; }
	float get_front_steps_per_mm() { return Front_Gripper_Steps_Per_Mm; }
	float get_front_step_angle() { return Front_Gripper_Step_Angle; }
	float get_front_home_angle() { return Front_Gripper_Home_Angle; }
	float get_front_home_gap() { return Front_Gripper_Home_Gap; }
	float get_front_load_gap() { return Front_Gripper_Load_Gap; }
	float get_front_run_gap() { return Front_Gripper_Run_Gap; }
	float get_front_cam_eccen() { return Front_Gripper_Cam_Eccentricity; }
	float get_front_home_position() { return Front_Gripper_Home_Position; }
	float get_front_end_position() { return Front_Gripper_End_Position; }
	float get_front_load_position() {
		return (Front_Gripper_Home_Angle+180/M_PI*asin((Front_Gripper_Home_Gap-Front_Gripper_Load_Gap)/Front_Gripper_Cam_Eccentricity-sin(Front_Gripper_Home_Angle*M_PI/180)))/
		(Front_Gripper_Steps_Per_Mm*Front_Gripper_Step_Angle/Front_Gripper_Microsteps);
	}
	float get_front_run_position() { 
		return (Front_Gripper_Home_Angle+180/M_PI*asin((Front_Gripper_Home_Gap-Front_Gripper_Run_Gap)/Front_Gripper_Cam_Eccentricity-sin(Front_Gripper_Home_Angle*M_PI/180)))/
		(Front_Gripper_Steps_Per_Mm*Front_Gripper_Step_Angle/Front_Gripper_Microsteps);
	}
	float get_front_target_position() {
		return (Front_Gripper_Home_Angle+180/M_PI*asin((Front_Gripper_Home_Gap-0)/Front_Gripper_Cam_Eccentricity-sin(Front_Gripper_Home_Angle*M_PI/180)))/
		(Front_Gripper_Steps_Per_Mm*Front_Gripper_Step_Angle/Front_Gripper_Microsteps);
	}

// rear gripper getters
	float get_rear_microsteps() { return Rear_Gripper_Microsteps; }
	float get_rear_steps_per_mm() { return Rear_Gripper_Steps_Per_Mm; }
	float get_rear_step_angle() { return Rear_Gripper_Step_Angle; }
	float get_rear_home_angle(){ return Rear_Gripper_Home_Angle; }
	float get_rear_home_gap() { return Rear_Gripper_Home_Gap; }
	float get_rear_load_gap() { return Rear_Gripper_Load_Gap; }
	float get_rear_run_gap() { return Rear_Gripper_Run_Gap; }
	float get_rear_cam_eccen() { return Rear_Gripper_Cam_Eccentricity; }
	float get_rear_home_position() { return Rear_Gripper_Home_Position; }
	float get_rear_end_position() { return Rear_Gripper_End_Position; }
	float get_rear_load_position() {
		return (Rear_Gripper_Home_Angle+180/M_PI*asin((Rear_Gripper_Home_Gap-Rear_Gripper_Load_Gap)/Rear_Gripper_Cam_Eccentricity-sin(Rear_Gripper_Home_Angle*M_PI/180)))/
		(Rear_Gripper_Steps_Per_Mm*Rear_Gripper_Step_Angle/Rear_Gripper_Microsteps);
	}
	float get_rear_run_position() {
		return (Rear_Gripper_Home_Angle+180/M_PI*asin((Rear_Gripper_Home_Gap-Rear_Gripper_Run_Gap)/Rear_Gripper_Cam_Eccentricity-sin(Rear_Gripper_Home_Angle*M_PI/180)))/
		(Rear_Gripper_Steps_Per_Mm*Rear_Gripper_Step_Angle/Rear_Gripper_Microsteps);
	}
	float get_rear_target_position() { // 0.3 value needs variable -> will be taken from MMC
		return (Rear_Gripper_Home_Angle+180/M_PI*asin((Rear_Gripper_Home_Gap-0.01)/Rear_Gripper_Cam_Eccentricity-sin(Rear_Gripper_Home_Angle*M_PI/180)))/
		(Rear_Gripper_Steps_Per_Mm*Rear_Gripper_Step_Angle/Rear_Gripper_Microsteps);
	}

// advance getters 
	float get_advance_microsteps() { return Advancement_Microsteps; }
	float get_advance_steps_per_mm() { return Advancement_Steps_Per_Mm; }
	float get_advance_step_angle() { return Advancement_Step_Angle; }
	float get_advance_home_angle() { return Advancement_Home_Angle; }
	float get_advance_home_gap() { return Advancement_Home_Gap; }
	float get_advance_load_gap() { return Advancement_Load_Gap; }
	float get_advance_run_gap() { return Advancement_Run_Gap; }
	float get_advance_cam_eccen() { return Advancement_Cam_Eccentricity; }
	float get_advance_home_position() { return Advancement_Home_Position; }
	float get_advance_end_position() { return Advancement_End_Position; }
	float get_max_advance() { return Max_advancement_distance; }
	float get_advance_load_position() {
		return (Advancement_Home_Angle+180/M_PI*asin((Advancement_Home_Gap - Advancement_Load_Gap)/Advancement_Cam_Eccentricity-sin(Advancement_Home_Angle*M_PI/180)))/
	(Advancement_Steps_Per_Mm*Advancement_Step_Angle/Advancement_Microsteps);
	}
	float get_advance_run_position() {
		return (Advancement_Home_Angle+180/M_PI*asin((Advancement_Home_Gap-Advancement_Run_Gap)/Advancement_Cam_Eccentricity-sin(Advancement_Home_Angle*M_PI/180)))/
		(Advancement_Steps_Per_Mm*Advancement_Step_Angle/Advancement_Microsteps);
	}
	float get_advance_target_position(float distance){
		return (Advancement_Home_Angle+180/M_PI*asin((Advancement_Home_Gap-(Advancement_Home_Gap - distance))/Advancement_Cam_Eccentricity-sin(Advancement_Home_Angle*M_PI/180)))/
		(Advancement_Steps_Per_Mm*Advancement_Step_Angle/Advancement_Microsteps);
	}

	string get_3D_home_position() { return Home_3D_Rotation_Position; }		// TBD VALUE
	string get_3D_run_position() { return Run_3D_Rotation_Position; }		// TBD VALUE
	string get_3D_reduction_ratio() { return Reduction_3D_Rotation_Ratio; }	// TBD VALUE
	float get_max_3D() { return Max_3D_Rotation_Angle; }				// TBD VALUE. leaving as 15 for now since TBD will break it 

// vertical getters
	float get_vert_microsteps() {return Vertical_Microsteps; }
	float get_vert_steps_per_mm() {return Vertical_Steps_Per_Mm; }
	float get_vert_step_angle() {return Vertical_Step_Angle; }
	string get_vert_home_angle() {return Vertical_Home_Angle; } // TBD VALUE
	float get_vert_load_angle() {return Vertical_Load_Angle; }
	float get_vert_run_angle() {return Vertical_Run_Angle; }
	float get_vert_reduction() {return Vertical_Reduction_Ratio; }
	string get_vert_home_position() {return Vertical_Home_Position;} // TBD VALUE
	string get_vert_end_position() {return Vertical_End_Position; } // TBD VALUE
	float get_vert_home_offset() {return Vertical_Home_Offset; }
	float get_vert_drop_angle() {return Vertical_Drop_Angle; }

	float get_vertical_load_position() {
		return Vertical_Reduction_Ratio*Vertical_Load_Angle/(Vertical_Steps_Per_Mm*Vertical_Step_Angle/Vertical_Microsteps);
	}
	float get_vertical_run() {
		return Vertical_Reduction_Ratio*Vertical_Run_Angle/(Vertical_Steps_Per_Mm*Vertical_Step_Angle/Vertical_Microsteps); 
	}	
	float get_vertical_target_position(float theta) {
		return Vertical_Reduction_Ratio * theta/(Vertical_Steps_Per_Mm * Vertical_Step_Angle/Vertical_Microsteps);
	}
	float get_vertical_home_offset_position() {
		return Vertical_Reduction_Ratio*-Vertical_Home_Offset/(Vertical_Steps_Per_Mm*Vertical_Step_Angle/Vertical_Microsteps); 
	}
	float get_vertical_drop() {
		return Vertical_Reduction_Ratio*-Vertical_Drop_Angle/(Vertical_Steps_Per_Mm*Vertical_Step_Angle/Vertical_Microsteps); 
	}
private:
	string Machine_Model_Code;
	vector<string> motors;

	float Bending_Pin_Microsteps;
	float Bending_Pin_Steps_Per_Mm;
	float Bending_Pin_Step_Angle;
	string Bending_Pin_Home_Angle;				// TBD VALUE
	float Bending_Pin_Load_Angle;
	float Bending_Pin_Run_Angle;
	float Bending_Pin_Reduction_Ratio;			
	float Bending_Pin_Home_Position;
	float Bending_Pin_End_Position;
	float Bending_Pin_Home_Offset;

	float Front_Gripper_Microsteps;
	float Front_Gripper_Steps_Per_Mm;
	float Front_Gripper_Step_Angle;
	float Front_Gripper_Home_Angle;
	float Front_Gripper_Home_Gap;
	float Front_Gripper_Load_Gap;
	float Front_Gripper_Run_Gap;
	float Front_Gripper_Cam_Eccentricity;
	float Front_Gripper_Home_Position;
	float Front_Gripper_End_Position;

	float Rear_Gripper_Microsteps;
	float Rear_Gripper_Steps_Per_Mm;
	float Rear_Gripper_Step_Angle;
	float Rear_Gripper_Home_Angle;
	float Rear_Gripper_Home_Gap;
	float Rear_Gripper_Load_Gap;
	float Rear_Gripper_Run_Gap;
	float Rear_Gripper_Cam_Eccentricity;
	float Rear_Gripper_Home_Position;
	float Rear_Gripper_End_Position;
	
	float Advancement_Microsteps;
	float Advancement_Steps_Per_Mm;
	float Advancement_Step_Angle;
	float Advancement_Home_Angle;
	float Advancement_Home_Gap;
	float Advancement_Load_Gap;
	float Advancement_Run_Gap;
	float Advancement_Cam_Eccentricity;
	float Advancement_Home_Position;
	float Advancement_End_Position;

	float Max_advancement_distance;
	
	string Home_3D_Rotation_Position;			// TBD
	string Run_3D_Rotation_Position;				// TBD
	string Reduction_3D_Rotation_Ratio;			// TBD
	float Max_3D_Rotation_Angle;			// TBD - left as 15

	float Vertical_Microsteps;
	float Vertical_Steps_Per_Mm;
	float Vertical_Step_Angle;
	string Vertical_Home_Angle;	// TBD
	float Vertical_Load_Angle;
	float Vertical_Run_Angle;
	float Vertical_Reduction_Ratio;
	string Vertical_Home_Position;	// TBD
	string Vertical_End_Position; // TBD
	float Vertical_Home_Offset;
	float Vertical_Drop_Angle;
};

