// Copyright (c) 1998-99 Relic Entertainment Inc.
// Written by Janik Joire
//
// $History: $
//
// Version 1.6a

#ifndef FQUANT_H
#define FQUANT_H

#include <stdint.h>

// General constants
#ifndef OK
#define OK		0
#endif

#ifndef ERR
#define ERR		-1
#endif

#define FQ_LEN		260			// Block length (bytes)
#define FQ_SCALE	10.0F		// Quantization scale
#define FQ_EXP		6			// # of exponents
#define FQ_FMASK	1.0F		// Frequency masking factor
#define FQ_TPMASK	2.0F		// Primary time masking factor
#define FQ_TSMASK	4.0F		// Secondary time masking factor
#define	FQ_THRES	aTBlock		// Hearing thresholds
#define FQ_STAT		pSStream	// Statistics stream

// Scale constants
#define FQ_SDATA	aSDBlock	// Scale data

// Encoder flags
#define FQ_ENORM	0x0000	// Normal
#define FQ_ERESET	0x0001	// Reset exponents
#define FQ_EMIX		0x0002	// Mix primary and secondary blocks

// Quantization limits (# of bits)
#define FQ_NBMAX	6		// Maximum
#define FQ_NBMIN	1		// Minimum

// Compression bitrates (per block)
//		Name		Bits/block	   Kbps/channel
#define	FQ_BR176	2048		// 176
#define	FQ_BR88		1024		// 88
#define	FQ_BR44		512			// 44
#define	FQ_BR22		256			// 22

// Critical band constants
#define	FQ_CBDATA	aCBBlock	// Critical band data
#define FQ_CBNUM	27			// # of critical bands

// Sub-band constants
#define FQ_SBMODE	22			// # of modes
#define FQ_SBSIZE	aSBSBlock	// Sub-band sizes
#define	FQ_SBDATA	aSBDBlock	// Sub-band data
#define FQ_SBNUM	256			// # of sub-bands

#ifdef __cplusplus
extern "C" {
#endif

// Functions

// int32_t fqInitQuant(FILE* pStream);
//
// int32_t fqQuantBlock(char* aQBlock, float* aFPBlock, float* aFSBlock, float* aFMBlock, uint8_t* aEBlock, uint32_t nLen,
//                      uint32_t nRate, uint32_t nSize, int32_t nFlags);

int32_t fqInitDequant(void);

int32_t fqDequantBlock(const char* aQBlock, float* aFPBlock, float* aFSBlock, uint8_t* aEBlock, uint32_t nLen, uint32_t nRate,
                       uint32_t nSize);

// int32_t fqSPack(int16_t nVal, uint32_t nLen, uint32_t nPos, char* aBlock);

_inline int32_t fqSUnpack(uint32_t nLen, uint32_t nPos, const char* aBlock);

_inline uint32_t fqUnpack(uint32_t nLen, uint32_t nPos, const char* aBlock);

// int32_t linint(float* xa, float* ya, uint32_t n, float x, float* y);

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // FQUANT_H
