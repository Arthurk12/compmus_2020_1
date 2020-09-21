
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>

#include "../read_write_wav.h"

using namespace std;

//helper functions
float midi2freq(int m)
{
    return 440*pow(2,(m-69.0f)/12.0f);
}

float freq2midi(float freq)
{
    return 12*log2( freq/440) + 69;
}
//


class SoundProcessor  
{
    public:

        virtual void process(float*, int)
        {
        }
};


class SineOscillator : public SoundProcessor
{
    public: 

        float freq;
        float amp;
        float Fs;


        SineOscillator(int nota_midi, float _amp, float _Fs)
        {
            freq = midi2freq(nota_midi);
            amp = _amp;
            Fs = _Fs;
        }

        ~SineOscillator()
        {
        }   

        void process(float* audio_buffer, int buffer_len) 
        {
            cout << "sine class -> call process: "<<  buffer_len << " amp: "<<  amp << " freq: "<< freq << "  Fs: " << Fs << endl;
            for (int n=0; n<buffer_len; n++)
            {
                //audio_buffer[n] += amp * sin(2*M_PI*freq*(((float)n)/Fs)); 
                audio_buffer[n] = amp * sin(2*M_PI*freq*(((float)n)/Fs)); 
            }
        }

};

class MusicNote
{
    public:
        float start_time; // pos in seconds
        float end_time; // pos in seconds
        SoundProcessor* sp;   

    MusicNote(SoundProcessor *o, float s, float e):sp(o), start_time(s), end_time(e)
    {
    }
};



int main(int c, char** argv)
{
    const float duration = 3.5; //seconds
    const float Fs = 44100; //sample rate (samples /second)
    const int buffer_len = round(duration*Fs); // samples
    float *audio_buffer;
    vector<MusicNote> notes;

    //===============================
    SineOscillator* s1 = new SineOscillator(60, 0.5, Fs);
    MusicNote m1(s1, 0.0f, 5.0f);
    notes.push_back(m1);

    SineOscillator* s2 = new SineOscillator(67, 0.8, Fs);
    MusicNote m2(s2, 2.0f, 7.0f);
    notes.push_back(m2);

    SineOscillator* s3 = new SineOscillator(69, 0.5, Fs);
    MusicNote m3(s3, 6.0f, 8.0f);
    notes.push_back(m3);

    SineOscillator* s4 = new SineOscillator(81, 0.5, Fs);
    MusicNote m4(s4, 6.0f, 8.0f);
    notes.push_back(m4);

    SineOscillator* s5 = new SineOscillator(73, 0.9, Fs);
    MusicNote m5(s5, 8.0f, 9.0f);
    notes.push_back(m5);

    SineOscillator* s6 = new SineOscillator(69, 0.7, Fs);
    MusicNote m6(s6, 9.0f, 10.0f);
    notes.push_back(m6);


    //===============================
    // get max signal duration
    int max_pos = 0;
    for (int k=0; k<notes.size(); k++)
    {
        max_pos = std::max((float)max_pos, (float)round(notes[k].end_time*Fs));
    }
    audio_buffer = new float[max_pos];
    memset(audio_buffer, 0, max_pos);

    cout << "maxPos: " << max_pos <<  endl;
    // write the notes into the audio buffer
    for (int k=0; k<notes.size(); k++)
    {
        int startPos = notes[k].start_time*Fs;
        int endPos = notes[k].end_time*Fs;
        cout << "startPos: " << startPos << endl;
        cout << "endPos: " << endPos << endl;
        cout << "opa" << endl;
        notes[k].sp->process(audio_buffer + startPos, endPos-startPos);
    }

    // ============================
    // save output wave
    string wav_name = "melody.wav";
    write_wave_file (wav_name.c_str(), audio_buffer, max_pos, Fs);
    cout << "done." << endl;
    delete [] audio_buffer;
    return 0;
}



