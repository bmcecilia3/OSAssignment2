// compile: g++ -std=c++11 -o bin/osshell osshell.cpp
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>

using namespace std;

string* history = new string[128];
bool exe;

std::vector<std::string> splitString(std::string text, char d);
std::string getFullPath(std::string cmd, const std::vector<std::string>& os_path_list);
bool fileExists(std::string full_path, bool *executable);

bool isNumber(string s);
void leftRotatebyOne(string arr[], int n);

int main (int argc, char **argv)
{
    std::string input;
    char* os_path = getenv("PATH");
    std::vector<std::string> os_path_list = splitString(os_path, ':');

    std::cout << "Welcome to OSShell! Please enter your commands ('exit' to quit)." << std::endl;

    // Repeat:
    //  Print prompt for user input: "osshell> " (no newline)
    //  Get user input for next command
    //  If command is `exit` exit loop / quit program
    //  If command is `history` print previous N commands
    //  For all other commands, check if an executable by that name is in one of the PATH directories
    //   If yes, execute it
    //   If no, print error statement: "<command_name>: Error running command" (do include newline)
	ifstream infile;
	string entry;
	infile.open ("src/array.txt");
	int v = 0;
	while(getline(infile, entry))
	{
		if(history[v].compare("") == 0)
		{
			history[v] = entry;
			v = v + 1;
		}
	}
	infile.close();
    int w = 0;
	while(w == 0)
	{
		string input;
		std::cout << "osshell> ";
		getline(cin, input);
		int place = -1;
		for(int j = 0; j < 128; j++)
		{
			if(history[j].compare("") == 0)
			{
				history[j] = input;
				place = j;
				j = 129;
			}
		}
		if(place == -1)
		{
			leftRotatebyOne(history, 128);
			history[127] = input;
		}
		if(input.find("history") == 0)
		{
			if(input.compare("history") == 0)
			{
				for(int k = 0; k < 127; k++)
				{
					if(history[k].compare("") != 0 && history[k+1].compare("") != 0)
					{
						std::cout << "\t" << k+1 << ": " << history[k] << std::endl;
					}
					else
					{
						k = 129;
					}
				}
			}
			else if(input.compare("history clear") == 0)
			{
				for(int i = 0; i < 128; i++)
				{
					history[i] = "";
				}
			}
			else
			{
				string num = input.substr(8);
				if(!isNumber(num))
				{
					std::cout << "Error: history expects an integer >0 (or 'clear')" << std::endl;
				}
				else
				{
					int line = place - stoi(num);
					if(line < 0)
					{
						line = 0;
					}
					while(line < place)
					{
						std::cout << "\t" << line+1 << ": " << history[line] << std::endl;
						line = line + 1;
					}
				}
			}
		}
		else if(input.compare("\n") == 0){}
		else if(input.compare("exit") == 0)
		{
			w = 1;
		}
		else
		{
			string fullPath = getFullPath(input.substr(0, input.find_first_of(' ', 0)), os_path_list);
			if(fullPath.compare("") == 0)
			{
				std::cout << input << ": Error running command" << std::endl;
			}
			else if(!exe)
			{
				std::cout << input << ": File exists but is not executable" << std::endl;
			}
			else
			{
				std::vector<std::string> full_path_list = splitString(fullPath + " " + input.substr(input.find_first_of(' ', 0) + 1, input.length()), ' ');
				pid_t pid = fork();
				pid_t wpid;
				int status = 0;
				if(pid == 0)
				{
					char *args[full_path_list.size() + 1];
					for(int z = 0; z < full_path_list.size(); z++)
					{
						args[z] = (const_cast<char *>(full_path_list[z].c_str()));
					}
					args[full_path_list.size()] = NULL;
					execv(args[0], &args[0]);
				}
				else
				{
					while ((wpid = wait(&status)) > 0);
				}
			}
		}
	}
	ofstream outfile;
	outfile.open("src/array.txt", std::ofstream::out | std::ofstream::trunc);
	for(int j = 0; j < 128; j++)
	{
		if(history[j].compare("") == 0)
		{
			j = 129;
		}
		else
		{
			outfile << history[j] << "\n";
		}
	}
	outfile.close();
    return 0;
}

// Returns vector of strings created by splitting `text` on every occurance of `d`
std::vector<std::string> splitString(std::string text, char d)
{
    std::vector<std::string> result;
	int found = text.find_first_of(d,0);
	string before;
	string after;
	before = text.substr(0, found);
	after = text.substr(found+1, text.length());
	while(found != std::string::npos)
	{		
		result.push_back(before);
		found = after.find_first_of(d,0);
		before = after.substr(0, found);
		after = after.substr(found+1, text.length());
	}
	result.push_back(after);
    return result;
}

// Returns a string for the full path of a command if it is found in PATH, otherwise simply return ""
std::string getFullPath(std::string cmd, const std::vector<std::string>& os_path_list)
{
	string full;
	for(int y = 0; y < os_path_list.size(); y++)
	{
		full = os_path_list[y] + "/" + cmd;
		if(fileExists(full, &exe))
		{
			return full;
		}
	}
    return "";
}

// Returns whether a file exists or not; should also set *executable to true/false 
// depending on if the user has permission to execute the file
bool fileExists(std::string full_path, bool *executable)
{
	if(access(full_path.c_str(), X_OK) != -1)
	{
	    exe = true;
	}
	else
	{
		exe = false;
	}	
	ifstream ifile;
	ifile.open(full_path);
	if(ifile)
	{
		return true;
	}
	else{}
    return false;
}

bool isNumber(string s) 
{ 
    for (int i = 0; i < s.length(); i++) 
    {
        if (isdigit(s[i]) == false) 
        {
            return false; 
        }
    }
    return true; 
} //https://www.geeksforgeeks.org/program-check-input-integer-string/

void leftRotatebyOne(string arr[], int n) 
{ 
    string temp = arr[0]; 
    int i;
    for (i = 0; i < n - 1; i++) 
    {
        arr[i] = arr[i + 1]; 
    }
    arr[i] = temp; 
} //https://www.geeksforgeeks.org/array-rotation/
