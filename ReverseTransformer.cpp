#include "ReverseTransformer.h"

void ReverseTransformer::transform() {
    byte curr;
    while (readByte(curr)) {
       streamData.push(curr); 
    }
    while(!streamData.empty()) {
        writeByte(streamData.top());
        streamData.pop();
    }
}
