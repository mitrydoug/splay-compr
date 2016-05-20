
#ifndef AlphaRunLengthEncoder_Included
#define AlphaRunLengthEncoder_Included

#include "StreamEncoder.h"

using namespace std;

class AlphaRunLengthEncoder : public StreamEncoder {

  public:

    AlphaRunLengthEncoder(EncoderMode mode) : StreamEncoder(mode) {}

  protected:

    void encode();

    void decode();

};

#endif
