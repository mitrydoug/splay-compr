#ifndef HopefulEncoder_Included
#define HopefulEncoder_Included

#include <unordered_map>
#include <queue>

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

    static const size_t HE_PSEUDO_EOF;
    static const size_t INTERNAL;
    static const size_t TREE_LEAVES;

    typedef enum Dir {
        UP,
        LEFT,
        RIGHT
    } Dir;

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

        size_t numLeavesUp;
        size_t numLeavesLeft;
        size_t numLeavesRight;

    } Node;

    Node *leafParent(Node *n, Dir &d);
    Node *largerSubtree(Node *n);

    void sizesAndLeafMap(Node *c, unordered_map<size_t, Node*> &map);
    void sizesAndLeafMapHelper(Node *c, unordered_map<size_t, Node*> &map,
            size_t &numLeaves);

    void writeMoveBit(Node *prev, Node *finger, Node *next);

    void freeMemory(Node *parent, Node *n);

    Node*   otherNeighbor(Node*, Node*, Node*);
    Node*&  outwardPtr(Node *src, Node *dest);
    size_t& outwardLeafCnt(Node *src, Node *dest);

    size_t leavesAway(Node *src, Node *dst);

    void splayPath(queue<Node*> &q);

};

#endif
