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
Phone: 014-3773108
**********|**********|**********|
Name: Harvind a/l Sethu Pathy
ID: 243UC247DM
Email: harvind.sethu.pathy@student.mmu.edu.my
Phone: 6019 454 6875
**********|**********|**********|
Name: Isaiah Naden a/l Felix Arokianathan
ID: 243UC2466L
Email: isaiah.naden.felix@student.mmu.edu.my
Phone:010-212 2720
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
        virtual void see(int,int) = 0;
        virtual void move(int, int) = 0;
        virtual void shoot(int,int) = 0;
        virtual void think() = 0;

        void setPosX(int x){robotPosX=x;}
        void setPosY(int y){robotPosY=y;}
        void setRobotName(string n){robotName=n;}
        void setRobotSymbol(char c){robotSymbol=c;}
        void setKills(int k){robotKills=k;}
        void setLife(int l){robotLife=l;}
        int getPosX(){return robotPosX;}
        int getPosY(){return robotPosY;}
        char getrobotSymbol(){return robotSymbol;}
        int getLife(){return robotLife;}
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
        void removeRobot(Robot* robot);
        static vector<Robot*> robotsGlobal;
        
};

vector<Robot*> Battlefield::robotsGlobal;

Battlefield::Battlefield(int r, int c, int s){
    row=r;
    col=c;
    steps=s;
}

void Battlefield::addRobot(Robot* robot){
    robots.push_back(robot);
    robotsGlobal.push_back(robot);
}

void Battlefield::removeRobot(Robot* robot) {
    for (auto it = robots.begin(); it != robots.end(); ) {
        if (*it == robot) {
            it = robots.erase(it);
        } else {
            ++it;
        }
    }
    for (auto it = robotsGlobal.begin(); it != robotsGlobal.end(); ) {
        if (*it == robot) {
            it = robotsGlobal.erase(it);
        } else {
            ++it;
        }
    }
}

void Battlefield::displayBattlefield(){
    for (int i=0;i<row;i++){
        cout << endl;
        for (int j=0;j<col;j++){
            bool robotFound = false;
            for (Robot* activeBots: robots){
                if (activeBots->getPosX()==j && activeBots->getPosY()==i && activeBots->getLife() > 0){
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

void Battlefield::beginSimulation() {
    for (int i = 0; i < steps;) {
        for (auto it = robots.begin(); it != robots.end();) {
            if (i == steps) {
                break;
            } else {
                Robot* robot = *it;
                robot->see(0, 0);
                robot->move(getCol(),getRow());
                displayBattlefield();
                i++;
                
                // Check if robot was killed during move/shoot
                if (robot->getLife() <= 0) {
                    it = robots.erase(it);
                } else {
                    it++;
                }
            }
        }
    }
}

class ShootingRobot : virtual public Robot {
protected:
    int shells = 10;

public:
    virtual void shoot(int x, int y) = 0;
    int getShells() { return shells; }
};

class MovingRobot : virtual public Robot{
    public:
    int newpos_x;
    int newpos_y;
    int dx;
    int dy;
    virtual void move(int,int) = 0;
    int setdx(int col){
       
       return (rand()%col)-(col/2);
    }
    int setdy(int row){
        
        return (rand()%row)-(row/2);
    }
};

class SeeingRobot : virtual public Robot{
    public:
        int enemyX= -1;
        int enemyY = -1;
        bool enemyFound= false;
        virtual void see(int,int) = 0;
};

class ThinkingRobot : virtual public Robot{
    public:
        virtual void think() = 0;
};

class GenericRobot : public MovingRobot, public SeeingRobot, public ShootingRobot, public ThinkingRobot{
    public:
        void shoot(int x, int y) override{
            if (shells > 0) {
                cout << robotSymbol << " shoots at (" << x << ", " << y << ")" << endl;
                shells--;
                
                // Check if target is adjacent (within 1 space)
                int dx = abs(getPosX() - x);
                int dy = abs(getPosY() - y);
                if (dx <= 1 && dy <= 1) {
                    // 70% chance to kill
                    if ((rand() % 100) < 70) {
                        for (Robot* target : Battlefield::robotsGlobal) {
                            if (target != this && target->getPosX() == x && target->getPosY() == y) {
                                cout << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                target->setLife(0);
                                robotKills++;
                                break;
                            }
                        }
                    } else {
                        cout << "Shot missed the target!" << endl;
                    }
                } else {
                    cout << "Target not in adjacent position, shot missed!" << endl;
                }
                
                if (shells == 0) {
                    cout << robotSymbol << " is out of ammo and self-destructs!" << endl;
                    robotLife = 0;
                }
            } else {
                cout << robotSymbol << " has no shells left!" << endl;
            }
        }
        
        void think() override{}
        
        void move(int col,int row) override{
            cout << "Robot" << getrobotSymbol()<<" starting at("<<getPosX()<<","<<getPosY()<<")"<<endl;
            
            if(enemyFound){
                // Check if enemy is adjacent
                int dx = abs(getPosX() - enemyX);
                int dy = abs(getPosY() - enemyY);
                if (dx <= 1 && dy <= 1) {
                    shoot(enemyX, enemyY);
                    return; // Skip movement after shooting
                }
                
                dx = (enemyX > getPosX()) ? 1 : (enemyX < getPosX()) ? -1 : 0;
                dy = (enemyY > getPosY()) ? 1 : (enemyY < getPosY()) ? -1 : 0;
                cout << robotSymbol << " moves towards enemy robot at("<<enemyX<<","<<enemyY<<")"<< endl;
            } else {
                dx = setdx(col);
                dy = setdy(row);
                cout << "dx=" << dx << ",dy=" << dy << endl;
            }
            
            newpos_x = getPosX() + (dx > 0 ? 1 : (dx < 0 ? -1 : 0));
            newpos_y = getPosY() + (dy > 0 ? 1 : (dy < 0 ? -1 : 0));

            if(newpos_x >=0 && newpos_x<col && newpos_y>=0 && newpos_y<row){
                bool occupied = false;
                for (Robot* other : Battlefield::robotsGlobal) { 
                    if (other != this && other->getPosX() == newpos_x && other->getPosY() == newpos_y && other->getLife() > 0) {
                        occupied=true;
                        // Check if adjacent to shoot
                        int adj_dx = abs(getPosX() - newpos_x);
                        int adj_dy = abs(getPosY() - newpos_y);
                        if (adj_dx <= 1 && adj_dy <= 1) {
                            shoot(newpos_x, newpos_y);
                            return;
                        }
                        break;
                    }
                }
                
                if(!occupied){
                    setPosX(newpos_x);
                    setPosY(newpos_y);
                    cout << "Robot" << getrobotSymbol()<<" moved to("<<getPosX()<<","<<getPosY()<<")"<<endl;
                }
                else{
                    cout<< "Robot would not move to already occupied space" << endl;
                }
            }
        }
        
        void see(int x, int y) override {
            int centerX = getPosX() + x;
            int centerY = getPosY() + y;
            enemyFound = false;
            
            cout << "Robot " << robotSymbol << " is looking around (" << centerX << ", " << centerY << "):\n";
            
            for (int dy = -3; dy <= 3; dy++) {
                for (int dx = -3; dx <= 3; dx++) {
                    int nx = centerX + dx;
                    int ny = centerY + dy;
        
                    if (nx >= 0 && nx < 20 && ny >= 0 && ny < 20) { 
                        for (Robot* other : Battlefield::robotsGlobal) { 
                            if (other != this && other->getPosX() == nx && other->getPosY() == ny && other->getLife() > 0) {
                                enemyX = nx;
                                enemyY = ny;
                                enemyFound = true;
                                cout << "  Enemy robot found at (" << nx << ", " << ny << ") with symbol: " << other->getrobotSymbol() << "\n";
                                break;
                            }
                        }
                    } else {
                        cout << "  (" << nx << ", " << ny << ") is out of battlefield bounds.\n";
                    }
                }
            }
        }
};

int main(){
    srand(time(0));
    Battlefield b(5,5,10);
    Robot *r1 = new GenericRobot;
    Robot *r = new GenericRobot;
    Robot *r2 = new GenericRobot;
    r->setPosX(4);
    r->setPosY(4);
    r->setRobotSymbol('r');
    r1->setPosX(3);
    r1->setPosY(3);
    r1->setRobotSymbol('e');
    r2->setPosX(1);
    r2->setPosY(1);
    r2->setRobotSymbol('d');
    b.addRobot(r);
    b.addRobot(r1);
    b.addRobot(r2);
    b.beginSimulation();

    delete r;
    delete r1;
    delete r2;
    
    return 0;
}