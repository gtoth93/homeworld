#ifndef MIXFFT_H
#define MIXFFT_H

#ifdef __cplusplus
extern "C" {
#endif

void fft(long n,float *xRe,float *xIm,float *yRe,float *yIm);

#ifdef __cplusplus
}		// extern "C"
#endif

#endif
