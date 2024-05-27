#include "dct.h"
#include "mixfft.h"
#include <array>
#include <cmath>
#include <numbers>


auto idct(const float* f, float* g, const float* c, const uint32_t n) -> int32_t {
    const auto size_half = n >> 1;
    const auto size_quarter = n >> 2;
    const auto size_3quarter = size_quarter * 3;
    const float factor = 8.0F / std::sqrtf(static_cast<float>(n));
    std::array<float, HALFSIZE> fft_re{};
    std::array<float, HALFSIZE> fft_im{};
    std::array<float, HALFSIZE> fft_out_re{};
    std::array<float, HALFSIZE> fft_out_im{};
    std::array<float, DOUBSIZE> samples_tmp{};

    for (size_t i = 0; i < size_quarter; i++) {
        const auto i2 = i << 1;
        const auto coef1 = f[i2] * 0.5F;
        const auto coef2 = f[size_half - 1 - i2] * 0.5F;
        fft_re[i] = coef1 * c[i + size_quarter] + coef2 * c[i];
        fft_im[i] = -coef1 * c[i] + coef2 * c[i + size_quarter];
    }
    fft(static_cast<int32_t>(size_quarter), fft_re.data(), fft_im.data(), fft_out_re.data(), fft_out_im.data());
    for (size_t i = 0; i < size_quarter; i++) {
        const auto fft_temp = fft_out_re[i];
        fft_out_re[i] = (fft_out_re[i] * c[i + size_quarter] + fft_out_im[i] * c[i]) * factor;
        fft_out_im[i] = (-fft_temp * c[i] + fft_out_im[i] * c[i + size_quarter]) * factor;
    }
    for (size_t i = 0; i < size_quarter; i++) {
        const auto i2 = i << 1;
        samples_tmp[i2] = fft_out_re[i];
        samples_tmp[size_half + i2] = fft_out_im[i];
    }
    for (size_t i = 1; i < n; i += 2) {
        samples_tmp[i] = -samples_tmp[n - i - 1];
    }
    size_t i = 0;
    while (i < size_3quarter) {
        g[i] = samples_tmp[i + size_quarter];
        i++;
    }
    while (i < n) {
        g[i] = -samples_tmp[i - size_3quarter];
        i++;
    }
    return OK;
}

auto Initdct(float* c, const uint32_t n) -> int32_t {
    const auto qlen = n >> 2;
    for (size_t i = 0; i < qlen; i++) {
        const auto f = (static_cast<float>(i) + 0.125F) * (2.0F * std::numbers::pi_v<float>) * (
                           1.0F / static_cast<float>(n));
        c[i] = std::sinf(f);
        c[i + qlen] = std::cosf(f);
    }
    return OK;
}
