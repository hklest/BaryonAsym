///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
// Creating an eventlist by entering events one by one
// and write it to a file afterwards.
// Also writes a separate file of various Event Distributions.
//
// Author     : Gage Tustin
///////////////////////////////////////////////////////

#include <stdlib.h>
#include <iostream>
#include "H1Steering/H1Steer.h"
#include "TVector3.h"
#include "TRotation.h"
#include "TLorentzVector.h"
#include <TArrayD.h>

// ROOT includes
#include <TFile.h>
#include <TROOT.h>
#include <THStack.h>
#include <TH1.h>
#include <TH2.h>
#include <TClonesArray.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TDatabasePDG.h>
#include <TApplication.h>
#include <TMultiLayerPerceptron.h>
#include <TSynapse.h>
#include <TObject.h>
#include <TString.h>
#include <TObjArray.h>
#include <TMatrixD.h>
#include <TNeuron.h>
#include <TMath.h>
#include <TF1.h>
#include <TLegend.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include "TEllipse.h"

// H1 OO includes
#include "H1Skeleton/H1Tree.h"
#include "H1Skeleton/H1EventList.h"
#include "H1Pointers/H1FloatPtr.h"
#include "H1Pointers/H1IntPtr.h"
#include "H1Steering/H1StdCmdLine.h"
#include "H1Mods/H1PartEmArrayPtr.h"
#include "H1Mods/H1PartEm.h"
#include "H1Tools/H1RunList.h"
#include "H1Tools/H1LumiYield.h"
#include "H1Geom/H1DetectorStatus.h"

#include "H1Clusters/H1Cell.h"
#include "H1Geom/H1CellGeometry.h"
#include "H1Geom/H1SpaCalCellGeo.h"
#include "H1Geom/H1CaloGeometry.h"
#include "H1Geom/H1SpacalGeometry.h"
#include "H1Geom/H1DBManager.h"
#include "H1Calculator/H1CalcGenericInterface.h"


#include "H1Tools/H1LumiCalculator.h"
#include "H1Calculator/H1Calculator.h"
#include "H1Calculator/H1CalcTrig.h"
#include "H1Calculator/H1CalcWeight.h"
#include "H1Calculator/H1CalcVertex.h"
#include "H1Calculator/H1CalcEvent.h"
#include "H1Calculator/H1CalcKine.h"
#include "H1Calculator/H1CalcElec.h"
#include "H1Calculator/H1CalcFs.h"
#include "H1Calculator/H1CalcHad.h"
#include "H1Calculator/H1CalcTrack.h"
#include "H1Calculator/H1CalcBgTiming.h"
#include "H1Calculator/H1CalcSystematic.h"

// Electron Cut includes from Daniel's low Q2 jet analysis
// Modular fomrat was not working, and some functions were not defined in reference directory for some reason,
// So I will explicitly include necessary functions
//#include "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/MyCalibCuts.h"
//#include "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/MyElec.h"			// Outer cut oin SpaCal Radius = 74cm (ELAN)
//#include "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/elecCut.h"
//#include "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/SpacLinearity.h"
//#include "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/Alignment.h"
using namespace std;

//Define some variables
Double_t fCutR;
int fMyRunYear;
Int_t fElecID;
Double_t fElecE;     // energy of scattered electron: HAT
TVector3 fposE;// re-aligne electron position
TLorentzVector fElecBPCHlx, fElecBPCH1, fElecSpacAlign;
//TLorentzVector GetHelixMom(Double_t e, const TVector3& posrel, Int_t charge) const;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//define Alignment.h
//#ifndef H_ALIGNMENT
//#define H_ALIGNMENT

#include "H1Steering/H1Steer.h"
#include "TVector3.h"
#include "TRotation.h"
#include "TLorentzVector.h"

using namespace std;


TVector3 fOrigin;
Double_t fPhi;
Double_t fOctant;
Double_t fZ0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//define Alignment.C

void Alignment() {
    //initialize all Alignment variables
    fPhi = 0.0;
    fOctant = 0.0;
    fZ0 = 0.0;
    fOrigin.SetXYZ(0, 0, 0);
    //return true;
}


//void SetOrigin(const char* in) {
//    TArrayD tmp;
//    StringToArray(in, tmp);
//    for (int i = 0; (i < 3) && (i < tmp.GetSize()); i++) {
//        fOrigin[i] = tmp[i];
//    }
//}

void SetOrigin(Float_t tmp[3]) {
    //TArrayD tmp;
    //StringToArray(in, tmp);
    for (int i = 0; i < 3; i++) {
        fOrigin[i] = tmp[i];
    }
}

void SetPhi(Double_t p) {
    fPhi = p;
}

void SetOctant(Double_t o) {
    fOctant = o;
}

void SetZ0(Double_t z0) {
    fZ0 = z0;
}

//void ImportParameters(Alignment const* a) {
//    fOrigin = a->fOrigin;
//    fPhi = a->fPhi;
//    fOctant = a->fOctant;
//    fZ0 = a->fZ0;
//}

//TVector3 Transform(TVector3 const& x) const {
TVector3 Transform(TVector3 const& x) {
#ifdef PROFILE
    Profile::start("Alignment::Transform");
#endif
    TVector3 r = x;
    Double_t phi0 = r.Phi();
    r.SetPhi(phi0 + fPhi + fOctant * fmod(phi0 * 180. / M_PI + 180., 45.));
    r += fOrigin;
#ifdef PROFILE
    Profile::stop("Alignment::Transform");
#endif
    return r;
}

//void PrintSteer(ostream& out) const {
//    out << "Alignment(\"" << GetName() << "\") {\n";
//    if (fPhi != 0.0) out << "  fPhi=" << fPhi << ";\n";
//    if (fOctant != 0.0) out << "  fOctant=" << fOctant << ";\n";
//    if ((fOrigin[0] != 0.0) || (fOrigin[1] != 0.0) || (fOrigin[2] != 0.0)) {
//        out << "  fOrigin=\"" << fOrigin[0] << "," << fOrigin[1] << "," << fOrigin[2] << "\";\n";
//    }
//    if (fZ0 != 0.0) {
//        out << "  fZ0=" << fZ0 << ";\n";
//    }
//    out << "}\n";
//}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//define SpacLinearity.h
//#ifndef H_SPACLINEARITY
//#define H_SPACLINEARITY
//
//#include <TVector3.h>
//#include "H1Steering/H1Steer.h"
//
//class SpacLinearity : public H1Steer {
//public:
//    SpacLinearity(void);
//    void SetAlpha(const char* in);
//    void SetSuperX(const char* in);
//    void SetSuperY(const char* in);
//    void SetSuperXY(int i, double dx, double dy);
//    void SetAlphaByIndex(int i, Float_t a);
//    inline Float_t GetAlpha(int k) const { return fAlpha[k]; }
//    inline Float_t GetSuperX(int i) const { return fSuperX[i]; }
//    inline Float_t GetSuperY(int i) const { return fSuperY[i]; }
//    TVector3 GetPosCorr(TVector3 const& pos,
//        Float_t const* grid, Float_t const* offset) const;
//    void PrintSteer(ostream& out, bool forcePrint = false) const;
//protected:
//    static void PrintN(ostream& out, char const* name, Float_t const* f, int n,
//        int npl = 0);
//    mutable bool alreadyPrinted;
//    Float_t fAlpha[5];
//    Float_t fSuperX[81];
//    Float_t fSuperY[81];
//    ClassDef(SpacLinearity, 0)
//};
//
//#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SpacLinearity.C
//ClassImp(SpacLinearity)

bool alreadyPrinted;
Float_t fAlpha[5];
Float_t fSuperX[81];
Float_t fSuperY[81];

void SpacLinearity() {
    //initialize variables
    alreadyPrinted = false;
    for (int i = 0; i < 5; i++) {
        fAlpha[i] = 0.0;
    }
    for (int i = 0; i < 81; i++) {
        fSuperX[i] = 0.;
        fSuperY[i] = 0.;
    }
}

//void SetAlpha(const char* in) {
//    TArrayD tmp;
//    StringToArray(in, tmp);
//    for (int i = 0; (i < 5) && (i < tmp.GetSize()); i++) {
//        fAlpha[i] = tmp[i];
//    }
//}

void SetAlpha(Float_t tmp[5]) {
    //TArrayD tmp;
    //StringToArray(in, tmp);
    for (int i = 0; (i < 5) && (i < (sizeof(tmp) / sizeof(Float_t))); i++) {
        fAlpha[i] = tmp[i];
    }
}


//void SetSuperX(const char* in) {
//    TArrayD tmp;
//    StringToArray(in, tmp);
//    for (int i = 0; i < 81; i++) {
//        fSuperX[i] = tmp[i];
//    }
//}

void SetSuperX(Float_t tmp[81]) {
    //TArrayD tmp;
    //StringToArray(in, tmp);
    for (int i = 0; i < 81; i++) {
        fSuperX[i] = tmp[i];
    }
}

//void SetSuperY(const char* in) {
//    TArrayD tmp;
//    StringToArray(in, tmp);
//    for (int i = 0; i < 81; i++) {
//        fSuperY[i] = tmp[i];
//    }
//}

void SetSuperY(Float_t tmp[81]) {
    //TArrayD tmp;
    //StringToArray(in, tmp);
    for (int i = 0; i < 81; i++) {
        fSuperY[i] = tmp[i];
    }
}


void SetSuperXY(int i, double x, double y) {
    fSuperX[i] = x;
    fSuperY[i] = y;
}

void SetAlphaByIndex(int i, Float_t a) {
    fAlpha[i] = a;
}

TVector3 GetPosCorr(TVector3 const& pos, Float_t const* grid,
    Float_t const* offset) {
    TVector3 r(pos);
    // cell-by-cell linearity correction
    double rr = r.Pt();
    for (int i = 0; i < 2; i++) {
        double arg = (r[i] - offset[i]) / grid[i] * 2. * M_PI;
        r[i] += fAlpha[0] * (1. + fAlpha[1] * rr + fAlpha[2] * fabs(r[i]))
            * (sin(arg) + fAlpha[3] * sin(2. * arg) + fAlpha[4] * sin(3. * arg));
    }

    // supermodule position correction

    // supermodule number
    int isup[2][2];
    for (int ic = 0; ic < 2; ic++) {
        for (int ix = 0; ix < 2; ix++) {
            isup[ic][ix] = (int)(pos[ic] / grid[ic] / 4. + 4. + ix);
            if (isup[ic][ix] < 0) isup[ic][ix] = 0;
            if (isup[ic][ix] > 8) isup[ic][ix] = 8;
        }
    }
    // correct by linear interpolation along both directions
    double f[2];
    for (int ic = 0; ic < 2; ic++) {
        f[ic] = pos[ic] / grid[ic] / 4. + 4. - isup[ic][0];
    }

    r[0] +=
        (fSuperX[isup[0][0] + 9 * isup[1][0]] * (1. - f[0]) +
            fSuperX[isup[0][1] + 9 * isup[1][0]] * (f[0])) * (1. - f[1]) +
        (fSuperX[isup[0][0] + 9 * isup[1][1]] * (1. - f[0]) +
            fSuperX[isup[0][1] + 9 * isup[1][1]] * (f[0])) * f[1];
    r[1] +=
        (fSuperY[isup[0][0] + 9 * isup[1][0]] * (1. - f[0]) +
            fSuperY[isup[0][1] + 9 * isup[1][0]] * (f[0])) * (1. - f[1]) +
        (fSuperY[isup[0][0] + 9 * isup[1][1]] * (1. - f[0]) +
            fSuperY[isup[0][1] + 9 * isup[1][1]] * (f[0])) * f[1];

    return r;
}

//void PrintSteer(ostream& out, bool forcePrint) const {
//    if (forcePrint || !alreadyPrinted) {
//        out << "SpacLinearity(\"" << GetName() << "\") {\n";
//        PrintN(out, "Alpha", fAlpha, 5);
//        PrintN(out, "SuperX", fSuperX, 81, 9);
//        PrintN(out, "SuperY", fSuperY, 81, 9);
//        out << "}\n\n";
//        alreadyPrinted = true;
//    }
//}

void PrintN(ostream& out, char const* name, Float_t const* f,
    int n, int npl) {
    out << "  f" << name << "=\"";
    for (int i = 0; i < n; i++) {
        out << (f[i]);
        if (i != (n - 1)) {
            out << ",";
            if (npl && ((i + 1) % npl == 0)) out << "\n     ";
        }
    }
    out << "\";\n";
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//define elecCut class
#ifndef _ELECCUT_H_
#define _ELECCUT_H_

#define MAX_ENERGY_FRAC_HAD    0.15 
#define MAX_ECRA               4     //energy weihted cluster radius 
#define MAX_BPC_DIST           6   // distance to BPC-Hit in cm, to be checked!!!  

//inner radius cut: 
#define RadCut 12  

#define DeadCellCut        3.025  
#define DeadTriggCellCut   2.035
#define MAX_DEAD_CELLS 50 
#define MAX_DEAD_TRIG_CELLS 50
#define MAX_CIRC_CUTS 10 

class elecCut {// : public TObject { 
private:
    int dbg;


    Int_t deadCells[MAX_DEAD_CELLS];
    Int_t deadTrigCells[MAX_DEAD_TRIG_CELLS];
    Double_t posDeadCellsX[MAX_DEAD_CELLS];
    Double_t posDeadCellsY[MAX_DEAD_CELLS];
    Double_t posDeadTrigCellsX[MAX_DEAD_TRIG_CELLS];
    Double_t posDeadTrigCellsY[MAX_DEAD_TRIG_CELLS];

    Int_t nCircularCuts;
    Double_t circCutsRadius[MAX_CIRC_CUTS];
    Double_t circCutsXzero[MAX_CIRC_CUTS];
    Double_t circCutsYzero[MAX_CIRC_CUTS];

    //initialise the cuts needed for the different run periods: 
    int initElecCut(int RunNumber_);
    void clearDeadCellPositions();
    void fillDeadCellPositions();

    //make the actual cuts: 
    int goodElec_TrackCond(H1PartEm* elec);
    int elecCutSpatial(Float_t x, Float_t y);
    int elecCutHadrEnergy(H1PartEm* elec);
    int elecCutTrack(H1PartEm* elec);
    int goodElec_noSpatialCuts(H1PartEm* elec);

    Int_t RunNumber;
    Int_t RunRange;
    Int_t nDeadCells;
    Int_t nDeadTrigCells;

public:
    elecCut(int RunNumber_);
    elecCut();
    ~elecCut() {};

    void initCuts2004();
    void initCuts2005();
    void initCuts2006();
    void initCuts2007();

    //  int initElecCut(int RunNumber); 
    int goodElec(H1PartEm* elec, Int_t RunNumber_);
    int goodElec_TrackCond(H1PartEm* elec, Int_t RunNumber_);
    int goodElec_noSpatialCuts(H1PartEm* elec, Int_t RunNumber_);
    int elecCutTrack(H1PartEm* elec, Int_t RunNumber_);
    void DrawCuts(Int_t RunNumber_);
    int isInDeadCellArray(H1PartEm* elec, Int_t i);

    //ClassDef(elecCut,1) 

};
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//elecCut class functions
static H1FloatPtr ElecTheta("ElecTheta");      // theta of scattered electron
static H1ShortPtr RunType("RunType"); //Q2 variable aquired using sigma method
//ClassImp(elecCut) 

/***************************************************************
 * initElecCut --
 * for MC a run number must be provided in order to set the
 * run range of the mc.
 */
elecCut::elecCut(int RunNumber_) {
    dbg = kTRUE;
    nDeadCells = 0;
    nDeadTrigCells = 0;
    nCircularCuts = 0;

    RunNumber = 0;
    RunRange = 0;

    //std circle (always present): 
    if (dbg) printf("elecCut::elecCut Initialising std. circular cut\n");
    nCircularCuts = 1;
    circCutsRadius[0] = 12;
    circCutsXzero[0] = -2.025;
    circCutsYzero[0] = 0;

    if (RunNumber_ != 0) {
        printf("elecCut::elecCut: Initialising Cuts for given runNumber=%d\n", RunNumber);
        initElecCut(RunNumber_);
    }
    dbg = kFALSE;
}

/***************************************************************
 * initElecCut --
 * for MC a run number must be provided in order to set the
 * run range of the mc.
 */
elecCut::elecCut() {
    dbg = kTRUE;
    nDeadCells = 0;
    nDeadTrigCells = 0;
    nCircularCuts = 0;

    RunNumber = 0;
    RunRange = 0;

    //std circle (always present): 
    if (dbg) printf("elecCut::elecCut Initialising std. circular cut\n");
    nCircularCuts = 1;
    circCutsRadius[0] = 12;
    circCutsXzero[0] = -2.025;
    circCutsYzero[0] = 0;

    dbg = kFALSE;
}

/*****************************************************************
 * initElecCut -- initialise the box cuts according to the given
 * run number. At the moment three different run periods are used
 * which have different initialsing functions. These function are
 * from here according to the given run number.
 ****************************************************************/
int elecCut::initElecCut(int RunNumber_) {
    int newRunRange = 0;

    if (RunNumber_ != RunNumber) {
        if (dbg) printf(" elecCut::initElecCut: RunNumber has changed from %d to %d check if SpaCal-Cuts needs to be changed\n", RunNumber, RunNumber_);

        // if(RunNumber_ >=367257 && RunNumber_ <= 398818)  //2004     (e+ running)  (2004 until end: 398818)
        //   newRunRange=2004 ;
        // else 
        if (RunNumber_ > 398286 && RunNumber_ <= 436893) //2004-2005 (e- running)
            newRunRange = 2005;
        else if (RunNumber_ > 436893 && RunNumber_ <= 492558)  //2006: 466977
            newRunRange = 2006;
        else if (RunNumber_ > 492558 && RunNumber_ <= 500611) {//2007:
            newRunRange = 2007;
            //      	printf("RunRange 2006b-2007 requested. Not analysed yet. Using Spacal Cuts of 2006 instead.\n"); 
        }
        else {
            printf(" elecCut::initElecCut: -- WARNING -- Run %d is in an unknown RunRange. Using current RunRange %d instead\n", RunNumber_, RunRange);
            newRunRange = RunRange;
        }

        if (newRunRange != RunRange) {
            RunNumber = RunNumber_;
            RunRange = newRunRange;
            if (dbg) printf(" elecCut::initElecCut: RunNumber has changed and RunRange also -> Initialising new Cuts for the SpaCal\n");
            if (dbg) printf(" elecCut::initElecCut: Initilising SpaCalCut for the year %d\n", newRunRange);
            clearDeadCellPositions();
            if (newRunRange == 2004)  initCuts2004();
            else if (newRunRange == 2005)  initCuts2005();
            else if (newRunRange == 2006)  initCuts2006();
            else if (newRunRange == 2007)  initCuts2007();
            else printf("elecCut::initElecCut: -- WARNING -- RunRange %d is not known. NO CUTS initialised\n", newRunRange);
            RunRange = newRunRange;
        }
        RunNumber = RunNumber_;
    }
    //}
    return 1;
}


/*****************************************************************
 * initCuts2004 -- box cuts for the year 2004
 *
 *
 ****************************************************************/
void elecCut::initCuts2004() {

    printf("elecCut::elecCut: Initialising deadSpacal and TriggerCells for year 2004\n");

    clearDeadCellPositions();

    nDeadCells = 6;
    nDeadTrigCells = 13;

    deadCells[0] = 487;
    deadCells[1] = 128;
    deadCells[2] = 403;
    deadCells[3] = 197;
    deadCells[4] = 306;
    deadCells[5] = 763;

    //initialise array of dead trigger cells: 
    deadTrigCells[0] = 22;
    deadTrigCells[1] = 23;
    deadTrigCells[2] = 45;
    deadTrigCells[3] = 46;
    deadTrigCells[4] = 59;
    deadTrigCells[5] = 60;
    deadTrigCells[6] = 66;
    deadTrigCells[7] = 67;
    deadTrigCells[8] = 94;
    deadTrigCells[9] = 95;
    deadTrigCells[10] = 103;
    deadTrigCells[11] = 104;
    deadTrigCells[12] = 89;

    //circular cuts: 
    printf("Implementing an additional circ. cut (before ncuts=%d) \n", nCircularCuts);
    nCircularCuts++;
    circCutsRadius[nCircularCuts - 1] = 10.4;
    circCutsXzero[nCircularCuts - 1] = 0.260701;
    circCutsYzero[nCircularCuts - 1] = 3.15767;

    nCircularCuts++;
    circCutsRadius[nCircularCuts - 1] = 7.87295;
    circCutsXzero[nCircularCuts - 1] = 1.63656;
    circCutsYzero[nCircularCuts - 1] = -4.20146;

    printf("There are now %d circ. cuts implemented\n", nCircularCuts);

    H1DBManager::Instance()->StartRun(367258);
    fillDeadCellPositions();
    RunRange = 2004;
    return;
}

/*****************************************************************
 * initCuts2005 -- box cuts for the year 2005
 *
 *
 ****************************************************************/
void elecCut::initCuts2005() {
    dbg = kFALSE;

    clearDeadCellPositions();

    printf("elecCut::elecCut: Initialising deadSpacal and TriggerCells for year 2005\n");

    nDeadCells = 11;
    nDeadTrigCells = 17;

    //initialise array of dead cells: 
    deadCells[0] = 75;
    deadCells[1] = 76;
    deadCells[2] = 223;
    deadCells[3] = 306;
    deadCells[4] = 190;
    deadCells[5] = 258;
    deadCells[6] = 403;
    deadCells[7] = 371;
    deadCells[8] = 1010;
    deadCells[9] = 915;
    deadCells[10] = 789;

    deadTrigCells[0] = 22;
    deadTrigCells[1] = 23;
    deadTrigCells[2] = 45;
    deadTrigCells[3] = 46;
    deadTrigCells[4] = 44;
    deadTrigCells[5] = 59;
    deadTrigCells[6] = 60;
    deadTrigCells[7] = 94;
    deadTrigCells[8] = 95;

    deadTrigCells[9] = 590;
    deadTrigCells[10] = 591;
    deadTrigCells[11] = 691;
    deadTrigCells[12] = 692;
    deadTrigCells[13] = 763;

    deadTrigCells[14] = 241;

    deadTrigCells[15] = 644;
    deadTrigCells[16] = 547;

    //circular cuts: 
    printf("Implementing an additional circ. cut (before ncuts=%d) \n", nCircularCuts);
    nCircularCuts++;
    circCutsRadius[nCircularCuts - 1] = 10.2;
    circCutsXzero[nCircularCuts - 1] = 0.258829;
    circCutsYzero[nCircularCuts - 1] = 2.09418;

    nCircularCuts++;
    circCutsRadius[nCircularCuts - 1] = 7.7;
    circCutsXzero[nCircularCuts - 1] = 2.46162;
    circCutsYzero[nCircularCuts - 1] = -2.89197;

    printf("There are now %d circ. cuts implemented\n", nCircularCuts);


    H1DBManager::Instance()->StartRun(398820);
    fillDeadCellPositions();
    RunRange = 2005;
    return;
}

/*****************************************************************
 * initCuts2006 -- box cuts for the year 2006
 *
 *
 ****************************************************************/
void elecCut::initCuts2006() {
    printf("elecCut::elecCut: Initialising deadSpacal and TriggerCells for year 2006\n");

    clearDeadCellPositions();

    nDeadCells = 2;
    deadCells[0] = 403;
    deadCells[1] = 297;

    nDeadTrigCells = 4;
    deadTrigCells[0] = 22;
    deadTrigCells[1] = 23;
    deadTrigCells[2] = 45;
    deadTrigCells[3] = 46;


    //deadTrigCells[0] = 6; 
    //deadTrigCells[1] = 9; 
    //deadTrigCells[2] = 18; 
    //deadTrigCells[3] = 19;
    //deadTrigCells[6] = 26;
    //deadTrigCells[7] = 27;
    //deadTrigCells[10] = 21 ;
    //deadTrigCells[11] = 24 ;
    //deadTrigCells[10] = 59 ;
    //deadTrigCells[11] = 60 ;
    //deadTrigCells[14] = 76 ;
    //deadTrigCells[15] = 77 ;
    //deadTrigCells[16] = 94 ;
    //deadTrigCells[17] = 95 ;

    H1DBManager::Instance()->StartRun(436894);
    fillDeadCellPositions();


    //circular cuts:
    printf("Implementing an additional circ. cut (before ncuts=%d) \n", nCircularCuts);
    nCircularCuts++;
    circCutsRadius[nCircularCuts - 1] = 10.4;
    circCutsXzero[nCircularCuts - 1] = 0.337138;
    circCutsYzero[nCircularCuts - 1] = 2.01292;

    nCircularCuts++;
    circCutsRadius[nCircularCuts - 1] = 7.26146;
    circCutsXzero[nCircularCuts - 1] = 2.47736;
    circCutsYzero[nCircularCuts - 1] = -4.52853;

    printf("There are now %d circ. cuts implemented\n", nCircularCuts);
    RunRange = 2006;

    return;
}

/*****************************************************************
 * initCuts2007 -- box cuts for the year 2007
 * - upt to now 12.09.07 the code is a copy of
 * the one from 2006 !!!
 ****************************************************************/
void elecCut::initCuts2007() {
    printf("elecCut::elecCut: Initialising deadSpacal and TriggerCells for year 2007 - be aware this is just a copy of 2006-cuts! \n");

    clearDeadCellPositions();

    nDeadCells = 2;
    deadCells[0] = 403;
    deadCells[1] = 297;

    nDeadTrigCells = 4;
    deadTrigCells[0] = 22;
    deadTrigCells[1] = 23;
    deadTrigCells[2] = 45;
    deadTrigCells[3] = 46;


    //deadTrigCells[0] = 6; 
    //deadTrigCells[1] = 9; 
    //deadTrigCells[2] = 18; 
    //deadTrigCells[3] = 19;
    //deadTrigCells[6] = 26;
    //deadTrigCells[7] = 27;
    //deadTrigCells[10] = 21 ;
    //deadTrigCells[11] = 24 ;
    //deadTrigCells[10] = 59 ;
    //deadTrigCells[11] = 60 ;
    //deadTrigCells[14] = 76 ;
    //deadTrigCells[15] = 77 ;
    //deadTrigCells[16] = 94 ;
    //deadTrigCells[17] = 95 ;

    H1DBManager::Instance()->StartRun(492558);
    fillDeadCellPositions();


    //circular cuts:
    printf("Implementing an additional circ. cut (before ncuts=%d) \n", nCircularCuts);
    nCircularCuts++;
    circCutsRadius[nCircularCuts - 1] = 10.4;
    circCutsXzero[nCircularCuts - 1] = 0.337138;
    circCutsYzero[nCircularCuts - 1] = 2.01292;

    nCircularCuts++;
    circCutsRadius[nCircularCuts - 1] = 7.26146;
    circCutsXzero[nCircularCuts - 1] = 2.47736;
    circCutsYzero[nCircularCuts - 1] = -4.52853;

    printf("There are now %d circ. cuts implemented\n", nCircularCuts);
    RunRange = 2007;

    return;
}

/*******************************************************************
 * clearDeadCellPositions -- clear arrays with cell positions
 *
 *
 *******************************************************************/
void elecCut::clearDeadCellPositions() {
    nDeadCells = 0;
    nDeadTrigCells = 0;

    for (int i = 0; i < MAX_DEAD_CELLS; i++) {
        deadCells[i] = 0;
        posDeadCellsX[i] = 0;
        posDeadCellsY[i] = 0;
    }
    for (int i = 0; i < MAX_DEAD_TRIG_CELLS; i++) {
        deadTrigCells[i] = 0;
        posDeadTrigCellsX[i] = 0;
        posDeadTrigCellsY[i] = 0;
    }

    //std circle (always present): 
    nCircularCuts = 1;
    circCutsRadius[0] = 12;
    circCutsXzero[0] = -2.025;
    circCutsYzero[0] = 0;

    for (int i = 1; i < MAX_CIRC_CUTS; i++) {
        circCutsRadius[i] = 0;
        circCutsXzero[i] = 0;
        circCutsYzero[i] = 0;
    }

    return;
}

/******************************************************************
 * fillDeadCellPositions -- use the given cell number to fill the
 * arrays with the cut bounds in order to cut out these cells. Used
 * for all run ranges.
 *****************************************************************/
void elecCut::fillDeadCellPositions() {
    if (dbg) printf("elecCut::fillDeadCellPositions: Getting Cell Positions from DB for Run %d\n", RunNumber);

    //make instance of DB specifiing the run number: 
    H1DBManager::Instance()->StartRun(RunNumber);

    if (dbg)printf("elecCut::fillDeadCellPositions: Trying to access DB for Geometry information\n");
    H1CellGeometry* fGeometry;
    H1CaloGeometry* caloGeom = static_cast<H1CaloGeometry*> (H1DBManager::Instance()->GetDBEntry(H1CaloGeometry::Class()));
    if (!caloGeom) {
        printf("elecCut::fillDeadCellPositions: GetDBGeometry: No H1CaloGeometry available.\n");
    }

    for (int i = 0; i < nDeadCells; i++) {

        if (dbg)printf("elecCut::fillDeadCellPositions: Getting SpaCal Position Information of caloCell: %d\n", deadCells[i]);
        fGeometry = caloGeom->GetCell(H1CaloGeometry::kSpaCal, deadCells[i]);
        if (fGeometry == NULL) {
            printf("elecCut::elecCut: Warning request to exclude not existing SpaCal Cell! Might give strange results\n");
            continue;
        }
        posDeadCellsX[i] = fGeometry->GetX();
        posDeadCellsY[i] = fGeometry->GetY();
        if (dbg)printf("elecCut::fillDeadCellPositions: SpaCal-Cell: %d; x=%f y=%f\n", deadCells[i], posDeadCellsX[i], posDeadCellsY[i]);
    }

    for (int i = 0; i < nDeadTrigCells; i++) {

        if (dbg) printf("elecCut::fillDeadCellPositions: Dead TrigCells: Getting SpaCal Position Information of caloCell: %d\n", deadTrigCells[i]);
        fGeometry = caloGeom->GetCell(H1CaloGeometry::kSpaCal, deadTrigCells[i]);


        if (fGeometry == NULL) {
            printf("elecCut::fillDeadCellPositions: Dead TrigCells: Warning request to exclude not existing SpaCal Cell! Might give strange results. Array Nmb: %d, CellNmb: %d\n", i, deadTrigCells[i]);
            continue;
        }
        posDeadTrigCellsX[i] = fGeometry->GetX();
        posDeadTrigCellsY[i] = fGeometry->GetY();
        if (dbg)printf("elecCut::fillDeadCellPositions: Dead TrigCells:SpaCal-Cell: %d; x=%f y=%f\n", deadTrigCells[i], posDeadTrigCellsX[i], posDeadTrigCellsY[i]);
    }

    return;
}

/****************************************************************
 * goodElec -- select a good electron asking for:
 *
 * 1. not inside of spacial cuts
 * 2. cut on hadronic energy fraction
 * 3. track poiting to cluster
 */
int elecCut::goodElec(H1PartEm* elec, Int_t RunNumber_) {

    //check if cuts are still ok: 
    initElecCut(RunNumber_);


    //elec whether h1oo finder found it: 
    if (!elec->IsScatElec()) {
        return 0;
    }

    //make spatial cuts: 
    if (elecCutSpatial(elec->GetXClus(), elec->GetYClus()) != 1) {
        return 0;
    }

    //make cut on hadronic energy part 
    if (elecCutHadrEnergy(elec) != 1) {
        return 0;
    }

    // --- Remove all track conditions on the electron 
    //check for tracks (either bpc or cjc (if in accaptance)): 
    //if(elecCutTrack(elec )!=1) { 
    //  return 0; 
    //}

    return 1;
}


/****************************************************************
 * goodElec_noSpatialCuts
 *
 * 1. cut on hadronic energy fraction
 * 2. track poiting to cluster
 */
int elecCut::goodElec_noSpatialCuts(H1PartEm* elec) {

    //elec whether h1oo finder found it: 
    if (!elec->IsScatElec()) {
        return 0;
    }

    //make cut on hadronic energy part 
    if (elecCutHadrEnergy(elec) != 1) {
        return 0;
    }

    // --- Remove all track conditions on the electron 
    //check for tracks (either bpc or cjc (if in accaptance)): 
    //if(elecCutTrack(elec )!=1) { 
    //  return 0; 
    //}
    return 1;
}
/****************************************************************
 * goodElec_noSpatialCuts
 */
int elecCut::goodElec_noSpatialCuts(H1PartEm* elec, Int_t RunNumber_) {
    //check if cuts are still ok: 
    initElecCut(RunNumber_);
    return goodElec_noSpatialCuts(elec);
}


/****************************************************************
* goodElec_NoTrackCond -- all electron cuts exect looking
* for a track
*/
int elecCut::goodElec_TrackCond(H1PartEm* elec) {

    //elec whether h1oo finder found it: 
    if (!elec->IsScatElec()) {
        return 0;
    }

    //make spatial cuts: 
    if (elecCutSpatial(elec->GetXClus(), elec->GetYClus()) != 1) {
        return 0;
    }

    //make cut on hadronic energy part 
    if (elecCutHadrEnergy(elec) != 1) {
        return 0;
    }

    //check for tracks (either bpc or cjc (if in accaptance)): 
    if (elecCutTrack(elec) != 1) {
        return 0;
    }

    return 1;
}

/****************************************************************
 * goodElec_TrackCond -- all electron cuts exect looking
 * for a track with previous check if cuts are still ok for the
 * given runnumber.
 */
int elecCut::goodElec_TrackCond(H1PartEm* elec, Int_t RunNumber_) {
    initElecCut(RunNumber_);
    return goodElec_TrackCond(elec);
}


/****************************************************************
 * name: elecCutSpatial
 * parameters:  x and y Position of the Cluster in SpaCal of the
 *              scattered Electron
 */
int elecCut::elecCutSpatial(Float_t x, Float_t y) {

    //Dead Cells: 
    for (int i = 0; i < nDeadCells; i++) {
        if (x >= posDeadCellsX[i] - DeadCellCut && x <= posDeadCellsX[i] + DeadCellCut &&
            y >= posDeadCellsY[i] - DeadCellCut && y <= posDeadCellsY[i] + DeadCellCut)
            return 0;
    }

    //Dead Trigger Cells: 
    for (int i = 0; i < nDeadTrigCells; i++) {
        if (x >= posDeadTrigCellsX[i] - DeadTriggCellCut && x <= posDeadTrigCellsX[i] + DeadTriggCellCut &&
            y >= posDeadTrigCellsY[i] - DeadTriggCellCut && y <= posDeadTrigCellsY[i] + DeadTriggCellCut)
            return 0;
    }

    //Radius Cut (circle is NOT located in the middle): 
    //Float_t rad =TMath::Sqrt( (x+2.025)*(x+2.025) + y*y ); 
    //if(rad<=RadCut) {
    //  return 0; 
    // }

    //new implementation: 
    Float_t rad;
    for (int j = 0; j < nCircularCuts; j++) {
        rad = TMath::Sqrt((x - circCutsXzero[j]) * (x - circCutsXzero[j]) + (y - circCutsYzero[j]) * (y - circCutsYzero[j]));
        if (rad <= circCutsRadius[j]) {
            return 0;
        }
    }

    return 1;
}

/****************************************************************
 * elecCutHadrEnergy --
 *
 */
int elecCut::elecCutHadrEnergy(H1PartEm* elec) {
    H1FloatPtr ElecE("ElecE");              // energy of scattered electron (acc. to e-finder)
    Float_t EnHadSpac;

    //the two fraction: 
    Float_t EnFracEmCalo;
    Float_t EnFracHadCalo;

    EnFracEmCalo = elec->GetEaem();  //energy fraction in electromagnetic part of calo 
    EnHadSpac = elec->GetEnHadSpac();
    EnFracHadCalo = EnHadSpac / *ElecE;

    //cut on either fraction (check wheter both are consistent!) 
    if (EnFracHadCalo > MAX_ENERGY_FRAC_HAD) return 0;

    //energy weighted cluster radius (already made by h1oo finder, just to be able to strengenth cut)
    if (elec->GetEcra() > MAX_ECRA) return 0;


    return 1;
}

/****************************************************************
 * elecCutTrack -- idea is look for any track or hit in the bpc.
 *                 or in the CJC, depending which acceptance fits
 *                 better. If cluster is neither acceptance range
 *                 accept the event.
 */
int elecCut::elecCutTrack(H1PartEm* elec) {
    Float_t distanceToBPC;

    //look for any corresponding track: 

    //check if in CJC Acceptance: 
    if (*ElecTheta < 2.7925) {
        if (elec->GetTrType() > 0) {
            if (dbg > 0) printf("elecCutTrack: Elektron Candidate has corresponding track of type: %d\n", elec->GetTrType());
            return 1;
        }
        else return 0;
    }

    //check wether we are in the BPC acceptance:
    Double_t x = elec->GetXClus();
    Double_t y = elec->GetYClus();
    Double_t xshift = 2.5;
    Double_t r = (TMath::Sqrt((x + xshift) * (x + xshift) + y * y));
    if ((TMath::Abs(y) > 5.5) && r > 19) {

        //in case no track look for bpc hits: 
        distanceToBPC = elec->GetBcRsp();  // get distance to BPC extrapolation
        if (distanceToBPC < MAX_BPC_DIST) {

            if (dbg > 0) printf("elecCutTrack: Elektron has bpc-track at distance %f\n", distanceToBPC);
            return 1;
        }
        return 0;
    }


    if (dbg > 0) printf("elecCutTrack: Cluster neither in BPC nor in CJC acceptance.\n");
    return 1; // in neither acceptance -> cannot do something so simply hope that it is no photon
}

/****************************************************************
 * elecCutTrack -- calls above function but checks wether the cuts
 * are still ok for this event.
 */
int elecCut::elecCutTrack(H1PartEm* elec, Int_t RunNumber_) {
    initElecCut(RunNumber_);
    return elecCutTrack(elec);
}


/****************************************************************
 * isInDeadCellArray -- check whether the given position in
 * the cell which is at the dead cell array at the given position.
 * Returns 1 if this is the case.
 */
int elecCut::isInDeadCellArray(H1PartEm* elec, Int_t i) {

    //check range: 
    if (i >= nDeadCells || i < 0) {
        printf("elecCut::isInDeadCellArray: Error request position in dead-cell array is out of bounds\n");
        return 0;
    }
    Float_t x = elec->GetXClus();
    Float_t y = elec->GetYClus();
    if (              //cell dimension like for dead trigger cells
        x >= posDeadCellsX[i] - DeadTriggCellCut && x <= posDeadCellsX[i] + DeadTriggCellCut &&
        y >= posDeadCellsY[i] - DeadTriggCellCut && y <= posDeadCellsY[i] + DeadTriggCellCut)
        return 1;
    return 0;
}


/**********************************************************************
 * DrawCuts
 */
void elecCut::DrawCuts(Int_t RunNumber_) {
    //----- 
    // Draw the cuts into the current pad.
    // Blue lines are dead Trigger Cell Cuts and 
    // red cut are dead cell cut. 
    // -----

    initElecCut(RunNumber_);

    TBox* CellArr[nDeadCells];
    TBox* TrigCellArr[nDeadTrigCells];


    //Dead Cells: 
    for (int i = 0; i < nDeadCells; i++) {
        if (dbg) printf("Drawing %d-deadCell(Nmb %d) at: %f %f\n", i, deadCells[i], posDeadCellsX[i], posDeadCellsY[i]);

        CellArr[i] = new TBox(posDeadCellsX[i] - DeadCellCut, posDeadCellsY[i] - DeadCellCut, posDeadCellsX[i] + DeadCellCut, posDeadCellsY[i] + DeadCellCut);
        CellArr[i]->SetFillStyle(0);
        CellArr[i]->SetLineColor(kRed);
        CellArr[i]->SetLineWidth(2);
        CellArr[i]->Draw("same");
    }

    if (dbg) printf("\n");
    //Dead Trigger Cells: 
    for (int i = 0; i < nDeadTrigCells; i++) {
        if (dbg) printf("Drawing %d-deadTriggCell (Nmb %d) at: %f %f\n", i, deadTrigCells[i], posDeadTrigCellsX[i], posDeadTrigCellsY[i]);
        TrigCellArr[i] = new TBox(posDeadTrigCellsX[i] - DeadTriggCellCut, posDeadTrigCellsY[i] - DeadTriggCellCut, posDeadTrigCellsX[i] + DeadTriggCellCut, posDeadTrigCellsY[i] + DeadTriggCellCut);
        TrigCellArr[i]->SetFillStyle(0);
        TrigCellArr[i]->SetLineColor(kBlue);
        TrigCellArr[i]->SetLineWidth(2);
        TrigCellArr[i]->Draw("same");

    }


    //Radius Cut (circle is NOT located in the middle): 
    //TEllipse *ell = new TEllipse(-2.025,0, RadCut,RadCut);
    //ell->SetLineColor(kMagenta); ell->SetLineWidth(2);
    //ell->Draw("same");

    //new implementation: 
    if (dbg) printf("Drawing %d circular cuts\n", nCircularCuts);
    TEllipse* ellArr[nCircularCuts];
    for (int j = 0; j < nCircularCuts; j++) {
        if (dbg) printf("Drawing ciruclar Cut[%d]: at x=%f y=%f with radius=%f\n", j, circCutsXzero[j], circCutsYzero[j], circCutsRadius[j]);
        //Radius Cut (circle is NOT located in the middle): 
        ellArr[j] = new TEllipse(circCutsXzero[j], circCutsYzero[j], circCutsRadius[j], circCutsRadius[j]);
        ellArr[j]->SetLineColor(kMagenta); ellArr[j]->SetLineWidth(2);
        ellArr[j]->Draw("same");
    }

}





Float_t fLinearityGrid[2];
Float_t fLinearityOffset[2];


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Begin Functions for SpaCal Cuts

Int_t 	fRunType;		// fRunType: 0=data, 8=Django, 11=Rapgap

void SetSpacalAlignmentConstants(){
    // spacal linearity
    fLinearityGrid[0] = 4.05;
    fLinearityGrid[1] = 4.05;
    if (fRunType == 0) { //If Data, then we implement these Cuts
        if (fMyRunYear == 0) {
            fLinearityOffset[0] = 0.0156408;
            fLinearityOffset[1] = 0.114676;
            //SetAlpha({ 0.242819,-0.0205778,0.0260432,0.0383076,0.00562798 });
            Float_t fAlphatemp0[5] = { 0.242819,-0.0205778,0.0260432,0.0383076,0.00562798 };
            SetAlpha(fAlphatemp0);
            //SetSuperX("0.0167276,0.0299261,-0.00953007,0.0179944,-0.00972528,-0.0168475,-0.0319348,-0.0684563,-0.086717,     0.00352905,0.0693822,0.0442551,0.0216813,-0.0303227,-0.064708,-0.140036,-0.104978,-0.104978,     -0.0623242,0.0570739,0.0723443,0.00377095,0.00792301,-0.0689632,-0.156968,-0.210344,-0.154463,     0.105895,-0.0180959,-0.0503164,-0.0626292,-0.0209485,-0.115061,-0.114641,-0.210933,-0.0985816,     -0.0177701,-0.0033432,-0.0284922,-0.245953,-0.165167,-0.330278,-0.227396,-0.286239,-0.283827,     0.0458903,-0.00337742,-0.00804871,-0.0686681,-0.06349,-0.0585203,-0.0880737,-0.153037,-0.22619,     -0.0711327,-0.00400469,0.0395916,-0.0445892,-0.0199598,-0.0210018,-0.0910451,-0.208566,-0.217378,     0.00934288,0.0898184,0.000884791,0.0418213,0.031366,0.0286092,0.0496734,0.0150122,0.0150122,     0.0204303,0.0315178,-0.0267829,0.0643862,0.0987499,0.190735,0.152378,0.0836953,0.0493537");
            Float_t fSuperXtemp0[81] = {0.0167276,0.0299261,-0.00953007,0.0179944,-0.00972528,-0.0168475,-0.0319348,-0.0684563,-0.086717,     0.00352905,0.0693822,0.0442551,0.0216813,-0.0303227,-0.064708,-0.140036,-0.104978,-0.104978,     -0.0623242,0.0570739,0.0723443,0.00377095,0.00792301,-0.0689632,-0.156968,-0.210344,-0.154463,     0.105895,-0.0180959,-0.0503164,-0.0626292,-0.0209485,-0.115061,-0.114641,-0.210933,-0.0985816,     -0.0177701,-0.0033432,-0.0284922,-0.245953,-0.165167,-0.330278,-0.227396,-0.286239,-0.283827,     0.0458903,-0.00337742,-0.00804871,-0.0686681,-0.06349,-0.0585203,-0.0880737,-0.153037,-0.22619,     -0.0711327,-0.00400469,0.0395916,-0.0445892,-0.0199598,-0.0210018,-0.0910451,-0.208566,-0.217378,     0.00934288,0.0898184,0.000884791,0.0418213,0.031366,0.0286092,0.0496734,0.0150122,0.0150122,     0.0204303,0.0315178,-0.0267829,0.0643862,0.0987499,0.190735,0.152378,0.0836953,0.0493537};
            SetSuperX(fSuperXtemp0);
            //SetSuperY("-0.184199,-0.117911,-0.131192,0.0389027,0.115862,0.0307898,-0.00752875,0.0168387,0.0290224,     -0.250487,-0.104629,-0.134111,0.0322301,0.277894,0.0913244,0.088867,0.0412062,0.0412062,     -0.396345,-0.165978,-0.052143,-0.0679408,0.094628,0.000819872,0.0634958,-0.0925063,-0.158756,     -0.427338,-0.247281,-0.152371,-0.0822678,0.14742,-0.017831,-0.04241,-0.0952114,-0.225006,     -0.42054,-0.226968,-0.174085,-0.0912892,0.00987576,-0.0897532,-0.0868805,-0.2153,-0.293616,     -0.376793,-0.271311,-0.213815,-0.035344,0.0731252,-0.104741,-0.126334,-0.216685,-0.36775,     -0.418636,-0.311764,-0.241163,-0.0179121,0.174335,-0.0133711,-0.0981814,-0.216164,-0.291957,     -0.383396,-0.348156,-0.278025,-0.106292,0.106834,-0.0755157,-0.000586344,-0.144734,-0.144734,     -0.297412,-0.211428,-0.0746996,-0.0728105,0.139452,0.00418469,0.0516676,-0.046533,-0.0956334");
            Float_t fSuperYtemp0[81] = {-0.184199, -0.117911, -0.131192, 0.0389027, 0.115862, 0.0307898, -0.00752875, 0.0168387, 0.0290224, -0.250487, -0.104629, -0.134111, 0.0322301, 0.277894, 0.0913244, 0.088867, 0.0412062, 0.0412062, -0.396345, -0.165978, -0.052143, -0.0679408, 0.094628, 0.000819872, 0.0634958, -0.0925063, -0.158756, -0.427338, -0.247281, -0.152371, -0.0822678, 0.14742, -0.017831, -0.04241, -0.0952114, -0.225006, -0.42054, -0.226968, -0.174085, -0.0912892, 0.00987576, -0.0897532, -0.0868805, -0.2153, -0.293616, -0.376793, -0.271311, -0.213815, -0.035344, 0.0731252, -0.104741, -0.126334, -0.216685, -0.36775, -0.418636, -0.311764, -0.241163, -0.0179121, 0.174335, -0.0133711, -0.0981814, -0.216164, -0.291957, -0.383396, -0.348156, -0.278025, -0.106292, 0.106834, -0.0755157, -0.000586344, -0.144734, -0.144734, -0.297412, -0.211428, -0.0746996, -0.0728105, 0.139452, 0.00418469, 0.0516676, -0.046533, -0.0956334};
            SetSuperY(fSuperYtemp0);
        }
        else if (fMyRunYear == 1) {
            fLinearityOffset[0] = -0.00917979;
            fLinearityOffset[1] = -0.00399605;
            Float_t fAlphatemp1[5] = {0.215161,-0.0182998,0.02826,0.0813706,0.0425845};
            SetAlpha(fAlphatemp1);
            Float_t fSuperXtemp1[81] = { 0.113454,0.0932743,0.00347954,0.0244398,0.000883084,-0.02592,-0.048668,-0.048668,-0.048668,     0.133634,0.183069,0.0783529,0.00478777,-0.006267,-0.0698909,-0.198329,-0.186984,-0.194305,     0.0841982,0.162651,0.122075,-0.00819927,-0.00605708,-0.0732004,-0.197964,-0.175639,-0.201626,     -0.0110601,0.0824709,-0.0221144,-0.0785848,-0.043309,-0.085669,-0.157766,-0.208162,-0.227613,     0.0962547,0.160219,-0.0573068,-0.0154746,-0.0723762,-0.180757,-0.235262,-0.347882,-0.267003,     0.114549,0.154212,-0.00724216,-0.0806062,-0.0499646,-0.0119954,-0.0830394,-0.173053,-0.227015,     0.0996372,0.010951,0.041264,-0.0171611,-0.0106343,-0.0112487,-0.0913166,-0.16963,-0.198323,     -0.0758361,-0.25131,-0.00221716,0.0891758,0.07131,0.0383883,0.0679292,-0.077471,-0.077471,     -0.0727452,-0.0696542,0.112001,0.121105,0.160444,0.162914,0.108163,0.0153459,-0.0310625 };
            SetSuperX(fSuperXtemp1);
            Float_t fSuperYtemp1[81] = { -0.0568055, 0.00757551, -0.0138492, 0.239303, 0.49331, 0.234933, 0.131305, 0.131305, 0.131305, -0.121187, 0.0290002, 0.0804248, 0.202316, 0.44011, 0.270634, 0.33842, 0.213046, 0.106836, -0.271373, -0.0327233, 0.104232, 0.0892096, 0.229055, 0.140664, 0.217952, 0.0876717, 0.000627167, -0.33823, -0.12907, -0.0255489, 0.0386677, 0.168288, 0.0900059, 0.108489, 0.0692465, -0.0864174, -0.340812, -0.178303, -0.0309631, 0.0522595, 0.12588, 0.0191011, 0.0384868, -0.0643555, -0.166005, -0.360134, -0.184179, -0.0944194, 0.118833, 0.263871, 0.074698, 0.00841553, -0.12264, -0.282888, -0.376758, -0.259897, -0.171783, 0.0444287, 0.293484, 0.0922775, -0.0279638, -0.129469, -0.206179, -0.341813, -0.306869, -0.161932, -0.0779615, 0.120118, 0.00289883, 0.092326, -0.0608599, -0.0608599, -0.270403, -0.198992, -0.0911157, -0.106834, 0.17157, 0.0265606, 0.0858969, 0.0125185, -0.0241707 };
            SetSuperY(fSuperYtemp1);
        }
        else if (fMyRunYear == 2 || fMyRunYear == 3) {
            fLinearityOffset[0] = -0.0259109;
            fLinearityOffset[1] = -0.0682793;
            Float_t fAlphatemp2or3[5] = {0.201435,-0.0164299,0.0289763,0.0623963,0.0485568};
            SetAlpha(fAlphatemp2or3);
            Float_t fSuperXtemp2or3[81] = { -0.027421, -0.0661337, -0.118057, -0.0725348, -0.122883, -0.126813, -0.194818, -0.236544, -0.257407, 0.0112918, -0.0142107, -0.0606004, -0.0844394, -0.106726, -0.124312, -0.266982, -0.27827, -0.27827, 0.0367943, 0.0839472, -0.00353726, -0.0446818, -0.0403773, -0.126336, -0.280727, -0.300782, -0.278092, -0.0828938, 0.0461634, -0.0276134, -0.0743554, -0.0567722, -0.097099, -0.210123, -0.289722, -0.255402, 0.0953872, 0.0384836, -0.0606722, 0.00199704, -0.0779063, -0.198728, -0.225476, -0.322192, -0.322667, 0.119855, 0.0727173, -0.0258439, -0.100627, -0.0581218, -0.0153945, -0.0890133, -0.161109, -0.156465, 0.100099, 0.143794, 0.106222, 0.0163566, 0.0180538, 0.0290426, -0.0439715, -0.106904, -0.131684, 0.158211, 0.216323, 0.196453, 0.1841, 0.135436, 0.120477, 0.165764, 0.0440787, 0.0440787, 0.191282, 0.224353, 0.232383, 0.277551, 0.235759, 0.287543, 0.274074, 0.159077, 0.101578 };
            SetSuperX(fSuperXtemp2or3);
            Float_t fSuperYtemp2or3[81] = { 0.0832672,0.116299,0.0796698,0.288301,0.444733,0.138438,0.0511331,0.0745593,0.0862724,     0.0502349,0.152929,0.155398,0.274107,0.406879,0.188989,0.153456,0.0979855,0.0979855,     -0.0524594,0.132172,0.19865,0.109674,0.189768,0.0639354,0.0773207,-0.0848675,-0.158384,     -0.170991,-0.0389322,0.0138136,0.0409491,0.141092,0.0129768,0.005133,-0.0741054,-0.231901,     -0.19702,-0.0765572,-0.00642098,0.0551221,0.101666,-0.0377834,-0.0310269,-0.18888,-0.314892,     -0.210198,-0.0787831,-0.0256518,0.131021,0.248235,0.0305683,-0.0664505,-0.23373,-0.377147,     -0.214609,-0.148201,-0.0631045,0.0980578,0.281142,0.0286138,-0.0880433,-0.267968,-0.322557,     -0.197529,-0.18045,-0.159395,-0.0431372,0.157218,-0.0682303,-0.00545162,-0.187903,-0.187903,     -0.165193,-0.132856,-0.0852625,-0.0632842,0.157503,-0.0372449,0.0230227,-0.0824403,-0.135172 };
            SetSuperY(fSuperYtemp2or3);
        }
    }
    else { // If MC we use these cuts
        if (fMyRunYear == 0) {
            fLinearityOffset[0] = 0.0756862;
            fLinearityOffset[1] = -0.0219959;
            Float_t fAlphatemp0MC[5] = { 0.229871,-0.017306,0.0232054,0.131317,-0.00428949 };
            SetAlpha(fAlphatemp0MC);
            Float_t fSuperXtemp0MC[81] = { -0.112575, -0.074685, 0.00748334, 0.0883198, 0.032299, 0.0341294, 0.0318277, 0.0767779, 0.104297, -0.150466, -0.156853, -0.01483, 0.0817488, 0.00842957, 0.00376256, 0.0253491, 0.121728, 0.131816, -0.144078, -0.0998387, 0.00125859, 0.0250847, 0.0153244, -0.000281896, 0.0352053, 0.0952164, 0.141903, -0.188003, -0.0436979, 0.0033839, 0.0428401, 0.0115921, -0.00891917, 0.0286446, 0.0824917, 0.14654, -0.133778, -0.0711502, -0.00898231, 0.197168, 0.0316641, -0.0800458, 0.013842, 0.0817766, 0.199707, -0.0802596, -0.0862033, -0.0240626, 0.0152991, -0.00205817, -0.0276893, 0.0195639, 0.0441681, 0.145387, -0.202458, -0.164271, -0.040604, 0.000968214, 0.00519532, -0.027744, 0.0278173, 0.0765082, 0.147582, -0.163364, -0.124269, -0.0300521, 0.00723123, 0.0209032, -0.0251591, 0.0633327, 0.0699204, 0.147582, -0.13167, -0.099976, -0.0756829, 0.0142271, 0.00926576, 0.00534871, 0.0123014, 0.0123014, 0.0799415 };
            SetSuperX(fSuperXtemp0MC);
            Float_t fSuperYtemp0MC[81] = { -0.144135, -0.173582, -0.181029, -0.139002, -0.131031, -0.0966886, -0.137053, -0.133316, -0.0971309, -0.114688, -0.166135, -0.0206866, -0.110896, -0.0956497, -0.0864086, -0.074221, -0.129578, -0.0609461, -0.0632406, -0.130691, -0.022945, -0.0270785, 0.0021297, -0.0255188, 0.00418772, -0.0362467, 0.00768569, 0.0128899, 0.00582616, 0.0144889, 0.0452309, 0.101753, 0.0288762, 0.0225374, 0.0230434, 0.038765, 0.0540212, 0.0402587, 0.0640518, 0.0548586, 0.0192469, 0.0559277, 0.029758, 0.110632, 0.0191278, 0.0241061, 0.0172935, -0.0257002, -0.0355205, -0.135551, -0.0119597, 0.00692445, 0.0181148, 0.0184323, 0.00932207, 0.0530526, -0.000444617, 0.0199773, -0.00914643, 0.0109903, -0.00238056, 0.144719, 0.0978406, 0.0421419, 0.0749617, 0.0649934, 0.0339293, 0.0282025, 0.0816079, 0.118511, 0.131615, 0.0978406, 0.0813916, 0.120641, 0.166321, 0.11437, 0.213517, 0.178427, 0.216286, 0.216286, 0.157063 };
            SetSuperY(fSuperYtemp0MC);
        }
        else if (fMyRunYear == 1) {
            fLinearityOffset[0] = 0.0450836;
            fLinearityOffset[1] = -0.0213424;
            Float_t fAlphatemp1MC[5] = { 0.222361,-0.0168027,0.0236539,0.152251,0.0535807 };
            SetAlpha(fAlphatemp1MC);
            Float_t fSuperXtemp1MC[81] = { -0.0814931, -0.0624145, -0.0186974, 0.00286625, -0.00761901, -0.00697797, 0.0572935, 0.00603995, 0.014152, -0.100572, -0.106132, 0.0365898, 0.0349152, 0.0184479, -0.0158802, 0.0355114, -0.0452136, 0.022264, -0.0950118, -0.0606038, -0.0034592, 0.0290239, 0.0115734, -0.0194702, 0.0165276, -0.0110035, 0.0897416, -0.069195, -0.0209813, 0.00326107, 0.0194679, 0.0199152, -0.011225, 0.0187123, 0.0778457, 0.0614661, -0.0538265, -0.0351037, 0.0135774, 0.0913752, 0.0248889, -0.0635821, 0.03024, 0.0173539, -0.0136285, -0.178637, -0.0857887, -0.0446763, 0.014262, 0.0518472, 0.00376849, 0.0147571, 0.0646203, 0.0863166, -0.167387, -0.178946, -0.0270536, 0.00840976, -0.00116491, -0.0165572, -0.00484165, 0.0575968, 0.17801, -0.125565, -0.0837435, -0.0236064, -0.0101158, -0.0198898, -0.0291642, -0.0167258, 0.0756735, 0.126842, -0.100742, -0.0759191, -0.0680946, -0.0267012, -0.0314931, -0.0214365, 0.015769, 0.0457212, 0.0862816 };
            SetSuperX(fSuperXtemp1MC);
            Float_t fSuperYtemp1MC[81] = { -0.0478533, -0.0734058, -0.102354, -0.0901828, -0.102747, -0.135577, -0.13048, -0.125585, -0.0937399, -0.0223007, -0.0444575, -0.0635622, -0.0654469, -0.109459, -0.0840868, -0.0960087, -0.12069, -0.0618948, -0.000143904, -0.0169939, -0.0110756, -0.0198622, -0.0115892, -0.0140679, -0.00176534, -0.0620212, -0.00309938, -0.0136523, -0.00114039, 0.0142631, 0.0298079, 0.104713, 0.00984711, -0.0126593, 0.0328655, -0.0690261, 0.0388193, 0.0171203, 0.0636595, 0.0528344, 0.0153614, -0.0185018, 0.026403, 0.00649027, -0.0131804, -0.0369861, -0.0182689, -0.0200602, -0.0117647, -0.0775996, -0.00403396, -0.0104298, 0.0142977, -0.0251816, 0.00863487, -0.00211022, -0.0127156, -0.0054499, 0.0213396, 0.00636913, -0.0337177, 0.0291456, 0.0105337, 0.107779, 0.206924, 0.0367384, 0.0348488, 0.0080741, 0.0713293, 0.0852914, 0.0296444, 0.020089, 0.13577, 0.16376, 0.120596, 0.0584171, 0.111699, 0.108132, 0.128273, 0.0789586, 0.0495238 };
            SetSuperY(fSuperYtemp1MC);
        }
        else if (fMyRunYear == 2 || fMyRunYear == 3) {
            fLinearityOffset[0] = 0.0766019;
            fLinearityOffset[1] = 0.00496387;
            Float_t fAlphatemp2or3MC[5] = { 0.23006,-0.0170495,0.0218784,0.133303,0.0680848 };
            SetAlpha(fAlphatemp2or3MC);
            Float_t fSuperXtemp2or3MC[81] = { -0.105101, -0.0864725, -0.0864725, -0.0254642, -0.01637, -0.0774192, -0.0543677, 0.0650274, 0.0865292, -0.123729, -0.0667832, -0.00246041, 0.02645, 0.0130464, -0.037152, -0.0673978, 0.184422, 0.108031, -0.123729, -0.131106, -0.00889165, 0.0195066, 0.00343204, -0.0168376, 0.00830687, 0.0131382, 0.0316398, -0.152101, -0.0989825, -0.0192033, 0.00705822, 0.0129416, -0.00145142, -0.0268912, 0.0506005, 0.0559731, -0.0932843, -0.0139406, 0.00979134, 0.10799, 0.0109557, -0.0708491, -0.000634028, 0.0772012, 0.0856791, -0.0208314, -0.0346946, -0.0356812, -0.0106477, -0.0062601, 0.0247147, 0.0507293, 0.129753, 0.123757, -0.0968788, -0.0683252, -0.00403248, 0.0253514, 0.0143435, -0.0104396, 0.0555418, 0.0970307, 0.178005, -0.0921231, -0.0873674, 0.178199, 0.00990737, 0.00882421, 0.0165779, 0.0338397, 0.0654352, 0.178005, -0.0529985, -0.0138738, 0.0596198, 0.064537, 0.0366806, 0.0165779, 0.0338397, 0.0496375, 0.178005 };
            SetSuperX(fSuperXtemp2or3MC);
            Float_t fSuperYtemp2or3MC[81] = { -0.0696401, -0.123768, -0.123768, -0.0843785, -0.0929632, -0.115565, -0.144531, -0.170222, -0.152396, -0.0155126, -0.0192892, 0.0358808, -0.0364046, -0.0900375, -0.0842288, -0.105893, -0.195914, -0.13457, -0.0155126, -0.0744593, 0.0258037, -0.024498, -0.0622983, -0.0254199, -0.0137218, -0.0974735, -0.073227, 0.027563, 0.0460149, 0.0182597, 0.0148962, 0.0231636, 0.00223306, 0.0136436, -0.0248595, -0.0231475, 0.00941376, 0.052282, 0.000749501, -0.0531546, -0.00549001, 0.0568937, 0.0616965, 0.0705135, 0.028644, -0.0678167, 0.0111752, -0.0143891, -0.00210435, -0.0488627, -0.00326151, -0.0110533, 0.0236442, -0.0163954, 0.00447923, 0.0746244, 0.0401569, 0.0505877, 0.00819511, 0.0236468, -0.097244, -0.0199611, 0.0658008, 0.0807088, 0.156938, -0.0127381, 0.0540481, 0.0685823, 0.039811, 0.0117737, -0.00409371, 0.0658008, 0.116129, 0.15155, 0.146162, 0.25611, 0.162346, 0.039811, 0.0117737, 0.00384001, 0.0658008 };
            SetSuperY(fSuperYtemp2or3MC);
        }
        //fSpacLinearity.PrintSteer(std::cout);
    }

        // spacal alignment 
        if (fRunType == 0) {
            if (fMyRunYear == 0) {
                SetPhi(0.000179124);
                //SetOrigin("-0.0193911,-0.0159895,0.0710439");
                Float_t fOrigintemp0[3] = { -0.0193911,-0.0159895,0.0710439 };
                SetOrigin(fOrigintemp0);
                SetZ0(-162.648);
            }
            else if (fMyRunYear == 1) {
                SetPhi(0.000589021);
                Float_t fOrigintemp1[3] = { -0.0380174,0.00789926,-1.07628 };
                SetOrigin(fOrigintemp1);
                SetZ0(-162.49);
            }
            else if (fMyRunYear == 2 || fMyRunYear == 3) {
                SetPhi(-0.000705192);
                Float_t fOrigintemp2or3[3] = { -0.0212224,0.0212467,-0.204696 };
                SetOrigin(fOrigintemp2or3);
                SetZ0(-161.639);
            }
        }
        else { // MC
            if (fMyRunYear == 0) {
                SetPhi(0.0001963);
                Float_t fOrigintemp0MC[3] = { 0.00405899,0.0144147,0.0468047 };
                SetOrigin(fOrigintemp0MC);
                SetZ0(-162.188);
            }
            else if (fMyRunYear == 1) {
                SetPhi(0.00050441);
                Float_t fOrigintemp1MC[3] = { 0.00461259,0.0155858,0.260419 };
                SetOrigin(fOrigintemp1MC);
                SetZ0(-161.985);
            }
            else if (fMyRunYear == 2 || fMyRunYear == 3) {
                SetPhi(-0.00125922);
                Float_t fOrigintemp2or3MC[3] = { 0.0129104,0.0243385,0.206787 };
                SetOrigin(fOrigintemp2or3MC);
                SetZ0(-162.045);
            }
        }
    }

TLorentzVector GetHelixMom(Double_t e, const TVector3& posrel, Int_t charge) {
    // calculate 4-vector of momentum at vertex
    // posrel: vector pointing from vertex to measured space point
    // e: energy
    // charge: particle charge
    TLorentzVector mom;
    // first approximation: straight line
    mom.SetVectM(posrel, 0.);
    mom *= e / mom.E();

    Double_t z = posrel.Z();
    Double_t r = posrel.Pt();

    if (charge != 1 && charge != -1) {
        cout << "[GetHelixMom] invalid charge=" << charge << "\n";
        exit(10);
    }

    Double_t phi1 = posrel.Phi();
    for (int ii = 0; ii < 5; ii++) {
        // present guess of transverse momentum
        Double_t pt = mom.Pt();
        if (pt <= 0.0) break;
        // present guess of signed inverse radius
        const double B_TO_KAPPA = -(0.00029979) * 11.6083;
        Double_t kappa = charge * B_TO_KAPPA / pt;
        //
        Double_t asin_arg = 0.5 * r * kappa;
        if (asin_arg >= 1.0) asin_arg = 1.0;
        if (asin_arg < -1.0) asin_arg = -1.0;
        // present guess of difference in phi
        // between posrel and momentum at vertex
        // (note: the full arc length is given by 2*dphi/kappa )
        Double_t dphi = asin(asin_arg);
        mom.SetPhi(phi1 - dphi);
        // adjust Theta angle
        // not safe if kappa=0 !!!
        // but this can not happen, because
        //   the energy is finite and charge!=0
        mom.SetTheta(atan2(2. * dphi / kappa, z));
    }
    return mom;

}

void DoTilt(TLorentzVector& val, const TVector2& TiltVec) {
    TLorentzVector tilted = val;
    Double_t tz = TMath::Sqrt(1. - TiltVec.Mod2());
    tilted[2] = tz * val[2] + TiltVec.X() * val[0] + TiltVec.Y() * val[1];
    tilted[0] -= TiltVec.X() * val[2];
    tilted[1] -= TiltVec.Y() * val[2];
    Double_t ptprime = tilted.Vect().Pt();
    if (ptprime > 0) {
        Double_t f = TMath::Sqrt(val.Vect().Mag2() - tilted[2] * tilted[2]) / ptprime;
        if (TMath::Finite(f)) {
            tilted[0] *= f;
            tilted[1] *= f;
        }
    }
    val = tilted;
}

//bool FiducialCut(Double_t x_clus, Double_t y_clus, Int_t RunYear)
//{
//    /// 	From Emmanuel Sauwan   LAST CUTS from inclusive diffractive analysis
//    Double_t Rspac;
//    Double_t rspacal = TMath::Sqrt(x_clus * x_clus + y_clus * y_clus);
//    ///	Double_t elZClus = elecs[pointe] -> GetZClus();	
//
//    ///	if(FiduPlot) hFiduCutBF -> Fill(x_clus, y_clus, fWeight);
//    if (RunYear >= 7) {
//        //---- (box removed from S3,S2,S9,S0) (seems to be the case)
//        if (x_clus > -16.2 && x_clus<8.1 && y_clus>-8.1 && y_clus < 16.2)   return false;
//        //         if(rspacal<15||rspacal>70) return false;	//  SpaCal outer radius cut.   Sauwan
//        //         if(rspacal<18||rspacal>74) return false;       //  SpaCal inner and outer radius cut.   ELAN
//
//        if (rspacal < 15 || rspacal>74) return false;	//  SpaCal outer radius cut.   ELAN
//
//    }
//    if (RunYear == 8) {  //--- new from compton analysis
//        if (x_clus > -15.0 && x_clus<-10.0 && y_clus>-5.0 && y_clus < 5.0)    return false;
//        if (x_clus > -12.5 && x_clus<8.5 && y_clus>-4.5 && y_clus < 4.5)    return false;
//        if (x_clus > -12.5 && x_clus<-8.4 && y_clus>-8.5 && y_clus < -4.0)   return false;
//
//        if (x_clus > 0.0 && x_clus<17.0 && y_clus>-5.0 && y_clus < 5.0)    return false;
//
//        if (x_clus > -28.35 && x_clus<-24.3 && y_clus >-52.65 && y_clus < -44.55) return false;
//
//        if (x_clus > -28.35 && x_clus<-24.3 && y_clus >-52.65 && y_clus < -44.55) return false;
//        if (x_clus > -36.45 && x_clus<-32.4 && y_clus >-36.45 && y_clus < -32.4)  return false;
//        if (x_clus > -24.3 && x_clus < -20.25 && y_clus >32 && y_clus < 40)     return false;
//        if (x_clus > -28. && x_clus < -20.25 && y_clus >32 && y_clus < 36)     return false;
//        if (x_clus > 28.35 && x_clus<32.4 && y_clus >-24.3 && y_clus < -20.25) return false;
//        if (x_clus > 60.75 && x_clus < 64.8 && y_clus >20.25 && y_clus < 24.5)   return false;
//        if (x_clus > 12.15 && x_clus < 20.25 && y_clus >4.05 && y_clus < 8.1)    return false;
//
//        if (x_clus > 0.0 && x_clus < 20.25 && y_clus>0.0 && y_clus < 8.1)    return false;
//
//        //---- new 12/09/2006
//        //---- central part
//        if (x_clus > -16.2 && x_clus<20.25 && y_clus >-4.05 && y_clus < 8.1)    return false;
//        //---- another dead cells
//        if (x_clus > -28.35 && x_clus < -24.3 && y_clus >4.05 && y_clus < 8.1)    return false;
//        if (x_clus > 64.8 && x_clus<68.85 && y_clus >-20.25 && y_clus < -16.2)  return false;
//
//        //---- special for S17/S41 ?
//        //         if(x_clus>-28.35 &&  x_clus<-24.3  && y_clus >40.5   && y_clus<44.55)  return false;
//        //         if(x_clus>-32.4  &&  x_clus<-28.35 && y_clus >56.7   && y_clus<60.75)  return false;
//
//    }
//
//    if (RunYear == 9) {
//        //----enlarged central part
//        if (x_clus > -17 && x_clus<9 && y_clus>-9 && y_clus < 17)     return false;
//        if (x_clus > -28.35 && x_clus<-24.3 && y_clus >-52.65 && y_clus < -44.55) return false;
//        if (x_clus > 60.75 && x_clus < 64.8 && y_clus >20.25 && y_clus < 24.5)   return false;
//        if (x_clus > 64.8 && x_clus<68.85 && y_clus >-20.25 && y_clus < -16.2)  return false;
//        if (x_clus > -68.85 && x_clus < -64.8 && y_clus >0. && y_clus < 8.1)    return false;
//
//        //---- central part
//        if (x_clus > -16.2 && x_clus<16.2 && y_clus >-4.05 && y_clus < 4.05)   return false;
//        if (x_clus > 0.0 && x_clus<17.0 && y_clus>-5.0 && y_clus < 5.0)    return false;
//
//        if (x_clus > 10.0 && x_clus < 15.0 && y_clus>5 && y_clus < 8.0)    return false;
//
//        //--new one 33
//        if (x_clus > 10.0 && x_clus < 15.0 && y_clus>16 && y_clus < 22)     return false;
//
//    }
//    if (RunYear == 10) {
//        //----enlarged central part
//        if (x_clus > -17 && x_clus<9 && y_clus>-9 && y_clus < 17)     return false;
//
//        if (x_clus > -28.35 && x_clus<-24.3 && y_clus >-52.65 && y_clus < -44.55) return false;
//        if (x_clus > 60.75 && x_clus < 64.8 && y_clus >20.25 && y_clus < 24.5)   return false;
//        if (x_clus > 64.8 && x_clus<68.85 && y_clus >-20.25 && y_clus < -16.2)  return false;
//        if (x_clus > -68.85 && x_clus < -64.8 && y_clus >0. && y_clus < 8.1)    return false;
//
//        //---- central part
//        if (x_clus > -16.2 && x_clus<16.2 && y_clus >-4.05 && y_clus < 4.05)   return false;
//        if (x_clus > 0.0 && x_clus<17.0 && y_clus>-5.0 && y_clus < 5.0)    return false;
//        //---- from 2006 e-p
//        if (x_clus > 10.0 && x_clus < 15.0 && y_clus>5 && y_clus < 8.0)    return false;
//
//        //--new33
//        if (x_clus > 5 && x_clus < 16.0 && y_clus>8 && y_clus < 13.0)   return false;
//
//        //---- from 0607DST1 data ...
//        if (x_clus > -10.0 && x_clus<10.0 && y_clus>-17 && y_clus < -10.0)  return false;
//    }
//    //       if(FiduPlot)hFiduCut -> Fill(x_clus, y_clus, fWeight); 
//    Rspac = rspacal;
//    return true;
//}

//End Functions for SpaCal Cuts
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int
main(int argc, char* argv[])
{
    // parse the command line
    H1StdCmdLine opts;
    opts.Parse(&argc, argv);

    // Load mODS/HAT files
    H1Tree::Instance()->Open();            // this statement must be there!

    //H1LumiYield::Instance()->Open();

     // do a selection
    // Can implement some selections here, I chose to use H1FloatPtr instead to variables and do a check later
     
    //H1Tree::Instance()->SelectHat("Q2e>5.");
    //H1Tree::Instance()->SelectHat("Q2e<100.");
    //H1Tree::Instance()->SelectHat("ElecE>10.");
    //H1Tree::Instance()->SelectHat("Ye>0.1");
    //H1Tree::Instance()->SelectHat("Ye<0.6");
    //H1Tree::Instance()->SelectHat("Epz>35.");
    //H1Tree::Instance()->SelectHat("Epz<70.");
    H1Tree::Instance()->SelectHat("fIl1ac[61]>0");
    
    // create new H1EventList
    H1EventList *list= new H1EventList("mylist");

    // and enter interesting events into this list
    // H1FloatPtr q2e("Q2e"); //Q2 variable aquired using electron method
    H1FloatPtr q2s("Q2s"); //Q2 variable aquired using sigma method
    // H1FloatPtr q2egen("Q2eGen");//Q2 gen level variable aquired using electron method
    H1FloatPtr q2sGen("Q2sGen"); //Q2 gen level variable aquired using sigma method
    H1FloatPtr eE("ElecE"); //Energy of electron as deposited in SpaCal
    //H1FloatPtr ye("Ye"); //Inelasticity aquired through sigma method
    H1FloatPtr ys("Ys"); //Inelasticity aquired through sigma method
    H1FloatPtr ysGen("YsGen"); //Inelasticity aquired through sigma method
    H1FloatPtr vtxZ("VtxZ");
    H1FloatPtr epz("Epz");

    //Values for Epz calculation
    H1FloatPtr ElecTheta("ElecTheta"); //Theta of scattered electron
    H1FloatPtr ElecPhi("ElecPhi");
    H1FloatPtr ElecE("ElecE"); //Energy of scattered electron
    H1FloatPtr EBeamE("EBeamE"); //Energy of electron beam from DMIS 
    H1FloatPtr EBeamP("EBeamP"); //Energy of proton beam from DMIS
    H1FloatPtr HadPzX("HadPzX");
    H1FloatPtr HadEX("HadEX");
    H1FloatPtr hfsE("FullHfsCombE");
    H1FloatPtr hfsTheta("FullHfsCombTheta");
    H1FloatPtr btx("BeamTiltX0");
    H1FloatPtr bty("BeamTiltY0");

    H1FloatPtr w1("Weight1");
    H1FloatPtr w2("Weight2");

    //Defining Root Canvas to work on
    TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 800);

    TH2F* hXYclusterBefore = new TH2F("XY Cluster Position Before", "XY Cluster Position Before", 1000, -80, 80, 1000, -80, 80);
    TH2F* hXYclusterAfter = new TH2F("XY Cluster Position After", "XY Cluster Position After", 1000, -80, 80, 1000, -80, 80);

    static H1PartEmArrayPtr PartEm;


    



    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Consistenct with Lumi calculation using SelectedRuns.root oolumi output file
    //TString goodRunFileName("/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/SelectedRuns.root");
    //TFile goodRunFile(goodRunFileName);
    //if (!goodRunFile.IsOpen()) {
    //    cerr << "Error: could not open file " << goodRunFileName << "\n";
    //    return 2;
    //}

    //H1RunList* goodRunList
    //    = (H1RunList*)goodRunFile.Get("H1RunList");

    //if (!goodRunList) {
    //    cerr << "Error: no runlist in file - return!\n";
    //    return 2;
    //}

    //H1DetectorStatus* detectorStatus
    //    = (H1DetectorStatus*)goodRunFile.Get("MyDetectorStatus");

    //if (!detectorStatus) {
    //    cerr << "Error: no detector status in file - return!\n";
    //    return 3;
    //}

    //goodRunFile.Close();
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Defining TTrees to syore event info
    TTree* EventTree = new TTree("Data Event Tree", "Data Event Tree");
    double weight1, weight2, elecPhi, elecTheta, Epz, elecE, x, xGen, Q2s, Ys, Q2sGen, YsGen, RTheta, ClusterRadius, Zvertex, ProtonEnergy, ElectronEnergy, Xcluster, Ycluster, Zcluster, RunNumber, RunLumi, fposEX, fposEY; //, RCluster,;
    //Defining Tree Branches
    EventTree->Branch("elecPhi", &elecPhi, "elecPhi/D");
    EventTree->Branch("elecTheta", &elecTheta, "elecTheta/D");
    EventTree->Branch("x", &x, "x/D");
    EventTree->Branch("Q2s", &Q2s, "Q2s/D");
    EventTree->Branch("Ys", &Ys, "Ys/D");
    EventTree->Branch("Q2sGen", &Q2sGen, "Q2sGen/D");
    EventTree->Branch("YsGen", &YsGen, "YsGen/D");
    EventTree->Branch("Epz", &Epz, "Epz/D");
    EventTree->Branch("elecE", &elecE, "elecE/D");
    EventTree->Branch("RTheta", &RTheta, "RTheta/D");
    EventTree->Branch("ClusterRadius", &ClusterRadius, "ClusterRadius/D");
    EventTree->Branch("Zvertex", &Zvertex, "Zvertex/D");
    EventTree->Branch("ProtonEnergy", &ProtonEnergy, "ProtonEnergy/D");
    EventTree->Branch("ElectronEnergy", &ElectronEnergy, "ElectronEnergy/D");
    EventTree->Branch("Xcluster", &Xcluster, "Xcluster/D");
    EventTree->Branch("Ycluster", &Ycluster, "Ycluster/D");
    EventTree->Branch("Zcluster", &Zcluster, "Zcluster/D");
    EventTree->Branch("RunNumber", &RunNumber, "RunNumber/D");
    EventTree->Branch("RunLumi", &RunLumi, "RunLumi/D");
    EventTree->Branch("fRunType", &fRunType, "fRunType/I");
    EventTree->Branch("fMyRunYear", &fMyRunYear, "fMyRunYear/I");
    EventTree->Branch("fposEX", &fposEX, "fposEX/D");
    EventTree->Branch("fposEY", &fposEY, "fposEY/D");
    EventTree->Branch("weight1", &weight1, "weight1/D");
    EventTree->Branch("weight2", &weight2, "weight2/D");

    //TTree* EventTreeBeforeSelections = new TTree("Data Event Tree Before Selections", "Data Event Tree Before Selections");
    ////double weight1, weight2, elecPhi, elecTheta, Epz, elecE, x, xGen, Q2s, Ys, Q2sGen, YsGen, RTheta, ClusterRadius, Zvertex, ProtonEnergy, ElectronEnergy, Xcluster, Ycluster, Zcluster, RunNumber, RunLumi, fposEX, fposEY; //, RCluster,;
    ////Defining Tree Branches
    //EventTreeBeforeSelections->Branch("elecPhi", &elecPhi, "elecPhi/D");
    //EventTreeBeforeSelections->Branch("elecTheta", &elecTheta, "elecTheta/D");
    //EventTreeBeforeSelections->Branch("x", &x, "x/D");
    //EventTreeBeforeSelections->Branch("Q2s", &Q2s, "Q2s/D");
    //EventTreeBeforeSelections->Branch("Ys", &Ys, "Ys/D");
    //EventTreeBeforeSelections->Branch("Q2sGen", &Q2sGen, "Q2sGen/D");
    //EventTreeBeforeSelections->Branch("YsGen", &YsGen, "YsGen/D");
    //EventTreeBeforeSelections->Branch("Epz", &Epz, "Epz/D");
    //EventTreeBeforeSelections->Branch("elecE", &elecE, "elecE/D");
    //EventTreeBeforeSelections->Branch("RTheta", &RTheta, "RTheta/D");
    //EventTreeBeforeSelections->Branch("ClusterRadius", &ClusterRadius, "ClusterRadius/D");
    //EventTreeBeforeSelections->Branch("Zvertex", &Zvertex, "Zvertex/D");
    //EventTreeBeforeSelections->Branch("ProtonEnergy", &ProtonEnergy, "ProtonEnergy/D");
    //EventTreeBeforeSelections->Branch("ElectronEnergy", &ElectronEnergy, "ElectronEnergy/D");
    //EventTreeBeforeSelections->Branch("Xcluster", &Xcluster, "Xcluster/D");
    //EventTreeBeforeSelections->Branch("Ycluster", &Ycluster, "Ycluster/D");
    //EventTreeBeforeSelections->Branch("Zcluster", &Zcluster, "Zcluster/D");
    //EventTreeBeforeSelections->Branch("RunNumber", &RunNumber, "RunNumber/D");
    //EventTreeBeforeSelections->Branch("RunLumi", &RunLumi, "RunLumi/D");
    //EventTreeBeforeSelections->Branch("fRunType", &fRunType, "fRunType/I");
    //EventTreeBeforeSelections->Branch("fMyRunYear", &fMyRunYear, "fMyRunYear/I");
    //EventTreeBeforeSelections->Branch("fposEX", &fposEX, "fposEX/D");
    //EventTreeBeforeSelections->Branch("fposEY", &fposEY, "fposEY/D");
    //EventTreeBeforeSelections->Branch("weight1", &weight1, "weight1/D");
    //EventTreeBeforeSelections->Branch("weight2", &weight2, "weight2/D");

    TTree* MigrationEventTree = new TTree("Event Migrations", "Event Migrations");
    MigrationEventTree->Branch("x", &x, "x/D");
    MigrationEventTree->Branch("Q2s", &Q2s, "Q2s/D");
    MigrationEventTree->Branch("Ys", &Ys, "Ys/D");
    MigrationEventTree->Branch("Q2sGen", &Q2sGen, "Q2sGen/D");
    MigrationEventTree->Branch("YsGen", &YsGen, "YsGen/D");
    MigrationEventTree->Branch("weight1", &weight1, "weight1/D");
    MigrationEventTree->Branch("weight2", &weight2, "weight2/D");

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //initializing counts, counting events after each selection just as a consistency check
    double TriggerEvents = 0;
    double Q2Events = 0;
    double yEvents = 0;
    double eEnergyEvents = 0;
    double zvtxEvents = 0;
    double ClusterREvents = 0;
    double NoScattPartEm = 0;
    double EnergyBalanceEvents = 0;
    double SpaCalEvents = 0;
    double GoodDetectorEvents = 0;
    double GoodRunEvents = 0;
    //double DeadCellEvents = 0;
    //double DeadTriggerEvents = 0;
    //double RadClusterEvents = 0;
    double BoxCutEvents = 0;
    double ElipseCutEvents = 0;
    double nonalignedspacalcuts = 0;
    double alignedspacalcuts = 0;

    //initializing PartEM checks
    double scatelCount = 0;

    
  
    Int_t sel=0;
    gH1Calc = H1Calculator::Instance();
    while (H1Tree::Instance()->Next()) {
        gH1Calc->Reset();
        TriggerEvents++;
        //H1Track* ProtonH1Track = (H1Track*)LambdaCand[loop]->GetProtonTrack();
        //H1RunLumi* gH1Lumi = (H1RunLumi*)gH1Calc;
        //fRunType = gH1Calc->IsMC(); //0 for data and 1 for MC
        fRunType = *RunType;
        //cout << fRunType << endl;
        RunNumber = H1Tree::Instance()->GetRunNumber();
        //RunLumi = H1LumiYield::Instance()->GetRunLumi(RunNumber);
        //RunLumi = gH1Lumi->GetCorrectedLumi();
        //cout << "Run Lumi: " << RunLumi << endl;
        //cout << "Run Number: " << RunNumber << endl;
        //cout << "IsMC: " << gH1Calc->IsMC() << endl;

       scatelCount = 0;

        for (Int_t loop = 0; loop < PartEm.GetEntries(); loop++) {
            
            if (PartEm[loop]->IsScatElec() == true) { 
                scatelCount++;
                //ClusterRadius = PartEm[loop]->GetRadiusFromVectors();
                ClusterRadius = PartEm[loop]->GetEcra();
                Xcluster = PartEm[loop]->GetXClus();
                Ycluster = PartEm[loop]->GetYClus();
                Zcluster = PartEm[loop]->GetZClus();
                fElecID = loop;
            }
        }

        //cout << "Scattered e count: " << scatelCount << endl;

        
        //Fill Event Tree Here before making any selections
        //x = ((*eE) / (*EBeamP)) * (cos((*ElecTheta) / (2))) * (cos((*ElecTheta) / (2))) / (*ys);
        //Q2s = *q2s;
        //Ys = *ys;
        //Q2sGen = *q2sGen;
        //YsGen = *ysGen;
        //weight1 = *w1;
        //weight2 = *w2;

        //elecE = *eE;
        //Epz = *epz;
        //elecPhi = *ElecPhi;
        //elecTheta = *ElecTheta;
        //RTheta = abs((*vtxZ + 160) * tan(*ElecTheta));
        //Zvertex = *vtxZ;
        //ProtonEnergy = *EBeamP;
        //ElectronEnergy = *EBeamE;
        ////fposEX = fposE[0];
        ////fposEY = fposE[1];
        //weight1 = *w1;
        //weight2 = *w2;
        //EventTreeBeforeSelections->Fill();

        /////////////////////////////////////////////////////
        
        //if (*q2s <= 5 || *q2s >= 100) { continue; } // 5 < Q2 [GeV] < 100
        //Q2Events++;
        //
        //if (*ys <= 0.1 || *ys >= 0.6) { continue; } // 0.1 < y < 0.6
        //yEvents++;

        if (*eE <= 11) { continue; } //E_electron [GeV] > 11
        eEnergyEvents++;

        if (*vtxZ<=-30 || *vtxZ>=30) { continue; } //-30 < Z_vertex [cm] < 30
        zvtxEvents++;

        if (*epz<=35 || *epz>=70) { continue; } // 35 < E-pz [GeV] < 70
        EnergyBalanceEvents++;

        //if (abs((*vtxZ + 160) * tan(*ElecTheta)) < 9.1) { continue; } //R_Theta [cm] > 9.1
        //SpaCalEvents++;

        if (scatelCount != 1) {
            //cout << "Scattered Electron Count != 1" << endl;
            continue;
        } //skip event if scattered electron is not found in PartEm class
        NoScattPartEm++;

        if (ClusterRadius > 3.5) { continue; } // e Cluster Radius [cm] < 3.5
        ClusterREvents++;

        if (gH1Calc->IsMC() == 0) {//If this is data, we must exclude bad runs, all simulated MC runs are good
            // skip runs not in list of good runs
            if (!goodRunList->FindRun(H1Tree::Instance()->GetRunNumber())) { continue; }
            GoodRunEvents++;
            // skip data events with bad detector status
            if (!detectorStatus->IsOn()) { continue; }
            GoodDetectorEvents++;
        }


        hXYclusterBefore->Fill(Xcluster,Ycluster);


     ///////////////////////////////////////////////////////////////////////////////////////
        //SpaCal box cut for scattered electron
        if ((Xcluster >= 9.0 && Xcluster <= 16.24) && (Ycluster >= -4.105 && Ycluster <= 4.377)) { continue; }
        BoxCutEvents++;


        //SpaCal elipse cut 
        if ((Xcluster + 78.4461) * (Xcluster + 78.4461) / ((11.027) * (11.027)) + (Ycluster) * (Ycluster) / ((20.5) * (20.5)) < 1.0) { continue; }
        ElipseCutEvents++;

     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Begin SpaCal Cuts
        const Int_t RunPerN = 4;
        Int_t RUN_PERIOD_DOWN[RunPerN] = { 399101, 444094, 468530, 492508 };
        Int_t RUN_PERIOD_UP[RunPerN] = { 436894, 466998, 492500, 500612 };

        static H1IntPtr RunNumberHat("RunNumber");
        Int_t RunPerChanger = *RunNumberHat;  /// Using real Data Run Numbers we deteminated Run Period

        //TString YEAR[4] = { "2005e-", "2006e-", "2006e+", "2007e+" };

        for (Int_t i = 0; i < RunPerN; i++) {
            if ((RunPerChanger >= RUN_PERIOD_DOWN[i]) && (RunPerChanger <= RUN_PERIOD_UP[i])) fMyRunYear = i;
        }
        //cout << RunPerChanger << endl;
        //cout << fMyRunYear << endl;
        //cout << "=====================   MyRunPeriod = " << YEAR[MyRunYear] << endl;

        /*switch (MyRunYear) {
        case 0: RunYear = 8;  break;
        case 1: RunYear = 9;  break;
        case 2: RunYear = 10; break;
        case 3: RunYear = 10; break;
        default: cout << "Run-Year Out of Analysing Range:" << endl;
        }*/

        ////This cut seems to only be for data
        //if (FiducialCut(Xcluster, Ycluster, RunYear) == false) { continue; } //FiducialCut funciton returns true if all initial cuts are passed, there are some year dependent cuts 

        static elecCut myElecCut = 0;
        bool bFiducialCuts = myElecCut.goodElec(PartEm[fElecID], RunNumber);
        if (!bFiducialCuts) { continue; }
        if (bFiducialCuts == 0) { continue; }
        Alignment();
        //Alignment fSpacAlignment;
        // --- spacal alignment
        SetSpacalAlignmentConstants();

        const TLorentzVector& ScattElec = gH1Calc->Elec()->GetFirstElectron();
        fElecE = ScattElec.E();

        // ------------------------------------- //
    // --- --- calculate Spacal Elec --- --- //
        const  double          charge = fMyRunYear > 1 ? +1 : -1;
        // --- spacal alignment
        SpacLinearity();
        //SpacLinearity fSpacLinearity;
        
        // --- realign electron cluster
        fposE = TVector3(Xcluster, Ycluster, Zcluster); // HAT-cluster position 
        fposE = GetPosCorr(fposE, fLinearityGrid, fLinearityOffset); // spacal linearity
        fposE = Transform(fposE); // spacal alignment
        fposE -= TVector3(gH1Calc->Vertex()->GetX(), gH1Calc->Vertex()->GetY(), gH1Calc->Vertex()->GetZ());// subtract vertex pos
        // spacal electron four-vector
        fElecSpacAlign = GetHelixMom(fElecE, fposE, charge);
        DoTilt(fElecSpacAlign, TVector2((*btx), (*bty)));

        // --- calculate cutR
        // correct for spacal nom. position (not needed for 4-vector)
        const double spacalnom = -161;
        fposE *= (spacalnom - *vtxZ) / fposE[2]; // correct for spacal nom. position
        // beam tilt correction
        fposE[0] -= (*btx) * fposE[2];
        fposE[1] -= (*bty) * fposE[2];
        fCutR = fposE.Pt();
    
//// --- spacal alignment
//        SpacLinearity();
//        //SpacLinearity fSpacLinearity;
//        Alignment();
//        //Alignment fSpacAlignment;

    // non-aligned
        if (Xcluster > -16 && Xcluster<0 && Ycluster > -10 && Ycluster < 6) { continue; } //bFiducialCuts = false; // all y, eE regions bad
        if (Xcluster > -18 && Xcluster<0 && Ycluster > -4 && Ycluster < 2) { continue; } //bFiducialCuts = false; // all y, eE regions bad
        nonalignedspacalcuts++;
       Float_t fLinearityGrid[2];
       Float_t fLinearityOffset[2];
       //SpacLinearity fSpacLinearity;
       //Alignment fSpacAlignment;
                                                                                         // aligned
        if (fposE[0] > -14 && fposE[0] < 10 && fposE[1] >  0 && fposE[1] < 14) { continue; } //bFiducialCuts = false; // all y, eE regions bad
        if (fposE[0] > -14 && fposE[0]<8 && fposE[1] > -12 && fposE[1] < 0) { continue; } //bFiducialCuts = false; // all y, eE regions bad
        if (fCutR < 12.5) { continue; } //bFiducialCuts = false;
        //if (fCutR > c_Spacal_Rxy_max - 0.5) { continue; } //bFiducialCuts = false;
        if (fCutR > 74.0 - 0.5) { continue; } //bFiducialCuts = false;
        alignedspacalcuts++;

    // End SpaCal Cuts
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        //Fill Event Migration Tree Before Applying Phase Space Cuts
        x = ((*eE) / (*EBeamP)) * (cos((*ElecTheta) / (2))) * (cos((*ElecTheta) / (2))) / (*ys);
        Q2s = *q2s;
        Ys = *ys;
        Q2sGen = *q2sGen;
        YsGen = *ysGen;
        weight1 = *w1;
        weight2 = *w2;
        MigrationEventTree->Fill();

    //Phase Space Cuts

        if (*q2s <= 5 || *q2s >= 100) { continue; } // 5 < Q2 [GeV] < 100
        Q2Events++;

        if (*ys <= 0.1 || *ys >= 0.6) { continue; } // 0.1 < y < 0.6
        yEvents++;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        hXYclusterAfter->Fill(Xcluster, Ycluster);
       

      if (*ys < 0.6 && *ys>0.1 && *q2s > 5 && *q2s < 100 && *eE>10 && *epz > 35 && *epz < 70 && *vtxZ < 30 && *vtxZ > -30)    {
        //cout << "E: " << *Echeck << endl;
        //cout << "EPZ: " << *epz << endl;
        //cout << "EPZ calc: " <<  ( * ElecE) + abs((*ElecE)*cos(*ElecTheta)) + (*HadEX)-(*HadPzX) << endl;
        //cout << "EPZ calc: " <<  ( * ElecE) + abs((*ElecE)*cos(*ElecTheta)) + (*hfsE) - abs((*hfsE) * cos(*hfsTheta)) << endl;
          
          gH1Constants->SetConstants(gH1Calc->GetRunNumber()); // SetConstants takes either period or run

          int RunPeriod = gH1Constants->GetRunPeriod();

         /* cout << "Run Period                          = ";
          if (RunPeriod == H1Constants::eEplus9900)  cout << "e+ 99/00"
              << endl;
          else if (RunPeriod == H1Constants::eEplus0304)  cout << "e+ 03/04"
              << endl;
          else if (RunPeriod == H1Constants::eEminus0405) cout << "e- 04/05"
              << endl;
          else if (RunPeriod == H1Constants::eEminus06)   cout << "e- 06" <<
              endl;
          else if (RunPeriod == H1Constants::eEplus0607)  cout << "e+ 06/07"
              << endl;
          else {
              Error("AnalysisBase::InitialSettings", "Unkown Run Period. Please correct the steering.");
                  exit(1);
              cout << RunPeriod << endl;
          }*/


          elecE = *eE;
          Epz = *epz;
          elecPhi = *ElecPhi;
          elecTheta = *ElecTheta;
          //x = (*q2e) / ((*ye) * 920 * 27.6 * 4); //Bjorken X calculated from Electron Method
          x = ((*eE) / (*EBeamP)) * (cos((*ElecTheta) / (2))) * (cos((*ElecTheta) / (2))) / (*ys);
          Q2s = *q2s;
          Ys = *ys;
          RTheta = abs((*vtxZ + 160) * tan(*ElecTheta));
          Zvertex = *vtxZ;
          ProtonEnergy = *EBeamP;
          ElectronEnergy = *EBeamE;
          fposEX = fposE[0];
          fposEY = fposE[1];
          weight1 = *w1;
          weight2 = *w2;
          Q2sGen = *q2sGen;
          YsGen = *ysGen;
          EventTree->Fill();

          sel++;
         
          list->Enter();
      }
    }
    cout << "Selected "<<sel<<" entries"<<endl;

    cout << "check" << endl;
      // Write eventlist to file
    TFile file(opts.GetOutput(), "RECREATE");
    list->Write();
    list->Print();
    // close
    //file.Write();
    //file.Close();

    //Write Event Distributions to File
    //TFile fileEventDist("/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/EventDists06.root","RECREATE");
    //string str1 = "123";
    //string str2 = str1.substr(0, str1.length() - 1);

    //string StdDirectory = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/";
    //string StdOutput = opts.GetOutput();
    //string EventDistsOutput = StdOutput.substr(0, StdOutput.length()-5) + "_EventDists06.root";
    //TFile fileEventDist(EventDistsOutput.c_str(), "RECREATE");
    //fileEventDist.cd();
    EventTree->Write();

    MigrationEventTree->Write();

    //EventTreeBeforeSelections->Write();

    //hXYclusterBefore->SetStats(0);
    //hXYclusterBefore->GetXaxis()->SetTitle("X");
    //hXYclusterBefore->GetYaxis()->SetTitle("Y");
    ////XYvtxdensitytot->GetXaxis()->SetTitleSize(0.03);
    ////XYvtxdensitytot->GetXaxis()->SetTitleOffset(1);
    ////XYvtxdensitytot->GetYaxis()->SetTitleOffset(1);
    ////XYvtxdensitytot->GetYaxis()->SetTitleSize(0.03);
    //hXYclusterBefore->Draw("colz");
    //canvas->SetName("SpaCal XY Positions Before");
    //canvas->Write();

    //hXYclusterAfter->SetStats(0);
    //hXYclusterAfter->GetXaxis()->SetTitle("X");
    //hXYclusterAfter->GetYaxis()->SetTitle("Y");
    ////XYvtxdensitytot->GetXaxis()->SetTitleSize(0.03);
    ////XYvtxdensitytot->GetXaxis()->SetTitleOffset(1);
    ////XYvtxdensitytot->GetYaxis()->SetTitleOffset(1);
    ////XYvtxdensitytot->GetYaxis()->SetTitleSize(0.03);
    //hXYclusterAfter->Draw("colz");
    //canvas->SetName("SpaCal XY Positions After");
    //canvas->Write();

    file.Write();
    file.Close();


    cout << "Eventlist written to " << opts.GetOutput() << endl;
    //cout << "Event Dists written to " << EventDistsOutput << endl;

    //initial amount of events in 2006 e+p and e-p: 
    cout << "Events After Trigger: " << TriggerEvents << endl;
    cout << "Events After Q2: " << Q2Events << endl;
    cout << "Events After y: " << yEvents << endl;
    cout << "Events After eE: " << eEnergyEvents << endl;
    cout << "Events After zvtx: " << zvtxEvents << endl;
    cout << "Events After Epz: " << EnergyBalanceEvents << endl;
    cout << "Events After RTheta: " << SpaCalEvents << endl;
    cout << "Events After Requiring Part Em Scattered electron: " << NoScattPartEm << endl;
    cout << "Events After Cluster Radius: " << ClusterREvents << endl;
    cout << "Events Run Check: " << GoodRunEvents << endl;
    cout << "Events After Detector Check: " << GoodDetectorEvents << endl;
    //cout << "Events After Dead Cell Check: " << DeadCellEvents << endl;
    //cout << "Events After Dead Trigger Check: " << DeadTriggerEvents << endl;
    //cout << "Events After Radial Cluster Pos Check: " << RadClusterEvents << endl;
    //cout << "Events After Trigger: " << TriggerEvents << endl;
    cout << "Events After Box Cut: " << BoxCutEvents << endl;
    cout << "Events After Elipse Cut: " << ElipseCutEvents << endl;
    cout << "Events After Non Aligned Spa Cal Cuts: " << nonalignedspacalcuts << endl;
    cout << "Events After Aligned Spa Cal Cuts: " << alignedspacalcuts << endl;

    //double TriggerEvents = 0;
    //double Q2Events = 0;
    //double yEvents = 0;
    //double eEnergyEvents = 0;
    //double zvtxEvents = 0;
    //double ClusterREvents = 0;
    //double EnergyBalanceEvents = 0;
    //double SpaCalEvents = 0;
    
    return 0;
}


