# -*- coding: utf-8 -*-
"""
Created on Sun Jul 27 11:55:03 2022

@author: Ash Noel

This program was created to write helper files in C++ for the iCurve Medical project
"""
import os.path as f

print("NOTE TO USER: This program will not send \
the header file to the correct directory AND does not include any \
libraries that the current machine.h has.\nThe result will be output \
to a .h file and can be copied and pasted onto the file of your liking.\
\n\nThis program was created to make any updates to the machine.h file; however,\
if other classes follow the same structure of the machine class, this tool can \
be used to create the structure for those classes as well.\
\n\nNOTE: Be aware that some of the variables used on different files may be named \
differently if you choose to change them (i.e., variables being used in write_gcode.h \
may be off by a few letters/words. You can easily change this by using the find and replace tool\
 on any text editor.\
\n\n\
")

# data = ('pythonmmc.csv')

parsed_data = [] # list of everything in the file (i.e., the MMC file)
# categ = set(); # i.e., for the machine.h file, the motors will be here

file_check = f.exists("machine_copy_paste.h")
if (file_check == True):
    output = open("machine_copy_paste.h", "w")
else:
    output = open("machine_copy_paste.h", "x")

def shorten (parsed):
    i = 0
    category = []
    rewrite = parsed[:]
    
    while i < len(rewrite) - 1:
        title = rewrite[i][0]
        if (title[0] != rewrite[i-1][0][0]):             # if you've reached the end of a category, add a new line and a comment 
            print("\n // {}".format(title[0]))
            manual = input("Please enter shortened title for {} related variables\n>>".format(title[0])).strip()
            category.append((title[0],manual))
        
        i += 1
    # print(category)                                         # uncomment to view the changed titles
    i = 0
    while i < len(rewrite):   
        j = 0
        while j < len(category):
            # print ("CATEG: {}, TITLE: {}".format(category[j][0],parsed[i][0][0]))
            if (category[j][0] == rewrite[i][0][0]):
                rewrite[i][0].append(category[j][1])
                # print(parsed[i][0][0])
            j += 1
        
        i += 1
    
    return(rewrite)
    # print(parsed)
    

def class_constructor (parsed, output): # function used to 
    border = "_"*100
    print(border + "\n// Class Constructor\n")
    output.write(border + "\n// Class Constructor\n")   # writing to file
    
    i = 0
    text = ''
    while i < len(parsed):
        title = parsed[i][0]
        value = parsed[i][1]
        if (len(parsed[i]) == 2):
            if(value[0].isalpha() and value[0] == "TBD"):
                text = "\t{} = input[{}][1];\t\t // TBD VALUE".format("_".join(title),i)
            else:
                text = "\t{} = stof(input[{}][1]);".format("_".join(title),i)
               
        elif (len(parsed[i]) > 2):
            text = "\t// MULTIPLE VARIABLES IN LINE FOR '{}'- CODE MANUALLY".format("_".join(title))
            
        if (title[0] != parsed[i-1][0][0]):             # if you've reached the end of a category, add a new line and a comment 
            print("\n // {}".format(title[0]))
            output.write("\n // {}\n".format(title[0]))   # writing to file
        
        print(text)
        output.write(text+"\n")   # writing to file
        
        i += 1 


def private_variables (parsed, output):
    border = "_"*100    
    print(border + "\n// Private Variables\n")
    output.write(border + "\n// Private Variables\n")   # writing to file
    i = 1

    while i < len(parsed) - 1:
        title = parsed[i][0]
        value = parsed[i][1]
        # ask how the user would like to format- otherwise, automatically title
        tbd_float = "float"
        comment = ""
        
        if (len(parsed[i]) == 2):
            if(value[0].isalpha() and value[0] == "TBD"):
                comment = "\t\t// TBD VALUE"
                tbd_float = "string"
            text = "\t{} {}{};".format(tbd_float, "_".join(title), comment)
        elif (len(parsed[i]) > 2):
            text = "\tvector<string> {}; \t// SEE COMMENT FOR CODING MANUALLY".format("_".join(title).lower())
        
        if (title[0] != parsed[i-1][0][0]):             # if you've reached the end of a category, add a new line and a comment 
            print("\n // {}".format(title[0]))
            output.write("\n // {}\n".format(title[0]))   # writing to file
        
        print(text)
        output.write(text+"\n")   # writing to file
        
        i += 1     
    

def get_functions(parsed, output, flag):
    border = "_"*100
    print(border + "\n// Getters")
    output.write(border + "\n// Getters")   # writing to file
    i = 1
    
    while i < len(parsed) - 1:
        # ask how the user would like to format- otherwise, automatically title
        # not enough time to implement this ^
        title = parsed[i][0]
        value = parsed[i][1]
        tbd_float = "float"
        comment = ""
        # text = ''
        if (len(value) == 1):
        
            if(value[0].isalpha() and value[0] == "TBD"):
                comment = "\t// TBD VALUE"
                tbd_float = "string"
            
            if (flag == True):
                text = "\t{} get_{}_{}() {{return {}; }} {}".format(tbd_float,title[-1],"_".join(title[1:-1]).lower(),"_".join(title[:-1]), comment)
            elif (flag == False):
                text = "\t{} get_{}() {{return {}; }} {}".format(tbd_float,"_".join(title).lower(),"_".join(title), comment)
        elif (len(value) > 1):
            text = "\tvector<string> get_{}() {{ return {}; }}\t// SEE COMMENT FOR CODING MANUALLY".format("_".join(title).lower(),"_".join(title).lower())
        
        if (title[0] != parsed[i-1][0][0]):             # if you've reached the end of a category, add a new line and a comment 
            print("\n // {}".format(title[0]))
            output.write("\n // {}\n".format(title[0]))   # writing to file
             
        print(text)
        output.write(text+"\n")   # writing to file

        i += 1    

if __name__ == "__main__":
    # filename = input("Select ")
    flag = False  # has the title been shortened?
    
    data = input("Enter the file you would like to create a header file for\n>>").strip()
    
    check_name = f.exists(data)
    while(check_name == False):
        print ("File does not exist OR was entered incorrectly.")
        data = input("Enter the file you would like to create a header file for\n>>").strip()
        check_name = f.exists(data)

        # list of tuples. parsed_data = [ ([1broken, 1up, 1title], 1value), ([2broken, 2up, 2title], 2value) ]
        # print(text)
    
    select = input("Select which text you'd like printed (type 0 to end program):\n\n\t\t(1) Class Constructor\n\t\t(2) Private\n\t\t(3) Get Functions\n\t\t(4) PRINT ALL\n\n>>").strip()
    while select != "1" or select != "2" or select != "3" or select != "4" or select != "0": 

        if (select == "1"):
            class_constructor(parsed_data, output)
            output = open("machine_copy_paste.h", "r")
            output.close()
            break
    
        elif (select == "2"):
            private_variables(parsed_data, output)
            output = open("machine_copy_paste.h", "r")
            output.close()
            break
            
        elif (select == "3"):
            change_title = input("Would you like the shorten the variable name?\t (1) yes (2) no \n \t (i.e., get_vertical_reduction_ratio() would become get_vert_reduction_ratio()\n>>")
            new_data = []
            while change_title != "1" or change_title != "2":
                
                if (change_title == "1"):
                    new_data = shorten(parsed_data)
                    flag = True
                    break
                elif (change_title == "2"):
                    new_data = parsed_data
                    break
                else:
                    print("Please enter a valid response")
                    change_title = input("Would you like the shorten the variable name?\t (1) yes (2) no \n \t (i.e., get_vertical_reduction_ratio() would become get_vert_reduction_ratio()\n>>")
    
            get_functions(new_data, output, flag)
            output = open("machine_copy_paste.h", "r")
            output.close()
            break
            
        elif (select == "4"):
            class_constructor(parsed_data, output)
            print("\n")
            private_variables(parsed_data, output)
            print("\n")
            
            change_title = input("Would you like the shorten the variable name?\t (1) yes (2) no \n \t (i.e., get_vertical_reduction_ratio() would become get_vert_reduction_ratio()\n>>")
            new_data = []
            while change_title != "1" or change_title != "2":
               
               if (change_title == "1"):
                   new_data = shorten(parsed_data)
                   flag = True
                   break
               elif (change_title == "2"):
                   new_data = parsed_data
                   break
               else:
                   print("Please enter a valid response")
                   change_title = input("Would you like the shorten the variable name?\t (1) yes (2) no \n \t (i.e., get_vertical_reduction_ratio() would become get_vert_reduction_ratio()\n>>")
                # change_title = input("Would you like the shorten the variable name?\t (1) yes (2) no \n \t (i.e., get_vertical_reduction_ratio() would become get_vert_reduction_ratio()\n>>")
            
            get_functions(parsed_data, output, flag)
            output = open("machine_copy_paste.h", "r")
            output.close()
            break
            
        elif (select == "0"):
            print("👋")
            break
        else:
            print("Invalid option- please enter options 1 through 4, or 0 to end program")
            select = input("Select which text you'd like printed (type 0 to end program):\n\n\t\t(1) Class Constructor\n\t\t(2) Private\n\t\t(3) Get Functions\n\t\t(4) PRINT ALL\n\n>>").strip()
























