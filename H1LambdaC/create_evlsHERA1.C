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

    
    static H1ShortPtr RunType("RunType"); //Q2 variable aquired using sigma method

    H1FloatPtr w1("Weight1");
    H1FloatPtr w2("Weight2");

    //Defining Root Canvas to work on
    TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 800);

    TH2F* hXYclusterBefore = new TH2F("XY Cluster Position Before", "XY Cluster Position Before", 1000, -80, 80, 1000, -80, 80);
    TH2F* hXYclusterAfter = new TH2F("XY Cluster Position After", "XY Cluster Position After", 1000, -80, 80, 1000, -80, 80);

    static H1PartEmArrayPtr PartEm;


    



    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Consistenct with Lumi calculation using SelectedRuns.root oolumi output file
    TString goodRunFileName("/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/SelectedRuns.root");
    TFile goodRunFile(goodRunFileName);
    if (!goodRunFile.IsOpen()) {
        cerr << "Error: could not open file " << goodRunFileName << "\n";
        return 2;
    }

    H1RunList* goodRunList
        = (H1RunList*)goodRunFile.Get("H1RunList");

    if (!goodRunList) {
        cerr << "Error: no runlist in file - return!\n";
        return 2;
    }

    H1DetectorStatus* detectorStatus
        = (H1DetectorStatus*)goodRunFile.Get("MyDetectorStatus");

    if (!detectorStatus) {
        cerr << "Error: no detector status in file - return!\n";
        return 3;
    }

    goodRunFile.Close();
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Defining TTrees to syore event info
    TTree* EventTree = new TTree("Data Event Tree", "Data Event Tree");
    double weight1, weight2, elecPhi, elecTheta, Epz, elecE, x, Q2s, Ys, Q2sGen, YsGen, RTheta, ClusterRadius, Zvertex, ProtonEnergy, ElectronEnergy, Xcluster, Ycluster, Zcluster, RunNumber, RunLumi, fposEX, fposEY; //, RCluster,;
    int fRunType;		// fRunType: 0=data, 8=Django, 11=Rapgap
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

        if (gH1Calc->IsMC()==0) {//If this is data, we must exclude bad runs, all simulated MC runs are good
            // skip runs not in list of good runs
            if (!goodRunList->FindRun(H1Tree::Instance()->GetRunNumber())) { continue; }
            GoodRunEvents++;
            // skip data events with bad detector status
            if (!detectorStatus->IsOn()) { continue; }
            GoodDetectorEvents++;
        }


        hXYclusterBefore->Fill(Xcluster,Ycluster);


     /////////////////////////////////////////////////////////////////////////////////////////
     //   //SpaCal box cut for scattered electron
     //   if ((Xcluster >= 9.0 && Xcluster <= 16.24) && (Ycluster >= -4.105 && Ycluster <= 4.377)) { continue; }
     //   BoxCutEvents++;


     //   //SpaCal elipse cut 
     //   if ((Xcluster + 78.4461) * (Xcluster + 78.4461) / ((11.027) * (11.027)) + (Ycluster) * (Ycluster) / ((20.5) * (20.5)) < 1.0) { continue; }
     //   ElipseCutEvents++;

     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Begin SpaCal Cuts
        //const Int_t RunPerN = 4;
        //Int_t RUN_PERIOD_DOWN[RunPerN] = { 399101, 444094, 468530, 492508 };
        //Int_t RUN_PERIOD_UP[RunPerN] = { 436894, 466998, 492500, 500612 };

        //static H1IntPtr RunNumberHat("RunNumber");
        //Int_t RunPerChanger = *RunNumberHat;  /// Using real Data Run Numbers we deteminated Run Period

        ////TString YEAR[4] = { "2005e-", "2006e-", "2006e+", "2007e+" };

        //for (Int_t i = 0; i < RunPerN; i++) {
        //    if ((RunPerChanger >= RUN_PERIOD_DOWN[i]) && (RunPerChanger <= RUN_PERIOD_UP[i])) fMyRunYear = i;
        //}
        ////cout << RunPerChanger << endl;
        ////cout << fMyRunYear << endl;
        ////cout << "=====================   MyRunPeriod = " << YEAR[MyRunYear] << endl;

        ///*switch (MyRunYear) {
        //case 0: RunYear = 8;  break;
        //case 1: RunYear = 9;  break;
        //case 2: RunYear = 10; break;
        //case 3: RunYear = 10; break;
        //default: cout << "Run-Year Out of Analysing Range:" << endl;
        //}*/

        //////This cut seems to only be for data
        ////if (FiducialCut(Xcluster, Ycluster, RunYear) == false) { continue; } //FiducialCut funciton returns true if all initial cuts are passed, there are some year dependent cuts 

        //static elecCut myElecCut = 0;
        //bool bFiducialCuts = myElecCut.goodElec(PartEm[fElecID], RunNumber);
        //if (!bFiducialCuts) { continue; }
        //if (bFiducialCuts == 0) { continue; }
        //Alignment();
        ////Alignment fSpacAlignment;
        //// --- spacal alignment
        //SetSpacalAlignmentConstants();

        const TLorentzVector& ScattElec = gH1Calc->Elec()->GetFirstElectron();
        fElecE = ScattElec.E();

        // ------------------------------------- //
    // --- --- calculate Spacal Elec --- --- //
        //const  double          charge = fMyRunYear > 1 ? +1 : -1;
        //// --- spacal alignment
        //SpacLinearity();
        ////SpacLinearity fSpacLinearity;
        
        // --- realign electron cluster
        //fposE = TVector3(Xcluster, Ycluster, Zcluster); // HAT-cluster position 
        //fposE = GetPosCorr(fposE, fLinearityGrid, fLinearityOffset); // spacal linearity
        //fposE = Transform(fposE); // spacal alignment
        //fposE -= TVector3(gH1Calc->Vertex()->GetX(), gH1Calc->Vertex()->GetY(), gH1Calc->Vertex()->GetZ());// subtract vertex pos
        //// spacal electron four-vector
        //fElecSpacAlign = GetHelixMom(fElecE, fposE, charge);
        //DoTilt(fElecSpacAlign, TVector2((*btx), (*bty)));

        //// --- calculate cutR
        //// correct for spacal nom. position (not needed for 4-vector)
        //const double spacalnom = -161;
        //fposE *= (spacalnom - *vtxZ) / fposE[2]; // correct for spacal nom. position
        //// beam tilt correction
        //fposE[0] -= (*btx) * fposE[2];
        //fposE[1] -= (*bty) * fposE[2];
        //fCutR = fposE.Pt();
    
//// --- spacal alignment
//        SpacLinearity();
//        //SpacLinearity fSpacLinearity;
//        Alignment();
//        //Alignment fSpacAlignment;

    // non-aligned
       // if (Xcluster > -16 && Xcluster<0 && Ycluster > -10 && Ycluster < 6) { continue; } //bFiducialCuts = false; // all y, eE regions bad
       // if (Xcluster > -18 && Xcluster<0 && Ycluster > -4 && Ycluster < 2) { continue; } //bFiducialCuts = false; // all y, eE regions bad
       // nonalignedspacalcuts++;
       //Float_t fLinearityGrid[2];
       //Float_t fLinearityOffset[2];
       ////SpacLinearity fSpacLinearity;
       ////Alignment fSpacAlignment;
       //                                                                                  // aligned
       // if (fposE[0] > -14 && fposE[0] < 10 && fposE[1] >  0 && fposE[1] < 14) { continue; } //bFiducialCuts = false; // all y, eE regions bad
       // if (fposE[0] > -14 && fposE[0]<8 && fposE[1] > -12 && fposE[1] < 0) { continue; } //bFiducialCuts = false; // all y, eE regions bad
       // if (fCutR < 12.5) { continue; } //bFiducialCuts = false;
       // //if (fCutR > c_Spacal_Rxy_max - 0.5) { continue; } //bFiducialCuts = false;
       // if (fCutR > 74.0 - 0.5) { continue; } //bFiducialCuts = false;
       // alignedspacalcuts++;

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


