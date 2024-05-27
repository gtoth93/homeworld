#include "fquant.h"
#include <array>

std::array<uint16_t, FQ_CBNUM> FQ_CBDATA{
    0, 1, 2, 3, 4, 5, 6, 7, 9, 11, 13, 15, 17, 20, 23, 27, 31, 37, 43, 51, 62, 74, 89, 110, 139, 180, 256
};

std::array<float, FQ_EXP> FQ_SDATA{};

auto fqInitDequant() -> int32_t {
    FQ_SDATA[0] = 10.0F;
    for (size_t i = 1; i < FQ_EXP; i++) {
        FQ_SDATA[i] = FQ_SDATA[i - 1] * FQ_SCALE;
    }
    for (size_t i = 0; i < FQ_EXP; i++) {
        FQ_SDATA[i] *= 1.0F / static_cast<float>((1 << i + 1) - 1);
    }
    return OK;
}

auto fqDequantBlock(const char* aQBlock, float* aFPBlock, float* aFSBlock, uint8_t* aEBlock, const uint32_t nLen,
                    const uint32_t nRate, const uint32_t nSize) -> int32_t {
    std::fill_n(aFPBlock, 256, 0.0F);
    std::fill_n(aFSBlock, 256, 0.0F);

    const auto nLen8 = nLen << 3;
    uint32_t pos = 0;
    const auto ua = fqUnpack(2, pos, aQBlock);
    pos += 2;
    const auto ub = fqUnpack(3, pos, aQBlock);
    pos += 3;
    const auto uc = fqUnpack(2, pos, aQBlock);
    pos += 2;
    const auto ud = fqUnpack(4, pos, aQBlock);
    pos += 4;
    if ((ua & 1) == 1) {
        std::fill_n(aEBlock, 256, 0);
    }
    if (ub > 0 && uc > 0) {
        uint8_t counter = 0;
        for (size_t i = 0; i < FQ_CBNUM - 1 && pos < nRate && pos < nLen8; i++) {
            const uint8_t u = fqUnpack(ub, pos, aQBlock);
            pos += ub;
            if (i > 0 && u == 0) {
                break;
            }
            counter += u;
            const uint8_t u2 = fqUnpack(uc, pos, aQBlock);
            pos += uc;
            for (size_t j = FQ_CBDATA[counter]; j < FQ_CBDATA[counter + 1]; j++) {
                aEBlock[j] = u2;
            }
        }
    }
    if (nSize != 0 && ud != 0) {
        uint8_t counter = 0;
        for (size_t i = 0; i < 256 && pos < nRate && pos < nLen8; i++) {
            const uint8_t u = fqUnpack(ud, pos, aQBlock);
            pos += ud;
            if (i > 0 && u == 0) {
                break;
            }
            counter += u;
            const auto block = aEBlock[counter];
            const auto len = block + 2;
            const auto s = static_cast<float>(fqSUnpack(len, pos, aQBlock));
            pos += len;
            if (s != 0.0F && counter < nSize && block < FQ_EXP) {
                aFPBlock[counter] = s * FQ_SDATA[block];
            }
        }
        if ((ua & 2) == 2) {
            std::copy_n(aFPBlock, 256, aFSBlock);
        }
        else {
            counter = 0;
            for (size_t i = 0; i < 256 && pos < nRate && pos < nLen8; i++) {
                const uint8_t u = fqUnpack(ud, pos, aQBlock);
                pos += ud;
                if (i > 0 && u == 0) {
                    break;
                }
                counter += u;
                const auto block = aEBlock[counter];
                const auto len = block + 2;
                const auto s = static_cast<float>(fqSUnpack(len, pos, aQBlock));
                pos += len;
                if (s != 0.0 && counter < nSize && block < FQ_EXP) {
                    aFSBlock[counter] = s * FQ_SDATA[block];
                }
            }
        }
    }
    return OK;
}

auto fqSUnpack(const uint32_t nLen, const uint32_t nPos, const char* aBlock) -> int32_t {
    const auto nPos_shr = nPos >> 3;
    const auto shift = nPos - (nPos_shr << 3);
    const auto unpacked = (*reinterpret_cast<const uint32_t *>(&aBlock[nPos_shr]) & (1 << nLen) - 1 << shift) >> shift;
    if (unpacked >> nLen - 1 == 1) {
        const auto shift2 = 32 - nLen + 1;
        return -static_cast<int32_t>(unpacked << shift2 >> shift2);
    }
    return static_cast<int32_t>(unpacked);
}

auto fqUnpack(const uint32_t nLen, const uint32_t nPos, const char* aBlock) -> uint32_t {
    const auto npos_shr = nPos >> 3;
    const auto shift = nPos - (npos_shr << 3);
    return (*reinterpret_cast<const uint32_t *>(&aBlock[npos_shr]) & (1 << nLen) - 1 << shift) >> shift;
}
