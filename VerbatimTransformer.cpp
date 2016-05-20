#include "VerbatimTransformer.h"

void VerbatimTransformer::transform() {
    byte curr;
    while (readByte(curr)) {
        writeByte(curr);
    }
}
