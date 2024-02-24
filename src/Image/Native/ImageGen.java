package Image.Native;

public class ImageGen {
    static{
        System.load("C:\\Users\\ltsAhMe\\Documents\\Src\\ImageReborn\\src\\Native.dll");
    }
    native public static int[] blur(int[] RGBA,int size,int ThreadNum);
    native public static int[] ColorCut(int[] RGBA,int start,int end);



}
