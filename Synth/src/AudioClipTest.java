import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class AudioClipTest {

    AudioClip test2 = new AudioClip();

    @Test
    void getSample() {
        test2.setSample(0, 5);
        test2.getSample(0);
        System.out.println(test2.getSample(0));
        System.out.println(test2.getSample(1));
        assertEquals(5, test2.getSample(0));
        test2.setSample(2, 10);
        test2.getSample(2);
        test2.setSample(4, 257);
        assertEquals(257, test2.getSample(4));
        System.out.println(test2.getSample(0));
        System.out.println(test2.getSample(1));
        System.out.println(test2.getSample(4));
        test2.setSample(5, -257);
        System.out.println(test2.getSample(5));
        assertEquals(-257, test2.getSample(5));
    }

//    val = Short MIN_VALUE
//    for(int i =0; i<2*Short.Max;  i++){
//        realShortArray[i] = val;
//        audioClip.set(i, val);
//        assertEquals(realShortArray[i], audioClip.get(i));
//          val++;
//    }

    @Test
    void setSample() {

    }

    @Test
    void getData() {
    }
}