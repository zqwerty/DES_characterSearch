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

int XOR_Pair_Table[8][64][16]={0};

int main(int argc, const char * argv[]) {
    freopen("/Users/zhuqi/Documents/Cryptanalysis/DES_search/DES_search/data.txt", "w", stdout);
    Create_XOR_Pair_Table();
    for(int i=0;i<64;i++){
        for(int j=0;j<16;j++){
            cout<<XOR_Pair_Table[1][i][j]<<" ";
        }
        cout<<endl;
    }
    return 0;
}
