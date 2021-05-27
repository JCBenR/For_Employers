public class LinearRamp implements AudioComponent{


    private float start;
    private float stop;
    int numSamples = 88200;
    public String name = "LinearRamp";

    public LinearRamp(float start, float stop){
        this.start = start;
        this.stop = stop;
    }

    int getTotalSamples(){
        return numSamples;
    }
    
    
    @Override
    public AudioClip getClip() {
        AudioClip clip = new AudioClip();
        for(int i=0; i<numSamples; i++) {
            clip.setSample(i, (int)(start * (numSamples - i) + stop * i) / numSamples);
        }
        return clip;
    }

    @Override
    public boolean hasInput() {
        return false;
    }

    @Override
    public void connectInput(AudioComponent input) {

    }

    public float getStart(){
        return start;
    }

    public float getStop() {
        return stop;
    }

    public void setStart(float start){
        this.start = start;
    }

    public void setStop(float stop){
        this.stop = stop;
    }
}
