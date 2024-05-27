// Copyright (c) 1997-99 Relic Entertainment Inc.
// Written by Janik Joire
//
// $History: $
//
// Version 1.6a

#ifndef DCT_H
#define DCT_H

#include <stdint.h>

// General constants
#ifndef OK
#define OK		0
#endif

#ifndef ERR
#define ERR		-1
#endif

#ifndef PI
#define PI		3.14159265358979323846F
#endif

#define DOUBSIZE 512
#define FULLSIZE 256
#define HALFSIZE 128

#ifdef __cplusplus
extern "C" {
#endif

    // int32_t dct(float* f, float* g, float* c, uint32_t n);

    int32_t idct(const float* f, float* g, const float* c, uint32_t n);

    int32_t Initdct(float* c, uint32_t n);

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // DCT_H
