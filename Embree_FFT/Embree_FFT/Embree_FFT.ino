
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "rtdspc.h"
using namespace std;

/*********************************************************************

RTPSE.C - Real-Time Power spectral estimation using the FFT

This program does power spectral estimation on input samples.
The average power spectrum in each block is determined
and used to generate a series of outputs.

Length of each FFT snapshot: 64 points
Number of FFTs to average: 16 FFTs
Amount of overlap between each FFT: 60 points

*********************************************************************/

/* FFT length must be a power of 2 */
#define SAMPLE_RATE 44100
#define FFT_LENGTH 512
#define M 9            /* must be log2(FFT_LENGTH) */
int micIN= A0;
int audioVal = 0;
float delayUs = (1000000.0/SAMPLE_RATE)*2/3;

void setup() {
}


void loop() {
  Serial.begin(9600);
  
  int numav = 3;
  int ovlap = 100;
  int            i,j,k;
  float          scale,tempflt;
  static float   mag[FFT_LENGTH], sig[FFT_LENGTH], hamw[FFT_LENGTH];
  static COMPLEX samp[FFT_LENGTH];
  

/* overall scale factor */
  scale = 1.0f/(float)FFT_LENGTH;
  scale *= scale/(float)numav;

/* calculate hamming window */
  tempflt = 8.0*atan(1.0)/(FFT_LENGTH-1);
  for(i = 0 ; i < FFT_LENGTH ; i++)
    hamw[i] = 0.54 - 0.46*cos(tempflt*i);

for(;;){
    /* read in the first FFT_LENGTH samples, overlapped samples read in loop */
    for (i = 0; i < FFT_LENGTH; i++) {
      sig[i] = analogRead(micIN)-505;
      delayMicroseconds(delayUs);
      //Serial.println(sig[i]);
      //printf("%f\n", sig[i]);
    }
    

    for (k=0; k<FFT_LENGTH; k++) mag[k] = 0;

    for (j=0; j<numav; j++){

      for (k=0; k<FFT_LENGTH; k++){
        samp[k].real = hamw[k]*sig[k];
        samp[k].imag = 0;
      }

      fft(samp,M);

      for (k=0; k<FFT_LENGTH; k++){
        tempflt  = samp[k].real * samp[k].real;
        tempflt += samp[k].imag * samp[k].imag;
        tempflt = scale*tempflt;
        mag[k] += tempflt;
      }

/* overlap the new samples with the old */
      for(k = 0 ; k < ovlap ; k++) sig[k] = sig[k+FFT_LENGTH-ovlap];
      for( ; k < FFT_LENGTH ; k++) {
      sig[k] = analogRead(micIN)-505;
      delayMicroseconds(delayUs);
      }
    }

    float max = 0;
/*  Take log after averaging the magnitudes.  */
    for (k=0; k<FFT_LENGTH/2; k++){
      tempflt = mag[k];
      if(tempflt < 1.e-10f) tempflt = 1.e-10f;

      //float freq = ((float)k / FFT_LENGTH) * 44100;
      //char freq_buff[8];
      //sprintf(freq_buff, "%08.2f", freq);
      
        float output = 10.000*log10(tempflt);
        float freq = ((float)k/FFT_LENGTH)*SAMPLE_RATE;

        if (mag[k] > max){
          max = freq;
        }
    
        char freq_buff[8];
        sprintf(freq_buff, "%.6f", freq);
        Serial.print(freq_buff[0]);
        Serial.print(freq_buff[1]);
        Serial.print(freq_buff[2]);
        Serial.print(freq_buff[3]);     
        Serial.print(freq_buff[4]);
        Serial.print(freq_buff[5]);     
        Serial.print(freq_buff[6]);
        Serial.print(freq_buff[7]);

    Serial.print(" ");

        char output_buff[5];
        sprintf(output_buff, "%.3f", output);
        Serial.print(output_buff[0]);
        Serial.print(output_buff[1]);
        Serial.print(output_buff[2]);
        Serial.print(output_buff[3]);     
        Serial.print(output_buff[4]);
        

    }
    //Serial.print(delayUs);
     //Serial.print(max);
     //Serial.print("\n");
  }

}
