#ifndef VerbatimTransformer_Included
#define VerbatimTransformer_Included

#include "StreamTransformer.h"

using namespace std;

/* Simply copies a stream over from input to output unmodified */
class VerbatimTransformer : public StreamTransformer {

  protected:

    void transform();
};

#endif
