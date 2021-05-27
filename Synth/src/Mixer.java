import java.util.ArrayList;


public class Mixer implements AudioComponent{
    public String name = "Mixer";

    ArrayList<AudioClip> clipList = new ArrayList<>();

//    AudioComponent test1 = new SineWave(329);
//    AudioComponent test2 = new SineWave(262);
//    AudioComponent test3 = new SineWave(392);
public static int clamp(int setVolume){
    return Math.max(Short.MIN_VALUE, Math.min(setVolume, Short.MAX_VALUE));
}
    @Override
    public AudioClip getClip() {
        AudioClip newSample = new AudioClip();
        for(int i=0; i< clipList.size(); i++){
            AudioClip mix = clipList.get(i);
            for(int j = 0; j<88200; j++){
                short sample = (short) mix.getSample(j);
                newSample.setSample(j, clamp(newSample.getSample(j)+sample));
            }
        }return newSample;
    }

    @Override
    public boolean hasInput() {
        return true;
    }

    @Override
    public void connectInput(AudioComponent input) {
        clipList.add(input.getClip());
    }
}
