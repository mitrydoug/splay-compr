#ifndef PrefixTransformer_Included
#define PrefixTransformer_Included

#include "StreamTransformer.h"

class PrefixTransformer : public StreamTransformer {
  public:
      PrefixTransformer(std::string p) : prefix(p) {}

  protected:
      void transform();

  private:
      std::string prefix;
};

#endif
