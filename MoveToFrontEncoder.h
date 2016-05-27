#ifndef MoveToFrontEncoder_Included
#define MoveToFrontEncoder_Included

#include "StreamEncoder.h"

using namespace std;

/* Class MoveToFrontEncoder
 * ---------------------------
 */
class MoveToFrontEncoder : public StreamEncoder {

  public:

    /* Constructor, to interface with encoding */
    MoveToFrontEncoder(EncoderMode mode) : StreamEncoder(mode) {}

  protected:

    /* These functions are no longer purely virtual, so we will define them
     * here in this class
     */

    void encode();

    void decode();

  private:

    typedef struct Node {
        Node(size_t v, struct Node *n)
            : val(v), next(n) {}
        size_t val;
        struct Node *next;
    } Node;

    Node *buildList();

    void freeList(Node *front);

    size_t getIndex(Node *&front, byte b);

};

#endif
