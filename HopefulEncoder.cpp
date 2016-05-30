#include <iostream>
#include <unordered_map>
#include <queue>

#include "HopefulEncoder.h"

using namespace std;

const size_t HopefulEncoder::PSEUDO_EOF = 256;
const size_t HopefulEncoder::INTERNAL = 257;

void HopefulEncoder::encode() {
    unordered_map<Node*> leafMap;

    Node *up    = new Node(0);
    Node *left  = new Node(1);
    Node *right = new Node(2);
    Node *center = new Node(INTERNAL, up, left, right);

    queue<Node*> q;
    queue.push(up);
    queue.push(left);
    queue.push(right);

    for (size_t leaves=3; leaves<256; leaves++) {
        Node *oldLeaf = queue.pop();
        Node *newLeaf = new Node(INTERNAL);
    }
}

void HopefulEncoder::decode() {

}
