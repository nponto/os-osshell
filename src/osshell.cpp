
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <filesystem>
#include <fstream>

#include <algorithm>
namespace fs = std::experimental::filesystem;
// g++ -o osshell osshell.cpp 

void splitString(std::string text, char d, std::vector<std::string>& result);
void vectorOfStringsToArrayOfCharArrays(std::vector<std::string>& list, char ***result);
void freeArrayOfCharArrays(char **array, size_t array_length);

int main (int argc, char **argv)
{
    // Get list of paths to binary executables
    // `os_path_list` supports up to 16 directories in PATH, 
    //     each with a directory name length of up to 64 characters
    // Get list of paths to binary executables
    std::vector<std::string> os_path_list;
    char* os_path = getenv("PATH");
    splitString(os_path, ':', os_path_list);


    // Example code for how to loop over NULL terminated list of strings
    /*int i = 0;
    while (i < os_path_list.size())
    {
        printf("PATH[%2d]: %s\n", i, os_path_list[i]);
       i++;
    }*/


    // Welcome message
    printf("Welcome to OSShell! Please enter your commands ('exit' to quit).\n");


    // Repeat:
    //  Print prompt for user input: "osshell> " (no newline)
    //  Get user input for next command
    //  If command is `exit` exit loop / quit program
    //  If command is `history` print previous N commands
    //  For all other commands, check if an executable by that name is in one of the PATH directories
    //   If yes, execute it
    //   If no, print error statement: "<command_name>: Error command not found" (do include newline)
    

    while(true)

    {   
        std::string command_input;
        std::vector<std::string> command_list;
        getline(std::cin, command_input);
        
        splitString(command_input, ' ', command_list);
        
        if(command_list.size() == 0)
        {
            //if the command is just a newline, go to next iteration of while loop
            continue;
        }
        else
        {
            //add command to the history file
            std::ofstream history_file;
            history_file.open("history.txt", std::ios_base::app);
            history_file << command_input <<std::endl;
            history_file.close();
        }
        
        if(command_list[0] == "exit")
        {
            std::cout<<"exiting shell"<<std::endl;
            break;
        }
        //HISTORY command
        else if(command_list[0].find("history") != std::string::npos)
        {
            //if history is the only parameter
            if(command_list.size() == 1)
            {
                std::ifstream get_history("history.txt");
                //get_history.open("history.txt", std::ios_base::in);
                std::string line;

                int j = 0;
                while(std::getline(get_history,line))
                {
                    std::cout << j << ": " << line << std::endl;
                    j++;
                }
                get_history.close();
            }
            //check if there is more than one parameter
            if(command_list.size() > 1)
            {
                //if second paramter is clear, clear the history.txt
                if(command_list[1].find("clear") != std::string::npos)
                {
                    std::fstream delete_history;
                    delete_history.open("history.txt", std::ios_base::out);
                    delete_history << "";
                    std::cout << "history cleared"<<std::endl;
                    delete_history.close();
                    continue;
                }

                //check to see if second parameter is a digit
                bool is_digit = true;
                for(int i = 0; i < command_list[1].size(); i++)
                {
                    if(std::isdigit(command_list[1][i]) == false)
                    {
                        is_digit == false;
                    }
                }
                //if second parameter is a digit, then create a new int with that value
                int far_back = 0;
                if(is_digit == true)
                {
                    far_back = std::stoi(command_list[1]);
                    std::vector<std::string> partial_history_list;
                    if(far_back < 1)
                    {
                        std::cout << "Error: history expects an integer > 0(or clear" << std::endl;
                    }

                    std::fstream partial_history;
                    partial_history.open("history.txt", std::ios_base::in);
                    

                }
                
                
            
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
    }

    return 0;
}

/*
   list: vector of strings to convert to an array of character arrays
   result: pointer to an array of character arrays when the vector of strings is copied to
*/
void vectorOfStringsToArrayOfCharArrays(std::vector<std::string>& list, char ***result)
{
    int i;
    int result_length = list.size() + 1;
    *result = new char*[result_length];
    for (i = 0; i < list.size(); i++)
    {
        (*result)[i] = new char[list[i].length() + 1];
        strcpy((*result)[i], list[i].c_str());
    }
    (*result)[list.size()] = NULL;
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
void splitString(std::string text, char d, std::vector<std::string>& result)
{
    enum states { NONE, IN_WORD, IN_STRING } state = NONE;

    int i;
    std::string token;
    result.clear();
    for (i = 0; i < text.length(); i++)
    {
        char c = text[i];
        switch (state) {
            case NONE:
                if (c != d)
                {
                    if (c == '\"')
                    {
                        state = IN_STRING;
                        token = "";
                    }
                    else
                    {
                        state = IN_WORD;
                        token = c;
                    }
                }
                break;
            case IN_WORD:
                if (c == d)
                {
                    result.push_back(token);
                    state = NONE;
                }
                else
                {
                    token += c;
                }
                break;
            case IN_STRING:
                if (c == '\"')
                {
                    result.push_back(token);
                    state = NONE;
                }
                else
                {
                    token += c;
                }
                break;
        }
    }
    if (state != NONE)
    {
        result.push_back(token);
    }
}
