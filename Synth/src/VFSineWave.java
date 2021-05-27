public class VFSineWave implements AudioComponent{
    private AudioComponent input;

    public int SampleRate = 44100;

    @Override
    public AudioClip getClip() {
        float phase = 0;
        AudioClip clip = new AudioClip();
        AudioClip original = input.getClip();
        for(int i = 0; i<88200; i++){
            phase += 2*Math.PI*original.getSample(i) / SampleRate;
            clip.setSample(i, (int)(Short.MAX_VALUE*Math.sin(phase)));
        }
        return clip;
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
