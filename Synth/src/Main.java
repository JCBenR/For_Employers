import com.sun.glass.ui.Application;
import javafx.event.ActionEvent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.sound.sampled.LineUnavailableException;
import java.util.Random;
import static javafx.application.Application.launch;

public class Main {

    public static void playSound(AudioClip clip, int frequency, int start, int stop, double scale) throws LineUnavailableException {

        Clip c = AudioSystem.getClip();

        AudioFormat format16 = new AudioFormat(44100, 16, 1, true, false);
        SineWave newClip = new SineWave(frequency);
        Volume vol = new Volume(scale);
        vol.connectInput(newClip);
        clip = vol.getClip();
        c.open(format16, clip.getData(), 0, clip.getData().length); //reads data from my byte array to play it

        System.out.println("about to play");
        c.start(); //plays it
        c.loop(0); //plays it 2 more times if desired, so 3 seconds total

        while(c.getFramePosition() < 88200 || c.isActive() || c.isRunning()){}

        System.out.println("done");
        c.close();

    }



    public static void main(String [] args) throws ArrayIndexOutOfBoundsException, LineUnavailableException {


        SineWave testSW = new SineWave(440);
        AudioClip testAC = new AudioClip();

        Clip c = AudioSystem.getClip();

        AudioFormat format16 = new AudioFormat(44100, 16, 1, true, false);

        AudioComponent gen = new SineWave(220);
        AudioComponent gen2 = new SineWave(440);
        AudioComponent test1 = new SineWave(262);
        AudioComponent test2 = new SineWave(327);
        AudioComponent test3 = new SineWave(393);
        AudioComponent vol = new Volume( .1);
        AudioComponent square = new SquareWave(220, 500);
//        vol.connectInput(test1);
        Mixer mix = new Mixer();
        mix.connectInput(test1);
        mix.connectInput(test2);
        mix.connectInput(test3);
        vol.connectInput(mix);
        AudioComponent test4 = new VFSineWave();
        AudioComponent lr = new LinearRamp(50,10000);
        test4.connectInput(lr);
//        square.connectInput(test2);
//        vol.connectInput(gen);
//        mix.connectInput(gen);
//        mix.connectInput(gen2);



        AudioClip clip = test4.getClip();

        c.open(format16, clip.getData(), 0, clip.getData().length); //reads data from my byte array to play it

        System.out.println("about to play");
        c.start(); //plays it
        c.loop(0); //plays it 2 more times if desired, so 3 seconds total

//makes sure the program doesn't quit before the sound plays
        while(c.getFramePosition() < 88200 || c.isActive() || c.isRunning()){}

        System.out.println("done");
        c.close();

    }
}
