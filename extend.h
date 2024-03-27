#ifndef SPLENDOR_EXTEND_H
#define SPLENDOR_EXTEND_H
#include "splendor.h"
class GameInfo;
//城市扩展，修改游戏结束条件
class Extend{
public:
    Extend()=default;
    Extend(int n);
    ~Extend()=default;
    int score;
    int red;
    int blue;
    int green;
    int white;
    int black;
    int grey;   //任意一种颜色
    bool isreached;
    void printInfo() const;
};

int extendGame(GameInfo* gameMune);

#endif //SPLENDOR_EXTEND_H
