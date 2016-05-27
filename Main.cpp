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
#include "PPMDelegateEncoder.h"
#include "RGBSplitEncoder.h"
#include "SpaceFillingCurveEncoder.h"
#include "MoveToFrontEncoder.h"

using namespace std;

typedef SeriesTransformer SS;
typedef ParallelTransformer PP;

/* The Main function of this test suite
 */
int main() {

    ifstream imgIn("hilbert.ppm", ios::binary);
    ofstream imgOut("hilbert_rbg_spc_mtf.ppm", ios::binary);
    RGBSplitEncoder rgbe(ENCODE);
    SpaceFillingCurveEncoder sfce(ENCODE);
    MoveToFrontEncoder mtfe(ENCODE);
    PPMDelegateEncoder ppmmtfe(ENCODE, &mtfe);
    try {
        SS(&rgbe, &sfce, &ppmmtfe).exec(&imgIn, &imgOut);
    } catch (const char *c) {
        cout << endl << c << endl; }

    imgIn.close();
    imgOut.close();

    /*VerbatimBitTransformer vbt;
    PrefixTransformer pt("Warning: ");
    ReverseTransformer rt;
    stringstream is("The nuclear core is about to melt down!");
    stringstream os;
    SS(&pt, &rt, &vbt).exec(&is, &os);
    cout << os.str() << endl;

    stringstream os2;
    stringstream is2("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabb");
    AlphaRunLengthEncoder arle(ENCODE);
    SS(&arle).exec(&is2, &os2);
    cout << os2.str() << endl;

    stringstream os3;
    stringstream is3("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabb");
    AlphaRunLengthEncoder arle2(ENCODE);
    AlphaRunLengthEncoder arld(DECODE);
    SS(&arle2, &arld).exec(&is3, &os3);
    cout << os3.str() << endl;

    cout << "Alpha Run Length Digest:\n" << arle2.getDigest() << endl;

    ifstream ppm("building.ppm", ios::binary);
    ofstream ppm_cpy("cpy_building.ppm", ios::binary);
    RGBSplitEncoder rgbe(ENCODE);
    RGBSplitEncoder rgbd(DECODE);
    SpaceFillingCurveEncoder sfce(ENCODE);
    SpaceFillingCurveEncoder sfcd(DECODE);
    try {
        SS(&rgbe, &sfce, &sfcd, &rgbd).exec(&ppm, &ppm_cpy);
    } catch (const char *c) {
        cout << endl << c << endl; }

    ppm.close();
    ppm_cpy.close();

    ifstream cpy_ppm("cpy_ryan.ppm", ios::binary);
    ofstream ppm_cpy_cpy("cpy_cpy_ryan.ppm", ios::binary);
    RGBSplitEncoder rgb(DECODE);
    SpaceFillingCurveEncoder sfc1(ENCODE);
    SpaceFillingCurveEncoder sfc2(ENCODE);
    SpaceFillingCurveEncoder sfc3(ENCODE);
    P sfcs(&sfc1, &sfc2, &sfc3);
    S rgbsfc(&rgb, &sfcs);
    try {
        PPMImageEncoder(DECODE, &rgbsfc).exec(&cpy_ppm, &ppm_cpy_cpy);
    } catch (const char *c) {
        cout << endl << c << endl; }

    return 0;*/
}
