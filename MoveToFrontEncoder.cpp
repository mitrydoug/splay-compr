#include <iostream>

#include "MoveToFrontEncoder.h"

using namespace std;

static const size_t CODE_SIZE = 256;

void MoveToFrontEncoder::freeList(MoveToFrontEncoder::Node *front) {
    while (front != nullptr) {
        Node *temp = front->next;
        delete front;
        front = temp;
    }
}

MoveToFrontEncoder::Node *MoveToFrontEncoder::buildList() {
    /* Initialize the list */
    Node *front = nullptr;
    for (size_t n=0; n<CODE_SIZE; n++) {
        front = new Node((byte)n, front);
    } 
    return nullptr;
}

size_t MoveToFrontEncoder::getIndex(
        MoveToFrontEncoder::Node *&front, byte b) {
    size_t index = 0;
    if (front->val != b) {
        index++;
        Node *curr = front;
        while (curr->next->val != b) {
            curr = curr->next;
            index++;
        }
        // splice the node out
        Node* temp = curr->next;
        curr->next = curr->next->next;
        temp->next = front;
        front = temp;
    }
    return index;
}

void MoveToFrontEncoder::encode() {

    Node *front = buildList();

    byte b;
    while (readByte(b)) {
        size_t index = getIndex(front, b);
        if (index == 0) {
            writeBit(BIT_0);
        } else if (index <= 3) {
            index -= 1;
            writeBit(BIT_1);
        } else if (index <= 18) {
            index -= 4;
            writeBit(BIT_1);
            writeBit(BIT_1);
            writeBit(BIT_1);
        } else {
            index -= 19;
            writeBit(BIT_1); writeBit(BIT_1);
            writeBit(BIT_1); writeBit(BIT_1);
            writeBit(BIT_1); writeBit(BIT_1);
            writeBit(BIT_1);
        }
        while (index > 0) {
            writeBit(index % 2 == 0);
            index /= 2;
        }
    }

    // finish the last byte with 1s
    flushByte(BIT_1);
}

void MoveToFrontEncoder::decode() {
    //Node *front = buildList();

//    byte by;
    bit bt;
    while (readBit(bt)) {
        
    }
}







