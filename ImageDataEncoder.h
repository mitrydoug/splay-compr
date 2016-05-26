#ifndef ImageDataEncoder_Included
#define ImageDataEncoder_Included

#include "StreamEncoder.h"

class ImageDataEncoder : public StreamEncoder {

  public:

    ImageDataEncoder(EncoderMode mode)
      : StreamEncoder(mode),
        imageWidth(0),
        imageHeight(0) {}

    void setDimensions(size_t width, size_t height);

  protected:

    size_t imageWidth;
    size_t imageHeight;

};

#endif
