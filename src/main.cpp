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

class Robot{
    private:
        int robotPosX;
        int robotPosY;
    protected:
        int robotLife = 3;
        int robotKills;
        string robotName;
        char robotSymbol;
    public:
        virtual void see(int,int) = 0;
        virtual void move() = 0;
        virtual void shoot(int,int) = 0;
        virtual void think() = 0;  //to be changed in derived classes

        void setPosX(int x){robotPosX=x;}
        void setPosY(int y){robotPosY=y;}
        void setRobotName(string n){robotName=n;}
        void setRobotSymbol(char c){robotSymbol=c;}
        void setKills(int k){robotKills=k;}
};

class Battlefield{
    private:
        int row,col,steps;
        vector<char> field;
    public:
        Battlefield(int,int,int);
        int getRow(){return row;}
        int getCol(){return col;}
        int getSteps(){return steps;}
        void displayBattlefield();
};

Battlefield::Battlefield(int r, int c, int s){
    row=r;
    col=c;
    steps=s;
}

void Battlefield::displayBattlefield(){
    for (int i=0;i<row;i++){
        cout << endl;
        for (int j=0;j<col;j++){
            cout << '.';
        }
    }
    cout << endl;
}

int main(){
    Battlefield b(3,15,2);
    b.displayBattlefield();
    return 0;
}