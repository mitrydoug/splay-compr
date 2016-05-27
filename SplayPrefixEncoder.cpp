#include "SplayPrefixEncoder.h"

#include <cctype>
#include <sstream>
#include <stdlib.h>
#include <utility>
#include <stack>

#define MAX_LEVEL 8 // 2^8 possible byte values so 2^8 leaves

struct treeNode {
    struct treeNode *left;
    struct treeNode *right;
    struct treeNode *parent;
    byte ch;
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
struct treeNode* SplayPrefixEncoder::buildInitialTree(int level, byte &by, struct treeNode* parent, unordered_map<byte, struct treeNode*>& map) {
    if(level > MAX_LEVEL) return NULL;
    struct treeNode *newRoot = (struct treeNode*)malloc(sizeof(struct treeNode));
    newRoot->parent = parent;
    newRoot->left = buildInitialTree(level + 1, by, newRoot, map);
    newRoot->right = buildInitialTree(level + 1, by, newRoot, map);
    if(level == MAX_LEVEL) {
        newRoot->ch = by;
        pair<byte, struct treeNode*> toInsert(by, newRoot);
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
        if(parent->parent == NULL || leaf->parent == NULL) break;
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
 * This function reads from the input and compresses the data. First, a 
 * perfectly balanced tree is created. Then, each byte read from the input.
 * Each byte corresponds to a leaf node in the tree. The path to this leaf
 * is traversed and the proper bits are written to the ouput. Then, the tree
 * is splayed.
 */
void SplayPrefixEncoder::encode() {
    unordered_map<byte, struct treeNode*> map;
    byte by = 0;
    struct treeNode *root = buildInitialTree(0, by, NULL, map); 
    stack<bit> bitStack;
    
    while(readByte(by)) {
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
        while(!bitStack.empty()) {
            writeBit(bitStack.top());
            bitStack.pop();
        }
        splay(leaf);
    }
    
    freeTree(root);
}

/**
 * This function builds an initial perfectly balanced tree containing each
 * possible byte. Bits are read from the output and the tree is traversed
 * until a leaf is reached. The byte of that leaf node is written to the 
 * output. Then, the tree is splayed from that node.
 */
void SplayPrefixEncoder::decode() { 
    unordered_map<byte, struct treeNode*> map;
    byte by = 0;
    struct treeNode *root = buildInitialTree(0, by, NULL, map); 

    bit bi;
    struct treeNode *node = root;
    while(readBit(bi)) {
        if(bi == 0) {
            node = node->left;
        } else {
            node = node->right;
        }
        // Reached a leaf
        if(node->left == NULL && node->right == NULL) {
            byte by = node->ch;
            writeByte(by);
            splay(node);
            node = root;
        }
    }

    freeTree(root);
}
