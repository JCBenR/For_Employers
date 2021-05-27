public class SquareWave implements AudioComponent {


    private final int maxValue;
    private AudioComponent input;
    private int scale;
    private int sampleRate = 44100;
    private int frequency;

    public SquareWave(int frequency, int maxValue){
        this.maxValue = maxValue;
        this.frequency = frequency;
    }

    public static int clamp(int setVolume){
        return Math.max(Short.MIN_VALUE, Math.min(setVolume, Short.MAX_VALUE));
    }

    @Override
    public AudioClip getClip() {

        AudioClip set = input.getClip();
        //AudioClip newClip = new AudioClip();
        for(int i =0; i< 88200/2; i++){
            if(  ( frequency * i / sampleRate) % 1 > 0.5) { set.setSample(i, maxValue);}
            else { set.setSample(i, (-maxValue));}
        }
        return set;
    }

    @Override
    public boolean hasInput() {
        return false;
    }

    @Override
    public void connectInput(AudioComponent input) {
        this.input = input;
    }
}
