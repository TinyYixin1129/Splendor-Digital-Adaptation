#include"splendor.h"

int main() {
    int choose;
    srand((unsigned)time(0));
    cout<<"Splendor"<<endl;
    cout<<"1.four players"<<endl;
    cout<<"2.three players"<<endl;
    cout<<"3.two players"<<endl;
    cout<<"choose : "<<endl;
    cin>>choose;
    switch(choose){
        case 1:
            StartGame(4);
            break;
        case 2:
            StartGame(3);
            break;
        case 3:
            StartGame(2);
            break;
        default:
            break;
    }
    return 0;
}
