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
#include "H1Mods/H1PartK0ArrayPtr.h"
#include "H1Mods/H1PartK0.h"
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
    TH1F* hKaon1 = new TH1F("Kaon1", "Kaon pT Bin 1", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon2 = new TH1F("Kaon2", "Kaon pT Bin 2", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon3 = new TH1F("Kaon3", "Kaon pT Bin 3", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon4 = new TH1F("Kaon4", "Kaon pT Bin 4", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon5 = new TH1F("Kaon5", "Kaon pT Bin 5", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon6 = new TH1F("Kaon6", "Kaon pT Bin 6", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon7 = new TH1F("Kaon7", "Kaon pT Bin 7", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon8 = new TH1F("Kaon8", "Kaon pT Bin 8", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon9 = new TH1F("Kaon9", "Kaon pT Bin 9", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon10 = new TH1F("Kaon10", "Kaon pT Bin 10", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon11 = new TH1F("Kaon11", "Kaon pT Bin 11", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon12 = new TH1F("Kaon12", "Kaon pT Bin 12", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon13 = new TH1F("Kaon13", "Kaon pT Bin 13", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon14 = new TH1F("Kaon14", "Kaon pT Bin 14", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon15 = new TH1F("Kaon15", "Kaon pT Bin 15", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon16 = new TH1F("Kaon16", "Kaon pT Bin 16", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon17 = new TH1F("Kaon17", "Kaon pT Bin 17", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon18 = new TH1F("Kaon18", "Kaon pT Bin 18", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon19 = new TH1F("Kaon19", "Kaon pT Bin 19", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaon20 = new TH1F("Kaon20", "Kaon pT Bin 20", kaonbins, kaonlowerbinrange, kaonupperbinrange);



    //Stack to plot multiple histograms, want total and also for each pt bin
    THStack* hs = new THStack("hs", "Stacked K0 Mass Plots");
    THStack* hs1 = new THStack("hs1", "Stacked K0 Mass Plots pT Bin 1");
    THStack* hs2 = new THStack("hs2", "Stacked K0 Mass Plots pT Bin 2");
    THStack* hs3 = new THStack("hs3", "Stacked K0 Mass Plots pT Bin 3");
    THStack* hs4 = new THStack("hs4", "Stacked K0 Mass Plots pT Bin 4");
    THStack* hs5 = new THStack("hs5", "Stacked K0 Mass Plots pT Bin 5");
    THStack* hs6 = new THStack("hs6", "Stacked K0 Mass Plots pT Bin 6");
    THStack* hs7 = new THStack("hs7", "Stacked K0 Mass Plots pT Bin 7");
    THStack* hs8 = new THStack("hs8", "Stacked K0 Mass Plots pT Bin 8");
    THStack* hs9 = new THStack("hs9", "Stacked K0 Mass Plots pT Bin 9");
    //Total Lambda plot, diff hLambdas for different cuts, want this also for each bin, so 9 copies of these as well for 9 pt bins
    TH1F* hK0 = new TH1F("K0", "K0", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hKaonpTbefore = new TH1F("Kaon pT", "Kaon pT", 1000, 0, 25);
    TH1F* hKaonpTafter = new TH1F("Kaon pT", "Kaon pT", 1000, 0, 25);
    TH1F* hK0PIDproton = new TH1F("K0 PID Proton", "K0 PID Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK0PIDpion = new TH1F("K0 PID Pion", "K0 PID Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK0Alpha = new TH1F("K0 Alpha", "K0 Alpha", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK0ptrel = new TH1F("K0 pTrel", "K0 pT_rel", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK0pt = new TH1F("K0 pT", "K0 pT", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK0Eta = new TH1F("K0 Psuedorapidity", "K0 Psuedorapidity", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK0tracklength = new TH1F("K0 Track Length", "K0 Track Length", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK0DCAoverdDCAproton = new TH1F("K0 dDCA Proton", "K0 dDCA Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK0DCAoverdDCApion = new TH1F("K0 dDCA Pion", "K0 dDCA Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK0LambdaCont = new TH1F("K0-Lambdas", "K0 Lambda Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    //TH1F* hK0PhotonCont = new TH1F("K0-Photons", "K0 Photon Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);

    TH1F* h1K0 = new TH1F("K01", "K01", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h1K0PIDproton = new TH1F("K01 PID Proton", "K01 PID Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h1K0PIDpion = new TH1F("K01 PID Pion", "K01 PID Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h1K0Alpha = new TH1F("K01 Alpha", "K01 Alpha", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h1K0ptrel = new TH1F("K01 pTrel", "K01 pT_rel", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h1K0pt = new TH1F("K01 pT", "K01 pT", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h1K0Eta = new TH1F("K01 Psuedorapidity", "K01 Psuedorapidity", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h1K0tracklength = new TH1F("K01 Track Length", "K01 Track Length", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h1K0DCAoverdDCAproton = new TH1F("K01 dDCA Proton", "K01 dDCA Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h1K0DCAoverdDCApion = new TH1F("K01 dDCA Pion", "K01 dDCA Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h1K0LambdaCont = new TH1F("K0-Lambdas", "K01 Lambda Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    //TH1F* h1K0PhotonCont = new TH1F("K0-Photons1", "K01 Photon Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);

    TH1F* h2K0 = new TH1F("K02", "K02", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h2K0PIDproton = new TH1F("K01 PID Proton", "K01 PID Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h2K0PIDpion = new TH1F("K01 PID Pion", "K01 PID Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h2K0Alpha = new TH1F("K01 Alpha", "K01 Alpha", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h2K0ptrel = new TH1F("K01 pTrel", "K01 pT_rel", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h2K0pt = new TH1F("K01 pT", "K01 pT", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h2K0Eta = new TH1F("K01 Psuedorapidity", "K01 Psuedorapidity", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h2K0tracklength = new TH1F("K01 Track Length", "K01 Track Length", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h2K0DCAoverdDCAproton = new TH1F("K01 dDCA Proton", "K01 dDCA Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h2K0DCAoverdDCApion = new TH1F("K01 dDCA Pion", "K01 dDCA Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h2K0LambdaCont = new TH1F("K0-Lambdas2", "Lambda2 Lambda Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    //TH1F* h2LambdaPhotonCont = new TH1F("Lambda-Photons1", "Lambda1 Photon Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);

    TH1F* h3K0 = new TH1F("K03", "K03", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h3K0PIDproton = new TH1F("K01 PID Proton", "K01 PID Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h3K0PIDpion = new TH1F("K01 PID Pion", "K01 PID Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h3K0Alpha = new TH1F("K01 Alpha", "K01 Alpha", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h3K0ptrel = new TH1F("K01 pTrel", "K01 pT_rel", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h3K0pt = new TH1F("K01 pT", "K01 pT", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h3K0Eta = new TH1F("K01 Psuedorapidity", "K01 Psuedorapidity", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h3K0tracklength = new TH1F("K01 Track Length", "K01 Track Length", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h3K0DCAoverdDCAproton = new TH1F("K01 dDCA Proton", "K01 dDCA Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h3K0DCAoverdDCApion = new TH1F("K01 dDCA Pion", "K01 dDCA Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h3K0LambdaCont = new TH1F("K0-Lambdas3", "Lambda3 Lambda Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    //TH1F* h3LambdaPhotonCont = new TH1F("Lambda-Photons1", "Lambda1 Photon Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);

    TH1F* h4K0 = new TH1F("K04", "K04", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h4K0PIDproton = new TH1F("K01 PID Proton", "K01 PID Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h4K0PIDpion = new TH1F("K01 PID Pion", "K01 PID Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h4K0Alpha = new TH1F("K01 Alpha", "K01 Alpha", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h4K0ptrel = new TH1F("K01 pTrel", "K01 pT_rel", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h4K0pt = new TH1F("K01 pT", "K01 pT", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h4K0Eta = new TH1F("K01 Psuedorapidity", "K01 Psuedorapidity", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h4K0tracklength = new TH1F("K01 Track Length", "K01 Track Length", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h4K0DCAoverdDCAproton = new TH1F("K01 dDCA Proton", "K01 dDCA Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h4K0DCAoverdDCApion = new TH1F("K01 dDCA Pion", "K01 dDCA Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h4K0LambdaCont = new TH1F("K0-Lambdas4", "Lambda4 Lambda Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    //TH1F* h4LambdaPhotonCont = new TH1F("Lambda-Photons1", "Lambda1 Photon Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);

    TH1F* h5K0 = new TH1F("K05", "K05", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h5K0PIDproton = new TH1F("K01 PID Proton", "K01 PID Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h5K0PIDpion = new TH1F("K01 PID Pion", "K01 PID Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h5K0Alpha = new TH1F("K01 Alpha", "K01 Alpha", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h5K0ptrel = new TH1F("K01 pTrel", "K01 pT_rel", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h5K0pt = new TH1F("K01 pT", "K01 pT", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h5K0Eta = new TH1F("K01 Psuedorapidity", "K01 Psuedorapidity", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h5K0tracklength = new TH1F("K01 Track Length", "K01 Track Length", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h5K0DCAoverdDCAproton = new TH1F("K01 dDCA Proton", "K01 dDCA Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h5K0DCAoverdDCApion = new TH1F("K01 dDCA Pion", "K01 dDCA Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h5K0LambdaCont = new TH1F("K0-Lambdas5", "Lambda5 Lambda Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    //TH1F* h5LambdaPhotonCont = new TH1F("Lambda-Photons1", "Lambda1 Photon Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);

    TH1F* h6K0 = new TH1F("K06", "K06", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h6K0PIDproton = new TH1F("K01 PID Proton", "K01 PID Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h6K0PIDpion = new TH1F("K01 PID Pion", "K01 PID Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h6K0Alpha = new TH1F("K01 Alpha", "K01 Alpha", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h6K0ptrel = new TH1F("K01 pTrel", "K01 pT_rel", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h6K0pt = new TH1F("K01 pT", "K01 pT", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h6K0Eta = new TH1F("K01 Psuedorapidity", "K01 Psuedorapidity", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h6K0tracklength = new TH1F("K01 Track Length", "K01 Track Length", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h6K0DCAoverdDCAproton = new TH1F("K01 dDCA Proton", "K01 dDCA Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h6K0DCAoverdDCApion = new TH1F("K01 dDCA Pion", "K01 dDCA Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h6K0LambdaCont = new TH1F("K0-Lambdas6", "Lambda6 Lambda Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    //TH1F* h6LambdaPhotonCont = new TH1F("Lambda-Photons1", "Lambda1 Photon Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);

    TH1F* h7K0 = new TH1F("K07", "K07", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h7K0PIDproton = new TH1F("K01 PID Proton", "K01 PID Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h7K0PIDpion = new TH1F("K01 PID Pion", "K01 PID Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h7K0Alpha = new TH1F("K01 Alpha", "K01 Alpha", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h7K0ptrel = new TH1F("K01 pTrel", "K01 pT_rel", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h7K0pt = new TH1F("K01 pT", "K01 pT", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h7K0Eta = new TH1F("K01 Psuedorapidity", "K01 Psuedorapidity", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h7K0tracklength = new TH1F("K01 Track Length", "K01 Track Length", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h7K0DCAoverdDCAproton = new TH1F("K01 dDCA Proton", "K01 dDCA Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h7K0DCAoverdDCApion = new TH1F("K01 dDCA Pion", "K01 dDCA Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h7K0LambdaCont = new TH1F("K0-Lambdas7", "Lambda7 Lambda Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    //TH1F* h7LambdaPhotonCont = new TH1F("Lambda-Photons1", "Lambda1 Photon Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);

    TH1F* h8K0 = new TH1F("K08", "K08", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h8K0PIDproton = new TH1F("K01 PID Proton", "K01 PID Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h8K0PIDpion = new TH1F("K01 PID Pion", "K01 PID Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h8K0Alpha = new TH1F("K01 Alpha", "K01 Alpha", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h8K0ptrel = new TH1F("K01 pTrel", "K01 pT_rel", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h8K0pt = new TH1F("K01 pT", "K01 pT", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h8K0Eta = new TH1F("K01 Psuedorapidity", "K01 Psuedorapidity", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h8K0tracklength = new TH1F("K01 Track Length", "K01 Track Length", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h8K0DCAoverdDCAproton = new TH1F("K01 dDCA Proton", "K01 dDCA Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h8K0DCAoverdDCApion = new TH1F("K01 dDCA Pion", "K01 dDCA Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h8K0LambdaCont = new TH1F("K0-Lambdas8", "Lambda8 Lambda Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    //TH1F* h8LambdaPhotonCont = new TH1F("Lambda-Photons1", "Lambda1 Photon Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);

    TH1F* h9K0 = new TH1F("K09", "K09", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h10K0 = new TH1F("K010", "K010", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h11K0 = new TH1F("K011", "K011", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h12K0 = new TH1F("K012", "K012", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h13K0 = new TH1F("K013", "K013", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h14K0 = new TH1F("K014", "K014", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h15K0 = new TH1F("K015", "K015", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h16K0 = new TH1F("K016", "K016", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h17K0 = new TH1F("K017", "K017", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h18K0 = new TH1F("K018", "K018", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h19K0 = new TH1F("K019", "K019", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h20K0 = new TH1F("K020", "K020", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h9K0PIDproton = new TH1F("K01 PID Proton", "K01 PID Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h9K0PIDpion = new TH1F("K01 PID Pion", "K01 PID Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h9K0Alpha = new TH1F("K01 Alpha", "K01 Alpha", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h9K0ptrel = new TH1F("K01 pTrel", "K01 pT_rel", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h9K0pt = new TH1F("K01 pT", "K01 pT", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h9K0Eta = new TH1F("K01 Psuedorapidity", "K01 Psuedorapidity", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h9K0tracklength = new TH1F("K01 Track Length", "K01 Track Length", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h9K0DCAoverdDCAproton = new TH1F("K01 dDCA Proton", "K01 dDCA Proton", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h9K0DCAoverdDCApion = new TH1F("K01 dDCA Pion", "K01 dDCA Pion", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* h9K0LambdaCont = new TH1F("K0-Lambdas9", "Lambda9 Lambda Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    //TH1F* h9LambdaPhotonCont = new TH1F("Lambda-Photons1", "Lambda1 Photon Contamination Cut", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    
    //K0s in pt bins
    TH1F* hK01 = new TH1F("K01", "K0 pT Bin 1", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK02 = new TH1F("K02", "K0 pT Bin 2", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK03 = new TH1F("K03", "K0 pT Bin 3", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK04 = new TH1F("K04", "K0 pT Bin 4", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK05 = new TH1F("K05", "K0 pT Bin 5", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK06 = new TH1F("K06", "K0 pT Bin 6", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK07 = new TH1F("K07", "K0 pT Bin 7", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK08 = new TH1F("K08", "K0 pT Bin 8", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK09 = new TH1F("K09", "K0 pT Bin 9", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK010 = new TH1F("K010", "K0 pT Bin 10", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK011 = new TH1F("K011", "K0 pT Bin 11", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK012 = new TH1F("K012", "K0 pT Bin 12", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK013 = new TH1F("K013", "K0 pT Bin 13", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK014 = new TH1F("K014", "K0 pT Bin 14", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK015 = new TH1F("K015", "K0 pT Bin 15", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK016 = new TH1F("K016", "K0 pT Bin 16", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK017 = new TH1F("K017", "K0 pT Bin 17", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK018 = new TH1F("K018", "K0 pT Bin 18", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK019 = new TH1F("K019", "K0 pT Bin 19", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hK020 = new TH1F("K020", "K0 pT Bin 20", kaonbins, kaonlowerbinrange, kaonupperbinrange);

    //Armenteros plot for particle ID lambdas vs kaons
    TH2F* hAPbefore = new TH2F("AP", "Armenteros-Podolansky Plot (After H1FindK0)", 200, -1, 1, 200, 0, 0.3);
    TH2F* hAPafter = new TH2F("AP", "Armenteros-Podolansky Plot (After All Cuts)", 200, -1, 1, 200, 0, 0.3);

    //dEdx plot for particle ID protons vs pions
    TH2D* hdEdxBothBefore = new TH2D("dEdxvP Initial (After H1FindK0)", "dE/dx Initial (After H1FindK0)", 500, 0, 2, 500, 0, 10);
    TH2D* hdEdxBothAfterLcut = new TH2D("dEdxvP After Likelihood Cut", "dE/dx After Likelihood Cut", 500, 0, 2, 500, 0, 10);
    TH2D* hdEdxBothAfterAllcuts = new TH2D("dEdxvP After All Cuts", "dE/dx After All Cuts", 500, 0, 2, 500, 0, 10);
   
    //DCA/dDCA histograms
    TH1F* hdDCApion = new TH1F("Pion dDCA", "Pion dDCA", 1000, 0, 10);
    TH1F* hDCApion = new TH1F("Pion DCA", "Pion DCA", 1000, -10, 10);
    TH1F* hDCAoverdDCApion = new TH1F("Pion DCA/dDCA", "Pion DCA/dDCA", 1000, -10, 10);
    //Likelihood histograms
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
    static H1PartK0ArrayPtr K0Cand;
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
        for (Int_t loop = 0; loop < K0Cand.GetEntries(); loop++) {

        if (K0Cand[loop]->GetNumOfParticles() < 2) continue;

        Double_t invMasskaon = (K0Cand[loop]->GetPosPionFourVector()
            + K0Cand[loop]->GetNegPionFourVector()).M();

        //Getting Vectors for Armenteros-Podolansky Plot
        H1Track* PosPionH1Track = (H1Track*)K0Cand[loop]->GetPosPionTrack();
        H1Track* NegPionH1Track = (H1Track*)K0Cand[loop]->GetNegPionTrack();

        TVector3 vecPosPion = PosPionH1Track->GetFourVector(mPion).Vect();
        TVector3 vecNegPion = NegPionH1Track->GetFourVector(mPion).Vect();
        TVector3 vecK0 = vecPosPion + vecNegPion;

        Double_t qlPosPion = vecK0.Dot(vecPosPion) / vecK0.Mag();
        Double_t qlNegPion = vecK0.Dot(vecNegPion) / vecK0.Mag();
        Double_t pTrel = vecK0.Cross(vecNegPion).Mag() / vecK0.Mag();
        Double_t alpha = (qlPosPion - qlNegPion) / (qlPosPion + qlNegPion);

        hAPbefore->Fill(alpha,pTrel);
        
        //Call H1PartLambda class data that references H1CentralFittedTrack for Pion and Proton
        const H1CentralFittedTrack* PosPionCFTrack = K0Cand[loop]->GetPosPionTrack();
        const H1CentralFittedTrack* NegPionCFTrack = K0Cand[loop]->GetNegPionTrack();


        hdEdxBothBefore->Fill(PosPionCFTrack->GetMomentum().Mag(), PosPionCFTrack->GetCorrectedDedx());
        hdEdxBothBefore->Fill(NegPionCFTrack->GetMomentum().Mag(), NegPionCFTrack->GetCorrectedDedx());

        //Getting H1PartSelTracks for daughters
        const H1PartSelTrack* PosPionPSTrack = K0Cand[loop]->GetPosPion();
        const H1PartSelTrack* NegPionPSTrack = K0Cand[loop]->GetNegPion();

        //Plotting Likelihood to help visualize likelihood cut
        hDedxLpion->Fill(PosPionPSTrack->GetDedxLikelihood(H1Dedx::kPion));
        hDedxLpion->Fill(NegPionPSTrack->GetDedxLikelihood(H1Dedx::kPion));
        //////////////////////////////////////////////////////////////////
        
        //Prepating DCA/dDCA information to get a histogram to help visualize this cut
        const H1CentralTrack* pospionFittedNoV = PosPionCFTrack->GetCentralTrack();
        const H1Trajectory* pospionTrajectory = pospionFittedNoV->GetTrajectory();

        const H1CentralTrack* negpionFittedNoV = NegPionCFTrack->GetCentralTrack();
        const H1Trajectory* negpionTrajectory = negpionFittedNoV->GetTrajectory();

        H1SelVertex* PosPionVertex = (H1SelVertex*)K0Cand[loop]->GetPosPionTrack();
        const H1SelVertex* PosPionPrimaryVertex = PosPionVertex->GetPrimaryVertex();
        TVector3 pv = PosPionPrimaryVertex->GetPosition();//is same for both pions, so just using pos one here and previous couple lines
        TVector2 V2PV(pv.X(), pv.Y());
        hdDCApion->Fill((pospionTrajectory->GetdDca(V2PV)));
        hdDCApion->Fill((negpionTrajectory->GetdDca(V2PV)));
        hDCApion->Fill((pospionTrajectory->GetDca(V2PV)));
        hDCApion->Fill((negpionTrajectory->GetDca(V2PV)));
        hDCAoverdDCApion->Fill((pospionTrajectory->GetDca(V2PV))/(pospionTrajectory->GetdDca(V2PV)));
        hDCAoverdDCApion->Fill((negpionTrajectory->GetDca(V2PV))/(negpionTrajectory->GetdDca(V2PV)));

        ///////////////////////////////////////////////////////////////////////////////
        //Defining pT before cuts and putting stuff into pt bins
        const H1CentralFittedV0* v0 = K0Cand[loop]->GetV0();
        Double_t pT = v0->GetPt();

        //Raw K0 Candidate data after our event selection defined in create_evls.C
        hK0->Fill(invMasskaon);
        hKaonpTbefore->Fill(pT);
        
        //Fill pT bins for Raw K0 Candidate data after our event selection defined in create_evls.C
        if (pT >= ptbin1l && pT < ptbin1u) {
            h1K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin2l && pT < ptbin2u) {
            h2K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin3l && pT < ptbin3u) {
            h3K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin4l && pT < ptbin4u) {
            h4K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin5l && pT < ptbin5u) {
            h5K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin6l && pT < ptbin6u) {
            h6K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin7l && pT < ptbin7u) {
            h7K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin8l && pT < ptbin8u) {
            h8K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin9l && pT < ptbin9u) {
            h9K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin10l && pT < ptbin10u) {
            h10K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin11l && pT < ptbin11u) {
            h11K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin12l && pT < ptbin12u) {
            h12K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin13l && pT < ptbin13u) {
            h13K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin14l && pT < ptbin14u) {
            h14K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin15l && pT < ptbin15u) {
            h15K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin16l && pT < ptbin16u) {
            h16K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin17l && pT < ptbin17u) {
            h17K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin18l && pT < ptbin18u) {
            h18K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin19l && pT < ptbin19u) {
            h19K0->Fill(invMasskaon);
        }
        else if (pT >= ptbin20l && pT < ptbin20u) {
            h20K0->Fill(invMasskaon);
        }
        
        //PID Likelihood Cut for Pions < 0.003
        if (PosPionPSTrack->GetDedxLikelihood(H1Dedx::kPion) < 0.003 || NegPionPSTrack->GetDedxLikelihood(H1Dedx::kPion) < 0.003) continue;
      
       
        //DCA over dDCA proton cut
        if (abs((pospionTrajectory->GetDca(V2PV)) / (pospionTrajectory->GetdDca(V2PV))) < 1 || abs((negpionTrajectory->GetDca(V2PV)) / (negpionTrajectory->GetdDca(V2PV))) < 1) continue;
       
        //ptrel(proton/pion) cut, want it < 0.12 GeV
        Double_t pTrelPosPion = K0Cand[loop]->GetPosPionPtRel();
        Double_t pTrelNegPion = K0Cand[loop]->GetNegPionPtRel();
        if (pTrelPosPion < 0.10 || pTrelNegPion < 0.10) continue;
       
        //Cut on Track Length of proton and pion
        if (PosPionCFTrack->GetLength() < 15 || NegPionCFTrack->GetLength() < 15) continue;
        
        //Cut against Lambda contamination
        Double_t invMasslambdaCont = 1.125; //initialization
        if (qlPosPion > qlNegPion) {
            Double_t invMasslambdaCont = (PosPionH1Track->GetFourVector(mProton)
                + NegPionH1Track->GetFourVector(mPion)).M();
        }
        else if (qlPosPion < qlNegPion) {
            Double_t invMasslambdaCont = (PosPionH1Track->GetFourVector(mPion)
                + NegPionH1Track->GetFourVector(mProton)).M();
        }
       
        if (invMasslambdaCont > 1.108 && invMasslambdaCont < 1.125) continue;
       
        //Fill plots for after all cuts
      
        hKaonpTafter->Fill(pT);
        //After All necessary cuts, now can fill different pt bins
        if (pT >= ptbin1l && pT < ptbin1u) {
            hK01->Fill(invMasskaon);
        }
        else if (pT >= ptbin2l && pT < ptbin2u) {
            hK02->Fill(invMasskaon);
        }
        else if (pT >= ptbin3l && pT < ptbin3u) {
            hK03->Fill(invMasskaon);
        }
        else if (pT >= ptbin4l && pT < ptbin4u) {
            hK04->Fill(invMasskaon);
        }
        else if (pT >= ptbin5l && pT < ptbin5u) {
            hK05->Fill(invMasskaon);
        }
        else if (pT >= ptbin6l && pT < ptbin6u) {
            hK06->Fill(invMasskaon);
        }
        else if (pT >= ptbin7l && pT < ptbin7u) {
            hK07->Fill(invMasskaon);
        }
        else if (pT >= ptbin8l && pT < ptbin8u) {
            hK08->Fill(invMasskaon);
        }
        else if (pT >= ptbin9l && pT < ptbin9u) {
            hK09->Fill(invMasskaon);
        } 
        else if (pT >= ptbin10l && pT < ptbin10u) {
            hK010->Fill(invMasskaon);
        }
        else if (pT >= ptbin11l && pT < ptbin11u) {
            hK011->Fill(invMasskaon);
        }
        else if (pT >= ptbin12l && pT < ptbin12u) {
            hK012->Fill(invMasskaon);
        }
        else if (pT >= ptbin13l && pT < ptbin13u) {
            hK013->Fill(invMasskaon);
        }
        else if (pT >= ptbin14l && pT < ptbin14u) {
            hK014->Fill(invMasskaon);
        }
        else if (pT >= ptbin15l && pT < ptbin15u) {
            hK015->Fill(invMasskaon);
        }
        else if (pT >= ptbin16l && pT < ptbin16u) {
            hK016->Fill(invMasskaon);
        }
        else if (pT >= ptbin17l && pT < ptbin17u) {
            hK017->Fill(invMasskaon);
        }
        else if (pT >= ptbin18l && pT < ptbin18u) {
            hK018->Fill(invMasskaon);
        }
        else if (pT >= ptbin19l && pT < ptbin19u) {
            hK019->Fill(invMasskaon);
        }
        else if (pT >= ptbin20l && pT < ptbin20u) {
            hK020->Fill(invMasskaon);
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
       
       hK0->SetStats(0);
       hK0->SetLineColor(kBlack);
       h1K0->SetLineColor(kBlack);
       h2K0->SetLineColor(kBlack);
       h3K0->SetLineColor(kBlack);
       h4K0->SetLineColor(kBlack);
       h5K0->SetLineColor(kBlack);
       h6K0->SetLineColor(kBlack);
       h7K0->SetLineColor(kBlack);
       h8K0->SetLineColor(kBlack);
       h9K0->SetLineColor(kBlack);
       hK0->GetXaxis()->SetTitle("Mass [Gev]");
       hK0->GetYaxis()->SetTitle("Count");
       hK0->Draw();
       hK0->Write();

       h1K0->SetStats(0);
       h1K0->SetLineColor(kBlack);
       h1K0->GetXaxis()->SetTitle("Mass [Gev]");
       h1K0->GetYaxis()->SetTitle("Count");
       h1K0->Draw();
       h1K0->Write();

       h2K0->SetStats(0);
       h2K0->SetLineColor(kBlack);
       h2K0->GetXaxis()->SetTitle("Mass [Gev]");
       h2K0->GetYaxis()->SetTitle("Count");
       h2K0->Draw();
       h2K0->Write();

       h3K0->SetStats(0);
       h3K0->SetLineColor(kBlack);
       h3K0->GetXaxis()->SetTitle("Mass [Gev]");
       h3K0->GetYaxis()->SetTitle("Count");
       h3K0->Draw();
       h3K0->Write();

       h4K0->SetStats(0);
       h4K0->SetLineColor(kBlack);
       h4K0->GetXaxis()->SetTitle("Mass [Gev]");
       h4K0->GetYaxis()->SetTitle("Count");
       h4K0->Draw();
       h4K0->Write();

       h5K0->SetStats(0);
       h5K0->SetLineColor(kBlack);
       h5K0->GetXaxis()->SetTitle("Mass [Gev]");
       h5K0->GetYaxis()->SetTitle("Count");
       h5K0->Draw();
       h5K0->Write();

       h6K0->SetStats(0);
       h6K0->SetLineColor(kBlack);
       h6K0->GetXaxis()->SetTitle("Mass [Gev]");
       h6K0->GetYaxis()->SetTitle("Count");
       h6K0->Draw();
       h6K0->Write();

       h7K0->SetStats(0);
       h7K0->SetLineColor(kBlack);
       h7K0->GetXaxis()->SetTitle("Mass [Gev]");
       h7K0->GetYaxis()->SetTitle("Count");
       h7K0->Draw();
       h7K0->Write();

       h8K0->SetStats(0);
       h8K0->SetLineColor(kBlack);
       h8K0->GetXaxis()->SetTitle("Mass [Gev]");
       h8K0->GetYaxis()->SetTitle("Count");
       h8K0->Draw();
       h8K0->Write();

       h9K0->SetStats(0);
       h9K0->SetLineColor(kBlack);
       h9K0->GetXaxis()->SetTitle("Mass [Gev]");
       h9K0->GetYaxis()->SetTitle("Count");
       h9K0->Draw();
       h9K0->Write();

       h10K0->SetStats(0);
       h10K0->SetLineColor(kBlack);
       h10K0->GetXaxis()->SetTitle("Mass [Gev]");
       h10K0->GetYaxis()->SetTitle("Count");
       h10K0->Draw();
       h10K0->Write();

       h11K0->SetStats(0);
       h11K0->SetLineColor(kBlack);
       h11K0->GetXaxis()->SetTitle("Mass [Gev]");
       h11K0->GetYaxis()->SetTitle("Count");
       h11K0->Draw();
       h11K0->Write();

       h12K0->SetStats(0);
       h12K0->SetLineColor(kBlack);
       h12K0->GetXaxis()->SetTitle("Mass [Gev]");
       h12K0->GetYaxis()->SetTitle("Count");
       h12K0->Draw();
       h12K0->Write();

       h13K0->SetStats(0);
       h13K0->SetLineColor(kBlack);
       h13K0->GetXaxis()->SetTitle("Mass [Gev]");
       h13K0->GetYaxis()->SetTitle("Count");
       h13K0->Draw();
       h13K0->Write();

       h14K0->SetStats(0);
       h14K0->SetLineColor(kBlack);
       h14K0->GetXaxis()->SetTitle("Mass [Gev]");
       h14K0->GetYaxis()->SetTitle("Count");
       h14K0->Draw();
       h14K0->Write();

       h15K0->SetStats(0);
       h15K0->SetLineColor(kBlack);
       h15K0->GetXaxis()->SetTitle("Mass [Gev]");
       h15K0->GetYaxis()->SetTitle("Count");
       h15K0->Draw();
       h15K0->Write();

       h16K0->SetStats(0);
       h16K0->SetLineColor(kBlack);
       h16K0->GetXaxis()->SetTitle("Mass [Gev]");
       h16K0->GetYaxis()->SetTitle("Count");
       h16K0->Draw();
       h16K0->Write();

       h17K0->SetStats(0);
       h17K0->SetLineColor(kBlack);
       h17K0->GetXaxis()->SetTitle("Mass [Gev]");
       h17K0->GetYaxis()->SetTitle("Count");
       h17K0->Draw();
       h17K0->Write();

       h18K0->SetStats(0);
       h18K0->SetLineColor(kBlack);
       h18K0->GetXaxis()->SetTitle("Mass [Gev]");
       h18K0->GetYaxis()->SetTitle("Count");
       h18K0->Draw();
       h18K0->Write();

       h19K0->SetStats(0);
       h19K0->SetLineColor(kBlack);
       h19K0->GetXaxis()->SetTitle("Mass [Gev]");
       h19K0->GetYaxis()->SetTitle("Count");
       h19K0->Draw();
       h19K0->Write();

       h20K0->SetStats(0);
       h20K0->SetLineColor(kBlack);
       h20K0->GetXaxis()->SetTitle("Mass [Gev]");
       h20K0->GetYaxis()->SetTitle("Count");
       h20K0->Draw();
       h20K0->Write();

       hK01->SetStats(0);
       hK01->SetLineColor(kBlack);
       hK01->GetXaxis()->SetTitle("Mass [Gev]");
       hK01->GetYaxis()->SetTitle("Count");
       hK01->Draw();
       hK01->Write();

       hK02->SetStats(0);
       hK02->SetLineColor(kBlack);
       hK02->GetXaxis()->SetTitle("Mass [Gev]");
       hK02->GetYaxis()->SetTitle("Count");
       hK02->Draw();
       hK02->Write();

       hK03->SetStats(0);
       hK03->SetLineColor(kBlack);
       hK03->GetXaxis()->SetTitle("Mass [Gev]");
       hK03->GetYaxis()->SetTitle("Count");
       hK03->Draw();
       hK03->Write();

       hK04->SetStats(0);
       hK04->SetLineColor(kBlack);
       hK04->GetXaxis()->SetTitle("Mass [Gev]");
       hK04->GetYaxis()->SetTitle("Count");
       hK04->Draw();
       hK04->Write();

       hK05->SetStats(0);
       hK05->SetLineColor(kBlack);
       hK05->GetXaxis()->SetTitle("Mass [Gev]");
       hK05->GetYaxis()->SetTitle("Count");
       hK05->Draw();
       hK05->Write();

       hK06->SetStats(0);
       hK06->SetLineColor(kBlack);
       hK06->GetXaxis()->SetTitle("Mass [Gev]");
       hK06->GetYaxis()->SetTitle("Count");
       hK06->Draw();
       hK06->Write();

       hK07->SetStats(0);
       hK07->SetLineColor(kBlack);
       hK07->GetXaxis()->SetTitle("Mass [Gev]");
       hK07->GetYaxis()->SetTitle("Count");
       hK07->Draw();
       hK07->Write();

       hK08->SetStats(0);
       hK08->SetLineColor(kBlack);
       hK08->GetXaxis()->SetTitle("Mass [Gev]");
       hK08->GetYaxis()->SetTitle("Count");
       hK08->Draw();
       hK08->Write();

       hK09->SetStats(0);
       hK09->SetLineColor(kBlack);
       hK09->GetXaxis()->SetTitle("Mass [Gev]");
       hK09->GetYaxis()->SetTitle("Count");
       hK09->Draw();
       hK09->Write();

       hK010->SetStats(0);
       hK010->SetLineColor(kBlack);
       hK010->GetXaxis()->SetTitle("Mass [Gev]");
       hK010->GetYaxis()->SetTitle("Count");
       hK010->Draw();
       hK010->Write();

       hK011->SetStats(0);
       hK011->SetLineColor(kBlack);
       hK011->GetXaxis()->SetTitle("Mass [Gev]");
       hK011->GetYaxis()->SetTitle("Count");
       hK011->Draw();
       hK011->Write();

       hK012->SetStats(0);
       hK012->SetLineColor(kBlack);
       hK012->GetXaxis()->SetTitle("Mass [Gev]");
       hK012->GetYaxis()->SetTitle("Count");
       hK012->Draw();
       hK012->Write();

       hK013->SetStats(0);
       hK013->SetLineColor(kBlack);
       hK013->GetXaxis()->SetTitle("Mass [Gev]");
       hK013->GetYaxis()->SetTitle("Count");
       hK013->Draw();
       hK013->Write();

       hK014->SetStats(0);
       hK014->SetLineColor(kBlack);
       hK014->GetXaxis()->SetTitle("Mass [Gev]");
       hK014->GetYaxis()->SetTitle("Count");
       hK014->Draw();
       hK014->Write();

       hK015->SetStats(0);
       hK015->SetLineColor(kBlack);
       hK015->GetXaxis()->SetTitle("Mass [Gev]");
       hK015->GetYaxis()->SetTitle("Count");
       hK015->Draw();
       hK015->Write();

       hK016->SetStats(0);
       hK016->SetLineColor(kBlack);
       hK016->GetXaxis()->SetTitle("Mass [Gev]");
       hK016->GetYaxis()->SetTitle("Count");
       hK016->Draw();
       hK016->Write();

       hK017->SetStats(0);
       hK017->SetLineColor(kBlack);
       hK017->GetXaxis()->SetTitle("Mass [Gev]");
       hK017->GetYaxis()->SetTitle("Count");
       hK017->Draw();
       hK017->Write();

       hK018->SetStats(0);
       hK018->SetLineColor(kBlack);
       hK018->GetXaxis()->SetTitle("Mass [Gev]");
       hK018->GetYaxis()->SetTitle("Count");
       hK018->Draw();
       hK018->Write();

       hK019->SetStats(0);
       hK019->SetLineColor(kBlack);
       hK019->GetXaxis()->SetTitle("Mass [Gev]");
       hK019->GetYaxis()->SetTitle("Count");
       hK019->Draw();
       hK019->Write();

       hK020->SetStats(0);
       hK020->SetLineColor(kBlack);
       hK020->GetXaxis()->SetTitle("Mass [Gev]");
       hK020->GetYaxis()->SetTitle("Count");
       hK020->Draw();
       hK020->Write();

       hKaonpTbefore->SetStats(0);
       hKaonpTbefore->SetLineColor(kBlack);
       hKaonpTbefore->GetXaxis()->SetTitle("pT [Gev]");
       hKaonpTbefore->GetYaxis()->SetTitle("Count");
       hKaonpTbefore->Draw();
       hKaonpTbefore->Write();

       hKaonpTafter->SetStats(0);
       hKaonpTafter->SetLineColor(kBlack);
       hKaonpTafter->GetXaxis()->SetTitle("pT [Gev]");
       hKaonpTafter->GetYaxis()->SetTitle("Count");
       hKaonpTafter->Draw();
       hKaonpTafter->Write();

       hAPbefore->SetStats(0);
       hAPbefore->GetXaxis()->SetTitle("\\alpha = (p^{+}_{L} - p^{-}_{L})/(p^{+}_{L} + p^{-}_{L})");
       hAPbefore->GetYaxis()->SetTitle("p_{T} [GeV]");
       hAPbefore->GetXaxis()->SetTitleSize(0.04);
       hAPbefore->GetXaxis()->SetTitleOffset(1);
       hAPbefore->GetYaxis()->SetTitleSize(0.05);
       hAPbefore->Draw("colz");
       hAPbefore->Write();
       gPad->Update();
       canvas->SaveAs("kaonAPbefore.root");

       hAPafter->SetStats(0);
       hAPafter->GetXaxis()->SetTitle("\\alpha = (p^{+}_{L} - p^{-}_{L})/(p^{+}_{L} + p^{-}_{L})");
       hAPafter->GetYaxis()->SetTitle("p_{T} [GeV]");
       hAPafter->GetXaxis()->SetTitleSize(0.04);
       hAPafter->GetXaxis()->SetTitleOffset(1);
       hAPafter->GetYaxis()->SetTitleSize(0.05);
       hAPafter->Draw("colz");
       hAPafter->Write();
       gPad->Update();
       canvas->SaveAs("kaonAPafter.root");

       hK0PIDpion->SetStats(0);
       hK0PIDpion->SetLineColor(kOrange);
       h1K0PIDpion->SetLineColor(kOrange);
       h2K0PIDpion->SetLineColor(kOrange);
       h3K0PIDpion->SetLineColor(kOrange);
       h4K0PIDpion->SetLineColor(kOrange);
       h5K0PIDpion->SetLineColor(kOrange);
       h6K0PIDpion->SetLineColor(kOrange);
       h7K0PIDpion->SetLineColor(kOrange);
       h8K0PIDpion->SetLineColor(kOrange);
       h9K0PIDpion->SetLineColor(kOrange);
       hK0PIDpion->Draw();
       hK0PIDpion->Write();

       hK0DCAoverdDCApion->SetStats(0);
       hK0DCAoverdDCApion->SetLineColor(kCyan+3);
       h1K0DCAoverdDCApion->SetLineColor(kCyan + 3);
       h2K0DCAoverdDCApion->SetLineColor(kCyan + 3);
       h3K0DCAoverdDCApion->SetLineColor(kCyan + 3);
       h4K0DCAoverdDCApion->SetLineColor(kCyan + 3);
       h5K0DCAoverdDCApion->SetLineColor(kCyan + 3);
       h6K0DCAoverdDCApion->SetLineColor(kCyan + 3);
       h7K0DCAoverdDCApion->SetLineColor(kCyan + 3);
       h8K0DCAoverdDCApion->SetLineColor(kCyan + 3);
       h9K0DCAoverdDCApion->SetLineColor(kCyan + 3);
       hK0DCAoverdDCApion->Draw();
       hK0DCAoverdDCApion->Write();

       hK0ptrel->SetStats(0);
       hK0ptrel->SetLineColor(kGreen);
       h1K0ptrel->SetLineColor(kGreen);
       h2K0ptrel->SetLineColor(kGreen);
       h3K0ptrel->SetLineColor(kGreen);
       h4K0ptrel->SetLineColor(kGreen);
       h5K0ptrel->SetLineColor(kGreen);
       h6K0ptrel->SetLineColor(kGreen);
       h7K0ptrel->SetLineColor(kGreen);
       h8K0ptrel->SetLineColor(kGreen);
       h9K0ptrel->SetLineColor(kGreen);
       hK0ptrel->Draw();
       hK0ptrel->Write();

       hK0pt->SetStats(0);
       hK0pt->SetLineColor(kBlue);
       h1K0pt->SetLineColor(kBlue);
       h2K0pt->SetLineColor(kBlue);
       h3K0pt->SetLineColor(kBlue);
       h4K0pt->SetLineColor(kBlue);
       h5K0pt->SetLineColor(kBlue);
       h6K0pt->SetLineColor(kBlue);
       h7K0pt->SetLineColor(kBlue);
       h8K0pt->SetLineColor(kBlue);
       h9K0pt->SetLineColor(kBlue);
       hK0pt->Draw();
       hK0pt->Write();

       //hLambdachi2->SetStats(0);
       //hLambdachi2->SetLineColor(kMagenta);
       //hLambdachi2->Draw();
       //hLambdachi2->Write();

       hK0Eta->SetStats(0);
       hK0Eta->SetLineColor(kGreen+3);
       h1K0Eta->SetLineColor(kGreen + 3);
       h2K0Eta->SetLineColor(kGreen + 3);
       h3K0Eta->SetLineColor(kGreen + 3);
       h4K0Eta->SetLineColor(kGreen + 3);
       h5K0Eta->SetLineColor(kGreen + 3);
       h6K0Eta->SetLineColor(kGreen + 3);
       h7K0Eta->SetLineColor(kGreen + 3);
       h8K0Eta->SetLineColor(kGreen + 3);
       h9K0Eta->SetLineColor(kGreen + 3);
       hK0Eta->Draw();
       hK0Eta->Write();

       hK0tracklength->SetStats(0);
       hK0tracklength->SetLineColor(kViolet);
       h1K0tracklength->SetLineColor(kViolet);
       h2K0tracklength->SetLineColor(kViolet);
       h3K0tracklength->SetLineColor(kViolet);
       h4K0tracklength->SetLineColor(kViolet);
       h5K0tracklength->SetLineColor(kViolet);
       h6K0tracklength->SetLineColor(kViolet);
       h7K0tracklength->SetLineColor(kViolet);
       h8K0tracklength->SetLineColor(kViolet);
       h9K0tracklength->SetLineColor(kViolet);
       hK0tracklength->Draw();
       hK0tracklength->Write();

       hK0LambdaCont->SetStats(0);
       hK0LambdaCont->Draw();
       hK0LambdaCont->Write();

       hDedxLpion->SetStats(0);
       hDedxLpion->SetFillColor(kRed);
       hDedxLpion->GetXaxis()->SetTitle("Likelihood");
       hDedxLpion->GetYaxis()->SetTitle("Count");
       hDedxLpion->Draw();
       hDedxLpion->Write();

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

       hdEdxBothBefore->SetStats(0);
       hdEdxBothBefore->GetXaxis()->SetTitle("p [GeV]");
       hdEdxBothBefore->GetYaxis()->SetTitle("dE/dx");
       hdEdxBothBefore->Draw("colz");
       hdEdxBothBefore->Write();
       gPad->Update();
       canvas->SaveAs("kaondEdxBothBefore.root");

       hdEdxBothAfterLcut->SetStats(0);
       hdEdxBothAfterLcut->GetXaxis()->SetTitle("p [GeV]");
       hdEdxBothAfterLcut->GetYaxis()->SetTitle("dE/dx");
       hdEdxBothAfterLcut->Draw("colz");
       hdEdxBothAfterLcut->Write();
       gPad->Update();
       canvas->SaveAs("kaondEdxBothAfterLcut.root");

       hdEdxBothAfterAllcuts->SetStats(0);
       hdEdxBothAfterAllcuts->GetXaxis()->SetTitle("p [GeV]");
       hdEdxBothAfterAllcuts->GetYaxis()->SetTitle("dE/dx");
       hdEdxBothAfterAllcuts->Draw("colz");
       hdEdxBothAfterAllcuts->Write();
       gPad->Update();
       canvas->SaveAs("kaondEdxBothAfterAllcuts.root");

       hs1->Add(h1K0);
       hs1->Add(h1K0PIDpion);
       hs1->Add(h1K0DCAoverdDCApion);
       hs1->Add(h1K0ptrel);
       hs1->Add(h1K0pt);
       //hs1->Add(h1K0chi2);
       hs1->Add(h1K0Eta);
       hs1->Add(h1K0tracklength);
       hs1->Add(h1K0LambdaCont);
       //hs1->GetXaxis()->SetTitle("Mass [Gev]");
       //hs1->GetYaxis()->SetTitle("Count");
       hs1->Draw("nostack");
       hs1->Write();
       gPad->Update();
       canvas->SaveAs("kaonstack1.root");

       hs2->Add(h2K0);
       hs2->Add(h2K0PIDpion);
       hs2->Add(h2K0DCAoverdDCApion);
       hs2->Add(h2K0ptrel);
       hs2->Add(h2K0pt);
       //hs1->Add(h2K0chi2);
       hs2->Add(h2K0Eta);
       hs2->Add(h2K0tracklength);
       hs2->Add(h2K0LambdaCont);
       //hs1->GetXaxis()->SetTitle("Mass [Gev]");
       //hs1->GetYaxis()->SetTitle("Count");
       hs2->Draw("nostack");
       hs2->Write();
       gPad->Update();
       canvas->SaveAs("kaonstack2.root");

       hs3->Add(h3K0);
       hs3->Add(h3K0PIDpion);
       hs3->Add(h3K0DCAoverdDCApion);
       hs3->Add(h3K0ptrel);
       hs3->Add(h3K0pt);
       //hs3->Add(h2K0chi2);
       hs3->Add(h3K0Eta);
       hs3->Add(h3K0tracklength);
       hs3->Add(h3K0LambdaCont);
       //hs3->GetXaxis()->SetTitle("Mass [Gev]");
       //hs3->GetYaxis()->SetTitle("Count");
       hs3->Draw("nostack");
       hs3->Write();
       gPad->Update();
       canvas->SaveAs("kaonstack3.root");

       hs4->Add(h4K0);
       hs4->Add(h4K0PIDpion);
       hs4->Add(h4K0DCAoverdDCApion);
       hs4->Add(h4K0ptrel);
       hs4->Add(h4K0pt);
       //hs3->Add(h2K0chi2);
       hs4->Add(h4K0Eta);
       hs4->Add(h4K0tracklength);
       hs4->Add(h4K0LambdaCont);
       //hs3->GetXaxis()->SetTitle("Mass [Gev]");
       //hs3->GetYaxis()->SetTitle("Count");
       hs4->Draw("nostack");
       hs4->Write();
       gPad->Update();
       canvas->SaveAs("kaonstack4.root");

       hs5->Add(h5K0);
       hs5->Add(h5K0PIDpion);
       hs5->Add(h5K0DCAoverdDCApion);
       hs5->Add(h5K0ptrel);
       hs5->Add(h5K0pt);
       //hs3->Add(h2K0chi2);
       hs5->Add(h5K0Eta);
       hs5->Add(h5K0tracklength);
       hs5->Add(h5K0LambdaCont);
       //hs3->GetXaxis()->SetTitle("Mass [Gev]");
       //hs3->GetYaxis()->SetTitle("Count");
       hs5->Draw("nostack");
       hs5->Write();
       gPad->Update();
       canvas->SaveAs("kaonstack5.root");

       hs6->Add(h6K0);
       hs6->Add(h6K0PIDpion);
       hs6->Add(h6K0DCAoverdDCApion);
       hs6->Add(h6K0ptrel);
       hs6->Add(h6K0pt);
       //hs3->Add(h2K0chi2);
       hs6->Add(h6K0Eta);
       hs6->Add(h6K0tracklength);
       hs6->Add(h6K0LambdaCont);
       //hs3->GetXaxis()->SetTitle("Mass [Gev]");
       //hs3->GetYaxis()->SetTitle("Count");
       hs6->Draw("nostack");
       hs6->Write();
       gPad->Update();
       canvas->SaveAs("kaonstack6.root");

       hs7->Add(h7K0);
       hs7->Add(h7K0PIDpion);
       hs7->Add(h7K0DCAoverdDCApion);
       hs7->Add(h7K0ptrel);
       hs7->Add(h7K0pt);
       //hs3->Add(h2K0chi2);
       hs7->Add(h7K0Eta);
       hs7->Add(h7K0tracklength);
       hs7->Add(h7K0LambdaCont);
       //hs3->GetXaxis()->SetTitle("Mass [Gev]");
       //hs3->GetYaxis()->SetTitle("Count");
       hs7->Draw("nostack");
       hs7->Write();
       gPad->Update();
       canvas->SaveAs("kaonstack7.root");

       hs8->Add(h8K0);
       hs8->Add(h8K0PIDpion);
       hs8->Add(h8K0DCAoverdDCApion);
       hs8->Add(h8K0ptrel);
       hs8->Add(h8K0pt);
       //hs3->Add(h2K0chi2);
       hs8->Add(h8K0Eta);
       hs8->Add(h8K0tracklength);
       hs8->Add(h8K0LambdaCont);
       //hs3->GetXaxis()->SetTitle("Mass [Gev]");
       //hs3->GetYaxis()->SetTitle("Count");
       hs8->Draw("nostack");
       hs8->Write();
       gPad->Update();
       canvas->SaveAs("kaonstack8.root");

       hs9->Add(h9K0);
       hs9->Add(h9K0PIDpion);
       hs9->Add(h9K0DCAoverdDCApion);
       hs9->Add(h9K0ptrel);
       hs9->Add(h9K0pt);
       //hs3->Add(h2K0chi2);
       hs9->Add(h9K0Eta);
       hs9->Add(h9K0tracklength);
       hs9->Add(h9K0LambdaCont);
       //hs3->GetXaxis()->SetTitle("Mass [Gev]");
       //hs3->GetYaxis()->SetTitle("Count");
       hs9->Draw("nostack");
       hs9->Write();
       gPad->Update();
       canvas->SaveAs("kaonstack9.root");

       hs->Add(hK0);
       hs->Add(hK0PIDpion);
       hs->Add(hK0DCAoverdDCApion);
       hs->Add(hK0ptrel);
       hs->Add(hK0pt);
       //hs->Add(hK0chi2);
       hs->Add(hK0Eta);
       hs->Add(hK0tracklength);
       hs->Add(hK0LambdaCont);
       //hs->GetXaxis()->SetTitle("Mass [Gev]");
       //hs->GetYaxis()->SetTitle("Count");
       hs->Draw("nostack");
       hs->Write();
       gPad->Update();
       canvas->SaveAs("kaonstack.root");

       file.Write();
       file.Close();
       cout << "Histograms written to " << opts.GetOutput() << endl;


        return 0;
    }