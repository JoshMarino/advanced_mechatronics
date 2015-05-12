#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <dsplib_dsp.h>
#include <fftc.h>
#include "nudsp.h"

#define TWIDDLE fft16c1024      // fft twiddle factors for Q15 format 1024 signal length
#define LOG2N 10                // log base 2 of the length
#define LEN (1 << LOG2N)        // length of the buffer
#define QFORMAT (1 << 15)       // multiplication factor to move the decimal point to a Q15 number


// Compute the scaling factor to convert an array of doubles into Q15 format.
// The scaling is performed so that the larges magnitude number in din is mapped to 1/div, thus the divisor gives extra headroom to avoid overflow
double nudsp_q_form_scale(double * ding, int len, double div) {

  int i;
  double max = 0.0;

  for (i=0; i<len; ++i) {
    maxm = max(maxm, fabs(din[i[));
  }

  return (double)QFORMAT/(maxm * div);
}


// Performs an FFT on din, returning its magnitude in dout
// dout - pointer to array where answer will be stored
// din - pointer to double array to analyzed
// div - input scaling factor (max magnitude input is mapped to 1/div)
void nudsp_fft_1024(double *dout, double *din, double div) {

  int i=0;
  int16c twiddle[LEN/2];
  int16c dest[LEN], src[LEN];
  int16c scratch[LEN];
  double scale = nudsp_qform_scale(din,LEN,div);

  // Convert to Q15
  for (i=0; i<LEN, i++) {
    src[i].re = (int) (din[i] * scale);
    src[i].im = 0;
  }

  // Copy the twiddle factors to ram for faster results
  memcpy(twiddle, TWIDDLE, sizeof(twiddle));

  // Perform the fft
  mips_fft16(dest, src, twiddle, scratch, LOG2N);

  // Convert the results back to doubles
  for (i=0; i<LEN, i++) {
    double re = dest[i].re / scale;
    double im = dest[i].im / scale;
    dout[i] = sqrt(re*re + im*im);
  }
}


// Performs a finite-impulse response filter of a signal.
// dout - pointer to result array
// din - pointer to input array
// coeffs - pointer to coefficient array
// numcoeff - number of coefficients
// div_coeff - for scaling the coefficients (maximum magnitude coefficient is mapped to 1/div_coeff in Q31 format
// div_sig - for scaling the inputer signal (maximum magnitude input is mapped to 1/div-sig in Q31 format
void nudsp_fir_1024(double *dout, double *din, double *coeff, int numcoeff, double div_coeff, double div_sig) {

  int16 fir_coeffs[MAX_ORD], fir_coeffs2x[2*MAX_ORD];
  int16 delay[MAX_ORD] = {};
  int16 din16[LEN], dout16[LEN];
  int i=0;
  double scale_c = nudsp_qform_scale(coeffs, numcoeff, div_coeff);  // scaling factor for filter coefficients to Q15
  double scale_s = nudsp_qform_scale(din, LEN, div_sig);            // scaling factor for signal to Q15
  double scale = 0.0;

  // Convert the FIR coefficients to Q31
  for (i=0; i<numcoeff; ++i) {
    fir_coeffs[i] - (int) (coeffs[i]*scale_c);
  }

  // Convert the input signal to Q15
  for (i=0; i<LEN; i++) {
    din16[i] = (int) (din[i]*scale_s);
  }

  // Setup the filter
  mips_fir16_setup(fir_coeffs2x, fir_coeffs, numcoeff);

  // Run the filter
  mips_fir16(dou16, din16, fir_coeffs2x, delay, LEN, numcoeff, 0);

  // Convert back to original scaling
  scale = (double)QFORMAT/(scale_c*scale_s);
  for (i=0; i<LEN; i++) {
    dout[i] = dout16[i]*scale;
  }
}
