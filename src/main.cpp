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

//global var
ofstream outfile;

class Robot{
    private:
        int robotPosX;
        int robotPosY;
    protected:
        int robotLife = 3;
        int robotKills;
        string robotName;
        char robotSymbol;
        string robotType;
        
    public:
        Robot(string,string,int,int);
        virtual void see(int,int,int,int) = 0;
        virtual void move(int, int) = 0;
        virtual void shoot(int,int) = 0;
        virtual void think(int,int) = 0;

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

Robot::Robot(string type,string name,int xCoord,int yCoord){
    robotType = type; robotName = name; robotSymbol = robotName[0];
    robotPosX = xCoord; robotPosY = yCoord;
}

class Battlefield{
    private:
        int row,col,steps,numberOfRobots;
    public:
        int getRow(){return row;}
        int getCol(){return col;}
        int getSteps(){return steps;}
        void displayBattlefield();
        void addRobot(Robot* robot);
        void beginSimulation();
        void removeRobot(Robot* robot);
        void readFile(ifstream &file);
        ~Battlefield();

        static vector<Robot*> robotsGlobal;
        
};

vector<Robot*> Battlefield::robotsGlobal;

void Battlefield::addRobot(Robot* robot){
    robotsGlobal.push_back(robot);
}

Battlefield::~Battlefield(){
    for (auto it=robotsGlobal.begin();it != robotsGlobal.end();){
        Robot *robot = *it;
        robot = nullptr;
        delete robot;
        it++;
    }
}

void Battlefield::removeRobot(Robot* robot) {
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
        outfile << endl;
        for (int j=0;j<col;j++){
            bool robotFound = false;
            for (Robot* activeBots: robotsGlobal){
                if (activeBots->getPosX()==j && activeBots->getPosY()==i && activeBots->getLife() > 0){
                    cout << activeBots->getrobotSymbol();
                    outfile << activeBots->getrobotSymbol();
                    robotFound = true;
                    break;
                }
            }
            if (!robotFound){
                cout << '.';
                outfile << '.';
            }
        }
    }
    cout << endl;
    outfile << endl;
}

void Battlefield::beginSimulation() {
    for (int i = 0; i < steps;) {
        for (auto it = robotsGlobal.begin(); it != robotsGlobal.end();) {
            if (i == steps) {
                break;
            } else {
                Robot* robot = *it;
                robot->think(col,row);
                displayBattlefield();
                i++;
                
                // Check if robot was killed during move/shoot
                if (robot->getLife() <= 0) {
                    it = robotsGlobal.erase(it);
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
        int lookCounter = 0;
        bool enemyFound= false;
        virtual void see(int,int,int,int) = 0;
};

class ThinkingRobot : virtual public Robot{
    public:
        virtual void think(int,int) = 0;
};

class GenericRobot : public MovingRobot, public SeeingRobot, public ShootingRobot, public ThinkingRobot{
    public:
        GenericRobot(string type,string name, int x, int y) : Robot(type,name,x,y){}

        void shoot(int x, int y) override{
            lookCounter --;
            if (shells > 0) {
                cout << robotSymbol << " shoots at (" << x << ", " << y << ")" << endl;
                outfile << robotSymbol << " shoots at (" << x << ", " << y << ")" << endl;
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
                                outfile << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                target->setLife(0);
                                robotKills++;
                                break;
                            }
                        }
                    } else {
                        cout << "Shot missed the target!" << endl;
                        outfile << "Shot missed the target!" << endl;
                    }
                } else {
                    cout << "Target not in adjacent position, shot missed!" << endl;
                    outfile << "Target not in adjacent position, shot missed!" << endl;
                }
                
                if (shells == 0) {
                    cout << robotSymbol << " is out of ammo and self-destructs!" << endl;
                    outfile << robotSymbol << " is out of ammo and self-destructs!" << endl;
                    robotLife = 0;
                }
            } else {
                cout << robotSymbol << " has no shells left!" << endl;
                outfile << robotSymbol << " has no shells left!" << endl;
            }
        }
        
        void think(int col,int row) override{
            int x = abs(getPosX() - col);
            int y = abs(getPosY() - row);
            if (lookCounter == 0){
                see(0,0,col,row);
                lookCounter ++;
                return;
            }
            else{
                if (x <= 1 && y <= 1){
                    shoot(col,row);
                    return;
                }
                else{
                    move(col,row);
                    return;
                }
            }
        }
        
        void move(int col,int row) override{
            lookCounter --;
            cout << "Robot " << getrobotSymbol()<<" starting at ("<<getPosX()<<","<<getPosY()<<")"<<endl;
            outfile << "Robot " << getrobotSymbol()<<" starting at ("<<getPosX()<<","<<getPosY()<<")"<<endl;
            
            if(enemyFound){
                dx = (enemyX > getPosX()) ? 1 : (enemyX < getPosX()) ? -1 : 0;
                dy = (enemyY > getPosY()) ? 1 : (enemyY < getPosY()) ? -1 : 0;
                cout << robotSymbol << " moves towards enemy robot at ("<<enemyX<<","<<enemyY<<")"<< endl;
                outfile << robotSymbol << " moves towards enemy robot at ("<<enemyX<<","<<enemyY<<")"<< endl;
            } else {
                dx = setdx(col);
                dy = setdy(row);
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
                    cout << "Robot " << getrobotSymbol()<<" moved to ("<<getPosX()<<","<<getPosY()<<")"<<endl;
                    outfile << "Robot " << getrobotSymbol()<<" moved to ("<<getPosX()<<","<<getPosY()<<")"<<endl;
                }
                else{
                    cout<< "Robot would not move to already occupied space" << endl;
                    outfile<< "Robot would not move to already occupied space" << endl;
                }
            }
        }
        
        void see(int x, int y,int col,int row) override {
            int centerX = getPosX() + x;
            int centerY = getPosY() + y;
            enemyFound = false;
            
            cout << "Robot " << robotSymbol << " is looking around (" << centerX << ", " << centerY << "):\n";
            outfile << "Robot " << robotSymbol << " is looking around (" << centerX << ", " << centerY << "):\n";
            
            for (int dy = -3; dy <= 3; dy++) {
                for (int dx = -3; dx <= 3; dx++) {
                    int nx = centerX + dx;
                    int ny = centerY + dy;
        
                    if (nx >= 0 && nx < col && ny >= 0 && ny < row) { 
                        for (Robot* other : Battlefield::robotsGlobal) { 
                            if (other != this && other->getPosX() == nx && other->getPosY() == ny && other->getLife() > 0) {
                                enemyX = nx;
                                enemyY = ny;
                                enemyFound = true;
                                cout << "Enemy robot found at (" << nx << ", " << ny << ") with symbol: " << other->getrobotSymbol() << "\n";
                                outfile << "Enemy robot found at (" << nx << ", " << ny << ") with symbol: " << other->getrobotSymbol() << "\n";
                                break;
                            }
                        }
                    } else {
                        cout << "(" << nx << ", " << ny << ") is out of battlefield bounds.\n";
                        outfile << "(" << nx << ", " << ny << ") is out of battlefield bounds.\n";
                    }
                }
            }
        }
};

class LongShotBot : public GenericRobot {
public:
    LongShotBot(string type, string name, int x, int y) 
        : Robot(type, name, x, y), GenericRobot(type, name, x, y) {}

    void shoot(int x, int y) override {
        lookCounter--;
        if (shells > 0) {
            cout << robotSymbol << " shoots at (" << x << ", " << y << ") with long range!" << endl;
            outfile << robotSymbol << " shoots at (" << x << ", " << y << ") with long range!" << endl;
            shells--;

            int distance = abs(getPosX() - x) + abs(getPosY() - y);
            
            if (distance <= 3) { 
                if ((rand() % 100) < 70) {
                    for (Robot* target : Battlefield::robotsGlobal) {
                        if (target != this && target->getPosX() == x && target->getPosY() == y) {
                            cout << "Long range hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                            outfile << "Long range hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                            target->setLife(0);
                            robotKills++;
                            break;
                        }
                    }
                } else {
                    cout << "Long range shot missed the target!" << endl;
                    outfile << "Long range shot missed the target!" << endl;
                }
            } else {
                cout << "Target out of long range, shot missed!" << endl;
                outfile << "Target out of long range, shot missed!" << endl;
            }

            if (shells == 0) {
                cout << robotSymbol << " is out of ammo and self-destructs!" << endl;
                outfile << robotSymbol << " is out of ammo and self-destructs!" << endl;
                robotLife = 0;
            }
        } else {
            cout << robotSymbol << " has no shells left!" << endl;
            outfile << robotSymbol << " has no shells left!" << endl;
        }
    }
};

class SemiAutoBot : public GenericRobot {
public:
    SemiAutoBot(string type, string name, int x, int y) 
        : Robot(type, name, x, y), GenericRobot(type, name, x, y) {}

    void shoot(int x, int y) override {
        lookCounter--;
        if (shells > 0) {
            cout << robotSymbol << " fires 3-shot burst at (" << x << ", " << y << ")!" << endl;
            outfile << robotSymbol << " fires 3-shot burst at (" << x << ", " << y << ")!" << endl;
            shells--;

            int dx = abs(getPosX() - x);
            int dy = abs(getPosY() - y);
            
            if (dx <= 1 && dy <= 1) {
                bool targetDestroyed = false;
                for (int i = 0; i < 3; i++) { 
                    if ((rand() % 100) < 70) {
                        for (Robot* target : Battlefield::robotsGlobal) {
                            if (target != this && target->getPosX() == x && target->getPosY() == y) {
                                if (!targetDestroyed) { 
                                    cout << "Burst shot hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                    outfile << "Burst shot hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                    target->setLife(0);
                                    robotKills++;
                                    targetDestroyed = true;
                                }
                                break;
                            }
                        }
                    }
                }
                
                if (!targetDestroyed) {
                    cout << "All burst shots missed the target!" << endl;
                    outfile << "All burst shots missed the target!" << endl;
                }
            } else {
                cout << "Target not in adjacent position, burst shots missed!" << endl;
                outfile << "Target not in adjacent position, burst shots missed!" << endl;
            }

            if (shells == 0) {
                cout << robotSymbol << " is out of ammo and self-destructs!" << endl;
                outfile << robotSymbol << " is out of ammo and self-destructs!" << endl;
                robotLife = 0;
            }
        } else {
            cout << robotSymbol << " has no shells left!" << endl;
            outfile << robotSymbol << " has no shells left!" << endl;
        }
    }
};

class ThirtyShotBot : public GenericRobot {
public:
    ThirtyShotBot(string type, string name, int x, int y) 
        : Robot(type, name, x, y), GenericRobot(type, name, x, y) {
        shells = 30;  
    }
};

void Battlefield::readFile(ifstream &file) {
    file.open("config.txt");
    string line;
    vector<string> robotTypeList;
    vector<string> robotNameList;
    vector<string> robotPosXList;
    vector<string> robotPosYList;
    int tempNumX, tempNumY;

    if (!file) {
        cout << "Fail to open config.txt" << endl;
        outfile << "Fail to open config.txt" << endl;
        return;
    }
    else {
        file >> line; file >> line; file >> line; file >> line; //skip M by N :
        file >> line; row = stoi(line); file >> line; col = stoi(line); //get row and col
        file >> line; file >> line; steps = stoi(line); //get steps
        file >> line; file >> line; numberOfRobots = stoi(line);//get number of robots
        for (int i = 0; i < numberOfRobots; i++) {
            file >> line; robotTypeList.push_back(line);
            file >> line; robotNameList.push_back(line);
            file >> line; robotPosXList.push_back(line);
            file >> line; robotPosYList.push_back(line);
        }
        for (int i = 0; i < numberOfRobots; i++) {
            if (robotPosXList[i] == "random") {
                tempNumX = rand() % getCol();
                if (robotPosYList[i] == "random") {
                    tempNumY = rand() % getRow();
                }
                else {
                    tempNumY = stoi(robotPosYList[i]);
                }
            }
            else {
                tempNumX = stoi(robotPosXList[i]);
                tempNumY = stoi(robotPosYList[i]);
            }

            if (robotTypeList[i] == "GenericRobot") {
                addRobot(new GenericRobot(robotTypeList[i], robotNameList[i], tempNumX, tempNumY));
            }
            else if (robotTypeList[i] == "LongShotBot") {
                addRobot(new LongShotBot(robotTypeList[i], robotNameList[i], tempNumX, tempNumY));
            }
            else if (robotTypeList[i] == "SemiAutoBot") {
                addRobot(new SemiAutoBot(robotTypeList[i], robotNameList[i], tempNumX, tempNumY));
            }
            else if (robotTypeList[i] == "ThirtyShotBot") {
                addRobot(new ThirtyShotBot(robotTypeList[i], robotNameList[i], tempNumX, tempNumY));
            }
        }
    }
}

int main(){
    ifstream infile;
    outfile.open("file.txt");
    srand(time(0));
    Battlefield b;
    b.readFile(infile);
    b.beginSimulation();
    
    return 0;
}