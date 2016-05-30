#include "ShannonEntropyCalc.h"

#include <unordered_map>
#include <cmath>
#include <ostream>

static const size_t IMAGE_DIM = 500;

void ShannonEntropyCalc::encode() {
    readHeader();
    unordered_map<byte, size_t> freq_table;
    byte b;
    while (readByte(b)) {
        freq_table[b]++;
    }
    double entropy = 0;
    for (const auto &it : freq_table) {
        double p = it.second / (double)(imageWidth * imageHeight * 3);
        entropy += -p * log(p) / log(2);
    }
    cout << "H: " << entropy << endl;
}

void ShannonEntropyCalc::decode() {
    /* No-Op */
}
