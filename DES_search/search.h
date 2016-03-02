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

//计算S盒置换 S_order=[0,7],input=[0,63]
int Sbox(int S_order,int input);

//生成输入的异或delta_X与输出的异或delta_Y的对应关系.
void Create_XOR_Pair_Table();

#endif
