#ifndef PPMDelegateEncoder_Included
#define PPMDelegateEncoder_Included

#include <vector>

#include "PPMImageEncoder.h"

class PPMDelegateEncoder : public PPMImageEncoder {

  public:

    PPMDelegateEncoder(EncoderMode mode, StreamTransformer *st)
      : PPMImageEncoder(mode), delegate(st) {}

  protected:

    void encode();

    void decode();

  private:

    StreamTransformer *delegate;
};

#endif
