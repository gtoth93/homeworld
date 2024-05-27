#include "fqcodec.h"
#include "dct.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <numbers>


std::array<float, FQ_QSIZE> FQ_HCOEF{};
std::array<float, FQ_HSIZE> FQ_COEF{};
std::array<float, FQ_SIZE> FQ_DCOEF{};

std::array<float, FQ_HSIZE> FQ_HWIN{};
std::array<float, FQ_SIZE> FQ_WIN{};
std::array<float, FQ_DSIZE> FQ_DWIN{};

double fChopT = 0;

auto fqDecOver(const float* aFPBlock, const float* aFSBlock, float* aTPBlock, float* aTSBlock, const float* aCBlock,
               const float* aWBlock, const uint32_t nSize) -> int32_t {
    const auto hlen = nSize >> 1;
    std::array<float, FQ_DSIZE> buf{};
    std::copy_n(aTSBlock, FQ_DSIZE, aTPBlock);
    idct(aFPBlock, buf.data(), aCBlock, nSize);
    idct(aFSBlock, aTSBlock, aCBlock, nSize);
    for (size_t i = 0; i < hlen; i++) {
        aTPBlock[i + hlen] = buf[i] * aWBlock[i] + aTPBlock[i + hlen] * aWBlock[i + hlen];
        aTSBlock[i] = aTSBlock[i] * aWBlock[i] + buf[i + hlen] * aWBlock[i + hlen];
    }
    return OK;
}

auto fqDecBlock(const float* aFPBlock, const float* aFSBlock, float* aTPBlock, float* aTSBlock, const int32_t nMode,
                const int32_t nFact) -> int32_t {
    if (nMode == FQ_MINIT) {
        std::fill_n(aTSBlock, FQ_DSIZE, 0.0F);
        for (size_t i = 0; i < FQ_HSIZE; i++) {
            FQ_HWIN[i] = std::sinf(static_cast<float>(i) * (std::numbers::pi_v<float> / FQ_HSIZE));
        }
        for (size_t i = 0; i < FQ_SIZE; i++) {
            FQ_WIN[i] = std::sinf(static_cast<float>(i) * (std::numbers::pi_v<float> / FQ_SIZE));
        }
        for (size_t i = 0; i < FQ_DSIZE; i++) {
            FQ_DWIN[i] = std::sinf(static_cast<float>(i) * (std::numbers::pi_v<float> / FQ_DSIZE));
        }
        Initdct(FQ_HCOEF.data(), FQ_HSIZE);
        Initdct(FQ_COEF.data(), FQ_SIZE);
        Initdct(FQ_DCOEF.data(), FQ_DSIZE);
    }
    else if (nFact == 4) {
        fqDecOver(aFPBlock, aFSBlock, aTPBlock, aTSBlock, FQ_HCOEF.data(), FQ_HWIN.data(), FQ_HSIZE);
    }
    else if (nFact == 2) {
        if (nMode == FQ_MHALF) {
            std::array<float, FQ_DSIZE> buf{};
            fqDecOver(aFPBlock, aFSBlock, buf.data(), aTSBlock, FQ_HCOEF.data(), FQ_HWIN.data(), FQ_HSIZE);
            for (size_t i = 0; i < FQ_SIZE - 1; i += 2) {
                aTPBlock[i] = buf[i >> 1];
                aTPBlock[i + 1] = buf[i >> 1];
            }
        }
        else {
            fqDecOver(aFPBlock, aFSBlock, aTPBlock, aTSBlock, FQ_COEF.data(), FQ_WIN.data(), FQ_SIZE);
        }
    }
    else if (nMode == FQ_MHALF) {
        std::array<float, FQ_DSIZE> buf{};
        fqDecOver(aFPBlock, aFSBlock, buf.data(), aTSBlock, FQ_HCOEF.data(), FQ_HWIN.data(), FQ_HSIZE);
        for (size_t i = 0; i < FQ_DSIZE - 1; i += 4) {
            aTPBlock[i] = buf[i >> 2];
            aTPBlock[i + 1] = buf[i >> 2];
            aTPBlock[i + 2] = buf[i >> 2];
            aTPBlock[i + 3] = buf[i >> 2];
        }
    }
    else if (nMode == FQ_MNORM) {
        std::array<float, FQ_DSIZE> buf{};
        fqDecOver(aFPBlock, aFSBlock, buf.data(), aTSBlock, FQ_COEF.data(), FQ_WIN.data(), FQ_SIZE);
        for (size_t i = 0; i < FQ_DSIZE - 1; i += 2) {
            aTPBlock[i] = buf[i >> 1];
            aTPBlock[i + 1] = buf[i >> 1];
        }
    }
    else {
        fqDecOver(aFPBlock, aFSBlock, aTPBlock, aTSBlock, FQ_DCOEF.data(), FQ_DWIN.data(), FQ_DSIZE);
    }
    return OK;
}

auto fqWriteTBlock(const float* aLBlock, const float* aRBlock, const int16_t nChan, void* pBuf1, const uint32_t nSize1,
                   void* pBuf2, const uint32_t nSize2) -> int32_t {
    size_t j = 0;
    if (pBuf1 != nullptr && nSize1 > 0) {
        for (size_t i = 0; i < nSize1 - 1; i += 2) {
            if (j > FQ_DSIZE) {
                return ERR;
            }
            const auto lword = std::clamp((fq_rint(aLBlock[j])), SHRT_MIN, SHRT_MAX);
            static_cast<int16_t *>(pBuf1)[i] = static_cast<int16_t>(lword);
            if (nChan > 1) {
                static_cast<int16_t *>(pBuf1)[i + 1] = static_cast<int16_t>(std::clamp(
                    (fq_rint(aRBlock[j])), SHRT_MIN, SHRT_MAX));
            }
            else {
                static_cast<int16_t *>(pBuf1)[i + 1] = static_cast<int16_t>(lword);
            }
            j++;
        }
    }
    if (pBuf2 != nullptr && nSize2 > 0) {
        for (size_t i = 0; i < nSize2 - 1; i += 2) {
            if (j > FQ_DSIZE) {
                return ERR;
            }
            const auto lword = std::clamp((fq_rint(aLBlock[j])), SHRT_MIN, SHRT_MAX);
            static_cast<int16_t *>(pBuf2)[i] = static_cast<int16_t>(lword);
            if (nChan > 1) {
                static_cast<int16_t *>(pBuf2)[i + 1] = static_cast<int16_t>(std::clamp(
                    (fq_rint(aRBlock[j])), SHRT_MIN, SHRT_MAX));
            }
            else {
                static_cast<int16_t *>(pBuf2)[i + 1] = static_cast<int16_t>(lword);
            }
            j++;
        }
    }
    return OK;
}
