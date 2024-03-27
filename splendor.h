#ifndef SPLENDOR_SPLENDOR_H
#define SPLENDOR_SPLENDOR_H
#include"extend.h"
#include<iostream>
#include<string>
#include<vector>
#include<time.h>
#include<cstring>
#include<stdlib.h>
using namespace std;
class DevelopmentCard;
class Aristocrats;      //贵族
class Extend;

//指示物属性
class Referent{
public:
    int num;
};

//玩家属性
class Player{
    string name;
    int redStoneNum;
    int blueStoneNum;
    int greenStoneNum;
    int whiteStoneNum;  //Diamond
    int blackStoneNum;  //Agate
    int goldNum;
    int Score;
public:
    bool is_extend_get;                         //是否到达扩展胜利条件
    int Bonus[5];                               //红利
    vector<DevelopmentCard*> m_developmentCards;//预定发展卡的容器
    Player()=default;
    Player(string _name);
    ~Player()=default;
    //设置、获取宝石数量
    int getstoneSum(){return redStoneNum+blueStoneNum+greenStoneNum+whiteStoneNum+blackStoneNum+goldNum;}
    void setRredStoneNum(int n){redStoneNum = n;}
    int getredStoneNum(){return redStoneNum;}
    void setRblueStoneNum(int n){blueStoneNum = n;}
    int getblueStoneNum(){return blueStoneNum;}
    void setRgreenStoneNum(int n){greenStoneNum = n;}
    int getgreenStoneNum(){return greenStoneNum;}
    void setRwhiteStoneNum(int n){whiteStoneNum = n;}
    int getwhiteStoneNum(){return whiteStoneNum;}
    void setRblackStoneNum(int n){blackStoneNum = n;}
    int getblackStoneNum(){return blackStoneNum;}
    void setRgoldNum(int n){goldNum=n;}
    int getgoldNum(){return goldNum;}
    //分数
    void setScore(int n){Score = n;}
    int getScore(){return Score;}
    //名字
    void setName(string _name){name = _name;}
    string getName(){return name;}
};

//发展卡属性
class DevelopmentCard{
    //enum Colour{red,blue,green,white,black};
public:
    int grade;  //等级
    int redStone;
    int blueStone;
    int greenStone;
    int whiteStone;
    int blackStone;
    int score;
    int m_colour;
    string colour;
    DevelopmentCard()=default;
    DevelopmentCard(int _grade);
    ~DevelopmentCard()=default;
    string ColourSelect(int n){
        switch(n){
            case 0: return "red";
            case 1: return "blue";
            case 2: return "green";
            case 3: return "white";
            case 4: return "black";
            default: return "0";
        }
    }
    //输出卡牌信息
    void printInfo(){
        cout << grade << "\t"<< colour << "\t" << score
        << "\t" <<redStone << "\t" << blueStone<< "\t" << greenStone << "\t" << whiteStone << "\t" << blackStone << endl;
    }
private:
};

//贵族
class Aristocrats{
public:
    int red;
    int blue;
    int green;
    int white;
    int black;
    int score;
    //Aristocrats();
    Aristocrats(int n);
    ~Aristocrats()=default;
    void printInfo(){cout<<red<<"\t"<<blue<<"\t"<<green<<"\t"<<white<<"\t"<<black<<"\t"<<score<<"\t"<<endl;}
private:
};

//游戏管理的基类
class GameInfo
{
public:
    vector<Extend*> extends;                //扩展信息容器
    vector<Aristocrats*>aristocrats;        //贵族容器
    Referent redStone;
    Referent blueStone;
    Referent greenStone;
    Referent whiteStone;
    Referent blackStone;
    Referent gold;
    int class_win_score;                    //基础游戏游戏结束分数条件
    vector<DevelopmentCard*> m_devepI;      //三个等级的卡牌
    vector<DevelopmentCard*> m_devepII;
    vector<DevelopmentCard*> m_devepIII;
    vector<Player*>m_players;               //玩家容器
    GameInfo()=default;
    virtual ~GameInfo()=default;
    //显示桌子上面的信息
    virtual void showDeskInfo(int num) = 0;
    //拿三种不同宝石
    virtual bool Take_three_different_gems(int num) = 0;
    //拿两种相同宝石
    virtual bool Take_two_same_gems(int num) = 0;
    //拿卡牌和黄金
    virtual bool TakeCarAndGold(int num)=0;
    //添加宝石
    virtual bool addGemsNum(int* choose, int num) = 0;
    //拿卡牌的接口
    virtual bool ispushCar(int num, int carNum) = 0;
    //购买卡牌
    virtual bool buyCarToInfo(int num) = 0;
    //是否达到扩展结束条件
    virtual bool is_reach_extend(int num)=0;
    //扩展
    virtual void ExtendWin()=0;
    //是否获胜
    virtual bool isWin()=0;
    // 是否有贵族来访
    virtual bool isAristocrats(int num) = 0;
    //购买自己手上的卡牌
    bool buyCarMyself(int num);
    //购买其他卡牌
    bool buyCarOther(int num);
    //在扩展游戏中显示桌子上面的信息
    virtual void showDeskInfoExtend(int num)=0;

    //玩家菜单
    void Menu(int num);
private:
    //买预定的卡
    bool StoneToCar(int num,int carId);
    //买一级卡
    bool StoneToCar1(int num,int carId);
    //买二级卡
    bool StoneToCar2(int num,int carId);
    //买三级卡
    bool StoneToCar3(int num,int carId);
};

class GameMune: public GameInfo{
public:
    GameMune()=default;
    ~GameMune();
    GameMune(int num);//num为游戏人数
private:
    virtual bool isAristocrats(int num);
    virtual bool Take_three_different_gems(int num);
    virtual bool Take_two_same_gems(int num);
    virtual bool TakeCarAndGold(int num);
    virtual bool addGemsNum(int* choose, int num);
    virtual bool ispushCar(int num, int carNum);
    virtual bool isWin();
    virtual bool is_reach_extend(int num);
    virtual void ExtendWin();
    virtual void showDeskInfo(int num);
    virtual void showDeskInfoExtend(int num);
    virtual bool buyCarToInfo(int num);
};

int StartGame(int num);




#endif //SPLENDOR_SPLENDOR_H
