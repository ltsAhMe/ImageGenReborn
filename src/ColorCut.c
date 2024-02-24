#include "Image_Native_ImageGen.h"
#include "DefC.h"
#include "stdlib.h"
int getBlurNum(int color){
	ARGB temp = getARGB(color);
	return (temp.R+temp.G+temp.B)/3;
}

JNIEXPORT jintArray JNICALL Java_Image_Native_ImageGen_ColorCut
  (JNIEnv *env, jclass class, jintArray JavaRGBAarray, jint start, jint end){
	  jsize Length = (*env)->GetArrayLength(env,JavaRGBAarray);
	  jint *array = (jint*)malloc(Length * sizeof(jint));
		(*env)->GetIntArrayRegion(env,JavaRGBAarray,0,Length,array);
	for (int i=0;i<Length-2;i++) {
		int temp = getBlurNum(array[i]);
		if (temp>=start && temp<=end) {
			
		}else{
			array[i]=0;
		}
		
	}
	jintArray DoneArray = (*env)->NewIntArray(env,Length);
	(*env)->SetIntArrayRegion(env,DoneArray,0,Length,array);
	return DoneArray;
  }
