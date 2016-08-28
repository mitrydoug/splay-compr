#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>

#include "HopefulEncoder.h"

using namespace std;

const size_t HopefulEncoder::HE_PSEUDO_EOF = 256;
const size_t HopefulEncoder::INTERNAL = 257;
const size_t HopefulEncoder::TREE_LEAVES = 257;

void HopefulEncoder::freeMemory(Node *parent, Node *n) {
    if (n == nullptr) return;
    if (n->up != parent) {
        freeMemory(n, n->up);
    }
    if (n->left != parent) {
        freeMemory(n, n->left);
    }
    if (n->right != parent) {
        freeMemory(n, n->right);
    }
    delete n;
}

void HopefulEncoder::sizesAndLeafMapHelper(
        Node *node, unordered_map<size_t, Node*> &leafMap,
        size_t &numLeaves) {

    if (node->value != INTERNAL) { // leaf node
        node->numLeavesUp = TREE_LEAVES - 1;
        node->numLeavesLeft = 0;
        node->numLeavesRight = 0;
        leafMap[node->value] = node;
        numLeaves = 1;
        return;
    }

    sizesAndLeafMapHelper(node->left,  leafMap, node->numLeavesLeft);
    sizesAndLeafMapHelper(node->right, leafMap, node->numLeavesRight);

    numLeaves = node->numLeavesLeft + node->numLeavesRight;
    node->numLeavesUp = TREE_LEAVES - numLeaves;
}

void HopefulEncoder::sizesAndLeafMap(
        Node *center, unordered_map<size_t, Node*> &leafMap) {
    sizesAndLeafMapHelper(center->up, leafMap, center->numLeavesUp);
    sizesAndLeafMapHelper(center->left, leafMap, center->numLeavesLeft);
    sizesAndLeafMapHelper(center->right, leafMap, center->numLeavesRight);
}

HopefulEncoder::Node* HopefulEncoder::otherNeighbor(Node *center, Node *one, Node *two) {
     if (center->up != one && center->up != two) {
        return center->up;
     } else if (center->left != one && center->left != two) {
        return center->left;
     } else {
        return center->right;
     }
}

HopefulEncoder::Node*& HopefulEncoder::outwardPtr(Node *src, Node *dst) {
    if (src->up == dst) {
        return src->up;
    } else if (src->left == dst) {
        return src->left;
    } else if (src->right == dst){
        return src->right;
    } else {
        throw "not valid";
        return src->right;
    }
}

size_t& HopefulEncoder::outwardLeafCnt(Node *src, Node *dst) {
    if (src->up == dst) {
        return src->numLeavesUp;
    } else if (src->left == dst) {
        return src->numLeavesLeft;
    } else if (src->right == dst){
        return src->numLeavesRight;
    } else {
        throw "not valid";
        return src->numLeavesRight;
    }
}

size_t HopefulEncoder::leavesAway(Node *src, Node *dst) {
    if (dst->value != INTERNAL) {
        return 1;
    }

    if (dst->up == src) {
        return dst->numLeavesLeft + dst->numLeavesRight;
    } else if (dst->left == src) {
        return dst->numLeavesUp + dst->numLeavesRight;
    } else if (dst->right == src){
        return dst->numLeavesLeft + dst->numLeavesUp;
    } else {
        throw "not valid";
        return dst->numLeavesLeft + dst->numLeavesRight;
    }
}

void HopefulEncoder::splayPath(queue<Node*> &q) {
    Node *D = q.front();
    q.pop();
    Node *A = q.front();
    q.pop();
    Node *B = q.front();
    q.pop();
    Node *C = q.front();
    q.pop();
    Node *E = otherNeighbor(A, D, B);

    //cout << "A->D: " << leavesAway(A, D) << endl;
    //cout << "A->B: " << leavesAway(A, B) << endl;
    //cout << "A->E: " << leavesAway(A, E) << endl;

    //cout << "B->A: " << leavesAway(B, A) << endl;
    //cout << "B->C: " << leavesAway(B, C) << endl;

    //cout << "E->A: " << leavesAway(E, A) << endl;

    outwardPtr(C, B) = A;
    outwardPtr(A, E) = C;
    outwardLeafCnt(A, C) = leavesAway(A, C);

    outwardPtr(B, C) = E;
    outwardPtr(E, A) = B;
    outwardLeafCnt(B, A) = leavesAway(B, A);
    outwardLeafCnt(B, E) = leavesAway(B, E);
    outwardLeafCnt(E, B) = leavesAway(E, B);
    outwardLeafCnt(A, B) = leavesAway(A, B);
    q.push(A);
    q.push(C);

    //cout << "\n\nA->D: " << leavesAway(A, D) << endl;
    //cout << "A->B: " << leavesAway(A, B) << endl;
    //cout << "A->C: " << leavesAway(A, C) << endl;

    //cout << "B->A: " << leavesAway(B, A) << endl;
    //cout << "B->E: " << leavesAway(B, E) << endl;

    //cout << "E->B: " << leavesAway(E, B) << endl;
}

HopefulEncoder::Node *HopefulEncoder::leafParent(Node *leaf, Dir &dir) {
    if (leaf == nullptr) {
        throw "\"leaf\" is a nullptr";
    } else if (leaf->value == INTERNAL || 
              (leaf->up == nullptr   ? 1 : 0) + 
              (leaf->left == nullptr ? 1 : 0) +
              (leaf->right == nullptr ? 1 : 0) != 1) {
        throw "\"leaf\" does not satisfy properties of leaf nodes";
    }
    if (leaf->up != nullptr) {
        dir = UP;
        return leaf->up;
    }
    if (leaf->left != nullptr) {
        dir = LEFT;
        return leaf->left;
    }
    dir = RIGHT;
    return leaf->right;
}

HopefulEncoder::

HopefulEncoder::Node *HopefulEncoder::largerSubtree(Node *node) {
    if (node == nullptr) throw "\"node\" is a nullptr";
    if (node->numLeavesUp > 128) return node->up;
    if (node->numLeavesLeft > 128) return node->left;
    if (node->numLeavesRight > 128) return node->right;
    return nullptr; // signals that we're sitting on the center node.
}

void HopefulEncoder::writeMoveBit(Node *prev, Node *finger, Node *next) {

    Dir toPrev; // will be set by if-else
    if (prev == finger->up) {
        toPrev = UP;
    } else if (prev == finger->left) {
        toPrev = LEFT;
    } else {
        toPrev = RIGHT;
    }

    if (next == finger->up) {
        writeBit(UP > (LEFT + RIGHT - toPrev));
    } else if (next == finger->left) {
        writeBit(LEFT > (UP + RIGHT - toPrev));
    } else if (next == finger->right) {
        writeBit(RIGHT > (UP + LEFT - toPrev));
    }
}

void HopefulEncoder::encode() {

    byte b;
    size_t val;
    bool more;

    // read a single byte and return if empty stream
    more = readByte(b);
    if (!more) return;

    unordered_map<size_t, Node*> leafMap;

    Node *up    = new Node(0);
    Node *left  = new Node(1);
    Node *right = new Node(2);
    Node *center = new Node(INTERNAL, up, left, right);
    up->up = center;
    left->up = center;
    right->up = center;

    queue<Node*> q;
    q.push(up);
    q.push(left);
    q.push(right);

    for (size_t leaves=3; leaves<257; leaves++) {
        Node *oldLeaf = q.front();
        q.pop();
        Node *newLeftLeaf  = new Node(oldLeaf->value, oldLeaf);
        Node *newRightLeaf = new Node(leaves, oldLeaf);
        oldLeaf->left  = newLeftLeaf;
        oldLeaf->right = newRightLeaf;
        oldLeaf->value = INTERNAL;
        q.push(newLeftLeaf);
        q.push(newRightLeaf);
    }


    sizesAndLeafMap(center, leafMap);

    // The first byte is sent without encoding.
    writeByte(b);

    // create a "finger" just above the leaf node of first byte
    Node *finger = leafMap[b];
    Node *lca = center;

    unordered_set<Node*> visited;

    while (more) {
       
        more = readByte(b);

        if (!more) {
            val = HE_PSEUDO_EOF;
        } else {
            val = b;
        }

        /*if (finger->value == val) {
            writeBit(BIT_0);
            continue;
        } else {
            writeBit(BIT_1);
        }*/

        stack<Node*> targetPath;
        queue<Node*> fingerPath;

        fingerPath.push(finger);

        Node *target = leafMap[val];
        targetPath.push(target);

        visited.clear();

        Node *midPoint = nullptr;

        while (midPoint == nullptr) {

            if (visited.find(target) != visited.end()) {
                midPoint = target;
            } else if(target != nullptr) {
                visited.insert(target);
            }

            if (visited.find(finger) != visited.end()) {
                midPoint = finger;
            } else if (finger != nullptr) {
                visited.insert(finger);
            }

            if (target != nullptr) { 
                target = largerSubtree(target);
            }
            targetPath.push(target);

            if (finger != nullptr) {
                finger = largerSubtree(finger);
            }
            fingerPath.push(finger);
        }

        queue<Node*> splayQueue;

        bool writePath = false;
        bool throughCenter = (largerSubtree(midPoint) == nullptr);

        Node *prev = fingerPath.front();
        fingerPath.pop();
        if (prev != midPoint) {
            finger = fingerPath.front();
            fingerPath.pop();
            splayQueue.push(prev);
            splayQueue.push(finger);
            if (finger == lca) {
                writePath = true;
                writeBit(BIT_1);
            }
            while (finger != midPoint) {
                Node *next = fingerPath.front();
                fingerPath.pop();
                splayQueue.push(next);
                if (writePath) {
                    writeMoveBit(prev, finger, next);
                }
                prev = finger;
                finger = next;
                if (finger == lca) {
                    writePath = true;
                    writeBit(BIT_1);
                }
                if (splayQueue.size() == 4) {
                    splayPath(splayQueue);
                }
            }
        }

        if (!writePath) {
            stack<Node*> lcaPath;
            Node *curr = midPoint;
            lcaPath.push(curr);
           //cout << "    " << lca->numLeavesUp << " " << lca->numLeavesLeft << " " << lca->numLeavesRight << endl;
            while (curr != lca) {
           //cout << curr->numLeavesUp << " " << curr->numLeavesLeft << " " << curr->numLeavesRight << endl;
                curr = largerSubtree(curr);
                lcaPath.push(curr);
            }

            writeBit(BIT_0);
            prev = lcaPath.top();
            lcaPath.pop();
            curr = lcaPath.top();
            lcaPath.pop();
            while (curr != midPoint) {
                Node *next = lcaPath.top();
                lcaPath.pop();
                writeMoveBit(prev, curr, next);
                prev = curr;
                curr = next;
            }
            writePath = true;
        }

        finger = targetPath.top();
        targetPath.pop();
        while (finger != midPoint) {
            finger = targetPath.top();
            targetPath.pop();
        }

        while (finger->value != val) {
            Node *next = targetPath.top();
            targetPath.pop();
            splayQueue.push(next);
            writeMoveBit(prev, finger, next);
            prev = finger;
            finger = next;
            if (splayQueue.size() == 4) {
                splayPath(splayQueue);
            }
        }

        lca = midPoint;
        if (throughCenter) {
            while (largerSubtree(lca) != nullptr) {
                lca = largerSubtree(lca);
            }
        } if (lca->value != INTERNAL) {
            lca = largerSubtree(lca);
        }
    }

    flushByte();

    freeMemory(nullptr, finger);
}

void HopefulEncoder::decode() {

}
