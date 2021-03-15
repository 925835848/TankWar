#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h> 
#include<conio.h>
#include <vector>
using std::vector;
#pragma comment(lib,"winmm.lib")
#define UP    1
#define DOWN  2
#define LEFT  3
#define RIGHT 4
#define MAX_LEVEL 8
#define BULLET_NUM 40
#define MAX_LIFE_1 4
#define MAX_LIFE_2 8

typedef struct            
{                         
    int fast_tank_order;  
    int firm_tank_order;   
} LevInfo;            
LevInfo level_info[MAX_LEVEL] = { {-1,-1},{3,-1},{-1,3},{2,3},{2,3},{2,3},{2,3},{2,3} }; 

void WriteChar(short x, short y, const char* psz, WORD Color)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, Color);
    COORD loc;
    loc.X = x * 2;
    loc.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), loc);
    printf(psz);
}
class Tank{  
public:
    int getDir();
    int getStop();
    void setStop(int stop);
    int getNum();
    void setNum(int num);
    int getMy();
    void setMy(int my);
    bool getAlive();
    void setAlive(bool alive);
    void BuildAITank(int* position, Tank* AI_tank); 
    void BuildMyTank(Tank* my_tank,int i);             
    void MoveAITank(Tank* AI_tank);              
    void MoveMyTank(int turn, int i);                  
    void ClearTank(int x, int y);                
    void PrintTank(Tank tank);                   
    bool TankCheak(Tank tank, int direction);     
    int  AIPositionCheak(int position);          
    int getX(void);
    int getY(void);
    int getcolor(void);
    void setcolor(int c);
    int getmodel(void);
    int getrevive(void);
    void setrevive(int r);
    int getCD(void);
    void setCD(int d);
    void setX(int a);
    void setY(int b);
private:
    int x, y;
    int color;      
    int model;     
    int revive;     
    int CD;        
    int direction;  
    int stop;       
    int num;      
    int my;        
    bool alive;    
};
Tank AI_tank[4], my_tank_1, my_tank_2; 

class GameManager{
public:
    void HideCursor();          
    void keyboard_1(Tank my_tank);       
    void keyboard_2(Tank my_tank);    
    void Initialize();        
    void Stop();              
    void GetMap();           
    void Frame();            
    void PrintMap();           
    void SideScreen();        
    void heak();          
    void ver(bool home);  
    void ClearMainScreen();     
    void NextLevel();          
    int RunGame();
    void BeginUI();
    int getGameMode();
    void setGameMode(int GameMode);
    void PlayMusic();
    void PlayOp();
    void PlayHit();
    void PlayDeath();
private:
    int GameMode;
};


void GoToxy(int x, int y);  
void ColorChoose(int color); 

class Bullet {
public:
    void BuildAIBullet(Tank* tank,GameManager obj);         
    void BuildBullet(Tank tank);                
    void BulletFly(Bullet bullet[BULLET_NUM], GameManager obj);
    void BulletHit(Bullet* bullet, GameManager* obj);         
    void PrintBullet(int x, int y, int T);        
    void ClearBullet(int x, int y, int T);       
    int  BulletCheak(int x, int y);            
    int getX();
    void setX(int x);
    int getY();
    void setY(int y);
    int getDir();
    void setDir(int direction);
    int getExist();
    void setExist(int exist);
    int getInitial();
    void setInitial(int initial);
    bool getMy();
    void setMy(int my);
private:
    int x, y;       
    int direction;  
    bool exist;    
    bool initial;  
    int my;       
};
Bullet bullet[BULLET_NUM]; 

int map[41][41]; 
int key_x;     
int bul_num;    
int position;    
int speed = 7;    
int level = 1;    
int score_1 = 0;  
int score_2 = 0;
int remain_enemy; 

const char* tank_figure[4][3][4] =
{
  {
    {"◢┃◣", "◢━◣", "◢┳◣", "◢┳◣"},
    {"┣●┫", "┣●┫", "━●┃", "┃●━"},
    {"◥━◤", "◥┃◤", "◥┻◤", "◥┻◤"}
  },
  {
    {"┏┃┓", "┏┳┓", "┏┳┓", "┏┳┓"},
    {"┣●┫", "┣●┫", "━●┫", "┣●━"},
    {"┗┻┛", "┗┃┛", "┗┻┛", "┗┻┛"}
  },
  {
    {"┏┃┓", "◢━◣", "┏┳◣", "◢┳┓"},
    {"┣●┫", "┣●┫", "━●┃", "┃●━"},
    {"◥━◤", "┗┃┛", "┗┻◤", "◥┻┛"}
  },
  {
    {"╔┃╗", "╔╦╗", "╔╦╗", "╔╦╗"},
    {"╠█╣", "╠█╣", "━█╣", "╠█━"},
    {"╚╩╝", "╚┃╝", "╚╩╝", "╚╩╝"}
  }
};
int main()                          
{ 

    GameManager obj;
    obj.PlayMusic();
    obj.RunGame();
    return 0;
}
void  GoToxy(int x, int y)  
{
    COORD  coord;        
    coord.X = x;         
    coord.Y = y;
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);  
    SetConsoleCursorPosition(a, coord);       
}
void  ColorChoose(int color) 
{
    switch (color)
    {
    case 1:              
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;
    case 2:             
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
        break;
    case 3:              
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
        break;
    case 4:              
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
        break;
    case 5:              
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
        break;
    case 6:             
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
        break;
    case 7:              
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
        break;
    }
}

void Bullet::BuildAIBullet(Tank* tank,GameManager obj)  
{
    if (tank->getCD() == 15)
    {
        if (!(rand() % 11))    
        {
            BuildBullet(*tank);
            tank->setCD(0);
        }
    }
    else
        tank->setCD(tank->getCD() + 1);
    if (tank->getCD() >= 14)     
    {
        if (tank->getY()== 38)    
        {
            if (tank->getX() < 20)
            {
                if (tank->getDir() == RIGHT) 
                {
                    BuildBullet(*tank); 
                    tank->setCD(0);
                }
            }
            else            
                if (tank->getDir() == LEFT) 
                {
                    BuildBullet(*tank);    
                    tank->setCD(0);
                }
        }
        else if (tank->getX() == my_tank_1.getX() + 1 || tank->getX() == my_tank_1.getX() || tank->getX() == my_tank_1.getX() - 1)
        {
            if (tank->getDir() == DOWN && my_tank_1.getY() > tank->getY() || tank->getDir() == UP && my_tank_1.getY() < tank->getY())
            {                           
                int big = my_tank_1.getY(), smal = tank->getY(), i;
                if (my_tank_1.getY() < tank->getY())
                {
                    big = tank->getY();
                    smal = my_tank_1.getY();
                }                  
                for (i = smal + 2; i <= big - 2; i++)  
                    if (map[i][tank->getX()] != 0 || map[i][tank->getX()] != 5)     
                        break;
                if (i == big - 1)               
                {
                    BuildBullet(*tank);   
                    tank->setCD(0);
                }
            }
        }
        else if (tank->getY() == my_tank_1.getY() + 1 || tank->getY() == my_tank_1.getY() || tank->getY() == my_tank_1.getY() - 1)
        {
            if (tank->getDir() == RIGHT && my_tank_1.getX() > tank->getX() || tank->getDir() == LEFT && my_tank_1.getX() < tank->getX())
            {                
                int big = my_tank_1.getY(), smal = tank->getY(), i;
                if (my_tank_1.getX() < tank->getX())
                {
                    big = tank->getX();
                    smal = my_tank_1.getX();
                }
                for (i = smal + 2; i <= big - 2; i++) 
                    if (map[tank->getY()][i] != 0 || map[tank->getY()][i] != 5)     
                        break;
                if (i == big - 1)   
                {
                    BuildBullet(*tank);   
                    tank->setCD(0);
                }
            }
        }
        else if (obj.getGameMode() == 2)
        {
            if (tank->getX() == my_tank_2.getX() + 1 || tank->getX() == my_tank_2.getX() || tank->getX() == my_tank_2.getX() - 1) 
            {
                if (tank->getDir() == DOWN && my_tank_2.getY() > tank->getY() || tank->getDir() == UP && my_tank_2.getY() < tank->getY())
                {                             
                    int big = my_tank_2.getY(), smal = tank->getY(), i;
                    if (my_tank_2.getY() < tank->getY())
                    {
                        big = tank->getY();
                        smal = my_tank_2.getY();
                    }
                    for (i = smal + 2; i <= big - 2; i++)
                        if (map[i][tank->getX()] != 0 || map[i][tank->getX()] != 5)    
                            break;
                    if (i == big - 1)             
                    {
                        BuildBullet(*tank);    
                        tank->setCD(0);
                    }
                }
            }
            else if (tank->getY() == my_tank_2.getY() + 1 || tank->getY() == my_tank_2.getY() || tank->getY() == my_tank_2.getY() - 1)
            {
                if (tank->getDir() == RIGHT && my_tank_2.getX() > tank->getX() || tank->getDir() == LEFT && my_tank_2.getX() < tank->getX())
                {                 
                    int big = my_tank_2.getY(), smal = tank->getY(), i;
                    if (my_tank_2.getX() < tank->getX())
                    {
                        big = tank->getX();
                        smal = my_tank_2.getX();
                    }
                    for (i = smal + 2; i <= big - 2; i++) 
                        if (map[tank->getY()][i] != 0 || map[tank->getY()][i] != 5)    
                            break;
                    if (i == big - 1)   
                    {
                        BuildBullet(*tank);   
                        tank->setCD(0);
                    }
                }
            }
        }
    }
}
void Bullet::BuildBullet(Tank tank) 
{                           
    switch (tank.getDir()) 
    {
    case UP:
        bullet[ bul_num].x = tank.getX();
        bullet[ bul_num].y = tank.getY() - 2;
        bullet[ bul_num].direction = 1;
        break;
    case DOWN:
        bullet[ bul_num].x = tank.getX();
        bullet[ bul_num].y = tank.getY() + 2;
        bullet[ bul_num].direction = 2;
        break;
    case LEFT:
        bullet[ bul_num].x = tank.getX() - 2;
        bullet[ bul_num].y = tank.getY();
        bullet[ bul_num].direction = 3;
        break;
    case RIGHT:
        bullet[ bul_num].x = tank.getX() + 2;
        bullet[ bul_num].y = tank.getY();
        bullet[ bul_num].direction = 4;
        break;
    }
    bullet[ bul_num].exist = 1;   
    bullet[ bul_num].initial = 1; 
    bullet[ bul_num].my = tank.getMy();   
     bul_num++;
    if ( bul_num == BULLET_NUM)       
         bul_num = 0;                 
}
void Bullet::BulletFly(Bullet bullet[BULLET_NUM],GameManager obj) 
{                                         
    for (int i = 0; i < BULLET_NUM; i++)
    {
        if (bullet[i].exist)             
        {
            if (bullet[i].initial == 0)   
            {
                if (map[bullet[i].y][bullet[i].x] == 0 || map[bullet[i].y][bullet[i].x] == 5)  
                    ClearBullet(bullet[i].x, bullet[i].y, BulletCheak(bullet[i].x, bullet[i].y));    
                switch (bullet[i].direction)                                     
                {
                case UP:(bullet[i].y)--; break;
                case DOWN:(bullet[i].y)++; break;
                case LEFT:(bullet[i].x)--; break;
                case RIGHT:(bullet[i].x)++; break;
                }
            }
            int collide = BulletCheak(bullet[i].x, bullet[i].y);  
            if (collide)                                             
                PrintBullet(bullet[i].x, bullet[i].y, collide);     
            else
                BulletHit(&bullet[i],&obj);      
            if (bullet[i].initial)            
                bullet[i].initial = 0;
            for (int j = 0; j < BULLET_NUM; j++) 
                if (bullet[j].exist && j != i && (bullet[i].my || bullet[j].my) && bullet[i].x == bullet[j].x && bullet[i].y == bullet[j].y)
                {                           
                    bullet[j].exist = 0;
                    bullet[i].exist = 0;
                    ClearBullet(bullet[j].x, bullet[j].y, BulletCheak(bullet[j].x, bullet[j].y)); 
                    break;
                }
        }
    }
}
void Bullet::BulletHit(Bullet* bullet, GameManager* obj) 
{                             
    int x = bullet->x;         
    int y = bullet->y;         
    int i;
    if (map[y][x] == 1 || map[y][x] == 2)  
    {
        if (bullet->direction == UP || bullet->direction == DOWN) 
            for (i = -1; i <= 1; i++)
                if (map[y][x + i] == 1 || map[y][x + i] == 2)
                {
                    map[y][x + i] = 0; 
                    GoToxy(2 * x + 2 * i, y);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
                    printf("  ");
                }
        if (bullet->direction == LEFT || bullet->direction == RIGHT)     
            for (i = -1; i <= 1; i++)
                if (map[y + i][x] == 1 || map[y + i][x] == 2)
                {
                    map[y + i][x] = 0;
                    GoToxy(2 * x, y + i);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED); 
                    printf("  ");
                }
        bullet->exist = 0;        
    }
    else if (map[y][x] == 4 || map[y][x] == 6) 
        bullet->exist = 0;
    else if (bullet->my && map[y][x] >= 100 && map[y][x] < 104)  
    {   
        obj->PlayHit();
        int num = map[y][x] % 100;   
        if (AI_tank[num].getmodel() == 3 && AI_tank[num].getcolor() == 2) 
            AI_tank[num].setcolor(3);                       
        else if (AI_tank[num].getmodel() == 3 && AI_tank[num].getcolor() == 3)
            AI_tank[num].setcolor(4);                      
        else                    
        {
            AI_tank[num].setAlive(0);
            AI_tank[num].ClearTank(AI_tank[num].getX(), AI_tank[num].getY());     
        }
        bullet->exist = 0;
        if(bullet->my == 1)
            score_1 += 100;
        if (bullet->my == 2)
            score_2 += 100;
        if(obj->getGameMode() == 1){
            GoToxy(102, 5);          
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf("%d ", score_1);
        }
        if (obj->getGameMode() == 2) {
            GoToxy(102, 5);            
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf("%d ", score_1);
            GoToxy(102, 6);            
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf("%d ", score_2);
        }
    }
    else if (map[y][x] == 200 && bullet->my == 0) 
    {
        obj->PlayHit();
        my_tank_1.setAlive(0);
        my_tank_1.ClearTank(my_tank_1.getX(), my_tank_1.getY());
        bullet->exist = 0;
        my_tank_1.setrevive(my_tank_1.getrevive() + 1);  
        score_1 -= 100;          
        GoToxy(102, 5);       
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        printf("%d   ",  score_1);
        GoToxy(102, 7);        
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
        if(obj->getGameMode() == 1)
            printf("%d   ", MAX_LIFE_1 - my_tank_1.getrevive());
        if(obj->getGameMode() == 2)
            printf("%d   ", MAX_LIFE_2 - my_tank_1.getrevive() - my_tank_2.getrevive());
    }
    else if (map[y][x] == 201 && bullet->my == 0)   
    {
        obj->PlayHit();
        my_tank_2.setAlive(0);
        my_tank_2.ClearTank(my_tank_2.getX(), my_tank_2.getY());
        bullet->exist = 0;
        my_tank_2.setrevive(my_tank_2.getrevive() + 1);     
        score_2 -= 100;          
        GoToxy(102, 6);       
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        printf("%d   ", score_2);
        GoToxy(102, 7);        
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
        printf("%d   ", MAX_LIFE_2 - my_tank_2.getrevive() - my_tank_1.getrevive());
    }
    else if (map[y][x] == 9)     
    {
        obj->PlayDeath();
        bullet->exist = 0;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
        GoToxy(38, 37);     printf("      ");
        GoToxy(38, 38);     printf("◢◣  ");
        GoToxy(38, 39);     printf("███");
        obj->ver(1);          
    }
}
int Bullet::BulletCheak(int x, int y)  
{                            
    if (map[y][x] == 0)
        return 1;
    else if (map[y][x] == 5)
        return 2;
    else
        return 0;
}
void Bullet::PrintBullet(int x, int y, int T)   
{
    if (T == 1)        
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    else if (T == 2)  
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE);
    GoToxy(2 * x, y);
    printf("·");
}
void Bullet::ClearBullet(int x, int y, int T)   
{
    GoToxy(2 * x, y);
    if (T == 2)       
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN);
        printf("～");
    }
    else if (T == 1)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
        printf("  ");
    }
}
int Bullet::getX() {
    return x;
};
void Bullet::setX(int x) {
    this->x = x;
};
int Bullet::getY() {
    return y;
};
void Bullet::setY(int y) {
    this->y = y;
};
int Bullet::getDir() {
    return direction;
};
void Bullet::setDir(int direction) {
    this->direction = direction;
};
int Bullet::getExist() {
    return exist;
};
void Bullet::setExist(int exist) {
    this->exist = exist;
};
int Bullet::getInitial() {
    return initial;
};
void Bullet::setInitial(int initial) {
    this->initial = initial;
};
bool Bullet::getMy() {
    return my;
};
void Bullet::setMy(int my) {
    this->my = my;
};

void Tank::BuildAITank(int* position, Tank* AI_tank) 
{                                       
    if (AIPositionCheak(*position))       
    {
        AI_tank->x = 20 + 18 * (*position); 
        AI_tank->y = 2;
        if (AI_tank->revive == level_info[ level - 1].firm_tank_order) 
        {
            AI_tank->model = 3;        
            AI_tank->color = 2;       
        }
        else if (AI_tank->revive == level_info[ level - 1].fast_tank_order)
        {
            AI_tank->model = 2;
            AI_tank->color = rand() % 6 + 1; 
        }
        else    
        {
            AI_tank->model = 1;
            AI_tank->color = rand() % 6 + 1; 
        }
        AI_tank->alive = 1;      
        AI_tank->direction = 2; 
        AI_tank->revive++;        
        PrintTank(*AI_tank);
        (*position)++;
         remain_enemy--;
        GoToxy(102, 9);            
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
        printf("%d ",  remain_enemy);
        if (*position == 2)         
            *position = -1;
        return;                
    }
}
int Tank::AIPositionCheak(int position)    
{
    int    x, y;
    if (position == 2)                    
        x = 15, y = 38;
    else
        y = 2, x = 20 + 18 * position;  
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (map[y + j - 1][x + i - 1] != 0)
                return 0;             
    return 1;                        
}
void Tank::MoveAITank(Tank* AI_tank) 
{
    if (AI_tank->alive)         
    {
        if (AI_tank->stop != 0)  
        {
            AI_tank->stop--; 
            return;
        }
        if (!(rand() % 23)) 
        {
            AI_tank->direction = rand() % 4 + 1;
            if (rand() % 3)   
            {
                AI_tank->stop = 2;
                return;
            }
        }
        ClearTank(AI_tank->x, AI_tank->y);
        if (TankCheak(*AI_tank, AI_tank->direction))  
            switch (AI_tank->direction)
            {
            case UP: AI_tank->y--; break; 
            case DOWN: AI_tank->y++; break;
            case LEFT: AI_tank->x--; break; 
            case RIGHT: AI_tank->x++; break; 
            }
        else                   
        {
            if (!(rand() % 4))    
            {
                AI_tank->direction = rand() % 4 + 1;
                AI_tank->stop = 2; 
                PrintTank(*AI_tank);
                return;        
            }
            else             
            {
                int j;
                for (j = 1; j <= 4; j++)
                    if (TankCheak(*AI_tank, j))  
                        break;
                if (j == 5)        
                {
                    PrintTank(*AI_tank);
                    return;     
                }
                while (TankCheak(*AI_tank, AI_tank->direction) == 0) 
                    AI_tank->direction = (rand() % 4 + 1);                    
            }
        }
        PrintTank(*AI_tank);    
    }
}
void Tank::BuildMyTank(Tank* my_tank, int i)
{
    if (i == 1) {
        my_tank_1.x = 15;
        my_tank_1.y = 38;
        my_tank_1.stop = NULL;
        my_tank_1.direction = 1;
        my_tank_1.model = 0;
        my_tank_1.color = 1;
        my_tank_1.alive = 1;
        my_tank_1.my = 1;
        my_tank_1.CD = 7;
        PrintTank(my_tank_1);  
    }
    if (i == 2) {
        my_tank_2.x = 25;
        my_tank_2.y = 38;
        my_tank_2.stop = NULL;
        my_tank_2.direction = 1;
        my_tank_2.model = 0;
        my_tank_2.color = 1;
        my_tank_2.alive = 1;
        my_tank_2.my = 2;
        my_tank_2.CD = 7;
        PrintTank(my_tank_2);   
    }
}
void Tank::MoveMyTank(int turn,int i)  
{
    if (i == 1) {
        if (my_tank_1.getAlive()) {
            ClearTank(my_tank_1.x, my_tank_1.y);        
            my_tank_1.direction = turn;                 
            if (TankCheak(my_tank_1, my_tank_1.direction))
                switch (turn)
                {
                case UP: my_tank_1.y--; break; 
                case DOWN: my_tank_1.y++; break;
                case LEFT: my_tank_1.x--; break;  
                case RIGHT: my_tank_1.x++; break; 
                }                                       
            PrintTank(my_tank_1);
        }
    }
    if (i == 2) {
        if (my_tank_2.getAlive()) {
            ClearTank(my_tank_2.x, my_tank_2.y);     
            my_tank_2.direction = turn;               
            if (TankCheak(my_tank_2, my_tank_2.direction)) 
                switch (turn)
                {
                case UP: my_tank_2.y--; break; 
                case DOWN: my_tank_2.y++; break;  
                case LEFT: my_tank_2.x--; break;  
                case RIGHT: my_tank_2.x++; break;  
                }                                      
            PrintTank(my_tank_2);
        }
    }
}
bool Tank::TankCheak(Tank tank, int direction) 
{
    switch (direction)                 
    {
    case UP:
        if (map[tank.y - 2][tank.x] == 0 && map[tank.y - 2][tank.x - 1] == 0 && map[tank.y - 2][tank.x + 1] == 0)
            return 1;
        else
            return 0;
    case DOWN:
        if (map[tank.y + 2][tank.x] == 0 && map[tank.y + 2][tank.x - 1] == 0 && map[tank.y + 2][tank.x + 1] == 0)
            return 1;
        else
            return 0;
    case LEFT:
        if (map[tank.y][tank.x - 2] == 0 && map[tank.y - 1][tank.x - 2] == 0 && map[tank.y + 1][tank.x - 2] == 0)
            return 1;
        else
            return 0;
    case RIGHT:
        if (map[tank.y][tank.x + 2] == 0 && map[tank.y - 1][tank.x + 2] == 0 && map[tank.y + 1][tank.x + 2] == 0)
            return 1;
        else
            return 0;
    default:
        printf("错误！！");
        Sleep(5000);
        return 0;
    }
}
void Tank::ClearTank(int x, int y)   
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {                  
            map[y + j - 1][x + i - 1] = 0;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
            GoToxy(2 * x + 2 * j - 2, y + i - 1);
            printf("  ");
        }
}
void Tank::PrintTank(Tank tank)     
{                     
    ColorChoose(tank.color);  
    const char* (*tankF)[4] = tank_figure[tank.model];  
    for (int i = 0; i < 3; i++)
    {
        GoToxy((tank.x - 1) * 2, tank.y - 1 + i);       
        printf("%s", tankF[i][tank.direction - 1]); 
        for (int j = 0; j < 3; j++) {
            if (tank.my == 1)       
                map[tank.y + j - 1][tank.x + i - 1] = 200;  
            else if(tank.my == 2)
                map[tank.y + j - 1][tank.x + i - 1] = 201;  
            else 
                map[tank.y + j - 1][tank.x + i - 1] = 100 + tank.num; 
        }
    }
}
int Tank::getX() {
    return x;
}
int Tank::getY() {
    return y;
}
void Tank::setX(int a) {
    x = a;
}
void Tank::setY(int b) {
    y = b;
}
int Tank::getcolor() {
    return color;
}
void Tank::setcolor(int c) {
    color = c;
}
int Tank::getmodel() {
    return model;
}
int Tank::getrevive() {
    return revive;
}
void Tank::setrevive(int r) {
    revive = r;
}
int Tank::getCD(void) {
    return CD;
}
void Tank::setCD(int d) {
    CD = d;
}
int Tank::getDir() {
    return direction;
}
int Tank::getStop() {
    return stop;
}
void Tank::setStop(int stop) {
    this->stop = stop;
}
int Tank::getNum() {
    return num;
}
void Tank::setNum(int num) {
    this->num = num;
}
int Tank::getMy() {
    return my;
}
void Tank::setMy(int my) {
    this->my = my;
}
bool Tank::getAlive() {
    return alive;
}
void Tank::setAlive(bool alive) {
    this->alive = alive;
}

void GameManager::HideCursor()  
{              
    CONSOLE_CURSOR_INFO cursor_info = { 1,0 };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void GameManager::Stop()  
{
    int color = 1, timing = 0;
    while (1)
    {
        if (timing++ % 30 == 0)
        {
            ColorChoose(color); 
            GoToxy(100, 13);    
            printf("游戏暂停");
            GoToxy(88, 17);
            printf("按P键回到游戏");
            GoToxy(88, 18);
            printf("按Esc键退出游戏");
            if (++color == 8)
                color = 1;
        }
        if (GetAsyncKeyState('P') & 0x8000)     
        {
            GoToxy(100, 13);       
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf("正在进行");  
            GoToxy(88, 17);
            printf("                     ");
            GoToxy(88, 18);
            printf("                     ");
            break;
        }
        else if (GetAsyncKeyState(0x1B) & 0x8000)
            exit(0);
        Sleep(20);
    }
}
void GameManager::ClearMainScreen() 
{
    for (int i = 1; i < 40; i++)
    {
        GoToxy(2, i);
        printf("                                                                              ");
    }
}
void GameManager::Frame()   
{              
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("  ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁  ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
    printf("  ▂▂▂▂▂▂▂▂▂▂▂▂▂ \n");
    for (int i = 0; i < 14; i++)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("▕                                                                              ▏");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
        printf(" |                          |\n");
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("▕                                                                              ▏");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
    printf(" |═════════════|\n");
    for (int i = 0; i < 24; i++)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("▕                                                                              ▏");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
        printf(" |                          |\n");
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("  ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔  ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
    printf(" ﹊﹊﹊﹊﹊﹊﹊﹊﹊﹊﹊﹊﹊﹊\n");
    SideScreen(); 
}
void GameManager::PrintMap()     
{
    for (int j = 0; j < 41; j++)
        for (int i = 0; i < 41; i++)
            if (map[i][j] == 6)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN
                    | FOREGROUND_RED | FOREGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE);
                GoToxy(2 * j, i);
                printf("■");
            }
            else if (map[i][j] == 2)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_GREEN | BACKGROUND_RED);
                GoToxy(2 * j, i);
                printf("▓");
            }
            else if (map[i][j] == 1)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | BACKGROUND_GREEN | BACKGROUND_RED);
                GoToxy(2 * j, i);
                printf("▓");
            }
            else if (map[i][j] == 5)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN);
                GoToxy(2 * j, i);
                printf("～");
            }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
    GoToxy(38, 37);     printf("◣※◢");
    GoToxy(38, 38);     printf("███");   
    GoToxy(38, 39);     printf("◢█◣");   
}
void GameManager::GetMap()     
{                 
    int i, j;     
    int Map[8][41][41] =
    {
        {
            {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,6,6,6,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,6,6,6,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,6,6,6,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,4},
            {4,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,4},
            {4,6,6,6,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,6,6,6,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4}
        },
        {
            {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,6,6,6,2,2,2,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,6,6,6,2,2,2,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,6,6,6,2,2,2,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,6,6,6,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,6,6,6,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,6,6,6,0,0,0,4},
            {4,1,1,1,2,2,2,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,6,6,6,0,0,0,4},
            {4,1,1,1,2,2,2,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,6,6,6,0,0,0,4},
            {4,1,1,1,2,2,2,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,6,6,6,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,2,2,2,1,1,1,6,6,6,6,6,6,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,2,2,2,1,1,1,6,6,6,6,6,6,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,2,2,2,1,1,1,6,6,6,6,6,6,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,2,2,2,6,6,6,1,1,1,2,2,2,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,2,2,2,6,6,6,1,1,1,2,2,2,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,2,2,2,6,6,6,1,1,1,2,2,2,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,1,1,1,6,6,6,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,1,1,1,6,6,6,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,1,1,1,6,6,6,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,1,1,1,6,6,6,1,1,1,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,1,1,1,6,6,6,1,1,1,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,1,1,1,6,6,6,1,1,1,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,2,2,2,1,1,1,6,6,6,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,2,2,2,1,1,1,6,6,6,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,2,2,2,1,1,1,6,6,6,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,6,6,6,6,6,6,6,6,6,2,2,2,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,4},
            {4,6,6,6,6,6,6,6,6,6,2,2,2,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,4},
            {4,6,6,6,6,6,6,6,6,6,2,2,2,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,4},
            {4,6,6,6,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,6,6,6,6,6,6,0,0,0,0,0,0,4},
            {4,6,6,6,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,6,6,6,6,6,6,0,0,0,0,0,0,4},
            {4,6,6,6,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,6,6,6,6,6,6,0,0,0,0,0,0,4},
            {4,6,6,6,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,1,1,1,1,1,1,0,0,0,4},
            {4,6,6,6,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,1,1,1,1,1,1,0,0,0,4},
            {4,6,6,6,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,1,1,1,1,1,1,0,0,0,4},
            {4,2,2,2,6,6,6,6,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,6,6,6,6,6,6,4},
            {4,2,2,2,6,6,6,6,6,6,6,6,6,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,6,6,6,6,6,6,4},
            {4,2,2,2,6,6,6,6,6,6,6,6,6,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,6,6,6,6,6,6,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,4},
            {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4}
        },
        {
            {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
            {4,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,6,6,6,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,6,6,6,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,6,6,6,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,6,6,6,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,1,1,1,6,6,6,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,1,1,1,6,6,6,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,4},
            {4,6,6,6,0,0,0,1,1,1,5,5,5,0,0,0,6,6,6,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,5,5,5,0,0,0,6,6,6,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,5,5,5,0,0,0,6,6,6,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,5,5,5,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,5,5,5,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,5,5,5,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,1,1,1,0,0,0,1,1,1,5,5,5,5,5,5,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,1,1,1,4},
            {4,1,1,1,0,0,0,1,1,1,5,5,5,5,5,5,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,1,1,1,4},
            {4,1,1,1,0,0,0,1,1,1,5,5,5,5,5,5,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,1,1,1,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,6,6,6,6,6,6,4},
            {4,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,4},
            {4,1,1,1,1,1,1,0,1,1,1,1,1,0,0,0,6,6,6,0,0,0,1,1,1,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,4},
            {4,1,1,1,1,1,1,0,1,1,1,1,1,0,0,0,6,6,6,0,0,0,1,1,1,0,0,0,5,5,5,0,0,0,1,1,1,1,1,1,4},
            {4,1,1,1,1,1,1,0,1,1,1,1,1,0,0,0,6,6,6,0,0,0,1,1,1,0,0,0,5,5,5,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,5,5,5,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,4},
            {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4}
        },
        {
            {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,6,6,6,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,4},
            {4,6,6,6,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,4},
            {4,5,5,5,0,0,0,0,1,1,1,0,0,0,6,0,0,0,0,0,6,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,4},
            {4,5,5,5,0,0,0,0,1,1,1,0,0,0,6,0,0,0,0,0,6,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,4},
            {4,5,5,5,0,0,0,0,1,1,1,0,0,0,6,0,0,0,0,0,6,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,5,5,5,4},
            {4,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,5,5,5,4},
            {4,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,5,5,5,4},
            {4,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,4},
            {4,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,4},
            {4,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,4},
            {4,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,4},
            {4,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,4},
            {4,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,4},
            {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4}
        },
        {
            {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
            {4,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,5,5,5,5,5,5,0,0,0,5,5,5,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,5,5,5,5,5,5,5,5,5,4},
            {4,5,5,5,5,5,5,0,0,0,5,5,5,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,5,5,5,5,5,5,5,5,5,4},
            {4,5,5,5,5,5,5,0,0,0,5,5,5,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,5,5,5,5,5,5,5,5,5,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,5,5,5,0,0,0,5,5,5,0,0,0,5,5,5,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,5,5,5,0,0,0,5,5,5,0,0,0,5,5,5,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,5,5,5,0,0,0,5,5,5,0,0,0,5,5,5,4},
            {4,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,6,6,6,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,6,6,6,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,6,6,6,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,5,5,5,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,5,5,5,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,5,5,5,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,4},
            {4,5,5,5,5,5,5,0,0,0,5,5,5,0,0,0,5,5,5,5,5,5,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,4},
            {4,5,5,5,5,5,5,0,0,0,5,5,5,0,0,0,5,5,5,5,5,5,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,4},
            {4,5,5,5,5,5,5,0,0,0,5,5,5,0,0,0,5,5,5,5,5,5,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,5,5,5,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,5,5,5,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,5,5,5,4},
            {4,0,0,0,5,5,5,5,5,5,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,1,1,1,0,0,0,5,5,5,4},
            {4,0,0,0,5,5,5,5,5,5,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,1,1,1,0,0,0,5,5,5,4},
            {4,0,0,0,5,5,5,5,5,5,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,1,1,1,0,0,0,5,5,5,4},
            {4,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,5,5,5,4},
            {4,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,5,5,5,4},
            {4,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,5,5,5,4},
            {4,5,5,5,5,5,5,0,0,0,5,5,5,5,5,5,0,0,0,5,5,5,1,1,1,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,4},
            {4,5,5,5,5,5,5,0,0,0,5,5,5,5,5,5,0,0,0,5,5,5,1,1,1,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,4},
            {4,5,5,5,5,5,5,0,0,0,5,5,5,5,5,5,0,0,0,5,5,5,1,1,1,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,5,5,5,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,5,5,5,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,5,5,5,4},
            {4,0,0,0,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,5,5,5,5,5,5,4},
            {4,0,0,0,5,5,5,5,5,5,5,5,5,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,5,5,5,0,0,0,5,5,5,5,5,5,4},
            {4,0,0,0,5,5,5,5,5,5,5,5,5,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,5,5,5,0,0,0,5,5,5,5,5,5,4},
            {4,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4}
        },
        {
            {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,4},
            {4,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,4},
            {4,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,2,2,2,2,2,2,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,4},
            {4,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,2,2,2,2,2,2,2,2,0,0,0,1,1,0,0,0,0,0,0,0,1,1,4},
            {4,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,2,2,2,2,2,2,2,2,0,0,1,1,1,0,0,0,0,0,0,0,1,1,4},
            {4,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,1,1,4},
            {4,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,1,1,4},
            {4,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,1,1,4},
            {4,1,1,0,0,0,0,0,0,0,0,0,1,1,1,2,2,2,2,6,6,6,6,6,6,2,2,2,2,1,1,1,0,0,0,0,0,1,1,1,4},
            {4,1,1,0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,6,6,6,6,6,6,2,2,2,2,1,1,1,1,0,0,0,0,1,1,1,4},
            {4,1,1,0,0,0,0,0,0,0,1,1,1,1,1,2,2,2,2,6,6,6,6,6,6,2,2,2,2,1,1,1,1,0,0,0,1,1,1,1,4},
            {4,0,1,1,0,0,0,0,0,0,1,1,1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,1,1,1,0,0,1,1,1,1,4},
            {4,0,1,1,1,0,0,0,0,1,1,1,1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,1,1,1,1,1,1,1,1,1,4},
            {4,0,0,1,1,1,1,1,1,1,1,1,1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,1,1,1,1,1,1,1,1,1,4},
            {4,0,0,0,1,1,1,1,1,1,1,1,1,6,6,6,6,6,6,1,1,1,6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,1,0,4},
            {4,0,0,0,1,1,1,1,1,1,1,1,1,6,6,6,6,6,6,1,1,1,6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,1,0,4},
            {4,0,0,0,0,1,1,1,1,1,1,1,1,6,6,6,6,6,6,1,1,1,6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,0,0,4},
            {4,0,0,0,0,0,1,1,1,1,1,1,1,6,6,6,0,0,0,1,1,1,0,0,0,6,6,6,1,1,1,1,1,1,1,1,1,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,1,1,1,6,6,6,0,0,0,1,1,1,0,0,0,6,6,6,1,1,1,1,1,1,1,1,1,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,1,1,1,6,6,6,0,0,0,1,1,1,0,0,0,6,6,6,1,1,1,1,1,1,1,1,1,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,4},
            {4,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,4},
            {4,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,4},
            {4,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,4},
            {4,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,4},
            {4,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,4},
            {4,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,4},
            {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4}
        },
        {
            {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
            {4,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,6,6,6,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,6,6,6,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,6,6,6,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,6,6,6,0,0,0,6,6,6,6,6,6,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,6,6,6,0,0,0,6,6,6,6,6,6,4},
            {4,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,6,6,6,0,0,0,6,6,6,6,6,6,4},
            {4,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,6,6,6,1,1,1,0,0,0,0,0,0,0,0,0,6,6,6,4},
            {4,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,6,6,6,1,1,1,0,0,0,0,0,0,0,0,0,6,6,6,4},
            {4,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,6,6,6,1,1,1,0,0,0,0,0,0,0,0,0,6,6,6,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,6,6,6,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,6,6,6,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,6,6,6,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,6,6,6,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,4},
            {4,0,0,0,0,0,0,6,6,6,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,4},
            {4,0,0,0,0,0,0,6,6,6,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,4},
            {4,6,6,6,0,0,0,6,6,6,0,0,0,0,0,0,1,1,1,0,0,0,6,6,6,0,0,0,0,0,0,6,6,6,1,1,1,0,0,0,4},
            {4,6,6,6,0,0,0,6,6,6,0,0,0,0,0,0,1,1,1,0,0,0,6,6,6,0,0,0,0,0,0,6,6,6,1,1,1,0,0,0,4},
            {4,6,6,6,0,0,0,6,6,6,0,0,0,0,0,0,1,1,1,0,0,0,6,6,6,0,0,0,0,0,0,6,6,6,1,1,1,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,6,6,6,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,6,6,6,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,6,6,6,0,0,0,0,0,0,4},
            {4,0,0,0,6,6,6,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,4},
            {4,0,0,0,6,6,6,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,4},
            {4,0,0,0,6,6,6,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,4},
            {4,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,4},
            {4,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,4},
            {4,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,4},
            {4,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,6,6,6,0,0,0,6,6,6,0,0,0,0,0,0,1,1,1,4},
            {4,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,6,6,6,0,0,0,6,6,6,0,0,0,0,0,0,1,1,1,4},
            {4,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,6,6,6,0,0,0,6,6,6,0,0,0,0,0,0,1,1,1,4},
            {4,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,6,6,6,0,0,0,0,0,0,4},
            {4,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,6,6,6,0,0,0,0,0,0,4},
            {4,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,6,6,6,0,0,0,0,0,0,4},
            {4,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,4},
            {4,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,4},
            {4,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,4},
            {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4}
        },
        {
            {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,5,5,5,5,5,5,0,0,0,6,6,6,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,5,5,5,5,5,5,0,0,0,6,6,6,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,5,5,5,5,5,5,0,0,0,6,6,6,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,5,5,5,5,5,5,1,1,1,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,6,6,6,0,0,0,4},
            {4,0,0,0,0,0,0,5,5,5,5,5,5,1,1,1,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,6,6,6,0,0,0,4},
            {4,0,0,0,0,0,0,5,5,5,5,5,5,1,1,1,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,6,6,6,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,1,1,1,0,0,0,0,0,0,4},
            {4,0,0,0,6,6,6,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,6,6,6,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,6,6,6,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,5,5,5,5,5,5,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,5,5,5,5,5,5,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,5,5,5,5,5,5,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,6,6,6,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,6,6,6,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,6,6,6,4},
            {4,0,0,0,1,1,1,5,5,5,5,5,5,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,5,5,5,5,5,5,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,1,1,1,5,5,5,5,5,5,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,6,6,6,0,0,0,5,5,5,5,5,5,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,4},
            {4,6,6,6,0,0,0,5,5,5,5,5,5,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,4},
            {4,6,6,6,0,0,0,5,5,5,5,5,5,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,4},
            {4,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
            {4,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,0,0,0,1,1,1,6,6,6,0,0,0,4},
            {4,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,0,0,0,1,1,1,6,6,6,0,0,0,4},
            {4,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,9,9,9,1,1,0,0,0,0,0,0,0,1,1,1,6,6,6,0,0,0,4},
            {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4}
        },
    };
    for (i = 0; i < 41; i++)
        for (j = 0; j < 41; j++)
            map[i][j] = Map[ level - 1][i][j];
    PrintMap();        
}
void GameManager::ver(bool home)
{
    int timing = 0, color = 1;
    while (1)
    {
        if (timing++ % 30 == 0)       
        {
            ColorChoose(color);   
            if (home)             
            {
                GoToxy(37, 19);    
                printf("老家被毁!");
            }
            GoToxy(37, 20);       
            printf("游戏结束!");
            GoToxy(100, 13);      
            printf("游戏结束");
            GoToxy(88, 17);
            printf("请按回车键重新开始!");
            GoToxy(88, 18);
            printf("或按 Esc键退出游戏!");
            if (++color == 8)
                color = 1;
        }
        if (GetAsyncKeyState(0xD) & 0x8000)
        {
            score_1 -= 500;      
            score_2 -= 500;
            ClearMainScreen();   
            Initialize();        
            break;
        }
        else if (GetAsyncKeyState(0x1B) & 0x8000) 
            exit(0);
        Sleep(20);
    }
}
void GameManager::NextLevel()
{
    int timing = 0, color = 1;
    level++;
    if (level <= MAX_LEVEL)
        while (1)
        {
            if (timing++ % 10 == 0)
            {
                ColorChoose(color);   
                GoToxy(37, 20);        
                printf("恭喜过关!");
                GoToxy(100, 13);      
                printf("等待下关");
                GoToxy(87, 17);
                printf("请按回车键进入下一关!");
                GoToxy(88, 18);
                printf("或按Esc键退出游戏!");
                if (++color == 8)
                    color = 1;
            }
            if (GetAsyncKeyState(0xD) & 0x8000) 
            {
                GoToxy(88, 17);       
                printf("                     ");
                GoToxy(88, 18);
                printf("                     ");
                ClearMainScreen();  
                Initialize();      
                break;
            }
            else if (GetAsyncKeyState(0x1B) & 0x8000)  
                exit(0);
            Sleep(20);
        }
    else   
        while (1)
        {
            if (timing++ % 5 == 0)
            {
                ColorChoose(color);
                GoToxy(33, 20);      
                printf("恭喜通过全部关卡!");
                GoToxy(100, 13);      
                printf("已通全关");
                GoToxy(88, 17);
                printf("恭喜通过全部关卡!");
                GoToxy(88, 19);
                printf("按Esc键退出游戏!");
                if (++color == 8)
                    color = 1;
            }
            if (GetAsyncKeyState(0x1B) & 0x8000) 
                exit(0);
            Sleep(10);
        }
}
void GameManager::heak()
{                           
    if (remain_enemy <= 0 && !AI_tank[0].getAlive() && !AI_tank[1].getAlive() && !AI_tank[2].getAlive() && !AI_tank[3].getAlive())
        NextLevel();     
    if(GameMode == 1)
        if (my_tank_1.getrevive() >= MAX_LIFE_1)  
            ver(0);      
    if(GameMode == 2)
        if (my_tank_1.getrevive()+ my_tank_2.getrevive() >= MAX_LIFE_2)  
            ver(0);     
}
void GameManager::SideScreen()  
{              
    GoToxy(93, 2);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
    printf("第     关");
    if (GameMode == 1) {
        GoToxy(92, 5);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
        printf("分  数：");
    }
    if (GameMode == 2) {
        GoToxy(90, 5);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
        printf("一号分数");
        GoToxy(90, 6);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
        printf("二号分数");
    }
    GoToxy(92, 7);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    printf("生  命：");
    GoToxy(86, 9);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
    printf("剩余敌方坦克：");
    GoToxy(86, 11);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
    printf("当前游戏速度：  %d", 21 - speed);
    GoToxy(86, 13);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("当前游戏状态：");
    GoToxy(94, 19);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
    GoToxy(94, 19);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
    printf("帮  助");
    GoToxy(86, 23);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    printf("玩家一：方向键WASD移动");
    if (GameMode == 2) {
        GoToxy(84, 27);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
        printf("玩家二：方向键←↑→↓移动");
    }
    GoToxy(93, 25);
    printf("J 键 射击");
    if (GameMode == 2) {
        GoToxy(93, 29);
        printf("Enter键 射击");
    }
    GoToxy(89, 31);
    printf("+ - 调整游戏速度");
    GoToxy(90, 33);
    printf("游戏速度范围1~20");
    GoToxy(90, 35);
    printf("字母O键 暂停游戏");
    GoToxy(90, 37);
    printf("Esc键  退出游戏");
}
void GameManager::Initialize()    
{
    remain_enemy = 16;
    my_tank_1.setrevive(0);
    my_tank_2.setrevive(0);
    position = 0;
    bul_num = 0;
    GetMap();
    my_tank_1.BuildMyTank(&my_tank_1, 1);
    if (GameMode == 2)
        my_tank_2.BuildMyTank(&my_tank_2, 2);
    for (int i = 0; i < 36; i++)   
    {
        bullet[i].setExist(0);
        bullet[i].setInitial(0);
    }
    for (int i = 0; i <= 3; i++)         
    {
        AI_tank[i].setrevive(0);
        AI_tank[i].setAlive(0);  
        AI_tank[i].setStop(0);
        AI_tank[i].setNum(i);
        AI_tank[i].setMy(0);
        AI_tank[i].setCD(0);
    }
    GoToxy(97, 2);                        
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
    printf("%d", level);
    if (GameMode == 1) {
        GoToxy(102, 5);                       
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
        printf("%d   ", score_1);
    }
    if (GameMode == 2) {
        GoToxy(102, 5);                      
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
        printf("%d   ", score_1);
        GoToxy(102, 6);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
        printf("%d   ", score_2);
    }
    GoToxy(102, 7);                     
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    if(GameMode == 1)
        printf("%d", MAX_LIFE_1 - my_tank_1.getrevive());
    if(GameMode == 2)
        printf("%d", MAX_LIFE_2 - my_tank_1.getrevive() - my_tank_2.getrevive());
    GoToxy(102, 9);                     
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
    printf("%d ", remain_enemy);
    GoToxy(100, 13);                    
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
    printf("正在游戏");
}
int GameManager::RunGame() {
    int i;
    GameManager obj;
    Bullet bul;
    unsigned int interval[14] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1 };  
    srand(time(NULL));
    obj.HideCursor();                       
    system("mode con cols=112 lines=42"); 
    obj.BeginUI();
    if (obj.GameMode == 4)
        return 0;
    obj.Frame();                           
    obj.Initialize();                    
    for (;;)
    {
        if (interval[0]++ % speed == 0)    
        {
            obj.heak();                 
            bul.BulletFly(bullet, obj);
            for (i = 0; i <= 3; i++)        
            {
                if (AI_tank[i].getmodel() == 2 && interval[i + 1]++ % 2 == 0) 
                    AI_tank[i].MoveAITank(&AI_tank[i]);
                if (AI_tank[i].getmodel() != 2 && interval[i + 5]++ % 3 == 0) 
                    AI_tank[i].MoveAITank(&AI_tank[i]);
            }
            for (i = 0; i <= 3; i++)                                   
                if (AI_tank[i].getAlive() == 0 && AI_tank[i].getrevive() < 4 && interval[9]++ % 90 == 0) 
                {                                              
                    AI_tank[i].BuildAITank(&position, &AI_tank[i]);   
                    break;                                  
                }
            for (i = 0; i <= 3; i++)
                if (AI_tank[i].getAlive())
                    bul.BuildAIBullet(&AI_tank[i],obj);              
            if (obj.GameMode == 1) {
                if (my_tank_1.getAlive() && interval[10]++ % 2 == 0)
                    obj.keyboard_1(my_tank_1);
                if (my_tank_1.getAlive() == 0 && interval[11]++ % 30 == 0 && my_tank_1.getrevive() + my_tank_2.getrevive() < MAX_LIFE_1)
                    my_tank_1.BuildMyTank(&my_tank_1, 1);
            }
            if (obj.GameMode == 2) {
                if (my_tank_1.getAlive() && interval[10]++ % 2 == 0)
                    obj.keyboard_1(my_tank_1);
                if (my_tank_1.getAlive() == 0 && interval[11]++ % 30 == 0 && my_tank_1.getrevive() + my_tank_2.getrevive() < MAX_LIFE_2)
                    my_tank_1.BuildMyTank(&my_tank_1, 1);

                if (my_tank_2.getAlive() && interval[12]++ % 2 == 0)
                    obj.keyboard_2(my_tank_2);
                if (my_tank_2.getAlive() == 0 && interval[13]++ % 30 == 0 && my_tank_1.getrevive() + my_tank_2.getrevive() < MAX_LIFE_2)
                    my_tank_2.BuildMyTank(&my_tank_2, 2);
            }
        }
        Sleep(5);
    }
    return 0;
}
int GameManager::getGameMode() {
    return GameMode;
}
void GameManager::setGameMode(int GameMode) {
    this->GameMode = GameMode;
}

void GameManager::BeginUI()
{
    char szBuffer[4][20] = { "1.单人模式","2.双人模式","3.地图编辑(未完成)","4.退出游戏" };
    int nIndex = 1;
    int nKeyTemp = 1;
    int nFlag = 1;
    system("cls");
    WriteChar(15, 15, "模式选择:(J键确认)", 0x0E);
    while (true)
    {
        if (nFlag)
        {
            nFlag = 0;
            for (int i = 0; i < 4; i++)
            {
                if (i + 1 == nKeyTemp)
                    WriteChar(15, 17 + i, szBuffer[i], 0x06);
                else
                    WriteChar(15, 17 + i, szBuffer[i], 0x0E);
            }
        }
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'j' || ch == 'J')
                break;
            else if (ch == 'w' || ch == 'W')
            {
                nKeyTemp--;
                nFlag = 1;
            }
            else if (ch == 's' || ch == 'S')
            {
                nKeyTemp++;
                nFlag = 1;
            }
        }
        if (nKeyTemp < 1)
            nKeyTemp += 4;
        else if (nKeyTemp > 4)
            nKeyTemp -= 4;
    }
    GameMode = nKeyTemp;
}
void GameManager::keyboard_1(Tank my_tank)
{     
    int count = 0;
    Bullet bul;
    if (my_tank.getAlive()) {
        if (GetAsyncKeyState('W') & 0x8000)
            my_tank_1.MoveMyTank(UP, 1);
        else if (GetAsyncKeyState('S') & 0x8000)
            my_tank_1.MoveMyTank(DOWN, 1);
        else if (GetAsyncKeyState('A') & 0x8000)
            my_tank_1.MoveMyTank(LEFT, 1);
        else if (GetAsyncKeyState('D') & 0x8000)
            my_tank_1.MoveMyTank(RIGHT, 1);
        else if (GetAsyncKeyState(0x1B) & 0x8000)
            exit(0);                              
        else if (GetAsyncKeyState('O') & 0x8000)  
            Stop();
        else if (count++ % 7 == 0)           
        {
            if (speed > 1 && GetAsyncKeyState(0x6B) & 0x8000)  
            {
                speed--;
                GoToxy(102, 11);          
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED);
                printf("%d ", 21 - speed);  
            }
            else if (speed < 20 && GetAsyncKeyState(0x6D) & 0x8000)  
            {
                speed++;
                GoToxy(102, 11);     
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED);
                printf("%d ", 21 - speed); 
            }
        }
        if (my_tank_1.getCD() == 7)
        {
            if (GetAsyncKeyState('J') & 0x8000)
            {
                PlayOp();
                bul.BuildBullet(my_tank_1);
                my_tank_1.setCD(0);
            }
        }
        else
            my_tank_1.setCD(my_tank_1.getCD() + 1);
    }
}
void GameManager::keyboard_2(Tank my_tank){
    int count = 0;
    Bullet bul;
    if (my_tank.getAlive()) {
            if (GetAsyncKeyState(VK_UP) & 0x8000)
                my_tank_2.MoveMyTank(UP, 2);
            else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
                my_tank_2.MoveMyTank(DOWN, 2);
            else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
                my_tank_2.MoveMyTank(LEFT, 2);
            else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
                my_tank_2.MoveMyTank(RIGHT, 2);
            if (my_tank_2.getCD() == 7)
            {
                if (GetAsyncKeyState(0XD) & 0x8000)
                {
                    PlayOp();
                    bul.BuildBullet(my_tank_2);
                    my_tank_2.setCD(0);
                }
            }
            else
                my_tank_2.setCD(my_tank_2.getCD() + 1);
    }
}
void GameManager::PlayMusic()
{
    char temp[255];
    mciSendString(("open sound\\start.mp3 alias MUSIC"), temp, 255, 0);
    mciSendString(("open sound\\Ak.wav alias MUSIC1"), temp, 255, 0);
    mciSendString(("open sound\\Death.wav alias MUSIC2"), temp, 255, 0);
    mciSendString(("open sound\\Hit.wav alias MUSIC3"), temp, 255, 0);
    mciSendString("play MUSIC repeat", NULL, 0, 0);
}

void GameManager::PlayOp()
{
    mciSendString("seek MUSIC1 to start", NULL, 0, 0);
    mciSendString("play MUSIC1", NULL, 0, 0);
}

void GameManager::PlayDeath()
{
    mciSendString("seek MUSIC2 to start", NULL, 0, 0);
    mciSendString("play MUSIC2", NULL, 0, 0);
}

void GameManager::PlayHit()
{
    mciSendString("seek MUSIC3 to start", NULL, 0, 0);
    mciSendString("play MUSIC3", NULL, 0, 0);
}