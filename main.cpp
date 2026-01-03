#include <iostream>

#define ACCELERATE_NEW_LAPACK
#include <map>
#include <Accelerate/Accelerate.h>

#include "Benchmark.h"

#include <algorithm>

#include <arm_neon.h>
#include <stdint.h>

void custom_sscal(const __LAPACK_int N, const float ALPHA, float * _Nullable X, const __LAPACK_int INCX) {
}

void exploring_cblas();


#include "MyAppDelegate.h"

int main()
{
    NS::AutoreleasePool* pAutoreleasePool = NS::AutoreleasePool::alloc()->init();

    MyAppDelegate del;

    NS::Application* pSharedApplication = NS::Application::sharedApplication();
    pSharedApplication->setDelegate( &del );
    pSharedApplication->run();

    pAutoreleasePool->release();

    return 0;
}

void run() {
}

void exploring_cblas() {
    float16x8_t test_float = {32.0, 32.0, 32.0, 32.0, 32.0, 32.0, 32.0, 32.0};
    float16x8_t res = vmulq_n_f16(test_float, 2);

    for (int i = 0; i < 8; i++) {
        std::cout << res[i] << std::endl;
    }

    size_t size = 100000000;

    std::vector<float> vec1(size);
    for (int i = 0; i < vec1.size(); i++) {
        vec1[i] = i;
    }

    std::vector<float> vec2(size);
    for (int i = 0; i < vec2.size(); i++) {
        vec2[i] = i;
    }

    std::vector<float> vec3(size);
    for (int i = 0; i < vec3.size(); i++) {
        vec3[i] = i;
    }

    Benchmark::benchmark("for_each", [&] () {
        std::ranges::for_each(vec1, [] (float& x) { x *= 2; });
    });

    Benchmark::benchmark("cblas", [&] () {
        cblas_sscal(vec2.size(), 2, vec2.begin().base(), 1);
    });

    for (int i = 0; i < vec1.size(); i++) {
        if (vec1[i] != vec2[i]) {
            std::cout << "something went wrong..." << std::endl;

        }
    }

    std::cout << "test finished fine" << std::endl;
}
