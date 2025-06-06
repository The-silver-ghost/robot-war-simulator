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
        int robotKills;
        string robotName;
        char robotSymbol;
        string robotType;
        
    public:
        int robotLife = 3,upgradeCounterShoot = 0,upgradeCounterSee = 0,upgradeCounterMove = 0, killNeeded = 1;
        bool isDead = false;
        bool ishiding= false;
        int hidecount=3;

        virtual void see(int,int,int,int) = 0;
        virtual void move(int, int) = 0;
        virtual void shoot(int,int) = 0;
        virtual void think(int,int) = 0;

        Robot(string,string,int,int);
        void setPosX(int x){robotPosX=x;}
        void setPosY(int y){robotPosY=y;}
        void setRobotName(string n){robotName=n;}
        string getRobotType(){return robotType;}
        string getRobotName(){return robotName;}
        void setRobotSymbol(char c){robotSymbol=c;}
        void setKills(int k){robotKills=k;}
        int getKills(){return robotKills;}
        void setLife(){isDead = true;robotLife --;}
        int getPosX(){return robotPosX;}
        int getPosY(){return robotPosY;}
        char getrobotSymbol(){return robotSymbol;}
};

Robot::Robot(string type,string name,int xCoord,int yCoord){
    robotType = type; robotName = name; robotSymbol = robotName[0];
    robotPosX = xCoord; robotPosY = yCoord;
}

class Battlefield{
    private:
        int row,col,steps,numberOfRobots;
        void displayBattlefield();
    public:
        int getRow(){return row;}
        int getCol(){return col;}
        int getSteps(){return steps;}
        void addRobot(Robot* robot);
        void beginSimulation();
        void readFile(ifstream &file);
        void upgradeBot(Robot*);
        ~Battlefield();

        static vector<Robot*> robotsGlobal;
        static vector<Robot*> robotsQueueGlobal;
        
};

ostream& operator<<(ostream& out, Robot *bot){
    out << "(" << bot->getPosX() << "," << bot->getPosY() << ")" << endl;
    return out;
}

vector<Robot*> Battlefield::robotsGlobal;
vector<Robot*> Battlefield::robotsQueueGlobal;

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
    for (auto it=robotsQueueGlobal.begin();it != robotsQueueGlobal.end();){
        Robot *robot = *it;
        robot = nullptr;
        delete robot;
        it++;
    }
}

void Battlefield::displayBattlefield(){
    for (int i=0;i<row;i++){
        cout << endl;
        outfile << endl;
        for (int j=0;j<col;j++){
            bool robotFound = false;
            for (Robot* activeBots: robotsGlobal){
                if (activeBots->getPosX()==j && activeBots->getPosY()==i && activeBots->isDead == false){
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
        for (auto it = robotsGlobal.begin(); it != robotsGlobal.end();){
            Robot* robot = *it;
            if (robotsGlobal.size() == 1){
                cout << "One Robot remains" << endl;
                outfile << "One Robot remains" << endl;
                return;
            }
            if (robot->isDead == false){
            cout << "Step: " << i+1 << endl;
            outfile << "Step: " << i+1 << endl;
                robot->think(col,row);
                if (robot->getKills() > 0 && robot->getKills() < 4){
                    it = robotsGlobal.erase(it);
                    upgradeBot(robot);
                }
                displayBattlefield();
                i++;
                it++;
            }
            else if (robot->isDead){
                robotsQueueGlobal.push_back(robot);
                it = robotsGlobal.erase(it);
            }
            if (!robotsQueueGlobal.empty()){
            Robot *respawnBot = robotsQueueGlobal.front();
            robotsQueueGlobal.erase(robotsQueueGlobal.begin());
            if (respawnBot->robotLife > 0){
                int x = rand() % getCol();
                int y = rand() % getRow();
                respawnBot->setPosX(x);respawnBot->setPosY(y);
                respawnBot->isDead = false;
                robotsGlobal.push_back(respawnBot);
                cout << respawnBot->getrobotSymbol() << " has rejoined the fray at " << respawnBot;
                outfile << respawnBot->getrobotSymbol() << " has rejoined the fray at " << respawnBot;
            }
            else{
                cout << respawnBot->getrobotSymbol() << " has been permanently damaged." << endl;
                outfile << respawnBot->getrobotSymbol() << " has been permanently damaged." << endl;
                respawnBot = nullptr;
                }
            }
        }
    }
    cout << "Max Steps Reached" << endl;
    outfile << "Max Steps Reached" << endl;
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
                                if (target->ishiding == true){
                                    cout << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                                    outfile << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                                }
                                else{
                                    cout << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                    outfile << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                    target->setLife();
                                    robotKills++;
                                    break;   
                                }
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
                    setLife();
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
                    if (other != this && other->getPosX() == newpos_x && other->getPosY() == newpos_y && other->isDead == false) {
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
                            if (other != this && other->getPosX() == nx && other->getPosY() == ny && other->isDead == false) {
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

class AdvanceMoveBot : public MovingRobot, public SeeingRobot, public ShootingRobot, public ThinkingRobot{
    public:
    AdvanceMoveBot(string type,string name, int x, int y) : Robot(type,name,x,y){
        robotLife = 3;upgradeCounterShoot++;upgradeCounterSee++;upgradeCounterMove++;
    }
        int advancedmovecount=2;
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
                                if (target->ishiding == true){
                                    cout << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                                    outfile << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                                }
                                else{
                                    cout << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                    outfile << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                    target->setLife();
                                    robotKills++;
                                    break;   
                                }
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
                    setLife();
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

            if (advancedmovecount>0){
                advancedmovecount--;
                if(enemyFound){
                dx = (enemyX > getPosX()) ? 3 : (enemyX < getPosX()) ? -3 : 0;
                dy = (enemyY > getPosY()) ? 3 : (enemyY < getPosY()) ? -3 : 0;
                cout << robotSymbol << " moves towards enemy robot at ("<<enemyX<<","<<enemyY<<")"<< endl;
                outfile << robotSymbol << " moves towards enemy robot at ("<<enemyX<<","<<enemyY<<")"<< endl;
            } else {
                dx = setdx(col);
                dy = setdy(row);
            }
            
            newpos_x = getPosX() + (dx > 0 ? 3 : (dx < 0 ? -3 : 0));
            newpos_y = getPosY() + (dy > 0 ? 3 : (dy < 0 ? -3 : 0));

            if(newpos_x >=0 && newpos_x<col && newpos_y>=0 && newpos_y<row){
                bool occupied = false;
                for (Robot* other : Battlefield::robotsGlobal) { 
                    if (other != this && other->getPosX() == newpos_x && other->getPosY() == newpos_y && other->isDead == false) {
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
             cout << "The advancedmovecount" << advancedmovecount  << endl;
            }
            else{
                if (enemyFound){
                    dx = (enemyX > getPosX()) ? 1 : (enemyX < getPosX()) ? -1 : 0;
                    dy = (enemyY > getPosY()) ? 1 : (enemyY < getPosY()) ? -1 : 0;
                    cout << getrobotSymbol() << " moves towards enemy robot at (" << enemyX << "," << enemyY << ")" << endl;
                    outfile << getrobotSymbol() << " moves towards enemy robot at (" << enemyX << "," << enemyY << ")" << endl;
                }
            }
            
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
                    if (other != this && other->getPosX() == newpos_x && other->getPosY() == newpos_y && other->isDead == false) {
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
                            if (other != this && other->getPosX() == nx && other->getPosY() == ny && other->isDead == false) {
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

class HideBot : public MovingRobot, public SeeingRobot, public ShootingRobot, public ThinkingRobot{
    public:
        HideBot(string type,string name, int x, int y) : Robot(type,name,x,y){
            shells=10;
            robotLife = 3;upgradeCounterShoot++;upgradeCounterSee++;upgradeCounterMove++;
        }
        

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
                                if (target->ishiding == true){
                                    cout << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                                    outfile << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                                }
                                else{
                                    cout << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                    outfile << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                    target->setLife();
                                    robotKills++;
                                    break;   
                                }
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
                    setLife();
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
            if (hidecount>0){
                ishiding=true;
                hidecount--;
                cout<< "Robot " << getrobotSymbol() << " is hiding" << endl;
                cout << "Remaining hidecount: " << hidecount << endl;
                outfile << "Robot " << getrobotSymbol() << " is hiding" << endl;
                outfile << "Remaining hidecount: " << hidecount << endl;
            }
            else{
                ishiding = false;
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
                    if (other != this && other->getPosX() == newpos_x && other->getPosY() == newpos_y && other->isDead == false) {
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
        }
        
        
        void see(int x, int y,int col,int row) override {
            int centerX = getPosX() + x;
            int centerY = getPosY() + y;
            enemyFound = false;
            
            cout << "Robot " << robotSymbol << " is looking around (" << centerX << ", " << centerY << "):\n";
            outfile << "Robot " << robotSymbol << " is looking around (" << centerX << ", " << centerY << "):\n";
            
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int nx = centerX + dx;
                    int ny = centerY + dy;
        
                    if (nx >= 0 && nx < col && ny >= 0 && ny < row) { 
                        for (Robot* other : Battlefield::robotsGlobal) { 
                            if (other != this && other->getPosX() == nx && other->getPosY() == ny && other->isDead == false) {
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

class JumpBot : public MovingRobot, public SeeingRobot, public ShootingRobot, public ThinkingRobot{
    public:
        JumpBot(string type,string name, int x, int y) : Robot(type,name,x,y){
            shells=10;
            robotLife = 3;upgradeCounterShoot++;upgradeCounterSee++;upgradeCounterMove++;
        }

        int jumpcount = 3;
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
                                if (target->ishiding == true){
                                    cout << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                                    outfile << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                                }
                                else{
                                    cout << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                    outfile << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                    target->setLife();
                                    robotKills++;
                                    break;   
                                }
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
                    setLife();
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
            if (jumpcount>0){
                cout << "The jumpBot will activate now" << endl;
                outfile << "The jumpBot will activate now" << endl;
                newpos_x=rand() % col;
                newpos_y = rand() % row;
                if(newpos_x >=0 && newpos_x<col && newpos_y>=0 && newpos_y<row){
                setPosX(newpos_x);
                setPosY(newpos_y);
                cout << "jumped to" << newpos_x <<","<<newpos_y<<endl;
                outfile << "jumped to " << newpos_x << "," << newpos_y << endl;
                 }
                 else{
                    cout << "could not jump beyond boundary" << endl;
                    outfile << "could not jump beyond boundary" << endl;
                 }
                jumpcount--;
                cout << "The remaining jumps are " << jumpcount << endl;
                outfile << "The remaining jumps are " << jumpcount<< endl;
            }
           else{
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
                    if (other != this && other->getPosX() == newpos_x && other->getPosY() == newpos_y && other->isDead == false) {
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
        }
        
        void see(int x, int y,int col,int row) override {
            int centerX = getPosX() + x;
            int centerY = getPosY() + y;
            enemyFound = false;
            
            cout << "Robot " << robotSymbol << " is looking around (" << centerX << ", " << centerY << "):\n";
            outfile << "Robot " << robotSymbol << " is looking around (" << centerX << ", " << centerY << "):\n";
            
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int nx = centerX + dx;
                    int ny = centerY + dy;
        
                    if (nx >= 0 && nx < col && ny >= 0 && ny < row) { 
                        for (Robot* other : Battlefield::robotsGlobal) { 
                            if (other != this && other->getPosX() == nx && other->getPosY() == ny && other->isDead == false) {
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

class LongShotBot : public MovingRobot, public SeeingRobot, public ShootingRobot, public ThinkingRobot {
    public:
        LongShotBot(string type, string name, int x, int y) : Robot(type, name, x, y) {upgradeCounterShoot++;}

        void shoot(int x, int y) override {
            lookCounter--;
            if (shells > 0) {
                cout << robotSymbol << " shoots at (" << x << ", " << y << ")" << endl;
                outfile << robotSymbol << " shoots at (" << x << ", " << y << ")" << endl;
                shells--;
                
                int dx = abs(getPosX() - x);
                int dy = abs(getPosY() - y);
                if (dx <= 3 && dy <= 3) {
                    if ((rand() % 100) < 70) {
                        for (Robot* target : Battlefield::robotsGlobal) {
                            if (target != this && target->getPosX() == x && target->getPosY() == y) {
                                if (target->ishiding == true) {
                                    cout << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                                    outfile << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                                }
                                else {
                                    cout << "Long range hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                    outfile << "Long range hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                    target->setLife();
                                    robotKills++;
                                    break;   
                                }
                            }
                        }
                    } else {
                        cout << "Long range shot missed the target!" << endl;
                        outfile << "Long range shot missed the target!" << endl;
                    }
                } else {
                    cout << "Target not in range, shot missed!" << endl;
                    outfile << "Target not in range, shot missed!" << endl;
                }
                
                if (shells == 0) {
                    cout << robotSymbol << " is out of ammo and self-destructs!" << endl;
                    outfile << robotSymbol << " is out of ammo and self-destructs!" << endl;
                    setLife();
                }
            } else {
                cout << robotSymbol << " has no shells left!" << endl;
                outfile << robotSymbol << " has no shells left!" << endl;
            }
        }
        
        void think(int col, int row) override {
            int x = abs(getPosX() - col);
            int y = abs(getPosY() - row);
            if (lookCounter == 0) {
                see(0, 0, col, row);
                lookCounter++;
                return;
            }
            else {
                if (enemyFound && abs(getPosX() - enemyX) <= 3 && abs(getPosY() - enemyY) <= 3) {
                    shoot(enemyX, enemyY);
                    return;
                }
                else {
                    move(col, row);
                    return;
                }
            }
        }
        
        void move(int col, int row) override {
            lookCounter--;
            cout << "Robot " << getrobotSymbol() << " starting at (" << getPosX() << "," << getPosY() << ")" << endl;
            outfile << "Robot " << getrobotSymbol() << " starting at (" << getPosX() << "," << getPosY() << ")" << endl;
            
            if (enemyFound) {
                dx = (enemyX > getPosX()) ? 1 : (enemyX < getPosX()) ? -1 : 0;
                dy = (enemyY > getPosY()) ? 1 : (enemyY < getPosY()) ? -1 : 0;
                cout << robotSymbol << " moves towards enemy robot at (" << enemyX << "," << enemyY << ")" << endl;
                outfile << robotSymbol << " moves towards enemy robot at (" << enemyX << "," << enemyY << ")" << endl;
            } else {
                dx = setdx(col);
                dy = setdy(row);
            }
            
            newpos_x = getPosX() + (dx > 0 ? 1 : (dx < 0 ? -1 : 0));
            newpos_y = getPosY() + (dy > 0 ? 1 : (dy < 0 ? -1 : 0));

            if (newpos_x >= 0 && newpos_x < col && newpos_y >= 0 && newpos_y < row) {
                bool occupied = false;
                for (Robot* other : Battlefield::robotsGlobal) { 
                    if (other != this && other->getPosX() == newpos_x && other->getPosY() == newpos_y && other->isDead == false) {
                        occupied = true;
                        // Check if within shooting range (3 spaces)
                        int adj_dx = abs(getPosX() - newpos_x);
                        int adj_dy = abs(getPosY() - newpos_y);
                        if (adj_dx <= 3 && adj_dy <= 3) {
                            shoot(newpos_x, newpos_y);
                            return;
                        }
                        break;
                    }
                }
                
                if (!occupied) {
                    setPosX(newpos_x);
                    setPosY(newpos_y);
                    cout << "Robot " << getrobotSymbol() << " moved to (" << getPosX() << "," << getPosY() << ")" << endl;
                    outfile << "Robot " << getrobotSymbol() << " moved to (" << getPosX() << "," << getPosY() << ")" << endl;
                }
                else {
                    cout << "Robot would not move to already occupied space" << endl;
                    outfile << "Robot would not move to already occupied space" << endl;
                }
            }
        }
        
        void see(int x, int y, int col, int row) override {
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
                            if (other != this && other->getPosX() == nx && other->getPosY() == ny && other->isDead == false) {
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

class SemiAutoBot : public MovingRobot, public SeeingRobot, public ShootingRobot, public ThinkingRobot {
public:
    SemiAutoBot(string type, string name, int x, int y) : Robot(type, name, x, y) {
        shells = 10; // Standard number of shells
        robotLife = 3;
        upgradeCounterShoot++;
    }

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
                                if (target->ishiding) {
                                    cout << getrobotSymbol() << " fired a burst but " << target->getrobotSymbol() << " successfully hid from all the shots" << endl;
                                    outfile << getrobotSymbol() << " fired a burst but " << target->getrobotSymbol() << " successfully hid from all the shots" << endl;
                                }
                                else if (!targetDestroyed) {
                                    cout << "Burst shot hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                    outfile << "Burst shot hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                    target->setLife();
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
                setLife();
            }
        } else {
            cout << robotSymbol << " has no shells left!" << endl;
            outfile << robotSymbol << " has no shells left!" << endl;
        }
    }
    
    void think(int col, int row) override {
        int x = abs(getPosX() - col);
        int y = abs(getPosY() - row);
        if (lookCounter == 0) {
            see(0, 0, col, row);
            lookCounter++;
            return;
        }
        else {
            if (x <= 1 && y <= 1) {
                shoot(col, row);
                return;
            }
            else {
                move(col, row);
                return;
            }
        }
    }
    
    void move(int col, int row) override {
        lookCounter--;
        cout << "Robot " << getrobotSymbol() << " starting at (" << getPosX() << "," << getPosY() << ")" << endl;
        outfile << "Robot " << getrobotSymbol() << " starting at (" << getPosX() << "," << getPosY() << ")" << endl;
        
        if(enemyFound) {
            dx = (enemyX > getPosX()) ? 1 : (enemyX < getPosX()) ? -1 : 0;
            dy = (enemyY > getPosY()) ? 1 : (enemyY < getPosY()) ? -1 : 0;
            cout << robotSymbol << " moves towards enemy robot at (" << enemyX << "," << enemyY << ")" << endl;
            outfile << robotSymbol << " moves towards enemy robot at (" << enemyX << "," << enemyY << ")" << endl;
        } else {
            dx = setdx(col);
            dy = setdy(row);
        }
        
        newpos_x = getPosX() + (dx > 0 ? 1 : (dx < 0 ? -1 : 0));
        newpos_y = getPosY() + (dy > 0 ? 1 : (dy < 0 ? -1 : 0));

        if(newpos_x >= 0 && newpos_x < col && newpos_y >= 0 && newpos_y < row) {
            bool occupied = false;
            for (Robot* other : Battlefield::robotsGlobal) { 
                if (other != this && other->getPosX() == newpos_x && other->getPosY() == newpos_y && other->isDead == false) {
                    occupied = true;
                    int adj_dx = abs(getPosX() - newpos_x);
                    int adj_dy = abs(getPosY() - newpos_y);
                    if (adj_dx <= 1 && adj_dy <= 1) {
                        shoot(newpos_x, newpos_y);
                        return;
                    }
                    break;
                }
            }
            
            if(!occupied) {
                setPosX(newpos_x);
                setPosY(newpos_y);
                cout << "Robot " << getrobotSymbol() << " moved to (" << getPosX() << "," << getPosY() << ")" << endl;
                outfile << "Robot " << getrobotSymbol() << " moved to (" << getPosX() << "," << getPosY() << ")" << endl;
            }
            else {
                cout << "Robot would not move to already occupied space" << endl;
                outfile << "Robot would not move to already occupied space" << endl;
            }
        }
    }
    
    void see(int x, int y, int col, int row) override {
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
                        if (other != this && other->getPosX() == nx && other->getPosY() == ny && other->isDead == false) {
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

private:
    int setdx(int col) {
        return (rand() % col) - (col / 2);
    }

    int setdy(int row) {
        return (rand() % row) - (row / 2);
    }
};

class ThirtyShotBot : public MovingRobot, public SeeingRobot, public ShootingRobot, public ThinkingRobot {
public:
    ThirtyShotBot(string type, string name, int x, int y) : Robot(type, name, x, y) {
        shells = 30; // Increased ammo capacity (30 instead of 10)
        upgradeCounterShoot++;
    }

    void shoot(int x, int y) override {
        lookCounter--;
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
                            if (target->ishiding == true) {
                                cout << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                                outfile << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                            }
                            else {
                                cout << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                outfile << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                target->setLife();
                                robotKills++;
                                break;   
                            }
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
                setLife();
            }
        } else {
            cout << robotSymbol << " has no shells left!" << endl;
            outfile << robotSymbol << " has no shells left!" << endl;
        }
    }
    
    void think(int col, int row) override {
        int x = abs(getPosX() - col);
        int y = abs(getPosY() - row);
        if (lookCounter == 0) {
            see(0, 0, col, row);
            lookCounter++;
            return;
        }
        else {
            if (x <= 1 && y <= 1) {
                shoot(col, row);
                return;
            }
            else {
                move(col, row);
                return;
            }
        }
    }
    
    void move(int col, int row) override {
        lookCounter--;
        cout << "Robot " << getrobotSymbol() << " starting at (" << getPosX() << "," << getPosY() << ")" << endl;
        outfile << "Robot " << getrobotSymbol() << " starting at (" << getPosX() << "," << getPosY() << ")" << endl;
        
        if(enemyFound) {
            dx = (enemyX > getPosX()) ? 1 : (enemyX < getPosX()) ? -1 : 0;
            dy = (enemyY > getPosY()) ? 1 : (enemyY < getPosY()) ? -1 : 0;
            cout << robotSymbol << " moves towards enemy robot at (" << enemyX << "," << enemyY << ")" << endl;
            outfile << robotSymbol << " moves towards enemy robot at (" << enemyX << "," << enemyY << ")" << endl;
        } else {
            dx = setdx(col);
            dy = setdy(row);
        }
        
        newpos_x = getPosX() + (dx > 0 ? 1 : (dx < 0 ? -1 : 0));
        newpos_y = getPosY() + (dy > 0 ? 1 : (dy < 0 ? -1 : 0));

        if(newpos_x >= 0 && newpos_x < col && newpos_y >= 0 && newpos_y < row) {
            bool occupied = false;
            for (Robot* other : Battlefield::robotsGlobal) { 
                if (other != this && other->getPosX() == newpos_x && other->getPosY() == newpos_y && other->isDead == false) {
                    occupied = true;
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
            
            if(!occupied) {
                setPosX(newpos_x);
                setPosY(newpos_y);
                cout << "Robot " << getrobotSymbol() << " moved to (" << getPosX() << "," << getPosY() << ")" << endl;
                outfile << "Robot " << getrobotSymbol() << " moved to (" << getPosX() << "," << getPosY() << ")" << endl;
            }
            else {
                cout << "Robot would not move to already occupied space" << endl;
                outfile << "Robot would not move to already occupied space" << endl;
            }
        }
    }
    
    void see(int x, int y, int col, int row) override {
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
                        if (other != this && other->getPosX() == nx && other->getPosY() == ny && other->isDead == false) {
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

private:
    int setdx(int col) {
        return (rand() % col) - (col / 2);
    }

    int setdy(int row) {
        return (rand() % row) - (row / 2);
    }
};

class StealBot : public MovingRobot, public SeeingRobot, public ShootingRobot, public ThinkingRobot {
public:
    StealBot(string type, string name, int x, int y) : Robot(type, name, x, y) {
        shells = 10;  // Starting ammo
        maxShells = 30;  // Maximum ammo capacity
        upgradeCounterShoot++;
    }

    void shoot(int x, int y) override {
        lookCounter--;
        if (shells > 0) {
            cout << robotSymbol << " shoots at (" << x << ", " << y << ")" << endl;
            outfile << robotSymbol << " shoots at (" << x << ", " << y << ")" << endl;
            shells--;
            
            int dx = abs(getPosX() - x);
            int dy = abs(getPosY() - y);
            if (dx <= 1 && dy <= 1) {
                if ((rand() % 100) < 70) {
                    for (Robot* target : Battlefield::robotsGlobal) {
                        if (target != this && target->getPosX() == x && target->getPosY() == y) {
                            if (target->ishiding == true) {
                                cout << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                                outfile << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                            }
                            else {
                                cout << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                outfile << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                
                                // Ammo stealing logic
                                if (ShootingRobot* shootingTarget = dynamic_cast<ShootingRobot*>(target)) {
                                    int stolenAmmo = shootingTarget->getShells();
                                    if (stolenAmmo > 0) {
                                        shells += stolenAmmo;
                                        if (shells > maxShells) shells = maxShells;
                                        cout << robotSymbol << " stole " << stolenAmmo << " shells! Now has " << shells << " shells." << endl;
                                        outfile << robotSymbol << " stole " << stolenAmmo << " shells! Now has " << shells << " shells." << endl;
                                    }
                                }
                                
                                target->setLife();
                                robotKills++;
                                break;   
                            }
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
                setLife();
            }
        } else {
            cout << robotSymbol << " has no shells left!" << endl;
            outfile << robotSymbol << " has no shells left!" << endl;
        }
    }
    
    void think(int col, int row) override {
        int x = abs(getPosX() - col);
        int y = abs(getPosY() - row);
        if (lookCounter == 0) {
            see(0, 0, col, row);
            lookCounter++;
            return;
        }
        else {
            if (x <= 1 && y <= 1) {
                shoot(col, row);
                return;
            }
            else {
                move(col, row);
                return;
            }
        }
    }
    
    void move(int col, int row) override {
        lookCounter--;
        cout << "Robot " << getrobotSymbol() << " starting at (" << getPosX() << "," << getPosY() << ")" << endl;
        outfile << "Robot " << getrobotSymbol() << " starting at (" << getPosX() << "," << getPosY() << ")" << endl;
        
        if(enemyFound) {
            dx = (enemyX > getPosX()) ? 1 : (enemyX < getPosX()) ? -1 : 0;
            dy = (enemyY > getPosY()) ? 1 : (enemyY < getPosY()) ? -1 : 0;
            cout << robotSymbol << " moves towards enemy robot at (" << enemyX << "," << enemyY << ")" << endl;
            outfile << robotSymbol << " moves towards enemy robot at (" << enemyX << "," << enemyY << ")" << endl;
        } else {
            dx = setdx(col);
            dy = setdy(row);
        }
        
        newpos_x = getPosX() + (dx > 0 ? 1 : (dx < 0 ? -1 : 0));
        newpos_y = getPosY() + (dy > 0 ? 1 : (dy < 0 ? -1 : 0));

        if(newpos_x >= 0 && newpos_x < col && newpos_y >= 0 && newpos_y < row) {
            bool occupied = false;
            for (Robot* other : Battlefield::robotsGlobal) { 
                if (other != this && other->getPosX() == newpos_x && other->getPosY() == newpos_y && other->isDead == false) {
                    occupied = true;
                    int adj_dx = abs(getPosX() - newpos_x);
                    int adj_dy = abs(getPosY() - newpos_y);
                    if (adj_dx <= 1 && adj_dy <= 1) {
                        shoot(newpos_x, newpos_y);
                        return;
                    }
                    break;
                }
            }
            
            if(!occupied) {
                setPosX(newpos_x);
                setPosY(newpos_y);
                cout << "Robot " << getrobotSymbol() << " moved to (" << getPosX() << "," << getPosY() << ")" << endl;
                outfile << "Robot " << getrobotSymbol() << " moved to (" << getPosX() << "," << getPosY() << ")" << endl;
            }
            else {
                cout << "Robot would not move to already occupied space" << endl;
                outfile << "Robot would not move to already occupied space" << endl;
            }
        }
    }
    
    void see(int x, int y, int col, int row) override {
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
                        if (other != this && other->getPosX() == nx && other->getPosY() == ny && other->isDead == false) {
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

private:
    int maxShells; 
    int setdx(int col) {
        return (rand() % col) - (col / 2);
    }

    int setdy(int row) {
        return (rand() % row) - (row / 2);
    }
};

class ScoutBot : public MovingRobot, public SeeingRobot, public ShootingRobot, public ThinkingRobot {
public:
    int enemyX = -1;
    int enemyY = -1;
    int lookCounter = 0;
    bool enemyFound = false;
    int newpos_x;
    int newpos_y;
    int dx;
    int dy;
    int scoutUses = 3;  // Can use scout ability 3 times

    ScoutBot(string type, string name, int x, int y) : Robot(type, name, x, y) {
        shells = 10;
        robotLife = 3;upgradeCounterShoot++;upgradeCounterSee++;
    }

    void shoot(int x, int y) override {
        lookCounter--;
        if (shells > 0) {
            cout << robotSymbol << " shoots at (" << x << ", " << y << ")" << endl;
            outfile << robotSymbol << " shoots at (" << x << ", " << y << ")" << endl;
            shells--;
            
            int dx = abs(getPosX() - x);
            int dy = abs(getPosY() - y);
            if (dx <= 1 && dy <= 1) {
                if ((rand() % 100) < 70) {
                    for (Robot* target : Battlefield::robotsGlobal) {
                        if (target != this && target->getPosX() == x && target->getPosY() == y) {
                                if (target->ishiding == true){
                                    cout << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                                    outfile << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                                }
                                else{
                                    cout << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                    outfile << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                    target->setLife();
                                    robotKills++;
                                    break;   
                                }
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
                setLife();
            }
        } else {
            cout << robotSymbol << " has no shells left!" << endl;
            outfile << robotSymbol << " has no shells left!" << endl;
        }
    }

    void move(int col, int row) override {
        lookCounter--;
        cout << "Robot " << getrobotSymbol() << " starting at (" << getPosX() << "," << getPosY() << ")" << endl;
        outfile << "Robot " << getrobotSymbol() << " starting at (" << getPosX() << "," << getPosY() << ")" << endl;
        
        if(enemyFound) {
            dx = (enemyX > getPosX()) ? 1 : (enemyX < getPosX()) ? -1 : 0;
            dy = (enemyY > getPosY()) ? 1 : (enemyY < getPosY()) ? -1 : 0;
            cout << robotSymbol << " moves towards enemy robot at (" << enemyX << "," << enemyY << ")" << endl;
            outfile << robotSymbol << " moves towards enemy robot at (" << enemyX << "," << enemyY << ")" << endl;
        } else {
            dx = setdx(col);
            dy = setdy(row);
        }
        
        newpos_x = getPosX() + (dx > 0 ? 1 : (dx < 0 ? -1 : 0));
        newpos_y = getPosY() + (dy > 0 ? 1 : (dy < 0 ? -1 : 0));

        if(newpos_x >= 0 && newpos_x < col && newpos_y >= 0 && newpos_y < row) {
            bool occupied = false;
            for (Robot* other : Battlefield::robotsGlobal) { 
                if (other != this && other->getPosX() == newpos_x && other->getPosY() == newpos_y && other->isDead == false) {
                    occupied = true;
                    int adj_dx = abs(getPosX() - newpos_x);
                    int adj_dy = abs(getPosY() - newpos_y);
                    if (adj_dx <= 1 && adj_dy <= 1) {
                        shoot(newpos_x, newpos_y);
                        return;
                    }
                    break;
                }
            }
            
            if(!occupied) {
                setPosX(newpos_x);
                setPosY(newpos_y);
                cout << "Robot " << getrobotSymbol() << " moved to (" << getPosX() << "," << getPosY() << ")" << endl;
                outfile << "Robot " << getrobotSymbol() << " moved to (" << getPosX() << "," << getPosY() << ")" << endl;
            } else {
                cout << "Robot would not move to already occupied space" << endl;
                outfile << "Robot would not move to already occupied space" << endl;
            }
        }
    }

    void see(int x, int y, int col, int row) override {
        // Check if we should use scout ability
        if (scoutUses > 0 && (x != 0 || y != 0)) {
            cout << robotSymbol << " is activating Scout ability to view entire battlefield!" << endl;
            outfile << robotSymbol << " is activating Scout ability to view entire battlefield!" << endl;
            scoutUses--;
            
            enemyFound = false;
            cout << "Robot " << robotSymbol << " can see the entire battlefield:\n";
            outfile << "Robot " << robotSymbol << " can see the entire battlefield:\n";
            
            // Scan the entire battlefield
            for (int ny = 0; ny < row; ny++) {
                for (int nx = 0; nx < col; nx++) {
                    if (nx == getPosX() && ny == getPosY()) continue; // Skip self
                    
                    for (Robot* other : Battlefield::robotsGlobal) { 
                        if (other != this && other->getPosX() == nx && other->getPosY() == ny && other->isDead == false) {
                            if (!enemyFound) {
                                enemyX = nx;
                                enemyY = ny;
                                enemyFound = true;
                            }
                            cout << "Enemy robot found at (" << nx << ", " << ny << ") with symbol: " << other->getrobotSymbol() << "\n";
                            outfile << "Enemy robot found at (" << nx << ", " << ny << ") with symbol: " << other->getrobotSymbol() << "\n";
                            break;
                        }
                    }
                }
            }
            
            if (!enemyFound) {
                cout << "No enemy robots found on battlefield." << endl;
                outfile << "No enemy robots found on battlefield." << endl;
            }
            return;
        }
        
        // Normal see functionality when not using scout ability
        int centerX = getPosX() + x;
        int centerY = getPosY() + y;
        enemyFound = false;
        
        cout << "Robot " << robotSymbol << " is looking around (" << centerX << ", " << centerY << "):\n";
        outfile << "Robot " << robotSymbol << " is looking around (" << centerX << ", " << centerY << "):\n";
        
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int nx = centerX + dx;
                int ny = centerY + dy;
    
                if (nx >= 0 && nx < col && ny >= 0 && ny < row) { 
                    for (Robot* other : Battlefield::robotsGlobal) { 
                        if (other != this && other->getPosX() == nx && other->getPosY() == ny && other->isDead == false) {
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

    void think(int col, int row) override {
        int x = abs(getPosX() - col);
        int y = abs(getPosY() - row);
        
        // Decision to use scout ability
        if (scoutUses > 0 && rand() % 100 < 30) { // 30% chance to use scout when available
            see(col, row, col, row); // Passing col,row as x,y to trigger scout mode
            return;
        }
        
        if (lookCounter == 0) {
            see(0, 0, col, row);
            lookCounter++;
            return;
        } else {
            if (x <= 1 && y <= 1) {
                shoot(col, row);
                return;
            } else {
                move(col, row);
                return;
            }
        }
    }

    int setdx(int col) {
        return (rand() % col) - (col / 2);
    }

    int setdy(int row) {
        return (rand() % row) - (row / 2);
    }
};

class TrackBot : public MovingRobot, public SeeingRobot, public ShootingRobot, public ThinkingRobot {
private:
    struct Tracker {
        Robot* target;
        bool active;
    };
    Tracker trackers[3];  // Array to hold 3 trackers
    int trackersUsed = 0; // Count of trackers currently in use

public:
    int enemyX = -1;
    int enemyY = -1;
    int lookCounter = 0;
    bool enemyFound = false;
    int newpos_x;
    int newpos_y;
    int dx;
    int dy;

    TrackBot(string type, string name, int x, int y) : Robot(type, name, x, y) {
        shells = 10;
        robotLife = 3;upgradeCounterShoot++;upgradeCounterSee++;
        // Initialize all trackers as inactive
        for (int i = 0; i < 3; i++) {
            trackers[i].target = nullptr;
            trackers[i].active = false;
        }
    }

    void shoot(int x, int y) override {
        lookCounter--;
        if (shells > 0) {
            cout << robotSymbol << " shoots at (" << x << ", " << y << ")" << endl;
            outfile << robotSymbol << " shoots at (" << x << ", " << y << ")" << endl;
            shells--;
            
            int dx = abs(getPosX() - x);
            int dy = abs(getPosY() - y);
            if (dx <= 1 && dy <= 1) {
                if ((rand() % 100) < 70) {
                    for (Robot* target : Battlefield::robotsGlobal) {
                        if (target != this && target->getPosX() == x && target->getPosY() == y) {
                            if (target->ishiding){
                                cout << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                                outfile << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                            }
                            else{
                                cout << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                            outfile << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                            
                            // Remove any trackers on this target
                            for (int i = 0; i < 3; i++) {
                                if (trackers[i].active && trackers[i].target == target) {
                                    trackers[i].active = false;
                                    trackers[i].target = nullptr;
                                    trackersUsed--;
                                    cout << "Tracker on " << target->getrobotSymbol() << " was removed due to destruction." << endl;
                                    outfile << "Tracker on " << target->getrobotSymbol() << " was removed due to destruction." << endl;
                                }
                            }
                            
                                target->setLife();
                                robotKills++;
                                break;
                            }
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
                setLife();
            }
        } else {
            cout << robotSymbol << " has no shells left!" << endl;
            outfile << robotSymbol << " has no shells left!" << endl;
        }
    }

    void move(int col, int row) override {
        lookCounter--;
        cout << "Robot " << getrobotSymbol() << " starting at (" << getPosX() << "," << getPosY() << ")" << endl;
        outfile << "Robot " << getrobotSymbol() << " starting at (" << getPosX() << "," << getPosY() << ")" << endl;
        
        // Check if we have any active trackers
        bool hasTrackedTarget = false;
        for (int i = 0; i < 3; i++) {
            if (trackers[i].active && !trackers[i].target->isDead) {
                enemyX = trackers[i].target->getPosX();
                enemyY = trackers[i].target->getPosY();
                hasTrackedTarget = true;
                cout << robotSymbol << " is tracking " << trackers[i].target->getrobotSymbol() 
                     << " at (" << enemyX << "," << enemyY << ")" << endl;
                outfile << robotSymbol << " is tracking " << trackers[i].target->getrobotSymbol() 
                       << " at (" << enemyX << "," << enemyY << ")" << endl;
                break;
            }
        }
        
        if(hasTrackedTarget) {
            dx = (enemyX > getPosX()) ? 1 : (enemyX < getPosX()) ? -1 : 0;
            dy = (enemyY > getPosY()) ? 1 : (enemyY < getPosY()) ? -1 : 0;
            cout << robotSymbol << " moves towards tracked enemy robot at (" << enemyX << "," << enemyY << ")" << endl;
            outfile << robotSymbol << " moves towards tracked enemy robot at (" << enemyX << "," << enemyY << ")" << endl;
        } else if(enemyFound) {
            dx = (enemyX > getPosX()) ? 1 : (enemyX < getPosX()) ? -1 : 0;
            dy = (enemyY > getPosY()) ? 1 : (enemyY < getPosY()) ? -1 : 0;
            cout << robotSymbol << " moves towards enemy robot at (" << enemyX << "," << enemyY << ")" << endl;
            outfile << robotSymbol << " moves towards enemy robot at (" << enemyX << "," << enemyY << ")" << endl;
        } else {
            dx = setdx(col);
            dy = setdy(row);
        }
        
        newpos_x = getPosX() + (dx > 0 ? 1 : (dx < 0 ? -1 : 0));
        newpos_y = getPosY() + (dy > 0 ? 1 : (dy < 0 ? -1 : 0));

        if(newpos_x >= 0 && newpos_x < col && newpos_y >= 0 && newpos_y < row) {
            bool occupied = false;
            for (Robot* other : Battlefield::robotsGlobal) { 
                if (other != this && other->getPosX() == newpos_x && other->getPosY() == newpos_y && other->isDead == false) {
                    occupied = true;
                    int adj_dx = abs(getPosX() - newpos_x);
                    int adj_dy = abs(getPosY() - newpos_y);
                    if (adj_dx <= 1 && adj_dy <= 1) {
                        shoot(newpos_x, newpos_y);
                        return;
                    }
                    break;
                }
            }
            
            if(!occupied) {
                setPosX(newpos_x);
                setPosY(newpos_y);
                cout << "Robot " << getrobotSymbol() << " moved to (" << getPosX() << "," << getPosY() << ")" << endl;
                outfile << "Robot " << getrobotSymbol() << " moved to (" << getPosX() << "," << getPosY() << ")" << endl;
            } else {
                cout << "Robot would not move to already occupied space" << endl;
                outfile << "Robot would not move to already occupied space" << endl;
            }
        }
    }

    void see(int x, int y, int col, int row) override {
        int centerX = getPosX() + x;
        int centerY = getPosY() + y;
        enemyFound = false;
        
        cout << "Robot " << robotSymbol << " is looking around (" << centerX << ", " << centerY << "):\n";
        outfile << "Robot " << robotSymbol << " is looking around (" << centerX << ", " << centerY << "):\n";
        
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int nx = centerX + dx;
                int ny = centerY + dy;
    
                if (nx >= 0 && nx < col && ny >= 0 && ny < row) { 
                    for (Robot* other : Battlefield::robotsGlobal) { 
                        if (other != this && other->getPosX() == nx && other->getPosY() == ny && other->isDead == false) {
                            enemyX = nx;
                            enemyY = ny;
                            enemyFound = true;
                            cout << "Enemy robot found at (" << nx << ", " << ny << ") with symbol: " << other->getrobotSymbol() << "\n";
                            outfile << "Enemy robot found at (" << nx << ", " << ny << ") with symbol: " << other->getrobotSymbol() << "\n";
                            
                            // Try to plant a tracker if we have any left
                            if (trackersUsed < 3) {
                                for (int i = 0; i < 3; i++) {
                                    if (!trackers[i].active) {
                                        // Check if we're not already tracking this robot
                                        bool alreadyTracking = false;
                                        for (int j = 0; j < 3; j++) {
                                            if (trackers[j].active && trackers[j].target == other) {
                                                alreadyTracking = true;
                                                break;
                                            }
                                        }
                                        
                                        if (!alreadyTracking) {
                                            trackers[i].target = other;
                                            trackers[i].active = true;
                                            trackersUsed++;
                                            cout << "Tracker planted on " << other->getrobotSymbol() << " at (" 
                                                 << nx << "," << ny << ")" << endl;
                                            outfile << "Tracker planted on " << other->getrobotSymbol() << " at (" 
                                                   << nx << "," << ny << ")" << endl;
                                            break;
                                        }
                                    }
                                }
                            }
                            break;
                        }
                    }
                } else {
                    cout << "(" << nx << ", " << ny << ") is out of battlefield bounds.\n";
                    outfile << "(" << nx << ", " << ny << ") is out of battlefield bounds.\n";
                }
            }
        }
        
        // Display current tracking status
        if (trackersUsed > 0) {
            cout << robotSymbol << " is currently tracking:" << endl;
            outfile << robotSymbol << " is currently tracking:" << endl;
            for (int i = 0; i < 3; i++) {
                if (trackers[i].active && !trackers[i].target->isDead) {
                    cout << "  " << trackers[i].target->getrobotSymbol() << " at " << trackers[i].target; 
                    outfile << "  " << trackers[i].target->getrobotSymbol() << " at " << trackers[i].target;
                }
            }
        }
    }

    void think(int col, int row) override {
        int x = abs(getPosX() - col);
        int y = abs(getPosY() - row);
        if (lookCounter == 0) {
            see(0, 0, col, row);
            lookCounter++;
            return;
        } else {
            if (x <= 1 && y <= 1) {
                shoot(col, row);
                return;
            } else {
                move(col, row);
                return;
            }
        }
    }

    int setdx(int col) {
        return (rand() % col) - (col / 2);
    }

    int setdy(int row) {
        return (rand() % row) - (row / 2);
    }
};

class DroneBot : public MovingRobot, public SeeingRobot, public ShootingRobot, public ThinkingRobot {
private:
    int droneUses = 2; // only 2 scans per match

public:
    int enemyX = -1;
    int enemyY = -1;
    int lookCounter = 0;
    bool enemyFound = false;
    int newpos_x;
    int newpos_y;
    int dx;
    int dy;
    
    DroneBot(string type, string name, int x, int y) : Robot(type, name, x, y) {
        shells = 10;
        robotLife = 3;upgradeCounterShoot++;upgradeCounterSee++;
    }

    void shoot(int x, int y) override {
        lookCounter--;
        if (shells > 0) {
            cout << robotSymbol << " shoots at (" << x << ", " << y << ")" << endl;
            outfile << robotSymbol << " shoots at (" << x << ", " << y << ")" << endl;
            shells--;
            
            int dx = abs(getPosX() - x);
            int dy = abs(getPosY() - y);
            if (dx <= 1 && dy <= 1) {
                if ((rand() % 100) < 70) {
                    for (Robot* target : Battlefield::robotsGlobal) {
                        if (target != this && target->getPosX() == x && target->getPosY() == y) {
                                if (target->ishiding == true){
                                    cout << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                                    outfile << getrobotSymbol() << " shoots at " << target->getrobotSymbol() << " but " << target->getrobotSymbol() << " hid and is safe" << endl;
                                }
                                else{
                                    cout << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                    outfile << "Direct hit! " << target->getrobotSymbol() << " was destroyed!" << endl;
                                    target->setLife();
                                    robotKills++;
                                    break;   
                                }
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
                setLife();
            }
        } else {
            cout << robotSymbol << " has no shells left!" << endl;
            outfile << robotSymbol << " has no shells left!" << endl;
        }
    }

    void move(int col, int row) override {
        lookCounter--;
        cout << "Robot " << getrobotSymbol() << " starting at (" << getPosX() << "," << getPosY() << ")" << endl;
        outfile << "Robot " << getrobotSymbol() << " starting at (" << getPosX() << "," << getPosY() << ")" << endl;
        
        if(enemyFound) {
            dx = (enemyX > getPosX()) ? 1 : (enemyX < getPosX()) ? -1 : 0;
            dy = (enemyY > getPosY()) ? 1 : (enemyY < getPosY()) ? -1 : 0;
            cout << robotSymbol << " moves towards enemy robot at (" << enemyX << "," << enemyY << ")" << endl;
            outfile << robotSymbol << " moves towards enemy robot at (" << enemyX << "," << enemyY << ")" << endl;
        } else {
            dx = setdx(col);
            dy = setdy(row);
        }
        
        newpos_x = getPosX() + (dx > 0 ? 1 : (dx < 0 ? -1 : 0));
        newpos_y = getPosY() + (dy > 0 ? 1 : (dy < 0 ? -1 : 0));

        if(newpos_x >= 0 && newpos_x < col && newpos_y >= 0 && newpos_y < row) {
            bool occupied = false;
            for (Robot* other : Battlefield::robotsGlobal) { 
                if (other != this && other->getPosX() == newpos_x && other->getPosY() == newpos_y && other->isDead == false) {
                    occupied = true;
                    int adj_dx = abs(getPosX() - newpos_x);
                    int adj_dy = abs(getPosY() - newpos_y);
                    if (adj_dx <= 1 && adj_dy <= 1) {
                        shoot(newpos_x, newpos_y);
                        return;
                    }
                    break;
                }
            }
            
            if(!occupied) {
                setPosX(newpos_x);
                setPosY(newpos_y);
                cout << "Robot " << getrobotSymbol() << " moved to (" << getPosX() << "," << getPosY() << ")" << endl;
                outfile << "Robot " << getrobotSymbol() << " moved to (" << getPosX() << "," << getPosY() << ")" << endl;
            } else {
                cout << "Robot would not move to already occupied space" << endl;
                outfile << "Robot would not move to already occupied space" << endl;
            }
        }
    }

    void see(int x, int y, int col, int row) override {
    if (droneUses > 0 && (x != 0 || y != 0)) {
        cout << robotSymbol << " deploys a drone to (" << x << "," << y << ") to scout a 9x9 area:\n";
        outfile << robotSymbol << " deploys a drone to (" << x << "," << y << ") to scout a 9x9 area:\n";
        droneUses--;

        for (int dy = -4; dy <= 4; dy++) {
            for (int dx = -4; dx <= 4; dx++) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < col && ny >= 0 && ny < row) {
                    bool found = false;
                    for (Robot* r : Battlefield::robotsGlobal) {
                        if (r != this && r->getPosX() == nx && r->getPosY() == ny && !r->isDead) {
                            cout << "  Drone sees enemy at (" << nx << ", " << ny << ") symbol: " << r->getrobotSymbol() << "\n";
                            outfile << "  Drone sees enemy at (" << nx << ", " << ny << ") symbol: " << r->getrobotSymbol() << "\n";
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        cout << "  (" << nx << ", " << ny << ") is empty.\n";
                        outfile << "  (" << nx << ", " << ny << ") is empty.\n";
                    }
                }
            }
        }
    } else {
        cout << robotSymbol << " has no drones left or default scanning.\n";
        outfile << robotSymbol << " has no drones left or default scanning.\n";
    }
}


    void think(int col,int row) override{
            int x = abs(getPosX() - col);
            int y = abs(getPosY() - row);

            if (droneUses > 0 && rand() % 100 < 20) {
                int tx = rand() % col;
                int ty = rand() % row;
                see(tx, ty, col, row);
                return;
            }   
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
            else if (robotTypeList[i] == "StealBot") {
                addRobot(new StealBot(robotTypeList[i], robotNameList[i], tempNumX, tempNumY));
            }
            else if (robotTypeList[i] == "ScoutBot") {
                addRobot(new ScoutBot(robotTypeList[i], robotNameList[i], tempNumX, tempNumY));
            }
            else if (robotTypeList[i] == "TrackBot") {
                addRobot(new TrackBot(robotTypeList[i], robotNameList[i], tempNumX, tempNumY));
            }
            else if (robotTypeList[i] == "DroneBot") {
                addRobot(new DroneBot(robotTypeList[i], robotNameList[i], tempNumX, tempNumY));
            }
            else if (robotTypeList[i] == "JumpBot") {
                addRobot(new JumpBot(robotTypeList[i], robotNameList[i], tempNumX, tempNumY));
            }
            else if (robotTypeList[i] == "HideBot") {
                addRobot(new HideBot(robotTypeList[i], robotNameList[i], tempNumX, tempNumY));
            }
            else if (robotTypeList[i] == "AdvanceMoveBot") {
                addRobot(new AdvanceMoveBot(robotTypeList[i], robotNameList[i], tempNumX, tempNumY));
            }
            
        }
    }
}

void Battlefield::upgradeBot(Robot *bot){
    string name = bot->getRobotName();
    int x = bot->getPosX();int y = bot->getPosY();
    if (bot->killNeeded == 4){
        robotsGlobal.push_back(bot);
        return;
    }
    if(bot->getKills() == bot->killNeeded){
        bot->killNeeded ++;
        if (bot->upgradeCounterShoot == 0){
            // bot->upgradeCounterShoot++;
            int randomUpgrade = rand() % 4;
            if (randomUpgrade == 0){
                bot = new LongShotBot("LongShotBot",name,x,y);
            }
            else if (randomUpgrade == 1){
                bot = new SemiAutoBot("SemiAutoBot",name,x,y);
            }
            else if (randomUpgrade == 2){
                bot = new ThirtyShotBot("ThirtyShotBot",name,x,y);
            }
            else if (randomUpgrade == 3){
            bot = new StealBot("StealBot",name,x,y);
            }
        }
        else if (bot->upgradeCounterSee == 0){
            // bot->upgradeCounterSee++;
            int randomUpgrade = rand() % 3;
            if (randomUpgrade == 0){
                bot = new ScoutBot("ScoutBot",name,x,y);
            }
            else if (randomUpgrade == 1){
                bot = new TrackBot("TrackBot",name,x,y);
            }
            else if (randomUpgrade == 2){
                bot = new DroneBot("DroneBot",name,x,y);
            }
        }
        else if (bot->upgradeCounterMove == 0){
            // bot->upgradeCounterMove++;
            int randomUpgrade = rand() % 3;
            if (randomUpgrade == 0){
                bot = new HideBot("HideBot",name,x,y);
            }
            else if (randomUpgrade == 1){
                bot = new JumpBot("JumpBot",name,x,y);
            }
            else if (randomUpgrade == 2){
                bot = new AdvanceMoveBot("AdvanceMoveBot",name,x,y);
            }
        }
        cout << bot->getrobotSymbol() << " has upgraded to " << bot->getRobotType() << endl;
        outfile << bot->getrobotSymbol() << " has upgraded to " << bot->getRobotType() << endl;
        robotsGlobal.push_back(bot);
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
