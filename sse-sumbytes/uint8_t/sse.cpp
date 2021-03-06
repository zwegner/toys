#include "sse.h"

#include <immintrin.h>

uint32_t sse_sumbytes(uint8_t* array, size_t size) {

    __m128i accumulator = _mm_setzero_si128();

    for (size_t i=0; i < size; i += 16) {
        const __m128i v = _mm_loadu_si128((__m128i*)(array + i));
        const __m128i v0_3   = v;
        const __m128i v4_7   = _mm_bsrli_si128(v, 1*4);
        const __m128i v8_11  = _mm_bsrli_si128(v, 2*4);
        const __m128i v12_15 = _mm_bsrli_si128(v, 3*4);
        
        const __m128i t0 = _mm_cvtepu8_epi32(v0_3);
        const __m128i t1 = _mm_cvtepu8_epi32(v4_7);
        const __m128i t2 = _mm_cvtepu8_epi32(v8_11);
        const __m128i t3 = _mm_cvtepu8_epi32(v12_15);

        const __m128i t01 = _mm_add_epi32(t0, t1);
        const __m128i t23 = _mm_add_epi32(t2, t3);

        accumulator = _mm_add_epi32(accumulator, t01);
        accumulator = _mm_add_epi32(accumulator, t23);
    }

    return uint32_t(_mm_extract_epi32(accumulator, 0)) +
           uint32_t(_mm_extract_epi32(accumulator, 1)) +
           uint32_t(_mm_extract_epi32(accumulator, 2)) +
           uint32_t(_mm_extract_epi32(accumulator, 3));
}


uint32_t sse_sumbytes_variant2(uint8_t* array, size_t size) {

    const __m128i lobyte_mask = _mm_set1_epi32(0x000000ff);

    __m128i accumulator = _mm_setzero_si128();

    for (size_t i=0; i < size; i += 16) {
        const __m128i v = _mm_loadu_si128((__m128i*)(array + i));
        const __m128i v0 = v;
        const __m128i v1 = _mm_srli_epi32(v, 1*8);
        const __m128i v2 = _mm_srli_epi32(v, 2*8);
        const __m128i v3 = _mm_srli_epi32(v, 3*8);
        
        const __m128i t0 = _mm_and_si128(lobyte_mask, v0);
        const __m128i t1 = _mm_and_si128(lobyte_mask, v1);
        const __m128i t2 = _mm_and_si128(lobyte_mask, v2);
        const __m128i t3 = v3;

        accumulator = _mm_add_epi32(accumulator, t0);
        accumulator = _mm_add_epi32(accumulator, t1);
        accumulator = _mm_add_epi32(accumulator, t2);
        accumulator = _mm_add_epi32(accumulator, t3);
    }

    return uint32_t(_mm_extract_epi32(accumulator, 0)) +
           uint32_t(_mm_extract_epi32(accumulator, 1)) +
           uint32_t(_mm_extract_epi32(accumulator, 2)) +
           uint32_t(_mm_extract_epi32(accumulator, 3));
}
