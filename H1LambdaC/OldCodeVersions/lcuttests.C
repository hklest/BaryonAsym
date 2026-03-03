///////////////////////////////////////////////////////
// K0 finder on ODS
// 
// Modified version of kaonfind_ods in H1Examples
// 
// Author     : wessling
// Created    : 2002/01/23
// Last update: $Date: 2006/09/22 09:00:40 $ (UTC)
//          by: $Author: ozerov $
//
///////////////////////////////////////////////////////

// General Includes
#include <iostream>
#include <stdlib.h>
// ROOT includes
#include <TFile.h>
#include <THStack.h>
#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>
#include <TClonesArray.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TDatabasePDG.h>
#include <TApplication.h>
     //Added for fitting
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

// H1 OO includes
#include "H1Skeleton/H1Tree.h"
#include "H1Geom/H1DBManager.h"

// tracks
#include "H1Tracks/H1TrackEvent.h"
#include "H1Tracks/H1Track.h"
#include "H1Tracks/H1Trajectory.h"
#include "H1Tracks/H1CentralFittedTrack.h"
#include "H1Tracks/H1CentralFittedV0.h"
#include "H1Tracks/H1CentralTrack.h"
#include "H1Mods/H1PartSelTrack.h"
#include "H1Mods/H1PartSelTrackArrayPtr.h"
#include "H1Mods/H1PartJPsi.h"
#include "H1Mods/H1PartJPsiArrayPtr.h"
#include "H1Mods/H1PartCand.h"
#include "H1Mods/H1PartCandArrayPtr.h"
#include "H1Mods/H1PartMCArrayPtr.h"
#include "H1Mods/H1PartLambdaArrayPtr.h"
#include "H1Mods/H1PartLambda.h"
#include "H1Mods/H1SelVertex.h"
//#include "H1Tracks/H1CentralFittedTrackPtr.h"
// vertices
#include "H1Tracks/H1CentralVertex.h"
#include "H1Tracks/H1CentralVertexArrayPtr.h"
#include "H1Steering/H1StdCmdLine.h"


using namespace std;


int
main(int argc, char* argv[])
{
    // event counter
    Int_t eventCounter = 0;
    // parse the command line
    H1StdCmdLine opts;
    opts.Parse(&argc, argv);
    // needed for graphic, but MUST be AFTER Parse(...):
    TApplication theApp("kaonfind_ods", &argc, argv);

    // Load mODS/HAT files
    H1Tree::Instance()->Open();            // this statement must be here

    // Histograms to fill
    Double_t kaonbins = 500;
    Double_t lambdabins = 1000;
    Double_t kaonupperbinrange = 1.0;
    Double_t kaonlowerbinrange = 0.0;
    Double_t lambdaupperbinrange = 1.5;
    Double_t lambdalowerbinrange = 1.0;
    //Defining upper and lower bounds for different pt binning, each range with own histogram
    Double_t ptbin1l = 0.5;
    Double_t ptbin1u = 0.6;

    Double_t ptbin2l = 0.6;
    Double_t ptbin2u = 0.7;

    Double_t ptbin3l = 0.7;
    Double_t ptbin3u = 0.8;

    Double_t ptbin4l = 0.8;
    Double_t ptbin4u = 0.9;

    Double_t ptbin5l = 0.9;
    Double_t ptbin5u = 1.1;

    Double_t ptbin6l = 1.1;
    Double_t ptbin6u = 1.3;

    Double_t ptbin7l = 1.3;
    Double_t ptbin7u = 1.6;

    Double_t ptbin8l = 1.6;
    Double_t ptbin8u = 2.2;

    Double_t ptbin9l = 2.2;
    Double_t ptbin9u = 3.5;


    Double_t kaonbinw = (kaonupperbinrange-kaonlowerbinrange) / kaonbins;
    Double_t lambdabinw = (lambdaupperbinrange-lambdalowerbinrange) / lambdabins;

    //Stack to plot multiple histograms, want total and also for each pt bin
    THStack* hs = new THStack("hs", "Stacked \\Lambda Mass Plots");
    THStack* hs1 = new THStack("hs1", "Stacked \\Lambda Mass Plots pT Bin 1");
    THStack* hs2 = new THStack("hs2", "Stacked \\Lambda Mass Plots pT Bin 2");
    THStack* hs3 = new THStack("hs3", "Stacked \\Lambda Mass Plots pT Bin 3");
    THStack* hs4 = new THStack("hs4", "Stacked \\Lambda Mass Plots pT Bin 4");
    THStack* hs5 = new THStack("hs5", "Stacked \\Lambda Mass Plots pT Bin 5");
    THStack* hs6 = new THStack("hs6", "Stacked \\Lambda Mass Plots pT Bin 6");
    THStack* hs7 = new THStack("hs7", "Stacked \\Lambda Mass Plots pT Bin 7");
    THStack* hs8 = new THStack("hs8", "Stacked \\Lambda Mass Plots pT Bin 8");
    THStack* hs9 = new THStack("hs9", "Stacked \\Lambda Mass Plots pT Bin 9");
    //Total Lambda plot, diff hLambdas for different cuts, want this also for each bin, so 9 copies of these as well for 9 pt bins
    TH1F* hLambda = new TH1F("Lambda", "Lambda", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDproton = new TH1F("Lambda PID Proton", "Lambda PID Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDpion = new TH1F("Lambda PID Pion", "Lambda PID Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaAlpha = new TH1F("Lambda Alpha", "Lambda Alpha", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaptrel = new TH1F("Lambda pTrel", "Lambda pT_rel", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdapt = new TH1F("Lambda pT", "Lambda pT", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaEta = new TH1F("Lambda Psuedorapidity", "Lambda Psuedorapidity", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdatracklength = new TH1F("Lambda Track Length", "Lambda Track Length", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAproton = new TH1F("Lambda dDCA Proton", "Lambda dDCA Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    //Delete Below Later
    TH1F* hLambdaDCAoverdDCAprotontest1 = new TH1F("Lambda dDCA Proton test1", "Lambda dDCA Proton test1", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest2 = new TH1F("Lambda dDCA Proton test2", "Lambda dDCA Proton test2", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest3 = new TH1F("Lambda dDCA Proton test3", "Lambda dDCA Proton test3", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest4 = new TH1F("Lambda dDCA Proton test4", "Lambda dDCA Proton test4", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest5 = new TH1F("Lambda dDCA Proton test5", "Lambda dDCA Proton test5", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest6 = new TH1F("Lambda dDCA Proton test6", "Lambda dDCA Proton test6", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest7 = new TH1F("Lambda dDCA Proton test7", "Lambda dDCA Proton test7", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest8 = new TH1F("Lambda dDCA Proton test8", "Lambda dDCA Proton test8", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest9 = new TH1F("Lambda dDCA Proton test9", "Lambda dDCA Proton test9", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest10 = new TH1F("Lambda dDCA Proton test10", "Lambda dDCA Proton test10", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest11 = new TH1F("Lambda dDCA Proton test11", "Lambda dDCA Proton test11", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest12 = new TH1F("Lambda dDCA Proton test12", "Lambda dDCA Proton test12", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest13 = new TH1F("Lambda dDCA Proton test13", "Lambda dDCA Proton test13", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest14 = new TH1F("Lambda dDCA Proton test14", "Lambda dDCA Proton test14", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest15 = new TH1F("Lambda dDCA Proton test15", "Lambda dDCA Proton test15", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest16 = new TH1F("Lambda dDCA Proton test16", "Lambda dDCA Proton test16", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest17 = new TH1F("Lambda dDCA Proton test17", "Lambda dDCA Proton test17", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest18 = new TH1F("Lambda dDCA Proton test18", "Lambda dDCA Proton test18", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest19 = new TH1F("Lambda dDCA Proton test19", "Lambda dDCA Proton test19", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest20 = new TH1F("Lambda dDCA Proton test20", "Lambda dDCA Proton test20", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest21 = new TH1F("Lambda dDCA Proton test21", "Lambda dDCA Proton test21", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest22 = new TH1F("Lambda dDCA Proton test22", "Lambda dDCA Proton test22", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest23 = new TH1F("Lambda dDCA Proton test23", "Lambda dDCA Proton test23", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest24 = new TH1F("Lambda dDCA Proton test24", "Lambda dDCA Proton test24", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAprotontest25 = new TH1F("Lambda dDCA Proton test25", "Lambda dDCA Proton test25", lambdabins, lambdalowerbinrange, lambdaupperbinrange);

    TH1F* hLambdaDCAoverdDCApiontest1 = new TH1F("Lambda dDCA Pion test1", "Lambda dDCA Pion test1", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest2 = new TH1F("Lambda dDCA Pion test2", "Lambda dDCA Pion test2", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest3 = new TH1F("Lambda dDCA Pion test3", "Lambda dDCA Pion test3", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest4 = new TH1F("Lambda dDCA Pion test4", "Lambda dDCA Pion test4", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest5 = new TH1F("Lambda dDCA Pion test5", "Lambda dDCA Pion test5", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest6 = new TH1F("Lambda dDCA Pion test6", "Lambda dDCA Pion test6", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest7 = new TH1F("Lambda dDCA Pion test7", "Lambda dDCA Pion test7", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest8 = new TH1F("Lambda dDCA Pion test8", "Lambda dDCA Pion test8", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest9 = new TH1F("Lambda dDCA Pion test9", "Lambda dDCA Pion test9", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest10 = new TH1F("Lambda dDCA Pion test10", "Lambda dDCA Pion test10", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest11 = new TH1F("Lambda dDCA Pion test11", "Lambda dDCA Pion test11", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest12 = new TH1F("Lambda dDCA Pion test12", "Lambda dDCA Pion test12", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest13 = new TH1F("Lambda dDCA Pion test13", "Lambda dDCA Pion test13", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest14 = new TH1F("Lambda dDCA Pion test14", "Lambda dDCA Pion test14", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest15 = new TH1F("Lambda dDCA Pion test15", "Lambda dDCA Pion test15", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest16 = new TH1F("Lambda dDCA Pion test16", "Lambda dDCA Pion test16", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest17 = new TH1F("Lambda dDCA Pion test17", "Lambda dDCA Pion test17", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest18 = new TH1F("Lambda dDCA Pion test18", "Lambda dDCA Pion test18", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest19 = new TH1F("Lambda dDCA Pion test19", "Lambda dDCA Pion test19", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest20 = new TH1F("Lambda dDCA Pion test20", "Lambda dDCA Pion test20", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest21 = new TH1F("Lambda dDCA Pion test21", "Lambda dDCA Pion test21", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest22 = new TH1F("Lambda dDCA Pion test22", "Lambda dDCA Pion test22", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest23 = new TH1F("Lambda dDCA Pion test23", "Lambda dDCA Pion test23", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest24 = new TH1F("Lambda dDCA Pion test24", "Lambda dDCA Pion test24", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCApiontest25 = new TH1F("Lambda dDCA Pion test25", "Lambda dDCA Pion test25", lambdabins, lambdalowerbinrange, lambdaupperbinrange);

    TH1F* hLambdaPIDpiontest0 = new TH1F("Lambda PID Pion test0", "Lambda PID Pion test0", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDpiontest1 = new TH1F("Lambda PID Pion test1", "Lambda PID Pion test1", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDpiontest2 = new TH1F("Lambda PID Pion test2", "Lambda PID Pion test2", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDpiontest3 = new TH1F("Lambda PID Pion test3", "Lambda PID Pion test3", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDpiontest4 = new TH1F("Lambda PID Pion test4", "Lambda PID Pion test4", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDpiontest5 = new TH1F("Lambda PID Pion test5", "Lambda PID Pion test5", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDpiontest6 = new TH1F("Lambda PID Pion test6", "Lambda PID Pion test6", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDpiontest7 = new TH1F("Lambda PID Pion test7", "Lambda PID Pion test7", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDpiontest8 = new TH1F("Lambda PID Pion test8", "Lambda PID Pion test8", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDpiontest9 = new TH1F("Lambda PID Pion test9", "Lambda PID Pion test9", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDpiontest10 = new TH1F("Lambda PID Pion test10", "Lambda PID Pion test10", lambdabins, lambdalowerbinrange, lambdaupperbinrange);

    TH1F* hLambdaPIDprotontest0 = new TH1F("Lambda PID Proton test0", "Lambda PID Proton test0", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDprotontest1 = new TH1F("Lambda PID Proton test1", "Lambda PID Proton test1", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDprotontest2 = new TH1F("Lambda PID Proton test2", "Lambda PID Proton test2", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDprotontest3 = new TH1F("Lambda PID Proton test3", "Lambda PID Proton test3", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDprotontest4 = new TH1F("Lambda PID Proton test4", "Lambda PID Proton test4", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDprotontest5 = new TH1F("Lambda PID Proton test5", "Lambda PID Proton test5", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDprotontest6 = new TH1F("Lambda PID Proton test6", "Lambda PID Proton test6", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDprotontest7 = new TH1F("Lambda PID Proton test7", "Lambda PID Proton test7", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDprotontest8 = new TH1F("Lambda PID Proton test8", "Lambda PID Proton test8", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDprotontest9 = new TH1F("Lambda PID Proton test9", "Lambda PID Proton test9", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDprotontest10 = new TH1F("Lambda PID Proton test10", "Lambda PID Proton test10", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    //Deleter Above LAter
    TH1F* hLambdaDCAoverdDCApion = new TH1F("Lambda dDCA Pion", "Lambda dDCA Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaKaonCont = new TH1F("Lambda-Kaons", "Lambda Kaon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPhotonCont = new TH1F("Lambda-Photons", "Lambda Photon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);

    TH1F* h1Lambda = new TH1F("Lambda1", "Lambda1", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h1LambdaPIDproton = new TH1F("Lambda1 PID Proton", "Lambda1 PID Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h1LambdaPIDpion = new TH1F("Lambda1 PID Pion", "Lambda1 PID Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h1LambdaAlpha = new TH1F("Lambda1 Alpha", "Lambda1 Alpha", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h1Lambdaptrel = new TH1F("Lambda1 pTrel", "Lambda1 pT_rel", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h1Lambdapt = new TH1F("Lambda1 pT", "Lambda1 pT", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h1LambdaEta = new TH1F("Lambda1 Psuedorapidity", "Lambda1 Psuedorapidity", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h1Lambdatracklength = new TH1F("Lambda1 Track Length", "Lambda1 Track Length", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h1LambdaDCAoverdDCAproton = new TH1F("Lambda1 dDCA Proton", "Lambda1 dDCA Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h1LambdaDCAoverdDCApion = new TH1F("Lambda1 dDCA Pion", "Lambda1 dDCA Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h1LambdaKaonCont = new TH1F("Lambda-Kaons1", "Lambda1 Kaon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h1LambdaPhotonCont = new TH1F("Lambda-Photons1", "Lambda1 Photon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);

    TH1F* h2Lambda = new TH1F("Lambda1", "Lambda1", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h2LambdaPIDproton = new TH1F("Lambda1 PID Proton", "Lambda1 PID Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h2LambdaPIDpion = new TH1F("Lambda1 PID Pion", "Lambda1 PID Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h2LambdaAlpha = new TH1F("Lambda1 Alpha", "Lambda1 Alpha", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h2Lambdaptrel = new TH1F("Lambda1 pTrel", "Lambda1 pT_rel", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h2Lambdapt = new TH1F("Lambda1 pT", "Lambda1 pT", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h2LambdaEta = new TH1F("Lambda1 Psuedorapidity", "Lambda1 Psuedorapidity", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h2Lambdatracklength = new TH1F("Lambda1 Track Length", "Lambda1 Track Length", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h2LambdaDCAoverdDCAproton = new TH1F("Lambda1 dDCA Proton", "Lambda1 dDCA Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h2LambdaDCAoverdDCApion = new TH1F("Lambda1 dDCA Pion", "Lambda1 dDCA Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h2LambdaKaonCont = new TH1F("Lambda-Kaons1", "Lambda1 Kaon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h2LambdaPhotonCont = new TH1F("Lambda-Photons1", "Lambda1 Photon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);

    TH1F* h3Lambda = new TH1F("Lambda1", "Lambda1", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h3LambdaPIDproton = new TH1F("Lambda1 PID Proton", "Lambda1 PID Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h3LambdaPIDpion = new TH1F("Lambda1 PID Pion", "Lambda1 PID Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h3LambdaAlpha = new TH1F("Lambda1 Alpha", "Lambda1 Alpha", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h3Lambdaptrel = new TH1F("Lambda1 pTrel", "Lambda1 pT_rel", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h3Lambdapt = new TH1F("Lambda1 pT", "Lambda1 pT", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h3LambdaEta = new TH1F("Lambda1 Psuedorapidity", "Lambda1 Psuedorapidity", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h3Lambdatracklength = new TH1F("Lambda1 Track Length", "Lambda1 Track Length", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h3LambdaDCAoverdDCAproton = new TH1F("Lambda1 dDCA Proton", "Lambda1 dDCA Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h3LambdaDCAoverdDCApion = new TH1F("Lambda1 dDCA Pion", "Lambda1 dDCA Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h3LambdaKaonCont = new TH1F("Lambda-Kaons1", "Lambda1 Kaon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h3LambdaPhotonCont = new TH1F("Lambda-Photons1", "Lambda1 Photon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);

    TH1F* h4Lambda = new TH1F("Lambda1", "Lambda1", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h4LambdaPIDproton = new TH1F("Lambda1 PID Proton", "Lambda1 PID Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h4LambdaPIDpion = new TH1F("Lambda1 PID Pion", "Lambda1 PID Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h4LambdaAlpha = new TH1F("Lambda1 Alpha", "Lambda1 Alpha", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h4Lambdaptrel = new TH1F("Lambda1 pTrel", "Lambda1 pT_rel", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h4Lambdapt = new TH1F("Lambda1 pT", "Lambda1 pT", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h4LambdaEta = new TH1F("Lambda1 Psuedorapidity", "Lambda1 Psuedorapidity", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h4Lambdatracklength = new TH1F("Lambda1 Track Length", "Lambda1 Track Length", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h4LambdaDCAoverdDCAproton = new TH1F("Lambda1 dDCA Proton", "Lambda1 dDCA Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h4LambdaDCAoverdDCApion = new TH1F("Lambda1 dDCA Pion", "Lambda1 dDCA Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h4LambdaKaonCont = new TH1F("Lambda-Kaons1", "Lambda1 Kaon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h4LambdaPhotonCont = new TH1F("Lambda-Photons1", "Lambda1 Photon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);

    TH1F* h5Lambda = new TH1F("Lambda1", "Lambda1", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h5LambdaPIDproton = new TH1F("Lambda1 PID Proton", "Lambda1 PID Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h5LambdaPIDpion = new TH1F("Lambda1 PID Pion", "Lambda1 PID Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h5LambdaAlpha = new TH1F("Lambda1 Alpha", "Lambda1 Alpha", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h5Lambdaptrel = new TH1F("Lambda1 pTrel", "Lambda1 pT_rel", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h5Lambdapt = new TH1F("Lambda1 pT", "Lambda1 pT", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h5LambdaEta = new TH1F("Lambda1 Psuedorapidity", "Lambda1 Psuedorapidity", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h5Lambdatracklength = new TH1F("Lambda1 Track Length", "Lambda1 Track Length", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h5LambdaDCAoverdDCAproton = new TH1F("Lambda1 dDCA Proton", "Lambda1 dDCA Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h5LambdaDCAoverdDCApion = new TH1F("Lambda1 dDCA Pion", "Lambda1 dDCA Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h5LambdaKaonCont = new TH1F("Lambda-Kaons1", "Lambda1 Kaon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h5LambdaPhotonCont = new TH1F("Lambda-Photons1", "Lambda1 Photon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);

    TH1F* h6Lambda = new TH1F("Lambda1", "Lambda1", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h6LambdaPIDproton = new TH1F("Lambda1 PID Proton", "Lambda1 PID Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h6LambdaPIDpion = new TH1F("Lambda1 PID Pion", "Lambda1 PID Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h6LambdaAlpha = new TH1F("Lambda1 Alpha", "Lambda1 Alpha", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h6Lambdaptrel = new TH1F("Lambda1 pTrel", "Lambda1 pT_rel", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h6Lambdapt = new TH1F("Lambda1 pT", "Lambda1 pT", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h6LambdaEta = new TH1F("Lambda1 Psuedorapidity", "Lambda1 Psuedorapidity", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h6Lambdatracklength = new TH1F("Lambda1 Track Length", "Lambda1 Track Length", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h6LambdaDCAoverdDCAproton = new TH1F("Lambda1 dDCA Proton", "Lambda1 dDCA Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h6LambdaDCAoverdDCApion = new TH1F("Lambda1 dDCA Pion", "Lambda1 dDCA Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h6LambdaKaonCont = new TH1F("Lambda-Kaons1", "Lambda1 Kaon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h6LambdaPhotonCont = new TH1F("Lambda-Photons1", "Lambda1 Photon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);

    TH1F* h7Lambda = new TH1F("Lambda1", "Lambda1", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h7LambdaPIDproton = new TH1F("Lambda1 PID Proton", "Lambda1 PID Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h7LambdaPIDpion = new TH1F("Lambda1 PID Pion", "Lambda1 PID Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h7LambdaAlpha = new TH1F("Lambda1 Alpha", "Lambda1 Alpha", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h7Lambdaptrel = new TH1F("Lambda1 pTrel", "Lambda1 pT_rel", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h7Lambdapt = new TH1F("Lambda1 pT", "Lambda1 pT", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h7LambdaEta = new TH1F("Lambda1 Psuedorapidity", "Lambda1 Psuedorapidity", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h7Lambdatracklength = new TH1F("Lambda1 Track Length", "Lambda1 Track Length", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h7LambdaDCAoverdDCAproton = new TH1F("Lambda1 dDCA Proton", "Lambda1 dDCA Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h7LambdaDCAoverdDCApion = new TH1F("Lambda1 dDCA Pion", "Lambda1 dDCA Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h7LambdaKaonCont = new TH1F("Lambda-Kaons1", "Lambda1 Kaon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h7LambdaPhotonCont = new TH1F("Lambda-Photons1", "Lambda1 Photon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);

    TH1F* h8Lambda = new TH1F("Lambda1", "Lambda1", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h8LambdaPIDproton = new TH1F("Lambda1 PID Proton", "Lambda1 PID Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h8LambdaPIDpion = new TH1F("Lambda1 PID Pion", "Lambda1 PID Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h8LambdaAlpha = new TH1F("Lambda1 Alpha", "Lambda1 Alpha", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h8Lambdaptrel = new TH1F("Lambda1 pTrel", "Lambda1 pT_rel", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h8Lambdapt = new TH1F("Lambda1 pT", "Lambda1 pT", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h8LambdaEta = new TH1F("Lambda1 Psuedorapidity", "Lambda1 Psuedorapidity", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h8Lambdatracklength = new TH1F("Lambda1 Track Length", "Lambda1 Track Length", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h8LambdaDCAoverdDCAproton = new TH1F("Lambda1 dDCA Proton", "Lambda1 dDCA Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h8LambdaDCAoverdDCApion = new TH1F("Lambda1 dDCA Pion", "Lambda1 dDCA Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h8LambdaKaonCont = new TH1F("Lambda-Kaons1", "Lambda1 Kaon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h8LambdaPhotonCont = new TH1F("Lambda-Photons1", "Lambda1 Photon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);

    TH1F* h9Lambda = new TH1F("Lambda1", "Lambda1", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h9LambdaPIDproton = new TH1F("Lambda1 PID Proton", "Lambda1 PID Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h9LambdaPIDpion = new TH1F("Lambda1 PID Pion", "Lambda1 PID Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h9LambdaAlpha = new TH1F("Lambda1 Alpha", "Lambda1 Alpha", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h9Lambdaptrel = new TH1F("Lambda1 pTrel", "Lambda1 pT_rel", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h9Lambdapt = new TH1F("Lambda1 pT", "Lambda1 pT", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h9LambdaEta = new TH1F("Lambda1 Psuedorapidity", "Lambda1 Psuedorapidity", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h9Lambdatracklength = new TH1F("Lambda1 Track Length", "Lambda1 Track Length", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h9LambdaDCAoverdDCAproton = new TH1F("Lambda1 dDCA Proton", "Lambda1 dDCA Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h9LambdaDCAoverdDCApion = new TH1F("Lambda1 dDCA Pion", "Lambda1 dDCA Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h9LambdaKaonCont = new TH1F("Lambda-Kaons1", "Lambda1 Kaon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h9LambdaPhotonCont = new TH1F("Lambda-Photons1", "Lambda1 Photon Contamination Cut", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    
    //Lambdas in pt bins
    TH1F* hLambda1 = new TH1F("Lambda1", "Lambda pT Bin 1", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda2 = new TH1F("Lambda2", "Lambda pT Bin 2", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda3 = new TH1F("Lambda3", "Lambda pT Bin 3", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda4 = new TH1F("Lambda4", "Lambda pT Bin 4", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda5 = new TH1F("Lambda5", "Lambda pT Bin 5", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda6 = new TH1F("Lambda6", "Lambda pT Bin 6", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda7 = new TH1F("Lambda7", "Lambda pT Bin 7", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda8 = new TH1F("Lambda8", "Lambda pT Bin 8", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda9 = new TH1F("Lambda9", "Lambda pT Bin 9", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    //Lambdas in all of pt bins
    TH1F* hLambdaWholepTrange = new TH1F("Lambda Bins pT 1-9", "Lambda pT Bins 1-9", lambdabins, lambdalowerbinrange, lambdaupperbinrange);

    //Armenteros plot for particle ID lambdas vs kaons
    TH2F* hAPbefore = new TH2F("AP", "Armenteros-Podolansky Plot (After H1FindLambda)", 200, -1, 1, 200, 0, 0.3);
    TH2F* hAPafter = new TH2F("AP", "Armenteros-Podolansky Plot (After All Cuts)", 200, -1, 1, 200, 0, 0.3);

    //DCA vs dDCA to help visualize DCA significance Cut
    TH2F* hprotonDCAvsdDCAbefore = new TH2F("Proton DCA vs dDCA Before", "Proton DCA vs dDCA (After H1FindLambda)", 200, 0, 3, 1000, -10, 10);
    TH2F* hprotonDCAvsdDCAafter = new TH2F("Proton DCA vs dDCA After", "Proton DCA vs dDCA (After Cuts)", 200, 0, 3, 1000, -10, 10);
    TH2F* hprotonDCAvsdDCArejected = new TH2F("Proton DCA vs dDCA Rejected", "Proton DCA vs dDCA (Rejected Candidates)", 200, 0, 3, 1000, -10, 10);
    TH2F* hpionDCAvsdDCAbefore = new TH2F("Pion DCA vs dDCA Before", "Pion DCA vs dDCA (After H1FindLambda)", 200, 0, 3, 1000, -10, -10);
    TH2F* hpionDCAvsdDCAafter = new TH2F("Pion DCA vs dDCA After", "Pion DCA vs dDCA (After Cuts)", 200, 0, 3, 1000, -10, -10);
    TH2F* hpionDCAvsdDCArejected = new TH2F("Pion DCA vs dDCA Rejected", "Pion DCA vs dDCA (Rejected Candidates)", 200, 0, 3, 1000, -10, 10);

    TH2F* hpiondDCAvspTbefore = new TH2F("Pion DCA vs pT Before", "Pion DCA vs pT (After H1FindLambda)", 500, 0, 10, 1000, -10, -10);
    TH2F* hpionDCAvspTbefore = new TH2F("Pion DCA vs pT Before", "Pion DCA vs pT (After H1FindLambda)", 500, 0, 10, 1000, -10, -10);
    TH2F* hpionDCAoverdDCAvspT = new TH2F("Pion DCA/dDCA vs pT Before", "Pion DCA/dDCA vs pT", 500, 0, 10, 1000, -10, -10);
    TH2F* hprotondDCAvspTbefore = new TH2F("Proton dDCA vs pT Before", "Proton dDCA vs pT (After H1FindLambda)", 500, 0, 10, 1000, -10, -10);
    TH2F* hprotonDCAoverdDCAvspT = new TH2F("Proton DCA/dDCA vs pT Before", "Proton DCA/dDCA vs pT", 500, 0, 10, 1000, -10, -10);
    TH2F* hprotonDCAvspTbefore = new TH2F("Proton dDCA vs pT Before", "Proton dDCA vs pT (After H1FindLambda)", 500, 0, 10, 1000, -10, -10);

    //dEdx plot for particle ID protons vs pions
    TH2D* hdEdxBothBefore = new TH2D("dEdxvP Initial (After H1FindLambda)", "dE/dx Initial (After H1FindLambda)", 500, 0, 2, 500, 0, 10);
    TH2D* hdEdxBothAfterLcut = new TH2D("dEdxvP After Likelihood Cut", "dE/dx After Likelihood Cut", 500, 0, 2, 500, 0, 10);
    TH2D* hdEdxBothAfterAllcuts = new TH2D("dEdxvP After All Cuts", "dE/dx After All Cuts", 500, 0, 2, 500, 0, 10);
    TH2D* hdEdxProtonBefore = new TH2D("Proton dEdxvP Initial (After H1FindLambda)", "Proton dE/dx Initial (After H1FindLambda)", 500, 0, 2, 500, 0, 10);
    TH2D* hdEdxProtonAfterLcut = new TH2D("Proton dEdxvP After Likelihood Cut", "Proton dE/dx After Likelihood Cut", 500, 0, 2, 500, 0, 10);
    TH2D* hdEdxProtonAfterAllcuts = new TH2D("Proton dEdxvP After All Cuts", "Proton dE/dx After All Cuts", 500, 0, 2, 500, 0, 10);
    TH2D* hdEdxPionBefore = new TH2D("Pion dEdxvP Initial (After H1FindLambda)", "Pion dE/dx Initial (After H1FindLambda)", 500, 0, 2, 500, 0, 10);
    TH2D* hdEdxPionAfterLcut = new TH2D("Pion dEdxvP After Likelihood Cut", "Pion dE/dx After Likelihood Cut", 500, 0, 2, 500, 0, 10);
    TH2D* hdEdxPionAfterAllcuts = new TH2D("Pion dEdxvP After All Cuts", "Pion dE/dx After All Cuts", 500, 0, 2, 500, 0, 10);
    //DCA/dDCA histograms
    TH1F* hdDCAproton = new TH1F("Proton dDCA", "Proton dDCA", 1000, 0, 10);
    TH1F* hdDCApion = new TH1F("Pion dDCA", "Pion dDCA", 1000, 0, 10);
    TH1F* hDCAproton = new TH1F("Proton DCA", "Proton DCA", 1000, -10, 10);
    TH1F* hDCApion = new TH1F("Pion DCA", "Pion DCA", 1000, -10, 10);
    TH1F* hDCAoverdDCAproton = new TH1F("Proton DCA/dDCA", "Proton DCA/dDCA", 1000, -10, 10);
    TH1F* hDCAoverdDCApion = new TH1F("Pion DCA/dDCA", "Pion DCA/dDCA", 1000, -10, 10);
    //Likelihood histograms
    TH1F* hDedxLproton = new TH1F("Proton Likelihood", "Proton Likelihood", 1000, 0, 1);
    TH1F* hDedxLpion = new TH1F("Pion Likelihood", "Pion Likelihood", 1000, 0, 1);
    //Defining Root Canvas to work on
    TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 400);

    // define radial vertex cuts for distance of closest approach and beampipe
    //const Float_t minDca = 2.;
    //const Float_t maxDca = 40.;
    //const Float_t lowerBeamPipe = 4.;
    //const Float_t upperBeamPipe = 5.5;

    // define transverse momentum cuts on tracks
    const Float_t minPtPion = 0.12;//0.12
    const Float_t minPtKZero = 0.5;
    const Float_t maxPtKZero = 3.5;
    const Float_t minTracklenPion = 10; //this is 10 in kaon/lambda finder, originally was 15 in example
    //need to add separate ptrelcuts for different plots
    const Float_t ptrelCutK = 0.10;
    const Float_t ptrelCutL = 0.12; //was 15
    //const Float_t ptrelkexcludelow = 0.48;
    //const Float_t ptrelkexcludehigh = 0.52;


    // Pointer to vertices to loop over
    static H1CentralVertexArrayPtr vertex;
    static H1PartLambdaArrayPtr LambdaCand;
    //static H1CentralFittedV0ArrayPtr v0;

    // Get the pion's mass
    Double_t mPion = TDatabasePDG::Instance()->GetParticle(211)->Mass();
    // Get proton mass
    Double_t mProton = TDatabasePDG::Instance()->GetParticle(2212)->Mass();
    // Get electron mass
    Double_t mElectron = TDatabasePDG::Instance()->GetParticle(11)->Mass();

    ///////////////BEGIN MAIN LOOP////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    // Loop as long as there's data
    while (H1Tree::Instance()->Next() && !opts.IsMaxEvent(eventCounter)) {

        // Loop over all vertices found in the event
        for (Int_t loop = 0; loop < LambdaCand.GetEntries(); loop++) {

        if (LambdaCand[loop]->GetNumOfParticles() < 2) continue;

        Double_t invMasslambda = (LambdaCand[loop]->GetProtonFourVector()
            + LambdaCand[loop]->GetPionFourVector()).M();

        //Getting Vectors for Armenteros-Podolansky Plot
        H1Track* ProtonH1Track = (H1Track*)LambdaCand[loop]->GetProtonTrack();
        H1Track* PionH1Track = (H1Track*)LambdaCand[loop]->GetPionTrack();

        TVector3 vecProton = ProtonH1Track->GetFourVector(mProton).Vect();
        TVector3 vecPion = PionH1Track->GetFourVector(mPion).Vect();
        TVector3 vecLambda = vecProton + vecPion;

        Double_t qlProton = vecLambda.Dot(vecProton) / vecLambda.Mag();
        Double_t qlPion = vecLambda.Dot(vecPion) / vecLambda.Mag();
        Double_t pTrel = vecLambda.Cross(vecPion).Mag() / vecLambda.Mag();
        Double_t chargeProton = LambdaCand[loop]->GetProtonCharge();
        Double_t chargePion = LambdaCand[loop]->GetPionCharge();
        Double_t alpha = (chargeProton * qlProton + chargePion * qlPion) / (qlProton + qlPion);

        hAPbefore->Fill(alpha,pTrel);
        
        //Call H1PartLambda class data that references H1CentralFittedTrack for Pion and Proton
        const H1CentralFittedTrack* ProtonCFTrack = LambdaCand[loop]->GetProtonTrack();
        const H1CentralFittedTrack* PionCFTrack = LambdaCand[loop]->GetPionTrack();


        hdEdxBothBefore->Fill(ProtonCFTrack->GetMomentum().Mag(), ProtonCFTrack->GetCorrectedDedx());
        hdEdxBothBefore->Fill(PionCFTrack->GetMomentum().Mag(), PionCFTrack->GetCorrectedDedx());

        hdEdxProtonBefore->Fill(ProtonCFTrack->GetMomentum().Mag(), ProtonCFTrack->GetCorrectedDedx());
        hdEdxPionBefore->Fill(PionCFTrack->GetMomentum().Mag(), PionCFTrack->GetCorrectedDedx());
        
        
        //Getting H1PartSelTracks for daughters
        const H1PartSelTrack* ProtonPSTrack = LambdaCand[loop]->GetProton();
        const H1PartSelTrack* PionPSTrack = LambdaCand[loop]->GetPion();

        //Plotting Likelihood to help visualize likelihood cut
        hDedxLproton->Fill(ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton));
        hDedxLpion->Fill(PionPSTrack->GetDedxLikelihood(H1Dedx::kPion));
        //////////////////////////////////////////////////////////////////
        
        //Prepating DCA/dDCA information to get a histogram to help visualize this cut
        const H1CentralTrack* protonFittedNoV = ProtonCFTrack->GetCentralTrack();
        const H1Trajectory* protonTrajectory = protonFittedNoV->GetTrajectory();

        const H1CentralTrack* pionFittedNoV = PionCFTrack->GetCentralTrack();
        const H1Trajectory* pionTrajectory = pionFittedNoV->GetTrajectory();

        H1SelVertex* ProtonVertex = (H1SelVertex*)LambdaCand[loop]->GetProtonTrack();
        const H1SelVertex* ProtonPrimaryVertex = ProtonVertex->GetPrimaryVertex();
        TVector3 pv = ProtonPrimaryVertex->GetPosition();//is same for both protons and pion daughters, so just using proton here and previous couple lines
        TVector2 V2PV(pv.X(), pv.Y());
        hdDCAproton->Fill((protonTrajectory->GetdDca(V2PV)));
        hdDCApion->Fill((pionTrajectory->GetdDca(V2PV)));
        hDCAproton->Fill((protonTrajectory->GetDca(V2PV)));
        hDCApion->Fill((pionTrajectory->GetDca(V2PV)));
        hDCAoverdDCAproton->Fill((protonTrajectory->GetDca(V2PV))/(protonTrajectory->GetdDca(V2PV)));
        hDCAoverdDCApion->Fill((pionTrajectory->GetDca(V2PV))/(pionTrajectory->GetdDca(V2PV)));


        
        ///////////////////////////////////////////////////////////////////////////////
        //Defining pT before cuts and putting stuff into pt bins
        const H1CentralFittedV0* v0 = LambdaCand[loop]->GetV0();
        Double_t pT = v0->GetPt();

       

        //Raw Lambda Candidate data after our event selection defined in create_evls.C
        hLambda->Fill(invMasslambda);
        //Fill pT bins for Raw Lambda Candidate data after our event selection defined in create_evls.C
        if (pT >= ptbin1l && pT < ptbin1u) {
            h1Lambda->Fill(invMasslambda);
        }
        else if (pT >= ptbin2l && pT < ptbin2u) {
            h2Lambda->Fill(invMasslambda);
        }
        else if (pT >= ptbin3l && pT < ptbin3u) {
            h3Lambda->Fill(invMasslambda);
        }
        else if (pT >= ptbin4l && pT < ptbin4u) {
            h4Lambda->Fill(invMasslambda);
        }
        else if (pT >= ptbin5l && pT < ptbin5u) {
            h5Lambda->Fill(invMasslambda);
        }
        else if (pT >= ptbin6l && pT < ptbin6u) {
            h6Lambda->Fill(invMasslambda);
        }
        else if (pT >= ptbin7l && pT < ptbin7u) {
            h7Lambda->Fill(invMasslambda);
        }
        else if (pT >= ptbin8l && pT < ptbin8u) {
            h8Lambda->Fill(invMasslambda);
        }
        else if (pT >= ptbin9l && pT < ptbin9u) {
            h9Lambda->Fill(invMasslambda);
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////
        //DELETE LATER
        // ///////////////////////////////////////////////////////////////////////
        //DCA over dDCA proton cut
        //if (abs((protonTrajectory->GetDca(V2PV)) / (protonTrajectory->GetdDca(V2PV))) < 2) {
        //    hpionDCAvsdDCArejected->Fill((pionTrajectory->GetdDca(V2PV)), (pionTrajectory->GetDca(V2PV)));
        //    hprotonDCAvsdDCArejected->Fill((protonTrajectory->GetdDca(V2PV)), (protonTrajectory->GetDca(V2PV)));
        //    continue;
        //}

        ////DCA over dDCA pion cut
        //if (abs((pionTrajectory->GetDca(V2PV)) / (pionTrajectory->GetdDca(V2PV))) < 1) {
        //    hpionDCAvsdDCArejected->Fill((pionTrajectory->GetdDca(V2PV)), (pionTrajectory->GetDca(V2PV)));
        //    hprotonDCAvsdDCArejected->Fill((protonTrajectory->GetdDca(V2PV)), (protonTrajectory->GetDca(V2PV)));
        //    continue;
        //}
        ////////////////////////////////////////////////////////////////////////
        //DELETE LATER
        ///////////////////////////////////////////////////////////////////




        hLambdaPIDprotontest0->Fill(invMasslambda);
        //Filling plots with diff PID cuts before the real on happens, reference point is the raw H1FindLambda output
        if (ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton) > 0.0001) {
            hLambdaPIDprotontest1->Fill(invMasslambda);
        }
        if (ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton) > 0.0002) {
            hLambdaPIDprotontest2->Fill(invMasslambda);
        }
        if (ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton) > 0.0003) {
            hLambdaPIDprotontest3->Fill(invMasslambda);
        }
        if (ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton) > 0.0004) {
            hLambdaPIDprotontest4->Fill(invMasslambda);
        }
        if (ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton) > 0.0005) {
            hLambdaPIDprotontest5->Fill(invMasslambda);
        }
        if (ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton) > 0.0006) {
            hLambdaPIDprotontest6->Fill(invMasslambda);
        }
        if (ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton) > 0.0007) {
            hLambdaPIDprotontest7->Fill(invMasslambda);
        }
        if (ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton) > 0.0008) {
            hLambdaPIDprotontest8->Fill(invMasslambda);
        }
        if (ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton) > 0.0009) {
            hLambdaPIDprotontest9->Fill(invMasslambda);
        }
        if (ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton) > 0.001) {
            hLambdaPIDprotontest10->Fill(invMasslambda);
        }



        hLambdaPIDpiontest0->Fill(invMasslambda);


        if (PionPSTrack->GetDedxLikelihood(H1Dedx::kPion) > 0.0001) {
            hLambdaPIDpiontest1->Fill(invMasslambda);
        }
        if (PionPSTrack->GetDedxLikelihood(H1Dedx::kPion) > 0.0002) {
            hLambdaPIDpiontest2->Fill(invMasslambda);
        }
        if (PionPSTrack->GetDedxLikelihood(H1Dedx::kPion) > 0.0003) {
            hLambdaPIDpiontest3->Fill(invMasslambda);
        }
        if (PionPSTrack->GetDedxLikelihood(H1Dedx::kPion) > 0.0004) {
            hLambdaPIDpiontest4->Fill(invMasslambda);
        }
        if (PionPSTrack->GetDedxLikelihood(H1Dedx::kPion) > 0.0005) {
            hLambdaPIDpiontest5->Fill(invMasslambda);
        }
        if (PionPSTrack->GetDedxLikelihood(H1Dedx::kPion) > 0.0006) {
            hLambdaPIDpiontest6->Fill(invMasslambda);
        }
        if (PionPSTrack->GetDedxLikelihood(H1Dedx::kPion) > 0.0007) {
            hLambdaPIDpiontest7->Fill(invMasslambda);
        }
        if (PionPSTrack->GetDedxLikelihood(H1Dedx::kPion) > 0.0008) {
            hLambdaPIDpiontest8->Fill(invMasslambda);
        }
        if (PionPSTrack->GetDedxLikelihood(H1Dedx::kPion) > 0.0009) {
            hLambdaPIDpiontest9->Fill(invMasslambda);
        }
        if (PionPSTrack->GetDedxLikelihood(H1Dedx::kPion) > 0.001) {
            hLambdaPIDpiontest10->Fill(invMasslambda);
        }


        //PID Likelihood Cut for Proton < 0.003
        if (ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton) < 0.003) { 
            hpionDCAvsdDCArejected->Fill((pionTrajectory->GetdDca(V2PV)), (pionTrajectory->GetDca(V2PV)));
            hprotonDCAvsdDCArejected->Fill((protonTrajectory->GetdDca(V2PV)), (protonTrajectory->GetDca(V2PV)));
            continue;
        }
        hLambdaPIDproton->Fill(invMasslambda);

        //Fill pT bins after PID Likelihood Cut for Proton < 0.003
        if (pT >= ptbin1l && pT < ptbin1u) {
            h1LambdaPIDproton->Fill(invMasslambda);
        }
        else if (pT >= ptbin2l && pT < ptbin2u) {
            h2LambdaPIDproton->Fill(invMasslambda);
        }
        else if (pT >= ptbin3l && pT < ptbin3u) {
            h3LambdaPIDproton->Fill(invMasslambda);
        }
        else if (pT >= ptbin4l && pT < ptbin4u) {
            h4LambdaPIDproton->Fill(invMasslambda);
        }
        else if (pT >= ptbin5l && pT < ptbin5u) {
            h5LambdaPIDproton->Fill(invMasslambda);
        }
        else if (pT >= ptbin6l && pT < ptbin6u) {
            h6LambdaPIDproton->Fill(invMasslambda);
        }
        else if (pT >= ptbin7l && pT < ptbin7u) {
            h7LambdaPIDproton->Fill(invMasslambda);
        }
        else if (pT >= ptbin8l && pT < ptbin8u) {
            h8LambdaPIDproton->Fill(invMasslambda);
        }
        else if (pT >= ptbin9l && pT < ptbin9u) {
            h9LambdaPIDproton->Fill(invMasslambda);
        }

        //PID Likelihood Cut for Pion < 0.003
        if (PionPSTrack->GetDedxLikelihood(H1Dedx::kPion) < 0.003) {
            hpionDCAvsdDCArejected->Fill((pionTrajectory->GetdDca(V2PV)), (pionTrajectory->GetDca(V2PV)));
            hprotonDCAvsdDCArejected->Fill((protonTrajectory->GetdDca(V2PV)), (protonTrajectory->GetDca(V2PV)));
            continue; }

        hLambdaPIDpion->Fill(invMasslambda);
        //Fill pT bins after PID Likelihood Cut for Pion < 0.003
        if (pT >= ptbin1l && pT < ptbin1u) {
            h1LambdaPIDpion->Fill(invMasslambda);
        }
        else if (pT >= ptbin2l && pT < ptbin2u) {
            h2LambdaPIDpion->Fill(invMasslambda);
        }
        else if (pT >= ptbin3l && pT < ptbin3u) {
            h3LambdaPIDpion->Fill(invMasslambda);
        }
        else if (pT >= ptbin4l && pT < ptbin4u) {
            h4LambdaPIDpion->Fill(invMasslambda);
        }
        else if (pT >= ptbin5l && pT < ptbin5u) {
            h5LambdaPIDpion->Fill(invMasslambda);
        }
        else if (pT >= ptbin6l && pT < ptbin6u) {
            h6LambdaPIDpion->Fill(invMasslambda);
        }
        else if (pT >= ptbin7l && pT < ptbin7u) {
            h7LambdaPIDpion->Fill(invMasslambda);
        }
        else if (pT >= ptbin8l && pT < ptbin8u) {
            h8LambdaPIDpion->Fill(invMasslambda);
        }
        else if (pT >= ptbin9l && pT < ptbin9u) {
            h9LambdaPIDpion->Fill(invMasslambda);
        }
        //Fill dE/dx plots for after the PID cuts
        hdEdxBothAfterLcut->Fill(ProtonCFTrack->GetMomentum().Mag(), ProtonCFTrack->GetCorrectedDedx());
        hdEdxBothAfterLcut->Fill(PionCFTrack->GetMomentum().Mag(), PionCFTrack->GetCorrectedDedx());

        hdEdxProtonAfterLcut->Fill(ProtonCFTrack->GetMomentum().Mag(), ProtonCFTrack->GetCorrectedDedx());
        hdEdxPionAfterLcut->Fill(PionCFTrack->GetMomentum().Mag(), PionCFTrack->GetCorrectedDedx());


        hpionDCAvsdDCAbefore->Fill((pionTrajectory->GetdDca(V2PV)), (pionTrajectory->GetDca(V2PV)));
        hprotonDCAvsdDCAbefore->Fill((protonTrajectory->GetdDca(V2PV)), (protonTrajectory->GetDca(V2PV)));
        hpionDCAvspTbefore->Fill(pT, (pionTrajectory->GetDca(V2PV)));
        hprotonDCAvspTbefore->Fill(pT, (protonTrajectory->GetDca(V2PV)));
        hpiondDCAvspTbefore->Fill(pT, (pionTrajectory->GetdDca(V2PV)));
        hprotondDCAvspTbefore->Fill(pT, (protonTrajectory->GetdDca(V2PV)));
        hprotonDCAoverdDCAvspT->Fill(pT, (protonTrajectory->GetDca(V2PV)) / (protonTrajectory->GetdDca(V2PV)));
        hpionDCAoverdDCAvspT->Fill(pT, (pionTrajectory->GetDca(V2PV)) / (pionTrajectory->GetdDca(V2PV)));

        //Diff DCA/dDCA cuts test //before point for both of these is right after PID cuts
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 0.2) {
            hLambdaDCAoverdDCAprotontest1->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) >0.4) {
            hLambdaDCAoverdDCAprotontest2->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 0.6) {
            hLambdaDCAoverdDCAprotontest3->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 0.8) {
            hLambdaDCAoverdDCAprotontest4->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 1.0) {
            hLambdaDCAoverdDCAprotontest5->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 1.2) {
            hLambdaDCAoverdDCAprotontest6->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 1.4) {
            hLambdaDCAoverdDCAprotontest7->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 1.6) {
            hLambdaDCAoverdDCAprotontest8->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 1.8) {
            hLambdaDCAoverdDCAprotontest9->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 2.0) {
            hLambdaDCAoverdDCAprotontest10->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 2.2) {
            hLambdaDCAoverdDCAprotontest11->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 2.4) {
            hLambdaDCAoverdDCAprotontest12->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 2.6) {
            hLambdaDCAoverdDCAprotontest13->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 2.8) {
            hLambdaDCAoverdDCAprotontest14->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 3.0) {
            hLambdaDCAoverdDCAprotontest15->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 3.2) {
            hLambdaDCAoverdDCAprotontest16->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 3.4) {
            hLambdaDCAoverdDCAprotontest17->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 3.6) {
            hLambdaDCAoverdDCAprotontest18->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 3.8) {
            hLambdaDCAoverdDCAprotontest19->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 4.0) {
            hLambdaDCAoverdDCAprotontest20->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 4.2) {
            hLambdaDCAoverdDCAprotontest21->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 4.4) {
            hLambdaDCAoverdDCAprotontest22->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 4.6) {
            hLambdaDCAoverdDCAprotontest23->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 4.8) {
            hLambdaDCAoverdDCAprotontest24->Fill(invMasslambda);
        }
        if (abs((protonTrajectory->GetDca(V2PV))) / (protonTrajectory->GetdDca(V2PV)) > 5.0) {
            hLambdaDCAoverdDCAprotontest25->Fill(invMasslambda);
        }



        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 0.2) {
            hLambdaDCAoverdDCApiontest1->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 0.4) {
            hLambdaDCAoverdDCApiontest2->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 0.6) {
            hLambdaDCAoverdDCApiontest3->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 0.8) {
            hLambdaDCAoverdDCApiontest4->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 1.0) {
            hLambdaDCAoverdDCApiontest5->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 1.2) {
            hLambdaDCAoverdDCApiontest6->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 1.4) {
            hLambdaDCAoverdDCApiontest7->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 1.6) {
            hLambdaDCAoverdDCApiontest8->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 1.8) {
            hLambdaDCAoverdDCApiontest9->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 2.0) {
            hLambdaDCAoverdDCApiontest10->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 2.2) {
            hLambdaDCAoverdDCApiontest11->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 2.4) {
            hLambdaDCAoverdDCApiontest12->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 2.6) {
            hLambdaDCAoverdDCApiontest13->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 2.8) {
            hLambdaDCAoverdDCApiontest14->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 3.0) {
            hLambdaDCAoverdDCApiontest15->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 3.2) {
            hLambdaDCAoverdDCApiontest16->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 3.4) {
            hLambdaDCAoverdDCApiontest17->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 3.6) {
            hLambdaDCAoverdDCApiontest18->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 3.8) {
            hLambdaDCAoverdDCApiontest19->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 4.0) {
            hLambdaDCAoverdDCApiontest20->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 4.2) {
            hLambdaDCAoverdDCApiontest21->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 4.4) {
            hLambdaDCAoverdDCApiontest22->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 4.6) {
            hLambdaDCAoverdDCApiontest23->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 4.8) {
            hLambdaDCAoverdDCApiontest24->Fill(invMasslambda);
        }
        if (abs((pionTrajectory->GetDca(V2PV))) / (pionTrajectory->GetdDca(V2PV)) > 5.0) {
            hLambdaDCAoverdDCApiontest25->Fill(invMasslambda);
        }
        


        //DCA over dDCA proton cut
        if (abs((protonTrajectory->GetDca(V2PV)) / (protonTrajectory->GetdDca(V2PV))) < 2) { 
            hpionDCAvsdDCArejected->Fill((pionTrajectory->GetdDca(V2PV)), (pionTrajectory->GetDca(V2PV)));
            hprotonDCAvsdDCArejected->Fill((protonTrajectory->GetdDca(V2PV)), (protonTrajectory->GetDca(V2PV)));
            continue; }

        hLambdaDCAoverdDCAproton->Fill(invMasslambda);
        
        // 
        //Fill pT bins after DCA over dDCA proton cut
        if (pT >= ptbin1l && pT < ptbin1u) {
            h1LambdaDCAoverdDCAproton->Fill(invMasslambda);
        }
        else if (pT >= ptbin2l && pT < ptbin2u) {
            h2LambdaDCAoverdDCAproton->Fill(invMasslambda);
        }
        else if (pT >= ptbin3l && pT < ptbin3u) {
            h3LambdaDCAoverdDCAproton->Fill(invMasslambda);
        }
        else if (pT >= ptbin4l && pT < ptbin4u) {
            h4LambdaDCAoverdDCAproton->Fill(invMasslambda);
        }
        else if (pT >= ptbin5l && pT < ptbin5u) {
            h5LambdaDCAoverdDCAproton->Fill(invMasslambda);
        }
        else if (pT >= ptbin6l && pT < ptbin6u) {
            h6LambdaDCAoverdDCAproton->Fill(invMasslambda);
        }
        else if (pT >= ptbin7l && pT < ptbin7u) {
            h7LambdaDCAoverdDCAproton->Fill(invMasslambda);
        }
        else if (pT >= ptbin8l && pT < ptbin8u) {
            h8LambdaDCAoverdDCAproton->Fill(invMasslambda);
        }
        else if (pT >= ptbin9l && pT < ptbin9u) {
            h9LambdaDCAoverdDCAproton->Fill(invMasslambda);
        }

        //DCA over dDCA pion cut
        if (abs((pionTrajectory->GetDca(V2PV)) / (pionTrajectory->GetdDca(V2PV))) < 1) { 
            hpionDCAvsdDCArejected->Fill((pionTrajectory->GetdDca(V2PV)), (pionTrajectory->GetDca(V2PV)));
            hprotonDCAvsdDCArejected->Fill((protonTrajectory->GetdDca(V2PV)), (protonTrajectory->GetDca(V2PV)));
            continue; }


        hpionDCAvsdDCAafter->Fill((pionTrajectory->GetdDca(V2PV)), (pionTrajectory->GetDca(V2PV)));
        hprotonDCAvsdDCAafter->Fill((protonTrajectory->GetdDca(V2PV)), (protonTrajectory->GetDca(V2PV)));

        hLambdaDCAoverdDCApion->Fill(invMasslambda);
        //Fill pT bins after DCA over dDCA pion cut
        if (pT >= ptbin1l && pT < ptbin1u) {
            h1LambdaDCAoverdDCApion->Fill(invMasslambda);
        }
        else if (pT >= ptbin2l && pT < ptbin2u) {
            h2LambdaDCAoverdDCApion->Fill(invMasslambda);
        }
        else if (pT >= ptbin3l && pT < ptbin3u) {
            h3LambdaDCAoverdDCApion->Fill(invMasslambda);
        }
        else if (pT >= ptbin4l && pT < ptbin4u) {
            h4LambdaDCAoverdDCApion->Fill(invMasslambda);
        }
        else if (pT >= ptbin5l && pT < ptbin5u) {
            h5LambdaDCAoverdDCApion->Fill(invMasslambda);
        }
        else if (pT >= ptbin6l && pT < ptbin6u) {
            h6LambdaDCAoverdDCApion->Fill(invMasslambda);
        }
        else if (pT >= ptbin7l && pT < ptbin7u) {
            h7LambdaDCAoverdDCApion->Fill(invMasslambda);
        }
        else if (pT >= ptbin8l && pT < ptbin8u) {
            h8LambdaDCAoverdDCApion->Fill(invMasslambda);
        }
        else if (pT >= ptbin9l && pT < ptbin9u) {
            h9LambdaDCAoverdDCApion->Fill(invMasslambda);
        }

        ////Alpha cut
        //if (alpha>-0.4 && alpha<0.4) continue;
        //hLambdaAlpha->Fill(invMasslambda);
        ////Fill pT bins after Alpha cut
        //if (pT >= ptbin1l && pT < ptbin1u) {
        //    h1LambdaAlpha->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin2l && pT < ptbin2u) {
        //    h2LambdaAlpha->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin3l && pT < ptbin3u) {
        //    h3LambdaAlpha->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin4l && pT < ptbin4u) {
        //    h4LambdaAlpha->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin5l && pT < ptbin5u) {
        //    h5LambdaAlpha->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin6l && pT < ptbin6u) {
        //    h6LambdaAlpha->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin7l && pT < ptbin7u) {
        //    h7LambdaAlpha->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin8l && pT < ptbin8u) {
        //    h8LambdaAlpha->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin9l && pT < ptbin9u) {
        //    h9LambdaAlpha->Fill(invMasslambda);
        //}

        ////ptrel(proton/pion) cut, want it < 0.12 GeV
        //Double_t pTrelProton = LambdaCand[loop]->GetProtonPtRel();
        //Double_t pTrelPion = LambdaCand[loop]->GetPionPtRel();
        //if (pTrelProton > 0.12 || pTrelPion > 0.12) continue;
        //hLambdaptrel->Fill(invMasslambda);
        ////Fill pT bins after ptrel(proton/pion) cut
        //if (pT >= ptbin1l && pT < ptbin1u) {
        //    h1Lambdaptrel->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin2l && pT < ptbin2u) {
        //    h2Lambdaptrel->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin3l && pT < ptbin3u) {
        //    h3Lambdaptrel->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin4l && pT < ptbin4u) {
        //    h4Lambdaptrel->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin5l && pT < ptbin5u) {
        //    h5Lambdaptrel->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin6l && pT < ptbin6u) {
        //    h6Lambdaptrel->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin7l && pT < ptbin7u) {
        //    h7Lambdaptrel->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin8l && pT < ptbin8u) {
        //    h8Lambdaptrel->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin9l && pT < ptbin9u) {
        //    h9Lambdaptrel->Fill(invMasslambda);
        //}

        ////Cut on Pt of v0 mother candidate, want pt > 0.5
        //if (pT < 0.5 || pT > ptbin9u) continue;
        //hLambdapt->Fill(invMasslambda);
        ////Fill pT bins after Cut on Pt of v0 mother candidate
        //if (pT >= ptbin1l && pT < ptbin1u) {
        //    h1Lambdapt->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin2l && pT < ptbin2u) {
        //    h2Lambdapt->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin3l && pT < ptbin3u) {
        //    h3Lambdapt->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin4l && pT < ptbin4u) {
        //    h4Lambdapt->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin5l && pT < ptbin5u) {
        //    h5Lambdapt->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin6l && pT < ptbin6u) {
        //    h6Lambdapt->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin7l && pT < ptbin7u) {
        //    h7Lambdapt->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin8l && pT < ptbin8u) {
        //    h8Lambdapt->Fill(invMasslambda);
        //}
        //else if (pT >= ptbin9l && pT < ptbin9u) {
        //    h9Lambdapt->Fill(invMasslambda);
        //}

        //Cut on Chi2 of seconday v0 fit
        //if (v0->GetFitChi2() > 5.0) continue;
        //hLambdachi2->Fill(invMasslambda);

        //Eta Cut
        if (abs(atanh((v0->GetMomentum().Z()) / (v0->GetMomentum().Mag()))) > 1.5) { 
            hpionDCAvsdDCArejected->Fill((pionTrajectory->GetdDca(V2PV)), (pionTrajectory->GetDca(V2PV)));
            hprotonDCAvsdDCArejected->Fill((protonTrajectory->GetdDca(V2PV)), (protonTrajectory->GetDca(V2PV)));
            continue; }
        hLambdaEta->Fill(invMasslambda);
        //Fill pT bins after eta cut
        if (pT >= ptbin1l && pT < ptbin1u) {
            h1LambdaEta->Fill(invMasslambda);
        }
        else if (pT >= ptbin2l && pT < ptbin2u) {
            h2LambdaEta->Fill(invMasslambda);
        }
        else if (pT >= ptbin3l && pT < ptbin3u) {
            h3LambdaEta->Fill(invMasslambda);
        }
        else if (pT >= ptbin4l && pT < ptbin4u) {
            h4LambdaEta->Fill(invMasslambda);
        }
        else if (pT >= ptbin5l && pT < ptbin5u) {
            h5LambdaEta->Fill(invMasslambda);
        }
        else if (pT >= ptbin6l && pT < ptbin6u) {
            h6LambdaEta->Fill(invMasslambda);
        }
        else if (pT >= ptbin7l && pT < ptbin7u) {
            h7LambdaEta->Fill(invMasslambda);
        }
        else if (pT >= ptbin8l && pT < ptbin8u) {
            h8LambdaEta->Fill(invMasslambda);
        }
        else if (pT >= ptbin9l && pT < ptbin9u) {
            h9LambdaEta->Fill(invMasslambda);
        }

        //Cut on Track Length of proton and pion
        if (ProtonCFTrack->GetLength() < 15 || PionCFTrack->GetLength() < 15) { 
            hpionDCAvsdDCArejected->Fill((pionTrajectory->GetdDca(V2PV)), (pionTrajectory->GetDca(V2PV)));
            hprotonDCAvsdDCArejected->Fill((protonTrajectory->GetdDca(V2PV)), (protonTrajectory->GetDca(V2PV)));
            continue; }
        hLambdatracklength->Fill(invMasslambda);
        //Fill pT bins after track length cut
        if (pT >= ptbin1l && pT < ptbin1u) {
            h1Lambdatracklength->Fill(invMasslambda);
        }
        else if (pT >= ptbin2l && pT < ptbin2u) {
            h2Lambdatracklength->Fill(invMasslambda);
        }
        else if (pT >= ptbin3l && pT < ptbin3u) {
            h3Lambdatracklength->Fill(invMasslambda);
        }
        else if (pT >= ptbin4l && pT < ptbin4u) {
            h4Lambdatracklength->Fill(invMasslambda);
        }
        else if (pT >= ptbin5l && pT < ptbin5u) {
            h5Lambdatracklength->Fill(invMasslambda);
        }
        else if (pT >= ptbin6l && pT < ptbin6u) {
            h6Lambdatracklength->Fill(invMasslambda);
        }
        else if (pT >= ptbin7l && pT < ptbin7u) {
            h7Lambdatracklength->Fill(invMasslambda);
        }
        else if (pT >= ptbin8l && pT < ptbin8u) {
            h8Lambdatracklength->Fill(invMasslambda);
        }
        else if (pT >= ptbin9l && pT < ptbin9u) {
            h9Lambdatracklength->Fill(invMasslambda);
        }



        //Cut against K0 contamination
        Double_t invMasskaonCont = (ProtonH1Track->GetFourVector(mPion)
            + PionH1Track->GetFourVector(mPion)).M();
        if (invMasskaonCont > 0.465 && invMasskaonCont < 0.53) { 
            hpionDCAvsdDCArejected->Fill((pionTrajectory->GetdDca(V2PV)), (pionTrajectory->GetDca(V2PV)));
            hprotonDCAvsdDCArejected->Fill((protonTrajectory->GetdDca(V2PV)), (protonTrajectory->GetDca(V2PV)));
            continue; }
        hLambdaKaonCont->Fill(invMasslambda);
        //Fill pT bins after K0 contmination cut
        if (pT >= ptbin1l && pT < ptbin1u) {
            h1LambdaKaonCont->Fill(invMasslambda);
        }
        else if (pT >= ptbin2l && pT < ptbin2u) {
            h2LambdaKaonCont->Fill(invMasslambda);
        }
        else if (pT >= ptbin3l && pT < ptbin3u) {
            h3LambdaKaonCont->Fill(invMasslambda);
        }
        else if (pT >= ptbin4l && pT < ptbin4u) {
            h4LambdaKaonCont->Fill(invMasslambda);
        }
        else if (pT >= ptbin5l && pT < ptbin5u) {
            h5LambdaKaonCont->Fill(invMasslambda);
        }
        else if (pT >= ptbin6l && pT < ptbin6u) {
            h6LambdaKaonCont->Fill(invMasslambda);
        }
        else if (pT >= ptbin7l && pT < ptbin7u) {
            h7LambdaKaonCont->Fill(invMasslambda);
        }
        else if (pT >= ptbin8l && pT < ptbin8u) {
            h8LambdaKaonCont->Fill(invMasslambda);
        }
        else if (pT >= ptbin9l && pT < ptbin9u) {
            h9LambdaKaonCont->Fill(invMasslambda);
        }

        //Cut against photon contamination
        Double_t invMassphotonCont = (ProtonH1Track->GetFourVector(mElectron)
            + PionH1Track->GetFourVector(mElectron)).M();
        if (invMassphotonCont < 0.05) { 
            hpionDCAvsdDCArejected->Fill((pionTrajectory->GetdDca(V2PV)), (pionTrajectory->GetDca(V2PV)));
            hprotonDCAvsdDCArejected->Fill((protonTrajectory->GetdDca(V2PV)), (protonTrajectory->GetDca(V2PV)));
            continue; }
        hLambdaPhotonCont->Fill(invMasslambda);
        //Fill pT bins after K0 contmination cut
        if (pT >= ptbin1l && pT < ptbin1u) {
            h1LambdaPhotonCont->Fill(invMasslambda);
        }
        else if (pT >= ptbin2l && pT < ptbin2u) {
            h2LambdaPhotonCont->Fill(invMasslambda);
        }
        else if (pT >= ptbin3l && pT < ptbin3u) {
            h3LambdaPhotonCont->Fill(invMasslambda);
        }
        else if (pT >= ptbin4l && pT < ptbin4u) {
            h4LambdaPhotonCont->Fill(invMasslambda);
        }
        else if (pT >= ptbin5l && pT < ptbin5u) {
            h5LambdaPhotonCont->Fill(invMasslambda);
        }
        else if (pT >= ptbin6l && pT < ptbin6u) {
            h6LambdaPhotonCont->Fill(invMasslambda);
        }
        else if (pT >= ptbin7l && pT < ptbin7u) {
            h7LambdaPhotonCont->Fill(invMasslambda);
        }
        else if (pT >= ptbin8l && pT < ptbin8u) {
            h8LambdaPhotonCont->Fill(invMasslambda);
        }
        else if (pT >= ptbin9l && pT < ptbin9u) {
            h9LambdaPhotonCont->Fill(invMasslambda);
        }


        //Fill plots for after all cuts
        hAPafter->Fill(alpha, pTrel);
        hdEdxBothAfterAllcuts->Fill(ProtonCFTrack->GetMomentum().Mag(), ProtonCFTrack->GetCorrectedDedx());
        hdEdxBothAfterAllcuts->Fill(PionCFTrack->GetMomentum().Mag(), PionCFTrack->GetCorrectedDedx());

        hdEdxProtonAfterAllcuts->Fill(ProtonCFTrack->GetMomentum().Mag(), ProtonCFTrack->GetCorrectedDedx());
        hdEdxPionAfterAllcuts->Fill(PionCFTrack->GetMomentum().Mag(), PionCFTrack->GetCorrectedDedx());

        
        //After All necessary cuts, now can fill different pt bins, and the Total pT bin Range Plot
        if (pT >= ptbin1l && pT < ptbin9u) {
            hLambdaWholepTrange->Fill(invMasslambda);
        }
        else if (pT >= ptbin1l && pT < ptbin1u) {
            hLambda1->Fill(invMasslambda);
        }
        else if (pT >= ptbin2l && pT < ptbin2u) {
            hLambda2->Fill(invMasslambda);
        }
        else if (pT >= ptbin3l && pT < ptbin3u) {
            hLambda3->Fill(invMasslambda);
        }
        else if (pT >= ptbin4l && pT < ptbin4u) {
            hLambda4->Fill(invMasslambda);
        }
        else if (pT >= ptbin5l && pT < ptbin5u) {
            hLambda5->Fill(invMasslambda);
        }
        else if (pT >= ptbin6l && pT < ptbin6u) {
            hLambda6->Fill(invMasslambda);
        }
        else if (pT >= ptbin7l && pT < ptbin7u) {
            hLambda7->Fill(invMasslambda);
        }
        else if (pT >= ptbin8l && pT < ptbin8u) {
            hLambda8->Fill(invMasslambda);
        }
        else if (pT >= ptbin9l && pT < ptbin9u) {
            hLambda9->Fill(invMasslambda);
        } 
        } // loop over vertices

            eventCounter++;
            if (eventCounter % 1000 == 0) {
                cout << "Processing ODS event " << eventCounter << endl;

                // Redraw the canvas every 1000 events
                //hKaon->Draw();
                //hLambda->Draw();
                //hAP->Draw();
                //gPad->Update();
            }
    }

        cout << "\n" << eventCounter << " events processed." << endl;
        ////////////////////////////////////////////////////////////////////
        //////////////END MAIN LOOP//////////////////////////////////////
        ////////////////////////////////////////////////////////////////////

        //Now that we have histograms with final counts, we can gather these counts at each 
        //bucket and use this to fill data into TTrees
        
        //Defining tree variables and TTrees to populate with data for data analysis
        //then in our data loop we can assign values to these variables and fill the data tree
 
       TFile file(opts.GetOutput(), "RECREATE");
       //Kaon plots uploading
       
       hLambda->SetStats(0);
       hLambda->SetLineColor(kBlack);
       h1Lambda->SetLineColor(kBlack);
       h2Lambda->SetLineColor(kBlack);
       h3Lambda->SetLineColor(kBlack);
       h4Lambda->SetLineColor(kBlack);
       h5Lambda->SetLineColor(kBlack);
       h6Lambda->SetLineColor(kBlack);
       h7Lambda->SetLineColor(kBlack);
       h8Lambda->SetLineColor(kBlack);
       h9Lambda->SetLineColor(kBlack);
       hLambda->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda->GetYaxis()->SetTitle("Count");
       hLambda->Draw();
       hLambda->Write();

       hLambda1->SetStats(0);
       hLambda1->SetLineColor(kBlack);
       hLambda1->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda1->GetYaxis()->SetTitle("Count");
       hLambda1->Draw();
       hLambda1->Write();

       hLambda2->SetStats(0);
       hLambda2->SetLineColor(kBlack);
       hLambda2->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda2->GetYaxis()->SetTitle("Count");
       hLambda2->Draw();
       hLambda2->Write();

       hLambda3->SetStats(0);
       hLambda3->SetLineColor(kBlack);
       hLambda3->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda3->GetYaxis()->SetTitle("Count");
       hLambda3->Draw();
       hLambda3->Write();

       hLambda4->SetStats(0);
       hLambda4->SetLineColor(kBlack);
       hLambda4->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda4->GetYaxis()->SetTitle("Count");
       hLambda4->Draw();
       hLambda4->Write();

       hLambda5->SetStats(0);
       hLambda5->SetLineColor(kBlack);
       hLambda5->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda5->GetYaxis()->SetTitle("Count");
       hLambda5->Draw();
       hLambda5->Write();

       hLambda6->SetStats(0);
       hLambda6->SetLineColor(kBlack);
       hLambda6->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda6->GetYaxis()->SetTitle("Count");
       hLambda6->Draw();
       hLambda6->Write();

       hLambda7->SetStats(0);
       hLambda7->SetLineColor(kBlack);
       hLambda7->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda7->GetYaxis()->SetTitle("Count");
       hLambda7->Draw();
       hLambda7->Write();

       hLambda8->SetStats(0);
       hLambda8->SetLineColor(kBlack);
       hLambda8->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda8->GetYaxis()->SetTitle("Count");
       hLambda8->Draw();
       hLambda8->Write();

       hLambda9->SetStats(0);
       hLambda9->SetLineColor(kBlack);
       hLambda9->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda9->GetYaxis()->SetTitle("Count");
       hLambda9->Draw();
       hLambda9->Write();

       hLambdaWholepTrange->SetStats(0);
       hLambdaWholepTrange->SetLineColor(kBlack);
       hLambdaWholepTrange->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaWholepTrange->GetYaxis()->SetTitle("Count");
       hLambdaWholepTrange->Draw();
       hLambdaWholepTrange->Write();

       hAPbefore->SetStats(0);
       hAPbefore->GetXaxis()->SetTitle("\\alpha = (p^{+}_{L} - p^{-}_{L})/(p^{+}_{L} + p^{-}_{L})");
       hAPbefore->GetYaxis()->SetTitle("p_{T} [GeV]");
       hAPbefore->GetXaxis()->SetTitleSize(0.04);
       hAPbefore->GetXaxis()->SetTitleOffset(1);
       hAPbefore->GetYaxis()->SetTitleSize(0.05);
       hAPbefore->Draw("colz");
       hAPbefore->Write();
       gPad->Update();
       canvas->SaveAs("lambdaAPbefore.root");

       hAPafter->SetStats(0);
       hAPafter->GetXaxis()->SetTitle("\\alpha = (p^{+}_{L} - p^{-}_{L})/(p^{+}_{L} + p^{-}_{L})");
       hAPafter->GetYaxis()->SetTitle("p_{T} [GeV]");
       hAPafter->GetXaxis()->SetTitleSize(0.04);
       hAPafter->GetXaxis()->SetTitleOffset(1);
       hAPafter->GetYaxis()->SetTitleSize(0.05);
       hAPafter->Draw("colz");
       hAPafter->Write();
       gPad->Update();
       canvas->SaveAs("lambdaAPafter.root");

       hLambdaPIDproton->SetStats(0);
       hLambdaPIDproton->SetLineColor(kRed);
       h1LambdaPIDproton->SetLineColor(kRed);
       h2LambdaPIDproton->SetLineColor(kRed);
       h3LambdaPIDproton->SetLineColor(kRed);
       h4LambdaPIDproton->SetLineColor(kRed);
       h5LambdaPIDproton->SetLineColor(kRed);
       h6LambdaPIDproton->SetLineColor(kRed);
       h7LambdaPIDproton->SetLineColor(kRed);
       h8LambdaPIDproton->SetLineColor(kRed);
       h9LambdaPIDproton->SetLineColor(kRed);
       hLambdaPIDproton->Draw();
       hLambdaPIDproton->Write();

       hLambdaPIDpion->SetStats(0);
       hLambdaPIDpion->SetLineColor(kOrange);
       h1LambdaPIDpion->SetLineColor(kOrange);
       h2LambdaPIDpion->SetLineColor(kOrange);
       h3LambdaPIDpion->SetLineColor(kOrange);
       h4LambdaPIDpion->SetLineColor(kOrange);
       h5LambdaPIDpion->SetLineColor(kOrange);
       h6LambdaPIDpion->SetLineColor(kOrange);
       h7LambdaPIDpion->SetLineColor(kOrange);
       h8LambdaPIDpion->SetLineColor(kOrange);
       h9LambdaPIDpion->SetLineColor(kOrange);
       hLambdaPIDpion->Draw();
       hLambdaPIDpion->Write();

       hLambdaDCAoverdDCAproton->SetStats(0);
       hLambdaDCAoverdDCAproton->SetLineColor(kCyan);
       h1LambdaDCAoverdDCAproton->SetLineColor(kCyan);
       h2LambdaDCAoverdDCAproton->SetLineColor(kCyan);
       h3LambdaDCAoverdDCAproton->SetLineColor(kCyan);
       h4LambdaDCAoverdDCAproton->SetLineColor(kCyan);
       h5LambdaDCAoverdDCAproton->SetLineColor(kCyan);
       h6LambdaDCAoverdDCAproton->SetLineColor(kCyan);
       h7LambdaDCAoverdDCAproton->SetLineColor(kCyan);
       h8LambdaDCAoverdDCAproton->SetLineColor(kCyan);
       h9LambdaDCAoverdDCAproton->SetLineColor(kCyan);
       hLambdaDCAoverdDCAproton->Draw();
       hLambdaDCAoverdDCAproton->Write();

       hLambdaDCAoverdDCAprotontest1->SetStats(0);
       hLambdaDCAoverdDCAprotontest1->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest1->Draw();
       hLambdaDCAoverdDCAprotontest1->Write();

       hLambdaDCAoverdDCAprotontest2->SetStats(0);
       hLambdaDCAoverdDCAprotontest2->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest2->Draw();
       hLambdaDCAoverdDCAprotontest2->Write();

       hLambdaDCAoverdDCAprotontest3->SetStats(0);
       hLambdaDCAoverdDCAprotontest3->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest3->Draw();
       hLambdaDCAoverdDCAprotontest3->Write();

       hLambdaDCAoverdDCAprotontest4->SetStats(0);
       hLambdaDCAoverdDCAprotontest4->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest4->Draw();
       hLambdaDCAoverdDCAprotontest4->Write();

       hLambdaDCAoverdDCAprotontest5->SetStats(0);
       hLambdaDCAoverdDCAprotontest5->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest5->Draw();
       hLambdaDCAoverdDCAprotontest5->Write();

       hLambdaDCAoverdDCAprotontest6->SetStats(0);
       hLambdaDCAoverdDCAprotontest6->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest6->Draw();
       hLambdaDCAoverdDCAprotontest6->Write();

       hLambdaDCAoverdDCAprotontest7->SetStats(0);
       hLambdaDCAoverdDCAprotontest7->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest7->Draw();
       hLambdaDCAoverdDCAprotontest7->Write();

       hLambdaDCAoverdDCAprotontest8->SetStats(0);
       hLambdaDCAoverdDCAprotontest8->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest8->Draw();
       hLambdaDCAoverdDCAprotontest8->Write();

       hLambdaDCAoverdDCAprotontest9->SetStats(0);
       hLambdaDCAoverdDCAprotontest9->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest9->Draw();
       hLambdaDCAoverdDCAprotontest9->Write();

       hLambdaDCAoverdDCAprotontest10->SetStats(0);
       hLambdaDCAoverdDCAprotontest10->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest10->Draw();
       hLambdaDCAoverdDCAprotontest10->Write();

       hLambdaDCAoverdDCAprotontest11->SetStats(0);
       hLambdaDCAoverdDCAprotontest11->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest11->Draw();
       hLambdaDCAoverdDCAprotontest11->Write();

       hLambdaDCAoverdDCAprotontest12->SetStats(0);
       hLambdaDCAoverdDCAprotontest12->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest12->Draw();
       hLambdaDCAoverdDCAprotontest12->Write();

       hLambdaDCAoverdDCAprotontest13->SetStats(0);
       hLambdaDCAoverdDCAprotontest13->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest13->Draw();
       hLambdaDCAoverdDCAprotontest13->Write();

       hLambdaDCAoverdDCAprotontest14->SetStats(0);
       hLambdaDCAoverdDCAprotontest14->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest14->Draw();
       hLambdaDCAoverdDCAprotontest14->Write();

       hLambdaDCAoverdDCAprotontest15->SetStats(0);
       hLambdaDCAoverdDCAprotontest15->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest15->Draw();
       hLambdaDCAoverdDCAprotontest15->Write();

       hLambdaDCAoverdDCAprotontest16->SetStats(0);
       hLambdaDCAoverdDCAprotontest16->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest16->Draw();
       hLambdaDCAoverdDCAprotontest16->Write();

       hLambdaDCAoverdDCAprotontest17->SetStats(0);
       hLambdaDCAoverdDCAprotontest17->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest17->Draw();
       hLambdaDCAoverdDCAprotontest17->Write();

       hLambdaDCAoverdDCAprotontest18->SetStats(0);
       hLambdaDCAoverdDCAprotontest18->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest18->Draw();
       hLambdaDCAoverdDCAprotontest18->Write();

       hLambdaDCAoverdDCAprotontest19->SetStats(0);
       hLambdaDCAoverdDCAprotontest19->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest19->Draw();
       hLambdaDCAoverdDCAprotontest19->Write();

       hLambdaDCAoverdDCAprotontest20->SetStats(0);
       hLambdaDCAoverdDCAprotontest20->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest20->Draw();
       hLambdaDCAoverdDCAprotontest20->Write();

       hLambdaDCAoverdDCAprotontest21->SetStats(0);
       hLambdaDCAoverdDCAprotontest21->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest21->Draw();
       hLambdaDCAoverdDCAprotontest21->Write();

       hLambdaDCAoverdDCAprotontest22->SetStats(0);
       hLambdaDCAoverdDCAprotontest22->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest22->Draw();
       hLambdaDCAoverdDCAprotontest22->Write();

       hLambdaDCAoverdDCAprotontest23->SetStats(0);
       hLambdaDCAoverdDCAprotontest23->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest23->Draw();
       hLambdaDCAoverdDCAprotontest23->Write();

       hLambdaDCAoverdDCAprotontest24->SetStats(0);
       hLambdaDCAoverdDCAprotontest24->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest24->Draw();
       hLambdaDCAoverdDCAprotontest24->Write();

       hLambdaDCAoverdDCAprotontest25->SetStats(0);
       hLambdaDCAoverdDCAprotontest25->SetLineColor(kCyan);
       hLambdaDCAoverdDCAprotontest25->Draw();
       hLambdaDCAoverdDCAprotontest25->Write();

       hLambdaDCAoverdDCApiontest1->SetStats(0);
       hLambdaDCAoverdDCApiontest1->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest1->Draw();
       hLambdaDCAoverdDCApiontest1->Write();

       hLambdaDCAoverdDCApiontest2->SetStats(0);
       hLambdaDCAoverdDCApiontest2->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest2->Draw();
       hLambdaDCAoverdDCApiontest2->Write();

       hLambdaDCAoverdDCApiontest3->SetStats(0);
       hLambdaDCAoverdDCApiontest3->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest3->Draw();
       hLambdaDCAoverdDCApiontest3->Write();

       hLambdaDCAoverdDCApiontest4->SetStats(0);
       hLambdaDCAoverdDCApiontest4->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest4->Draw();
       hLambdaDCAoverdDCApiontest4->Write();

       hLambdaDCAoverdDCApiontest5->SetStats(0);
       hLambdaDCAoverdDCApiontest5->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest5->Draw();
       hLambdaDCAoverdDCApiontest5->Write();

       hLambdaDCAoverdDCApiontest6->SetStats(0);
       hLambdaDCAoverdDCApiontest6->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest6->Draw();
       hLambdaDCAoverdDCApiontest6->Write();

       hLambdaDCAoverdDCApiontest7->SetStats(0);
       hLambdaDCAoverdDCApiontest7->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest7->Draw();
       hLambdaDCAoverdDCApiontest7->Write();

       hLambdaDCAoverdDCApiontest8->SetStats(0);
       hLambdaDCAoverdDCApiontest8->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest8->Draw();
       hLambdaDCAoverdDCApiontest8->Write();

       hLambdaDCAoverdDCApiontest9->SetStats(0);
       hLambdaDCAoverdDCApiontest9->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest9->Draw();
       hLambdaDCAoverdDCApiontest9->Write();

       hLambdaDCAoverdDCApiontest10->SetStats(0);
       hLambdaDCAoverdDCApiontest10->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest10->Draw();
       hLambdaDCAoverdDCApiontest10->Write();

       hLambdaDCAoverdDCApiontest11->SetStats(0);
       hLambdaDCAoverdDCApiontest11->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest11->Draw();
       hLambdaDCAoverdDCApiontest11->Write();

       hLambdaDCAoverdDCApiontest12->SetStats(0);
       hLambdaDCAoverdDCApiontest12->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest12->Draw();
       hLambdaDCAoverdDCApiontest12->Write();

       hLambdaDCAoverdDCApiontest13->SetStats(0);
       hLambdaDCAoverdDCApiontest13->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest13->Draw();
       hLambdaDCAoverdDCApiontest13->Write();

       hLambdaDCAoverdDCApiontest14->SetStats(0);
       hLambdaDCAoverdDCApiontest14->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest14->Draw();
       hLambdaDCAoverdDCApiontest14->Write();

       hLambdaDCAoverdDCApiontest15->SetStats(0);
       hLambdaDCAoverdDCApiontest15->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest15->Draw();
       hLambdaDCAoverdDCApiontest15->Write();

       hLambdaDCAoverdDCApiontest16->SetStats(0);
       hLambdaDCAoverdDCApiontest16->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest16->Draw();
       hLambdaDCAoverdDCApiontest16->Write();

       hLambdaDCAoverdDCApiontest17->SetStats(0);
       hLambdaDCAoverdDCApiontest17->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest17->Draw();
       hLambdaDCAoverdDCApiontest17->Write();

       hLambdaDCAoverdDCApiontest18->SetStats(0);
       hLambdaDCAoverdDCApiontest18->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest18->Draw();
       hLambdaDCAoverdDCApiontest18->Write();

       hLambdaDCAoverdDCApiontest19->SetStats(0);
       hLambdaDCAoverdDCApiontest19->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest19->Draw();
       hLambdaDCAoverdDCApiontest19->Write();

       hLambdaDCAoverdDCApiontest20->SetStats(0);
       hLambdaDCAoverdDCApiontest20->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest20->Draw();
       hLambdaDCAoverdDCApiontest20->Write();

       hLambdaDCAoverdDCApiontest21->SetStats(0);
       hLambdaDCAoverdDCApiontest21->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest21->Draw();
       hLambdaDCAoverdDCApiontest21->Write();

       hLambdaDCAoverdDCApiontest22->SetStats(0);
       hLambdaDCAoverdDCApiontest22->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest22->Draw();
       hLambdaDCAoverdDCApiontest22->Write();

       hLambdaDCAoverdDCApiontest23->SetStats(0);
       hLambdaDCAoverdDCApiontest23->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest23->Draw();
       hLambdaDCAoverdDCApiontest23->Write();

       hLambdaDCAoverdDCApiontest24->SetStats(0);
       hLambdaDCAoverdDCApiontest24->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest24->Draw();
       hLambdaDCAoverdDCApiontest24->Write();

       hLambdaDCAoverdDCApiontest25->SetStats(0);
       hLambdaDCAoverdDCApiontest25->SetLineColor(kCyan);
       hLambdaDCAoverdDCApiontest25->Draw();
       hLambdaDCAoverdDCApiontest25->Write();

       hLambdaPIDpiontest0->SetStats(0);
       hLambdaPIDpiontest0->SetLineColor(kCyan);
       hLambdaPIDpiontest0->Draw();
       hLambdaPIDpiontest0->Write();

       hLambdaPIDpiontest1->SetStats(0);
       hLambdaPIDpiontest1->SetLineColor(kCyan);
       hLambdaPIDpiontest1->Draw();
       hLambdaPIDpiontest1->Write();

       hLambdaPIDpiontest2->SetStats(0);
       hLambdaPIDpiontest2->SetLineColor(kCyan);
       hLambdaPIDpiontest2->Draw();
       hLambdaPIDpiontest2->Write();

       hLambdaPIDpiontest3->SetStats(0);
       hLambdaPIDpiontest3->SetLineColor(kCyan);
       hLambdaPIDpiontest3->Draw();
       hLambdaPIDpiontest3->Write();

       hLambdaPIDpiontest4->SetStats(0);
       hLambdaPIDpiontest4->SetLineColor(kCyan);
       hLambdaPIDpiontest4->Draw();
       hLambdaPIDpiontest4->Write();

       hLambdaPIDpiontest5->SetStats(0);
       hLambdaPIDpiontest5->SetLineColor(kCyan);
       hLambdaPIDpiontest5->Draw();
       hLambdaPIDpiontest5->Write();

       hLambdaPIDpiontest6->SetStats(0);
       hLambdaPIDpiontest6->SetLineColor(kCyan);
       hLambdaPIDpiontest6->Draw();
       hLambdaPIDpiontest6->Write();

       hLambdaPIDpiontest7->SetStats(0);
       hLambdaPIDpiontest7->SetLineColor(kCyan);
       hLambdaPIDpiontest7->Draw();
       hLambdaPIDpiontest7->Write();

       hLambdaPIDpiontest8->SetStats(0);
       hLambdaPIDpiontest8->SetLineColor(kCyan);
       hLambdaPIDpiontest8->Draw();
       hLambdaPIDpiontest8->Write();

       hLambdaPIDpiontest9->SetStats(0);
       hLambdaPIDpiontest9->SetLineColor(kCyan);
       hLambdaPIDpiontest9->Draw();
       hLambdaPIDpiontest9->Write();

       hLambdaPIDpiontest10->SetStats(0);
       hLambdaPIDpiontest10->SetLineColor(kCyan);
       hLambdaPIDpiontest10->Draw();
       hLambdaPIDpiontest10->Write();

       hLambdaPIDprotontest0->SetStats(0);
       hLambdaPIDprotontest0->SetLineColor(kCyan);
       hLambdaPIDprotontest0->Draw();
       hLambdaPIDprotontest0->Write();

       hLambdaPIDprotontest1->SetStats(0);
       hLambdaPIDprotontest1->SetLineColor(kCyan);
       hLambdaPIDprotontest1->Draw();
       hLambdaPIDprotontest1->Write();

       hLambdaPIDprotontest2->SetStats(0);
       hLambdaPIDprotontest2->SetLineColor(kCyan);
       hLambdaPIDprotontest2->Draw();
       hLambdaPIDprotontest2->Write();

       hLambdaPIDprotontest3->SetStats(0);
       hLambdaPIDprotontest3->SetLineColor(kCyan);
       hLambdaPIDprotontest3->Draw();
       hLambdaPIDprotontest3->Write();

       hLambdaPIDprotontest4->SetStats(0);
       hLambdaPIDprotontest4->SetLineColor(kCyan);
       hLambdaPIDprotontest4->Draw();
       hLambdaPIDprotontest4->Write();

       hLambdaPIDprotontest5->SetStats(0);
       hLambdaPIDprotontest5->SetLineColor(kCyan);
       hLambdaPIDprotontest5->Draw();
       hLambdaPIDprotontest5->Write();

       hLambdaPIDprotontest6->SetStats(0);
       hLambdaPIDprotontest6->SetLineColor(kCyan);
       hLambdaPIDprotontest6->Draw();
       hLambdaPIDprotontest6->Write();

       hLambdaPIDprotontest7->SetStats(0);
       hLambdaPIDprotontest7->SetLineColor(kCyan);
       hLambdaPIDprotontest7->Draw();
       hLambdaPIDprotontest7->Write();

       hLambdaPIDprotontest8->SetStats(0);
       hLambdaPIDprotontest8->SetLineColor(kCyan);
       hLambdaPIDprotontest8->Draw();
       hLambdaPIDprotontest8->Write();

       hLambdaPIDprotontest9->SetStats(0);
       hLambdaPIDprotontest9->SetLineColor(kCyan);
       hLambdaPIDprotontest9->Draw();
       hLambdaPIDprotontest9->Write();

       hLambdaPIDprotontest10->SetStats(0);
       hLambdaPIDprotontest10->SetLineColor(kCyan);
       hLambdaPIDprotontest10->Draw();
       hLambdaPIDprotontest10->Write();

       hLambdaDCAoverdDCApion->SetStats(0);
       hLambdaDCAoverdDCApion->SetLineColor(kCyan+3);
       h1LambdaDCAoverdDCApion->SetLineColor(kCyan + 3);
       h2LambdaDCAoverdDCApion->SetLineColor(kCyan + 3);
       h3LambdaDCAoverdDCApion->SetLineColor(kCyan + 3);
       h4LambdaDCAoverdDCApion->SetLineColor(kCyan + 3);
       h5LambdaDCAoverdDCApion->SetLineColor(kCyan + 3);
       h6LambdaDCAoverdDCApion->SetLineColor(kCyan + 3);
       h7LambdaDCAoverdDCApion->SetLineColor(kCyan + 3);
       h8LambdaDCAoverdDCApion->SetLineColor(kCyan + 3);
       h9LambdaDCAoverdDCApion->SetLineColor(kCyan + 3);
       hLambdaDCAoverdDCApion->Draw();
       hLambdaDCAoverdDCApion->Write();

       hLambdaAlpha->SetStats(0);
       hLambdaAlpha->SetLineColor(kGray);
       h1LambdaAlpha->SetLineColor(kGray);
       h2LambdaAlpha->SetLineColor(kGray);
       h3LambdaAlpha->SetLineColor(kGray);
       h4LambdaAlpha->SetLineColor(kGray);
       h5LambdaAlpha->SetLineColor(kGray);
       h6LambdaAlpha->SetLineColor(kGray);
       h7LambdaAlpha->SetLineColor(kGray);
       h8LambdaAlpha->SetLineColor(kGray);
       h9LambdaAlpha->SetLineColor(kGray);
       hLambdaAlpha->Draw();
       hLambdaAlpha->Write();

       hLambdaptrel->SetStats(0);
       hLambdaptrel->SetLineColor(kGreen);
       h1Lambdaptrel->SetLineColor(kGreen);
       h2Lambdaptrel->SetLineColor(kGreen);
       h3Lambdaptrel->SetLineColor(kGreen);
       h4Lambdaptrel->SetLineColor(kGreen);
       h5Lambdaptrel->SetLineColor(kGreen);
       h6Lambdaptrel->SetLineColor(kGreen);
       h7Lambdaptrel->SetLineColor(kGreen);
       h8Lambdaptrel->SetLineColor(kGreen);
       h9Lambdaptrel->SetLineColor(kGreen);
       hLambdaptrel->Draw();
       hLambdaptrel->Write();

       hLambdapt->SetStats(0);
       hLambdapt->SetLineColor(kBlue);
       h1Lambdapt->SetLineColor(kBlue);
       h2Lambdapt->SetLineColor(kBlue);
       h3Lambdapt->SetLineColor(kBlue);
       h4Lambdapt->SetLineColor(kBlue);
       h5Lambdapt->SetLineColor(kBlue);
       h6Lambdapt->SetLineColor(kBlue);
       h7Lambdapt->SetLineColor(kBlue);
       h8Lambdapt->SetLineColor(kBlue);
       h9Lambdapt->SetLineColor(kBlue);
       hLambdapt->Draw();
       hLambdapt->Write();

       //hLambdachi2->SetStats(0);
       //hLambdachi2->SetLineColor(kMagenta);
       //hLambdachi2->Draw();
       //hLambdachi2->Write();

       hLambdaEta->SetStats(0);
       hLambdaEta->SetLineColor(kGreen+3);
       h1LambdaEta->SetLineColor(kGreen + 3);
       h2LambdaEta->SetLineColor(kGreen + 3);
       h3LambdaEta->SetLineColor(kGreen + 3);
       h4LambdaEta->SetLineColor(kGreen + 3);
       h5LambdaEta->SetLineColor(kGreen + 3);
       h6LambdaEta->SetLineColor(kGreen + 3);
       h7LambdaEta->SetLineColor(kGreen + 3);
       h8LambdaEta->SetLineColor(kGreen + 3);
       h9LambdaEta->SetLineColor(kGreen + 3);
       hLambdaEta->Draw();
       hLambdaEta->Write();

       hLambdatracklength->SetStats(0);
       hLambdatracklength->SetLineColor(kViolet);
       h1Lambdatracklength->SetLineColor(kViolet);
       h2Lambdatracklength->SetLineColor(kViolet);
       h3Lambdatracklength->SetLineColor(kViolet);
       h4Lambdatracklength->SetLineColor(kViolet);
       h5Lambdatracklength->SetLineColor(kViolet);
       h6Lambdatracklength->SetLineColor(kViolet);
       h7Lambdatracklength->SetLineColor(kViolet);
       h8Lambdatracklength->SetLineColor(kViolet);
       h9Lambdatracklength->SetLineColor(kViolet);
       hLambdatracklength->Draw();
       hLambdatracklength->Write();

       hLambdaKaonCont->SetStats(0);
       hLambdaKaonCont->Draw();
       hLambdaKaonCont->Write();

       hLambdaPhotonCont->SetStats(0);
       hLambdaPhotonCont->SetLineColor(kBlack);
       h1LambdaPhotonCont->SetLineColor(kBlack);
       h2LambdaPhotonCont->SetLineColor(kBlack);
       h3LambdaPhotonCont->SetLineColor(kBlack);
       h4LambdaPhotonCont->SetLineColor(kBlack);
       h5LambdaPhotonCont->SetLineColor(kBlack);
       h6LambdaPhotonCont->SetLineColor(kBlack);
       h7LambdaPhotonCont->SetLineColor(kBlack);
       h8LambdaPhotonCont->SetLineColor(kBlack);
       h9LambdaPhotonCont->SetLineColor(kBlack);
       hLambdaPhotonCont->Draw();
       hLambdaPhotonCont->Write();

       hDedxLproton->SetStats(0);
       hDedxLproton->SetFillColor(kRed);
       hDedxLproton->GetXaxis()->SetTitle("Likelihood");
       hDedxLproton->GetYaxis()->SetTitle("Count");
       hDedxLproton->Draw();
       hDedxLproton->Write();

       hDedxLpion->SetStats(0);
       hDedxLpion->SetFillColor(kRed);
       hDedxLpion->GetXaxis()->SetTitle("Likelihood");
       hDedxLpion->GetYaxis()->SetTitle("Count");
       hDedxLpion->Draw();
       hDedxLpion->Write();

       hdDCAproton->SetStats(0);
       hdDCAproton->SetFillColor(kRed);
       hdDCAproton->GetXaxis()->SetTitle("dDCA");
       hdDCAproton->GetYaxis()->SetTitle("Count");
       hdDCAproton->Draw();
       hdDCAproton->Write();

       hDCAproton->SetStats(0);
       hDCAproton->SetFillColor(kRed);
       hDCAproton->GetXaxis()->SetTitle("DCA");
       hDCAproton->GetYaxis()->SetTitle("Count");
       hDCAproton->Draw();
       hDCAproton->Write();

       hdDCApion->SetStats(0);
       hdDCApion->SetFillColor(kRed);
       hdDCApion->GetXaxis()->SetTitle("dDCA");
       hdDCApion->GetYaxis()->SetTitle("Count");
       hdDCApion->Draw();
       hdDCApion->Write();

       hDCApion->SetStats(0);
       hDCApion->SetFillColor(kRed);
       hDCApion->GetXaxis()->SetTitle("DCA");
       hDCApion->GetYaxis()->SetTitle("Count");
       hDCApion->Draw();
       hDCApion->Write();

       hDCAoverdDCApion->SetStats(0);
       hDCAoverdDCApion->SetFillColor(kRed);
       hDCAoverdDCApion->GetXaxis()->SetTitle("DCA/dDCA");
       hDCAoverdDCApion->GetYaxis()->SetTitle("Count");
       hDCAoverdDCApion->Draw();
       hDCAoverdDCApion->Write();

       hDCAoverdDCAproton->SetStats(0);
       hDCAoverdDCAproton->SetFillColor(kRed);
       hDCAoverdDCAproton->GetXaxis()->SetTitle("DCA/dDCA");
       hDCAoverdDCAproton->GetYaxis()->SetTitle("Count");
       hDCAoverdDCAproton->Draw();
       hDCAoverdDCAproton->Write();

       hprotonDCAvsdDCAbefore->SetStats(0);
       hprotonDCAvsdDCAbefore->GetXaxis()->SetTitle("dDCA");
       hprotonDCAvsdDCAbefore->GetYaxis()->SetTitle("DCA");
       hprotonDCAvsdDCAbefore->Draw("colz");
       hprotonDCAvsdDCAbefore->Write();
       gPad->Update();
       canvas->SaveAs("lambdaprotonDCAvsDCAbefore.root");

       hprotonDCAvsdDCAafter->SetStats(0);
       hprotonDCAvsdDCAafter->GetXaxis()->SetTitle("dDCA");
       hprotonDCAvsdDCAafter->GetYaxis()->SetTitle("DCA");
       hprotonDCAvsdDCAafter->Draw("colz");
       hprotonDCAvsdDCAafter->Write();
       gPad->Update();
       canvas->SaveAs("lambdaprotonDCAvsDCAafter.root");

       hprotonDCAvsdDCArejected->SetStats(0);
       hprotonDCAvsdDCArejected->GetXaxis()->SetTitle("dDCA");
       hprotonDCAvsdDCArejected->GetYaxis()->SetTitle("DCA");
       hprotonDCAvsdDCArejected->Draw("colz");
       hprotonDCAvsdDCArejected->Write();
       gPad->Update();
       canvas->SaveAs("lambdaprotonDCAvsDCArejected.root");

       hpionDCAvsdDCAbefore->SetStats(0);
       hpionDCAvsdDCAbefore->GetXaxis()->SetTitle("dDCA");
       hpionDCAvsdDCAbefore->GetYaxis()->SetTitle("DCA");
       hpionDCAvsdDCAbefore->Draw("colz");
       hpionDCAvsdDCAbefore->Write();
       gPad->Update();
       canvas->SaveAs("lambdapionDCAvsDCAbefore.root");

       hpionDCAvsdDCAafter->SetStats(0);
       hpionDCAvsdDCAafter->GetXaxis()->SetTitle("dDCA");
       hpionDCAvsdDCAafter->GetYaxis()->SetTitle("DCA");
       hpionDCAvsdDCAafter->Draw("colz");
       hpionDCAvsdDCAafter->Write();
       gPad->Update();
       canvas->SaveAs("lambdapionDCAvsDCAafter.root");

       hpionDCAvsdDCArejected->SetStats(0);
       hpionDCAvsdDCArejected->GetXaxis()->SetTitle("dDCA");
       hpionDCAvsdDCArejected->GetYaxis()->SetTitle("DCA");
       hpionDCAvsdDCArejected->Draw("colz");
       hpionDCAvsdDCArejected->Write();
       gPad->Update();
       canvas->SaveAs("lambdapionDCAvsDCArejected.root");



       hpionDCAvspTbefore->SetStats(0);
       hpionDCAvspTbefore->GetXaxis()->SetTitle("pT");
       hpionDCAvspTbefore->GetYaxis()->SetTitle("DCA");
       hpionDCAvspTbefore->Draw("colz");
       hpionDCAvspTbefore->Write();
       gPad->Update();
       canvas->SaveAs("lambdapionDCAvspTbefore.root");


       hpiondDCAvspTbefore->SetStats(0);
       hpiondDCAvspTbefore->GetXaxis()->SetTitle("pT");
       hpiondDCAvspTbefore->GetYaxis()->SetTitle("dDCA");
       hpiondDCAvspTbefore->Draw("colz");
       hpiondDCAvspTbefore->Write();
       gPad->Update();
       canvas->SaveAs("lambdapiondDCAvspTbefore.root");


       hprotonDCAvspTbefore->SetStats(0);
       hprotonDCAvspTbefore->GetXaxis()->SetTitle("pT");
       hprotonDCAvspTbefore->GetYaxis()->SetTitle("DCA");
       hprotonDCAvspTbefore->Draw("colz");
       hprotonDCAvspTbefore->Write();
       gPad->Update();
       canvas->SaveAs("lambdaprotonDCAvspTbefore.root");


       hprotondDCAvspTbefore->SetStats(0);
       hprotondDCAvspTbefore->GetXaxis()->SetTitle("pT");
       hprotondDCAvspTbefore->GetYaxis()->SetTitle("dDCA");
       hprotondDCAvspTbefore->Draw("colz");
       hprotondDCAvspTbefore->Write();
       gPad->Update();
       canvas->SaveAs("lambdaprotondDCAvspTbefore.root");

       hprotonDCAoverdDCAvspT->SetStats(0);
       hprotonDCAoverdDCAvspT->GetXaxis()->SetTitle("pT");
       hprotonDCAoverdDCAvspT->GetYaxis()->SetTitle("dDCA");
       hprotonDCAoverdDCAvspT->Draw("colz");
       hprotonDCAoverdDCAvspT->Write();
       gPad->Update();
       canvas->SaveAs("lambdaprotonDCAoverdDCAvspT.root");

       hpionDCAoverdDCAvspT->SetStats(0);
       hpionDCAoverdDCAvspT->GetXaxis()->SetTitle("pT");
       hpionDCAoverdDCAvspT->GetYaxis()->SetTitle("dDCA");
       hpionDCAoverdDCAvspT->Draw("colz");
       hpionDCAoverdDCAvspT->Write();
       gPad->Update();
       canvas->SaveAs("lambdapionDCAoverdDCAvspT.root");

       hdEdxBothBefore->SetStats(0);
       hdEdxBothBefore->GetXaxis()->SetTitle("p [GeV]");
       hdEdxBothBefore->GetYaxis()->SetTitle("dE/dx");
       hdEdxBothBefore->Draw("colz");
       hdEdxBothBefore->Write();
       gPad->Update();
       canvas->SaveAs("lambdadEdxBothBefore.root");

       hdEdxProtonBefore->SetStats(0);
       hdEdxProtonBefore->GetXaxis()->SetTitle("p [GeV]");
       hdEdxProtonBefore->GetYaxis()->SetTitle("dE/dx");
       hdEdxProtonBefore->Draw("colz");
       hdEdxProtonBefore->Write();
       gPad->Update();
       canvas->SaveAs("lambdadEdxProtonBefore.root");

       hdEdxPionBefore->SetStats(0);
       hdEdxPionBefore->GetXaxis()->SetTitle("p [GeV]");
       hdEdxPionBefore->GetYaxis()->SetTitle("dE/dx");
       hdEdxPionBefore->Draw("colz");
       hdEdxPionBefore->Write();
       gPad->Update();
       canvas->SaveAs("lambdadEdxPionBefore.root");
       
       hdEdxBothAfterLcut->SetStats(0);
       hdEdxBothAfterLcut->GetXaxis()->SetTitle("p [GeV]");
       hdEdxBothAfterLcut->GetYaxis()->SetTitle("dE/dx");
       hdEdxBothAfterLcut->Draw("colz");
       hdEdxBothAfterLcut->Write();
       gPad->Update();
       canvas->SaveAs("lambdadEdxBothAfterLcut.root");

       hdEdxProtonAfterLcut->SetStats(0);
       hdEdxProtonAfterLcut->GetXaxis()->SetTitle("p [GeV]");
       hdEdxProtonAfterLcut->GetYaxis()->SetTitle("dE/dx");
       hdEdxProtonAfterLcut->Draw("colz");
       hdEdxProtonAfterLcut->Write();
       gPad->Update();
       canvas->SaveAs("lambdadEdxProtonAfterLcut.root");

       hdEdxPionAfterLcut->SetStats(0);
       hdEdxPionAfterLcut->GetXaxis()->SetTitle("p [GeV]");
       hdEdxPionAfterLcut->GetYaxis()->SetTitle("dE/dx");
       hdEdxPionAfterLcut->Draw("colz");
       hdEdxPionAfterLcut->Write();
       gPad->Update();
       canvas->SaveAs("lambdadEdxPionAfterLcut.root");

       hdEdxBothAfterAllcuts->SetStats(0);
       hdEdxBothAfterAllcuts->GetXaxis()->SetTitle("p [GeV]");
       hdEdxBothAfterAllcuts->GetYaxis()->SetTitle("dE/dx");
       hdEdxBothAfterAllcuts->Draw("colz");
       hdEdxBothAfterAllcuts->Write();
       gPad->Update();
       canvas->SaveAs("lambdadEdxBothAfterAllcuts.root");

       hdEdxProtonAfterAllcuts->SetStats(0);
       hdEdxProtonAfterAllcuts->GetXaxis()->SetTitle("p [GeV]");
       hdEdxProtonAfterAllcuts->GetYaxis()->SetTitle("dE/dx");
       hdEdxProtonAfterAllcuts->Draw("colz");
       hdEdxProtonAfterAllcuts->Write();
       gPad->Update();
       canvas->SaveAs("lambdadEdxProtonAfterAllcuts.root");

       hdEdxPionAfterAllcuts->SetStats(0);
       hdEdxPionAfterAllcuts->GetXaxis()->SetTitle("p [GeV]");
       hdEdxPionAfterAllcuts->GetYaxis()->SetTitle("dE/dx");
       hdEdxPionAfterAllcuts->Draw("colz");
       hdEdxPionAfterAllcuts->Write();
       gPad->Update();
       canvas->SaveAs("lambdadEdxPionAfterAllcuts.root");

       hs1->Add(h1Lambda);
       hs1->Add(h1LambdaPIDproton);
       hs1->Add(h1LambdaPIDpion);
       hs1->Add(h1LambdaDCAoverdDCAproton);
       hs1->Add(h1LambdaDCAoverdDCApion);
       hs1->Add(h1LambdaAlpha);
       hs1->Add(h1Lambdaptrel);
       hs1->Add(h1Lambdapt);
       //hs1->Add(h1Lambdachi2);
       hs1->Add(h1LambdaEta);
       hs1->Add(h1Lambdatracklength);
       hs1->Add(h1LambdaKaonCont);
       hs1->Add(h1LambdaPhotonCont);
       //hs1->GetXaxis()->SetTitle("Mass [Gev]");
       //hs1->GetYaxis()->SetTitle("Count");
       hs1->Draw("nostack");
       hs1->Write();
       gPad->Update();
       canvas->SaveAs("lambdastack1.root");

       hs2->Add(h2Lambda);
       hs2->Add(h2LambdaPIDproton);
       hs2->Add(h2LambdaPIDpion);
       hs2->Add(h2LambdaDCAoverdDCAproton);
       hs2->Add(h2LambdaDCAoverdDCApion);
       hs2->Add(h2LambdaAlpha);
       hs2->Add(h2Lambdaptrel);
       hs2->Add(h2Lambdapt);
       //hs1->Add(h2Lambdachi2);
       hs2->Add(h2LambdaEta);
       hs2->Add(h2Lambdatracklength);
       hs2->Add(h2LambdaKaonCont);
       hs2->Add(h2LambdaPhotonCont);
       //hs1->GetXaxis()->SetTitle("Mass [Gev]");
       //hs1->GetYaxis()->SetTitle("Count");
       hs2->Draw("nostack");
       hs2->Write();
       gPad->Update();
       canvas->SaveAs("lambdastack2.root");

       hs3->Add(h3Lambda);
       hs3->Add(h3LambdaPIDproton);
       hs3->Add(h3LambdaPIDpion);
       hs3->Add(h3LambdaDCAoverdDCAproton);
       hs3->Add(h3LambdaDCAoverdDCApion);
       hs3->Add(h3LambdaAlpha);
       hs3->Add(h3Lambdaptrel);
       hs3->Add(h3Lambdapt);
       //hs3->Add(h2Lambdachi2);
       hs3->Add(h3LambdaEta);
       hs3->Add(h3Lambdatracklength);
       hs3->Add(h3LambdaKaonCont);
       hs3->Add(h3LambdaPhotonCont);
       //hs3->GetXaxis()->SetTitle("Mass [Gev]");
       //hs3->GetYaxis()->SetTitle("Count");
       hs3->Draw("nostack");
       hs3->Write();
       gPad->Update();
       canvas->SaveAs("lambdastack3.root");

       hs4->Add(h4Lambda);
       hs4->Add(h4LambdaPIDproton);
       hs4->Add(h4LambdaPIDpion);
       hs4->Add(h4LambdaDCAoverdDCAproton);
       hs4->Add(h4LambdaDCAoverdDCApion);
       hs4->Add(h4LambdaAlpha);
       hs4->Add(h4Lambdaptrel);
       hs4->Add(h4Lambdapt);
       //hs3->Add(h2Lambdachi2);
       hs4->Add(h4LambdaEta);
       hs4->Add(h4Lambdatracklength);
       hs4->Add(h4LambdaKaonCont);
       hs4->Add(h4LambdaPhotonCont);
       //hs3->GetXaxis()->SetTitle("Mass [Gev]");
       //hs3->GetYaxis()->SetTitle("Count");
       hs4->Draw("nostack");
       hs4->Write();
       gPad->Update();
       canvas->SaveAs("lambdastack4.root");

       hs5->Add(h5Lambda);
       hs5->Add(h5LambdaPIDproton);
       hs5->Add(h5LambdaPIDpion);
       hs5->Add(h5LambdaDCAoverdDCAproton);
       hs5->Add(h5LambdaDCAoverdDCApion);
       hs5->Add(h5LambdaAlpha);
       hs5->Add(h5Lambdaptrel);
       hs5->Add(h5Lambdapt);
       //hs3->Add(h2Lambdachi2);
       hs5->Add(h5LambdaEta);
       hs5->Add(h5Lambdatracklength);
       hs5->Add(h5LambdaKaonCont);
       hs5->Add(h5LambdaPhotonCont);
       //hs3->GetXaxis()->SetTitle("Mass [Gev]");
       //hs3->GetYaxis()->SetTitle("Count");
       hs5->Draw("nostack");
       hs5->Write();
       gPad->Update();
       canvas->SaveAs("lambdastack5.root");

       hs6->Add(h6Lambda);
       hs6->Add(h6LambdaPIDproton);
       hs6->Add(h6LambdaPIDpion);
       hs6->Add(h6LambdaDCAoverdDCAproton);
       hs6->Add(h6LambdaDCAoverdDCApion);
       hs6->Add(h6LambdaAlpha);
       hs6->Add(h6Lambdaptrel);
       hs6->Add(h6Lambdapt);
       //hs3->Add(h2Lambdachi2);
       hs6->Add(h6LambdaEta);
       hs6->Add(h6Lambdatracklength);
       hs6->Add(h6LambdaKaonCont);
       hs6->Add(h6LambdaPhotonCont);
       //hs3->GetXaxis()->SetTitle("Mass [Gev]");
       //hs3->GetYaxis()->SetTitle("Count");
       hs6->Draw("nostack");
       hs6->Write();
       gPad->Update();
       canvas->SaveAs("lambdastack6.root");

       hs7->Add(h7Lambda);
       hs7->Add(h7LambdaPIDproton);
       hs7->Add(h7LambdaPIDpion);
       hs7->Add(h7LambdaDCAoverdDCAproton);
       hs7->Add(h7LambdaDCAoverdDCApion);
       hs7->Add(h7LambdaAlpha);
       hs7->Add(h7Lambdaptrel);
       hs7->Add(h7Lambdapt);
       //hs3->Add(h2Lambdachi2);
       hs7->Add(h7LambdaEta);
       hs7->Add(h7Lambdatracklength);
       hs7->Add(h7LambdaKaonCont);
       hs7->Add(h7LambdaPhotonCont);
       //hs3->GetXaxis()->SetTitle("Mass [Gev]");
       //hs3->GetYaxis()->SetTitle("Count");
       hs7->Draw("nostack");
       hs7->Write();
       gPad->Update();
       canvas->SaveAs("lambdastack7.root");

       hs8->Add(h8Lambda);
       hs8->Add(h8LambdaPIDproton);
       hs8->Add(h8LambdaPIDpion);
       hs8->Add(h8LambdaDCAoverdDCAproton);
       hs8->Add(h8LambdaDCAoverdDCApion);
       hs8->Add(h8LambdaAlpha);
       hs8->Add(h8Lambdaptrel);
       hs8->Add(h8Lambdapt);
       //hs3->Add(h2Lambdachi2);
       hs8->Add(h8LambdaEta);
       hs8->Add(h8Lambdatracklength);
       hs8->Add(h8LambdaKaonCont);
       hs8->Add(h8LambdaPhotonCont);
       //hs3->GetXaxis()->SetTitle("Mass [Gev]");
       //hs3->GetYaxis()->SetTitle("Count");
       hs8->Draw("nostack");
       hs8->Write();
       gPad->Update();
       canvas->SaveAs("lambdastack8.root");

       hs9->Add(h9Lambda);
       hs9->Add(h9LambdaPIDproton);
       hs9->Add(h9LambdaPIDpion);
       hs9->Add(h9LambdaDCAoverdDCAproton);
       hs9->Add(h9LambdaDCAoverdDCApion);
       hs9->Add(h9LambdaAlpha);
       hs9->Add(h9Lambdaptrel);
       hs9->Add(h9Lambdapt);
       //hs3->Add(h2Lambdachi2);
       hs9->Add(h9LambdaEta);
       hs9->Add(h9Lambdatracklength);
       hs9->Add(h9LambdaKaonCont);
       hs9->Add(h9LambdaPhotonCont);
       //hs3->GetXaxis()->SetTitle("Mass [Gev]");
       //hs3->GetYaxis()->SetTitle("Count");
       hs9->Draw("nostack");
       hs9->Write();
       gPad->Update();
       canvas->SaveAs("lambdastack9.root");

       hs->Add(hLambda);
       hs->Add(hLambdaPIDproton);
       hs->Add(hLambdaPIDpion);
       hs->Add(hLambdaDCAoverdDCAproton);
       hs->Add(hLambdaDCAoverdDCApion);
       hs->Add(hLambdaAlpha);
       hs->Add(hLambdaptrel);
       hs->Add(hLambdapt);
       //hs->Add(hLambdachi2);
       hs->Add(hLambdaEta);
       hs->Add(hLambdatracklength);
       hs->Add(hLambdaKaonCont);
       hs->Add(hLambdaPhotonCont);
       //hs->GetXaxis()->SetTitle("Mass [Gev]");
       //hs->GetYaxis()->SetTitle("Count");
       hs->Draw("nostack");
       hs->Write();
       gPad->Update();
       canvas->SaveAs("lambdastack.root");

       file.Write();
       file.Close();
       cout << "Histograms written to " << opts.GetOutput() << endl;


        return 0;
    }