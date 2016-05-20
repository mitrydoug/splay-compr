#ifndef StreamEncoder_Included
#define StreamEncoder_Included

#include "StreamTransformer.h"

using namespace std;

typedef enum EncoderMode {
    ENCODE,
    DECODE
} EncoderMode;

class StreamEncoder : public StreamTransformerImpl {

  protected:

    StreamEncoder(EncoderMode mode) : encodeOrDecode(mode) {}

    void transform();

    virtual void encode()=0;

    virtual void decode()=0;

  private:
    EncoderMode encodeOrDecode;
};

#endif

