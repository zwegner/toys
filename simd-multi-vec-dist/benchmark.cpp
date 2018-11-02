#include "algorithm.h"
#include "all.h"

#include <cstdio>
#include "benchmark.h"

#include <cstdlib>


class Test {

    static const size_t size = 1024 * 4;
    static const size_t n = 16;

    const size_t iterations = 10;

    float* input[n];
    float  output[n];
    float* const_vector;

public:
    Test() {
        for (size_t i=0; i < n; i++) {
            input[i] = new float[size];
            initialize_vector(input[i]);
        }

        const_vector = new float[size];
        initialize_vector(const_vector);
    }

    ~Test() {
        for (size_t i=0; i < n; i++) {
            delete[] input[i];
        }

        delete[] const_vector;
    }

    void run() {

        test_scalar("scalar", scalar_dist_vectors);
        test_scalar("SSE",    sse_dist_vector);

        test("SSE (custom)",  sse_dist_vector_many);
    }

private:
    void initialize_vector(float* v) {
        for (size_t i=0; i < size; i++) {
            v[i] = (rand() % 1000)/1000.0 - 0.5;
        }
    }

    template <typename FUNCTION>
    void test_scalar(const char* name, FUNCTION function) {
        
        auto fn = [this, function]() {
            dist_vector_many(function, size, const_vector, n, input, output);
        };

        BEST_TIME(/**/, fn(), name, iterations, size);
    }

    template <typename FUNCTION>
    void test(const char* name, FUNCTION dist_vector_many) {
        
        auto fn = [this, dist_vector_many]() {
            dist_vector_many(size, const_vector, n, input, output);
        };

        BEST_TIME(/**/, fn(), name, iterations, size);
    }
};

int main() {
    
    Test test;

    test.run();

    return EXIT_SUCCESS;
}
