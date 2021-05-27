public class SineWave implements AudioComponent {

    //sample[i] = maxValue * sine(2*pi*frequency * i / sampleRate);

    private int pitch;

    public int getPitch(){
        return pitch;
    }
    public String name = "Sine Wave";



    SineWave(int frequency){
        pitch = frequency;
    }

    @Override
    public AudioClip getClip() {
        AudioClip newClip = new AudioClip();
        for(int i =0; i<88200/2; i++){
            newClip.setSample(i, (int) ((Short.MAX_VALUE) * Math.sin(2*Math.PI*pitch *i/ newClip.getSampleRate())));
        }
        return newClip;
    }

    @Override
    public boolean hasInput() {
        return false;
    }

    @Override
    public void connectInput(AudioComponent input) {

    }

    public void setFrequency(int frequency){
        pitch = frequency;
    }

    public int getFrequency(){
        return pitch;
    }
}
