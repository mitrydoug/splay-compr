#ifndef RGBSplitEncoder_Included
#define RGBSplitEncoder_Included

#include <vector>

#include "ImageDataEncoder.h"

class RGBSplitEncoder : public ImageDataEncoder {

  public:

    RGBSplitEncoder(EncoderMode mode)
      : ImageDataEncoder(mode) {}

  protected:

    void encode();

    void decode();

  private:

    std::vector<byte> redChn;
    std::vector<byte> greenChn;
    std::vector<byte> blueChn;

    void readChannel(std::vector<byte> &v);

};

#endif
