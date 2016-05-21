#ifndef VerbatimBitTransformer_Included
#define VerbatimBitTransformer_Included

#include "StreamTransformer.h"

using namespace std;

/* Copies stream over from input to output. Does so bit-by-bit to test this
 * functionality
 */
class VerbatimBitTransformer : public StreamTransformer {

  protected:

    void transform();
};

#endif
