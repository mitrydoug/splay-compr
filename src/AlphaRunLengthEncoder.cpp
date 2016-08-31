#include "AlphaRunLengthEncoder.h"

#include <cctype>
#include <sstream>

void AlphaRunLengthEncoder::encode() {
    byte by; // byte used for reading
    byte running = '0'; // tracks the last seen byte
    size_t runlength = 0; // run-length of last-seen byte.
    while (readByte(by)) {
        if (isspace(by) || isalpha(by)) {
            if (running == by) {
                runlength++; // seen the last character once again.
            } else {
                if (running != '0') {
                    // convert integer to string and output characters to
                    // stream
                    ostringstream convertInt;
                    convertInt << runlength;
                    for(char ch : convertInt.str()) {
                        writeByte(ch);
                    }
                    writeByte(running);
                }
                // record the next running byte
                running = by;
                runlength = 1;
            }
        } else {
            // We don't want to worry about encoding strings with numbers
            throw "This encoder only handles aplphabet characters and spaces";
        }
    }
    // Output the run-length of the last seen character
    if (runlength > 0) {
        ostringstream convertInt;
        convertInt << runlength;
        for(char ch : convertInt.str()) {
            writeByte(ch);
        }
        writeByte(running);
    }
}

void AlphaRunLengthEncoder::decode() {
    byte by; // used for reading in bytes
    size_t runlength = 0; // hold the runlengths that are recorded
    while(readByte(by)) {
        if (isspace(by) || isalpha(by)) {
            if (runlength == 0) {
                throw "Alpha/spaces must be preceeded by a number";
            } else {
                // write run-length copies of the running char to stream
                for (size_t i=0; i<runlength; i++) {
                    writeByte(by);
                }
                runlength = 0; // reset
            }
        } else if (isdigit(by)) {
            // update runlength by another decimal digit
            runlength *= 10;
            runlength += (by - '0');
        }
    }
}
