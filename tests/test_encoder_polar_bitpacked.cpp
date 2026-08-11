#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cassert>

#include "Module/Encoder/Polar/Encoder_polar.hpp"
#include "Module/Encoder/Polar/Encoder_polar_sys.hpp"
#include "Module/Encoder/Polar/Encoder_polar_bitpacked.hpp"
#include "Module/Encoder/Polar/Encoder_polar_bitpacked_sys.hpp"

using namespace aff3ct::module;

template<typename B = int>
bool test_encoder_N(int N, int K, int n_frames = 100)
{
    std::cout << "Testing N = " << N << ", K = " << K << "... " << std::flush;

    // 1. Generate frozen bits vector
    std::vector<bool> frozen_bits(N, false);
    // Freeze N - K positions randomly
    std::mt19937 rng(42);
    std::vector<int> indices(N);
    for (int i = 0; i < N; ++i) indices[i] = i;
    std::shuffle(indices.begin(), indices.end(), rng);
    for (int i = 0; i < N - K; ++i) frozen_bits[indices[i]] = true;

    // 2. Instantiate non-systematic encoders
    Encoder_polar<B> enc_ref(K, N, frozen_bits);
    Encoder_polar_bitpacked<B> enc_fast(K, N, frozen_bits);

    // 3. Instantiate systematic encoders
    Encoder_polar_sys<B> enc_sys_ref(K, N, frozen_bits);
    Encoder_polar_bitpacked_sys<B> enc_sys_fast(K, N, frozen_bits);

    // 4. Generate random input frames
    std::uniform_int_distribution<int> dist(0, 1);
    std::vector<B> U_K(K);
    std::vector<B> X_N_ref(N);
    std::vector<B> X_N_fast(N);
    std::vector<B> X_N_sys_ref(N);
    std::vector<B> X_N_sys_fast(N);

    for (int f = 0; f < n_frames; ++f)
    {
        for (int i = 0; i < K; ++i) U_K[i] = dist(rng);

        // Non-systematic encoding
        enc_ref.encode(U_K.data(), X_N_ref.data());
        enc_fast.encode(U_K.data(), X_N_fast.data());

        for (int i = 0; i < N; ++i)
        {
            if (X_N_ref[i] != X_N_fast[i])
            {
                std::cerr << "\n[MISMATCH] Non-systematic N=" << N << ", frame " << f
                          << " at bit " << i << ": ref=" << X_N_ref[i] << ", fast=" << X_N_fast[i] << std::endl;
                return false;
            }
        }

        // Systematic encoding
        enc_sys_ref.encode(U_K.data(), X_N_sys_ref.data());
        enc_sys_fast.encode(U_K.data(), X_N_sys_fast.data());

        for (int i = 0; i < N; ++i)
        {
            if (X_N_sys_ref[i] != X_N_sys_fast[i])
            {
                std::cerr << "\n[MISMATCH] Systematic N=" << N << ", frame " << f
                          << " at bit " << i << ": ref=" << X_N_sys_ref[i] << ", fast=" << X_N_sys_fast[i] << std::endl;
                return false;
            }
        }
    }

    // Performance comparison for 10000 frames
    const int perf_frames = 10000;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int f = 0; f < perf_frames; ++f)
        enc_sys_ref.encode(U_K.data(), X_N_sys_ref.data());
    auto t1 = std::chrono::high_resolution_clock::now();

    auto t2 = std::chrono::high_resolution_clock::now();
    for (int f = 0; f < perf_frames; ++f)
        enc_sys_fast.encode(U_K.data(), X_N_sys_fast.data());
    auto t3 = std::chrono::high_resolution_clock::now();

    double dur_ref = std::chrono::duration<double, std::nano>(t1 - t0).count() / perf_frames;
    double dur_fast = std::chrono::duration<double, std::nano>(t3 - t2).count() / perf_frames;

    std::cout << "PASSED! [Speedup sys: " << (dur_ref / dur_fast) << "x | Ref: " << dur_ref << " ns/frame | Fast: " << dur_fast << " ns/frame]" << std::endl;
    return true;
}

int main()
{
    std::cout << "=== Testing Encoder_polar_bitpacked vs Encoder_polar ===" << std::endl;

    std::vector<int> Ns = {64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384};
    bool all_passed = true;

    for (int N : Ns)
    {
        int K = N / 2;
        if (!test_encoder_N<int>(N, K))
            all_passed = false;
    }

    if (all_passed)
    {
        std::cout << "\nSUCCESS: All bitpacked polar encoder tests passed!" << std::endl;
        return 0;
    }
    else
    {
        std::cerr << "\nFAILURE: Some tests failed!" << std::endl;
        return 1;
    }
}
