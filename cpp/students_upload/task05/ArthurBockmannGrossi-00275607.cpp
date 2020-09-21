
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>

#include "../../read_write_wav.h"

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
                audio_buffer[n] += amp * sin(2*M_PI*freq*(((float)n)/Fs)); 
            }
        }

};

class SawToothOscillator : public SoundProcessor
{
    public: 

        float freq;
        float amp;
        float Fs;
        float Fs_freq;


        SawToothOscillator(int nota_midi, float _amp, float _Fs)
        {
            freq = midi2freq(nota_midi);
            amp = _amp;
            Fs = _Fs;
            Fs_freq = Fs/freq;
        }

        SawToothOscillator(float freq, float _amp, float _Fs)
        {
            freq = freq;
            amp = _amp;
            Fs = _Fs;
            Fs_freq = Fs/freq;
        }

        ~SawToothOscillator()
        {
        }   

        void process(float* audio_buffer, int buffer_len) 
        {
            cout << "sawtooth class -> call process: "<<  buffer_len << " amp: "<<  amp << " freq: "<< freq << "  Fs: " << Fs << endl;
            for (int n=0; n<buffer_len; n++)
            {
                audio_buffer[n] += amp * -2/M_PI * atan(1/tan(freq * M_PI * (((float)n)/Fs)));
            }
        }

};

class Sampler : public SoundProcessor{
    
    public:

        int sample_audio_sample_rate;
        float* sample_audio_buffer;
        int sample_audio_buffer_len;
        char* filename;
        int n_channels;


        Sampler(char* _filename) :
        filename(_filename)
        {
            n_channels = read_wave_file(filename, sample_audio_buffer, &sample_audio_buffer_len, &sample_audio_sample_rate);
        }

        ~Sampler()
        {
            delete [] sample_audio_buffer;
        }

        void process(float* audio_buffer, int buffer_len){
            cout << "Sampler -> call proccess:      file:" << filename << "  sample rate:" << sample_audio_sample_rate << endl;
            for(int n=0; n<buffer_len;n++){
                audio_buffer[n] += 0.4 * sample_audio_buffer[n*2];
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
    const float duration = 8; //seconds
    const float Fs = 44100; //sample rate (samples /second)
    const int buffer_len = round(duration*Fs); // samples
    float *audio_buffer;
    vector<MusicNote> notes;

    //===============================

    SineOscillator* do_note = new SineOscillator(69, 0.3, Fs);
    SineOscillator* mi_note = new SineOscillator(73, 0.3, Fs);
    SineOscillator* sol_note = new SineOscillator(76, 0.3, Fs);

    SineOscillator* si_note = new SineOscillator(80, 0.3, Fs);
    SineOscillator* re_note = new SineOscillator(83, 0.3, Fs);

    SineOscillator* la_note = new SineOscillator(78, 0.3, Fs);

    SineOscillator* fa_note = new SineOscillator(74, 0.3, Fs);
    SineOscillator* do5_note = new SineOscillator(81, 0.3, Fs);

    Sampler *hey = new Sampler("./hey.wav");



    MusicNote m1(do_note, 0.0f, 2.0f);
    notes.push_back(m1);

    MusicNote m2(mi_note, 0.1f, 2.0f);
    notes.push_back(m2);

    MusicNote m3(sol_note, 0.2f, 2.0f);
    notes.push_back(m3);

    MusicNote m4(sol_note, 2.0f, 4.0f);
    notes.push_back(m4);

    MusicNote m5(si_note, 2.1f, 4.0f);
    notes.push_back(m5);

    MusicNote m6(re_note, 2.2f, 4.0f);
    notes.push_back(m6);

    MusicNote m7(la_note, 4.1f, 6.0f);
    notes.push_back(m7);

    MusicNote m8(si_note, 4.2f, 6.0f);
    notes.push_back(m8);

    MusicNote m9(re_note, 4.3f, 6.0f);
    notes.push_back(m9);

    MusicNote m10(fa_note, 6.1f, 8.0f);
    notes.push_back(m10);

    MusicNote m11(la_note, 6.2f, 8.0f);
    notes.push_back(m11);

    MusicNote m12(do5_note, 6.3f, 8.0f);
    notes.push_back(m12);
    
    MusicNote voice_effect1(hey, 1.5f, 2.0f);
    notes.push_back(voice_effect1);

    MusicNote voice_effect2(hey, 3.5f, 4.0f);
    notes.push_back(voice_effect2);

    MusicNote voice_effect3(hey, 5.5f, 6.0f);
    notes.push_back(voice_effect3);

    MusicNote voice_effect4(hey, 7.5f, 8.0f);
    notes.push_back(voice_effect4);


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
        notes[k].sp->process(audio_buffer + startPos, endPos-startPos);
    }

    // ============================
    // save output wave
    string wav_name = "ArthurBockmannGrossi-00275607.wav";
    write_wave_file (wav_name.c_str(), audio_buffer, max_pos, Fs);
    cout << "done." << endl;
    delete [] audio_buffer;
    return 0;
}



