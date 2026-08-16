/* mbed Microcontroller Library
 * Copyright (c) 2026 Jamie Smith
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "mbed.h"

#include <random>
#include <cinttypes>

using utest::v1::Case;

// Note: Benchmark code adapted from https://github.com/tana/pico_float_bench

constexpr size_t BENCHMARK_SIZE = 4000;

// Working arrays for FP and DP versions of the benchmark
float workingArrFloats[BENCHMARK_SIZE];
float meanFloat;
double workingArrDoubles[BENCHMARK_SIZE];
double meanDouble;

// Constant random seed, so we should get the same results each time
constexpr uint32_t randomSeed = 1348720;

template<float *x>
void generate_gaussian_float()
{
    Timer genTimer;
    genTimer.start();

    std::minstd_rand randGen(randomSeed);
    std::uniform_real_distribution<float> realDistrib(0, 1);

    for (size_t i = 0; i < BENCHMARK_SIZE / 2; i++) {
        // Generate two gaussian random numbers
        // using Box-Muller transformation
        //  See: https://mathworld.wolfram.com/Box-MullerTransformation.html
        const float random1 = realDistrib(randGen);
        const float random2 = realDistrib(randGen);
        const float sqrt_part = sqrtf(-2 * logf(random1));
        const float angle_part = 2 * M_PI * random2;
        x[2 * i] = sqrt_part * cosf(angle_part);
        x[2 * i + 1] = sqrt_part * sinf(angle_part);
    }

    genTimer.stop();
    printf("Elapsed time: %" PRIu64 "us\n", genTimer.elapsed_time().count());
    printf("Speed: %.02f kelements/s\n", BENCHMARK_SIZE / std::chrono::duration_cast<std::chrono::duration<float>>(genTimer.elapsed_time()).count() / 1e3);
}

template<double *x>
void generate_gaussian_double()
{
    Timer genTimer;
    genTimer.start();

    std::minstd_rand randGen(randomSeed);
    std::uniform_real_distribution<double> realDistrib(0, 1);

    for (size_t i = 0; i < BENCHMARK_SIZE / 2; i++) {
        // Generate two gaussian random numbers
        // using Box-Muller transformation
        //  See: https://mathworld.wolfram.com/Box-MullerTransformation.html
        const double random1 = realDistrib(randGen);
        const double random2 = realDistrib(randGen);
        const double sqrt_part = sqrt(-2 * log(random1));
        const double angle_part = 2 * M_PI * random2;
        x[2 * i] = sqrt_part * cos(angle_part);
        x[2 * i + 1] = sqrt_part * sin(angle_part);
    }

    genTimer.stop();
    printf("Elapsed time: %" PRIu64 "us\n", genTimer.elapsed_time().count());
    printf("Speed: %.02f kelements/s\n", BENCHMARK_SIZE / std::chrono::duration_cast<std::chrono::duration<float>>(genTimer.elapsed_time()).count() / 1e3);
}

template<typename FloatT, FloatT const * x, FloatT * meanPtr>
void calc_mean()
{
    Timer genTimer;
    genTimer.start();

    FloatT mean = 0;
    for (size_t i = 0; i < BENCHMARK_SIZE; i++) {
        mean = (i * mean + x[i]) / (i + 1);
    }

    genTimer.stop();

    printf("Mean: %f\n", mean);
    *meanPtr = mean;

    // Mean should be near zero
    TEST_ASSERT_FLOAT_WITHIN(.01, 0, mean);

    printf("Elapsed time: %" PRIu64 "us\n", genTimer.elapsed_time().count());
    printf("Speed: %.02f kelements/s\n", BENCHMARK_SIZE / std::chrono::duration_cast<std::chrono::duration<float>>(genTimer.elapsed_time()).count() / 1e3);
}

template<typename FloatT, FloatT const * x, FloatT const * meanPtr>
void calc_variance()
{
    Timer genTimer;
    genTimer.start();

    FloatT variance = 0;
    for (size_t i = 0; i < BENCHMARK_SIZE; i++) {
        FloatT x_minus_mean = x[i] - *meanPtr;
        variance = (i * variance + x_minus_mean * x_minus_mean) / (i + 1);
    }

    genTimer.stop();

    printf("Variance: %f\n", variance);

    // Variance should be near one
    TEST_ASSERT_FLOAT_WITHIN(.05, 1, variance);

    printf("Elapsed time: %" PRIu64 "us\n", genTimer.elapsed_time().count());
    printf("Speed: %.02f kelements/s\n", BENCHMARK_SIZE / std::chrono::duration_cast<std::chrono::duration<float>>(genTimer.elapsed_time()).count() / 1e3);
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(120, "default_auto");
    return utest::v1::verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Generate gaussian distribution [float]", generate_gaussian_float<workingArrFloats>),
    Case("Calc mean [float]", calc_mean<float, workingArrFloats, &meanFloat>),
    Case("Calc variance [float]", calc_variance<float, workingArrFloats, &meanFloat>),
    Case("Generate gaussian distribution [doubles]", generate_gaussian_double<workingArrDoubles>),
    Case("Calc mean [double]", calc_mean<double, workingArrDoubles, &meanDouble>),
    Case("Calc variance [double]", calc_variance<double, workingArrDoubles, &meanDouble>),
};

utest::v1::Specification specification(test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}
