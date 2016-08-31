#ifndef HuffmanEncoder_Included
#define HuffmanEncoder_Included

#include "StreamEncoder.h"

#include <map>
#include <vector>


#define PSEUDO_EOF 256
#define NOT_A_CHAR 257

using namespace std;

/* Class HuffmanEncoder
 * ---------------------------
 *  A stream encoder which uses the character frequencies across an entire
 *  input stream in order to map characters to bit encodings. More frequent
 *  characters are encoded with fewer bits.
 */
class HuffmanEncoder : public StreamEncoder {

  public:

    HuffmanEncoder(EncoderMode mode) : StreamEncoder(mode) {}

  protected:

    void encode();

    void decode();

  private:

    // struct taken from the Stanford C++ libraries
    typedef struct HuffmanNode {

        int character;       // character being represented by this node
        int count;           // number of occurrences of that character
        HuffmanNode* zero;   // 0 (left) subtree (NULL if empty)
        HuffmanNode* one;    // 1 (right) subtree (NULL if empty)
     
        /*
         * Constructs a new node to store the given character and its count,
         * along with the given child pointers.
         */
        HuffmanNode(int character = NOT_A_CHAR, int count = 0, HuffmanNode* zero = NULL, HuffmanNode* one = NULL) {
            this->character = character;
            this->count = count;
            this->zero = zero;
            this->one = one;
        }
     
        // ~HuffmanNode() {}
     
        /*
         * Returns true if this node is a leaf (has NULL children).
         */
        bool isLeaf() const {
            return zero == NULL && one == NULL;
        }
    } HuffmanNode;

    class Compare {
    public: 
        bool operator() (HuffmanNode* lhs, HuffmanNode* rhs) {
            return lhs->count > rhs->count;
        }
    };

    void buildFreqTable(vector<byte>& toEncode, map<int, int>& freqTable);
    void writeHeader(const map<int, int>& freqTable);
    HuffmanNode* buildTree(const map<int, int>& freqTable) const;
    void buildEncodingMap(const HuffmanNode* encodingTree, map<int, string>& encodingMap) const;
    void encodingTreeTraversal(const HuffmanNode* node, map<int, string>& encodingMap, string code) const;
    void encodeData(const vector<byte>& toEncode, const map<int, string>& encodingMap);
    void encodeByte(int character, const map<int, string>& encodingMap);
    void freeTree(HuffmanNode* root);
    void readHeader(map<int, int>& freqTable);
    void decodeData(const HuffmanNode* encodingTree);
    int decodeByte(const HuffmanNode* root);

};

#endif