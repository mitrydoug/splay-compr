#include "SeriesTransformer.h"

#include <sstream>

using namespace std;

void SeriesTransformer::transform() {
    /* A no-op for this class */
}

void SeriesTransformer::exec(istream *is, ostream *os) {
    istream *iss = is;
    ostream *oss = nullptr;
    
    for (size_t i=0; i<transformers.size(); i++) {
        if (i == transformers.size()-1) {
            oss = os;
        } else {
            oss = new stringstream();
        }
        transformers[i]->exec(iss, oss);
        if (i > 0) {
            delete iss;
        } 
        if(i != transformers.size()-1) {
            iss = (stringstream *)oss;
        }
    }
}
