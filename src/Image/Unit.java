package Image;

import Image.Native.ImageGen;

import java.awt.*;
import java.awt.image.BufferedImage;

public class Unit {
    public static int[] getRGBA(BufferedImage image){
        int w = image.getWidth();
        int h = image.getHeight();
        int[] RGBA = new int[(w*h)+2];
        image.getRGB(0,0,w,h,RGBA,0,w);
        RGBA[RGBA.length-1]=h;
        RGBA[RGBA.length-2]=w;
        return RGBA;
    }
    public static BufferedImage imageReSize(BufferedImage image,float Scale){
        if (Scale==1){
            return image;
        }
        return toBufferedImage(image.getScaledInstance((int) ((float)image.getWidth()*Scale), (int) ((float)image.getHeight()*Scale),Image.SCALE_FAST));
    }
    public static BufferedImage CreateImage(int[] RGBA){
        int h = RGBA[RGBA.length-1];
        int w = RGBA[RGBA.length-2];
        BufferedImage image = new BufferedImage(w,h,BufferedImage.TYPE_INT_ARGB);
        image.setRGB(0,0,w,h,RGBA,0,w);
        return image;
    }
    public static BufferedImage bloom(BufferedImage image,int start,int Thread,int blurSize){
        BufferedImage Doneimage = new BufferedImage(image.getWidth(),image.getHeight(),image.getType());
        Graphics2D g2d = (Graphics2D) Doneimage.getGraphics();
        g2d.drawImage(image,0,0,null);
        g2d.drawImage(Unit.CreateImage(ImageGen.blur(ImageGen.ColorCut(Unit.getRGBA(image),start,255),blurSize,Thread)),0,0,null);
        g2d.dispose();
        return Doneimage;
    }
    public static BufferedImage toBufferedImage(Image image) {
        if (image instanceof BufferedImage) {
            return (BufferedImage) image;
        }
        int width = image.getWidth(null);
        int height = image.getHeight(null);
        BufferedImage bimage = new BufferedImage(width,height,BufferedImage.TYPE_INT_ARGB);
        Graphics g = bimage.createGraphics();
        g.drawImage(image, 0, 0, null);
        g.dispose();
        return bimage;
    }
}
