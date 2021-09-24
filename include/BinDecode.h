#ifndef BinDecode_H
#define BinDecode_H

//C,C++ dependencies
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <iostream>

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
        int n_boards;
        double Sample;
        double TimeBin;
        double Trigger;
        string Savename;

        int Decode(string filename, string savename, int limit);
        void CreateRootFile(string savename);
        void CloseRootFile();
        int ToRoot(int evn);
        int CreateTreeAndBranches(int b, int ch, int evn);

    private:

};

#endif
