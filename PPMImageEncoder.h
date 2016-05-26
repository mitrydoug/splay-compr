#ifndef PPMImageEncoder_Included
#define PPMImageEncoder_Included

#include <string>

#include "StreamEncoder.h"
#include "ImageDataEncoder.h"

class PPMImageEncoder : public StreamEncoder {
  
  public:

      PPMImageEncoder(EncoderMode mode, ImageDataEncoder *de)
        : StreamEncoder(mode), dataEncoder(de),
          imageWidth(0),
          imageHeight(0),
          imageMaxLum(0) {}

  protected:

      void encode();

      void decode();

  private:

      ImageDataEncoder *dataEncoder;

      size_t imageWidth;
      size_t imageHeight;
      size_t imageMaxLum;

      void readHeader();
      size_t readHeaderInt();

      static const string magic;
};

#endif
