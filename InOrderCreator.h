#ifndef InOrderCreator_Included
#define InOrderCreator_Included

#include "StreamTransformer.h"

using namespace std;

/* Copies stream over from input to output. Does so bit-by-bit to test this
 * functionality
 */
class InOrderCreator : public StreamTransformer {

  protected:

    void transform();
};

#endif
