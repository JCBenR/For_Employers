import javafx.scene.shape.Line;

public class Cable {
    AudioComponent source;
    AudioComponent destination;
    //AudioClip payload = source.getClip();

    Line line;

    Widget w;
    /*
    mouseEvent Pressed
    --can only start at jack outputs
      --start line at that point
    --get name of jack
    --draw line as long as mouse is pressed
    --complete line when mouse is released on input jack
    //mouseEvent released--end line at that point
    --when released get name of input jack
    --can only be released on input jacks
    --transfer clip form output jack to input jack
    (another option might be to just press on the ouput jack of one node and press on the input jack of another and then draw the line between them)
     */

    Cable(Widget out, Widget in){
        line = new Line();
        //call connectInput();
        AudioComponent o = out.getAudioComponent();
        AudioComponent i = in.getAudioComponent();
        i.connectInput(o);
        System.out.println("CABLE: " + i + " is now connected to " + o);
        //use localToScene on the circle center to get the x&y of the window.
        line.setStartX(out.oj.getCenterX());
        line.setStartY(out.oj.getCenterY());
        line.setEndX(in.ij.getCenterX());
        line.setEndY(in.ij.getCenterY());
    }

}
