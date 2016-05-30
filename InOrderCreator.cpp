#include "InOrderCreator.h"

#include <cstdlib>
#include <time.h>
#include <string>

static const size_t IMAGE_DIM = 500;

void InOrderCreator::transform() {
    writeByte('P');
    writeByte('6');
    for (char c : string("\n500 500\n255\n")) {
        writeByte(c);
    }
    for (size_t i=0; i< IMAGE_DIM * IMAGE_DIM; i++) {
        writeByte((byte)(i%256));
        writeByte((byte)(i%256));
        writeByte((byte)(i%256));
    }
}
