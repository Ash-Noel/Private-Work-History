# iCurve_cpp
iCurve Device Gcode Writing Program
Author: Ash Noel (arnoel2001@gmail.com)
Last Updated: 8/17/2022


# Notes with '*' are important/used frequently within the program 
# If opening the unzipped version of this project, move the header files into the same folder as 'Source.cpp'
# I highly recommend using 'Visual Studio Code'(VSC) to run this program and remain consistent
# I will include a detailed tutorial on how to install my current setup in the Google Drive folder

# To run this program on VSC:
#    1. Change terminal from 'powershell' to 'Ubuntu (WSL)'
#    2. Select your file from Scopic (e.g. D5cm_Half_Spline_KMTest1_Printed.csv)
#    3. Compile the program- I compile with g++ but you can use clang++ (e.g. g++ Source.cpp -o ex.out) 
#    4. Run the program with the file of your choosing- make sure it is in the same folder as Source.cpp, or run knowing the directory of your chosen file (e.g. ./ex.out D5cm_Half_Spline_KMTest1_Printed.csv)
#    5. Enter the titles of the device, tool, and machine once prompted.
#    6. Check the 'gcode_output.txt' file for the gcode output (should appear in the same file that Source.cpp is in)

# NOTE: If you would like to see the output of each point in the terminal, use the 'console_out' function in Source.cpp.


#  Storing and Accessing Information
    -> As of the time that this is written, there are a total of 5 classes being used to read and save variables from different files (device, machine, mat_hys [material hysteresis], material, and tool). The data from the files are saved into these separate classes for organization and for the most part follow the same structure as each other. 

    Number values saved to each class will always have the 'stof' function being used, TBD values or titles will not need any conversaion function when declaring the variable.
________________________________________________________________________________________________________________

# 'Source.cpp'
    -> This is the file where we'll be running the program. The mathematical steps to write the G-Code necessary to create the device are separated by functions called within the file. All the functions used in this file are listed below in order of appearance.

        split(string,char) -> Returns a parsed string- takes in parameters from csv file. (line 86, line 228)

        write_csv(2d float vector, string) -> Not used, will delete after documentation. 

        write(1d string vector, string) -> Outputs vector into a file- takes in the output from 'write_gcode.h'. (line 302, uses variable 'output' from write_gcode.h)
            -> output initialized line 163, finalized on line 369 in write_gcode.h

        console_out(2d float vector) -> Outputs vector into the console- takes in the 'points' variable. The points variable is changed throughout the file. Use console_out to see how points changes before and after each function call. 

        * gather_file(string) -> This function reads given files into a 2d string vector, outputs the result into a declared variable. Used to read devices(AbCd.csv), machines(MMC.csv), materials(1.csv,H_MMC_AbCd_EfG_1.csv), tool(EfG.csv). The values get transferred into its respective helper class file. 
        
            (line 105 - device file ['device.h'],
            * line 118 - material file ['material.h'] -> material class adds information to itself while the device class is being created, 
            line 166 - tool file [HELPER CLASS DNE], 
            line 183 - machine file ['machine.h'], 
            line 195 - material hysteresis file ['mat_hys.h'])
    ________________________________________________________________________________________________________________

        gathering_input_device() -> This creates the device object. Since devices have specific materials, the material object is created here as well (number_of_materials).
#       IMPORTANT:  number_of_materials: 2, 1, 50, 2,250
#                   param1 (2) is the number of materials used
#                   param2 (1) is the material ID
#                   param3 (50) is the length of that material
#                   param4 (2) is the next material ID
#                   param4 (250) is the length of that material
        Comments starting on line 124 until 156 read the variables that store information from the device and material files.

        gathering_input_tool() -> This stores only the title of the tool in a class. There is no additional information in this object class.
        
        gathering_input_machine() -> This creates the machine object. All the motors are stored into a vector. The 'Parse_Help' file (written in Python) can assist with any updates made in the machine (MMC.csv) file.

        gathering_input_material_hys(machine, device, tool) -> This creates the material hysteresis object. Takes in the machine, device, and tool in the parameters. This stores the information from the material hysteresis file.

    ________________________________________________________________________________________________________________
#   STEPS (still 'Source.cpp')

        SpacedInterp(2d float vector, float) -> Declared in 'utility.h' (line 170). Takes in the raw data points and the interpolation value in the parameters. This interpolates each point for the device to make spacing more equal on the bends. 
        
            This function uses the 'findNextPoint' function- comments have been made to view each step AND to describe each case to determine the next point to be interpolated. 

            In 'Source.cpp', points is being re-written to 'SpacedInterp(points,1.000)' since it is updating itself to have interpolated points with an interpolation of 1mm. 

        return_bends(2d float vector) -> Declared in 'utility.h' (line 262). Takes in the interpolated points in the parameters. Updates points variable.

        threshold_pass(2d float vector, device) -> Declared in 'variable_analysis.h' (line 72). Takes in the updated points and curve tolerance in the parameters. Updates points variables by removing Theta(Y) and Beta(Z) values below a given threshold.
            The curve tolerance is a value stored in the device file (in this case, AbCd.csv. This information is stored in the 'device.h' helper file.)

        before_hysteresis(2d float vector, device) -> Declared in 'variable_analysis.h' (line 94).
        Takes in the updated points and device information. Updates the theta and beta values in the points variable. Certain materials have different qualifications to bend- refer to more information about Hysteresis in the comments on 'variable_analysis.h' or from Kevin. 

        condense(2d float vector, device, machine) -> Declared in 'variable_analysis.h' (line 283). Takes in points, device and machine information. This function gets rid of redundant instructions in the points variable. 

        flip_180(2d float vector) -> Declared in 'variable_analysis.h' (line 336). This function finds an equivalent angle if theta or beta is greater than 180 degrees. Updates the points variable.
#       We have yet to find a way to translate the point values into degrees. 
#       It is currently bending each device by mm.

        apply_hysteresis(2d float vector, machine, device, tool) -> Declared in 'variable_analysis.h'. Takes in the updated points, machine, device, and tool information. Due to different materials having different hysteresis values, the information from the machine, device, and tool files are necessary in opening the material hysteresis files. (This is due to the material hysteresis file's name being formatted as 'H_[machine]_[device]_[tool]_[material].csv'). Formula can be found on the 'Hysteresis Check' file in the Google Drive folder. 

        splitting(2d float vector, machine) -> Declared in 'variable_analysis.h' (line 356). Takes in updated points and the max 3d rotation angle from the machine class. 

_____________________________________________________________________________________________________________

# 'Write_gcode.h'
    -> This file translates the points vector into a series of steps written in gcode. These steps are all appended to the variable 'output'. All the variables declared in the machine class are declared once more in this file, and are converted from float to string *if they are not TBD* (lines 76-164 && 223-225). This includes the list of motors (lines 66-72).

    Starting at line 167 is where we initialize the gcode. To determine how to translate each point, we have a flow chart that decides what happens to the device (if statements start on line 199). 
    The cases are written in binary- i.e., 101 means to advance and bend beta, 100 means to advance with no bends, 110 means to advance and bend theta, etc. Refer to the flow chart and comments for more cases. 

    The final output is saved to a .txt file (writing to a .gcode file did not work). This file is then put into YAT- a terminal compatible with Duet3d. 
# * Refer to https://docs.duet3d.com/en/User_manual/Reference/Gcodes for future Gcode commands.