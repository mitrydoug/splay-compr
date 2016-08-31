
#include "PPMDelegateEncoder.h"
#include <iostream>

using namespace std;

void PPMDelegateEncoder::encode() {

    /* Read the PPM header */
    readHeader();
    delegate->exec(input, output);
}

void PPMDelegateEncoder::decode() {

    /* Read the PPM header */
    readHeader();
    delegate->exec(input, output);
}


