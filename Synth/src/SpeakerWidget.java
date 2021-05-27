import javafx.scene.Node;

import javax.sound.sampled.Clip;

public class SpeakerWidget extends Widget {
//    public Speaker c = new Speaker();
    SpeakerWidget(String title) {
        super(title, new Speaker());
        getIj();
    }

    public Node getSpeaker() {
        bp.setLayoutX(600);
        bp.setLayoutY(550);
        return this.bp;
    }
}
