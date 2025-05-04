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
        /*virtual void see(int,int) = 0;
        virtual void move() = 0;
        virtual void shoot(int,int) = 0;
        virtual void think() = 0;  //to be changed in derived classes*/ //commented for testing purposes

        void setPosX(int x){robotPosX=x;}
        void setPosY(int y){robotPosY=y;}
        void setRobotName(string n){robotName=n;}
        void setRobotSymbol(char c){robotSymbol=c;}
        void setKills(int k){robotKills=k;}
        int getPosX(){return robotPosX;}
        int getPosY(){return robotPosY;}
        char getrobotSymbol(){return robotSymbol;}
};

class Battlefield{
    private:
        int row,col,steps;
        vector<Robot*> robots;
    public:
        Battlefield(int,int,int);
        int getRow(){return row;}
        int getCol(){return col;}
        int getSteps(){return steps;}
        void displayBattlefield();
        void addRobot(Robot* robot);
};

Battlefield::Battlefield(int r, int c, int s){
    row=r;
    col=c;
    steps=s;
}

void Battlefield::addRobot(Robot* robot){
    robots.push_back(robot);
}

void Battlefield::displayBattlefield(){
    for (int i=0;i<row;i++){
        cout << endl;
        for (int j=0;j<col;j++){
            bool robotFound = false;
            for (Robot* activeBots: robots){
                if (activeBots->getPosX()==j && activeBots->getPosY()==i){
                    cout << activeBots->getrobotSymbol();
                    robotFound = true;
                    break;
                }
            }
            if (!robotFound){
                cout << '.';
            }
        }
    }
    cout << endl;
}

int main(){
    Battlefield b(10,10,2);
    Robot r;
    Robot r2;
    Robot r3;
    r.setPosX(9);
    r.setPosY(1);
    r.setRobotSymbol('r');
    r2.setPosX(2);
    r2.setPosY(5);
    r2.setRobotSymbol('e');
    r3.setPosX(6);
    r3.setPosY(7);
    r3.setRobotSymbol('d');

    b.addRobot(&r);
    b.addRobot(&r2);
    b.addRobot(&r3);
    b.displayBattlefield();
    return 0;
}