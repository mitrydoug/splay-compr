#ifndef ReverseTransformer_Included
#define ReverseTransformer_Included

#include <stack>

#include "StreamTransformer.h"

using namespace std;

/* An example of a transformer that preprocesses its entire input stream before
 * writing any bytes to the output stream. Huffman coding will need to use a
 * strategy like this one, for preprocessing the character frequencies.
 */
class ReverseTransformer : public StreamTransformer {

  protected:

    void transform();

  private:

    // used for storing input stream data and accessing it in reverse
    std::stack<byte> streamData;
};

#endif
