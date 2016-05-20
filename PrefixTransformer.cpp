#include "PrefixTransformer.h"

void PrefixTransformer::transform() {
    for(char ch : prefix) {
        writeByte(ch);
    }
    byte curr;
    while(readByte(curr)) {
        writeByte(curr);
    }
}
