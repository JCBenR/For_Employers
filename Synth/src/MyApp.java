import javafx.application.Application;
import javafx.geometry.Bounds;
import javafx.geometry.Insets;
import javafx.geometry.Point2D;
import javafx.scene.Cursor;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;
import javafx.stage.Stage;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.sound.sampled.LineUnavailableException;
import java.util.ArrayList;

public class MyApp extends Application {

        Widget out;
        Widget in;
        SpeakerWidget spkr;
    @Override
    public void start(Stage primaryStage) throws Exception {
        primaryStage.setTitle("Hello world");
        BorderPane parentPane = new BorderPane();
        parentPane.setPrefSize(900,700);
        ArrayList<Widget> widgetsArray = new ArrayList<>();

        //CREATE BUTTONS AND VBOX
        //create buttons for filters
            VBox buttonBox = new VBox();
            Button sineWave = new Button();
            sineWave.setText("Sine Wave");

            Button Volume = new Button();
            Volume.setText("Volume");

            Button mixer = new Button();
            mixer.setText("Mixer");

            Button linearRamp = new Button();
            linearRamp.setText("Linear Ramp");

            Button TestClass = new Button();
            TestClass.setText("testClass");

            //format buttons in Vbox
            buttonBox.setSpacing(10);
            buttonBox.getChildren().addAll(sineWave, Volume, mixer, linearRamp, TestClass);

            //create pane for widgets
            Pane widgetPane = new Pane();
            widgetPane.setPrefSize(800, 600);
            widgetPane.setPadding(new Insets(15));
            widgetPane.setStyle("-fx-background-color: #a5d6ba");
            widgetPane.setBorder(new Border(new BorderStroke(Color.DARKORANGE,
                    BorderStrokeStyle.SOLID, CornerRadii.EMPTY, BorderWidths.DEFAULT)));

            //create play button
            Button play = new Button();
            play.setText("Play");

//==========================================================================
        SineWaveFilterWidget dsw = new SineWaveFilterWidget("test", new SineWave(440));
//        AudioClip b = dsw.output;
        SineWave sw440 = new SineWave(440);
        SineWave sw330 = new SineWave(330);
        SineWave sw220 = new SineWave(220);

        //Volume v = new Volume(VolumeWidget);

        spkr = new SpeakerWidget("Speaker");//need to figure out what to pass here.
        widgetPane.getChildren().add(spkr.getSpeaker());
        widgetsArray.add(spkr);

//==========================================================================

        //button actions
        sineWave.setOnAction(eh->{
            SineWaveFilterWidget newSW = new SineWaveFilterWidget("SineWave Filter", new SineWave(440));
            widgetPane.getChildren().add(newSW.getSineWave());
            widgetsArray.add(newSW);
        });

        mixer.setOnAction(eh->{
            MixerWidget newMixer = new MixerWidget("Mixer", new Mixer());
            widgetPane.getChildren().add(newMixer.getMixer());
            widgetsArray.add(newMixer);
        });

        Volume.setOnAction(eh-> {
            VolumeWidget volW = new VolumeWidget("Volume", new Volume(1));
            widgetPane.getChildren().add(volW.getVol());
            widgetsArray.add(volW);
        });

        TestClass.setOnAction(eh-> {
            testClass test = new testClass("some test", new SineWave(440));
            widgetPane.getChildren().add(test.getTestClass());
            widgetsArray.add(test);
        });

        linearRamp.setOnAction(eh-> {
            LinearRampWidget lrW = new LinearRampWidget("Linear Ramp Filter", new LinearRamp(10,8000));
            widgetPane.getChildren().add(lrW.getLinerRamp());
            widgetsArray.add(lrW);
        });


        //widget base needs getters for both in and out jacks. when you press you want to see if you're inside one of the out jacks, when you release you want to see if you're in one of the in jacks.
        widgetPane.setOnMousePressed(eh->{
            Point2D click = new Point2D(eh.getSceneX(),eh.getSceneY());
            for(int i =0; i<widgetsArray.size(); i++){
                if(eh.getTarget() == widgetsArray.get(i).returnOj()){
                    out = widgetsArray.get(i);
                    System.out.println("out is: " + out);
                }
            }
        });

        widgetPane.setOnMouseReleased(eh->{
            Point2D clickPosition = new Point2D(eh.getSceneX(),eh.getSceneY());
            for(int i =0; i<widgetsArray.size(); i++){
                Circle current = widgetsArray.get(i).returnIj();
                Bounds ijLocation = current.localToScene(current.getBoundsInLocal());
                if(ijLocation.contains(clickPosition)){
                    in = widgetsArray.get(i);
                    System.out.println("in is: " + in);
                    System.out.println(out.getAudioComponent().getClip());
                    in.getAudioComponent().connectInput(out.getAudioComponent());
                    Cable c = new Cable(out, in);
                    Line line = new Line();
                    line.setStroke(Color.RED);
                    line.setStartX(out.oj.getCenterX());
                    line.setStartY(out.oj.getCenterY());
                    line.setEndX(in.ij.getCenterX());
                    line.setEndY(in.ij.getCenterY());
                }
            }
        });

//===========================PLAY BUTTON===============================================

        play.setOnAction(ae-> {
            System.out.println("about to play");
            try{
                //Volume vol = new Volume(VolumeWidget.getVolume());
                Clip c = AudioSystem.getClip();
                AudioFormat format16 = new AudioFormat(44100, 16, 1, true, false);
//                AudioComponent gen = new SineWave(SineWaveFilterWidget.getFrequency());
//                vol.connectInput(gen);
                //to get speaker working, activate the line below, remove 118-119 and 115 and then replace vol.getClip() with s.sound();
                //SpeakerWidget s = spkr;
                AudioClip clip = spkr.getAudioComponent().getClip();//vol.getClip();
                c.open(format16, clip.getData(), 0, clip.getData().length); //reads data from my byte array to play it
                c.start();
                while(c.getFramePosition() < 88200 || c.isActive() || c.isRunning()){}
                System.out.println("done");
                c.close();

            } catch (LineUnavailableException e){
                e.printStackTrace();
            }
        });

        parentPane.setRight(buttonBox);
        parentPane.setLeft(widgetPane);
        parentPane.setBottom(play);
        primaryStage.setScene(new Scene(parentPane, 900, 700));
        primaryStage.show();
    }
}
