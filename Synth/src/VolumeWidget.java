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

public class VolumeWidget extends Widget{

    VolumeWidget(String title, Volume v){
        super(title, v);
        getIj();
        getOj();


        GridPane cgp = new GridPane();
        cgp.setHgap(5);

        Slider slider = new Slider(0,1.5,v.getVolume());
        slider.valueProperty().addListener(new ChangeListener<Number>() {
            @Override
            public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                v.setVolume((slider.getValue()));
//                System.out.println(volume);
            }
        });
        System.out.println(v.getVolume());

        cgp.add(buildSlider("Volume", slider),0,0);
        bp.setCenter(cgp);
    }

    BorderPane getVol(){
        bp.setLayoutX(100);
        bp.setLayoutY(100);
        return this.bp;
    }

}
