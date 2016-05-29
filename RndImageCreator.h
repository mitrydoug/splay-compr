#ifndef RndImageCreator_Included
#define RndImageCreator_Included

#include "StreamTransformer.h"

using namespace std;

/* Copies stream over from input to output. Does so bit-by-bit to test this
 * functionality
 */
class RndImageCreator : public StreamTransformer {

  protected:

    void transform();
};

#endif
