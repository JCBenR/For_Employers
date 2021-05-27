import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.scene.control.Slider;
import javafx.scene.control.TextField;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.text.Text;

public class testClass extends Widget{
    double frequency;
    SineWave newSW = new SineWave(550);
    testClass(String title, AudioComponent ac) {
        super(title, ac);

        GridPane gp = new GridPane();
        gp.setHgap(5);
        Slider slide1 = new Slider(10,100,20);
        slide1.valueProperty().addListener(new ChangeListener<Number>() {
            @Override
            public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                frequency = (int)slide1.getValue();
            }
        });
        gp.add(buildSlider("slide1",slide1),0,0);
//        gp.add(buildSlider("slide2",1,10,3),0,1);
//        gp.add(buildSlider("slide3",10,50,15), 0,2);
        bp.setCenter(gp);
        getOj();
        getIj();

    }

    BorderPane getTestClass(){
        bp.setLayoutY(100);
        bp.setLayoutX(200);
        return this.bp;
    }

}
