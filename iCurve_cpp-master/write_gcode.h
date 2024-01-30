/*
	Created by Ash Noel
	Last Updated : 8/15/2022
	iCurve C++ GCode Writing Program
	Property of Armour Technologies

	back motor will stay in place- will just open close, front motor will odo the opposite
*/
#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <math.h>
#include <map>
#include "utility.h"
#include "material.h"
#include "device.h"
#include "variable_analysis.h"
#include "machine.h"
using namespace std;

/*@param row : A row from the Table of Instructions
* @returns x : A string in binary to catch the different 3 digit binary cases we
*				expect to see from the Table of Instructions
*/
string binary_code(vector<float>& row) {
	string x = "";
	for (int i = 0; i < row.size(); i++) {
		if (fabs(row[i]) > 0) {
			x += "1";
		}
		else {
			x += "0";
		}
	}
	return x;
}

												// vvvv ADD CONSTANTS TO TRIG FUNCTIONS vvvv //

//NOTE: Need conversion rate for theta
//NOTE: Might need to do boundary checking (stall guard) for tinyg/duet3D but unsure if that's only for G1 and other linear commands.
//NOTE: Positive Bend is clockwise
//NOTE: The Gcode is proprietary to the specific controller. Needs to be edited in the future
//NOTE: Assumption is made that rotary axis needs G0 rather than G1.

/*@param table : The Table of Instructions where rows are organized as (Advance/Index value, Theta, Beta)
* @param d1 : The Device that is being bent, and it's properties
* @param mac1 : The Machine that will be operating the GCode
* @returns output : A vector of strings that is the GCode that can be output to a file
*/
vector<string> return_gcode(vector<vector<float>>& table, device d1, machine mac1) {
	//side is true for postive bend (left side) and false for negative bend (right side). 0 does not change the bend.
	
	//IMPORTANT: CHANGE TO TRUE FOR ACTUAL MACHINE OPERATION.
	bool side = true;
	
	float distance_traversed = 0;
	int j = 0;
	material m = d1.get_mats()[j];

	//values that need to be considered when writing the gcode.
	//Edit between float and string as needed.
	string bend_motor = mac1.get_motors()[0];			// x
	string front_grip_motor = mac1.get_motors()[1]; 	// y
	string rear_grip_motor = mac1.get_motors()[2];		// z
	string rot3d_motor = mac1.get_motors()[3];			// u **  IN NEW MACHINE
	string bend_vertical_motor = mac1.get_motors()[4];  // v **  IN NEW MACHINE
	string advance_motor = mac1.get_motors()[5];		// w
	advance_motor = advance_motor[0];	// get rid of newline 

	
// bending pin
	string Bending_Pin_Microsteps = to_string(mac1.get_pin_microsteps());
	string Bending_Pin_Steps_Per_Mm = to_string(mac1.get_pin_steps_per_mm());
	string Bending_Pin_Step_Angle = to_string(mac1.get_pin_step_angle());
	string Bending_Pin_Home_Angle = mac1.get_pin_home_angle();  // TBD
	string Bending_Pin_Load_Angle = to_string(mac1.get_pin_load_angle());
	string Bending_Pin_Run_Angle = to_string(mac1.get_pin_run_angle());
	string Bending_Pin_Reduction_Ratio = to_string(mac1.get_pin_reduction_ratio());
	string Bending_Pin_Home_Position = to_string(mac1.get_pin_home_position());
	string Bending_Pin_End_Position = to_string(mac1.get_pin_end_position());
	string Bending_Pin_Home_Offset = to_string(mac1.get_pin_home_offset());

	string Bending_Pin_Load_Position = to_string(mac1.get_pin_load_position()); // Comes from a formula
	string Bending_Pin_Run_Position = to_string(mac1.get_pin_run_position()); 	// Comes from a formula					
	string Bending_Pin_Home_Offset_Position = to_string(mac1.get_pin_home_offset_position());	// Comes from a formula	
	// string Bending_Pin_Target_Position = to_string(mac1.get_pin_target_position()); // takes in theta as a parameter, so its moved in the for loop

// front gripper: 
	string Front_Gripper_Microsteps = to_string(mac1.get_front_microsteps());
	string Front_Gripper_Steps_Per_Mm = to_string(mac1.get_front_steps_per_mm());
	string Front_Gripper_Step_Angle = to_string(mac1.get_front_step_angle());
	string Front_Gripper_Home_Angle = to_string(mac1.get_front_home_angle());  
	string Front_Gripper_Home_Gap = to_string(mac1.get_front_home_gap());
	string Front_Gripper_Load_Gap = to_string(mac1.get_front_load_gap());
	string Front_Gripper_Run_Gap = to_string(mac1.get_front_run_gap()); 
	string Front_Gripper_Cam_Eccentricity = to_string(mac1.get_front_cam_eccen());
	string Front_Gripper_Home_Position = to_string(mac1.get_front_home_position());
	string Front_Gripper_End_Position = to_string(mac1.get_front_end_position());

	string Front_Gripper_Load_Position = to_string(mac1.get_front_load_position()); // Comes from a formula	
	string Front_Gripper_Run_Position  = to_string(mac1.get_front_run_position()); // Comes from a formula	
	string Front_Gripper_Target_Position = to_string(mac1.get_front_target_position()); // Comes from a formula	

// rear gripper:
	string Rear_Gripper_Microsteps = to_string(mac1.get_rear_microsteps());
	string Rear_Gripper_Steps_Per_Mm = to_string(mac1.get_rear_steps_per_mm());
	string Rear_Gripper_Step_Angle = to_string(mac1.get_rear_step_angle());
	string Rear_Gripper_Home_Angle = to_string(mac1.get_rear_home_angle());  
	string Rear_Gripper_Home_Gap = to_string(mac1.get_rear_home_gap());
	string Rear_Gripper_Load_Gap = to_string(mac1.get_rear_load_gap());
	string Rear_Gripper_Run_Gap = to_string(mac1.get_rear_run_gap()); 
	string Rear_Gripper_Cam_Eccentricity = to_string(mac1.get_rear_cam_eccen());
	string Rear_Gripper_Home_Position = to_string(mac1.get_rear_home_position());
	string Rear_Gripper_End_Position = to_string(mac1.get_rear_end_position());
	string Rear_Gripper_Load_Position = to_string(mac1.get_rear_load_position()); // Comes from a formula	
	string Rear_Gripper_Run_Position = to_string(mac1.get_rear_run_position()); // Comes from a formula	
	string Rear_Gripper_Target_Position = to_string(mac1.get_rear_target_position()); // Comes from a formula- see comment on machine.h

// advance:
	string Advancement_Microsteps = to_string(mac1.get_advance_microsteps());
	string Advancement_Steps_Per_Mm = to_string(mac1.get_advance_steps_per_mm());
	string Advancement_Step_Angle = to_string(mac1.get_advance_step_angle());
	string Advancement_Home_Angle = to_string(mac1.get_advance_home_angle());  
	string Advancement_Home_Gap = to_string(mac1.get_advance_home_gap());
	string Advancement_Load_Gap = to_string(mac1.get_advance_load_gap());
	string Advancement_Run_Gap = to_string(mac1.get_advance_run_gap()); 
	string Advancement_Cam_Eccentricity = to_string(mac1.get_advance_cam_eccen());
	string Advancement_Home_Position = to_string(mac1.get_advance_home_position());
	string Advancement_End_Position = to_string(mac1.get_advance_end_position());	
	string Max_advancement_distance = to_string(mac1.get_max_advance());

	string Advancement_Load_Position = to_string(mac1.get_advance_load_position()); // Comes from a formula	
	string Advancement_Run_Position = to_string(mac1.get_advance_run_position());	// Comes from a formula	
	// string Advancement_Target_Position = to_string(mac1.get_advance_target_position());	// takes in distance as a parameter, so its moved in the for loop

// 3d values: ** Delete entirely or? It looks like the 3d values aren't applied anyway
	string Home_3D_Rotation_Position = mac1.get_3D_home_position(); // TBD
	string Run_3D_Rotation_Position = mac1.get_3D_run_position(); // TBD
	string Reduction_3D_Rotation_Ratio = mac1.get_3D_reduction_ratio(); //TBD
	string Max_3D_Rotation_Angle = to_string(mac1.get_max_3D()); // TBD- changed value to 15 on MMC.csv so code wont break
	
	string Load_3D_Rotation_Position = "3";		// new value, will be given- NOT ON MMC.CSV
	string Target_3D_Rotation_Position = "3";		// new value, will be given- NOT ON MMC.CSV
	string Heat = "6";							// new value, will be given- NOT ON MMC.CSV

	string Vertical_Microsteps = to_string(mac1.get_vert_microsteps());
	string Vertical_Steps_Per_Mm = to_string(mac1.get_vert_steps_per_mm());
	string Vertical_Step_Angle = to_string(mac1.get_vert_step_angle());
	string Vertical_Home_Angle = mac1.get_vert_home_angle(); 				// TBD
	string Vertical_Load_Angle = to_string(mac1.get_vert_load_angle());
	string Vertical_Run_Angle = to_string(mac1.get_vert_run_angle());
	string Vertical_Reduction_Ratio = to_string(mac1.get_vert_reduction());
	string Vertical_Home_Position = mac1.get_vert_home_position();				// TBD
	string Vertical_End_Position = mac1.get_vert_end_position();				// TBD
	string Vertical_Home_Offset = to_string(mac1.get_vert_home_offset()); 
	string Vertical_Drop_Angle = to_string(mac1.get_vert_drop_angle());

	string Vertical_Load_Position = to_string(mac1.get_vertical_load_position());
	string Vertical_Run_Position = to_string(mac1.get_vertical_run()); 		
	string Vertical_Home_Offset_Position = to_string(mac1.get_vertical_home_offset_position());

	// HEATING ELEMENT

	map<string, int> BitMap;

	BitMap["000"] = 0;
	BitMap["001"] = 10;
	BitMap["010"] = 25;
	BitMap["011"] = 40;
	BitMap["100"] = 55;
	BitMap["101"] = 70;
	BitMap["110"] = 85;
	BitMap["111"] = 100;

	// ** HOMING COMMENTED OUT

cout << "MOTOR ?:" + front_grip_motor << endl;

vector<string> output = {
					// "G28 " + front_grip_motor 					// G28 Y
					//  + "\nG28 " + rear_grip_motor 					// G28 Z
					//  + "\nG28 " + advance_motor						// G28 W
					//  + "\nG28 " + rot3d_motor						// G28 U Doesnt exist on current device
					//  + "\nG28 " + bend_vertical_motor				// G28 V Doesnt exist on current device
					//  + "\nG28 " + bend_motor						// G28 X
					//  + "\nG01 " + bend_motor + "-" + Bending_Pin_Home_Offset_Position + " " + bend_vertical_motor + "-" + Vertical_Home_Offset_Position		// G01 X(-Bending_Pin_Home_Offset_Position) V(-Vertical_Home_Offset_Position)
					//  + "\nG92 " + bend_motor + "0 " + bend_vertical_motor + "0"		// G92 X0 V0

// vvvvv Uncomment below when these motors are available vvvvvv
// G01 Y(Front_Gripper_Load_Position) Z(Rear_Gripper_Load_Position) W(Advancement_Load_Position) U(3D_Rotate_Load_Position) X(Bending_Pin_Load_Position) V(Vertical_Load_Position) 
					 "M106 P1 S250\nM106 P2 S250\nM106 P3 S250\nG01 " 		 
					 + front_grip_motor + Front_Gripper_Load_Position + " " + rear_grip_motor + Rear_Gripper_Load_Position + " " 
					 + advance_motor + Advancement_Load_Position + " " 
					 + rot3d_motor + Load_3D_Rotation_Position + " " // ** returned 3D VALUES 1/16
					 + bend_motor + Bending_Pin_Load_Position + " " // + bend_vertical_motor + Vertical_Home_Offset_Position ** ASK KEVIN IF WE SHOULD UNCOMMENT THIS LOADING POSITION
					 + "\nG01 " + bend_motor + Bending_Pin_Load_Position + " " + front_grip_motor + Front_Gripper_Load_Position
					 + " " + rear_grip_motor + Rear_Gripper_Load_Position + " " + advance_motor + Advancement_Load_Position 

					 + "\nG4 P15000" // DWELL in miliseconds **


// G01 Y(Front_Gripper_Target_Position) Z(Rear_Gripper_Target_Position) W(Advancement_Run_Position) U(3D_Rotate_Run_Position) X(Bending_Pin_Run_Position) V(Vertical_Run_Position)
					 + "\nG01 " + front_grip_motor + Front_Gripper_Target_Position + " " + rear_grip_motor + Rear_Gripper_Target_Position + " " 
					 + advance_motor + Advancement_Run_Position + " " 
					 + rot3d_motor + Run_3D_Rotation_Position + " " // ** COMMENTED OUT 3D VALUES
					 + bend_motor + Bending_Pin_Run_Position //+ " " + bend_vertical_motor + Vertical_Home_Offset_Position  ** ASK KEVIN IF WE SHOULD UNCOMMENT THIS RUN POSITION
					 + "\nG01 " + bend_motor + Bending_Pin_Run_Position + " " + front_grip_motor + Front_Gripper_Target_Position + " " 
					 + rear_grip_motor + Rear_Gripper_Target_Position + " " + advance_motor + Advancement_Run_Position + "\nM106 P0 S250 <<- Initial heat on\n<< END OF LOAD POSITIONS - DWELL - RUN POSITIONS\n"};

// uncomment below after figuring the rest 
// cout << "\n\nRear gripper target position: 5 = "<< mac1.get_rear_target_position() << endl;


	for (int i = 0; i < table.size(); i++) {
		
		distance_traversed += table[i][0];
		if (distance_traversed >= m.get_length()) {
			distance_traversed = distance_traversed - m.get_length();
			j += 1;
			m = d1.get_mats()[j]; // MATERIAL CHANGES HERE!!
		}
		
		float heat = m.get_heat_level();
		string bit_heat = "";

		for (const auto& pair : BitMap) {
            if (pair.second == heat) {
                // cout << pair.first << endl; // ** comment out to see if the bits are printing properly
				int i = 0;

				while (i < pair.first.size()) {
					if (pair.first[i] == '0') {
						bit_heat += "M106 P" + to_string(i+1) + " S0\n";
					}
					else {
						bit_heat += "M106 P" + to_string(i+1) + " S250\n";
					}
					i += 1;
				}
				// cout << bit_heat << endl; // should print out the turning on of the heating bits in gcode
            }
		}



	

		float distance = table[i][0];	
		// distance = mac1.get_advance_home_position() + (90 - asin(1 - (distance / mac1.get_advance_cam_eccen())));	// do we change this?
		

		string index = to_string(distance);

		// reduction ratio, 3d home position and 3d reduction have NO VALUES YET. Code will not run without these 
		
		// 		 vvvvvvvvv UNCOMMENT WHEN THE TBD VALUES HAVE BEEN DETERMINED VVVVVVVVVVVV
		// string theta = to_string((mac1.get_pin_reduction_ratio() * table[i][1] + mac1.get_pin_home_position()));	// home variable wont exist- will change once editing MMC	
		// string beta = to_string(mac1.get_3D_home_position()+ table[i][2] *mac1.get_3D_reduction_ratio());
		
		string theta = to_string(table[i][1]);
		string beta = to_string(table[i][2]);

		string Bending_Pin_Target_Position = to_string(mac1.get_pin_target_position(table[i][1]));
		string Advancement_Target_Position = to_string(mac1.get_advance_target_position(distance));
		string Vertical_Target_Position = to_string(mac1.get_vertical_target_position(table[i][1]));

		/*
		string a_target = to_string(mac1.get_front_home() + 90 -asin(1-TBD_Front_Gripper/mac1.get_front_cam_eccentricity()));
		string b_target = to_string(mac1.get_rear_home() + 90 - asin(1-TBD_Rear_Gripper/mac1.get_rear_cam_eccentricity()));
		string c_target = to_string(mac1.get_bending_vertical_home() + 90-arcsin(1-TBD_Bending_Pin_UpDown/Bending_Pin_UpDown_Cam_Eccentricity));
		*/

		string code = binary_code(table[i]);
		
		//Use the line below when trying to create a file that will run on a device. Comment the other one out.
		string step = "";


		
		if (code == "101") { // this is for 3d curve, but the arrow was removed?
			step = step + "G01 " + rear_grip_motor + Rear_Gripper_Run_Position + "\n" 			// (D) Front release -- Rear release
						+ "G01 " + advance_motor + Advancement_Target_Position + "\n"				// (E) Rear advance  -- Front advance
						
						+ "G01 " + rot3d_motor + Target_3D_Rotation_Position + "\n" // ** returned 1/16	 // 3d rotate <15
						+ "G01 " + front_grip_motor + Front_Gripper_Target_Position + "\n"
						+ "G01 " + rear_grip_motor + Rear_Gripper_Run_Position + "\n"
						+ "G01 " + rot3d_motor + Run_3D_Rotation_Position + "\n" // ** returned 1/16
						+ "G01 " + rear_grip_motor + Rear_Gripper_Target_Position + "\n"
						
						+ "G01 " + front_grip_motor + Front_Gripper_Run_Position			// Front release
						+ "G01 " + advance_motor + Advancement_Run_Position				// Front withdraw
						+ "G01 " + front_grip_motor + Front_Gripper_Target_Position;		// Front grip
		}
		// ** 3D CURVE COMMENTED OUT-- returned 1/16
		else if (code == "001") {
			step = step + "G01 " + front_grip_motor + Front_Gripper_Run_Position + "\n" // Front release 
						+ "G01 " + rot3d_motor + Target_3D_Rotation_Position + "\n" // ** returned 1/16 				// 3d rotate <15
						+ "G01 " + front_grip_motor + Front_Gripper_Target_Position + "\n" // Front grip
						+ "G01 " + rear_grip_motor + Rear_Gripper_Run_Position + "\n" // 
						+ "G01 " + rot3d_motor + Run_3D_Rotation_Position + "\n" // ** returned 1/16 					// 3d rotate run
						+ "G01 " + rear_grip_motor + Rear_Gripper_Target_Position + "\n"; // Rear grip

		} 
		else if (code == "110") {

			if (table[i][1] < 0) {		// if on the left side, ( negative bend )
				if (side == false) { // if on the left side,
					step = step + "G01 " + rear_grip_motor + Rear_Gripper_Run_Position + "| REAR\n" 			// (D) Front release --> Rear release
								+ "G01 " + advance_motor + Advancement_Target_Position + "| ADVANCE\n"				// (E) Rear advance  --> Front advance
								+ "G01 " + rear_grip_motor + Rear_Gripper_Target_Position + "| REAR\n";			// (G) Front grip --> Rear grip
								
								// VVVVVV HEATING VVVVV							

				}			
				else { // if not, switch to the correct side (switch from the right to left)
					step = step + "G01 " + rear_grip_motor + Rear_Gripper_Run_Position + "| REAR\n" 			// (D) Front release --> Rear release
								+ "G01 " + advance_motor + Advancement_Target_Position + "| ADVANCE\n"				// (E) Rear advance  --> Front advance
								+ "G01 " + rear_grip_motor + Rear_Gripper_Target_Position + "| REAR\n"			// (G) Front grip --> Rear grip
				
								// ** returned 1/16 dropping pin
								+ "G01 " + bend_vertical_motor + to_string(mac1.get_vertical_drop() + mac1.get_vertical_run()) + "V(Vertical_Drop_Target_Position) -> pin drop \n" // G01 V(Vertical_Drop_Target_Position) -> pin drop 
								// + "G01 " + bend_motor + "-" + Bending_Pin_Run_Position  + " " + bend_vertical_motor + to_string(mac1.get_vertical_drop() - mac1.get_vertical_run()) + "\n" //**
								+ "G01 *" + bend_motor + "-" + Bending_Pin_Run_Position + "G01 X(Bending_Pin_Run_Position) -> pin rotate CW\n"// pin rotate counter clockwise 	- -- - 	ASK KEVIN ABOUT COMMENTING OUT VERTICAL AGAIN. How does this rotate counter clockwise
								+ "G01 " + bend_vertical_motor + " -" + Vertical_Target_Position + "G01 V(Vertical_Run_Target_Position) -> pin rise\n"; // G01 V(Vertical_Run_Target_Position) we dont have this variable. is it the run or target position? -> pin rise

								// VVVVVV HEATING VVVVV		** Will the process be the same as last time? i.e., turn on the heating, then turn it off after the bend?						

				}
			}
			else {	// if on the right side ( positive bend )
				if (side == true) { // if on the right side,
					step = step + "G01 " + rear_grip_motor + Rear_Gripper_Run_Position + "| REAR\n" 			// (D) Front release --> Rear release
								+ "G01 " + advance_motor + Advancement_Target_Position + "| ADVANCE\n"				// (E) Rear advance  --> Front advance
								+ "G01 " + rear_grip_motor + Rear_Gripper_Target_Position + "| REAR\n";		// (G) Front grip --> Rear grip
							
								// VVVVVV HEATING VVVVV		** Will the process be the same as last time? i.e., turn on the heating, then turn it off after the bend?						
	
				}
				else { // if not, switch (switch to right side by moving COUNTER CLOCKWISE)
					step = step + "G01 " + rear_grip_motor + Rear_Gripper_Run_Position + "| REAR\n" 			// (D) Front release --> Rear release
								+ "G01 " + advance_motor + Advancement_Target_Position + "| ADVANCE\n"				// (E) Rear advance  --> Front advance
								+ "G01 " + rear_grip_motor + Rear_Gripper_Target_Position + "| REAR\n"			// (G) Front grip --> Rear grip
								
								// ** REMOVE DROPPING THE PIN SECTION 
								+ "G01 " + bend_vertical_motor + to_string(mac1.get_vertical_drop() - mac1.get_vertical_run()) + "V(Vertical_Drop_Target_Position) -> pin drop \n" // Do I change to this variable? theres a function for "vertical drop position" G01 V(Vertical_Drop_Target_Position) -> pin drop. 
								// + "G01 " + bend_motor + Bending_Pin_Run_Position  + " " + bend_vertical_motor + to_string(mac1.get_vertical_drop() + mac1.get_vertical_run()) // **
								+ "G01 **" + bend_motor + Bending_Pin_Run_Position + "G01 X(Bending_Pin_Run_Position) -> pin rotate CW\n" // ASK KEVIN WHY THIS IS THE SAME AS THE CLOCK WISE ROTATION WHEN THIS SHOULD BE COUNTER CLOCKWISE
								+ "G01 " + bend_vertical_motor + Vertical_Target_Position + "G01 V(Vertical_Run_Target_Position) -> pin rise\n"; 
								
								// VVVVVV HEATING VVVVV		** Will the process be the same as last time? i.e., turn on the heating, then turn it off after the bend?						
				
				}
			}
					// moved down here since it remains the same for each case in 110. No longer in each embedded if/else statement past turning on the heat
					// heating vvv
					step = step + bit_heat + "G4 P5000\n" // Heat on + DWELL 
						// + "G01 " + bend_motor + Bending_Pin_Target_Position  +  " " + bend_vertical_motor + Vertical_Target_Position // ** returned 1/16 vertical only	 // ASK KEVIN ABOUT REMOVING VERTICAL MOTOR AGAIN
						+ "G01 ***" + bend_motor + Bending_Pin_Target_Position  // (T) Bend Pin Rotate
						// + "G01 " + bend_motor + Bending_Pin_Run_Position  + " " + bend_vertical_motor + Vertical_Run_Position // returned vert only 1/16
						+ "\nG01 ****" + bend_motor + Bending_Pin_Run_Position  // (V) Bend pin rotate home
						// heat off shows here
						+ "\nG01 " + front_grip_motor + Front_Gripper_Run_Position + "| FRONT\n" 				// Rear release --> Front release
						+ "G01 " + advance_motor + Advancement_Run_Position + "| WITHDRAW\n"					// Rear withdraw --> Front withdraw
						+ "G01 " + front_grip_motor + "-" + Front_Gripper_Target_Position + "| FRONT\n"; 		// Rear grip --> Front grip
												
		}
		else if (code == "100") {
			step = step + "G01 " + rear_grip_motor + Rear_Gripper_Run_Position + "| REAR\n" 			// (D) Front release --> Rear release
						+ "G01 " + advance_motor + Advancement_Target_Position + "| ADVANCE\n"				// (E) Rear advance  --> Front advance
						+ "G01 " + rear_grip_motor + Rear_Gripper_Target_Position + "| REAR\n"			// (G) Front grip --> Rear grip
						
						+ "G01 " + front_grip_motor + Front_Gripper_Run_Position + "| FRONT\n" 				// Rear release --> Front release
						+ "G01 " + advance_motor + Advancement_Run_Position + "| WITHDRAW\n"					// Rear withdraw --> Front withdraw
						+ "G01 " + front_grip_motor + "-" + Front_Gripper_Target_Position + "| FRONT\n"; 		// Rear grip --> Front grip
		}
		output.push_back(step);

	}
	
	// output.push_back("G1 " + advance_motor + advancement_home + " " + front_grip_motor + front_home + " " + rear_grip_motor + rear_home + " " + 
	// bend_motor + bend_home +" " + rot3d_motor + rot_3d_home + " " + bend_vertical_motor + /*bend_vertical_home+*/"\n");
	output.push_back("G01 " + front_grip_motor + Front_Gripper_Load_Position + " " + rear_grip_motor + Rear_Gripper_Load_Position 
					+ "\nM106 P1 S0\nM106 P2 S0\nM106 P3 S0\nM106 P0 S0");	// load position is nan
	// ** added turn off heat
	return output;
}
// Binary = 0 0 1
// M106 P1 S0 -> bit 1 = 0
// M106 P2 S0 -> bit 2 = 0
// M106 P3 S250 -> bit 3 = 1
// M106 P0 S250  -> enable switch

// dwell -> applied during lines using HEATING ** 

/* G1 Y7 ; close gripper
G4 P5000 ; dwell
G1 X17 ; bend
G1 X13 ; return pin to origin
G1 Z10 ; advance
G1 W0
G1 Z8
G1 Y10
G1 W2
G1 Y7 ; close gripper
G4 P5000 ; dwell */