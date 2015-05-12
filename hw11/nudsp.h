#ifndef NU__DSP__H__
#define NU__DSP__H__
// wraps some dsp library functions making it easier to use them with doubles
// all provided operations assume signal lengths of 1024 elements

#define MAX_ORD 128		// maximum order of the FIR filter

// compute a scaling factor for converting doubles into Q15 numbers
double nudsp_qform_scale(double * din, int len, double div);

// fft a signal that has 1024 samples
void nudsp_fft_1024(double * dout, double * din, double div);

// fit filter a signal that has 1024 samples
void nudsp_fir_1024(double *dout, double *din, double *coeffs, int numcoeff, double div_coeff, double div_sig);

#endif
