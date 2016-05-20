#ifndef PrefixTransformer_Included
#define PrefixTransformer_Included

#include "StreamTransformer.h"

class PrefixTransformer : public StreamTransformerImpl {
  public:
      PrefixTransformer(std::string p) : prefix(p) {}

  protected:
      virtual void transform();

  private:
      std::string prefix;
};

#endif
