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
    Double_t ptbin1l = 0.0;
    Double_t ptbin1u = 0.1;

    Double_t ptbin2l = 0.1;
    Double_t ptbin2u = 0.2;

    Double_t ptbin3l = 0.2;
    Double_t ptbin3u = 0.3;

    Double_t ptbin4l = 0.3;
    Double_t ptbin4u = 0.4;

    Double_t ptbin5l = 0.4;
    Double_t ptbin5u = 0.5;

    Double_t ptbin6l = 4.0;
    Double_t ptbin6u = 5.0;

    Double_t ptbin7l = 5.0;
    Double_t ptbin7u = 6.0;

    Double_t ptbin8l = 6.0;
    Double_t ptbin8u = 7.0;

    Double_t ptbin9l = 7.0;
    Double_t ptbin9u = 8.0;

    Double_t ptbin10l = 8.0;
    Double_t ptbin10u = 9.0;

    Double_t ptbin11l = 9.0;
    Double_t ptbin11u = 10.0;

    Double_t ptbin12l = 10.0;
    Double_t ptbin12u = 11.0;

    Double_t ptbin13l = 11.0;
    Double_t ptbin13u = 12.0;

    Double_t ptbin14l = 12.0;
    Double_t ptbin14u = 13.0;

    Double_t ptbin15l = 13.0;
    Double_t ptbin15u = 14.0;

    Double_t ptbin16l = 14.0;
    Double_t ptbin16u = 15.0;

    Double_t ptbin17l = 15.0;
    Double_t ptbin17u = 16.0;

    Double_t ptbin18l = 16.0;
    Double_t ptbin18u = 17.0;

    Double_t ptbin19l = 17.0;
    Double_t ptbin19u = 18.0;

    Double_t ptbin20l = 18.0;
    Double_t ptbin20u = 19.0;


    Double_t kaonbinw = (kaonupperbinrange-kaonlowerbinrange) / kaonbins;
    Double_t lambdabinw = (lambdaupperbinrange-lambdalowerbinrange) / lambdabins;

    //total kaon plot
    TH1F* hKaon = new TH1F("Kaon", "Kaon", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    //kaon plots in pt bins
    TH1F* hKaon1 = new TH1F("Kaon1", "Kaon1", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon2 = new TH1F("Kaon2", "Kaon2", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon3 = new TH1F("Kaon3", "Kaon3", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon4 = new TH1F("Kaon4", "Kaon4", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon5 = new TH1F("Kaon5", "Kaon5", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon6 = new TH1F("Kaon6", "Kaon6", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon7 = new TH1F("Kaon7", "Kaon7", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon8 = new TH1F("Kaon8", "Kaon8", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon9 = new TH1F("Kaon9", "Kaon9", kaonbins, kaonlowerbinrange, kaonupperbinrange);

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
    TH1F* hLambdapTbefore = new TH1F("Lambda pT", "Lambda pT", 1000, 0, 25);
    TH1F* hLambdapTafter = new TH1F("Lambda pT", "Lambda pT", 1000, 0, 25);
    TH1F* hLambdaPIDproton = new TH1F("Lambda PID Proton", "Lambda PID Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaPIDpion = new TH1F("Lambda PID Pion", "Lambda PID Pion", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaAlpha = new TH1F("Lambda Alpha", "Lambda Alpha", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaptrel = new TH1F("Lambda pTrel", "Lambda pT_rel", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdapt = new TH1F("Lambda pT", "Lambda pT", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaEta = new TH1F("Lambda Psuedorapidity", "Lambda Psuedorapidity", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdatracklength = new TH1F("Lambda Track Length", "Lambda Track Length", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambdaDCAoverdDCAproton = new TH1F("Lambda dDCA Proton", "Lambda dDCA Proton", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
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

    TH1F* h2Lambda = new TH1F("Lambda2", "Lambda2", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
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

    TH1F* h3Lambda = new TH1F("Lambda3", "Lambda3", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
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

    TH1F* h4Lambda = new TH1F("Lambda4", "Lambda4", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
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

    TH1F* h5Lambda = new TH1F("Lambda5", "Lambda5", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
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

    TH1F* h6Lambda = new TH1F("Lambda6", "Lambda6", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
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

    TH1F* h7Lambda = new TH1F("Lambda7", "Lambda7", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
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

    TH1F* h8Lambda = new TH1F("Lambda8", "Lambda8", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
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

    TH1F* h9Lambda = new TH1F("Lambda9", "Lambda9", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h10Lambda = new TH1F("Lambda10", "Lambda10", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h11Lambda = new TH1F("Lambda11", "Lambda11", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h12Lambda = new TH1F("Lambda12", "Lambda12", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h13Lambda = new TH1F("Lambda13", "Lambda13", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h14Lambda = new TH1F("Lambda14", "Lambda14", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h15Lambda = new TH1F("Lambda15", "Lambda15", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h16Lambda = new TH1F("Lambda16", "Lambda16", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h17Lambda = new TH1F("Lambda17", "Lambda17", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h18Lambda = new TH1F("Lambda18", "Lambda18", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h19Lambda = new TH1F("Lambda19", "Lambda19", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* h20Lambda = new TH1F("Lambda20", "Lambda20", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
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
    TH1F* hLambda10 = new TH1F("Lambda10", "Lambda pT Bin 10", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda11 = new TH1F("Lambda11", "Lambda pT Bin 11", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda12 = new TH1F("Lambda12", "Lambda pT Bin 12", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda13 = new TH1F("Lambda13", "Lambda pT Bin 13", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda14 = new TH1F("Lambda14", "Lambda pT Bin 14", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda15 = new TH1F("Lambda15", "Lambda pT Bin 15", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda16 = new TH1F("Lambda16", "Lambda pT Bin 16", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda17 = new TH1F("Lambda17", "Lambda pT Bin 17", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda18 = new TH1F("Lambda18", "Lambda pT Bin 18", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda19 = new TH1F("Lambda19", "Lambda pT Bin 19", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda20 = new TH1F("Lambda20", "Lambda pT Bin 20", lambdabins, lambdalowerbinrange, lambdaupperbinrange);

    //Armenteros plot for particle ID lambdas vs kaons
    TH2F* hAPbefore = new TH2F("AP", "Armenteros-Podolansky Plot (After H1FindLambda)", 200, -1, 1, 200, 0, 0.3);
    TH2F* hAPafter = new TH2F("AP", "Armenteros-Podolansky Plot (After All Cuts)", 200, -1, 1, 200, 0, 0.3);

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

        
        //Call H1PartLambda class data that references H1CentralFittedTrack for Pion and Proton
        const H1CentralFittedTrack* ProtonCFTrack = LambdaCand[loop]->GetProtonTrack();
        const H1CentralFittedTrack* PionCFTrack = LambdaCand[loop]->GetPionTrack();


        //Getting H1PartSelTracks for daughters
        const H1PartSelTrack* ProtonPSTrack = LambdaCand[loop]->GetProton();
        const H1PartSelTrack* PionPSTrack = LambdaCand[loop]->GetPion();

       
        //Prepating DCA/dDCA information to get a histogram to help visualize this cut
        const H1CentralTrack* protonFittedNoV = ProtonCFTrack->GetCentralTrack();
        const H1Trajectory* protonTrajectory = protonFittedNoV->GetTrajectory();

        const H1CentralTrack* pionFittedNoV = PionCFTrack->GetCentralTrack();
        const H1Trajectory* pionTrajectory = pionFittedNoV->GetTrajectory();

        H1SelVertex* ProtonVertex = (H1SelVertex*)LambdaCand[loop]->GetProtonTrack();
        const H1SelVertex* ProtonPrimaryVertex = ProtonVertex->GetPrimaryVertex();
        TVector3 pv = ProtonPrimaryVertex->GetPosition();//is same for both protons and pion daughters, so just using proton here and previous couple lines
        TVector2 V2PV(pv.X(), pv.Y());

        ///////////////////////////////////////////////////////////////////////////////
        //Defining pT before cuts and putting stuff into pt bins
        const H1CentralFittedV0* v0 = LambdaCand[loop]->GetV0();
        Double_t pT = v0->GetPt();

        //Raw Lambda Candidate data after our event selection defined in create_evls.C
        hLambda->Fill(invMasslambda);
        hLambdapTbefore->Fill(pT);
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
        else if (pT >= ptbin10l && pT < ptbin10u) {
            h10Lambda->Fill(invMasslambda);
        }
        else if (pT >= ptbin11l && pT < ptbin11u) {
            h11Lambda->Fill(invMasslambda);
        }
        else if (pT >= ptbin12l && pT < ptbin12u) {
            h12Lambda->Fill(invMasslambda);
        }
        else if (pT >= ptbin13l && pT < ptbin13u) {
            h13Lambda->Fill(invMasslambda);
        }
        else if (pT >= ptbin14l && pT < ptbin14u) {
            h14Lambda->Fill(invMasslambda);
        }
        else if (pT >= ptbin15l && pT < ptbin15u) {
            h15Lambda->Fill(invMasslambda);
        }
        else if (pT >= ptbin16l && pT < ptbin16u) {
            h16Lambda->Fill(invMasslambda);
        }
        else if (pT >= ptbin17l && pT < ptbin17u) {
            h17Lambda->Fill(invMasslambda);
        }
        else if (pT >= ptbin18l && pT < ptbin18u) {
            h18Lambda->Fill(invMasslambda);
        }
        else if (pT >= ptbin19l && pT < ptbin19u) {
            h19Lambda->Fill(invMasslambda);
        }
        else if (pT >= ptbin20l && pT < ptbin20u) {
            h20Lambda->Fill(invMasslambda);
        }
        
        //PID Likelihood Cut for Proton < 0.003
        if (ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton) < 0.003) continue;
        hLambdaPIDproton->Fill(invMasslambda);
        
        //PID Likelihood Cut for Pion < 0.003
        if (PionPSTrack->GetDedxLikelihood(H1Dedx::kPion) < 0.003) continue;
        hLambdaPIDpion->Fill(invMasslambda);
        
        //DCA over dDCA proton cut
        if (abs((protonTrajectory->GetDca(V2PV)) / (protonTrajectory->GetdDca(V2PV))) < 2) continue;
        hLambdaDCAoverdDCAproton->Fill(invMasslambda);
        
        //DCA over dDCA pion cut
        if (abs((pionTrajectory->GetDca(V2PV)) / (pionTrajectory->GetdDca(V2PV))) < 1) continue;
        hLambdaDCAoverdDCApion->Fill(invMasslambda);
        
       
        //Cut on Track Length of proton and pion
        if (ProtonCFTrack->GetLength() < 15 || PionCFTrack->GetLength() < 15) continue;
        hLambdatracklength->Fill(invMasslambda);
        
        //Cut against K0 contamination
        Double_t invMasskaonCont = (ProtonH1Track->GetFourVector(mPion)
            + PionH1Track->GetFourVector(mPion)).M();
        if (invMasskaonCont > 0.465 && invMasskaonCont < 0.53) continue;
        hLambdaKaonCont->Fill(invMasslambda);
        
        //Cut against photon contamination
        Double_t invMassphotonCont = (ProtonH1Track->GetFourVector(mElectron)
            + PionH1Track->GetFourVector(mElectron)).M();
        if (invMassphotonCont < 0.05) continue;
        hLambdaPhotonCont->Fill(invMasslambda);
        hLambdapTafter->Fill(pT);
        
        //After All necessary cuts, now can fill different pt bins
        if (pT >= ptbin1l && pT < ptbin1u) {
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
        else if (pT >= ptbin10l && pT < ptbin10u) {
            hLambda10->Fill(invMasslambda);
        }
        else if (pT >= ptbin11l && pT < ptbin11u) {
            hLambda11->Fill(invMasslambda);
        }
        else if (pT >= ptbin12l && pT < ptbin12u) {
            hLambda12->Fill(invMasslambda);
        }
        else if (pT >= ptbin13l && pT < ptbin13u) {
            hLambda13->Fill(invMasslambda);
        }
        else if (pT >= ptbin14l && pT < ptbin14u) {
            hLambda14->Fill(invMasslambda);
        }
        else if (pT >= ptbin15l && pT < ptbin15u) {
            hLambda15->Fill(invMasslambda);
        }
        else if (pT >= ptbin16l && pT < ptbin16u) {
            hLambda16->Fill(invMasslambda);
        }
        else if (pT >= ptbin17l && pT < ptbin17u) {
            hLambda17->Fill(invMasslambda);
        }
        else if (pT >= ptbin18l && pT < ptbin18u) {
            hLambda18->Fill(invMasslambda);
        }
        else if (pT >= ptbin19l && pT < ptbin19u) {
            hLambda19->Fill(invMasslambda);
        }
        else if (pT >= ptbin20l && pT < ptbin20u) {
            hLambda20->Fill(invMasslambda);
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
       h10Lambda->SetLineColor(kBlack);
       h11Lambda->SetLineColor(kBlack);
       h12Lambda->SetLineColor(kBlack);
       h13Lambda->SetLineColor(kBlack);
       h14Lambda->SetLineColor(kBlack);
       h15Lambda->SetLineColor(kBlack);
       h16Lambda->SetLineColor(kBlack);
       h17Lambda->SetLineColor(kBlack);
       h18Lambda->SetLineColor(kBlack);
       h19Lambda->SetLineColor(kBlack);
       h20Lambda->SetLineColor(kBlack);
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

       hLambda10->SetStats(0);
       hLambda10->SetLineColor(kBlack);
       hLambda10->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda10->GetYaxis()->SetTitle("Count");
       hLambda10->Draw();
       hLambda10->Write();

       hLambda11->SetStats(0);
       hLambda11->SetLineColor(kBlack);
       hLambda11->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda11->GetYaxis()->SetTitle("Count");
       hLambda11->Draw();
       hLambda11->Write();

       hLambda12->SetStats(0);
       hLambda12->SetLineColor(kBlack);
       hLambda12->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda12->GetYaxis()->SetTitle("Count");
       hLambda12->Draw();
       hLambda12->Write();

       hLambda13->SetStats(0);
       hLambda13->SetLineColor(kBlack);
       hLambda13->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda13->GetYaxis()->SetTitle("Count");
       hLambda13->Draw();
       hLambda13->Write();

       hLambda14->SetStats(0);
       hLambda14->SetLineColor(kBlack);
       hLambda14->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda14->GetYaxis()->SetTitle("Count");
       hLambda14->Draw();
       hLambda14->Write();

       hLambda15->SetStats(0);
       hLambda15->SetLineColor(kBlack);
       hLambda15->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda15->GetYaxis()->SetTitle("Count");
       hLambda15->Draw();
       hLambda15->Write();

       hLambda16->SetStats(0);
       hLambda16->SetLineColor(kBlack);
       hLambda16->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda16->GetYaxis()->SetTitle("Count");
       hLambda16->Draw();
       hLambda16->Write();

       hLambda17->SetStats(0);
       hLambda17->SetLineColor(kBlack);
       hLambda17->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda17->GetYaxis()->SetTitle("Count");
       hLambda17->Draw();
       hLambda17->Write();

       hLambda18->SetStats(0);
       hLambda18->SetLineColor(kBlack);
       hLambda18->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda18->GetYaxis()->SetTitle("Count");
       hLambda18->Draw();
       hLambda18->Write();

       hLambda19->SetStats(0);
       hLambda19->SetLineColor(kBlack);
       hLambda19->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda19->GetYaxis()->SetTitle("Count");
       hLambda19->Draw();
       hLambda19->Write();

       hLambda20->SetStats(0);
       hLambda20->SetLineColor(kBlack);
       hLambda20->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda20->GetYaxis()->SetTitle("Count");
       hLambda20->Draw();
       hLambda20->Write();

       hLambdapTbefore->SetStats(0);
       hLambdapTbefore->SetLineColor(kBlack);
       hLambdapTbefore->GetXaxis()->SetTitle("pT [Gev]");
       hLambdapTbefore->GetYaxis()->SetTitle("Count");
       hLambdapTbefore->Draw();
       hLambdapTbefore->Write();

       hLambdapTafter->SetStats(0);
       hLambdapTafter->SetLineColor(kBlack);
       hLambdapTafter->GetXaxis()->SetTitle("pT [Gev]");
       hLambdapTafter->GetYaxis()->SetTitle("Count");
       hLambdapTafter->Draw();
       hLambdapTafter->Write();

       hLambda20->SetStats(0);
       hLambda20->SetLineColor(kBlack);
       hLambda20->GetXaxis()->SetTitle("Mass [Gev]");
       hLambda20->GetYaxis()->SetTitle("Count");
       hLambda20->Draw();
       hLambda20->Write();

       h1Lambda->SetStats(0);
       h1Lambda->SetLineColor(kBlack);
       h1Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h1Lambda->GetYaxis()->SetTitle("Count");
       h1Lambda->Draw();
       h1Lambda->Write();

       h2Lambda->SetStats(0);
       h2Lambda->SetLineColor(kBlack);
       h2Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h2Lambda->GetYaxis()->SetTitle("Count");
       h2Lambda->Draw();
       h2Lambda->Write();

       h3Lambda->SetStats(0);
       h3Lambda->SetLineColor(kBlack);
       h3Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h3Lambda->GetYaxis()->SetTitle("Count");
       h3Lambda->Draw();
       h3Lambda->Write();

       h4Lambda->SetStats(0);
       h4Lambda->SetLineColor(kBlack);
       h4Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h4Lambda->GetYaxis()->SetTitle("Count");
       h4Lambda->Draw();
       h4Lambda->Write();

       h5Lambda->SetStats(0);
       h5Lambda->SetLineColor(kBlack);
       h5Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h5Lambda->GetYaxis()->SetTitle("Count");
       h5Lambda->Draw();
       h5Lambda->Write();

       h6Lambda->SetStats(0);
       h6Lambda->SetLineColor(kBlack);
       h6Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h6Lambda->GetYaxis()->SetTitle("Count");
       h6Lambda->Draw();
       h6Lambda->Write();

       h7Lambda->SetStats(0);
       h7Lambda->SetLineColor(kBlack);
       h7Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h7Lambda->GetYaxis()->SetTitle("Count");
       h7Lambda->Draw();
       h7Lambda->Write();

       h8Lambda->SetStats(0);
       h8Lambda->SetLineColor(kBlack);
       h8Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h8Lambda->GetYaxis()->SetTitle("Count");
       h8Lambda->Draw();
       h8Lambda->Write();

       h9Lambda->SetStats(0);
       h9Lambda->SetLineColor(kBlack);
       h9Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h9Lambda->GetYaxis()->SetTitle("Count");
       h9Lambda->Draw();
       h9Lambda->Write();

       h10Lambda->SetStats(0);
       h10Lambda->SetLineColor(kBlack);
       h10Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h10Lambda->GetYaxis()->SetTitle("Count");
       h10Lambda->Draw();
       h10Lambda->Write();

       h11Lambda->SetStats(0);
       h11Lambda->SetLineColor(kBlack);
       h11Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h11Lambda->GetYaxis()->SetTitle("Count");
       h11Lambda->Draw();
       h11Lambda->Write();

       h12Lambda->SetStats(0);
       h12Lambda->SetLineColor(kBlack);
       h12Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h12Lambda->GetYaxis()->SetTitle("Count");
       h12Lambda->Draw();
       h12Lambda->Write();

       h13Lambda->SetStats(0);
       h13Lambda->SetLineColor(kBlack);
       h13Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h13Lambda->GetYaxis()->SetTitle("Count");
       h13Lambda->Draw();
       h13Lambda->Write();

       h14Lambda->SetStats(0);
       h14Lambda->SetLineColor(kBlack);
       h14Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h14Lambda->GetYaxis()->SetTitle("Count");
       h14Lambda->Draw();
       h14Lambda->Write();

       h15Lambda->SetStats(0);
       h15Lambda->SetLineColor(kBlack);
       h15Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h15Lambda->GetYaxis()->SetTitle("Count");
       h15Lambda->Draw();
       h15Lambda->Write();

       h16Lambda->SetStats(0);
       h16Lambda->SetLineColor(kBlack);
       h16Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h16Lambda->GetYaxis()->SetTitle("Count");
       h16Lambda->Draw();
       h16Lambda->Write();

       h17Lambda->SetStats(0);
       h17Lambda->SetLineColor(kBlack);
       h17Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h17Lambda->GetYaxis()->SetTitle("Count");
       h17Lambda->Draw();
       h17Lambda->Write();

       h18Lambda->SetStats(0);
       h18Lambda->SetLineColor(kBlack);
       h18Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h18Lambda->GetYaxis()->SetTitle("Count");
       h18Lambda->Draw();
       h18Lambda->Write();

       h19Lambda->SetStats(0);
       h19Lambda->SetLineColor(kBlack);
       h19Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h19Lambda->GetYaxis()->SetTitle("Count");
       h19Lambda->Draw();
       h19Lambda->Write();

       h20Lambda->SetStats(0);
       h20Lambda->SetLineColor(kBlack);
       h20Lambda->GetXaxis()->SetTitle("Mass [Gev]");
       h20Lambda->GetYaxis()->SetTitle("Count");
       h20Lambda->Draw();
       h20Lambda->Write();

       hAPbefore->SetStats(0);
       hAPbefore->GetXaxis()->SetTitle("\\alpha = (p^{+}_{L} - p^{-}_{L})/(p^{+}_{L} + p^{-}_{L})");
       hAPbefore->GetYaxis()->SetTitle("p_{T} [GeV]");
       hAPbefore->GetXaxis()->SetTitleSize(0.04);
       hAPbefore->GetXaxis()->SetTitleOffset(1);
       hAPbefore->GetYaxis()->SetTitleSize(0.05);
       hAPbefore->Draw("colz");
       hAPbefore->Write();
       gPad->Update();
       canvas->SaveAs("APbefore.root");

       hAPafter->SetStats(0);
       hAPafter->GetXaxis()->SetTitle("\\alpha = (p^{+}_{L} - p^{-}_{L})/(p^{+}_{L} + p^{-}_{L})");
       hAPafter->GetYaxis()->SetTitle("p_{T} [GeV]");
       hAPafter->GetXaxis()->SetTitleSize(0.04);
       hAPafter->GetXaxis()->SetTitleOffset(1);
       hAPafter->GetYaxis()->SetTitleSize(0.05);
       hAPafter->Draw("colz");
       hAPafter->Write();
       gPad->Update();
       canvas->SaveAs("APafter.root");

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

       hdEdxBothBefore->SetStats(0);
       hdEdxBothBefore->GetXaxis()->SetTitle("p [GeV]");
       hdEdxBothBefore->GetYaxis()->SetTitle("dE/dx");
       hdEdxBothBefore->Draw("colz");
       hdEdxBothBefore->Write();
       gPad->Update();
       canvas->SaveAs("dEdxBothBefore.root");

       hdEdxProtonBefore->SetStats(0);
       hdEdxProtonBefore->GetXaxis()->SetTitle("p [GeV]");
       hdEdxProtonBefore->GetYaxis()->SetTitle("dE/dx");
       hdEdxProtonBefore->Draw("colz");
       hdEdxProtonBefore->Write();
       gPad->Update();
       canvas->SaveAs("dEdxProtonBefore.root");

       hdEdxPionBefore->SetStats(0);
       hdEdxPionBefore->GetXaxis()->SetTitle("p [GeV]");
       hdEdxPionBefore->GetYaxis()->SetTitle("dE/dx");
       hdEdxPionBefore->Draw("colz");
       hdEdxPionBefore->Write();
       gPad->Update();
       canvas->SaveAs("dEdxPionBefore.root");
       
       hdEdxBothAfterLcut->SetStats(0);
       hdEdxBothAfterLcut->GetXaxis()->SetTitle("p [GeV]");
       hdEdxBothAfterLcut->GetYaxis()->SetTitle("dE/dx");
       hdEdxBothAfterLcut->Draw("colz");
       hdEdxBothAfterLcut->Write();
       gPad->Update();
       canvas->SaveAs("dEdxBothAfterLcut.root");

       hdEdxProtonAfterLcut->SetStats(0);
       hdEdxProtonAfterLcut->GetXaxis()->SetTitle("p [GeV]");
       hdEdxProtonAfterLcut->GetYaxis()->SetTitle("dE/dx");
       hdEdxProtonAfterLcut->Draw("colz");
       hdEdxProtonAfterLcut->Write();
       gPad->Update();
       canvas->SaveAs("dEdxProtonAfterLcut.root");

       hdEdxPionAfterLcut->SetStats(0);
       hdEdxPionAfterLcut->GetXaxis()->SetTitle("p [GeV]");
       hdEdxPionAfterLcut->GetYaxis()->SetTitle("dE/dx");
       hdEdxPionAfterLcut->Draw("colz");
       hdEdxPionAfterLcut->Write();
       gPad->Update();
       canvas->SaveAs("dEdxPionAfterLcut.root");

       hdEdxBothAfterAllcuts->SetStats(0);
       hdEdxBothAfterAllcuts->GetXaxis()->SetTitle("p [GeV]");
       hdEdxBothAfterAllcuts->GetYaxis()->SetTitle("dE/dx");
       hdEdxBothAfterAllcuts->Draw("colz");
       hdEdxBothAfterAllcuts->Write();
       gPad->Update();
       canvas->SaveAs("dEdxBothAfterAllcuts.root");

       hdEdxProtonAfterAllcuts->SetStats(0);
       hdEdxProtonAfterAllcuts->GetXaxis()->SetTitle("p [GeV]");
       hdEdxProtonAfterAllcuts->GetYaxis()->SetTitle("dE/dx");
       hdEdxProtonAfterAllcuts->Draw("colz");
       hdEdxProtonAfterAllcuts->Write();
       gPad->Update();
       canvas->SaveAs("dEdxProtonAfterAllcuts.root");

       hdEdxPionAfterAllcuts->SetStats(0);
       hdEdxPionAfterAllcuts->GetXaxis()->SetTitle("p [GeV]");
       hdEdxPionAfterAllcuts->GetYaxis()->SetTitle("dE/dx");
       hdEdxPionAfterAllcuts->Draw("colz");
       hdEdxPionAfterAllcuts->Write();
       gPad->Update();
       canvas->SaveAs("dEdxPionAfterAllcuts.root");

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
       canvas->SaveAs("stack1.root");

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
       canvas->SaveAs("stack2.root");

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
       canvas->SaveAs("stack3.root");

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
       canvas->SaveAs("stack4.root");

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
       canvas->SaveAs("stack5.root");

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
       canvas->SaveAs("stack6.root");

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
       canvas->SaveAs("stack7.root");

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
       canvas->SaveAs("stack8.root");

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
       canvas->SaveAs("stack9.root");

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
       canvas->SaveAs("stack.root");

       file.Write();
       file.Close();
       cout << "Histograms written to " << opts.GetOutput() << endl;


        return 0;
    }