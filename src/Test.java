import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.sql.Time;

import Image.Native.ImageGen;
import Image.Unit;
import TimerTest.newTimerTest;

public class Test {
    public static void main(String[] args) {
        BufferedImage image;
        try {
            image = ImageIO.read(new File("C:\\Users\\ltsAhMe\\Desktop\\allCr\\imageTest\\v2-86848ef3286546c5e800b57756079473_b.jpg"));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        BufferedImage done = Unit.bloom(image,50,80,1);
        try {
            ImageIO.write(done,"png",new File("done.png"));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

    }
}
