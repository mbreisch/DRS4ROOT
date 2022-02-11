#ifndef BinDecode_H
#define BinDecode_H

//C,C++ dependencies
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

//Root dependencies
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;

typedef struct 
{
    char           tag[3];
    char           version;
} FHEADER;

typedef struct 
{
    char           time_header[4];
} THEADER;

typedef struct 
{
    char           bn[2];
    unsigned short board_serial_number;
} BHEADER;

typedef struct 
{
    char           event_header[4];
    unsigned int   event_serial_number;
    unsigned short year;
    unsigned short month;
    unsigned short day;
    unsigned short hour;
    unsigned short minute;
    unsigned short second;
    unsigned short millisecond;
    unsigned short range;
} EHEADER;

typedef struct 
{
    char           tc[2];
    unsigned short trigger_cell;
} TCHEADER;

typedef struct 
{
    char           c[1];
    char           cn[3];
} CHEADER;

class BinDecode
{
    public:
        BinDecode();

        TFile *RFile;
        double waveform[16][4][1024];
        double time[16][4][1024];
        double time_uncorr[16][4][1024];
        unsigned int scaler, Scaler;
        int n_boards;
        double Sample;
        double TimeBin,TimeBinUncorr;
        double Trigger;
        string Savename;
        vector<int> activeChannels;
        unsigned short Triggercell, glob_triggercell[2];
        unsigned short Timestamp_y, glob_timestamp_y[2];
        unsigned short Timestamp_m, glob_timestamp_m[2];
        unsigned short Timestamp_d, glob_timestamp_d[2];
        unsigned short Timestamp_hh, glob_timestamp_hh[2];
        unsigned short Timestamp_mm, glob_timestamp_mm[2];
        unsigned short Timestamp_ss, glob_timestamp_ss[2];
        unsigned short Timestamp_mils, glob_timestamp_mils[2];
        unsigned short Range, glob_range[2];
        vector<double*> MinV;

        int Decode(string filename, string savename, int limit);
        void CreateRootFile(string savename);
        void CloseRootFile(int n_evn);
        int ToRoot(int evn);
        int CreateTreeAndBranches(int b, int ch, int evn);

    private:

};

#endif
