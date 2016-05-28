#include "SplayPrefixEncoder.h"

#include <cctype>
#include <sstream>
#include <stdlib.h>
#include <utility>
#include <stack>

#define MAX_LEVEL 8 // 2^8 possible byte values so 2^8 leaves
#define PSEUDO_EOF 256

struct treeNode {
    struct treeNode *left;
    struct treeNode *right;
    struct treeNode *parent;
    unsigned short ch; // Needs to be a short to handle PSEUDO_EOF
};

/**
 * This function frees the memory associated with the tree rooted at the
 * parameter "tree".
 */
void SplayPrefixEncoder::freeTree(struct treeNode *tree) {
    if(tree == NULL) return;
    freeTree(tree->left);
    freeTree(tree->right);
    free(tree);
}

/**
 * buildInitialTree recursively builds a perfectly balanced tree in which
 * all of the leaves have a unique byte. In order to build this tree, we 
 * need to know what level we are at in the tree, how many byte nodes we
 * have already created, and the parent node (in order to keep the parent pointers 
 * up to date). Inserting a leaf node will increment the byte parameter, which
 * is passed by reference, by one. A map is populated that maps each byte value
 * to the corresponding leaf node in the tree.
 */
struct treeNode* SplayPrefixEncoder::buildInitialTree(unordered_map<unsigned short, struct treeNode*>& map) {
    byte by = 0;
    struct treeNode *root = buildTreeHelper(0, by, NULL, map); 

    // Modify the tree to put PSEUDO_EOF in it
    struct treeNode *oldLeaf = map.find(0)->second;
    struct treeNode *newLeaf1 = (struct treeNode *)malloc(sizeof(struct treeNode));
    newLeaf1->parent = oldLeaf;
    newLeaf1->left = NULL;
    newLeaf1->right = NULL;
    newLeaf1->ch = oldLeaf->ch;
    struct treeNode *newLeaf2 = (struct treeNode *)malloc(sizeof(struct treeNode));
    newLeaf2->parent = oldLeaf;
    newLeaf2->left = NULL;
    newLeaf2->right = NULL;
    newLeaf2->ch = PSEUDO_EOF;

    oldLeaf->left = newLeaf1;
    oldLeaf->right = newLeaf2; 

    map.find(0)->second = newLeaf1;
    pair<unsigned short, struct treeNode*> toInsert(PSEUDO_EOF, newLeaf2);
    map.insert(toInsert);

    return root;
}

struct treeNode* SplayPrefixEncoder::buildTreeHelper(int level, byte &by, struct treeNode* parent, unordered_map<unsigned short, struct treeNode*>& map) {
    if(level > MAX_LEVEL) return NULL;
    struct treeNode *newRoot = (struct treeNode*)malloc(sizeof(struct treeNode));
    newRoot->parent = parent;
    newRoot->left = buildTreeHelper(level + 1, by, newRoot, map);
    newRoot->right = buildTreeHelper(level + 1, by, newRoot, map);
    if(level == MAX_LEVEL) {
        newRoot->ch = (unsigned short)by;
        pair<unsigned short, struct treeNode*> toInsert(newRoot->ch, newRoot);
        map.insert(toInsert);
        by++;
    }
    return newRoot;
}

/**
 * This modified splaying algorithm uses semi-rotations to update the tree.
 * Leaf is the node that was just looked up in the tree. So, we will follow
 * 2 of its parents links and rewire the children to do a semi-rotation until
 * we get to the root of the tree.
 */
void SplayPrefixEncoder::splay(struct treeNode* leaf) {
    while(true) {
        struct treeNode* parent = leaf->parent;
        // If parent or current node (leaf) is the root, break
        if(leaf->parent == NULL || parent->parent == NULL) break;
        struct treeNode* grandparent = parent->parent;
        struct treeNode* gpChild = grandparent->left;
        if(gpChild == parent) { // parent is left child of grandparent
            gpChild = grandparent->right;
            grandparent->right = leaf;
        } else {
            grandparent->left = leaf;
        }
        if(leaf == parent->left) {
            // current node is a left child of parent
            parent->left = gpChild;
        } else {
            parent->right = gpChild;
        }
        leaf->parent = grandparent;
        gpChild->parent = parent;
        leaf = grandparent;
    }    
}

/**
 * This function writes the encoding of the given byte to the output stream
 * and then splays the tree. It does so by starting at the leaf node for that
 * character and then traversing up the tree to find the bit sequence to write.
 */
void SplayPrefixEncoder::traverseTree(unsigned short by, unordered_map<unsigned short, struct treeNode*> map) {
    stack<bit> bitStack;
    //cout << "Character: " << (char) by << endl;
    auto result = map.find(by);
    if(result == map.end()) {
        throw "Error in encode(): Could not find leaf node for " + by;
    }
    struct treeNode* leaf = result->second;
    struct treeNode* cur = leaf->parent;
    struct treeNode* prev = leaf;
    // Walk up tree pushing bits
    while(cur != NULL) { // != NULL since root->parent is NULL
        if(cur->right == prev) {
            // Right child so push a 1
            bitStack.push(1);
        } else {
            // Left child so push a 0
            bitStack.push(0);
        }
        prev = cur;
        cur = cur->parent;
    } 
    // Write the bits to the output
    //cout << "Writing bits..." << endl;
    while(!bitStack.empty()) {
        writeBit(bitStack.top());
        //cout << bitStack.top();
        bitStack.pop();
    }
    //cout << endl;
    splay(leaf);
}

/**
 * This function reads from the input and compresses the data. First, a 
 * perfectly balanced tree is created. Then, each byte read from the input.
 * Each byte corresponds to a leaf node in the tree. The path to this leaf
 * is traversed and the proper bits are written to the ouput. Then, the tree
 * is splayed.
 */
void SplayPrefixEncoder::encode() {
    unordered_map<unsigned short, struct treeNode*> map;
    byte by = 0;
    struct treeNode *root = buildInitialTree(map); 
    
    while(readByte(by)) {
        traverseTree((unsigned short)by, map);
    }

    // Write PSEUDO_EOF
    traverseTree(PSEUDO_EOF, map);
    flushByte();
    freeTree(root);
}

/**
 * This function builds an initial perfectly balanced tree containing each
 * possible byte. Bits are read from the output and the tree is traversed
 * until a leaf is reached. The byte of that leaf node is written to the 
 * output. Then, the tree is splayed from that node.
 */
void SplayPrefixEncoder::decode() { 
    unordered_map<unsigned short, struct treeNode*> map;
    struct treeNode *root = buildInitialTree(map); 

    bit bi;
    struct treeNode *node = root;
    while(readBit(bi)) {
        //cout << bi;
        if(bi == 0) {
            node = node->left;
        } else {
            node = node->right;
        }
        // Reached a leaf
        if(node->left == NULL && node->right == NULL) {
            if(node->ch == PSEUDO_EOF) break;
            byte by = node->ch;
            //cout << "Char: " << (char)by << endl;
            writeByte(by);
            splay(node);
            node = root;
        }
    }
    freeTree(root);
}
