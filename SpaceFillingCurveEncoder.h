#ifndef SpaceFillingCurveEncoder_Included
#define SpaceFillingCurveEncoder_Included

#include <vector>

#include "PPMImageEncoder.h"

class SpaceFillingCurveEncoder : public PPMImageEncoder {

  public:

    SpaceFillingCurveEncoder(EncoderMode mode)
      : PPMImageEncoder(mode) {}

  protected:

    void encode();

    void decode();

  private:

    void readGrid(std::vector<std::vector<byte>> &grid);

    void writeGrid(std::vector<std::vector<byte>> &grid);

    void hilbert(std::vector<std::vector<byte>> &grid);

    void hilbertRecurse(std::vector<std::vector<byte>> &grid, size_t *sym,
            double sizeX, double sizeY,
            double posX,  double posY);
};

#endif
