public class Volume implements AudioComponent{

    private double scale;
    //public AudioClip newClip;
    private AudioComponent input;

    public Volume(double scale){
        this.scale = scale;
    }

    public static int clamp(int setVolume){
        return Math.max(Short.MIN_VALUE, Math.min(setVolume, Short.MAX_VALUE));
    }

    @Override
    public AudioClip getClip() {
        AudioClip set = input.getClip();
        AudioClip newClip = new AudioClip();
        for(int i =0; i< 88200/2; i++){
            double volume = set.getSample(i) * scale;
            newClip.setSample(i, clamp((int)(volume)));
        }
        return newClip;
    }

    @Override
    public boolean hasInput() {
        return true;
    }

    @Override
    public void connectInput(AudioComponent input) {
        this.input = input;
    }

    public double getVolume(){
        return scale;
    }

    public void setVolume(double vol){
        scale = vol;
    }
}

