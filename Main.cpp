#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
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
#include "SplayPrefixEncoder.h"
#include "MoveToFrontEncoder.h"

using namespace std;

typedef SeriesTransformer SS;
typedef ParallelTransformer PP;

static const vector<string> txtFiles({
    "google_home.txt"
});

static const vector<string> images({
    "ryan.ppm",
    "hilbert.ppm",
    "building.ppm",
    "forest.ppm"
});

bool compareStreams(istream &is1, istream &is2) {
    bool cont = true;
    int in1;
    int in2;
    while (cont) {
        in1 = is1.get();
        in2 = is2.get();
        if (in1 != in2) return false;
        if (in1 == -1) cont = false;
    }
    return true;
}

template<class DataEncoder>
void testCompressor(string encoderName) {

    RGBSplitEncoder rgbe(ENCODE);
    RGBSplitEncoder rgbd(DECODE);
    SpaceFillingCurveEncoder sfce(ENCODE);
    SpaceFillingCurveEncoder sfcd(DECODE);
    DataEncoder de(ENCODE);
    DataEncoder dd(DECODE);
    PPMDelegateEncoder ppme(ENCODE, &de);
    PPMDelegateEncoder ppmd(DECODE, &de);


    printf("%s\n--------------------------------------------------------------"
           "------------------", encoderName.c_str());

    for (string file : txtFiles) {//size_t file_num=0; file_num<NUM_FILES; file_num++) {
        cout << endl << "Testing file: " << file << endl;
        ifstream cmp(file.c_str());
        ifstream is(file.c_str());
        stringstream ss;
        try {
            SS(&de, &dd).exec(&is, &ss);
            TransformDigest edg = de.getDigest();
            TransformDigest ddg = dd.getDigest();
            bool succ = edg.bytesIn == ddg.bytesOut &&
                        compareStreams(cmp, ss);
            cout << edg << "Successful Decode: " << (succ ? "Yes!" : "No :(") << endl;
        } catch (const char *e) {
            cout << "An Exception Occurred: " << e << endl;
        }
    }

    for (string file : images) {
        cout << endl << "Testing image: " << file << endl;
        ifstream cmp(file.c_str());
        ifstream is(file.c_str());
        stringstream ss;
        try {
            SS(&rgbe, &sfce, &de, &dd, &sfcd, &rgbd).exec(&is, &ss);
            TransformDigest edg = de.getDigest();
            TransformDigest ddg = dd.getDigest();
            bool succ = edg.bytesIn == ddg.bytesOut &&
                        compareStreams(cmp, ss);
            cout << edg << "Successful Decode: " << (succ ? "Yes!" : "No :(") << endl;
        } catch (const char *e) {
            cout << "An Exception Occurred: " << e << endl;
        }
    }
}

/* The Main function of this test suite
 */
int main() {

    testCompressor<SplayFrefixEncoder>("SplayPrefixEncoder");
    exit(0);

    ifstream imgIn("building.ppm", ios::binary);
    ofstream imgOut("building_encoded.dat", ios::binary);
    RGBSplitEncoder rgbe(ENCODE);
    SpaceFillingCurveEncoder sfce(ENCODE);
    SplayPrefixEncoder sple(ENCODE);
    PPMDelegateEncoder ppme(ENCODE, &sple);
  //  try {
        SS(&rgbe, &sfce, &ppme).exec(&imgIn, &imgOut);
  //  } catch (const char *c) {
  //      cout << endl << c << endl; }

    imgIn.close();
    imgOut.close();

    ifstream imgIn2("ryan_encoded.dat", ios::binary);
    ofstream imgOut2("ryan_decoded.ppm", ios::binary);
    RGBSplitEncoder rgbd(DECODE);
    SpaceFillingCurveEncoder sfcd(DECODE);
    SplayPrefixEncoder spld(DECODE);
    PPMDelegateEncoder ppmd(DECODE, &spld);
  //  try {
        SS(&ppmd, &sfcd, &rgbd).exec(&imgIn2, &imgOut2);
  //  } catch (const char *c) {
  //      cout << endl << c << endl; }

    imgIn2.close();
    imgOut2.close();

    /*VerbatimBitTransformer vbt;
    PrefixTransformer pt("Warning: ");
    ReverseTransformer rt;
    SplayPrefixEncoder splayEnc(ENCODE);
    SplayPrefixEncoder splayDec(DECODE);
    stringstream is("The nuclear core is about to melt down!");
    stringstream os;
    SS(&splayEnc, &splayDec).exec(&is, &os);
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
