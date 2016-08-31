#include "PrefixTransformer.h"


void PrefixTransformer::transform() {
    // write the prefix
    for(char ch : prefix) {
        writeByte(ch);
    }
    //copy the stream
    byte curr;
    while(readByte(curr)) {
        writeByte(curr);
    }
}
