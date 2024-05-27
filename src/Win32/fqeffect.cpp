#include "fqeffect.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <random>

int32_t (*pRandF)(int32_t) = rrand;

int32_t nRandP = 0;

double (*pSqrtF)(double) = std::sqrt;

uint32_t nBSize = 256;

std::array<uint32_t, 10> FQ_EQTAB{0, 1, 2, 4, 8, 16, 32, 64, 128, 256};

std::default_random_engine rnd{std::random_device()()};
std::uniform_int_distribution dist{0, RAND_MAX};

bool gotgauss = false;
float gauss = 0.0F;

auto fqRand(int32_t (*pFunc)(int32_t), const int32_t nParam) -> int32_t {
    pRandF = pFunc;
    nRandP = nParam;
    return OK;
}

auto fqSqrt(double (*pFunc)(double)) -> double {
    pSqrtF = pFunc;
    return OK;
}

auto fqSize(const uint32_t nSize) -> int32_t {
    nBSize = nSize;
    nBSize = std::min<uint32_t>(nBSize, 256);
    return OK;
}

auto fqAdd(float* aPBlock, const float* aSBlock) -> int32_t {
    for (size_t i = 0; i < nBSize; i++) {
        if (aPBlock[i] != 0.0F) {
            if (aSBlock[i] != 0.0F) {
                aPBlock[i] += aSBlock[i];
            }
        }
        else {
            aPBlock[i] = aSBlock[i];
        }
    }
    return OK;
}

auto fqMult(float* aPBlock, const float* aSBlock) -> int32_t {
    for (size_t i = 0; i < nBSize; i++) {
        if (aPBlock[i] != 0.0F) {
            if (aSBlock[i] != 0.0F) {
                aPBlock[i] *= aSBlock[i];
            }
            else {
                aPBlock[i] = 0.0F;
            }
        }
    }
    return OK;
}

auto fqMax(float* aPBlock, const float* aSBlock) -> int32_t {
    for (size_t i = 0; i < nBSize; i++) {
        const auto a = aPBlock[i] >= 0.0F ? aPBlock[i] : -aPBlock[i];
        if (const auto b = aSBlock[i] >= 0.0F ? aSBlock[i] : -aSBlock[i]; a < b) {
            aPBlock[i] = aSBlock[i];
        }
    }
    return OK;
}

auto fqScale(float* aBlock, const float fLev) -> int32_t {
    if (fLev != 1.0F) {
        if (fLev != 0.0F) {
            for (size_t i = 0; i < nBSize; i++) {
                if (aBlock[i] != 0.0F) {
                    aBlock[i] *= fLev;
                }
            }
        }
        else {
            std::fill_n(aBlock, nBSize, 0.0F);
        }
    }
    return OK;
}

auto fqMix(float* aPBlock, const float* aSBlock, const float fLev) -> int32_t {
    if (fLev != 0.0F) {
        if (fLev != 1.0F) {
            for (size_t i = 0; i < nBSize; i++) {
                aPBlock[i] += aSBlock[i] * fLev;
            }
        }
        else {
            for (size_t i = 0; i < nBSize; i++) {
                aPBlock[i] += aSBlock[i];
            }
        }
    }
    return OK;
}

auto fqFilter(float* aBlock, uint32_t nMinFreq, uint32_t nMaxFreq) -> int32_t {
    nMinFreq = std::min(nMinFreq, nBSize);
    nMaxFreq = std::min(nMaxFreq, nBSize);
    std::fill_n(aBlock, nMinFreq, 0.0F);
    std::fill_n(&aBlock[nMaxFreq], nBSize - nMaxFreq, 0.0F);
    return OK;
}

auto fqAddNoise(float* aBlock, const float fLev, uint32_t nMinFreq, uint32_t nMaxFreq) -> int32_t {
    if (fLev != 0.0F) {
        nMinFreq = std::min(nMinFreq, nBSize);
        nMaxFreq = std::min(nMaxFreq, nBSize);
        for (size_t i = nMinFreq; i < nMaxFreq; i++) {
            aBlock[i] += gaussian() * fLev;
        }
    }
    return OK;
}

auto fqGenNoise(float* aBlock, const float fLev, uint32_t nMinFreq, uint32_t nMaxFreq) -> int32_t {
    std::fill_n(aBlock, nBSize, 0.0F);
    if (fLev != 0.0F) {
        nMinFreq = std::min(nMinFreq, nBSize);
        nMaxFreq = std::min(nMaxFreq, nBSize);
        for (size_t i = nMinFreq; i < nMaxFreq; i++) {
            aBlock[i] = gaussian() * fLev;
        }
    }
    return OK;
}

auto fqPitchShift(float* aBlock, const float fShift) -> int32_t {
    if (fShift != 1.0F) {
        const auto max_pos = nBSize - 2;
        std::array<float, 256> buf{};
        for (size_t i = 0; i < nBSize; i++) {
            if (const auto pos = static_cast<float>(i) * fShift; pos < static_cast<float>(nBSize)) {
                auto ipos = static_cast<uint32_t>(pos);
                ipos = std::min(ipos, max_pos);
                buf[i] = (aBlock[ipos + 1] - aBlock[ipos]) * (pos - static_cast<float>(ipos)) + aBlock[ipos];
            }
            else {
                buf[i] = 0.0F;
            }
        }
        std::copy_n(buf.begin(), nBSize, aBlock);
    }
    return OK;
}

auto fqPitchSlide(float* aBlock, int32_t nSlideFreq) -> int32_t {
    if (nSlideFreq != 0) {
        if (nSlideFreq > nBSize - 1) {
            nSlideFreq = static_cast<int32_t>(nBSize - 1);
        }
        std::array<float, 256> buf{};
        if (nSlideFreq > 0) {
            std::copy_n(&aBlock[nSlideFreq], nBSize - nSlideFreq, buf.begin());
        }
        if (nSlideFreq < 0) {
            std::copy_n(aBlock, nBSize + nSlideFreq, buf.begin() + -nSlideFreq);
        }
        std::copy_n(buf.begin(), nBSize, aBlock);
    }
    return OK;
}

auto fqStretch(float* aRBlock, float* aBlock, float* aBuf) -> int32_t {
    if (aBlock == nullptr) {
        std::fill_n(aBuf, nBSize, 0.0F);
    }
    else {
        for (size_t i = 0; i < nBSize; i++) {
            aRBlock[i] = (aBlock[i] + aBuf[i]) / 2.0F;
        }
        std::copy_n(aBlock, nBSize, aBuf);
    }
    return OK;
}

auto fqGate(float* aBlock, float fLev) -> int32_t {
    if (fLev != 0.0F) {
        fLev *= 32767.0F;
        for (size_t i = 0; i < nBSize; i++) {
            if (aBlock[i] > 0.0F && aBlock[i] < fLev) {
                aBlock[i] = 0.0F;
            }
            if (aBlock[i] < 0.0F && -fLev < aBlock[i]) {
                aBlock[i] = 0.0F;
            }
        }
    }
    return OK;
}

auto fqLimit(float* aBlock, float fLev) -> int32_t {
    if (fLev != 1.0F) {
        if (fLev == 0.0F) {
            std::fill_n(aBlock, nBSize, 0.0F);
        }
        else {
            const auto scale = 1.0F / fLev * 3.0F;
            fLev *= 32767.0F;
            for (size_t i = 0; i < nBSize; i++) {
                if (aBlock[i] > 0.0F && aBlock[i] > fLev) {
                    aBlock[i] = fLev;
                }
                if (aBlock[i] < 0.0F && aBlock[i] < -fLev) {
                    aBlock[i] = -fLev;
                }
            }
            if (scale > 1.0F) {
                fqScale(aBlock, scale);
            }
        }
    }
    return OK;
}

auto fqEqualize(float* aBlock, const float* aEq) -> int32_t {
    for (size_t i = 0; i < FQ_EQNUM - 1; i++) {
        const auto tab1 = FQ_EQTAB[i];
        auto tab2 = FQ_EQTAB[i + 1];
        if (tab1 > nBSize - 1) {
            break;
        }
        tab2 = std::min(tab2, nBSize);
        if (aEq[i] == 0.0F) {
            std::fill_n(&aBlock[tab1], tab2 - tab1, 0.0F);
        }
        else if (aEq[i] != 1.0F) {
            for (size_t j = tab1; j < tab2; j++) {
                aBlock[j] *= aEq[i];
            }
        }
    }
    return OK;
}

auto fqDelay(float* aBlock, const float fLev, uint32_t nDur, float* aBuf, const int32_t nSize,
             int32_t* nPos) -> int32_t {
    if (aBlock == nullptr) {
        if (nSize < 256) {
            return ERR;
        }
        std::fill_n(aBuf, nSize, 0.0F);
        *nPos = 0;
        return OK;
    }
    if (fLev == 0.0F) {
        return OK;
    }
    if (nDur == 0) {
        return OK;
    }
    if (*nPos >= nSize) {
        *nPos = 0;
    }
    const auto max_dur = static_cast<uint32_t>((static_cast<float>(nSize) / 256.0F) * FQ_SLICE);
    nDur = std::min(nDur, max_dur);
    auto pos = *nPos - (static_cast<int32_t>(static_cast<float>(nDur) / FQ_SLICE) << 8);
    if (pos < 0) {
        pos += nSize;
    }
    fqAdd(aBlock, &aBuf[pos]);
    std::copy_n(aBlock, nBSize, &aBuf[*nPos]);
    fqScale(&aBuf[*nPos], fLev);
    *nPos += 256;
    return OK;
}

auto fqAcModel(float* aBlock, const float* aEq, uint32_t nDur, float* aBuf, const int32_t nSize,
               int32_t* nPos) -> int32_t {
    if (aBlock == nullptr) {
        if (nSize < 256) {
            return ERR;
        }
        std::fill_n(aBuf, nSize, 0.0F);
        *nPos = 0;
        return OK;
    }
    if (nDur == 0) {
        return OK;
    }
    if (*nPos >= nSize) {
        *nPos = 0;
    }
    const auto max_dur = static_cast<uint32_t>(static_cast<float>(nSize) / 256.0F * FQ_SLICE);
    nDur = std::min(nDur, max_dur);
    auto pos = *nPos - (static_cast<int32_t>(static_cast<float>(nDur) / FQ_SLICE) << 8);
    if (pos < 0) {
        pos += nSize;
    }
    fqAdd(aBlock, &aBuf[pos]);
    std::copy_n(aBlock, nBSize, &aBuf[*nPos]);
    fqEqualize(&aBuf[*nPos], aEq);
    *nPos += 256;
    return OK;
}

auto fqInitE(EFFECT* rEffect) -> int32_t {
    rEffect->nClockCount = 0;
    rEffect->fScaleLev = 1.0F;
    rEffect->nFiltMinFreq = 0;
    rEffect->nFiltMaxFreq = 255;
    rEffect->fNoiseLev = 0.0F;
    rEffect->fToneLev = 0.0F;
    rEffect->nToneMinFreq = 0;
    rEffect->nToneMaxFreq = 0;
    rEffect->nToneCount = 0;
    rEffect->nToneMute = 0;
    rEffect->nToneDur = 0;
    rEffect->nBreakMaxRate = 0;
    rEffect->nBreakMaxDur = 0;
    rEffect->nQNoiseMaxRate = 0;
    rEffect->nQNoiseMaxDur = 0;
    rEffect->fPitchShift = 1.0F;
    rEffect->fLimitLev = 1.0F;
    rEffect->pEQLev = nullptr;
    return OK;
}

auto fqScaleE(float* aBlock, const EFFECT* rEffect) -> int32_t {
    if (rEffect->fScaleLev != 1.0F) {
        const auto filt_min_freq = std::min(rEffect->nFiltMinFreq, nBSize);
        const auto filt_max_freq = std::min(rEffect->nFiltMaxFreq, nBSize);
        if (rEffect->fScaleLev == 0.0F) {
            std::fill_n(&aBlock[filt_min_freq], filt_max_freq - filt_min_freq, 0.0F);
        }
        else {
            for (size_t i = filt_min_freq; i < filt_max_freq; i++) {
                if (aBlock[i] != 0.0F) {
                    aBlock[i] *= rEffect->fScaleLev;
                }
            }
        }
    }
    return OK;
}

auto fqMixE(float* aPBlock, const float* aSBlock, const EFFECT* rEffect) -> int32_t {
    if (rEffect->fScaleLev != 0.0F) {
        const auto filt_min_freq = std::min(rEffect->nFiltMinFreq, nBSize);
        const auto filt_max_freq = std::min(rEffect->nFiltMaxFreq, nBSize);
        if (rEffect->fScaleLev == 1.0F) {
            for (size_t i = filt_min_freq; i < filt_max_freq; i++) {
                if (aPBlock[i] == 0.0F) {
                    aPBlock[i] = aSBlock[i];
                }
                else if (aSBlock[i] != 0.0F) {
                    aPBlock[i] += aSBlock[i];
                }
            }
        }
        else {
            for (size_t i = filt_min_freq; i < filt_max_freq; i++) {
                if (aPBlock[i] == 0.0F) {
                    aPBlock[i] = aSBlock[i] * rEffect->fScaleLev;
                }
                else if (aSBlock[i] != 0.0F) {
                    aPBlock[i] += aSBlock[i] * rEffect->fScaleLev;
                }
            }
        }
    }
    return OK;
}

auto fqFilterE(float* aBlock, const EFFECT* rEffect) -> int32_t {
    const auto filt_min_freq = std::min(rEffect->nFiltMinFreq, nBSize);
    const auto filt_max_freq = std::min(rEffect->nFiltMaxFreq, nBSize);
    std::fill_n(aBlock, filt_min_freq, 0.0F);
    std::fill_n(&aBlock[filt_max_freq], nBSize - filt_max_freq, 0.0F);
    return OK;
}

auto fqAddNoiseE(float* aBlock, const EFFECT* rEffect) -> int32_t {
    if (rEffect->fNoiseLev != 0.0F) {
        const auto filt_min_freq = std::min(rEffect->nFiltMinFreq, nBSize);
        const auto filt_max_freq = std::min(rEffect->nFiltMaxFreq, nBSize);
        for (size_t i = filt_min_freq; i < filt_max_freq; i++) {
            aBlock[i] += gaussian() * rEffect->fNoiseLev;
        }
    }
    return OK;
}

auto fqAddToneE(float* aBlock, EFFECT* rEffect) -> int32_t { // not implemented
    if (rEffect->nToneDur != 0 && rEffect->nToneCount != 0 && rEffect->fToneLev != 0.0F) {
        const auto filt_min_freq = std::min(rEffect->nFiltMinFreq, nBSize);
        const auto filt_max_freq = std::min(rEffect->nFiltMaxFreq, nBSize);
        rEffect->nToneMinFreq = std::min(rEffect->nToneMinFreq, rEffect->nToneMaxFreq - 1);
        if (const auto remainder = rEffect->nClockCount % (rEffect->nToneDur + rEffect->nToneMute);
            remainder < rEffect->nToneDur) {
            const auto tone_count = static_cast<float>(rEffect->nToneCount);
            if (const auto freq = static_cast<uint32_t>(
                    static_cast<float>(rEffect->nToneMinFreq) + static_cast<float>(
                        static_cast<uint32_t>((static_cast<float>(remainder) / static_cast<float>(rEffect->nToneDur) *
                                               tone_count)) * (rEffect->nToneMaxFreq - rEffect->nToneMinFreq)) /
                    tone_count)
                ; freq >= filt_min_freq && freq < filt_max_freq) {
                aBlock[freq] = aBlock[freq] + rEffect->fToneLev;
            }
        }
    }
    return OK;
}

auto fqAddBreakE(float* aBlock, const EFFECT* rEffect) -> int32_t { // not implemented
    if (rEffect->nBreakMaxRate != 0 && rEffect->nBreakMaxDur != 0) {
        const auto filt_min_freq = std::min(rEffect->nFiltMinFreq, nBSize);
        const auto filt_max_freq = std::min(rEffect->nFiltMaxFreq, nBSize);
        if (rEffect->nClockCount % (pRandF(nRandP) % rEffect->nBreakMaxRate + 1) > pRandF(nRandP) % rEffect->
            nBreakMaxDur) {
            std::fill_n(&aBlock[filt_min_freq], filt_max_freq - filt_min_freq, 0.0F);
        }
    }
    return OK;
}

auto fqGenNoiseE(float* aBlock, const EFFECT* rEffect) -> int32_t {
    const auto filt_min_freq = std::min(rEffect->nFiltMinFreq, nBSize);
    const auto filt_max_freq = std::min(rEffect->nFiltMaxFreq, nBSize);
    std::fill_n(&aBlock[filt_min_freq], filt_max_freq - filt_min_freq, 0.0F);
    if (rEffect->fNoiseLev != 0.0F) {
        for (size_t i = filt_min_freq; i < filt_max_freq; i++) {
            aBlock[i] = gaussian() * rEffect->fNoiseLev;
        }
    }
    return OK;
}

auto fqGenBreakE(float* fLev, const EFFECT* rEffect) -> int32_t {
    if (rEffect->nBreakMaxRate != 0 && rEffect->nBreakMaxDur != 0 && rEffect->nClockCount % (
            pRandF(nRandP) % rEffect->nBreakMaxRate + 1) > pRandF(nRandP) % rEffect->nBreakMaxDur) {
        *fLev = 0.0F;
    }
    return OK;
}

auto fqGenQNoiseE(char* aQBlock, uint32_t nRate, const EFFECT* rEffect) -> int32_t { // not implemented
    nRate = std::min<uint32_t>(nRate >> 3, 260);
    if (rEffect->nQNoiseMaxRate != 0 && rEffect->nQNoiseMaxDur != 0 && rEffect->nClockCount % (
            pRandF(nRandP) % rEffect->nQNoiseMaxRate + 1) > pRandF(nRandP) % rEffect->nQNoiseMaxDur) {
        for (size_t i = 0; i < nRate; i++) {
            aQBlock[i] = static_cast<char>(pRandF(nRandP) % 254);
        }
    }
    return OK;
}

auto fqPitchShiftE(float* aBlock, const EFFECT* rEffect) -> int32_t {
    if (rEffect->fPitchShift != 0.0F) {
        auto filt_min_freq = rEffect->nFiltMinFreq;
        auto filt_max_freq = rEffect->nFiltMaxFreq;
        filt_min_freq = std::min(filt_min_freq, nBSize);
        filt_max_freq = std::min(filt_max_freq, nBSize);
        std::array<float, 256> buf{};
        const auto size = nBSize - 2;
        for (size_t i = filt_min_freq; i < filt_max_freq; i++) {
            if (const auto pitch = static_cast<float>(i) * rEffect->fPitchShift; static_cast<float>(nBSize) > pitch) {
                const auto pitch_floor = std::min(static_cast<uint32_t>(pitch), size);
                const auto block1 = aBlock[pitch_floor];
                const auto block2 = aBlock[pitch_floor + 1];
                buf[i] = (pitch - static_cast<float>(pitch_floor)) * (block2 - block1) + block1;
            }
            else {
                buf[i] = 0.0F;
            }
        }
        std::copy_n(&buf[filt_min_freq], filt_max_freq - filt_min_freq, &aBlock[filt_min_freq]);
    }
    return OK;
}

auto fqLimitE(float* aBlock, const EFFECT* rEffect) -> int32_t {
    if (auto limit_lev = rEffect->fLimitLev; limit_lev != 1.0F) {
        const auto filt_min_freq = std::min(rEffect->nFiltMinFreq, nBSize);
        const auto filt_max_freq = std::min(rEffect->nFiltMaxFreq, nBSize);
        if (limit_lev == 0.0F) {
            std::fill_n(&aBlock[filt_min_freq], filt_max_freq - filt_min_freq, 0.0F);
        }
        else {
            const auto limit = 1.0F / limit_lev * 3.0F;
            limit_lev *= 32767.0F;
            for (size_t i = filt_min_freq; i < filt_max_freq; i++) {
                if (aBlock[i] > 0.0F && aBlock[i] > limit_lev) {
                    aBlock[i] = limit_lev;
                }
                if (aBlock[i] < 0.0F && aBlock[i] < -limit_lev) {
                    aBlock[i] = -limit_lev;
                }
            }
            if (limit > 1.0F) {
                for (size_t i = filt_min_freq; i < filt_max_freq; i++) {
                    if (aBlock[i] != 0.0F) {
                        aBlock[i] *= limit;
                    }
                }
            }
        }
    }
    return OK;
}

auto fqEqualizeE(float* aBlock, const EFFECT* rEffect) -> int32_t {
    if (rEffect->pEQLev == nullptr) {
        return ERR;
    }
    const auto filt_min_freq = std::min(rEffect->nFiltMinFreq, nBSize);
    const auto filt_max_freq = std::min(rEffect->nFiltMaxFreq, nBSize);
    for (size_t i = 0; i < FQ_EQNUM - 1; i++) {
        auto tab1 = FQ_EQTAB[i];
        if (tab1 > nBSize - 1) {
            break;
        }
        auto tab2 = std::min(FQ_EQTAB[i + 1], nBSize);
        tab1 = std::max(tab1, filt_min_freq);
        if (tab2 >= filt_min_freq + 1) {
            if (tab1 > filt_max_freq - 1) {
                break;
            }
            tab2 = std::min(tab2, filt_max_freq);
            if (rEffect->pEQLev[i] == 0.0F) {
                std::fill_n(&aBlock[tab1], tab2 - tab1, 0.0F);
            }
            else if (rEffect->pEQLev[i] != 1.0F) {
                for (size_t j = tab1; j < tab2; j++) {
                    aBlock[j] *= rEffect->pEQLev[i];
                }
            }
        }
    }
    return OK;
}

auto rrand([[maybe_unused]] int32_t nDummy) -> int32_t {
    return dist(rnd);
}

auto gaussian() -> float {
    if (gotgauss) {
        gotgauss = false;
        return gauss;
    }
    float a;
    float b;
    float c;
    do {
        a = (static_cast<float>((pRandF(nRandP) & RAND_MAX)) / 32767.0F - 0.5F) * 2.0F;
        b = (static_cast<float>((pRandF(nRandP) & RAND_MAX)) / 32767.0F - 0.5F) * 2.0F;
        c = a * a + b * b;
    }
    while (c > 1.0F);
    const auto d = static_cast<float>(pSqrtF(std::logf(c) * -2.0F / c));
    gauss = a * d;
    gotgauss = true;
    return b * d;
}
