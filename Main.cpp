#include <iostream>
#include <fstream>
#include <sstream>
#include "SeriesTransformer.h"
#include "ParallelTransformer.h"
#include "VerbatimTransformer.h"
#include "VerbatimBitTransformer.h"
#include "PrefixTransformer.h"
#include "ReverseTransformer.h"
#include "AlphaRunLengthEncoder.h"
#include "PPMImageEncoder.h"
#include "RGBSplitEncoder.h"

using namespace std;

typedef SeriesTransformer S;
typedef ParallelTransformer P;

/* The Main function of this test suite
 */
int main() {

    VerbatimBitTransformer vbt;
    PrefixTransformer pt("Warning: ");
    ReverseTransformer rt;
    stringstream is("The nuclear core is about to melt down!");
    stringstream os;
    S(&pt, &rt, &vbt).exec(&is, &os);
    cout << os.str() << endl;

    stringstream os2;
    stringstream is2("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabb");
    AlphaRunLengthEncoder arle(ENCODE);
    S(&arle).exec(&is2, &os2);
    cout << os2.str() << endl;

    stringstream os3;
    stringstream is3("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabb");
    AlphaRunLengthEncoder arle2(ENCODE);
    AlphaRunLengthEncoder arld(DECODE);
    S(&arle2, &arld).exec(&is3, &os3);
    cout << os3.str() << endl;

    cout << "Alpha Run Length Digest:\n" << arle2.getDigest() << endl;

    ifstream ppm("ryan.ppm", ios::binary);
    ofstream ppm_cpy("cpy_ryan.ppm", ios::binary);
    RGBSplitEncoder rgb(ENCODE);
    PPMImageEncoder(ENCODE, &rgb).exec(&ppm, &ppm_cpy);

    ppm.close();
    ppm_cpy.close();

    ifstream cpy_ppm("cpy_ryan.ppm", ios::binary);
    ofstream ppm_cpy_cpy("cpy_cpy_ryan.ppm", ios::binary);
    RGBSplitEncoder rgb2(DECODE);
    try {
        PPMImageEncoder(DECODE, &rgb2).exec(&cpy_ppm, &ppm_cpy_cpy);
    } catch (const char *c) {
        cout << endl << c << endl; }

    return 0;
}
