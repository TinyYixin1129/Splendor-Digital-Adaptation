#include "extend.h"
void Extend::printInfo() const {
    cout<<"score:"<<score;
    if (red!=0){cout<<"\tred:"<<red;}
    if (blue!=0){cout<<"\tblue:"<<blue;}
    if (green!=0){cout<<"\tgreen:"<<green;}
    if (white!=0){cout<<"\twhite:"<<white;}
    if (black!=0){cout<<"\tblack:"<<black;}
    if (grey!=0){cout<<"\tgrey:"<<grey;}
    cout<<endl;
}

Extend::Extend(int n) {
    int ex[14][7]={
            {11,3,3,0,3,3,0},
            {11,1,0,1,1,1,0},
            {12,0,0,0,0,0,6},
            {13,4,0,0,0,3,0},
            {13,0,3,4,0,0,0},
            {13,3,0,0,4,0,0},
            {13,0,4,0,3,0,0},
            {13,0,0,3,0,4,0},
            {13,2,2,2,2,2,0},
            {14,2,1,1,2,2,0},
            {14,0,4,0,0,4,0},
            {15,0,0,0,0,0,5},
            {16,1,1,1,1,1,0},
            {17,0,0,0,0,0,0}
    };
    score=ex[n][0];
    red=ex[n][1];
    blue=ex[n][2];
    green=ex[n][3];
    white=ex[n][4];
    black=ex[n][5];
    grey=ex[n][6];
    isreached=false;
}

bool GameMune::is_reach_extend(int num) {
    int t_bonus[5];
    int t_score=0;
    for(int j=0;j<5;j++){
        t_bonus[j]=m_players.at(num)->Bonus[j];
    }
    t_score=m_players.at(num)->getScore();
    for(int k=0;k<3;k++){
        if(extends[k]->grey!=0){
            int max_bonus=0;
            for(int l=0;l<5;l++){
                if(t_bonus[l]>=max_bonus){
                    max_bonus=t_bonus[l];
                }
            }
            if((max_bonus>=extends[k]->grey) && (t_score>=extends[k]->score) && (!extends[k]->isreached) && (!m_players.at(num)->is_extend_get)){
                extends[k]->isreached=true;
                m_players.at(num)->is_extend_get=true;
                return true;
            }
        }
        else{
            if((t_bonus[0]>=extends[k]->red)&&(t_bonus[1]>=extends[k]->blue)&&(t_bonus[2]>=extends[k]->green)&&(t_bonus[3]>=extends[k]->white)&&(t_bonus[4]>=extends[k]->black)&&(t_score>=extends[k]->score)&& (!extends[k]->isreached) && (!m_players.at(num)->is_extend_get)){
                extends[k]->isreached=true;
                m_players.at(num)->is_extend_get=true;
                return true;
            }
        }
    }
    return false;
}

void GameMune::ExtendWin() {
    int index=-1;
    int max_score=-1;
    for(int i=0;i<m_players.size();i++){
        if(m_players.at(i)->is_extend_get==true && m_players.at(i)->getScore()>max_score){
            index=i;
            max_score=m_players.at(i)->getScore();
        }
    }
    cout<<"player "<<m_players.at(index)->getName()<<" Winner!"<<endl;
    for(int i=0;i<m_players.size();i++){
        cout<<m_players[i]->getName()<<"'s score: "<<m_players[i]->getScore()<<endl;
    }
}

void GameMune::showDeskInfoExtend(int num){
    int gems[6]={m_players.at(num)->getredStoneNum(),m_players.at(num)->getblueStoneNum(),m_players.at(num)->getgreenStoneNum(),m_players.at(num)->getwhiteStoneNum(),m_players.at(num)->getblackStoneNum(),m_players.at(num)->getgoldNum()};
    cout<< "--------------------DeskInfo------------------------------------------------------------\n";
    cout<< "--------------------referent------------------------------------------------------------\n";
    cout<<"redStoneNum:\t"<<redStone.num<<endl;
    cout<<"blueStoneNum:\t"<<blueStone.num<<endl;
    cout<<"greenStoneNum:\t"<<greenStone.num<<endl;
    cout<<"whiteStoneNum:\t"<<whiteStone.num<<endl;
    cout<<"blackStoneNum:\t"<<blackStone.num<<endl;
    cout<<"goldNum:\t"<<gold.num<<endl;
    cout<< "--------------------CardsInfo-----------------------------------------------------------\n";
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
    cout << "--------------------PlyerInfo-----------------------------------------------------------\n";
    for (int i=0;i<m_players.size();i++){
        cout<<"NAME:  "<<m_players[i]->getName()<<endl;
        cout<<"SCORE: "<<m_players[i]->getScore()<<endl;
    }
    cout << "--------------------City----------------------------------------------------------------\n";
    for(int i=0;i<extends.size();i++){
        cout<<"City"<<i+1<<": ";
        extends[i]->printInfo();
    }
    cout << "--------------------MyselfInfo----------------------------------------------------------\n";
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

int extendGame(GameInfo* gameMune){
    bool isend=false;
    int n=0;
    while(true){
        for(int i=0;i<gameMune->m_players.size();i++){
            gameMune->showDeskInfoExtend(i);
            gameMune->Menu(i);
            isend=gameMune->is_reach_extend(i);
            if(isend){
                n++;
            }
            system("cls");
            system("pause");
        }
        if(n!=0) {
            gameMune->ExtendWin();
            return 0;
        }
    }
}