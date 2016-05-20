#include "AlphaRunLengthEncoder.h"

#include <cctype>
#include <sstream>

void AlphaRunLengthEncoder::encode() {
    byte by;
    byte running = '0';
    size_t runlength = 0;
    while (readByte(by)) {
        if (isspace(by) || isalpha(by)) {
            if (running == by) {
                runlength++;
            } else {
                if (running != '0') {
                    ostringstream convertInt;
                    convertInt << runlength;
                    for(char ch : convertInt.str()) {
                        writeByte(ch);
                    }
                    writeByte(running);
                }
                running = by;
                runlength = 1;
            }
        } else {
            throw "This encoder only handles aplphabet characters and spaces";
        }
    }
}

void AlphaRunLengthEncoder::decode() {
    byte by;
    size_t runlength = 0;
    while(readByte(by)) {
        if (isspace(by) || isalpha(by)) {
            if (runlength == 0) {
                throw "Alpha/spaces must be preceeded by a number";
            } else {
                for (size_t i=0; i<runlength; i++) {
                    writeByte(by);
                }
                runlength = 0;
            }
        } else if (isdigit(by)) {
            runlength *= 10;
            runlength += (by - '0');
        }
    }
}
