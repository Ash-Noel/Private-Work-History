____________________________________________________________________________________________________
// Private Variables

 // Motors
	float Motors;

 // Bending
	float Bending_Pin_Microsteps;
	float Bending_Pin_Steps_Per_Mm;
	float Bending_Pin_Step_Angle;
	string Bending_Pin_Home_Angle		// TBD VALUE;
	float Bending_Pin_Load_Angle;
	float Bending_Pin_Run_Angle;
	float Bending_Pin_Reduction_Ratio;
	float Bending_Pin_Home_Position;
	float Bending_Pin_End_Position;
	float Bending_Pin_Home_Offset;

 // Front
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

 // Rear
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

 // Advancement
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

 // Vertical
	float Vertical_Microsteps;
	float Vertical_Steps_Per_Mm;
	float Vertical_Step_Angle;
	string Vertical_Home_Angle		// TBD VALUE;
	float Vertical_Load_Angle;
	float Vertical_Run_Angle;
	float Vertical_Reduction_Ratio;
	string Vertical_Home_Position		// TBD VALUE;
	string Vertical_End_Position		// TBD VALUE;
	float Vertical_Home_Offset;
	float Vertical_Drop_Angle;

 // Max
	float Max_advancement_distance;

 // 3D
	string 3D_Rotation_Home_Position		// TBD VALUE;
	string 3D_Rotation_Run_Position		// TBD VALUE;
	string 3D_Rotation_Reduction_Ratio		// TBD VALUE;
