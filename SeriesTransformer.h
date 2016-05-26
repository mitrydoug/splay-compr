#ifndef SeriesTransformer_Included
#define SeriesTransformer_Included

#include <vector>

#include "StreamTransformer.h"

class SeriesTransformer : public StreamTransformer {

  private:

      void fillTrans(StreamTransformer *st) {
          transformers.push_back(st);
      }

      std::vector<StreamTransformer*> transformers;

      template<typename... Args>
      void fillTrans(StreamTransformer *st, Args... sts) {
          transformers.push_back(st);
          fillTrans(sts...);
      }

  public:

      template<typename... Args>
      SeriesTransformer(Args... sts) { fillTrans(sts...); }

      virtual void exec(std::istream *os, std::ostream *is);

  protected:

      void transform(); 

};

#endif
