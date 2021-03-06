
#include <cctype>
#include <ostream>
#include "PPMImageEncoder.h"

using namespace std;

const string PPMImageEncoder::magic = "P6";

size_t PPMImageEncoder::readHeaderInt() { 

    byte b;
    bool more;
    bool comment=false;
    while ((more = readByte(b))) {
        writeByte(b);
        if (b == '#') comment = true;
        if (b == '\n') comment = false;
        if (!comment && isdigit(b)) break;
    }

    if (!more) {
        throw "Could not read integer.";
    }
    
    size_t res = 0;
    do {
        res *= 10;
        res += b - '0';
        readByte(b);
        writeByte(b);
    } while (isdigit(b));

    return res;
}

void PPMImageEncoder::readHeader() {
    byte curr;
    /* Read in the PPM magic number */
    string readmagic;
    readByte(curr);
    readmagic += curr;
    readByte(curr);
    readmagic += curr;
    if (magic != readmagic) {
        throw "The PPM magic number is not correct";
    }
    writeByte('P'); /* P6: The magic prefix in PPM headers */
    writeByte('6');

    imageWidth = readHeaderInt();
    imageHeight = readHeaderInt();
    imageMaxLum = readHeaderInt();

    size_t temp = imageMaxLum;
    while(temp > 0) {
        bytesPerVal++;
        temp <<= 8;
    }
}
