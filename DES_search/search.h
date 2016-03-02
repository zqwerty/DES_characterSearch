//
//  search.h
//  DES_search
//
//  Created by 朱祺 on 16/3/1.
//  Copyright (c) 2016年 朱祺. All rights reserved.
//
// source:http://blog.csdn.net/kamaliang/article/details/6691653

#ifndef DES_search_search_h
#define DES_search_search_h

#define LL  long long

//计算S盒置换 S_order=[0,7],input=[0,63]
int Sbox(int S_order,int input);

//生成各Sbox输入的异或ΔX（6bit）与输出的异或ΔY（4bit）的对应关系.
void Create_XOR_Pair_Table();

struct bit32{
    int data[8];
    char ch[32];
    void ch2int();
    void int2ch();
    bool equal(bit32);
    bool next();
    void init();
    bit32 XOR(bit32);
    void E_expansion();//只改变data不改变ch
    bit32();
};



//给定轮函数输入的ΔX，输出的ΔY，计算对应的概率。返回的是成功的次数，还需除以2^32
LL int ProbabilityFromDeltaXToDeltaY(bit32 deltaX,bit32 deltaY);

//给定轮函数输入的ΔX，计算最大的概率。
LL int MaxProbabilityFromDeltaX(bit32 deltaX);

void MAINprocedure();

#endif
