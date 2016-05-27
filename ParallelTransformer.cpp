#include "ParallelTransformer.h"

#include <sstream>

using namespace std;

void ParallelTransformer::transform() {
    /* A no-op for this class */
}

void ParallelTransformer::exec(istream *is, ostream *os) { 
    for (size_t i=0; i<transformers.size(); i++) {
        transformers[i]->exec(is, os);
    }
}
