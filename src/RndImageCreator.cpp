#include "RndImageCreator.h"

#include <cstdlib>
#include <time.h>
#include <string>

static const size_t IMAGE_DIM = 500;

void RndImageCreator::transform() {
    writeByte('P');
    writeByte('6');
    for (char c : string("\n500 500\n255\n")) {
        writeByte(c);
    }
    srand(time(NULL));
    for (size_t r=0; r<IMAGE_DIM; r++) {
        for (size_t c=0; c<IMAGE_DIM; c++) {
            size_t lum = rand() % 256;
            writeByte((byte)lum);
            lum = rand() % 256;
            writeByte((byte)lum);
            lum = rand() % 256;
            writeByte((byte)lum);
        }
    }
}
