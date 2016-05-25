#include <iostream>
#include <fstream>
#include <sstream>
#include "VerbatimTransformer.h"
#include "VerbatimBitTransformer.h"
#include "PrefixTransformer.h"
#include "ReverseTransformer.h"
#include "AlphaRunLengthEncoder.h"

using namespace std;

/* The Main function of this test suite
 */
int main() {

    VerbatimBitTransformer vbt;
    PrefixTransformer pt("Warning: ");
    ReverseTransformer rt;
    stringstream is("The nuclear core is about to melt down!");
    stringstream os;
    connect(is, os, &pt, &rt, &vbt);
    cout << os.str() << endl;

    stringstream os2;
    stringstream is2("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabb");
    AlphaRunLengthEncoder arle(ENCODE);
    connect(is2, os2, &arle);
    cout << os2.str() << endl;

    stringstream os3;
    stringstream is3("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabb");
    AlphaRunLengthEncoder arle2(ENCODE);
    AlphaRunLengthEncoder arld(DECODE);
    connect(is3, os3, &arle2, &arld);
    cout << os3.str() << endl;

    cout << "Alpha Run Length Digest:\n" << arle2.getDigest() << endl;

    return 0;
}
