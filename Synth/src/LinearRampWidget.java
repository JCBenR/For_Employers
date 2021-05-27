import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.geometry.Insets;
import javafx.scene.control.Slider;
import javafx.scene.control.TextField;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.text.Text;
import javafx.util.StringConverter;

public class LinearRampWidget extends Widget{

    LinearRampWidget(String title, LinearRamp lr){
        super(title, lr);
        //get sound
        AudioComponent vfsw = new VFSineWave();
        vfsw.connectInput(lr);
        getIj();
        getOj();

                //slider
        Slider sliderStart = new Slider(1, 10000,lr.getStart());
        Slider sliderStop = new Slider(1, 10000, lr.getStop());

        GridPane cgp = new GridPane();
        cgp.setHgap(5);

        cgp.add(buildSlider("Start", sliderStart),0,0);
        cgp.add(buildSlider("Stop",sliderStop),0,1);

        sliderStart.valueProperty().addListener(new ChangeListener<Number>() {
            @Override
            public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                lr.setStart((float)sliderStart.getValue());
            }
        });

        sliderStop.valueProperty().addListener(new ChangeListener<Number>() {
            @Override
            public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                lr.setStop((float)sliderStop.getValue());
            }
        });

        bp.setCenter(cgp);
    }

    BorderPane getLinerRamp(){
        bp.setLayoutX(100);
        bp.setLayoutY(100);
        return this.bp;
    }
}
