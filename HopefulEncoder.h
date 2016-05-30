#ifndef HopefulEncoder_Included
#define HopefulEncoder_Included

#include "StreamEncoder.h"

using namespace std;

/* Class HopefulEncoder
 * ---------------------------
 */
class HopefulEncoder : public StreamEncoder {

  public:

    /* Constructor, to interface with encoding */
    HopefulEncoder(EncoderMode mode) : StreamEncoder(mode) {}

  protected:

    /* These functions are no longer purely virtual, so we will define them
     * here in this class
     */

    void encode();

    void decode();

  private:

    static const size_t PSEUDO_EOF;
    static const size_t INTERNAL;

    typedef struct Node {
        Node(size_t v,
             struct Node *u=nullptr,
             struct Node *l=nullptr,
             struct Node *r=nullptr)
            : value(v),
              up(u),
              left(l),
              right(r) {}

        size_t value;

        struct Node *up;
        struct Node *left;
        struct Node *right;

        size_t numUp;
        size_t numLeft;
        size_t numRight;

    } Node;

};

#endif
