#include <iostream>
#include <cmath>
#include <map>
#include <cstdlib>
#include <cstdio>
using namespace std;

int main(int c, char** argv)
{
// https://en.wikipedia.org/wiki/Equal_temperament
// http://www2.eca.usp.br/prof/iazzetta/tutor/acustica/introducao/tabela1.html
    cout << "Equal Temperament: 2^(1/12) = 1.059463" << endl;

    // standard tunning 440Hz
    
    //Eg.:  P40 = 440*pow(2,(60 -69)/12f); // A4 = 69 and C4 is middle C

std::map<int, float> freq_table;

for (int i=0; i<24; i++) freq_table[i] = 0;
for (int i = 24; i<93 ; i++)
{
    float freq =  440*pow(2,(i -69.0f)/12.0f);
    cout << "midi note: " << i << " --- " << freq << endl;  
    freq_table[i] = freq;
}
for (int i=93; i<128; i++) freq_table[i] = 0;

return 0;

}
