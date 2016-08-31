
#include <cmath>
#include <iostream>

#include "SpaceFillingCurveEncoder.h"

using namespace std;

void SpaceFillingCurveEncoder::readGrid(
        vector<vector<byte>> &grid) {
    bool more;
    byte by;
    for (size_t r=0; r<imageHeight; r++) {
        grid.emplace_back();
        for(size_t c=0; c<imageWidth; c++) {
            more = readByte(by);
            if (!more) {
                throw "Ran out of bytes.";
            }
            grid.back().push_back(by);
        }
    }
}

void SpaceFillingCurveEncoder::writeGrid(
        vector<vector<byte>> &grid) {
    for (size_t r=0; r<imageHeight; r++) {
        for(size_t c=0; c<imageWidth; c++) {
            writeByte(grid[r][c]);
        }
    }
}

static inline void s(size_t *sym) {
    size_t t = sym[1];
    sym[1] = sym[3];
    sym[3] = t;
}

static inline void r2s(size_t *sym) {
    size_t t = sym[0];
    sym[0] = sym[2];
    sym[2] = t;
}

void SpaceFillingCurveEncoder::hilbertRecurse(
        vector<vector<byte>> &grid, size_t *sym,
        double sizeX, double sizeY,
        double posX, double posY) {

    if (sizeX < 0.5 && sizeY < 0.5 &&
        posX - floor(posX) < sizeX &&
        posY - floor(posY) < sizeY) {
        if (encodeOrDecode == ENCODE) { 
            writeByte(grid[(size_t)floor(posY)]
                          [(size_t)floor(posX)]);
        } else { // DECODE
            byte by;
            bool more;
            more = readByte(by);
            if (!more) {
                throw "Not enough bytes";
            }
            grid[(size_t)floor(posY)][(size_t)floor(posX)] = by;
        }
        return;
    }
    
    if (sizeX < 0.5 && sizeY < 0.5) {
        return;
    }

    double newXSize = sizeX / 2.0;
    double newYSize = sizeY / 2.0;

    for (size_t i=0; i<4; i++) {

        double dx = sym[i] == 2 || sym[i] == 3 ? 0 : newXSize;
        double dy = sym[i] == 1 || sym[i] == 2 ? 0 : newYSize;

        if (i == 0) {
            s(sym);
        } else if (i == 3) {
            r2s(sym);
        }
        
        hilbertRecurse(grid, sym, newXSize, newYSize, posX + dx, posY + dy);

        if (i == 0) {
            s(sym);
        } else if (i == 3) {
            r2s(sym);
        }
    }
}

void SpaceFillingCurveEncoder::hilbert(
        vector<vector<byte>> &grid) {

    size_t sym[4] = {3, 2, 1, 4}; // order of hilbert curve

    if (imageWidth >= imageHeight) {
        size_t factor = imageWidth / imageHeight;
        double split = imageWidth / (double) factor;
        for (size_t f=0; f<factor; f++) {
            hilbertRecurse(grid, sym, split, imageHeight, f*split, 0.0);
        }
    } else {
        size_t factor = imageHeight / imageWidth;
        double split = imageHeight / (double) factor;
        for (size_t f=0; f<factor; f++) {
            hilbertRecurse(grid, sym, imageWidth, split, 0.0, f*split);
        }
    }
}

void SpaceFillingCurveEncoder::encode() {

    readHeader();

    for (size_t i=0; /* RGB */ i<3; i++) {
        vector<vector<byte>> grid;
        readGrid(grid);
        hilbert(grid);
    } 
}

void SpaceFillingCurveEncoder::decode() {

    readHeader();

    for (size_t i=0; /* RGB */ i<3; i++) {
        vector<vector<byte>> grid(imageHeight, vector<byte>(imageWidth));
        hilbert(grid);
        writeGrid(grid);
    }
}
