//
//  main.cpp
//  DES_search
//
//  Created by 朱祺 on 16/2/21.
//  Copyright (c) 2016年 朱祺. All rights reserved.
//
#include "search.h"
#include <iostream>
#include <string.h>
using namespace std;

#define rep(i,m,n)  for(int i=m;i<n;i++)
#define LL  long long


int XOR_Pair_Table[8][64][16]={0};

int main(int argc, const char * argv[]) {
    freopen("/Users/zhuqi/Documents/Cryptanalysis/DES_search/DES_search/data.txt", "w", stdout);
    Create_XOR_Pair_Table();
    bit32 t;
    int test[8] = {6,0,0,0,0,0,0,0};
    rep(i,0,8){
        t.data[i] = test[i];
    }
    t.int2ch();
    
    bit32 t2;
    int test2[8] = {0,0,8,0,8,2,0,0};
    rep(i,0,8){
        t2.data[i] = test2[i];
    }
    t2.int2ch();
    
    LL int ret = ProbabilityFromDeltaXToDeltaY(t, t2);
    cout<<ret<<endl;
    t.init();
    ret = MaxProbabilityFromDeltaX(t);
    cout<<ret<<endl;
    
    rep(i,0,32){
        cout<<t.ch[i];
    }
    cout<<endl;
    t.ch2int();
    rep(i,0,8){
        cout<<t.data[i]<<" ";
    }

    return 0;
}
