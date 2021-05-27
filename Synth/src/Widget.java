import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.geometry.Insets;
import javafx.scene.Cursor;
import javafx.scene.Node;
import javafx.scene.control.Slider;
import javafx.scene.control.TextField;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.text.Font;
import javafx.scene.text.Text;
import javafx.util.StringConverter;

public abstract class Widget {
    protected String title;
    public static BorderPane bp;
    public Circle oj, ij;
    public Widget out, in;
    double x,y;
    Boolean dragged = false;
    AudioComponent ac;
    //default constructor

    Widget(String title, AudioComponent inAC){

        ac = inAC;
        //create boarder pane
        bp = new BorderPane();
        bp.setPadding(new Insets(15));
        bp.setBorder(new Border(new BorderStroke(Color.BLUE,
                BorderStrokeStyle.SOLID, CornerRadii.EMPTY, BorderWidths.DEFAULT)));

        //create title
        Text wTitle = new Text(title);
        wTitle.setFont(Font.font(20));
        wTitle.setFill(Color.BLUE);
        bp.setTop(wTitle);
        StackPane spl = new StackPane();
        spl.setPrefWidth(35);
        bp.setLeft(spl);



        bp.setOnMousePressed(eh->{
            System.out.println(eh.getTarget());
            //if statement to exclude in/out jacks
            if(eh.getTarget() == oj || eh.getTarget() == ij){
                x = eh.getX();
                y = eh.getY();
                out = this;
            } else if(eh.getTarget() != oj || eh.getTarget() != ij) {
                dragged = true;
                x = eh.getX();
                y = eh.getY();
                bp.setCursor(Cursor.MOVE);
            }
        });

        bp.setOnMouseDragged(eh->{
            if(dragged) {
                bp.setLayoutX(eh.getSceneX() - x);
                bp.setLayoutY(eh.getSceneY() - y);
            }
        });

        bp.setOnMouseReleased(eh->{
            if(eh.getTarget() == ij ){
                System.out.println(this);
            }
            bp.setCursor(Cursor.HAND);
            dragged = false;
        });
    }

    public Widget getWidget(){
        return this;
    }
    //JACK METHODS
    //output jack
    public void getOj(){
        oj = new Circle(10);
        oj.setFill(Color.RED);
        oj.setId(title + " output");
        bp.setRight(oj);
    }

    public Circle returnOj(){
        return oj;
    }

    //input jack
    public void getIj() {
        ij = new Circle(10);
        ij.setFill(Color.GREEN);
        ij.setId(title + "input");
        bp.setLeft(ij);
        ij.onMouseClickedProperty();
    }

    public Circle returnIj(){
        return ij;
    }

    //GRID PANE CONSTRUCTOR
    //not using currently
    public void createGP(){
        GridPane cgp = new GridPane();
        cgp.setHgap(5);
    }

    public HBox buildSlider(String title, Slider slider)
    {
        HBox hb = new HBox(8);
        Text eTitle = new Text(title);
        TextField tf = new TextField();
        tf.setPrefWidth(50);
        hb.getChildren().addAll(eTitle, tf, slider);
        tf.textProperty().bindBidirectional(slider.valueProperty(), new StringConverter<Number>()
        {

            @Override
            public String toString(Number t)
            {
                return t.toString();
            }

            @Override
            public Number fromString(String string)
            {
                return Double.parseDouble(string);
            }

        });
        return hb;
    }


    public AudioComponent getAudioComponent(){
        return ac;
    }
}
