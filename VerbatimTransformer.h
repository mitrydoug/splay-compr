#ifndef VerbatimTransformer_Included
#define VerbatimTransformer_Included

#include "StreamTransformer.h"

using namespace std;

class VerbatimTransformer : public StreamTransformerImpl {

  protected:

    void transform();
};

#endif
