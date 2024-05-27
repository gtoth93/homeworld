// Copyright (c) 1998-99 Relic Entertainment Inc.
// Written by Janik Joire
//
// $History: $
//
// Version 1.6a

#ifndef FQEFFECT_H
#define FQEFFECT_H

#include <stdint.h>

// Constants
#ifndef OK
#define OK		0
#endif

#ifndef ERR
#define ERR		-1
#endif

#define UNIFORM		((float)(pRandF(nRandP)&RAND_MAX)/RAND_MAX-0.5F)

// Freq and time constants
#define FQ_BAND			86.1328125F		// Hz
#define FQ_SLICE		11.60997732426F	// msec

// Factor constants
#define FQ_FNOISE		1000.0F
#define FQ_FTONE		1000.0F

// EQ constants
#define FQ_EQNUM		10			// # of EQ bands
#define FQ_EQTAB		aEQBlock	// EQ bands

// Process constants
#define FQ_PGEN			0
#define FQ_PADD			1

// Structures
#pragma pack(push, 4)

typedef struct {
    uint32_t nClockCount; // 0 Clock count (1 unit = approx 6 ms)

    uint32_t nFiltMinFreq; // 4 Minimum filter frequency (1 unit = approx. 86 Hz)
    uint32_t nFiltMaxFreq; // 8 Maximum filter frequency (1 unit = approx. 86 Hz)

    uint32_t nToneMinFreq; // c Minimum tone frequency (1 unit = approx. 86 Hz)
    uint32_t nToneMaxFreq; // 10 Maximum tone frequency (1 unit = approx. 86 Hz)
    uint32_t nToneDur; // 14 Tone duration (1 unit = approx. 6 msec)
    uint32_t nToneMute; // 18 Tone mute (1 unit = approx. 6 msec)
    uint32_t nToneCount; // 1c Tone count

    uint32_t nBreakMaxRate; // 20 Maximum break rate (1 unit = approx. 6 msec)
    uint32_t nBreakMaxDur; // 24 Maximum break duration (1 unit = approx. 6 msec)

    uint32_t nQNoiseMaxRate; // 28 Maximum q-noise rate (1 unit = approx. 6 msec)
    uint32_t nQNoiseMaxDur; // 2c Maximum q-noise duration (1 unit = approx. 6 msec)

    float fScaleLev; // 30 Scale level (1.0 = 100%)
    float fNoiseLev; // 34 Noise level (maximum = arbit. 1000.0)
    float fToneLev; // 38 Tone level (maximum = arbit. 10000.0)
    float fLimitLev; // 3c Limiter level (1.0 = 100%)

    float fPitchShift; // 40 Pitch shift (0.5 = 1 octave down, 1.0 = none, 2.0 = 1 octave up)

    float* pEQLev; // 44 EQ levels (1.0 = 100%)
} EFFECT; // 72 bytes

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

// Functions
int32_t fqRand(int32_t (*pFunc)(int32_t), int32_t nParam);

double fqSqrt(double (*pFunc)(double));

int32_t fqSize(uint32_t nSize);

int32_t fqAdd(float* aPBlock, const float* aSBlock);

// int32_t fqMult(float* aPBlock, const float* aSBlock);
//
// int32_t fqMax(float* aPBlock, const float* aSBlock);

int32_t fqScale(float* aBlock, float fLev);

int32_t fqMix(float* aPBlock, const float* aSBlock, float fLev);

// int32_t fqFilter(float* aBlock, uint32_t nMinFreq, uint32_t nMaxFreq);
//
// int32_t fqAddNoise(float* aBlock, float fLev, uint32_t nMinFreq, uint32_t nMaxFreq);
//
// int32_t fqGenNoise(float* aBlock, float fLev, uint32_t nMinFreq, uint32_t nMaxFreq);

int32_t fqPitchShift(float* aBlock, float fShift);

// int32_t fqPitchSlide(float* aBlock, int32_t nSlideFreq);
//
// int32_t fqStretch(float* aRBlock, float* aBlock, float* aBuf);
//
// int32_t fqGate(float* aBlock, float fLev);
//
// int32_t fqLimit(float* aBlock, float fLev);

int32_t fqEqualize(float* aBlock, const float* aEq);

int32_t fqDelay(float* aBlock, float fLev, uint32_t nDur, float* aBuf, int32_t nSize, int32_t* nPos);

int32_t fqAcModel(float* aBlock, const float* aEq, uint32_t nDur, float* aBuf, int32_t nSize, int32_t* nPos);

int32_t fqInitE(EFFECT* rEffect);

// int32_t fqScaleE(float* aBlock, const EFFECT* rEffect);
//
// int32_t fqMixE(float* aPBlock, const float* aSBlock, const EFFECT* rEffect);

int32_t fqFilterE(float* aBlock, const EFFECT* rEffect);

int32_t fqAddNoiseE(float* aBlock, const EFFECT* rEffect);

int32_t fqAddToneE(float* aBlock, EFFECT* rEffect);

int32_t fqAddBreakE(float* aBlock, const EFFECT* rEffect);

// int32_t fqGenNoiseE(float* aBlock, const EFFECT* rEffect);
//
// int32_t fqGenBreakE(float* fLev, const EFFECT* rEffect);

int32_t fqGenQNoiseE(char* aQBlock, uint32_t nRate, const EFFECT* rEffect);

// int32_t fqPitchShiftE(float* aBlock, const EFFECT* rEffect);

int32_t fqLimitE(float* aBlock, const EFFECT* rEffect);

// int32_t fqEqualizeE(float* aBlock, const EFFECT* rEffect);

int32_t rrand(int32_t nDummy);

// Math functions
float gaussian();

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // FQEFFECT_H
