#ifndef RGBSplitEncoder_Included
#define RGBSplitEncoder_Included

#include <vector>

#include "PPMImageEncoder.h"

class RGBSplitEncoder : public PPMImageEncoder {

  public:

    RGBSplitEncoder(EncoderMode mode)
      : PPMImageEncoder(mode) {}

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
