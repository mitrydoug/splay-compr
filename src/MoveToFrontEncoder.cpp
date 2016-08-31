#include <iostream>

#include "MoveToFrontEncoder.h"

using namespace std;

static const size_t CODE_SIZE = 256;
static const size_t USE_2_BITS = 1;
static const size_t USE_4_BITS = 7;
static const size_t USE_8_BITS = 127;
static const size_t TWO_BITS = 3;
static const size_t FOUR_BITS = 15;
static const size_t EIGHT_BITS = 255;
static const size_t TWO_BIT_OFFSET = 1;
static const size_t FOUR_BIT_OFFSET = 4;
static const size_t EIGHT_BIT_OFFSET = 19;


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
    return front;
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

byte MoveToFrontEncoder::getVal(
        MoveToFrontEncoder::Node *&front, size_t index) {

    if (index != 0) {
        Node *curr = front;
        for (size_t i = 0; i<index-1; i++) {
           curr = curr->next;
        }
        // splice the node out
        Node* temp = curr->next;
        curr->next = curr->next->next;
        temp->next = front;
        front = temp;
    }
    return front->val;
}

void MoveToFrontEncoder::encode() {

    Node *front = buildList();

    byte b;
    while (readByte(b)) {
        size_t index = getIndex(front, b);
        size_t bits=0;
        if (index == 0) {
            writeBit(BIT_0);
            continue;
        } else if (index <= 3) {
            index -= 1;
            bits=2;
            writeBit(BIT_1);
        } else if (index <= 18) {
            index -= 4;
            bits=4;
            writeBit(BIT_1);
            writeBit(BIT_1);
            writeBit(BIT_1);
        } else {
            index -= 19;
            bits=8;
            writeBit(BIT_1); writeBit(BIT_1);
            writeBit(BIT_1); writeBit(BIT_1);
            writeBit(BIT_1); writeBit(BIT_1);
            writeBit(BIT_1);
        }
        for(size_t b=0; b<bits; b++) {
            writeBit(index % 2 == 1);
            index /= 2;
        }
    }

    freeList(front);

    // finish the last byte with 1s
    flushByte(BIT_1);
}

void MoveToFrontEncoder::decode() {
    Node *front = buildList();

    size_t bitBuffer=0; 
    bit bt;
    bool more;
    bool oes=false;
    while (readBit(bt)) {
        if (oes) { cout << "bit" << endl; }
        bitBuffer = bt ? 1 : 0;
        if (bitBuffer != USE_2_BITS) {
           writeByte(getVal(front, 0));
           continue;
        }
        bitBuffer <<= 2;
        for (size_t c=0; c<2; c++) {
            more = readBit(bt);
            if (!more) {
                if (((bitBuffer + 1) >> c) % 2 != 1) {
                    throw "Ran out of bits";
                } else {
                    return;
                }
            }
            bitBuffer += bt ? (1 << c) : 0;
        }
        if (bitBuffer != USE_4_BITS) {
            writeByte(getVal(
                front, (TWO_BITS & bitBuffer) + TWO_BIT_OFFSET));
            continue;
        }
        bitBuffer <<= 4;
        for (size_t c=0; c<4; c++) {
            more = readBit(bt);
            if (!more) {
                if (((bitBuffer + 1) >> c) % 2 != 1) {
                    throw "Ran out of bits";
                } else {
                    return;
                }
            }
            bitBuffer += bt ? (1 << c) : 0;
        }
        if (bitBuffer != USE_8_BITS) {
            writeByte(getVal(
                front, (FOUR_BITS & bitBuffer) + FOUR_BIT_OFFSET));
            continue;
        }
        bitBuffer <<= 8;
        for (size_t c=0; c < 8; c++) {
            more = readBit(bt);
            if (!more) {
                if (((bitBuffer + 1) >> c) % 2 != 1) {
                    throw "Ran out of bits";
                } else {
                    return;
                }
            }
            bitBuffer += bt ? 1 << c : 0;
        }
        writeByte(getVal(
            front, (EIGHT_BITS & bitBuffer) + EIGHT_BIT_OFFSET));
    }

    freeList(front);
}
