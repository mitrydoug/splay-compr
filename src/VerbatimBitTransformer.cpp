#include "VerbatimBitTransformer.h"

void VerbatimBitTransformer::transform() {
    bit curr;
    while (readBit(curr)) {
        writeBit(curr);
    }
}
