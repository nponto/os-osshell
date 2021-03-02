
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;
// g++ -o osshell osshell.cpp 

void allocateArrayOfCharArrays(char ***array_ptr, size_t array_length, size_t item_size);
void freeArrayOfCharArrays(char **array, size_t array_length);
void splitString(std::string text, char d, char **result);

int main (int argc, char **argv)
{
    // Get list of paths to binary executables
    // `os_path_list` supports up to 16 directories in PATH, 
    //     each with a directory name length of up to 64 characters
    char **os_path_list;
    allocateArrayOfCharArrays(&os_path_list, 16, 64);
    char* os_path = getenv("PATH");
    splitString(os_path, ':', os_path_list);


    // Example code for how to loop over NULL terminated list of strings
    int i = 0;
    while (os_path_list[i] != NULL)
    {
        printf("PATH[%2d]: %s\n", i, os_path_list[i]);
       i++;
    }


    // Welcome message
    printf("Welcome to OSShell! Please enter your commands ('exit' to quit).\n");

    // Allocate space for input command lists
    // `command_list` supports up to 32 command line parameters, 
    //     each with a parameter string length of up to 128 characters
    char **command_list;
    allocateArrayOfCharArrays(&command_list, 32, 128);

    // Repeat:
    //  Print prompt for user input: "osshell> " (no newline)
    //  Get user input for next command
    //  If command is `exit` exit loop / quit program
    //  If command is `history` print previous N commands
    //  For all other commands, check if an executable by that name is in one of the PATH directories
    //   If yes, execute it
    //   If no, print error statement: "<command_name>: Error command not found" (do include newline)
    i = 0;

    while(true)
    {   std::string command_input;
        std::cout<<"osshell>";
        
        std::cin>>command_input;
    
        //add command to the history file
        std::ofstream history_file;
        history_file.open("history.txt", std::ios_base::app);
        history_file << command_input <<std::endl;
        history_file.close();

        if(command_input == "exit")
        {
            std::cout<<"exiting shell"<<std::endl;
            break;
        }
        else if(command_input == "history")
        {
           std::fstream get_history;
           get_history.open("history.txt", std::ios_base::in);
           std::string line;
           int j = 0;
           while(getline(get_history,line))
           {
               std::cout << j << ": " << line << std::endl;
               j++;
           }
        }
        else
        {
            //TODO: implement executables
            char current = new char[128];
            bool foundExe = false;
            int compLen = 0;
            char compStr;
            int commandLen = 0;
            while (command_input[commandLen] != NULL) {
                commandLen += 1;
            }
            //Change 9 to be dynamic
            for (int i = 0; i < 9; i++) {
                std::string path = os_path_list[i];
                for (const auto & entry : fs::directory_iterator(path)) {
                    std::cout << entry.path() << std::endl; 
                    current = entry.path();
                    //This section compares the input to the last x characters in the current path executable
                    //x = the length of the input 
                    int compLen = 0;
                    compStr = new char[128[;]]
                    while (current[compLen] != NULL) {
                        compLen += 1;
                    }
                    for (int i = compLen - commandLen; i < compLen; i++) {
                        compStr[i] = current[i];
                    }
                    //Change this to match the x characters from the end of current
                    //to match command_input
                    if (command_input == compStr) {
                        foundExe = true;
                        break;
                    }
                }
                if (foundExe == true) {
                    break;
                }
            }
            if (!foundExe) {
                std::cout << command_input << ": Error command not found" << std::endl;
                continue;
            }

            //Should this be formatted differently
            int pid = fork();
            if (pid == 0){ 
                //Something
            } else {
                system(current);
            }
            pid.join();

        }
        i++;        
    }
    
  
    // Free allocated memory
    freeArrayOfCharArrays(os_path_list, 16);
    freeArrayOfCharArrays(command_list, 32);

    return 0;
}

/*
   array_ptr: pointer to list of strings to be allocated
   array_length: number of strings to allocate space for in the list
   item_size: length of each string to allocate space for
*/
void allocateArrayOfCharArrays(char ***array_ptr, size_t array_length, size_t item_size)
{
    int i;
    *array_ptr = new char*[array_length];
    for (i = 0; i < array_length; i++)
    {
        (*array_ptr)[i] = new char[item_size];
    }
}

/*
   array: list of strings to be freed
   array_length: number of strings in the list to free
*/
void freeArrayOfCharArrays(char **array, size_t array_length)
{
    int i;
    for (i = 0; i < array_length; i++)
    {
        delete[] array[i];
    }
    delete[] array;
}

/*
   text: string to split
   d: character delimiter to split `text` on
   result: NULL terminated list of strings (char **) - result will be stored here
*/
void splitString(std::string text, char d, char **result)
{
    int i;
    std::vector<std::string> list;
    std::stringstream ss(text);
    std::string token;
    
    while (std::getline(ss, token, d))
    {
        list.push_back(token);
    }

    for (i = 0; i < list.size(); i++)
    {
        strcpy(result[i], list[i].c_str());
    }
    result[list.size()] = NULL;
}
