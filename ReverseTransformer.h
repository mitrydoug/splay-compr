#ifndef ReverseTransformer_Included
#define ReverseTransformer_Included

#include <stack>

#include "StreamTransformer.h"

using namespace std;

class ReverseTransformer : public StreamTransformer {

  protected:

    void transform();

  private:
    std::stack<byte> streamData;
};

#endif
