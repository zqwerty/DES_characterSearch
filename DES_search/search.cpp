#include "search.h"

using namespace std;

/*S盒*/
int S[8][4][16] =
/*S1*/
{{{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
    {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
    {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
    {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
/*S2*/
{{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
    {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
    {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
    {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
/*S3*/
{{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
    {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
    {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
    {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
/*S4*/
{{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
    {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
    {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
    {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
/*S5*/
{{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
    {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
    {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
    {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
/*S6*/
{{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
    {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
    {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
    {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
/*S7*/
{{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
    {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
    {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
    {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
/*S8*/
{{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
    {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
    {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
    {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}};

/*扩充置换表E*/
int E_Table[48] = {
    31, 0, 1, 2, 3, 4,
    3,  4, 5, 6, 7, 8,
    7,  8,9,10,11,12,
    11,12,13,14,15,16,
    15,16,17,18,19,20,
    19,20,21,22,23,24,
    23,24,25,26,27,28,
    27,28,29,30,31, 0};

/*置换函数P*/
int P_Table[32] = {
    15,6,19,20,28,11,27,16,
    0,14,22,25,4,17,30,9,
    1,7,23,13,31,26,2,8,
    18,12,29,5,21,10,3,24};

int XOR_Pair_Table[8][64][16]={0};

//计算S盒置换
int Sbox(int S_order,int input){
    int output;
    char in[6];
    for(int i=5;i>=0;i--){
        in[5-i] = ((input>>i)&1) + '0';
    }
    int row = ((in[0]-'0')<<1) + (in[5]-'0');
    int col =   ((in[1]-'0')<<3) +
            ((in[2]-'0')<<2) +
            ((in[3]-'0')<<1) +
            ((in[4]-'0'));
    output = S[S_order][row][col];
    return output;
}

//生成各Sbox输入的异或delta_X与输出的异或delta_Y的对应关系. bit6->bit4
void Create_XOR_Pair_Table(){
    for(int S_order=0;S_order<8;S_order++){
        for(int delta_x=0;delta_x<64;delta_x++){
            for(int x=0;x<64;x++){
                int delta_y = Sbox(S_order, x^delta_x) ^ Sbox(S_order, x);
                XOR_Pair_Table[S_order][delta_x][delta_y]++;
            }
        }
    }
}

bit32::bit32(){
    init();
}
void bit32::ch2int(){
    int ret=0;
    for(int i=0;i<8;i++){
        for(int j=0;j<4;j++){
            ret += (ch[i*4+j]-'0')<<(3-j);
        }
        data[i] = ret;
        ret = 0;
    }
}
void bit32::int2ch(){
    for(int i=0;i<8;i++){
        for(int j=0;j<4;j++){
            ch[i*4+j] = ( (data[i]>>(3-j)) & 1 ) + '0';
        }
    }
}
bool bit32::equal(bit32 obj){
    for(int i=0;i<8;i++){
        if(data[i]!=obj.data[i]){
            return false;
        }
    }
    return true;
}
bool bit32::next(){
    int i=7;
    while(i>=0){
        data[i]++;
        if(data[i]!=16){
            break;
        }
        else{
            data[i]=0;
            i--;
        }
    }
    if(i>=0){
        int2ch();
        return true;
    }
    return false;
}
void bit32::init(){
    for(int i=0;i<8;i++){
        data[i] = 0;
    }
    int2ch();
}
bit32 bit32::XOR(bit32 obj){
    bit32 ret;
    ret.init();
    for(int i=0;i<8;i++){
        ret.data[i] = data[i]^obj.data[i];
    }
    ret.int2ch();
    return ret;
}
void bit32::E_expansion(){
    char ret[48];
    for(int i=0;i<48;i++) {
        ret[i] = ch[E_Table[i]];
    }
    int tmp=0;
    for(int i=0;i<8;i++) {
        for(int j=0;j<6;j++) {
            tmp += (ret[i*6+j]-'0')<<(5-j);
        }
        data[i] = tmp;
        tmp = 0;
    }
}
void bit32::output(){
    printf("%s\n",ch);
}

//给定轮函数输入的ΔX，输出的ΔY，计算对应的概率。
double ProbabilityFromDeltaXToDeltaY(bit32 deltaX,bit32 deltaY){
    //将ΔY逆置换P，得到ΔY_S
    bit32 deltaY_S;
    for(int i=0;i<32;i++) {
        deltaY_S.ch[P_Table[i]] = deltaY.ch[i];
    }
    deltaY_S.ch2int();
    //将ΔX进行E扩展，得到各Sbox输入的异或，以int[8]形式储存
    deltaX.E_expansion();
    //查找XOR_Pair_Table，将各S盒对应次数相乘
    LL int cnt=1;
    for(int i=0;i<8;i++) {
        cnt *= XOR_Pair_Table[i][deltaX.data[i]][deltaY_S.data[i]];
    }
    double ret = (double(cnt)/pow(2, 48));//(64^8)
    return ret;
}

//给定轮函数输入的ΔX，计算最大的概率。
double MaxProbabilityFromDeltaX(bit32 deltaX){
    //将ΔX进行E扩展，得到各Sbox输入的异或，以int[8]形式储存
    deltaX.E_expansion();
    LL int cnt=1;
    for(int i=0;i<8;i++) {
        int max=0;
        for(int j=0;j<16;j++) {
            if(XOR_Pair_Table[i][deltaX.data[i]][j]>max){
                max = XOR_Pair_Table[i][deltaX.data[i]][j];
            }
        }
        cnt *= max;
    }
    double ret = (double(cnt)/pow(2, 48));
    return ret;
}
//17=16+1
double bn[17]={0.0};
double p[17]={0.0};
bit32 deltaX[17];
bit32 deltaY[17];
//B0、B1、B2=？



//控制程序  roundcnt>=4 [1,16]
void MAINprocedure(int totalRound){
    Create_XOR_Pair_Table();
    //计算B1
//    bit32 deltaX;
//    while (deltaX.next()) {
//        double p1 = MaxProbabilityFromDeltaX(deltaX);
//        if(p1>Bn[0]){
//            Bn[0]=p1;
//        }
//    }
//    for(int i=0;i<16;i++) {
//        cout<<Bn[i]<<" ";
//    }
    /*******************/
    bn[1] = 0.25;
    bn[2] = 0.25;
    bn[3] = 0.0625;
//    bn[4] = 1.31/pow(2, 10);
    bn[4] = 1;
    /*******************/
    //从四轮特征开始
    for(int round=4;round<=totalRound;round++){
        firstRound(round);
        cout<<bn[round]<<endl;
    }
}
//n round
void firstRound(int n){
    while (deltaX[1].next()) {
        p[1] = MaxProbabilityFromDeltaX(deltaX[1]);
        if (p[1] * bn[n-1] >= bn[n]) {
            secondRound(n);
        }
        deltaX[1].output();
    }
}

void secondRound(int n){
    while (deltaX[2].next()) {
        while (deltaY[2].next()) {
            p[2] = ProbabilityFromDeltaXToDeltaY(deltaX[2], deltaY[2]);
            if(p[1] * p[2] * bn[n-2] >= bn[n]){
                Round(3, n);
            }
        }
    }
}

void Round(int i,int n){
    deltaX[i] = deltaX[i-2].XOR(deltaY[i-1]);
    while (deltaY[i].next()) {
        p[i] = ProbabilityFromDeltaXToDeltaY(deltaX[i], deltaY[i]);
        double product = 1;
        for(int cnt = 1;cnt<=i;cnt++){
            product *= p[cnt];
        }
        if (product * bn[n-i] >= bn[n]) {
            if(i==n-1){
                lastRound(n);
            }
            else{
                Round(i+1, n);
            }
        }
    }
}

void lastRound(int n){
    deltaX[n] = deltaX[n-2].XOR(deltaY[n-1]);
    p[n] = MaxProbabilityFromDeltaX(deltaX[n]);
    double product = 1;
    for(int cnt = 1;cnt<=n;cnt++) {
        product *= p[cnt];
    }
    if (product >= bn[n]) {
        bn[n] = product;
    }
}






















