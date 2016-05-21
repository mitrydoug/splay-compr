
#ifndef AlphaRunLengthEncoder_Included
#define AlphaRunLengthEncoder_Included

#include "StreamEncoder.h"

using namespace std;

/* Class AlphaRunLengthEncoder
 * ---------------------------
 *  A simple stream encoder. Accepts input consisting of alphabet characters
 *  and spaces, and performs a simple run-length encoding. Each character is
 *  prefixed by the number of times it is repeated in the data stream. For
 *  instance
 *
 *   encode("aaabbce") = "3a2b1c1e"
 */
class AlphaRunLengthEncoder : public StreamEncoder {

  protected:

    /* These functions are no longer purely virtual, so we will define them
     * here in this class
     */

    void encode();

    void decode();

};

#endif
