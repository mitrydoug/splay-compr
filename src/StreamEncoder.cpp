#include "StreamEncoder.h"

void StreamEncoder::transform() {
    switch (encodeOrDecode) {
        case ENCODE: encode(); break;
        case DECODE: decode(); break;
    }
}
