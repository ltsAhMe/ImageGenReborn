#include <math.h>
#include <stdlib.h>
#include "DefC.h"
#include "Image_Native_ImageGen.h"
#include <pthread.h>
int getColorBlur(int* Color, int* state,int size) {
        int sumA = 0;
        int sumR = 0;
        int sumG = 0;
        int sumB = 0;
        int totalWeight = 0;

        for (int i = 0; i <= size; i++) {
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
typedef struct {
    int* RGBAarray;
    int* doneRGBA;
    int w;
    int h;
    int blurSize;
} ThreadData;

void* processRow(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int *CheckDoneColor = (int*) malloc((2*data.blurSize+1)*(2*data.blurSize+1) * sizeof(int));
    int *CheckDoneState = (int*) malloc((2*data.blurSize+1)*(2*data.blurSize+2) * sizeof(int));
    for (int x = 0; x < data->w; x++) {
        for (int y = 0; y < data->h; y++) {
            int CheckDoneWhere = 0;
            int inArrayWhereXY = (y * data->w) + x;

            for (int offsetX = -data->blurSize; offsetX <= data->blurSize; offsetX++) {
                for (int offsetY = -data->blurSize; offsetY <= data->blurSize; offsetY++) {
                    int offedX = x + offsetX;
                    int offedY = y + offsetY;

                    if (offedX >= 0 && offedY >= 0 && offedX < data->w && offedY < data->h) {
                       *(CheckDoneColor+CheckDoneWhere) = *(date.orgRGBA+(offedY*date.w+offedX));
                        *(CheckDoneState+CheckDoneWhere) = intMax(offedX, offedY);
                        CheckDoneWhere++;
                    }
                }
            }

            data->doneRGBA[inArrayWhereXY] = getColorBlur(CheckDoneColor, CheckDoneState, CheckDoneWhere);
        }
    }

    pthread_exit(NULL);
}

JNIEXPORT jintArray JNICALL Java_Image_Native_ImageGen_blur
(JNIEnv *env, jclass class, jintArray ArrayRGBA, jint blurSize, jint ThreadNum) {
    jint *RGBAarray = (*env)->GetIntArrayElements(env, ArrayRGBA, 0);
    int RGBAsize = (*env)->GetArrayLength(env, ArrayRGBA);
    int w = *(RGBAarray + (RGBAsize - 2));
    int h = *(RGBAarray + (RGBAsize - 1));

    jint *doneRGBA = (jint*)malloc(RGBAsize * sizeof(jint));

    // 创建线程数组
    pthread_t threads[ThreadNum];
    ThreadData threadData[ThreadNum];

    // 每个线程处理的行数
    int rowsPerThread = h / ThreadNum;

    for (int i = 0; i < ThreadNum; i++) {
        threadData[i].RGBAarray = RGBAarray;
        threadData[i].doneRGBA = doneRGBA;
        threadData[i].w = w;
        threadData[i].h = rowsPerThread;
        threadData[i].blurSize = blurSize;

        pthread_create(&threads[i], NULL, processRow, (void*)&threadData[i]);

        // 更新偏移量以处理下一段行
        RGBAarray += rowsPerThread * w;
        doneRGBA += rowsPerThread * w;
    }

    // 等待所有线程完成
    for (int i = 0; i < ThreadNum; i++) {
        pthread_join(threads[i], NULL);
    }

    // 释放内存空间
    (*env)->ReleaseIntArrayElements(env, ArrayRGBA, RGBAarray, 0);

    // 创建并返回结果数组
    jintArray DoneRGBA = (*env)->NewIntArray(env, RGBAsize);
    (*env)->SetIntArrayRegion(env, DoneRGBA, 0, RGBAsize, doneRGBA);

    // 释放内存空间
    free(doneRGBA);

    return DoneRGBA;
}

