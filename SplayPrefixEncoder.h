#ifndef SplayPrefixEncoder_Included
#define SplayPrefixEncoder_Included

#include "StreamEncoder.h"
#include <unordered_map>

using namespace std;

struct treeNode;

/* Class SplayPrefixEncoder
 * ---------------------------
 * This encoder uses the semi-splaying compression algorithm described
 * in the paper. It uses prefix codes to encode the input and then semi-splays
 * the encoding tree as the stream is traversed.
 */
class SplayPrefixEncoder : public StreamEncoder {

  public:

    /* Constructor, to interface with encoding */
    SplayPrefixEncoder(EncoderMode mode) : StreamEncoder(mode) {}

  protected:

    /* These functions are no longer purely virtual, so we will define them
     * here in this class
     */

    void encode();

    void decode();

    void splay(struct treeNode* leaf);
    struct treeNode* buildInitialTree(int level, byte& by, struct treeNode* parent, unordered_map<byte, struct treeNode*>& map);
    void freeTree(struct treeNode* tree);
};

#endif
