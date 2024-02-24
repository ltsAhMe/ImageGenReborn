
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include "DefC.h"
#include "Image_Native_ImageGen.h"
#include <stdio.h>
int intMax(int a,int b){
	if (a>b) {
		return a;
	}
	return b;
}

int getColorBlur(int* Color, int* state,int size) {
        int sumA = 0;
        int sumR = 0;
        int sumG = 0;
        int sumB = 0;
        int totalWeight = 0;

        for (int i = 0; i < size; i++) {
            ARGB pixel = getARGB(*(Color+i));
            int weight = *(state+i);

                int weightedAlpha = pixel.A * weight;
                int weightedRed = pixel.R * weight;
                int weightedGreen = pixel.G * weight;
                int weightedBlue = pixel.B * weight;

                sumA += weightedAlpha;
                sumR += weightedRed;
                sumG += weightedGreen;
                sumB += weightedBlue;
                totalWeight += weight;
        }

        int averageA =  (sumA / totalWeight);
        int averageR =  (sumR / totalWeight);
        int averageG = (sumG / totalWeight);
        int averageB = (sumB / totalWeight);

        return intCreateARG(averageA,averageR,averageG,averageB);
    }
typedef struct{
	int w;
	int h;
	int blurSize;
	int Start;
	int End;
	int Size;
	jint *DoneRGBA;
	jint *orgRGBA;
}BlurThreadDate;

void* blurThreadFunc(void *input){
	BlurThreadDate date  = *((BlurThreadDate*)input);
	free((BlurThreadDate*)input);
	int CheckDoneWhere=0;
	int inArrayWhereXY;
    int *CheckDoneColor = (int*) malloc((2*date.blurSize+1)*(2*date.blurSize+1) * sizeof(int));
    int *CheckDoneState = (int*) malloc((2*date.blurSize+1)*(2*date.blurSize+2) * sizeof(int));
    for (int x=date.Start; x<date.End; x++) {
        for (int y=0; y<date.h; y++) {
            // Init
        CheckDoneWhere=0;
            inArrayWhereXY = (y*date.w)+x;

            for (int offsetX=-date.blurSize; offsetX<=date.blurSize; offsetX++) {
                for (int offsetY=-date.blurSize; offsetY<=date.blurSize; offsetY++) {
                    int offedX = x+offsetX;
                    int offedY = y+offsetY;

                    if (offedX>=0 && offedY>=0 && offedX<date.w && offedY<date.h) {
                        *(CheckDoneColor+CheckDoneWhere) = *(date.orgRGBA+(offedY*date.w+offedX));
                        *(CheckDoneState+CheckDoneWhere) = intMax(abs(offsetX),abs(offsetY));
                        CheckDoneWhere++;
                    }
                }
            }
            // Cache Done Color
//	    printf("im Thread%i im draw %i,%i. use Color:%i\n",date.Start,x,y, getColorBlur(CheckDoneColor, CheckDoneState, CheckDoneWhere));
//	if ( getColorBlur(CheckDoneColor, CheckDoneState, CheckDoneWhere)==0) {
//		printf("Thread%i:draw null,my color:",date.Start);
//		for (int i=0;i<=CheckDoneWhere;i++) {
//			ARGB the = getARGB(CheckDoneColor[i]);
//			printf("RGBA(%i,%i,%i,%i)  ",the.A,the.R,the.G,the.B);
//		}
//		printf("\n");
//	 } 
            *(date.DoneRGBA+inArrayWhereXY) = getColorBlur(CheckDoneColor, CheckDoneState, CheckDoneWhere);
        }
    }

    pthread_exit(NULL);
}

JNIEXPORT jintArray JNICALL Java_Image_Native_ImageGen_blur
  (JNIEnv *env, jclass class, jintArray ArrayRGBA, jint blurSize, jint ThreadNum){
jint *RGBAarray = (*env)->GetIntArrayElements(env, ArrayRGBA, 0);
    int RGBAsize = (*env)->GetArrayLength(env, ArrayRGBA);
    int w = *(RGBAarray+(RGBAsize-2));
    int h = *(RGBAarray+(RGBAsize-1));
    // 分配内存空间
    jint *doneRGBA = (jint*) malloc(RGBAsize * sizeof(jint));
    *(doneRGBA+(RGBAsize-2)) = w;

    *(doneRGBA+(RGBAsize-1)) = h;
	
	
	pthread_t Threads[ThreadNum];
	int ThreadNeedDo = w/ThreadNum;
//	printf("ThreadNeedDo:%i\n",ThreadNeedDo);
	for (int ThreadReading=0;ThreadReading<ThreadNum-2;ThreadReading++) {
		BlurThreadDate *Date = (BlurThreadDate*)malloc(sizeof(BlurThreadDate));
		
			Date->w = w;
			Date->h = h;
			Date->DoneRGBA =doneRGBA;
			Date->orgRGBA = RGBAarray;
			Date->Size = RGBAsize;
			Date->blurSize = blurSize;
			Date->Start = ThreadNeedDo*ThreadReading;
			if (ThreadNeedDo*(ThreadReading+1)>=w) {
				Date->End = w;
			}else{
				Date->End = ThreadNeedDo*(ThreadReading+1);
			}
			printf("Start:%i End:%i",Date->Start,Date->End);
			pthread_create(&Threads[ThreadReading],NULL,blurThreadFunc,(void*)Date);
	}

	
	for (int ThreadWating=0;ThreadWating<ThreadNum;ThreadWating++) {
		pthread_join(Threads[ThreadWating],NULL);
	}

    // 释放内存空间
    (*env)->ReleaseIntArrayElements(env, ArrayRGBA, RGBAarray, 0);

    jintArray DoneRGBA = (*env)->NewIntArray(env, RGBAsize);
    (*env)->SetIntArrayRegion(env, DoneRGBA, 0, RGBAsize, doneRGBA);

    // 释放内存空间
    free(doneRGBA);

    return DoneRGBA;  
  }
