import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;

//currently mixer will play combined sinewaves, but only based on the values they are set at BEFORE they go into the mixer. updates after they've gone into the mixer are not reflected.

public class MixerWidget extends Widget{
    MixerWidget(String title, Mixer mixer) {
        super(title, mixer);
        GridPane cgp = new GridPane();
        cgp.setHgap(5);
        getIj();
        getOj();
    }

    BorderPane getMixer(){
        bp.setLayoutX(100);
        bp.setLayoutY(100);
        return this.bp;
    }
}
