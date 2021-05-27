import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.scene.control.Slider;
import javafx.scene.layout.*;

public class SineWaveFilterWidget extends Widget{
//    SineWave swAudio;
    public static Boolean hasInput = false;
//    public static AudioClip output;
    SineWaveFilterWidget(String title, SineWave sw){
        super(title, sw);
        GridPane cgp = new GridPane();
        cgp.setHgap(5);
        getIj();
        getOj();

        //slider
        Slider slider = new Slider(65,1975,sw.getFrequency());
        slider.valueProperty().addListener(new ChangeListener<Number>() {
            @Override
            public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                sw.setFrequency((int)slider.getValue());
            }
        });
        System.out.println(sw.getFrequency());

        cgp.add(buildSlider("frequency", slider),0,0);
        bp.setCenter(cgp);
    }

    public Boolean isOj(){
        if(oj.isVisible()){
            return true;
        } return false;
    }

    BorderPane getSineWave(){
        bp.setLayoutX(100);
        bp.setLayoutY(100);
        return this.bp;
    }

}
