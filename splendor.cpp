#include "splendor.h"
const int GradeInum=40;
const int GradeIInum=30;
const int GradeIIInum=20;   //三个等级发展卡的数量
//const int WinScore=15;      //触发游戏结束的胜利点
const int StoneLimit=10;    //宝石数量上限

//从min到max随机取num个不一样的数
vector<int> GenerateDiffNumber(int min,int max,int num)
{
    int rnd;
    vector<int> diff;
    vector<int> tmp;//存储剩余的数
    //初始化
    for(int i = min;i < max+1 ; i++ ){tmp.push_back(i);}
    for(int i = 0 ; i < num ; i++){
        do{
            rnd = min+rand()%(max-min+1);
        }
        while(tmp.at(rnd-min)==-1);
        diff.push_back(rnd);
        tmp.at(rnd-min) = -1;
    }
    return diff;
}

//初始化玩家对象
Player::Player(string _name) {
    setName(_name);
    //memset(As,0,sizeof(As));
    memset(Bonus,0,sizeof(Bonus));
    is_extend_get=false;
    redStoneNum=0;
    blueStoneNum=0;
    greenStoneNum=0;
    whiteStoneNum=0;
    blackStoneNum=0;
    goldNum=0;
    Score=0;
}

//随机构造实现卡牌
DevelopmentCard::DevelopmentCard(int _grade) {
    int stone[5]={0};
    int num;
    switch(_grade){
        //一级卡随机0-1分，需要3-5个宝石
        case 1:
            score=rand()%2;
            num=rand()%3+3;
            break;
            //二级卡随机1-3分，需要6-9个宝石
        case 2:
            score=rand()%3+1;
            num=rand()%4+5;
            break;
            //三级卡随机3-5分，需要10-14个宝石
        case 3:
            score=rand()%3+3;
            num=rand()%5+10;
            break;
    }
    for (int i=0;i<num;i++){stone[rand()%5]++;}
    redStone=stone[0];
    blueStone=stone[1];
    greenStone=stone[2];
    whiteStone=stone[3];
    blackStone=stone[4];
    int n=rand()%5;
    colour=ColourSelect(n);
    m_colour=n;
    grade=_grade;
}

//随机生成贵族，33300或者44000形式
/*
Aristocrats::Aristocrats() {
    int card[5]={0};
    int n=rand()%2;
    int m;
    switch(n){
        case 0:
            for (int i=0;i<3;i++){
                m=rand()%5;
                if (card[m]==0) {card[m]=3;}
                else {i--;}
            }
            break;
        case 1:
            for (int i=0;i<2;i++){
                m=rand()%5;
                if (card[m]==0) {card[m]=4;}
                else {i--;}
            }
    }
    red=card[0];
    blue=card[1];
    green=card[2];
    white=card[3];
    black=card[4];
    score=3;
}
*/

Aristocrats::Aristocrats(int n) {
    int ar[10][6]={
            {0,3,0,3,3,3},
            {3,3,3,0,0,3},
            {3,0,0,3,3,3},
            {4,0,4,0,0,3},
            {0,4,4,0,0,3},
            {4,0,0,0,4,3},
            {0,0,0,4,4,3},
            {0,3,3,3,0,3},
            {3,0,3,0,3,3},
            {0,4,0,4,0,3}
    };
    red=ar[n][0];
    blue=ar[n][1];
    green=ar[n][2];
    white=ar[n][3];
    black=ar[n][4];
    score=ar[n][5];
}

GameMune::GameMune(int num) {
    //初始化三个等级的发展卡
    for (int i=0;i<GradeInum;i++){
        m_devepI.push_back(new DevelopmentCard(1));
    }
    for (int i=0;i<GradeIInum;i++){
        m_devepII.push_back(new DevelopmentCard(2));
    }
    for (int i=0;i<GradeIIInum;i++){
        m_devepIII.push_back(new DevelopmentCard(3));
    }
    //初始化宝石数量
    switch (num){
        case 4:
            redStone.num=blueStone.num=greenStone.num=whiteStone.num=blackStone.num=7;
            break;
        case 3:
            redStone.num=blueStone.num=greenStone.num=whiteStone.num=blackStone.num=5;
            break;
        case 2:
            redStone.num=blueStone.num=greenStone.num=whiteStone.num=blackStone.num=4;
        default:
            break;
    }
    gold.num=5;
    //添加贵族,数量为人数+1
    vector<int> vv=GenerateDiffNumber(0,9,num+1);
    for (int i = 0; i < num+1; i++)
    {
        aristocrats.push_back(new Aristocrats(vv[i]));
    }
    //添加城市
    vector<int> v=GenerateDiffNumber(0,13,3);
    for (int i = 0; i < 3; i++)
    {
        extends.push_back(new Extend(v[i]));
    }
}

GameMune::~GameMune() {
    aristocrats.clear();
    m_players.clear();
    extends.clear();
    m_devepI.clear();
    m_devepII.clear();
    m_devepIII.clear();
}

bool GameMune::isWin(){
    int size=m_players.size();
    int maxscore=0,index=0;
    for(int i=0;i<size;i++){
        if(m_players.at(i)->getScore()>=maxscore){
            maxscore=m_players.at(i)->getScore();
            index=i;
        }
    }
    if (maxscore>=class_win_score){
        cout<<"Player "<<m_players[index]->getName()<<" Winner!"<<endl;
        for(int i=0;i<size;i++){
            cout<<m_players[i]->getName()<<"'s score: "<<m_players[i]->getScore()<<endl;
        }
        return true;
    }
    return false;
}

void GameMune::showDeskInfo(int num) {
    int gems[6]={m_players.at(num)->getredStoneNum(),m_players.at(num)->getblueStoneNum(),m_players.at(num)->getgreenStoneNum(),m_players.at(num)->getwhiteStoneNum(),m_players.at(num)->getblackStoneNum(),m_players.at(num)->getgoldNum()};
    cout << "----------DeskInfo------------------------------------------------------\n";
    cout << "----------Referent------------------------------------------------------\n";
    cout<<"redStoneNum:\t"<<redStone.num<<endl;
    cout<<"blueStoneNum:\t"<<blueStone.num<<endl;
    cout<<"greenStoneNum:\t"<<greenStone.num<<endl;
    cout<<"whiteStoneNum:\t"<<whiteStone.num<<endl;
    cout<<"blackStoneNum:\t"<<blackStone.num<<endl;
    cout<<"goldNum:\t"<<gold.num<<endl;
    cout << "----------CardsInfo-----------------------------------------------------\n";
    cout<<"grade\tcolour\tscore\tred\tblue\tgreen\twhite\tblack\n";
    cout<<"CardI ----- rest : "<<m_devepI.size()<<endl;
    for (int i=0;i<4;i++){
        m_devepI[i]->printInfo();
    }
    cout<<"CardII ----- rest : "<<m_devepII.size()<<endl;
    for (int i=0;i<4;i++){
        m_devepII[i]->printInfo();
    }
    cout<<"CardIII ----- rest : "<<m_devepIII.size()<<endl;
    for (int i=0;i<4;i++){
        m_devepIII[i]->printInfo();
    }
    cout << "----------PlyerInfo-----------------------------------------------------\n";
    for (int i=0;i<m_players.size();i++){
        cout<<"NAME:  "<<m_players[i]->getName()<<endl;
        cout<<"SCORE: "<<m_players[i]->getScore()<<endl;
    }
    cout << "----------Nobility-----------------------------------------------------\n";
    cout<<"red\tblue\tgreen\twhite\tblack\tscore\n";
    for(int i=0;i<aristocrats.size();i++){
        aristocrats[i]->printInfo();
    }
    cout << "----------MyselfInfo---------------------------------------------------\n";
    cout<<"NAME         : "<<m_players[num]->getName();
    cout<<"     --------------------Bonus"<<endl;
    cout<<"redStoneNum  : "<<gems[0]<<"                    redBonus  : "<<m_players[num]->Bonus[0]<<endl;
    cout<<"blueStoneNum : "<<gems[1]<<"                    blueBonus : "<<m_players[num]->Bonus[1]<<endl;
    cout<<"greenStoneNum: "<<gems[2]<<"                    greenBonus: "<<m_players[num]->Bonus[2]<<endl;
    cout<<"whiteStoneNum: "<<gems[3]<<"                    whiteBonus: "<<m_players[num]->Bonus[3]<<endl;
    cout<<"blackStoneNum: "<<gems[4]<<"                    blackBonus: "<<m_players[num]->Bonus[4]<<endl;
    cout<<"goldNum      : "<<gems[5]<<endl;
    cout<<"total        : "<<m_players[num]->getstoneSum()<<endl;
    cout<<"score        : "<<m_players[num]->getScore()<<endl;
    cout<<"------yourCards------";
    cout<<"CardNum:"<<m_players[num]->m_developmentCards.size()<<endl;
    cout<<"       grade\tcolour\tscore\tred\tblue\tgreen\twhite\tblack\n";
    for(int i=0;i<m_players[num]->m_developmentCards.size();i++){
        cout<<"card"<<i+1<<"\t: ";
        m_players[num]->m_developmentCards[i]->printInfo();
    }
    cout<<endl;
}

bool GameMune::isAristocrats(int num) {
    int m_bonus[5]={m_players.at(num)->Bonus[0],m_players.at(num)->Bonus[1],m_players.at(num)->Bonus[2],m_players.at(num)->Bonus[3],m_players.at(num)->Bonus[4]};
    for (int i=0;i<aristocrats.size();i++){
        if((m_bonus[0]>=aristocrats[i]->red)&&(m_bonus[1]>=aristocrats[i]->blue)&&(m_bonus[2]>=aristocrats[i]->green)&&(m_bonus[3]>=aristocrats[i]->white)&&(m_bonus[4]>=aristocrats[i]->black)){
            aristocrats.erase(aristocrats.begin()+i);
            int n=m_players.at(num)->getScore();
            m_players.at(num)->setScore(n+3);
            return true;
        }
    }
    return false;
}

bool GameMune::Take_three_different_gems(int num) {
    int choose[3]={0};
    bool isok=false;
    int sum=redStone.num+blueStone.num+greenStone.num+whiteStone.num+blackStone.num;
    int stone_num[6]={redStone.num,blueStone.num,greenStone.num,whiteStone.num,blackStone.num,1};
    if(sum==0){
        cout<<"there is no gem"<<endl;
        return false;
    }
    while(true){
        cout <<"1.redStone\t2.blueStone\t3.greenStone\t4.whiteStone\t5.blackStone\t6.none\totherNum.cancel\n";
        int realTake=0;
        for (int i=0;i<3;i++) {
            cout << "please select num"<<i+1<<": "<<endl;
            cin >> choose[i];
            if(!(choose[i]>=1&&choose[i]<=6)){return false;}
            choose[i]--;
            if(choose[i]!=5){
                realTake++;
            }
        }
        //不能选相同的
        if ((choose[0] == choose[2] && choose[0] != 5) || (choose[1] == choose[0] && choose[1] != 5)||(choose[1]==choose[2] && choose[2] != 5)){
            cout << "You can't choose to repeat" << endl;
        }
        //数量不足
        else if((stone_num[choose[0]]<1) || (stone_num[choose[1]]<1) || (stone_num[choose[2]]<1)){
            cout<<"Not enough gems"<<endl;
        }
        //超上限
        else if(realTake+m_players.at(num)->getstoneSum()>StoneLimit){
            cout<<"You take too many gems"<<endl;
        }
        else {
            isok = addGemsNum(choose, num);
            break;
        }
    }
    return true;
}

bool GameMune::Take_two_same_gems(int num) {
    if(redStone.num<4&&blueStone.num<4&&greenStone.num<4&&whiteStone.num<4&&blackStone.num<4){
        cout<<"there are not enough gems"<<endl;
        return false;
    }
    if(m_players[num]->getstoneSum()+2>StoneLimit){
        cout<<"you have too many gems"<<endl;
        return false;
    }
    int choose;
    cout <<"1.redStone\t2.blueStone\t3.greenStone\t4.whiteStone\t5.blackStone\t6.cancel\n";
    while(true){
    cout<<"please select num : "<<endl;  cin>>choose;
    int gems=0;
        switch(choose){
            case 1:
                if (redStone.num<4){
                    cout<<"too less redStone"<<endl;
                    continue;
                }
                gems=m_players[num]->getredStoneNum()+2;
                m_players[num]->setRredStoneNum(gems);
                redStone.num-=2;
                return true;
            case 2:
                if (blueStone.num<4){
                    cout<<"too less blueStone"<<endl;
                    continue;
                }
                gems=m_players[num]->getblueStoneNum()+2;
                m_players[num]->setRblueStoneNum(gems);
                blueStone.num-=2;
                return true;
            case 3:
                if (greenStone.num<4){
                    cout<<"too less greenStone"<<endl;
                    continue;
                }
                gems=m_players[num]->getgreenStoneNum()+2;
                m_players[num]->setRgreenStoneNum(gems);
                greenStone.num-=2;
                return true;
            case 4:
                if (whiteStone.num<4){
                    cout<<"too less whiteStone"<<endl;
                    continue;
                }
                gems=m_players[num]->getwhiteStoneNum()+2;
                m_players[num]->setRwhiteStoneNum(gems);
                whiteStone.num-=2;
                return true;
            case 5:
                if (blackStone.num<4){
                    cout<<"too less blackStone"<<endl;
                    continue;
                }
                gems=m_players[num]->getblackStoneNum()+2;
                m_players[num]->setRblackStoneNum(gems);
                blackStone.num-=2;
                return true;
            case 6:
                return false;
            default:
                break;
        }
    }
    return true;
}

bool GameMune::addGemsNum(int *choose, int num) {
    int gems[5]={m_players.at(num)->getredStoneNum(),m_players.at(num)->getblueStoneNum(),m_players.at(num)->getgreenStoneNum(),m_players.at(num)->getwhiteStoneNum(),m_players.at(num)->getblackStoneNum()};
    for (int i=0;i<3;i++){
        switch(choose[i]){
            case 0:
                gems[0]++;
                m_players[num]->setRredStoneNum(gems[0]);
                redStone.num--;
                continue;
            case 1:
                gems[1]++;
                m_players[num]->setRblueStoneNum(gems[1]);
                blueStone.num--;
                continue;
            case 2:
                gems[2]++;
                m_players[num]->setRgreenStoneNum(gems[2]);
                greenStone.num--;
                continue;
            case 3:
                gems[3]++;
                m_players[num]->setRwhiteStoneNum(gems[3]);
                whiteStone.num--;
                continue;
            case 4:
                gems[4]++;
                m_players[num]->setRblackStoneNum(gems[4]);
                blackStone.num--;
                continue;
            case 5:
                continue;
        }
    }
    return true;
}

bool GameMune::ispushCar(int num, int carNum) {
    switch (carNum/4){
        case 0:
            m_players[num]->m_developmentCards.push_back(m_devepI[carNum%4]);
            m_devepI.erase(m_devepI.begin()+(+carNum%4));
            return true;
        case 1:
            m_players[num]->m_developmentCards.push_back(m_devepII[carNum%4]);
            m_devepII.erase(m_devepII.begin()+(+carNum%4));
            return true;
        case 2:
            m_players[num]->m_developmentCards.push_back(m_devepIII[carNum%4]);
            m_devepIII.erase(m_devepIII.begin()+(+carNum%4));
            return true;
    }
    return false;
}

bool GameMune::TakeCarAndGold(int num) {
    if (m_players[num]->m_developmentCards.size()>=3){
        cout<<"you already have 3 cards"<<endl;
        return false;
    }
    if(m_players[num]->getstoneSum()<StoneLimit && gold.num>0){
        int temp_gold_num=m_players[num]->getgoldNum();
        m_players[num]->setRgoldNum(temp_gold_num+1);
        gold.num--;
    }
    int choose=0;
        cout<<"Please select card (from 1 to 12, input other number to cancel) : "<<endl;
        cin >> choose;
    if (choose>12||choose<1){return false;}
    bool isok=ispushCar(num,choose-1);
    return isok;
}

bool GameMune::buyCarToInfo(int num) {
    int choose;
    bool isok=false;
    cout<<"buy card---------------------"<<endl;
    cout<<"1.buy reserved card"<<endl;
    cout<<"2.buy card on desk"<<endl;
    cout<<"3.skip"<<endl;
    cout<<"choose : "<<endl;cin>>choose;
    switch(choose){
        case 1:
            isok=buyCarMyself(num);
            break;
        case 2:
            isok=buyCarOther(num);
            break;
        case 3:
            isok=true;
        default:
            break;
    }
    return isok;
}

bool GameInfo::buyCarMyself(int num) {
    int choose;
    int size=m_players[num]->m_developmentCards.size();
    if (size==0){
        cout<<"you don't have any card"<<endl;
        return false;
    }
    cout <<"your card info"<<endl;
    cout<<"      grade\t\tcolour\tscore\tred\tblue\tgreen\twhite\tblack\n";
    for (int i = 0; i < size; i++)
    {
        cout << "Card" << i + 1 <<":" ;
        m_players[num]->m_developmentCards[i]->printInfo();
    }
    cout<<"you choose : "<<endl;
    cin>>choose;
    bool isok=StoneToCar(num,choose-1);
    return isok;
}

bool GameInfo::buyCarOther(int num) {
    int choose_grade,choose_num;
    size_t size = m_devepI.size() + m_devepII.size() + m_devepI.size();
    if (size == 0)
    {
        cout << "没有卡"<<endl;
        return false;
    }
    cout<<"please choose grade : "<<endl;cin>>choose_grade;
    cout<<"please choose num   : "<<endl;cin>>choose_num;
    bool isok=false;
    switch(choose_grade){
        case 1:
            isok=StoneToCar1(num,choose_num-1);
            break;
        case 2:
            isok=StoneToCar2(num,choose_num-1);
            break;
        case 3:
            isok=StoneToCar3(num,choose_num-1);
            break;
        default:
            break;
    }
    return isok;
}

bool GameInfo::StoneToCar(int num, int carId) {
    int goldNum=m_players.at(num)->getgoldNum();
    int gems1=m_players.at(num)->getredStoneNum()+m_players.at(num)->Bonus[0];
    int gems2=m_players.at(num)->getblueStoneNum()+m_players.at(num)->Bonus[1];
    int gems3=m_players.at(num)->getgreenStoneNum()+m_players.at(num)->Bonus[2];
    int gems4=m_players.at(num)->getwhiteStoneNum()+m_players.at(num)->Bonus[3];
    int gems5=m_players.at(num)->getblackStoneNum()+m_players.at(num)->Bonus[4];
    int card_gems1=m_players.at(num)->m_developmentCards[carId]->redStone;
    int card_gems2=m_players.at(num)->m_developmentCards[carId]->blueStone;
    int card_gems3=m_players.at(num)->m_developmentCards[carId]->greenStone;
    int card_gems4=m_players.at(num)->m_developmentCards[carId]->whiteStone;
    int card_gems5=m_players.at(num)->m_developmentCards[carId]->blackStone;
    int diff1=((gems1-card_gems1) > 0) ? 0 : (gems1-card_gems1) ;
    int diff2=((gems2-card_gems2) > 0) ? 0 : (gems2-card_gems2) ;
    int diff3=((gems3-card_gems3) > 0) ? 0 : (gems3-card_gems3) ;
    int diff4=((gems4-card_gems4) > 0) ? 0 : (gems4-card_gems4) ;
    int diff5=((gems5-card_gems5) > 0) ? 0 : (gems5-card_gems5) ;
    //int m_sum=gems1+gems2+gems3+gems4+gems5;
    //int card_sum=card_gems1+card_gems2+card_gems3+card_gems4+card_gems5;
    int diff_sum=diff1+diff2+diff3+diff4+diff5;
    int bonus_colour=m_players.at(num)->m_developmentCards[carId]->m_colour;
    int add_score=m_players.at(num)->m_developmentCards[carId]->score;
    if(diff_sum+m_players.at(num)->getgoldNum()<0){
        cout<<"You don't have enough gems"<<endl;
        return false;
    }
    if(diff_sum==0){            //不需要黄金
        //交宝石
        int give_gems1=(card_gems1-m_players.at(num)->Bonus[0])>0 ? (card_gems1-m_players.at(num)->Bonus[0]) : 0; //不能给负数
        m_players[num]->setRredStoneNum(m_players.at(num)->getredStoneNum()-give_gems1);
        int give_gems2=(card_gems2-m_players.at(num)->Bonus[1])>0 ? (card_gems2-m_players.at(num)->Bonus[1]) : 0;
        m_players[num]->setRblueStoneNum(m_players.at(num)->getblueStoneNum()-give_gems2);
        int give_gems3=(card_gems3-m_players.at(num)->Bonus[2])>0 ? (card_gems3-m_players.at(num)->Bonus[2]) : 0;
        m_players[num]->setRgreenStoneNum(m_players.at(num)->getgreenStoneNum()-give_gems3);
        int give_gems4=(card_gems4-m_players.at(num)->Bonus[3])>0 ? (card_gems4-m_players.at(num)->Bonus[3]) : 0;
        m_players[num]->setRwhiteStoneNum(m_players.at(num)->getwhiteStoneNum()-give_gems4);
        int give_gems5=(card_gems5-m_players.at(num)->Bonus[4])>0 ? (card_gems5-m_players.at(num)->Bonus[4]) : 0;
        m_players[num]->setRblackStoneNum(m_players.at(num)->getblackStoneNum()-give_gems5);
        //银行收宝石
        redStone.num+=give_gems1;
        blueStone.num+=give_gems2;
        greenStone.num+=give_gems3;
        whiteStone.num+=give_gems4;
        blackStone.num+=give_gems5;
        //加红利
        m_players[num]->Bonus[bonus_colour]++;
        //加分
        m_players[num]->setScore(m_players[num]->getScore()+add_score);
        //弃牌
        m_players[num]->m_developmentCards.erase(m_players[num]->m_developmentCards.begin()+carId);
    }
    else{                       //需要黄金
        if(diff1==0){           //if宝石足够
            int give_gems1=(card_gems1-m_players.at(num)->Bonus[0])>0 ? (card_gems1-m_players.at(num)->Bonus[0]) : 0; //不能给负数
            m_players[num]->setRredStoneNum(m_players.at(num)->getredStoneNum()-give_gems1);
            redStone.num+=give_gems1;
        }
        else {                  //else宝石不够
            redStone.num+=m_players[num]->getredStoneNum();
            m_players[num]->setRredStoneNum(0);
            gold.num+=(0-diff1);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff1);
        }
        if(diff2==0){
            int give_gems2=(card_gems2-m_players.at(num)->Bonus[1])>0 ? (card_gems2-m_players.at(num)->Bonus[1]) : 0;
            m_players[num]->setRblueStoneNum(m_players.at(num)->getblueStoneNum()-give_gems2);
            blueStone.num+=give_gems2;
        }
        else {
            blueStone.num+=m_players[num]->getblueStoneNum();
            m_players[num]->setRblueStoneNum(0);
            gold.num+=(0-diff2);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff2);
        }
        if(diff3==0){
            int give_gems3=(card_gems3-m_players.at(num)->Bonus[2])>0 ? (card_gems3-m_players.at(num)->Bonus[2]) : 0;
            m_players[num]->setRgreenStoneNum(m_players.at(num)->getgreenStoneNum()-give_gems3);
            greenStone.num+=give_gems3;
        }
        else {
            greenStone.num+=m_players[num]->getgreenStoneNum();
            m_players[num]->setRgreenStoneNum(0);
            gold.num+=(0-diff3);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff3);
        }
        if(diff4==0){
            int give_gems4=(card_gems4-m_players.at(num)->Bonus[3])>0 ? (card_gems4-m_players.at(num)->Bonus[3]) : 0;
            m_players[num]->setRwhiteStoneNum(m_players.at(num)->getwhiteStoneNum()-give_gems4);
            whiteStone.num+=give_gems4;
        }
        else {
            whiteStone.num+=m_players[num]->getwhiteStoneNum();
            m_players[num]->setRwhiteStoneNum(0);
            gold.num+=(0-diff4);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff4);
        }
        if(diff5==0){
            int give_gems5=(card_gems5-m_players.at(num)->Bonus[4])>0 ? (card_gems5-m_players.at(num)->Bonus[4]) : 0;
            m_players[num]->setRblackStoneNum(m_players.at(num)->getblackStoneNum()-give_gems5);
            blackStone.num+=give_gems5;
        }
        else {
            blackStone.num+=m_players[num]->getblackStoneNum();
            m_players[num]->setRblackStoneNum(0);
            gold.num+=(0-diff5);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff5);
        }
        //加红利
        m_players[num]->Bonus[bonus_colour]++;
        //加分
        m_players[num]->setScore(m_players[num]->getScore()+add_score);
        //弃牌
        m_players[num]->m_developmentCards.erase(m_players[num]->m_developmentCards.begin()+carId);
    }
    return true;
}

bool GameInfo::StoneToCar1(int num, int carId) {
    int goldNum=m_players.at(num)->getgoldNum();
    int gems1=m_players.at(num)->getredStoneNum()+m_players.at(num)->Bonus[0];
    int gems2=m_players.at(num)->getblueStoneNum()+m_players.at(num)->Bonus[1];
    int gems3=m_players.at(num)->getgreenStoneNum()+m_players.at(num)->Bonus[2];
    int gems4=m_players.at(num)->getwhiteStoneNum()+m_players.at(num)->Bonus[3];
    int gems5=m_players.at(num)->getblackStoneNum()+m_players.at(num)->Bonus[4];
    int card_gems1=m_devepI[carId]->redStone;
    int card_gems2=m_devepI[carId]->blueStone;
    int card_gems3=m_devepI[carId]->greenStone;
    int card_gems4=m_devepI[carId]->whiteStone;
    int card_gems5=m_devepI[carId]->blackStone;
    int diff1=((gems1-card_gems1) > 0) ? 0 : (gems1-card_gems1) ;
    int diff2=((gems2-card_gems2) > 0) ? 0 : (gems2-card_gems2) ;
    int diff3=((gems3-card_gems3) > 0) ? 0 : (gems3-card_gems3) ;
    int diff4=((gems4-card_gems4) > 0) ? 0 : (gems4-card_gems4) ;
    int diff5=((gems5-card_gems5) > 0) ? 0 : (gems5-card_gems5) ;
    int diff_sum=diff1+diff2+diff3+diff4+diff5;
    if(diff_sum+m_players.at(num)->getgoldNum()<0){
        cout<<"You don't have enough gems"<<endl;
        return false;
    }
    int bonus_colour=m_devepI[carId]->m_colour;
    int add_score=m_devepI[carId]->score;
    if(diff_sum==0){            //不需要黄金
        //交宝石
        int give_gems1=(card_gems1-m_players.at(num)->Bonus[0])>0 ? (card_gems1-m_players.at(num)->Bonus[0]) : 0; //不能给负数
        m_players[num]->setRredStoneNum(m_players.at(num)->getredStoneNum()-give_gems1);
        int give_gems2=(card_gems2-m_players.at(num)->Bonus[1])>0 ? (card_gems2-m_players.at(num)->Bonus[1]) : 0;
        m_players[num]->setRblueStoneNum(m_players.at(num)->getblueStoneNum()-give_gems2);
        int give_gems3=(card_gems3-m_players.at(num)->Bonus[2])>0 ? (card_gems3-m_players.at(num)->Bonus[2]) : 0;
        m_players[num]->setRgreenStoneNum(m_players.at(num)->getgreenStoneNum()-give_gems3);
        int give_gems4=(card_gems4-m_players.at(num)->Bonus[3])>0 ? (card_gems4-m_players.at(num)->Bonus[3]) : 0;
        m_players[num]->setRwhiteStoneNum(m_players.at(num)->getwhiteStoneNum()-give_gems4);
        int give_gems5=(card_gems5-m_players.at(num)->Bonus[4])>0 ? (card_gems5-m_players.at(num)->Bonus[4]) : 0;
        m_players[num]->setRblackStoneNum(m_players.at(num)->getblackStoneNum()-give_gems5);
        //银行收宝石
        redStone.num+=give_gems1;
        blueStone.num+=give_gems2;
        greenStone.num+=give_gems3;
        whiteStone.num+=give_gems4;
        blackStone.num+=give_gems5;
        //加红利
        m_players[num]->Bonus[bonus_colour]++;
        //加分
        m_players[num]->setScore(m_players[num]->getScore()+add_score);
        //弃牌
        m_devepI.erase(m_devepI.begin()+carId);
    }
    else{                       //需要黄金
        if(diff1==0){           //if宝石足够
            int give_gems1=(card_gems1-m_players.at(num)->Bonus[0])>0 ? (card_gems1-m_players.at(num)->Bonus[0]) : 0; //不能给负数
            m_players[num]->setRredStoneNum(m_players.at(num)->getredStoneNum()-give_gems1);
            redStone.num+=give_gems1;
        }
        else {                  //else宝石不够
            redStone.num+=m_players[num]->getredStoneNum();
            m_players[num]->setRredStoneNum(0);
            gold.num+=(0-diff1);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff1);
        }
        if(diff2==0){
            int give_gems2=(card_gems2-m_players.at(num)->Bonus[1])>0 ? (card_gems2-m_players.at(num)->Bonus[1]) : 0;
            m_players[num]->setRblueStoneNum(m_players.at(num)->getblueStoneNum()-give_gems2);
            blueStone.num+=give_gems2;
        }
        else {
            blueStone.num+=m_players[num]->getblueStoneNum();
            m_players[num]->setRblueStoneNum(0);
            gold.num+=(0-diff2);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff2);
        }
        if(diff3==0){
            int give_gems3=(card_gems3-m_players.at(num)->Bonus[2])>0 ? (card_gems3-m_players.at(num)->Bonus[2]) : 0;
            m_players[num]->setRgreenStoneNum(m_players.at(num)->getgreenStoneNum()-give_gems3);
            greenStone.num+=give_gems3;
        }
        else {
            greenStone.num+=m_players[num]->getgreenStoneNum();
            m_players[num]->setRgreenStoneNum(0);
            gold.num+=(0-diff3);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff3);
        }
        if(diff4==0){
            int give_gems4=(card_gems4-m_players.at(num)->Bonus[3])>0 ? (card_gems4-m_players.at(num)->Bonus[3]) : 0;
            m_players[num]->setRwhiteStoneNum(m_players.at(num)->getwhiteStoneNum()-give_gems4);
            whiteStone.num+=give_gems4;
        }
        else {
            whiteStone.num+=m_players[num]->getwhiteStoneNum();
            m_players[num]->setRwhiteStoneNum(0);
            gold.num+=(0-diff4);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff4);
        }
        if(diff5==0){
            int give_gems5=(card_gems5-m_players.at(num)->Bonus[4])>0 ? (card_gems5-m_players.at(num)->Bonus[4]) : 0;
            m_players[num]->setRblackStoneNum(m_players.at(num)->getblackStoneNum()-give_gems5);
            blackStone.num+=give_gems5;
        }
        else {
            blackStone.num+=m_players[num]->getblackStoneNum();
            m_players[num]->setRblackStoneNum(0);
            gold.num+=(0-diff5);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff5);
        }
        //加红利
        m_players[num]->Bonus[bonus_colour]++;
        //加分
        m_players[num]->setScore(m_players[num]->getScore()+add_score);
        //弃牌
        m_devepI.erase(m_devepI.begin()+carId);
    }
    return true;
}

bool GameInfo::StoneToCar2(int num, int carId) {
    int goldNum=m_players.at(num)->getgoldNum();
    int gems1=m_players.at(num)->getredStoneNum()+m_players.at(num)->Bonus[0];
    int gems2=m_players.at(num)->getblueStoneNum()+m_players.at(num)->Bonus[1];
    int gems3=m_players.at(num)->getgreenStoneNum()+m_players.at(num)->Bonus[2];
    int gems4=m_players.at(num)->getwhiteStoneNum()+m_players.at(num)->Bonus[3];
    int gems5=m_players.at(num)->getblackStoneNum()+m_players.at(num)->Bonus[4];
    int card_gems1=m_devepII[carId]->redStone;
    int card_gems2=m_devepII[carId]->blueStone;
    int card_gems3=m_devepII[carId]->greenStone;
    int card_gems4=m_devepII[carId]->whiteStone;
    int card_gems5=m_devepII[carId]->blackStone;
    int diff1=((gems1-card_gems1) > 0) ? 0 : (gems1-card_gems1) ;
    int diff2=((gems2-card_gems2) > 0) ? 0 : (gems2-card_gems2) ;
    int diff3=((gems3-card_gems3) > 0) ? 0 : (gems3-card_gems3) ;
    int diff4=((gems4-card_gems4) > 0) ? 0 : (gems4-card_gems4) ;
    int diff5=((gems5-card_gems5) > 0) ? 0 : (gems5-card_gems5) ;
    int diff_sum=diff1+diff2+diff3+diff4+diff5;
    if(diff_sum+m_players.at(num)->getgoldNum()<0){
        cout<<"You don't have enough gems"<<endl;
        return false;
    }
    int bonus_colour=m_devepII[carId]->m_colour;
    int add_score=m_devepII[carId]->score;
    if(diff_sum==0){            //不需要黄金
        //交宝石
        int give_gems1=(card_gems1-m_players.at(num)->Bonus[0])>0 ? (card_gems1-m_players.at(num)->Bonus[0]) : 0; //不能给负数
        m_players[num]->setRredStoneNum(m_players.at(num)->getredStoneNum()-give_gems1);
        int give_gems2=(card_gems2-m_players.at(num)->Bonus[1])>0 ? (card_gems2-m_players.at(num)->Bonus[1]) : 0;
        m_players[num]->setRblueStoneNum(m_players.at(num)->getblueStoneNum()-give_gems2);
        int give_gems3=(card_gems3-m_players.at(num)->Bonus[2])>0 ? (card_gems3-m_players.at(num)->Bonus[2]) : 0;
        m_players[num]->setRgreenStoneNum(m_players.at(num)->getgreenStoneNum()-give_gems3);
        int give_gems4=(card_gems4-m_players.at(num)->Bonus[3])>0 ? (card_gems4-m_players.at(num)->Bonus[3]) : 0;
        m_players[num]->setRwhiteStoneNum(m_players.at(num)->getwhiteStoneNum()-give_gems4);
        int give_gems5=(card_gems5-m_players.at(num)->Bonus[4])>0 ? (card_gems5-m_players.at(num)->Bonus[4]) : 0;
        m_players[num]->setRblackStoneNum(m_players.at(num)->getblackStoneNum()-give_gems5);
        //银行收宝石
        redStone.num+=give_gems1;
        blueStone.num+=give_gems2;
        greenStone.num+=give_gems3;
        whiteStone.num+=give_gems4;
        blackStone.num+=give_gems5;
        //加红利
        m_players[num]->Bonus[bonus_colour]++;
        //加分
        m_players[num]->setScore(m_players[num]->getScore()+add_score);
        //弃牌
        m_devepII.erase(m_devepII.begin()+carId);
    }
    else{                       //需要黄金
        if(diff1==0){           //if宝石足够
            int give_gems1=(card_gems1-m_players.at(num)->Bonus[0])>0 ? (card_gems1-m_players.at(num)->Bonus[0]) : 0; //不能给负数
            m_players[num]->setRredStoneNum(m_players.at(num)->getredStoneNum()-give_gems1);
            redStone.num+=give_gems1;
        }
        else {                  //else宝石不够
            redStone.num+=m_players[num]->getredStoneNum();
            m_players[num]->setRredStoneNum(0);
            gold.num+=(0-diff1);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff1);
        }
        if(diff2==0){
            int give_gems2=(card_gems2-m_players.at(num)->Bonus[1])>0 ? (card_gems2-m_players.at(num)->Bonus[1]) : 0;
            m_players[num]->setRblueStoneNum(m_players.at(num)->getblueStoneNum()-give_gems2);
            blueStone.num+=give_gems2;
        }
        else {
            blueStone.num+=m_players[num]->getblueStoneNum();
            m_players[num]->setRblueStoneNum(0);
            gold.num+=(0-diff2);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff2);
        }
        if(diff3==0){
            int give_gems3=(card_gems3-m_players.at(num)->Bonus[2])>0 ? (card_gems3-m_players.at(num)->Bonus[2]) : 0;
            m_players[num]->setRgreenStoneNum(m_players.at(num)->getgreenStoneNum()-give_gems3);
            greenStone.num+=give_gems3;
        }
        else {
            greenStone.num+=m_players[num]->getgreenStoneNum();
            m_players[num]->setRgreenStoneNum(0);
            gold.num+=(0-diff3);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff3);
        }
        if(diff4==0){
            int give_gems4=(card_gems4-m_players.at(num)->Bonus[3])>0 ? (card_gems4-m_players.at(num)->Bonus[3]) : 0;
            m_players[num]->setRwhiteStoneNum(m_players.at(num)->getwhiteStoneNum()-give_gems4);
            whiteStone.num+=give_gems4;
        }
        else {
            whiteStone.num+=m_players[num]->getwhiteStoneNum();
            m_players[num]->setRwhiteStoneNum(0);
            gold.num+=(0-diff4);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff4);
        }
        if(diff5==0){
            int give_gems5=(card_gems5-m_players.at(num)->Bonus[4])>0 ? (card_gems5-m_players.at(num)->Bonus[4]) : 0;
            m_players[num]->setRblackStoneNum(m_players.at(num)->getblackStoneNum()-give_gems5);
            blackStone.num+=give_gems5;
        }
        else {
            blackStone.num+=m_players[num]->getblackStoneNum();
            m_players[num]->setRblackStoneNum(0);
            gold.num+=(0-diff5);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff5);
        }
        //加红利
        m_players[num]->Bonus[bonus_colour]++;
        //加分
        m_players[num]->setScore(m_players[num]->getScore()+add_score);
        //弃牌
        m_devepII.erase(m_devepII.begin()+carId);
    }
    return true;
}

bool GameInfo::StoneToCar3(int num, int carId) {
    int goldNum=m_players.at(num)->getgoldNum();
    int gems1=m_players.at(num)->getredStoneNum()+m_players.at(num)->Bonus[0];
    int gems2=m_players.at(num)->getblueStoneNum()+m_players.at(num)->Bonus[1];
    int gems3=m_players.at(num)->getgreenStoneNum()+m_players.at(num)->Bonus[2];
    int gems4=m_players.at(num)->getwhiteStoneNum()+m_players.at(num)->Bonus[3];
    int gems5=m_players.at(num)->getblackStoneNum()+m_players.at(num)->Bonus[4];
    int card_gems1=m_devepIII[carId]->redStone;
    int card_gems2=m_devepIII[carId]->blueStone;
    int card_gems3=m_devepIII[carId]->greenStone;
    int card_gems4=m_devepIII[carId]->whiteStone;
    int card_gems5=m_devepIII[carId]->blackStone;
    int diff1=((gems1-card_gems1) > 0) ? 0 : (gems1-card_gems1) ;
    int diff2=((gems2-card_gems2) > 0) ? 0 : (gems2-card_gems2) ;
    int diff3=((gems3-card_gems3) > 0) ? 0 : (gems3-card_gems3) ;
    int diff4=((gems4-card_gems4) > 0) ? 0 : (gems4-card_gems4) ;
    int diff5=((gems5-card_gems5) > 0) ? 0 : (gems5-card_gems5) ;
    int diff_sum=diff1+diff2+diff3+diff4+diff5;
    if(diff_sum+m_players.at(num)->getgoldNum()<0){
        cout<<"You don't have enough gems"<<endl;
        return false;
    }
    int bonus_colour=m_devepIII[carId]->m_colour;
    int add_score=m_devepIII[carId]->score;
    if(diff_sum==0){            //不需要黄金
        //交宝石
        int give_gems1=(card_gems1-m_players.at(num)->Bonus[0])>0 ? (card_gems1-m_players.at(num)->Bonus[0]) : 0; //不能给负数
        m_players[num]->setRredStoneNum(m_players.at(num)->getredStoneNum()-give_gems1);
        int give_gems2=(card_gems2-m_players.at(num)->Bonus[1])>0 ? (card_gems2-m_players.at(num)->Bonus[1]) : 0;
        m_players[num]->setRblueStoneNum(m_players.at(num)->getblueStoneNum()-give_gems2);
        int give_gems3=(card_gems3-m_players.at(num)->Bonus[2])>0 ? (card_gems3-m_players.at(num)->Bonus[2]) : 0;
        m_players[num]->setRgreenStoneNum(m_players.at(num)->getgreenStoneNum()-give_gems3);
        int give_gems4=(card_gems4-m_players.at(num)->Bonus[3])>0 ? (card_gems4-m_players.at(num)->Bonus[3]) : 0;
        m_players[num]->setRwhiteStoneNum(m_players.at(num)->getwhiteStoneNum()-give_gems4);
        int give_gems5=(card_gems5-m_players.at(num)->Bonus[4])>0 ? (card_gems5-m_players.at(num)->Bonus[4]) : 0;
        m_players[num]->setRblackStoneNum(m_players.at(num)->getblackStoneNum()-give_gems5);
        //银行收宝石
        redStone.num+=give_gems1;
        blueStone.num+=give_gems2;
        greenStone.num+=give_gems3;
        whiteStone.num+=give_gems4;
        blackStone.num+=give_gems5;
        //加红利
        m_players[num]->Bonus[bonus_colour]++;
        //加分
        m_players[num]->setScore(m_players[num]->getScore()+add_score);
        //弃牌
        m_devepIII.erase(m_devepIII.begin()+carId);
    }
    else{                       //需要黄金
        if(diff1==0){           //if宝石足够
            int give_gems1=(card_gems1-m_players.at(num)->Bonus[0])>0 ? (card_gems1-m_players.at(num)->Bonus[0]) : 0; //不能给负数
            m_players[num]->setRredStoneNum(m_players.at(num)->getredStoneNum()-give_gems1);
            redStone.num+=give_gems1;
        }
        else {                  //else宝石不够
            redStone.num+=m_players[num]->getredStoneNum();
            m_players[num]->setRredStoneNum(0);
            gold.num+=(0-diff1);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff1);
        }
        if(diff2==0){
            int give_gems2=(card_gems2-m_players.at(num)->Bonus[1])>0 ? (card_gems2-m_players.at(num)->Bonus[1]) : 0;
            m_players[num]->setRblueStoneNum(m_players.at(num)->getblueStoneNum()-give_gems2);
            blueStone.num+=give_gems2;
        }
        else {
            blueStone.num+=m_players[num]->getblueStoneNum();
            m_players[num]->setRblueStoneNum(0);
            gold.num+=(0-diff2);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff2);
        }
        if(diff3==0){
            int give_gems3=(card_gems3-m_players.at(num)->Bonus[2])>0 ? (card_gems3-m_players.at(num)->Bonus[2]) : 0;
            m_players[num]->setRgreenStoneNum(m_players.at(num)->getgreenStoneNum()-give_gems3);
            greenStone.num+=give_gems3;
        }
        else {
            greenStone.num+=m_players[num]->getgreenStoneNum();
            m_players[num]->setRgreenStoneNum(0);
            gold.num+=(0-diff3);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff3);
        }
        if(diff4==0){
            int give_gems4=(card_gems4-m_players.at(num)->Bonus[3])>0 ? (card_gems4-m_players.at(num)->Bonus[3]) : 0;
            m_players[num]->setRwhiteStoneNum(m_players.at(num)->getwhiteStoneNum()-give_gems4);
            whiteStone.num+=give_gems4;
        }
        else {
            whiteStone.num+=m_players[num]->getwhiteStoneNum();
            m_players[num]->setRwhiteStoneNum(0);
            gold.num+=(0-diff4);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff4);
        }
        if(diff5==0){
            int give_gems5=(card_gems5-m_players.at(num)->Bonus[4])>0 ? (card_gems5-m_players.at(num)->Bonus[4]) : 0;
            m_players[num]->setRblackStoneNum(m_players.at(num)->getblackStoneNum()-give_gems5);
            blackStone.num+=give_gems5;
        }
        else {
            blackStone.num+=m_players[num]->getblackStoneNum();
            m_players[num]->setRblackStoneNum(0);
            gold.num+=(0-diff5);
            m_players[num]->setRgoldNum(m_players[num]->getgoldNum()+diff5);
        }
        //加红利
        m_players[num]->Bonus[bonus_colour]++;
        //加分
        m_players[num]->setScore(m_players[num]->getScore()+add_score);
        //弃牌
        m_devepIII.erase(m_devepIII.begin()+carId);
    }
    return true;
}

void GameInfo::Menu(int num) {
    bool isok=false;
    int choose;
    while(true){
        cout<<"1.take three different gems"<<endl;
        cout<<"2.take two same gems"<<endl;
        cout<<"3.reserve a card"<<endl;
        cout<<"4.buy a card"<<endl;
        cout<<"5.skip"<<endl;
        cout<<"please choose : "<<endl;
        cin>>choose;
        switch(choose) {
            case 1:
                isok = Take_three_different_gems(num);
                break;
            case 2:
                isok = Take_two_same_gems(num);
                break;
            case 3:
                isok = TakeCarAndGold(num);
                break;
            case 4:
                isok = buyCarToInfo(num);
                break;
            case 5:
                return;
            default:
                break;
        }
        if(isok){
            return;
        }
        cout<<"select again\n";
    }
}

int classGame(GameInfo* gameMune){
    cout<<"input win score : "<<endl;
    cin>>gameMune->class_win_score;
    while(true){
        for(int i=0;i<gameMune->m_players.size();i++){
            gameMune->showDeskInfo(i);
            gameMune->Menu(i);
            gameMune->isAristocrats(i);
            system("cls");
            system("pause");
        }
        bool isWinner = gameMune->isWin();
        if(isWinner){
            return 0;
        }
    }
}

int StartGame(int num){
    GameInfo* gameMune=new GameMune(num);
    string name;
    for (int i=0;i<num;i++){
        cout<<"input player's name : "<<endl; cin>>name;
        gameMune->m_players.push_back(new Player(name));
    }
    cout<<"1.standard game"<<endl;
    cout<<"2.extended game"<<endl;
    cout<<"3.exit"<<endl;
    cout<<"choose : "<<endl;
    int choose;
    cin>>choose;
    switch(choose){
        case 1:
            classGame(gameMune);
            break;
        case 2:
            extendGame(gameMune);
            break;
        case 3:
            return 0;
        default:
            break;
    }
    return 0;
}
