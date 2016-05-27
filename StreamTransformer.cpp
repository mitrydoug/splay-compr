#include "StreamTransformer.h"

using namespace std;

ostream& operator<<(ostream &out, TransformDigest &dg) {
    out << "Bytes Read: " << dg.bytesIn << endl;
    out << "Bytes Written: " << dg.bytesOut << endl;
    return out;
}

ostream& operator<<(ostream &out, TransformDigest &&dg) {
    out << "Bytes Read: " << dg.bytesIn << endl;
    out << "Bytes Written: " << dg.bytesOut << endl;
    return out;
}

TransformDigest StreamTransformer::getDigest() { return digest; }

bool StreamTransformer::readByte(byte &by) {

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

    // increment the number of bytes read.
    digest.bytesIn++;

    return true;
}

bool StreamTransformer::readBit(bit &bi) {

    if (readBits == 0) {
        if (!readByte(readByteBuf)) return false; // end of file
        readBits = 8;
    }
 
    /* Read the bit. From MSB to LSB */
    bi = ((readByteBuf >> (--readBits)) % 2) == 1;
    return true;
}

void StreamTransformer::dropByte() {
        readBits = 0;
}

void StreamTransformer::writeByte(byte by) {

    if (writeBits > 0) {
        throw "Can't intermingle writing bits/bytes."
              " Alternatively, call flushByte()";
    }

    /* Write the byte */
    output->put((char)by);

    if (output->fail()) {
        throw "Output Stream Error";
    }

    digest.bytesOut++;
}

void StreamTransformer::writeBit(bit bi) {

    writeByteBuf = (writeByteBuf << 1) + (bi ? 1 : 0);
    writeBits++;

    if (writeBits == 8) {
        writeBits = 0;
        writeByte(writeByteBuf);
    }
}

void StreamTransformer::flushByte(bit bt) {
    while (writeBits != 0) {
        writeBit(bt);
    }
}

void StreamTransformer::exec(istream *is, ostream *os) {
   
    /* Construct a new digest, copy is over */
    digest = TransformDigest();

    input = is;
    output = os;
    transform();
    input = nullptr;
    output = nullptr;
}
