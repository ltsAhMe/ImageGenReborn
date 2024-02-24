#include <stdint.h>
#include "DefC.h"
int Max(int a,int b)   //创建一个函数对a b进行大小比较，输出最大值
{
    if (a < b)
        return b;
    else
        return a;
}


ARGB newARGB(int A,int R,int G,int B){
	ARGB done;
	done.A = A;
	done.R = R;
	done.G = G;
	done.B = B;
	return done;
}
ARGB getARGB(int Color){
	return newARGB((Color >>24)&0xFF,(Color >>16)&0xFF,(Color >>8)&0xFF,Color&0xFF);
}
int CreateARGB(ARGB color){
	return (color.A<<24) | (color.R<<16) | (color.G<<8) | color.B;
}
int intCreateARG(int A,int R,int G,int B){
	return (A<<24) | (R<<16) | (G<<8) | B;
}
