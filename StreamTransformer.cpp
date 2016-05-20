#include "StreamTransformer.h"

using namespace std;

bool StreamTransformerImpl::readByte(byte &by) {

    if (readBits > 0) {
        throw "Can't intermingle reading bits/bytes."
              " Alternatively, call dropByte()";
    }
 
    int rd = input->get();

    if (rd == -1) {
        return false; // this is an eof
    }

    by = (byte) rd;

    if (input->fail()) {
        throw "Input Stream Error";
    }
    return true;
}

bool StreamTransformerImpl::readBit(bit &bi) {

    if (readBits == 0) {
        if (!readByte(readByteBuf)) return false; // end of file
        readBits = 8;
    }
 
    /* Read the bit. From MSB to LSB */
    bi = ((readByteBuf >> (--readBits)) % 2) == 1;
    return true;
}

void StreamTransformerImpl::dropByte() {
        readBits = 0;
}

void StreamTransformerImpl::writeByte(byte by) {

    if (writeBits > 0) {
        throw "Can't intermingle writing bits/bytes."
              " Alternatively, call flushByte()";
    }

    /* Write the byte */
    output->put((char)by);

    if (output->fail()) {
        throw "Output Stream Error";
    }
}

void StreamTransformerImpl::writeBit(bit bi) {

    writeByteBuf = (writeByteBuf << 1) + (bi ? 1 : 0);
    writeBits++;

    if (writeBits == 8) {
        writeBits = 0;
        writeByte(writeByteBuf);
    }
}

void StreamTransformerImpl::flushByte() {
    writeByteBuf <<= (8-writeBits);
    writeBits = 0; 
    writeByte(writeByteBuf);
}

void StreamTransformerImpl::pipe(istream &is, ostream &os) {
    input = &is;
    output = &os;
    transform();
}
