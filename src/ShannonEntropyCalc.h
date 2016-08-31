#ifndef ShannonEntropyCalc_Included
#define ShannonEntropyCalc_Included

#include "PPMImageEncoder.h"

using namespace std;

/* Copies stream over from input to output. Does so bit-by-bit to test this
 * functionality
 */
class ShannonEntropyCalc : public PPMImageEncoder {
  public:

    ShannonEntropyCalc(EncoderMode mode) : PPMImageEncoder(mode) {}

  protected:

    void encode();
    void decode();
};

#endif
