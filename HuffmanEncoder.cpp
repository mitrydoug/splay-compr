#include "HuffmanEncoder.h"

#include <queue>

void HuffmanEncoder::encode() {
    vector<byte> toEncode;
    map<int, int> freqTable;
    buildFreqTable(toEncode, freqTable);
    writeHeader(freqTable);
    HuffmanNode* encodingTree = buildTree(freqTable);
    map<int, string> encodingMap;
    buildEncodingMap(encodingTree, encodingMap);
    encodeData(toEncode, encodingMap);
    freeTree(encodingTree);
}

void HuffmanEncoder::decode() {
    map<int, int> freqTable;
    readHeader(freqTable);
    HuffmanNode* encodingTree = buildTree(freqTable);
    decodeData(encodingTree);
    freeTree(encodingTree);
}

/**
 * Reads through the input file and stores all of the data in a vector
 * while incrementing the frequency count for each character seen
 */
void HuffmanEncoder::buildFreqTable(vector<byte>& toEncode, map<int, int>& freqTable) {
    byte by;
    while (readByte(by)) {
        toEncode.push_back(by);
        freqTable[by]++;
    }
    freqTable[PSEUDO_EOF] = 1;
}

/**
 * Writes the frequency table to the beginning of the compressed file
 * so that it can be decompressed later
 */
void HuffmanEncoder::writeHeader(const map<int, int>& freqTable) {
    for (const auto& keyVal : freqTable) {
        *output << keyVal.first << ' ' << keyVal.second << ' ';
    }
    *output << endl;
}

/**
 * Creates a tree of characters present in the file where the most frequent
 * characters are at the top. This is used to determine the bit encoding
 * of each character.
 */
HuffmanEncoder::HuffmanNode* HuffmanEncoder::buildTree(const map<int, int>& freqTable) const {
    if (freqTable.size() <= 1) return NULL;

    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

    for (const auto& keyVal : freqTable) {
        HuffmanNode* node = new HuffmanNode(keyVal.first, keyVal.second);
        pq.push(node);
    }

    while (pq.size() > 1) {
        HuffmanNode* zero = pq.top();
        pq.pop();
        HuffmanNode* one = pq.top();
        pq.pop();
        HuffmanNode* parent = new HuffmanNode(NOT_A_CHAR, zero->count + one->count, zero, one);
        pq.push(parent);
    }

    return pq.top();
}

/**
 * Traverses the encoding tree and maps all of the characters (each in a leaf)
 * to a bit encoding based on the path taken to reach their nodes
 */
void HuffmanEncoder::buildEncodingMap(const HuffmanNode* encodingTree, map<int, string>& encodingMap) const {
    if (encodingTree == NULL) return;
    encodingTreeTraversal(encodingTree, encodingMap, "");
}

/**
 * Traverses down the encoding tree, adding a bit to the code for each child
 * followed and finalizing the encoding for each character at the leaves
 */
void HuffmanEncoder::encodingTreeTraversal(const HuffmanNode* node, map<int, string>& encodingMap, string code) const {
    if (node->isLeaf()) {
        encodingMap[node->character] = code;
    } else {
        encodingTreeTraversal(node->zero, encodingMap, code + "0");
        encodingTreeTraversal(node->one, encodingMap, code + "1");
    }
}

/**
 * Writes the encoded version of each stream character to the output file
 */
void HuffmanEncoder::encodeData(const vector<byte>& toEncode, const map<int, string>& encodingMap) {
    size_t numBytes = toEncode.size();
    for (size_t i = 0; i < numBytes; ++i) {
        encodeByte(toEncode[i], encodingMap);
    }
    encodeByte(PSEUDO_EOF, encodingMap);
    flushByte();
}

/**
 * Writes the encoded version of one character to the output file
 */
void HuffmanEncoder::encodeByte(int character, const map<int, string>& encodingMap) {
    string code = encodingMap.at(character);
    size_t len = code.length();
    for (size_t i = 0; i < len; ++i) {
        bit bi = code[i] - '0';
        writeBit(bi);
    }
}

/**
 * Frees all memory associated with a Huffman tree
 */
void HuffmanEncoder::freeTree(HuffmanNode* root) {
    if (root == NULL) return;
    freeTree(root->zero);
    freeTree(root->one);
    delete root;
}

/**
 * Reconstructs a character frequency table in a file to be compressed
 */
void HuffmanEncoder::readHeader(map<int, int>& freqTable) {
    int key;
    int value;
    do {
        *input >> key >> value;
        freqTable[key] = value;
    } while (key != PSEUDO_EOF);
    *input >> ws;
}

/**
 * Reads through the input file and stores all of the data in a vector
 * while incrementing the frequency count for each character seen
 */
void HuffmanEncoder::decodeData(const HuffmanNode* encodingTree) {
    if (encodingTree == NULL) return;
    int character = decodeByte(encodingTree);
    while (character != PSEUDO_EOF) {
        writeByte(character);
        character = decodeByte(encodingTree);
    }
    dropByte();
}

/**
 * Traverses down the encoding tree until we hit a leaf and return its character
 */
int HuffmanEncoder::decodeByte(const HuffmanNode* root) {
    if (root->isLeaf()) {
        return root->character;
    }
    bit bi;
    readBit(bi);
    if (bi) {
        return decodeByte(root->one);
    } else {
        return decodeByte(root->zero);
    } 
}