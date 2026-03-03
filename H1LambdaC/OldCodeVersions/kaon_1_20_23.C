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
#include <cmath>
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
//H1 OO includes
#include "H1Pointers/H1FloatPtr.h"
#include "H1Skeleton/H1EventList.h"
#include "H1Skeleton/H1Tree.h"
#include "H1Geom/H1DBManager.h"
// tracks
#include "H1Tracks/H1TrackEvent.h"
#include "H1Tracks/H1Track.h"
#include "H1Tracks/H1Trajectory.h"
#include "H1Tracks/H1CentralFittedTrack.h"
#include "H1Tracks/H1CentralFittedV0.h"
#include "H1Tracks/H1CentralFittedV0ArrayPtr.h"
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
#include "H1Mods/H1PartK0ArrayPtr.h"
#include "H1Mods/H1PartK0.h"
#include "H1Mods/H1SelVertex.h"
#include "H1Tracks/H1ReconstructedVertex.h"
#include "H1Tracks/H1CentralVertex.h"
#include "H1Tracks/H1CentralVertexArrayPtr.h"
#include "H1Steering/H1StdCmdLine.h"



using namespace std;


int
main(int argc, char* argv[])
{
    //Output file name and path
    string output = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/kaon.root";

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

    //Kaon lower and upper beak bounds in GeV
    double kaonlowerbound = 0.44;
    double kaonupperbound = 0.56;

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
    //Kaons in whole pt bin range
    TH1F* hKaonWholepTrange = new TH1F("Kaon Bins", "Kaon pT Bins 1-9", kaonbins, kaonlowerbinrange, kaonupperbinrange);

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

    TH1F* h2K0 = new TH1F("K01", "K01", kaonbins, kaonlowerbinrange, kaonupperbinrange);
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

    TH1F* h3K0 = new TH1F("K01", "K01", kaonbins, kaonlowerbinrange, kaonupperbinrange);
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

    TH1F* h4K0 = new TH1F("K01", "K01", kaonbins, kaonlowerbinrange, kaonupperbinrange);
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

    TH1F* h5K0 = new TH1F("K01", "K01", kaonbins, kaonlowerbinrange, kaonupperbinrange);
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

    TH1F* h6K0 = new TH1F("K01", "K01", kaonbins, kaonlowerbinrange, kaonupperbinrange);
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

    TH1F* h7K0 = new TH1F("K01", "K01", kaonbins, kaonlowerbinrange, kaonupperbinrange);
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

    TH1F* h8K0 = new TH1F("K01", "K01", kaonbins, kaonlowerbinrange, kaonupperbinrange);
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

    TH1F* h9K0 = new TH1F("K01", "K01", kaonbins, kaonlowerbinrange, kaonupperbinrange);
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

    //Armenteros plot for particle ID lambdas vs kaons
    TH2F* hAPbefore = new TH2F("AP", "Armenteros-Podolansky Plot (After H1FindK0)", 200, -1, 1, 200, 0, 0.3);
    TH2F* hAPafter = new TH2F("AP", "Armenteros-Podolansky Plot (After All Cuts)", 200, -1, 1, 200, 0, 0.3);

    //DCA vs dDCA to help visualize DCA significance Cut
    TH2F* hpionDCAvsdDCAbefore = new TH2F("Pion DCA vs dDCA Before", "Pion DCA vs dDCA (After H1FindK0)", 200, 0, 3, 1000, -10, -10);
    TH2F* hpionDCAvsdDCAafter = new TH2F("Pion DCA vs dDCA After", "Pion DCA vs dDCA (After Cuts)", 200, 0, 3, 1000, -10, -10);
    TH2F* hpionDCAvspTbefore = new TH2F("Pion DCA vs pT Before", "Pion DCA vs pT (After H1FindK0)", 500, 0, 10, 1000, -10, -10);
    TH2F* hpiondDCAvspTbefore = new TH2F("Pion dDCA vs pT Before", "Pion DCA vs pT (After H1FindK0)", 500, 0, 10, 1000, -10, -10);

    //dEdx plot for particle ID protons vs pions
    TH2D* hdEdxBothBefore = new TH2D("dEdxvP Initial (After H1FindK0)", "dE/dx Initial (After H1FindK0)", 500, 0, 2, 500, 0, 10);
    TH2D* hdEdxBothAfterLcut = new TH2D("dEdxvP After Likelihood Cut", "dE/dx After Likelihood Cut", 500, 0, 2, 500, 0, 10);
    TH2D* hdEdxBothAfterAllcuts = new TH2D("dEdxvP After All Cuts", "dE/dx After All Cuts", 500, 0, 2, 500, 0, 10);
   
    //DCA/dDCA histograms
    TH1F* hdDCApion = new TH1F("Pion dDCA", "Pion dDCA", 1000, 0, 10);
    TH1F* hDCApion = new TH1F("Pion DCA", "Pion DCA", 1000, -10, 10);
    TH1F* hDCAoverdDCApion = new TH1F("Pion DCA/dDCA", "Pion DCA/dDCA", 1000, -10, 10);
    
    
    //Plots for K0 Distributions
    TH1F* hK0VsBjorkenX = new TH1F("K0 Bjorken X", "K0 Bjorken X", 996, 0.00004, 0.01);
    TH1F* hK0VspT = new TH1F("K0 pT", "K0 pT", 2000, 0, 20);
    TH1F* hK0VsEta = new TH1F("K0 Eta", "K0 Eta", 2400, -12, 12);
    TH1F* hK0VsQ2 = new TH1F("K0 Q2", "K0 Q2", 980, 2, 100);
    TH1F* hK0Vsy = new TH1F("K0 y", "K0 y", 100, 0, 1);
    TH1F* hK0Vsphi = new TH1F("K0 Phi", "K0 Phi", 800, -4, 4);

 

    //Likelihood histograms
    TH1F* hDedxLpion = new TH1F("Pion Likelihood", "Pion Likelihood", 1000, 0, 1);
    //Defining Root Canvas to work on
    TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 400);

    // load in some HAT data
    H1FloatPtr q2e("Q2e");
    H1FloatPtr ebeamE("EBeamE");
    H1FloatPtr ebeamP("EBeamP");
    H1FloatPtr ye("Ye");
    

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


    //LambdaC plots
    double cbins = 195;
    TH1F* hC1 = new TH1F("LambdaC 1", "LambdaC 1", cbins, 0.28646, 4.28646);
    TH1F* hC2 = new TH1F("LambdaC 2", "LambdaC 2", cbins, 0.28646, 4.28646);
    TH1F* hC3 = new TH1F("LambdaC 3", "LambdaC 3", cbins, 0.28646, 4.28646);
    TH1F* hC4 = new TH1F("LambdaC 4", "LambdaC 4", cbins, 0.28646, 4.28646);
    TH1F* hC5 = new TH1F("LambdaC 5", "LambdaC 5", cbins, 0.28646, 4.28646);
    TH1F* hC6 = new TH1F("LambdaC 6", "LambdaC 6", cbins, 0.28646, 4.28646);
    TH1F* hC7 = new TH1F("LambdaC 7", "LambdaC 7", cbins, 0.28646, 4.28646);
    TH1F* hC8 = new TH1F("LambdaC 8", "LambdaC 8", cbins, 0.28646, 4.28646);
    TH1F* hCfinal = new TH1F("LambdaC Final", "LambdaC Final", cbins, 0.28646, 4.28646);

    TH1F* hCfinalbw1 = new TH1F("LambdaC Final BW1", "LambdaC Final BW1", 400, -0.00146, 3.99854);//bw=0.010
    TH1F* hCfinalbw2 = new TH1F("LambdaC Final BW2", "LambdaC Final BW2", 333, 0.45554, 4.45154);//bw=0.012
    TH1F* hCfinalbw3 = new TH1F("LambdaC Final BW3", "LambdaC Final BW3", 286, 0.00254, 4.00654);//bw=0.014
    TH1F* hCfinalbw4 = new TH1F("LambdaC Final BW4", "LambdaC Final BW4", 250, -0.00646, 3.99354);//bw=0.016
    TH1F* hCfinalbw5 = new TH1F("LambdaC Final BW5", "LambdaC Final BW5", 222, 0.00854, 4.00454);//bw=0.018
    TH1F* hCfinalbw6 = new TH1F("LambdaC Final BW6", "LambdaC Final BW6", 200, 0.00354, 4.00354);//bw=0.020
    TH1F* hCfinalbw7 = new TH1F("LambdaC Final BW7", "LambdaC Final BW7", 182, -0.00946, 3.99454);//bw=0.022
    TH1F* hCfinalbw8 = new TH1F("LambdaC Final BW8", "LambdaC Final BW8", 167, 0.00554, 4.01354);//bw=0.024
    TH1F* hCfinalbw9 = new TH1F("LambdaC Final BW9", "LambdaC Final BW9", 154, -0.01146, 3.99254);//bw=0.026
    TH1F* hCfinalbw10 = new TH1F("LambdaC Final BW10", "LambdaC Final BW10", 143, -0.00446, 3.99954);//bw=0.028
    TH1F* hCfinalbw11 = new TH1F("LambdaC Final BW11", "LambdaC Final BW11", 133, 0.00854, 3.99854);//bw=0.030

    THStack* hsC = new THStack("hsC", "Stacked K_{S}^{0} Mass Plots");

    TH2D* hdEdxProtonCBefore = new TH2D("LambdaC Proton dEdxvP Before", "LambdaC Proton dEdxvP Before", 500, 0, 2, 500, 0, 10);
    TH2D* hdEdxProtonCAfter = new TH2D("LambdaC Proton dEdxvP After", "LambdaC Proton dEdxvP After", 500, 0, 2, 500, 0, 10);
    TH1F* hDedxLprotonC = new TH1F("LambdaC Proton Likelihood", "LambdaC Proton Likelihood", 1000, 0, 1);








    // Pointer to vertices to loop over
    static H1CentralVertexArrayPtr vertex;
    static H1PartK0ArrayPtr K0Cand;
    static H1PartSelTrackArrayPtr PST;//loop over vertices now to try and get LambdaC peaks
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

        hpionDCAvspTbefore->Fill(pT, (pospionTrajectory->GetDca(V2PV)));
        hpionDCAvspTbefore->Fill(pT, (negpionTrajectory->GetDca(V2PV)));
        hpiondDCAvspTbefore->Fill(pT, (negpionTrajectory->GetdDca(V2PV)));
        hpiondDCAvspTbefore->Fill(pT, (pospionTrajectory->GetdDca(V2PV)));


        //Raw K0 Candidate data after our event selection defined in create_evls.C
        hK0->Fill(invMasskaon);
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
   ////////////////////////////////////////////////////////////////////////////////////////////////
        //PID Likelihood Cut for Pions < 0.001
        if (PosPionPSTrack->GetDedxLikelihood(H1Dedx::kPion) < 0.001 || NegPionPSTrack->GetDedxLikelihood(H1Dedx::kPion) < 0.001) { continue; }
      
        hK0PIDpion->Fill(invMasskaon);
        //Fill pT bins after PID Likelihood Cut for Pion < 0.001
        if (pT >= ptbin1l && pT < ptbin1u) {
            h1K0PIDpion->Fill(invMasskaon);
        }
        else if (pT >= ptbin2l && pT < ptbin2u) {
            h2K0PIDpion->Fill(invMasskaon);
        }
        else if (pT >= ptbin3l && pT < ptbin3u) {
            h3K0PIDpion->Fill(invMasskaon);
        }
        else if (pT >= ptbin4l && pT < ptbin4u) {
            h4K0PIDpion->Fill(invMasskaon);
        }
        else if (pT >= ptbin5l && pT < ptbin5u) {
            h5K0PIDpion->Fill(invMasskaon);
        }
        else if (pT >= ptbin6l && pT < ptbin6u) {
            h6K0PIDpion->Fill(invMasskaon);
        }
        else if (pT >= ptbin7l && pT < ptbin7u) {
            h7K0PIDpion->Fill(invMasskaon);
        }
        else if (pT >= ptbin8l && pT < ptbin8u) {
            h8K0PIDpion->Fill(invMasskaon);
        }
        else if (pT >= ptbin9l && pT < ptbin9u) {
            h9K0PIDpion->Fill(invMasskaon);
        }
        //Fill dE/dx plots for after the PID cuts
        hdEdxBothAfterLcut->Fill(PosPionCFTrack->GetMomentum().Mag(), PosPionCFTrack->GetCorrectedDedx());
        hdEdxBothAfterLcut->Fill(NegPionCFTrack->GetMomentum().Mag(), NegPionCFTrack->GetCorrectedDedx());


        hpionDCAvsdDCAbefore->Fill((pospionTrajectory->GetdDca(V2PV)), (pospionTrajectory->GetDca(V2PV)));
        hpionDCAvsdDCAbefore->Fill((negpionTrajectory->GetdDca(V2PV)), (negpionTrajectory->GetDca(V2PV)));
        hpionDCAvspTbefore->Fill(pT, (pospionTrajectory->GetDca(V2PV)));
        hpionDCAvspTbefore->Fill(pT, (negpionTrajectory->GetDca(V2PV)));
        hpiondDCAvspTbefore->Fill(pT, (negpionTrajectory->GetdDca(V2PV)));
        hpiondDCAvspTbefore->Fill(pT, (pospionTrajectory->GetdDca(V2PV)));

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //DCA over dDCA proton cut
        if (abs((pospionTrajectory->GetDca(V2PV)) / (pospionTrajectory->GetdDca(V2PV))) < 1 || abs((negpionTrajectory->GetDca(V2PV)) / (negpionTrajectory->GetdDca(V2PV))) < 1) {  continue;}


        hpionDCAvsdDCAafter->Fill((pospionTrajectory->GetdDca(V2PV)), (pospionTrajectory->GetDca(V2PV)));
        hpionDCAvsdDCAafter->Fill((negpionTrajectory->GetdDca(V2PV)), (negpionTrajectory->GetDca(V2PV)));

        hK0DCAoverdDCApion->Fill(invMasskaon);
        //Fill pT bins after DCA over dDCA pion cut
        if (pT >= ptbin1l && pT < ptbin1u) {
            h1K0DCAoverdDCApion->Fill(invMasskaon);
        }
        else if (pT >= ptbin2l && pT < ptbin2u) {
            h2K0DCAoverdDCApion->Fill(invMasskaon);
        }
        else if (pT >= ptbin3l && pT < ptbin3u) {
            h3K0DCAoverdDCApion->Fill(invMasskaon);
        }
        else if (pT >= ptbin4l && pT < ptbin4u) {
            h4K0DCAoverdDCApion->Fill(invMasskaon);
        }
        else if (pT >= ptbin5l && pT < ptbin5u) {
            h5K0DCAoverdDCApion->Fill(invMasskaon);
        }
        else if (pT >= ptbin6l && pT < ptbin6u) {
            h6K0DCAoverdDCApion->Fill(invMasskaon);
        }
        else if (pT >= ptbin7l && pT < ptbin7u) {
            h7K0DCAoverdDCApion->Fill(invMasskaon);
        }
        else if (pT >= ptbin8l && pT < ptbin8u) {
            h8K0DCAoverdDCApion->Fill(invMasskaon);
        }
        else if (pT >= ptbin9l && pT < ptbin9u) {
            h9K0DCAoverdDCApion->Fill(invMasskaon);
        }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //ptrel(proton/pion) cut, want it < 0.12 GeV
        Double_t pTrelPosPion = K0Cand[loop]->GetPosPionPtRel();
        Double_t pTrelNegPion = K0Cand[loop]->GetNegPionPtRel();
        //if (pTrelPosPion < 0.10 || pTrelNegPion < 0.10) {continue;}
        //hK0ptrel->Fill(invMasskaon);
        //Fill pT bins after ptrel(proton/pion) cut
        /*if (pT >= ptbin1l && pT < ptbin1u) {
            h1K0ptrel->Fill(invMasskaon);
        }
        else if (pT >= ptbin2l && pT < ptbin2u) {
            h2K0ptrel->Fill(invMasskaon);
        }
        else if (pT >= ptbin3l && pT < ptbin3u) {
            h3K0ptrel->Fill(invMasskaon);
        }
        else if (pT >= ptbin4l && pT < ptbin4u) {
            h4K0ptrel->Fill(invMasskaon);
        }
        else if (pT >= ptbin5l && pT < ptbin5u) {
            h5K0ptrel->Fill(invMasskaon);
        }
        else if (pT >= ptbin6l && pT < ptbin6u) {
            h6K0ptrel->Fill(invMasskaon);
        }
        else if (pT >= ptbin7l && pT < ptbin7u) {
            h7K0ptrel->Fill(invMasskaon);
        }
        else if (pT >= ptbin8l && pT < ptbin8u) {
            h8K0ptrel->Fill(invMasskaon);
        }
        else if (pT >= ptbin9l && pT < ptbin9u) {
            h9K0ptrel->Fill(invMasskaon);
        }*/
        
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Cut on Track Length of proton and pion
        if (PosPionCFTrack->GetLength() < 15 || NegPionCFTrack->GetLength() < 15) { continue; }
        hK0tracklength->Fill(invMasskaon);
        //Fill pT bins after track length cut
        if (pT >= ptbin1l && pT < ptbin1u) {
            h1K0tracklength->Fill(invMasskaon);
        }
        else if (pT >= ptbin2l && pT < ptbin2u) {
            h2K0tracklength->Fill(invMasskaon);
        }
        else if (pT >= ptbin3l && pT < ptbin3u) {
            h3K0tracklength->Fill(invMasskaon);
        }
        else if (pT >= ptbin4l && pT < ptbin4u) {
            h4K0tracklength->Fill(invMasskaon);
        }
        else if (pT >= ptbin5l && pT < ptbin5u) {
            h5K0tracklength->Fill(invMasskaon);
        }
        else if (pT >= ptbin6l && pT < ptbin6u) {
            h6K0tracklength->Fill(invMasskaon);
        }
        else if (pT >= ptbin7l && pT < ptbin7u) {
            h7K0tracklength->Fill(invMasskaon);
        }
        else if (pT >= ptbin8l && pT < ptbin8u) {
            h8K0tracklength->Fill(invMasskaon);
        }
        else if (pT >= ptbin9l && pT < ptbin9u) {
            h9K0tracklength->Fill(invMasskaon);
        }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

        if (invMasslambdaCont > 1.108 && invMasslambdaCont < 1.125) { continue;}
        hK0LambdaCont->Fill(invMasskaon);
        //Fill pT bins after K0 contmination cut
        if (pT >= ptbin1l && pT < ptbin1u) {
            h1K0LambdaCont->Fill(invMasskaon);
        }
        else if (pT >= ptbin2l && pT < ptbin2u) {
            h2K0LambdaCont->Fill(invMasskaon);
        }
        else if (pT >= ptbin3l && pT < ptbin3u) {
            h3K0LambdaCont->Fill(invMasskaon);
        }
        else if (pT >= ptbin4l && pT < ptbin4u) {
            h4K0LambdaCont->Fill(invMasskaon);
        }
        else if (pT >= ptbin5l && pT < ptbin5u) {
            h5K0LambdaCont->Fill(invMasskaon);
        }
        else if (pT >= ptbin6l && pT < ptbin6u) {
            h6K0LambdaCont->Fill(invMasskaon);
        }
        else if (pT >= ptbin7l && pT < ptbin7u) {
            h7K0LambdaCont->Fill(invMasskaon);
        }
        else if (pT >= ptbin8l && pT < ptbin8u) {
            h8K0LambdaCont->Fill(invMasskaon);
        }
        else if (pT >= ptbin9l && pT < ptbin9u) {
            h9K0LambdaCont->Fill(invMasskaon);
        }
////////////////////////////////////////////////////////////////////////////////////////////////////////

////////Get Raw pT and Eta distributions before implementing those cuts/////////////////////////////////////////////////////////////////////////////
        if (invMasskaon > kaonlowerbound && invMasskaon < kaonupperbound) {
            //Fill in K0 Candidate Distributions vs kinematic variables

                hK0VsBjorkenX->Fill((*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4));
                hK0VspT->Fill(pT);
                hK0VsEta->Fill(K0Cand[loop]->GetFourVector().Vect().Eta());
                hK0VsQ2->Fill(*q2e);
                hK0Vsy->Fill(*ye);
                hK0Vsphi->Fill(K0Cand[loop]->GetFourVector().Vect().Phi());
        }



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Final Kinematic cuts on pT and Eta now that distributions are filled
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Eta Cut
        if (abs(K0Cand[loop]->GetFourVector().Vect().Eta()) > 1.3) { continue; }
        hK0Eta->Fill(invMasskaon);
        //Fill pT bins after eta cut
        if (pT >= ptbin1l && pT < ptbin1u) {
            h1K0Eta->Fill(invMasskaon);
        }
        else if (pT >= ptbin2l && pT < ptbin2u) {
            h2K0Eta->Fill(invMasskaon);
        }
        else if (pT >= ptbin3l && pT < ptbin3u) {
            h3K0Eta->Fill(invMasskaon);
        }
        else if (pT >= ptbin4l && pT < ptbin4u) {
            h4K0Eta->Fill(invMasskaon);
        }
        else if (pT >= ptbin5l && pT < ptbin5u) {
            h5K0Eta->Fill(invMasskaon);
        }
        else if (pT >= ptbin6l && pT < ptbin6u) {
            h6K0Eta->Fill(invMasskaon);
        }
        else if (pT >= ptbin7l && pT < ptbin7u) {
            h7K0Eta->Fill(invMasskaon);
        }
        else if (pT >= ptbin8l && pT < ptbin8u) {
            h8K0Eta->Fill(invMasskaon);
        }
        else if (pT >= ptbin9l && pT < ptbin9u) {
            h9K0Eta->Fill(invMasskaon);
        }

        //pT Cut
        if (pT <= 0.5 || pT >= 3.5) { continue; }
        ////Cut on Pt of v0 mother candidate, want pt > 0.5
       //if (pT < 0.5 || pT > ptbin9u) continue;
       //hK0pt->Fill(invMasskaon);
       ////Fill pT bins after Cut on Pt of v0 mother candidate
       //if (pT >= ptbin1l && pT < ptbin1u) {
       //    h1K0pt->Fill(invMasskaon);
       //}
       //else if (pT >= ptbin2l && pT < ptbin2u) {
       //    h2K0pt->Fill(invMasskaon);
       //}
       //else if (pT >= ptbin3l && pT < ptbin3u) {
       //    h3K0pt->Fill(invMasskaon);
       //}
       //else if (pT >= ptbin4l && pT < ptbin4u) {
       //    h4K0pt->Fill(invMasskaon);
       //}
       //else if (pT >= ptbin5l && pT < ptbin5u) {
       //    h5K0pt->Fill(invMasskaon);
       //}
       //else if (pT >= ptbin6l && pT < ptbin6u) {
       //    h6K0pt->Fill(invMasskaon);
       //}
       //else if (pT >= ptbin7l && pT < ptbin7u) {
       //    h7K0pt->Fill(invMasskaon);
       //}
       //else if (pT >= ptbin8l && pT < ptbin8u) {
       //    h8K0pt->Fill(invMasskaon);
       //}
       //else if (pT >= ptbin9l && pT < ptbin9u) {
       //    h9K0pt->Fill(invMasskaon);
       //}


        //Fill plots for after all cuts
        hAPafter->Fill(alpha, pTrel);
        hdEdxBothAfterAllcuts->Fill(PosPionCFTrack->GetMomentum().Mag(), PosPionCFTrack->GetCorrectedDedx());
        hdEdxBothAfterAllcuts->Fill(NegPionCFTrack->GetMomentum().Mag(), NegPionCFTrack->GetCorrectedDedx());

        //After All necessary cuts, now can fill different pt bins
        if (pT >= ptbin1l && pT < ptbin9u) {
            hKaonWholepTrange->Fill(invMasskaon);
        }
        if (pT >= ptbin1l && pT < ptbin1u) {
            hK01->Fill(invMasskaon);
        }
        if (pT >= ptbin2l && pT < ptbin2u) {
            hK02->Fill(invMasskaon);
        }
        if (pT >= ptbin3l && pT < ptbin3u) {
            hK03->Fill(invMasskaon);
        }
        if (pT >= ptbin4l && pT < ptbin4u) {
            hK04->Fill(invMasskaon);
        }
        if (pT >= ptbin5l && pT < ptbin5u) {
            hK05->Fill(invMasskaon);
        }
        if (pT >= ptbin6l && pT < ptbin6u) {
            hK06->Fill(invMasskaon);
        }
        if (pT >= ptbin7l && pT < ptbin7u) {
            hK07->Fill(invMasskaon);
        }
        if (pT >= ptbin8l && pT < ptbin8u) {
            hK08->Fill(invMasskaon);
        }
        if (pT >= ptbin9l && pT < ptbin9u) {
            hK09->Fill(invMasskaon);
        } 
        
        ///////////////////////////////////////////////////////////////
        //Fill Plots for Distributions here Later


        ////////////////////////////////////////////////////////////


        //LAMBDA C 
        for (Int_t loop2 = 0; loop2 < PST.GetEntries(); loop2++) {

            if (PST[loop2]->IsFromPrimary() != true) { continue; }

            Double_t invMasslambdaC = (PST[loop2]->GetFourVector(mProton)
                + K0Cand[loop]->GetFourVector()).M();

            if (invMasslambdaC < 2.05 || invMasslambdaC > 2.55) { continue; }

            if (invMasskaon < kaonlowerbound || invMasskaon > kaonupperbound) { continue; }

            hC1->Fill(invMasslambdaC);

            hdEdxProtonCBefore->Fill(PST[loop2]->GetMomentum().Mag(), PST[loop2]->GetDedx());
            hDedxLprotonC->Fill(PST[loop2]->GetDedxLikelihood(H1Dedx::kProton));

            if (PST[loop2]->GetDedxLikelihood(H1Dedx::kProton) < 0.001) { continue; }

            hdEdxProtonCAfter->Fill(PST[loop2]->GetMomentum().Mag(), PST[loop2]->GetDedx());

            hC2->Fill(invMasslambdaC);

            

            if (PST[loop2]->GetRadLength() < 10) { continue; }

            hC3->Fill(invMasslambdaC);

            if (PST[loop2]->GetPt() < 0.2) { continue; }

            hCfinal->Fill(invMasslambdaC);
            hCfinalbw1->Fill(invMasslambdaC);
            hCfinalbw2->Fill(invMasslambdaC);
            hCfinalbw3->Fill(invMasslambdaC);
            hCfinalbw4->Fill(invMasslambdaC);
            hCfinalbw5->Fill(invMasslambdaC);
            hCfinalbw6->Fill(invMasslambdaC);
            hCfinalbw7->Fill(invMasslambdaC);
            hCfinalbw8->Fill(invMasslambdaC);
            hCfinalbw9->Fill(invMasslambdaC);
            hCfinalbw10->Fill(invMasslambdaC);
            hCfinalbw11->Fill(invMasslambdaC);

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
 
       TFile file(output.c_str(), "RECREATE");
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

       hKaonWholepTrange->SetStats(0);
       hKaonWholepTrange->SetLineColor(kBlack);
       hKaonWholepTrange->GetXaxis()->SetTitle("Mass [Gev]");
       hKaonWholepTrange->GetYaxis()->SetTitle("Count");
       hKaonWholepTrange->Draw();
       hKaonWholepTrange->Write();

       hAPbefore->SetStats(0);
       hAPbefore->GetXaxis()->SetTitle("\\alpha = (p^{+}_{L} - p^{-}_{L})/(p^{+}_{L} + p^{-}_{L})");
       hAPbefore->GetYaxis()->SetTitle("p_{T} [GeV]");
       hAPbefore->GetXaxis()->SetTitleSize(0.04);
       hAPbefore->GetXaxis()->SetTitleOffset(1);
       hAPbefore->GetYaxis()->SetTitleSize(0.05);
       hAPbefore->Draw("colz");
       hAPbefore->Write();
       gPad->Update();
       //canvas->SaveAs("kaonAPbefore.root");
       canvas->SetName("K0 AP Before");
       canvas->Write();

       hAPafter->SetStats(0);
       hAPafter->GetXaxis()->SetTitle("\\alpha = (p^{+}_{L} - p^{-}_{L})/(p^{+}_{L} + p^{-}_{L})");
       hAPafter->GetYaxis()->SetTitle("p_{T} [GeV]");
       hAPafter->GetXaxis()->SetTitleSize(0.04);
       hAPafter->GetXaxis()->SetTitleOffset(1);
       hAPafter->GetYaxis()->SetTitleSize(0.05);
       hAPafter->Draw("colz");
       hAPafter->Write();
       gPad->Update();
       //canvas->SaveAs("kaonAPafter.root");
       canvas->SetName("K0 AP After");
       canvas->Write();

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

       hpionDCAvsdDCAbefore->SetStats(0);
       hpionDCAvsdDCAbefore->GetXaxis()->SetTitle("dDCA");
       hpionDCAvsdDCAbefore->GetYaxis()->SetTitle("DCA");
       hpionDCAvsdDCAbefore->Draw("colz");
       hpionDCAvsdDCAbefore->Write();
       gPad->Update();
       //canvas->SaveAs("kaonpionDCAvsDCAbefore.root");
       canvas->SetName("Pion DCA vs dDCA Before");
       canvas->Write();

       hpionDCAvsdDCAafter->SetStats(0);
       hpionDCAvsdDCAafter->GetXaxis()->SetTitle("dDCA");
       hpionDCAvsdDCAafter->GetYaxis()->SetTitle("DCA");
       hpionDCAvsdDCAafter->Draw("colz");
       hpionDCAvsdDCAafter->Write();
       gPad->Update();
       //canvas->SaveAs("kaonpionDCAvsDCAafter.root");
       canvas->SetName("Pion DCA vs dDCA After");
       canvas->Write();

       hpionDCAvspTbefore->SetStats(0);
       hpionDCAvspTbefore->GetXaxis()->SetTitle("pT");
       hpionDCAvspTbefore->GetYaxis()->SetTitle("DCA");
       hpionDCAvspTbefore->Draw("colz");
       hpionDCAvspTbefore->Write();
       gPad->Update();
       //canvas->SaveAs("kaonpionDCAvspTbefore.root");
       canvas->SetName("Pion DCA vs pT Before");
       canvas->Write();

       hpiondDCAvspTbefore->SetStats(0);
       hpiondDCAvspTbefore->GetXaxis()->SetTitle("pT");
       hpiondDCAvspTbefore->GetYaxis()->SetTitle("dDCA");
       hpiondDCAvspTbefore->Draw("colz");
       hpiondDCAvspTbefore->Write();
       gPad->Update();
       //canvas->SaveAs("kaonpiondDCAvspTbefore.root");
       canvas->SetName("Pion dDCA vs pT Before");
       canvas->Write();

       hdEdxBothBefore->SetStats(0);
       hdEdxBothBefore->GetXaxis()->SetTitle("p [GeV]");
       hdEdxBothBefore->GetYaxis()->SetTitle("dE/dx");
       hdEdxBothBefore->Draw("colz");
       hdEdxBothBefore->Write();
       gPad->Update();
       //canvas->SaveAs("kaondEdxBothBefore.root");
       canvas->SetName("dE/dx Both Daughter Before");
       canvas->Write();

       hdEdxBothAfterLcut->SetStats(0);
       hdEdxBothAfterLcut->GetXaxis()->SetTitle("p [GeV]");
       hdEdxBothAfterLcut->GetYaxis()->SetTitle("dE/dx");
       hdEdxBothAfterLcut->Draw("colz");
       hdEdxBothAfterLcut->Write();
       gPad->Update();
       //canvas->SaveAs("kaondEdxBothAfterLcut.root");
       canvas->SetName("dE/dx Both Daughters After Likelihood Cut");
       canvas->Write();

       hdEdxBothAfterAllcuts->SetStats(0);
       hdEdxBothAfterAllcuts->GetXaxis()->SetTitle("p [GeV]");
       hdEdxBothAfterAllcuts->GetYaxis()->SetTitle("dE/dx");
       hdEdxBothAfterAllcuts->Draw("colz");
       hdEdxBothAfterAllcuts->Write();
       gPad->Update();
       //canvas->SaveAs("kaondEdxBothAfterAllcuts.root");
       canvas->SetName("dE/dx Both Daughters After All Cuts");
       canvas->Write();

       hdEdxProtonCBefore->SetStats(0);
       hdEdxProtonCBefore->GetXaxis()->SetTitle("p [GeV]");
       hdEdxProtonCBefore->GetYaxis()->SetTitle("dE/dx");
       hdEdxProtonCBefore->Draw("colz");
       canvas->SetName("dE/dx Charm Proton After");
       canvas->Write();

       hdEdxProtonCAfter->SetStats(0);
       hdEdxProtonCAfter->GetXaxis()->SetTitle("p [GeV]");
       hdEdxProtonCAfter->GetYaxis()->SetTitle("dE/dx");
       hdEdxProtonCAfter->Draw("colz");
       canvas->SetName("dE/dx Charm Proton Before");
       canvas->Write();

       hDedxLprotonC->SetStats(0);
       hDedxLprotonC->SetFillColor(kRed);
       hDedxLprotonC->GetXaxis()->SetTitle("Likelihood");
       hDedxLprotonC->GetYaxis()->SetTitle("Count");
       hDedxLprotonC->Draw();
       hDedxLprotonC->Write();

       //Writing the Distributions
       hK0VsBjorkenX->SetStats(0);
       hK0VsBjorkenX->SetLineColor(kBlack);
       hK0VsBjorkenX->GetXaxis()->SetTitle("Bjorken X");
       hK0VsBjorkenX->GetYaxis()->SetTitle("Count");
       hK0VsBjorkenX->Draw();
       hK0VsBjorkenX->Write();

       hK0VspT->SetStats(0);
       hK0VspT->SetLineColor(kBlack);
       hK0VspT->GetXaxis()->SetTitle("pT [GeV]");
       hK0VspT->GetYaxis()->SetTitle("Count");
       hK0VspT->Draw();
       hK0VspT->Write();

       hK0VsEta->SetStats(0);
       hK0VsEta->SetLineColor(kBlack);
       hK0VsEta->GetXaxis()->SetTitle("Eta");
       hK0VsEta->GetYaxis()->SetTitle("Count");
       hK0VsEta->Draw();
       hK0VsEta->Write();

       hK0VsQ2->SetStats(0);
       hK0VsQ2->SetLineColor(kBlack);
       hK0VsQ2->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
       hK0VsQ2->GetYaxis()->SetTitle("Count");
       hK0VsQ2->Draw();
       hK0VsQ2->Write();

       hK0Vsy->SetStats(0);
       hK0Vsy->SetLineColor(kBlack);
       hK0Vsy->GetXaxis()->SetTitle("y");
       hK0Vsy->GetYaxis()->SetTitle("Count");
       hK0Vsy->Draw();
       hK0Vsy->Write();

       hK0Vsphi->SetStats(0);
       hK0Vsphi->SetLineColor(kBlack);
       hK0Vsphi->GetXaxis()->SetTitle("y");
       hK0Vsphi->GetYaxis()->SetTitle("Count");
       hK0Vsphi->Draw();
       hK0Vsphi->Write();



       hC1->SetStats(0);
       hC1->SetLineColor(kBlack);
       hC1->GetXaxis()->SetTitle("Mass [Gev]");
       hC1->GetYaxis()->SetTitle("Count");
       hC1->Draw();
       hC1->Write();

       hC2->SetStats(0);
       hC2->SetLineColor(kMagenta);
       hC2->GetXaxis()->SetTitle("Mass [Gev]");
       hC2->GetYaxis()->SetTitle("Count");
       hC2->Draw();
       hC2->Write();

       hC3->SetStats(0);
       hC3->SetLineColor(kRed);
       hC3->GetXaxis()->SetTitle("Mass [Gev]");
       hC3->GetYaxis()->SetTitle("Count");
       hC3->Draw();
       hC3->Write();

       hC4->SetStats(0);
       hC4->SetLineColor(kBlue);
       hC4->GetXaxis()->SetTitle("Mass [Gev]");
       hC4->GetYaxis()->SetTitle("Count");
       hC4->Draw();
       hC4->Write();

       hC5->SetStats(0);
       hC5->SetLineColor(kGreen);
       hC5->GetXaxis()->SetTitle("Mass [Gev]");
       hC5->GetYaxis()->SetTitle("Count");
       hC5->Draw();
       hC5->Write();

       hC6->SetStats(0);
       hC6->SetLineColor(kMagenta);
       hC6->GetXaxis()->SetTitle("Mass [Gev]");
       hC6->GetYaxis()->SetTitle("Count");
       hC6->Draw();
       hC6->Write();

       hC7->SetStats(0);
       hC7->SetLineColor(kMagenta);
       hC7->GetXaxis()->SetTitle("Mass [Gev]");
       hC7->GetYaxis()->SetTitle("Count");
       hC7->Draw();
       hC7->Write();

       hC8->SetStats(0);
       hC8->SetLineColor(kMagenta);
       hC8->GetXaxis()->SetTitle("Mass [Gev]");
       hC8->GetYaxis()->SetTitle("Count");
       hC8->Draw();
       hC8->Write();

       hCfinal->SetStats(0);
       hCfinal->SetLineColor(kBlack);
       hCfinal->GetXaxis()->SetTitle("Mass [Gev]");
       hCfinal->GetYaxis()->SetTitle("Count");
       hCfinal->Draw();
       hCfinal->Write();

       hCfinalbw1->SetStats(0);
       hCfinalbw1->SetLineColor(kBlack);
       hCfinalbw1->GetXaxis()->SetTitle("Mass [Gev]");
       hCfinalbw1->GetYaxis()->SetTitle("Count");
       hCfinalbw1->Draw();
       hCfinalbw1->Write();

       hCfinalbw2->SetStats(0);
       hCfinalbw2->SetLineColor(kBlack);
       hCfinalbw2->GetXaxis()->SetTitle("Mass [Gev]");
       hCfinalbw2->GetYaxis()->SetTitle("Count");
       hCfinalbw2->Draw();
       hCfinalbw2->Write();

       hCfinalbw3->SetStats(0);
       hCfinalbw3->SetLineColor(kBlack);
       hCfinalbw3->GetXaxis()->SetTitle("Mass [Gev]");
       hCfinalbw3->GetYaxis()->SetTitle("Count");
       hCfinalbw3->Draw();
       hCfinalbw3->Write();

       hCfinalbw4->SetStats(0);
       hCfinalbw4->SetLineColor(kBlack);
       hCfinalbw4->GetXaxis()->SetTitle("Mass [Gev]");
       hCfinalbw4->GetYaxis()->SetTitle("Count");
       hCfinalbw4->Draw();
       hCfinalbw4->Write();

       hCfinalbw5->SetStats(0);
       hCfinalbw5->SetLineColor(kBlack);
       hCfinalbw5->GetXaxis()->SetTitle("Mass [Gev]");
       hCfinalbw5->GetYaxis()->SetTitle("Count");
       hCfinalbw5->Draw();
       hCfinalbw5->Write();

       hCfinalbw6->SetStats(0);
       hCfinalbw6->SetLineColor(kBlack);
       hCfinalbw6->GetXaxis()->SetTitle("Mass [Gev]");
       hCfinalbw6->GetYaxis()->SetTitle("Count");
       hCfinalbw6->Draw();
       hCfinalbw6->Write();

       hCfinalbw7->SetStats(0);
       hCfinalbw7->SetLineColor(kBlack);
       hCfinalbw7->GetXaxis()->SetTitle("Mass [Gev]");
       hCfinalbw7->GetYaxis()->SetTitle("Count");
       hCfinalbw7->Draw();
       hCfinalbw7->Write();

       hCfinalbw8->SetStats(0);
       hCfinalbw8->SetLineColor(kBlack);
       hCfinalbw8->GetXaxis()->SetTitle("Mass [Gev]");
       hCfinalbw8->GetYaxis()->SetTitle("Count");
       hCfinalbw8->Draw();
       hCfinalbw8->Write();

       hCfinalbw9->SetStats(0);
       hCfinalbw9->SetLineColor(kBlack);
       hCfinalbw9->GetXaxis()->SetTitle("Mass [Gev]");
       hCfinalbw9->GetYaxis()->SetTitle("Count");
       hCfinalbw9->Draw();
       hCfinalbw9->Write();

       hCfinalbw10->SetStats(0);
       hCfinalbw10->SetLineColor(kBlack);
       hCfinalbw10->GetXaxis()->SetTitle("Mass [Gev]");
       hCfinalbw10->GetYaxis()->SetTitle("Count");
       hCfinalbw10->Draw();
       hCfinalbw10->Write();

       hCfinalbw11->SetStats(0);
       hCfinalbw11->SetLineColor(kBlack);
       hCfinalbw11->GetXaxis()->SetTitle("Mass [Gev]");
       hCfinalbw11->GetYaxis()->SetTitle("Count");
       hCfinalbw11->Draw();
       hCfinalbw11->Write();



//Writing the Stacks
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
       //canvas->SaveAs("kaonstack1.root");
       canvas->SetName("Kaon Stack 1");
       canvas->Write();

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
       //canvas->SaveAs("kaonstack2.root");
       canvas->SetName("Kaon Stack 2");
       canvas->Write();

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
       //canvas->SaveAs("kaonstack3.root");
       canvas->SetName("Kaon Stack 3");
       canvas->Write();

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
       //canvas->SaveAs("kaonstack4.root");
       canvas->SetName("Kaon Stack 4");
       canvas->Write();

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
       //canvas->SaveAs("kaonstack5.root");
       canvas->SetName("Kaon Stack 5");
       canvas->Write();

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
       //canvas->SaveAs("kaonstack6.root");
       canvas->SetName("Kaon Stack 6");
       canvas->Write();

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
       //canvas->SaveAs("kaonstack7.root");
       canvas->SetName("Kaon Stack 7");
       canvas->Write();

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
       //canvas->SaveAs("kaonstack8.root");
       canvas->SetName("Kaon Stack 8");
       canvas->Write();

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
       //canvas->SaveAs("kaonstack9.root");
       canvas->SetName("Kaon Stack 9");
       canvas->Write();

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
       //canvas->SaveAs("kaonstack.root");
       canvas->SetName("Kaon Stack");
       canvas->Write();

       hsC->Add(hC1);
       hsC->Add(hC2);
       hsC->Add(hC3);
       hsC->Add(hC4);
       hsC->Add(hC5);
       hsC->Add(hC6);
       hsC->Add(hC7);
       hsC->Add(hC8);
       hsC->Add(hCfinal);
       hsC->Draw("nostack");
       canvas->SetName("LambdaC Cut Stack");
       canvas->Write();


       file.Write();
       file.Close();
       cout << "Histograms written to " << opts.GetOutput() << endl;


        return 0;
    }