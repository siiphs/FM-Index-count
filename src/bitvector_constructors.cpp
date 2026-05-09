#include "../include/bitvector-lib/bitvector.hpp"

namespace CDS_UDEC {
    BitVector::BitVector() {}

    BitVector::BitVector(uint32_t sz, uint8_t df_value) {
        size = sz;
        uint64_t default_value = df_value == 0 ? ZERO : ONE;
        uint32_t buckets_needed = size >> 6; //size / (2^i)
        uint64_t module_value = size & 63; //size % 64
        if(module_value != ZERO)
            buckets_needed++;
        bitarray.assign(buckets_needed, default_value);
    }
}