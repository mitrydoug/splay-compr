#ifndef PPMImageEncoder_Included
#define PPMImageEncoder_Included

#include <string>

#include "StreamEncoder.h"

class PPMImageEncoder : public StreamEncoder {
  
  public:

      PPMImageEncoder(EncoderMode mode)
        : StreamEncoder(mode),
          imageWidth(0),
          imageHeight(0),
          imageMaxLum(0),
          bytesPerVal(0) {}

  protected:

      size_t imageWidth;
      size_t imageHeight;
      size_t imageMaxLum;
      size_t bytesPerVal;

      void readHeader();

  private:

      size_t readHeaderInt();

      static const string magic;
};

#endif
