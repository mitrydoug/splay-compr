#ifndef PrefixTransformer_Included
#define PrefixTransformer_Included

#include "StreamTransformer.h"

/* Class Prefix Transformer
 * ------------------------
 *  Copies the bytes from the input stream to the output stream, however it
 *  adds a prefix to the stream, given to the constructor.
 */
class PrefixTransformer : public StreamTransformer {
  public:

      /* @param p the prefix to be placed on the stream */
      PrefixTransformer(std::string p) : prefix(p) {}

  protected:
      void transform();

  private:
      std::string prefix;
};

#endif
