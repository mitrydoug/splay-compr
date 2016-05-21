#ifndef StreamEncoder_Included
#define StreamEncoder_Included

#include "StreamTransformer.h"

using namespace std;

/* A simple enum used to help initialize stream encoders in either ENCODE mode
 * or DECODe mode
 */
typedef enum EncoderMode {
    ENCODE,
    DECODE
} EncoderMode;


/* Class StreamEncoder
 * -------------------
 *  Defines a slight extension to the StreamTransformer abstraction.
 *
 *  A StreamEncoder is a StreamTransformer that can be run in one of two
 *  modes: encode or decode. These two functions must be inverses of one
 *  another, so that a StreamEncoder runing in decode mode on data produced
 *  by the same encoder in encode mode must result in the original data.
 *
 *  This abstraction could be used for any form of communication (i.e.
 *  encryption, error-correcting codes, protocols, etc). We will be using
 *  these to implement our data compressors.
 */
class StreamEncoder : public StreamTransformer {

  protected:

    StreamEncoder(EncoderMode mode) : encodeOrDecode(mode) {}

    /* Implemented by this class. Does nothing more than direct the call to
     * encode or decode based on the mode of the encoder.
     */
    void transform();

    /* Given a raw data stream, transforms the stream by some encoding defined
     * by the class. This encoding must be invertible, specifically by the
     * decode function below.
     */
    virtual void encode()=0;

    /* This must be the inverse of the encode(). Converts an encoded data
     * stream back into the original raw stream
     */
    virtual void decode()=0;

  private:
    /* The mode of encoder */
    const EncoderMode encodeOrDecode;
};

#endif

