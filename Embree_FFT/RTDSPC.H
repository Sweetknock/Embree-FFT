/* real-time DSP C code include file */

/* common sampling rate for sound cards on IBM/PC */
#define SAMPLE_RATE 44100

#define PI 3.14159265358979323846

/* COMPLEX STRUCTURE */

typedef struct {
    float real, imag;
} COMPLEX;

/* function prototypes for fft and filter functions */
	extern void fft(COMPLEX *,int);
    extern float fir_filter(float input,float *coef,int n,float *history);
    extern float iir_filter(float input,float *coef,int n,float *history);
    extern float gaussian(void);

    extern void setup_codec(int),key_down(),int_enable(),int_disable();
    extern int flags(int);

    //extern float getinput(void);
   // extern void sendout(float),flush();
