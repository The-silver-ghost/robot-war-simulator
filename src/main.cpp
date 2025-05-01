/**********|**********|**********|
Program: YOUR_FILENAME.cpp / YOUR_FILENAME.h
Course: Object-Oriented Programming and Data Structures
Trimester: 2410
Name: Sanjeevan A/L Rames
ID: 243UC245LQ
Email: sanjeevan.rames@student.mmu.edu.my
Phone: 019-254 7818
Name: 
ID: 
Email: 
Phone: 
Name: 
ID: 
Email: 
Phone:
Name: 
ID: 
Email: 
Phone:
Lecture Section: TC3L
Tutorial Section: T12L
**********|**********|**********/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

class Battlefield{
};

int main(){
    ifstream configFile("config.cfg");
    string line;
    int row,col,numberOfSteps,numberOfRobots;
    while(getline(configFile,line)){
        if (line.find("M by N") != string::npos){
            string coordinates = line.substr(line.find(':')+2);
            row = stoi(coordinates.substr(0,coordinates.find(' ')));
            col = stoi(coordinates.substr(coordinates.find(' ')+1,-1));
        }
        else if (line.find("steps:") != string::npos){
            numberOfSteps = stoi(line.substr(line.find(':') +2,-1));
        }
        else if (line.find("robots") != string::npos){
            numberOfRobots = stoi(line.substr(line.find(':')+2,-1));
        }
        else if (line.find("Generic") != string::npos){
            cout << line << endl;
        }
    }
    return 0;
}