public class Speaker implements AudioComponent {

    private AudioComponent input;
    @Override
    public AudioClip getClip() {
        return input.getClip();
    }

    @Override
    public boolean hasInput() {
        return true;
    }

    @Override
    public void connectInput(AudioComponent input) {
        this.input = input;
    }
}
