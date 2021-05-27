public class AudioClip {

    public static final double duration = 2.0;
    public static final int sampleRate = 44100;
    public byte [] byteArray = new byte[176400];


    public AudioClip(){

    }


    public int getSample(int index){
        short low = byteArray[2*index];
        low = (short)(low & 0xFF);
        short high = byteArray[2*index+1];
        high = (short)(high<<8);
        int sample = low | high;
        return sample;
    }

    public void setSample(int index, int value){
        int mask = 0x00ff;//maybe not necessary
        byteArray[2*index] = (byte)(value& mask);
        mask = mask <<8;
        short high = (short)(value & mask);
        byteArray[2*index+1]= (byte)(high >>8);

    }

    byte [] getData(){
        return byteArray;
    }

    int getSampleRate(){
        return sampleRate;
    }
}
