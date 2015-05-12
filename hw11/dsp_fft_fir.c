#include "NU32.h"
#include "nudsp.h"
// Recieves a signal and FIR filter coefficients from the computer.
// Filters the signal and FFT's the signal and filtered signal, returning the results.
// We omit error checking for clariy, but always include it in your own coder.

#define SIGNAL_LENGTH 1024
#define FFT_SCALE 10.0
#define FIR_COEFF_SCALE 10.0
#define FIR_SIG_SCALE 10.0

#define MSG_LEN 128

int main(void) {
	char msg[MSG_LEN];											// communication buffer
	double fft_orig[SIGNAL_LENGTH] = {};		// fft of the original signal
	double fft_fir[SIGNAL_LENGTH] = {};			// fft of the fir filtered signal
	double xfir[SIGNAL_LENGTH] = {};				// fir filtered signal
	double singal[SIGNAL_LENGTH] = {};			// signal
	double fir[MAX_ORD] = {};								// fir filter coefficients
	int i = 0;
	int slen, clen; 												// signal and coefficient lengths

	NU32_Startup();

	while (1) {
		// Read the signal from UART
		NU32_ReadUART1(msg, MSG_LEN);
		sscanf(msg,"%d",&slen);
		for (i=0; i<slen; ++i) {
			NU32_ReadUART1(msg, MSG_LEN);
			sscanf(msg,"%f",&signal[i]);
		}

		// Read the filter coefficients from UART
		NU32_ReadUART1(msg, MSG_LEN);
		sscanf(msg,"%d",&clen);
		for (i=0; i<clen; ++i) {
			NU32_ReadUART1(msg, MSG_LEN);
			sscanf(msg,"%f",&fir[i]);
		}

		// fir filter the signal
		nudsp_fir_1024(xfir, signal, fir, clen, FIR_COEFF_SCALE, FIR_SIG_SCALE);

		// FFT the original signal
		nudsp_fft_1024(fft_orig, signal, FFT_SCALE);

		// FFT the FIR signal
		nudsp_fft_1024(fft_fir, xfir, FFT_SCALE);

		// Send the results to the computer
		sprintf(msg,"%d\r\n",SIGNAL_LENGTH);
		NU32_WriteUART1(msg);
		for (i=0; i<SIGNAL_LENGTH; ++i) {
			sprintf(msg,"%12.6f, %12.6f, %12.6f, %12.6f\r\n", signal[i], xfir[i], fft_orig[i], fft_fir[i]);
			NU32_WriteUART1(msg);
		}
	}
	return 0;
}
