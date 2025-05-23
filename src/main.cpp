/**********|**********|**********|
Program: YOUR_FILENAME.cpp / YOUR_FILENAME.h
Course: Object-Oriented Programming and Data Structures
Trimester: 2510
**********|**********|**********|
Name: Sanjeevan A/L Rames
ID: 243UC245LQ
Email: sanjeevan.rames@student.mmu.edu.my
Phone: 019-254 7818
**********|**********|**********|
Name: Hemaraj A/L Rajan
ID: 243UC247BQ
Email: hemaraj.rajan@student.mmu.edu.my
Phone: 
**********|**********|**********|
Name: Harvind a/l Sethu Pathy
ID: 243UC247DM
Email: harvind.sethu.pathy@student.mmu.edu.my
Phone: 6019 454 6875
**********|**********|**********|
Name: 
ID: 
Email: 
Phone:
**********|**********|**********|
Lecture Section: TC3L
Tutorial Section: T12L
**********|**********|**********/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
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
       // virtual void see(int,int) = 0;
        virtual void move() = 0;
        //virtual void shoot(int,int) = 0;
       // virtual void think() = 0;  // //commented for testing purposes

        void setPosX(int x){robotPosX=x;}
        void setPosY(int y){robotPosY=y;}
        void setRobotName(string n){robotName=n;}
        void setRobotSymbol(char c){robotSymbol=c;}
        void setKills(int k){robotKills=k;}
        int getPosX(){return robotPosX;}
        int getPosY(){return robotPosY;}
        char getrobotSymbol(){return robotSymbol;}
};

class ShootingRobot : virtual public Robot {
protected:
    int shells = 10;

public:
    virtual void shoot(int x, int y) = 0;
    int getShells() { return shells; }
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
        void beginSimulation();
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

class ShootingBot : public ShootingRobot {
public:
    void move() override {
        cout << robotSymbol << " stays in place." << endl;
    }

    void shoot(int x, int y) override {
        if (shells > 0) {
            cout << robotSymbol << " shoots at (" << x << ", " << y << ")" << endl;
            shells--;
            if (shells == 0) {
                cout << robotSymbol << " is out of ammo and self-destructs!" << endl;
            }
        } else {
            cout << robotSymbol << " has no shells left!" << endl;
        }
    }
};

void Battlefield::beginSimulation() {
    for (int i = 0; i < steps;) {
        for (auto it = robots.begin(); it != robots.end();) {
            if (i == steps) {
                break;
            } else {
                Robot* robot = *it;

                robot->move();

                ShootingRobot* shooter = dynamic_cast<ShootingRobot*>(robot);
                if (shooter && shooter->getShells() > 0) {
                    int tx = robot->getPosX() + (rand() % 3) - 1;
                    int ty = robot->getPosY() + (rand() % 3) - 1;
                    if (tx >= 0 && tx < col && ty >= 0 && ty < row) {
                        shooter->shoot(tx, ty);
                    }
                }

                displayBattlefield();
                cout << "Steps: " << i + 1 << endl;
                cout << robot->getrobotSymbol() << " moved to " 
                     << robot->getPosX() << " " << robot->getPosY() << endl;

                i++;
                it++;
            }
        }
    }
}

class MovingBot : public Robot{
    public:
    int newpos_x;
    int newpos_y;
    int dx;
    int dy;
    Battlefield * battlefield = nullptr;

    void setBattleField(Battlefield * bf){battlefield=bf;} 

    int setdx(){
       
       return (rand()%battlefield->getCol())-(battlefield->getCol()/2);
    }
    int setdy(){
        
        return (rand()%battlefield->getRow())-(battlefield->getRow()/2);
    }

    void move() override{
        dx= setdx();
        dy= setdy();
        newpos_x = getPosX() + (dx > 0 ? 1 : (dx < 0 ? -1 : 0));
        newpos_y = getPosY() + (dy > 0 ? 1 : (dy < 0 ? -1 : 0));

        if(newpos_x >=0 && newpos_x<battlefield->getCol() && newpos_y>=0 && newpos_y<battlefield->getRow()){
        setPosX(newpos_x);
        setPosY(newpos_y); //srand(time(0)) % (max-min+1)
        }
}

};

int main(){
    srand(time(0));
    Battlefield b(20,20,10);
    Robot * r = nullptr;
    Robot  * r2 = nullptr;
    Robot * r3 = nullptr;

    r = new MovingBot();
    r2 = new MovingBot();
    r3 = new MovingBot();

    r->setPosX(9);
    r->setPosY(1);
    r->setRobotSymbol('r');
    
    r2->setPosX(2);
    r2->setPosY(5);
    r2->setRobotSymbol('e');

    r3->setPosX(6);
    r3->setPosY(7);
    r3->setRobotSymbol('d');

    static_cast<MovingBot*>(r)->setBattleField(&b);
    static_cast<MovingBot*>(r2)->setBattleField(&b);
    static_cast<MovingBot*>(r3)->setBattleField(&b);

    Robot* shooter = new ShootingBot();
    shooter->setPosX(4);
    shooter->setPosY(4);
    shooter->setRobotSymbol('s');
    b.addRobot(shooter);

    b.addRobot(r);
    b.addRobot(r2);
    b.addRobot(r3);
    b.displayBattlefield();
    cout << "Initial Condition" << endl;

    b.beginSimulation();

    delete r;
    r = nullptr;
    delete r2;
    r2 = nullptr;
    delete r3;
    r3 = nullptr;
    
    return 0;
}