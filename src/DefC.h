#ifndef DefC_H
#define DefC_H

#include <stdint.h>

int Max(int a,int b);   //声明一个函数对a b进行大小比较，输出最大值

typedef struct{
    int A;
    int R;
    int G;
    int B;
}ARGB;

ARGB newARGB(int A,int R,int G,int B);
ARGB getARGB(int Color);
int CreateARGB(ARGB color);
int intCreateARG(int A,int R,int G,int B);

#endif // DefC_H
