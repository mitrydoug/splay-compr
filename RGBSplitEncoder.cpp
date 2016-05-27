
#include "RGBSplitEncoder.h"
#include <iostream>

using namespace std;

void RGBSplitEncoder::encode() {

    /* Read the PPM header */
    readHeader();

    byte b;
    size_t count=0;
    for ( ; readByte(b); count++) {
        switch (count % 3) {
            // Red channel is sent along right away
            case 0: writeByte(b);          break;
            case 1: greenChn.push_back(b); break;
            case 2: blueChn.push_back(b);  break;
        }
    }
    if (count != 3 * imageWidth * imageHeight) {
        cout << endl << count << endl;
        throw "Image dimensions did not match number of bytes read.";
    }
    for (byte by : greenChn) writeByte(by);
    for (byte by : blueChn)  writeByte(by);
}

void RGBSplitEncoder::readChannel(vector<byte> &chn) {
    byte b;
    bool more;
    for (size_t i=0; i < imageWidth * imageHeight; i++) {
        more = readByte(b);
        if (!more) {
            cout << i << endl;
            throw "Image dimensions does not match number of available"
                  " bytes.";
        }
        chn.push_back(b);
    }
}

void RGBSplitEncoder::decode() {

    /* Read the PPM header */
    readHeader();

    readChannel(redChn);
    cout << "red" << endl;
    readChannel(greenChn);
    cout << "green" << endl;
    readChannel(blueChn);
    cout << "blue" << endl;
    byte dummy;
    if (readByte(dummy)) {
        throw "More available bytes than expected.";
    }
    for (size_t count=0; count < imageWidth * imageHeight; count++) {
        writeByte(redChn[count]);
        writeByte(greenChn[count]);
        writeByte(blueChn[count]);
    }
}
