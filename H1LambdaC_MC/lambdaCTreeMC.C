///////////////////////////////////////////////////////
// 
// Lambda Finder 
// Additional Selections building on H1FindLambda.C selections
// 
// Author     : gtustin
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
    //string output = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/lambda.root";
    // event counter
    Int_t eventCounter = 0;
    // parse the command line
    H1StdCmdLine opts;
    opts.Parse(&argc, argv);
    // needed for graphic, but MUST be AFTER Parse(...):
    //TApplication theApp("kaonfind_ods", &argc, argv);

    // Load mODS/HAT files
    H1Tree::Instance()->Open();            // this statement must be here

    // Histograms Binning
    Double_t lambdabins = 1000;
    Double_t lambdaupperbinrange = 1.5;
    Double_t lambdalowerbinrange = 1.0;

    Double_t lambdaCbins = 50;
    Double_t lambdaCupperbinrange = 3.317;
    Double_t lambdaClowerbinrange = 2.017;
    
    // load in some HAT data
    H1FloatPtr q2e("Q2e");
    H1FloatPtr ebeamE("EBeamE");
    H1FloatPtr ebeamP("EBeamP");
    H1FloatPtr ye("Ye");

    //Armenteros-Podolansky plot for charmed lambdas
    TH2F* hAPbeforeC = new TH2F("Charm AP", "Charm Armenteros-Podolansky Plot Before pT_rel Cut", 200, -1, 1, 500, 0, 1);
    TH2F* hAPafterC = new TH2F("Charm AP", "Charm Armenteros-Podolansky Plot Before pT_rel Cut", 200, -1, 1, 500, 0, 1);

    TH2D* hdEdxPionCBefore = new TH2D("LambdaC Pion dEdxvP Before", "LambdaC Pion dEdxvP Before", 500, 0, 2, 500, 0, 10);
    TH2D* hdEdxPionCAfter = new TH2D("LambdaC Pion dEdxvP After", "LambdaC Pion dEdxvP After", 500, 0, 2, 500, 0, 10);

    TH2F* pXYvtxdensitytot = new TH2F("Total Lambda XY Vertices", "Total Lambda XY Vertices", 200, -10, 10, 200, -10, 10);
    TH2F* pZXvtxdensitytot = new TH2F("Total Lambda ZX Vertices", "Total Lambda ZX Vertices", 500, -40, 40, 200, -10, 10);
    TH2F* pZYvtxdensitytot = new TH2F("Total Lambda ZY Vertices", "Total Lambda ZY Vertices", 500, -40, 40, 200, -10, 10);
    TH1F* pdecaylengthmag = new TH1F("Lambda Decay Length Magnitude", "Lambda Decay Length Magnitude", 200, 0, 20);

   

    //Defining TTrees to store Lambda, Lambda Bar, and Combined Lambda candidates and their associated kinematic variables
    TTree* lambdaRegTree = new TTree("Lambda Reg", "Lambda Reg");
    TTree* lambdaBarTree = new TTree("Lambda Bar", "Lambda Bar");
    TTree* lambdaTotTree = new TTree("Lambda Tot", "Lambda Tot");
    //defining the variables I want to store in Lambda Trees
    double pT, Eta, x, invMasslambda, Q2e, Ye;
    //Defining Tree Branches
    lambdaRegTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaRegTree->Branch("pT",&pT,"pT/D");
    lambdaRegTree->Branch("Eta", &Eta, "Eta/D");
    lambdaRegTree->Branch("x", &x, "x/D");
    lambdaRegTree->Branch("Q2e", &Q2e, "Q2e/D");
    lambdaRegTree->Branch("Ye", &Ye, "Ye/D");

    lambdaBarTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaBarTree->Branch("pT", &pT, "pT/D");
    lambdaBarTree->Branch("Eta", &Eta, "Eta/D");
    lambdaBarTree->Branch("x", &x, "x/D");
    lambdaBarTree->Branch("Q2e", &Q2e, "Q2e/D");
    lambdaBarTree->Branch("Ye", &Ye, "Ye/D");

    lambdaTotTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaTotTree->Branch("pT", &pT, "pT/D");
    lambdaTotTree->Branch("Eta", &Eta, "Eta/D");
    lambdaTotTree->Branch("x", &x, "x/D");
    lambdaTotTree->Branch("Q2e", &Q2e, "Q2e/D");
    lambdaTotTree->Branch("Ye", &Ye, "Ye/D");
    
    TTree* lambdaCTree = new TTree("LambdaC", "LambdaC");

    double invMasslambdaC;
    lambdaCTree->Branch("invMasslambdaC", &invMasslambdaC, "invMasslambdaC/D");

    //////////////////////////////////////////////////////////////////////////
    // Defining Cut Progression Histograms

    THStack* hs = new THStack("hs", "Stacked Total Lambda Mass Plots");
    THStack* hsC = new THStack("hsC", "Stacked Total LambdaC Mass Plots");

    TH1F* LambdaCutProgression[9];
    for (int a = 0; a < 9; a++) {
        string lambdacutprogstring = "Lambda Cut " + to_string(a);
        LambdaCutProgression[a] = new TH1F(lambdacutprogstring.c_str(), lambdacutprogstring.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    }

    TH1F* LambdaCutTest1 = new TH1F("Lambda Cut Test 1", "Lambda Cut Test 1", lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
    TH1F* LambdaCutTest2 = new TH1F("Lambda Cut Test 2", "Lambda Cut Test 2", lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
    TH1F* LambdaCutTest3 = new TH1F("Lambda Cut Test 3", "Lambda Cut Test 3", lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
    TH1F* LambdaCutTest4 = new TH1F("Lambda Cut Test 4", "Lambda Cut Test 4", lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
    TH1F* LambdaCutTest5 = new TH1F("Lambda Cut Test 5", "Lambda Cut Test 5", lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
    TH1F* LambdaCutTest6 = new TH1F("Lambda Cut Test 6", "Lambda Cut Test 6", lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
    TH1F* LambdaCutTest7 = new TH1F("Lambda Cut Test 7", "Lambda Cut Test 7", lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
    TH1F* LambdaCutTest8 = new TH1F("Lambda Cut Test 8", "Lambda Cut Test 8", lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
    TH1F* LambdaCutTest9 = new TH1F("Lambda Cut Test 9", "Lambda Cut Test 9", lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
    TH1F* LambdaCutTest10 = new TH1F("Lambda Cut Test 10", "Lambda Cut Test 10", lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);

    TH1F* LambdaCCutProgression[20];
    for (int a = 0; a < 20; a++) {
        string lambdaCcutprogstring = "LambdaC Cut " + to_string(a);
        LambdaCCutProgression[a] = new TH1F(lambdaCcutprogstring.c_str(), lambdaCcutprogstring.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
    }

    //Defining arrays of histograms for testing cuts on LambdaC
    TH1F* LambdaInvMassCutTest[20];
    TH1F* PionDedxCutTest[20];
    TH1F* RadLengthPionTrackCutTest[20];
    TH1F* LambdaCpTCutTest[20];
    //TH1F* LambdaFromPrimCutTest[20];
    TH1F* pTrelLowerBoundCutTest[20];
    TH1F* pTrelUpperBoundCutTest[20];
    TH1F* ZvtxCutTest[20];
    TH1F* RvtxCutTest[20];
    TH1F* AlphaCutTest[20];
    TH1F* EtaCutTest[20];
    TH1F* DCApionCutTest[20];

    

    //Cut values should go from least severe to most severe
    double cutvalues1[20] = {0.085,0.080,0.075,0.070,0.065,0.060,0.055,0.050,0.045,0.040,0.035,0.030,0.025,0.02,0.015,0.01,0.005,0.004,0.003,0.002}; //abs(lambdainvMass-1.11568)<CutValue
    double cutvalues2[20] = { 0.010,0.02,0.03,0.04,0.05,0.06,0.07,0.08,0.09,0.1,0.11,0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.19,0.2 };
    double cutvalues3[20] = {10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29};
    double cutvalues4[20] = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0};
    //double cutvalues5[20] = {};
    double cutvalues6[11] = {0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6};
    //double cutvalues7[20] = {0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95};
    double cutvalues7[8] = { 0.95,0.9,0.85,0.8,0.75,0.7,0.65,0.6 };
    double cutvalues8[20] = {35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16};
    double cutvalues9[20] = { 35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16 };
    //double cutvalues10[20] = {-0.4,-0.35,-0.3,-0.25,-0.2,-0.15,-0.1,-0.05,0.0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.40,0.45,0.50,0.55};
    double cutvalues10[20] = {0.9,0.85,0.80,0.75,0.70,0.65,0.6,0.55,0.50,0.45,0.40,0.35,0.3,0.25,0.2,0.15,0.1,0.05,0,-0.1};
    double cutvalues11[11] = {2.0,1.9,1.8,1.7,1.6,1.5,1.4,1.3,1.2,1.1,1.0};
    //double cutvalues12[20] = {0.5,1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8,8.5,9,9.5,10};
    //double cutvalues12[20] = { 6.0,5.5,5.0,4.5,4.0,3.5,3.0,2.5,2.0,1.5,1.0,0.5,0.4,0.3,0.2,0.1,0.05,0.04,0.03,0.02 };
    double cutvalues12[20] = {1.0,0.95,0.9,0.85,0.8,0.75,0.7,0.65,0.6,0.55,0.5,0.45,0.4,0.35,0.3,0.25,0.2,0.15,0.1,0.05};

    //Round 2 of cut tests
    TH1F* pionpTCutTest[20];
    TH1F* pionEtaCutTest[20];
    TH1F* lambdaSDCApionCutTest[20];
    TH1F* lambdaSDCAprotonCutTest[20];
    TH1F* lambdaEtaCutTest[20];
    TH1F* lambdapTCutTest[20];
    double cutvalues13[20] = {0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.0,3.2,3.4,3.6,3.8,4.0};
    double cutvalues14[20] = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0};
    double cutvalues15[20] = {0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.0,3.2,3.4,3.6,3.8,4.0};
    double cutvalues16[20] = {0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.0,3.2,3.4,3.6,3.8,4.0};
    //double cutvalues15[20] = {0.02,0.04,0.06,0.08,0.1,0.12,0.14,0.16,0.18,0.20,0.22,0.24,0.26,0.28,0.30,0.32,0.34,0.36,0.38,0.4};
    //double cutvalues16[20] = {0.02,0.04,0.06,0.08,0.1,0.12,0.14,0.16,0.18,0.20,0.22,0.24,0.26,0.28,0.30,0.32,0.34,0.36,0.38,0.4};
    double cutvalues17[20] = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0};
    double cutvalues18[20] = {0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.0,3.2,3.4,3.6,3.8,4.0};


    for (int a = 0; a < 20; a++) {
        string cutstring1 = "Lambda Inv Mass Cut " + to_string(a+1);
        string cutstring2 = "Pion dEdx Cut " + to_string(a+1);
        string cutstring3 = "Pion Rad Track Length Cut " + to_string(a+1);
        string cutstring4 = "LambcaC pT Cut " + to_string(a+1);
        //string cutstring5 = "Lambda from Prim Cut " + to_string(a);
        string cutstring6 = "pTrel Lower Bound Cut " + to_string(a+1);
        string cutstring7 = "pTrel Upper Bound Cut " + to_string(a+1);
        string cutstring8 = "Z vtx Cut " + to_string(a+1);
        string cutstring9 = "R vtx Cut " + to_string(a+1);
        string cutstring10 = "Alpha Cut " + to_string(a+1);
        string cutstring11 = "Eta Cut " + to_string(a+1);
        string cutstring12 = "DCApion Cut " + to_string(a+1);

        string cutstring13 = "Pion pT Cut " + to_string(a + 1);
        string cutstring14 = "Pion Eta Cut " + to_string(a + 1);
        string cutstring15 = "SDCA Pion Cut " + to_string(a + 1);
        string cutstring16 = "SDCA Proton Cut " + to_string(a + 1);
        string cutstring17 = "Lambda Eta Cut " + to_string(a + 1);
        string cutstring18 = "Lambda pT Cut " + to_string(a + 1);

        LambdaInvMassCutTest[a] = new TH1F(cutstring1.c_str(), cutstring1.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
        PionDedxCutTest[a] = new TH1F(cutstring2.c_str(), cutstring2.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
        RadLengthPionTrackCutTest[a] = new TH1F(cutstring3.c_str(), cutstring3.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
        LambdaCpTCutTest[a] = new TH1F(cutstring4.c_str(), cutstring4.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
        //LambdaFromPrimCutTest[a] = new TH1F(cutstring5.c_str(), cutstring5.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
        pTrelLowerBoundCutTest[a] = new TH1F(cutstring6.c_str(), cutstring6.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
        pTrelUpperBoundCutTest[a] = new TH1F(cutstring7.c_str(), cutstring7.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
        ZvtxCutTest[a] = new TH1F(cutstring8.c_str(), cutstring8.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
        RvtxCutTest[a] = new TH1F(cutstring9.c_str(), cutstring9.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
        AlphaCutTest[a] = new TH1F(cutstring10.c_str(), cutstring10.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
        EtaCutTest[a] = new TH1F(cutstring11.c_str(), cutstring11.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
        DCApionCutTest[a] = new TH1F(cutstring12.c_str(), cutstring12.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
        
        pionpTCutTest[a] = new TH1F(cutstring13.c_str(), cutstring13.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
        pionEtaCutTest[a] = new TH1F(cutstring14.c_str(), cutstring14.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
        lambdaSDCApionCutTest[a] = new TH1F(cutstring15.c_str(), cutstring15.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
        lambdaSDCAprotonCutTest[a] = new TH1F(cutstring16.c_str(), cutstring16.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
        lambdaEtaCutTest[a] = new TH1F(cutstring17.c_str(), cutstring17.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
        lambdapTCutTest[a] = new TH1F(cutstring18.c_str(), cutstring18.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Defining Root Canvas to work on
    TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 800);

   //
   // 
    // Pointer to vertices to loop over
    static H1CentralVertexArrayPtr vertex;
    static H1PartLambdaArrayPtr LambdaCand;
    static H1PartSelTrackArrayPtr PST;
    
    
    // Get the pion's mass
    Double_t mPion = TDatabasePDG::Instance()->GetParticle(211)->Mass();
    // Get proton mass
    Double_t mProton = TDatabasePDG::Instance()->GetParticle(2212)->Mass();
    // Get electron mass
    Double_t mElectron = TDatabasePDG::Instance()->GetParticle(11)->Mass();
    // Get Lambda mass
    Double_t mLambda = TDatabasePDG::Instance()->GetParticle(3122)->Mass();

    ///////////////BEGIN MAIN LOOP////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    // Loop as long as there's data
    while (H1Tree::Instance()->Next() && !opts.IsMaxEvent(eventCounter)) {

        // Loop over all Lambda candidates on MODS level as determined by rough cuts in H1FindLambda code
        for (Int_t loop = 0; loop < LambdaCand.GetEntries(); loop++) {

        if (LambdaCand[loop]->GetNumOfParticles() < 2) continue;

        invMasslambda = (LambdaCand[loop]->GetProtonFourVector()
            + LambdaCand[loop]->GetPionFourVector()).M();

        H1SelVertex* ProtonVertex = (H1SelVertex*)LambdaCand[loop]->GetProtonTrack();
        const H1SelVertex* ProtonPrimaryVertex = ProtonVertex->GetPrimaryVertex();
        TVector3 pv = ProtonPrimaryVertex->GetPosition();//is same for both protons and pion daughters, so just using proton here and previous couple lines
        TVector2 V2PV(pv.X(), pv.Y());
        TVector3 sv = LambdaCand[loop]->GetV0()->GetDecayVertex()->GetPosition();
        double ProtonPosX = sv.X();
        double ProtonPosY = sv.Y();
        double ProtonPosZ = sv.Z();

        //if ((pv.Z() - ProtonPosZ) < -10 || (pv.Z() - ProtonPosZ) > 10) { continue; }

        //Getting Vectors for Armenteros-Podolansky Plot
        H1Track* ProtonH1Track = (H1Track*)LambdaCand[loop]->GetProtonTrack();
        H1Track* PionH1Track = (H1Track*)LambdaCand[loop]->GetPionTrack();

        TVector3 vecProton = ProtonH1Track->GetFourVector(mProton).Vect();
        TVector3 vecPion = PionH1Track->GetFourVector(mPion).Vect();
        TVector3 vecLambda = vecProton + vecPion;
        
        //Call H1PartLambda class data that references H1CentralFittedTrack for Pion and Proton
        const H1CentralFittedTrack* ProtonCFTrack = LambdaCand[loop]->GetProtonTrack();
        const H1CentralFittedTrack* PionCFTrack = LambdaCand[loop]->GetPionTrack();

        //Getting H1PartSelTracks for daughters
        const H1PartSelTrack* ProtonPSTrack = LambdaCand[loop]->GetProton();
        const H1PartSelTrack* PionPSTrack = LambdaCand[loop]->GetPion();

        //Prepating DCA/dDCA information to get a histogram to help visualize this DCA Significance cut
        const H1CentralTrack* protonFittedNoV = ProtonCFTrack->GetCentralTrack();
        const H1Trajectory* protonTrajectory = protonFittedNoV->GetTrajectory();

        const H1CentralTrack* pionFittedNoV = PionCFTrack->GetCentralTrack();
        const H1Trajectory* pionTrajectory = pionFittedNoV->GetTrajectory();

        //H1SelVertex* ProtonVertex = (H1SelVertex*)LambdaCand[loop]->GetProtonTrack();
        //const H1SelVertex* ProtonPrimaryVertex = ProtonVertex->GetPrimaryVertex();
        //TVector3 pv = ProtonPrimaryVertex->GetPosition();//is same for both protons and pion daughters, so just using proton here and previous couple lines
        //TVector2 V2PV(pv.X(), pv.Y());
        //TVector3 sv = LambdaCand[loop]->GetV0()->GetDecayVertex()->GetPosition();
        //double ProtonPosX = sv.X();
        //double ProtonPosY = sv.Y();
        //double ProtonPosZ = sv.Z();

        //Defining pT before cuts and putting stuff into pt bins
        const H1CentralFittedV0* v0 = LambdaCand[loop]->GetV0();
        pT = v0->GetPt();

        LambdaCutProgression[0]->Fill(invMasslambda); //Raw Lambda Candidates

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //PID Likelihood Cut for Proton
        if (ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton) < 0.001) { 
            continue;
        }

        LambdaCutProgression[1]->Fill(invMasslambda);
        
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //PID Likelihood Cut for Pion
        if (PionPSTrack->GetDedxLikelihood(H1Dedx::kPion) < 0.001) {
            continue; }

        LambdaCutProgression[2]->Fill(invMasslambda);
       

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////        
        //DCA over dDCA proton cut
        if (abs((protonTrajectory->GetDca(V2PV)) / (protonTrajectory->GetdDca(V2PV))) < 0.2) { //originally 1
            continue; }

        LambdaCutProgression[3]->Fill(invMasslambda);
        
        //Delete Below later
        //if (abs((protonTrajectory->GetDca(V2PV)) / (protonTrajectory->GetdDca(V2PV))) > 0.4) {
        //    LambdaCutProgression[3]->Fill(invMasslambda);
        //}

        //LambdaCutProgression[3]->Fill(invMasslambda);
        //Delete Above Later
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //DCA over dDCA pion cut
        if (abs((pionTrajectory->GetDca(V2PV)) / (pionTrajectory->GetdDca(V2PV))) < 0.2) { //originally 1
            continue; }

        LambdaCutProgression[4]->Fill(invMasslambda);

        //Delete Below Later
        //if (abs((pionTrajectory->GetDca(V2PV)) / (pionTrajectory->GetdDca(V2PV))) > 0.4) {
        //    LambdaCutProgression[4]->Fill(invMasslambda);
        //}
        //Delete Above Later
        
       
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Cut against K0 contamination
        Double_t invMasskaonCont = (ProtonH1Track->GetFourVector(mPion)
            + PionH1Track->GetFourVector(mPion)).M();
        if (invMasskaonCont > 0.465 && invMasskaonCont < 0.53) { 
            continue; }

        LambdaCutProgression[5]->Fill(invMasslambda);
        
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Cut against photon contamination
        Double_t invMassphotonCont = (ProtonH1Track->GetFourVector(mElectron)
            + PionH1Track->GetFourVector(mElectron)).M();
        if (invMassphotonCont < 0.05) { 
            continue; }

        LambdaCutProgression[6]->Fill(invMasslambda);
        
        /*if (abs(pv.Z() - ProtonPosZ) > 6.0) { LambdaCutTest1->Fill(invMasslambda); }
        if (abs(pv.Z() - ProtonPosZ) > 7.0) { LambdaCutTest2->Fill(invMasslambda); }
        if (abs(pv.Z() - ProtonPosZ) > 8.0) { LambdaCutTest3->Fill(invMasslambda); }
        if (abs(pv.Z() - ProtonPosZ) > 9.0) { LambdaCutTest4->Fill(invMasslambda); }
        if (abs(pv.Z() - ProtonPosZ) > 10.0) { LambdaCutTest5->Fill(invMasslambda); }
        if (abs(pv.Z() - ProtonPosZ) > 11.0) { LambdaCutTest6->Fill(invMasslambda); }
        if (abs(pv.Z() - ProtonPosZ) > 12.0) { LambdaCutTest7->Fill(invMasslambda); }
        if (abs(pv.Z() - ProtonPosZ) > 13.0) { LambdaCutTest8->Fill(invMasslambda); }
        if (abs(pv.Z() - ProtonPosZ) > 14.0) { LambdaCutTest9->Fill(invMasslambda); }*/
        


        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // STORE LAMBDA CANDIDATES INTO TTREES AFTER ALL CUTS EXCEPT THE PT AND ETA CUT HERE
        Eta = LambdaCand[loop]->GetFourVector().Vect().Eta();
        x = (*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4);
        Q2e = *q2e;
        Ye = *ye;

        lambdaTotTree->Fill();

        if (LambdaCand[loop]->IsLambdaBar() == true) { lambdaBarTree->Fill(); }
        if (LambdaCand[loop]->IsLambdaBar() != true) { lambdaRegTree->Fill(); }
    
            
        


        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //LAMBDA C 
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Now starting new loop over H1PartCandArray to grab protons coming out of ventral vertex to get LambdaC candidates
        for (Int_t loop2 = 0; loop2 < PST.GetEntries(); loop2++) {

            invMasslambdaC = (PST[loop2]->GetFourVector(mPion)
                + LambdaCand[loop]->GetFourVector()).M();

            LambdaCCutProgression[0]->Fill(invMasslambdaC);

            if (LambdaCand[loop]->IsLambdaBar() == true && PST[loop2]->GetCharge() > 0) { continue; }

            if (LambdaCand[loop]->IsLambdaBar() != true && PST[loop2]->GetCharge() < 0) { continue; }

            if (PST[loop2]->GetCharge() == 0) { continue; }

            LambdaCCutProgression[1]->Fill(invMasslambdaC);

            if (PST[loop2]->IsFromPrimary() != true) { continue; }

            LambdaCCutProgression[2]->Fill(invMasslambdaC);

            if (invMasslambdaC < 2.00 || invMasslambdaC > 2.8) { continue; }

            LambdaCCutProgression[3]->Fill(invMasslambdaC);

            // Armenteros Plot and pT_rel cut calculations

            TVector3 vecProton = ProtonH1Track->GetFourVector(mProton).Vect();
            TVector3 vecPion = PionH1Track->GetFourVector(mPion).Vect();
            TVector3 vecLambda = vecProton + vecPion;

            TVector3 vecPionC = PST[loop2]->GetFourVector(mPion).Vect();
            TVector3 vecLambdaC = LambdaCand[loop]->GetFourVector().Vect() + vecPionC;

            //Double_t qlProton = vecLambda.Dot(vecProton) / vecLambda.Mag();
            //Double_t qlPion = vecLambda.Dot(vecPion) / vecLambda.Mag();
            //Double_t pTrel = vecLambda.Cross(vecPion).Mag() / vecLambda.Mag();
            //Double_t chargeProton = LambdaCand[loop]->GetProtonCharge();
            //Double_t chargePion = LambdaCand[loop]->GetPionCharge();
            //Double_t alpha = (chargeProton * qlProton + chargePion * qlPion) / (qlProton + qlPion);

            double pTrelC = vecLambdaC.Cross(vecPionC).Mag() / (vecLambdaC.Mag());
            double qlPionC = vecLambdaC.Dot(vecPionC) / vecLambdaC.Mag();
            //double qlLambdaC = vecLambdaC.Dot(vecLambda) / vecLambdaC.Mag();
            double alphaC = (2 * (qlPionC)) / (vecLambdaC.Mag()) - 1;

            double EtaC = (PST[loop2]->GetFourVector(mPion)
                + LambdaCand[loop]->GetFourVector()).Vect().Eta();

            //CutTestsHERE
           
            //for (int a = 0; a < 20; a++) {
            //    if (PST[loop2]->GetPt() > cutvalues13[a]) { pionpTCutTest[a]->Fill(invMasslambdaC); }
            //}
            //for (int a = 0; a < 20; a++) {
            //    if (abs(PST[loop2]->GetEta()) < cutvalues14[a]) { pionEtaCutTest[a]->Fill(invMasslambdaC); }
            //}
            //for (int a = 0; a < 20; a++) {
            //    if (abs((pionTrajectory->GetDca(V2PV)) / (pionTrajectory->GetdDca(V2PV))) > cutvalues15[a]) { lambdaSDCApionCutTest[a]->Fill(invMasslambdaC); }
            //}
            //for (int a = 0; a < 20; a++) {
            //    if (abs((protonTrajectory->GetDca(V2PV)) / (protonTrajectory->GetdDca(V2PV))) > cutvalues16[a]) { lambdaSDCAprotonCutTest[a]->Fill(invMasslambdaC); }
            //}
            //for (int a = 0; a < 20; a++) {
            //    if (abs(LambdaCand[loop]->GetFourVector().Vect().Eta()) < cutvalues17[a]) { lambdaEtaCutTest[a]->Fill(invMasslambdaC); }
            //}
            //for (int a = 0; a < 20; a++) {
            //    if (pT > cutvalues18[a]) { lambdapTCutTest[a]->Fill(invMasslambdaC); }
            //}

           
            //endCutTests

            if (abs(invMasslambda - 1.11568) > 0.01) { continue; }

            LambdaCCutProgression[4]->Fill(invMasslambdaC);

            //hC1->Fill(invMasslambdaC);

            hdEdxPionCBefore->Fill(PST[loop2]->GetMomentum().Mag(), PST[loop2]->GetDedx());

            if (PST[loop2]->GetDedxLikelihood(H1Dedx::kPion) < 0.01) { continue; }

            hdEdxPionCAfter->Fill(PST[loop2]->GetMomentum().Mag(), PST[loop2]->GetDedx());

            //hC2->Fill(invMasslambdaC);
            LambdaCCutProgression[5]->Fill(invMasslambdaC);
            

            if (PST[loop2]->GetRadLength() < 12) { continue; }

            //hC3->Fill(invMasslambdaC);
            LambdaCCutProgression[6]->Fill(invMasslambdaC);

            

            //if (alphaC > 0.7) { continue; }

            //LambdaCCutProgression[7]->Fill(invMasslambdaC);

            //if (pTrelC > 0.9) { continue; }

            //LambdaCCutProgression[8]->Fill(invMasslambdaC);

            //pT cut for Lambda C
            //if (PST[loop2]->GetPt() < 0.2) { continue; }
            //if ((sqrt(vecLambdaC.X() * vecLambdaC.X() + vecLambdaC.Y() * vecLambdaC.Y())) < 0.5) { continue; }

            //hC4->Fill(invMasslambdaC);
            //LambdaCCutProgression[10]->Fill(invMasslambdaC);


            //ensuring the larger longitudinal momentum with respect to charm lambda is the lambda, skip if not
            //if (qlPionC > qlLambdaC) { continue; }
            //hC5->Fill(invMasslambdaC);

            //LambdaCCutProgression[8]->Fill(invMasslambdaC);

            hAPbeforeC->Fill(alphaC, pTrelC);

            //Calculations to cut on Lambda Candidate vertex position
            //we only care about lambdas that come from the primary vertex
            H1SelVertex* ProtonVertex = (H1SelVertex*)LambdaCand[loop]->GetProtonTrack();//Get the proton secondary vertex, which should be the spot the Lambda decay took place
            TVector3 ProtonVertexPosVec = ProtonVertex->GetPosition();
            //double ProtonPosX = sv.X();
            //double ProtonPosY = sv.Y();
            //double ProtonPosZ = sv.Z();

            Double_t lE = LambdaCand[loop]->GetE(); //GeV
            Double_t lgamma = lE / (mLambda); //mass is in GeV/c^2
            //ctau given in particle's rest frame, for lab frame must multiply documented ctau by a factor of gamma

            //cout << "Total Decay Length: " << sqrt((pv.X() - ProtonVertexPosVec.X()) * (pv.X() - ProtonVertexPosVec.X()) + (pv.Y() - ProtonVertexPosVec.Y()) * (pv.Y() - ProtonVertexPosVec.Y()) + (pv.Z() - ProtonVertexPosVec.Z()) * (pv.Z() - ProtonVertexPosVec.Z())) << endl;
            
            pdecaylengthmag->Fill(sqrt((pv.X() - ProtonPosX) * (pv.X() - ProtonPosX) + (pv.Y() - ProtonPosY) * (pv.Y() - ProtonPosY) + (pv.Z() - ProtonPosZ) * (pv.Z() - ProtonPosX)));
            
            //if (sqrt((pv.X() - ProtonPosX) * (pv.X() - ProtonPosX) + (pv.Y() - ProtonPosY) * (pv.Y() - ProtonPosY) + (pv.Z() - ProtonPosZ) * (pv.Z() - ProtonPosZ)) > (8)*(lgamma)) {continue;}
            //if (pTrelC > 0.9) { continue; }
            //cout << "X: " << ProtonPosX << endl;
            //cout << "Y: " << ProtonPosY << endl;
            //cout << "Z: " << ProtonPosZ << endl;

            //if (abs(pv.Z() - ProtonPosZ) > 32) { continue; }

            pXYvtxdensitytot->Fill(pv.X()-ProtonPosX, pv.Y()-ProtonPosY);
            pZXvtxdensitytot->Fill(pv.Z()-ProtonPosZ, pv.X()-ProtonPosX);
            pZYvtxdensitytot->Fill(pv.Z()-ProtonPosZ, pv.Y()-ProtonPosY);

            //if (pTrelC < 0.3 || pTrelC > 0.88) { continue; } //pT,rel cut for Armenteros Plot
            //LambdaCCutProgression[8]->Fill(invMasslambdaC);

            //if (pTrelC < 0.5) { continue; } //final cut

            //LambdaCCutProgression[10]->Fill(invMasslambdaC);

            //if (abs(EtaC) > 1.8) { continue; }

            //LambdaCCutProgression[11]->Fill(invMasslambdaC);

            //if (PST[loop2]->GetNHitDedx() < 10) { continue; }

            //LambdaCCutProgression[14]->Fill(invMasslambdaC);

            if (PST[loop2]->GetDcaPrime() > 0.25) { continue; }

            LambdaCCutProgression[12]->Fill(invMasslambdaC);

            hAPafterC->Fill(alphaC, pTrelC);

            
            //New Cuts
            //for (int a = 0; a < 20; a++) {
            //    if (PST[loop2]->GetPt() > cutvalues13[a]) { pionpTCutTest[a]->Fill(invMasslambdaC); }
            //}
            //for (int a = 0; a < 20; a++) {
            //    if (abs(PST[loop2]->GetEta()) < cutvalues14[a]) { pionEtaCutTest[a]->Fill(invMasslambdaC); }
            //}
            //for (int a = 0; a < 20; a++) {
            //    if (abs((pionTrajectory->GetDca(V2PV)) / (pionTrajectory->GetdDca(V2PV))) > cutvalues15[a]) { lambdaSDCApionCutTest[a]->Fill(invMasslambdaC); }
            //}
            //for (int a = 0; a < 20; a++) {
            //    if (abs((protonTrajectory->GetDca(V2PV)) / (protonTrajectory->GetdDca(V2PV))) > cutvalues16[a]) { lambdaSDCAprotonCutTest[a]->Fill(invMasslambdaC); }
            //}
            //for (int a = 0; a < 20; a++) {
            //    if (abs(LambdaCand[loop]->GetFourVector().Vect().Eta()) < cutvalues17[a]) { lambdaEtaCutTest[a]->Fill(invMasslambdaC); }
            //}
            //for (int a = 0; a < 20; a++) {
            //    if (pT > cutvalues18[a]) { lambdapTCutTest[a]->Fill(invMasslambdaC); }
            //}

            

            //pion Eta cut
            if (abs(PST[loop2]->GetEta()) > 1.8) { continue; }
            LambdaCCutProgression[13]->Fill(invMasslambdaC);

            //Lambda Eta cut
            if (abs(LambdaCand[loop]->GetFourVector().Vect().Eta()) > 1.8) { continue; }
            LambdaCCutProgression[14]->Fill(invMasslambdaC);

            //Lambda pT cut
            if (pT < 0.6) { continue; }
            LambdaCCutProgression[15]->Fill(invMasslambdaC);




            /////////////////////////////////////////////////////////////////////////////////////////////////////////
            //pion pT cut Tests
            if (PST[loop2]->GetPt() > 0.1) { LambdaCutTest1->Fill(invMasslambdaC); }
            if (PST[loop2]->GetPt() > 0.2) { LambdaCutTest2->Fill(invMasslambdaC); }
            if (PST[loop2]->GetPt() > 0.3) { LambdaCutTest3->Fill(invMasslambdaC); }
            if (PST[loop2]->GetPt() > 0.4) { LambdaCutTest4->Fill(invMasslambdaC); }
            if (PST[loop2]->GetPt() > 0.5) { LambdaCutTest5->Fill(invMasslambdaC); }
            if (PST[loop2]->GetPt() > 0.6) { LambdaCutTest6->Fill(invMasslambdaC); }
            if (PST[loop2]->GetPt() > 0.7) { LambdaCutTest7->Fill(invMasslambdaC); }
            if (PST[loop2]->GetPt() > 0.8) { LambdaCutTest8->Fill(invMasslambdaC); }
            if (PST[loop2]->GetPt() > 0.9) { LambdaCutTest9->Fill(invMasslambdaC); }
            //LambdaCCutProgression[13]->Fill(invMasslambdaC);

            
            lambdaCTree->Fill();

        }


        }// loop over lambda candidate array
    

            eventCounter++;
            if (eventCounter % 1000 == 0) {
                cout << "Processing ODS event " << eventCounter << endl;

                // Redraw the canvas every 1000 events
                //hKaon->Draw();
                //hLambda->Draw();
                //hAP->Draw();
                //gPad->Update();
            }

    } // loop over events

        cout << "\n" << eventCounter << " events processed." << endl;
        ////////////////////////////////////////////////////////////////////
        //////////////END MAIN LOOP//////////////////////////////////////
        ////////////////////////////////////////////////////////////////////

      
       /////////////////////////////////////////////////////////////////////////////////////////////////////////////
       //Output File just for Lambda TTrees and few odd histograms as sanity checks
       
        TFile treefile(opts.GetOutput(), "RECREATE");
        
        
        for (int a = 0; a < 9; a++) {
            LambdaCutProgression[a]->SetStats(0);
            LambdaCutProgression[a]->SetLineColor(kBlack + a);
            LambdaCutProgression[a]->GetXaxis()->SetTitle("Mass [GeV]");
            LambdaCutProgression[a]->GetYaxis()->SetTitle("Count");
            LambdaCutProgression[a]->Draw();
            LambdaCutProgression[a]->Write();

            hs->Add(LambdaCutProgression[a]);
        }

        hs->Draw("nostack");
        canvas->SetName("Lambda Cut Stack");
        canvas->Write();

        for (int a = 0; a < 20; a++) {
            LambdaCCutProgression[a]->SetStats(0);
            if (a <= 10) { LambdaCCutProgression[a]->SetLineColor(kBlack + a); }
            if (a > 10) { LambdaCCutProgression[a]->SetLineColor(kBlack + a-9); }
            LambdaCCutProgression[a]->GetXaxis()->SetTitle("Mass [GeV]");
            LambdaCCutProgression[a]->GetYaxis()->SetTitle("Count");
            LambdaCCutProgression[a]->Draw();
            LambdaCCutProgression[a]->Write();

            hsC->Add(LambdaCCutProgression[a]);
        }

        LambdaCutTest1->SetStats(0);
        LambdaCutTest1->SetLineColor(kBlack);
        LambdaCutTest1->GetXaxis()->SetTitle("Mass [GeV]");
        LambdaCutTest1->GetYaxis()->SetTitle("Count");
        LambdaCutTest1->Draw();
        LambdaCutTest1->Write();

        LambdaCutTest2->SetStats(0);
        LambdaCutTest2->SetLineColor(kBlack);
        LambdaCutTest2->GetXaxis()->SetTitle("Mass [GeV]");
        LambdaCutTest2->GetYaxis()->SetTitle("Count");
        LambdaCutTest2->Draw();
        LambdaCutTest2->Write();

        LambdaCutTest3->SetStats(0);
        LambdaCutTest3->SetLineColor(kBlack);
        LambdaCutTest3->GetXaxis()->SetTitle("Mass [GeV]");
        LambdaCutTest3->GetYaxis()->SetTitle("Count");
        LambdaCutTest3->Draw();
        LambdaCutTest3->Write();

        LambdaCutTest4->SetStats(0);
        LambdaCutTest4->SetLineColor(kBlack);
        LambdaCutTest4->GetXaxis()->SetTitle("Mass [GeV]");
        LambdaCutTest4->GetYaxis()->SetTitle("Count");
        LambdaCutTest4->Draw();
        LambdaCutTest4->Write();

        LambdaCutTest5->SetStats(0);
        LambdaCutTest5->SetLineColor(kBlack);
        LambdaCutTest5->GetXaxis()->SetTitle("Mass [GeV]");
        LambdaCutTest5->GetYaxis()->SetTitle("Count");
        LambdaCutTest5->Draw();
        LambdaCutTest5->Write();

        LambdaCutTest6->SetStats(0);
        LambdaCutTest6->SetLineColor(kBlack);
        LambdaCutTest6->GetXaxis()->SetTitle("Mass [GeV]");
        LambdaCutTest6->GetYaxis()->SetTitle("Count");
        LambdaCutTest6->Draw();
        LambdaCutTest6->Write();

        LambdaCutTest7->SetStats(0);
        LambdaCutTest7->SetLineColor(kBlack);
        LambdaCutTest7->GetXaxis()->SetTitle("Mass [GeV]");
        LambdaCutTest7->GetYaxis()->SetTitle("Count");
        LambdaCutTest7->Draw();
        LambdaCutTest7->Write();

        LambdaCutTest8->SetStats(0);
        LambdaCutTest8->SetLineColor(kBlack);
        LambdaCutTest8->GetXaxis()->SetTitle("Mass [GeV]");
        LambdaCutTest8->GetYaxis()->SetTitle("Count");
        LambdaCutTest8->Draw();
        LambdaCutTest8->Write();
        
        LambdaCutTest9->SetStats(0);
        LambdaCutTest9->SetLineColor(kBlack);
        LambdaCutTest9->GetXaxis()->SetTitle("Mass [GeV]");
        LambdaCutTest9->GetYaxis()->SetTitle("Count");
        LambdaCutTest9->Draw();
        LambdaCutTest9->Write();

        hsC->Draw("nostack");
        canvas->SetName("LambdaC Cut Stack");
        canvas->Write();
        
        hdEdxPionCBefore->SetStats(0);
        hdEdxPionCBefore->GetXaxis()->SetTitle("p [GeV]");
        hdEdxPionCBefore->GetYaxis()->SetTitle("dE/dx");
        hdEdxPionCBefore->Draw("colz");
        canvas->SetName("dE/dx Charm Pion After");
        canvas->Write();

        hdEdxPionCAfter->SetStats(0);
        hdEdxPionCAfter->GetXaxis()->SetTitle("p [GeV]");
        hdEdxPionCAfter->GetYaxis()->SetTitle("dE/dx");
        hdEdxPionCAfter->Draw("colz");
        canvas->SetName("dE/dx Charm Pion Before");
        canvas->Write();


        hAPbeforeC->SetStats(0);
        hAPbeforeC->GetXaxis()->SetTitle("\\alpha = 2 p^{Ch}_{L}/P - 1");
        hAPbeforeC->GetYaxis()->SetTitle("p_{T} [GeV]");
        hAPbeforeC->GetXaxis()->SetTitleSize(0.04);
        hAPbeforeC->GetXaxis()->SetTitleOffset(1);
        hAPbeforeC->GetYaxis()->SetTitleSize(0.05);
        hAPbeforeC->Draw("colz");
        canvas->SetName("AP Before C");
        canvas->Write();

        hAPafterC->SetStats(0);
        hAPafterC->GetXaxis()->SetTitle("\\alpha = 2 p^{Ch}_{L}/P - 1");
        hAPafterC->GetYaxis()->SetTitle("p_{T} [GeV]");
        hAPafterC->GetXaxis()->SetTitleSize(0.04);
        hAPafterC->GetXaxis()->SetTitleOffset(1);
        hAPafterC->GetYaxis()->SetTitleSize(0.05);
        hAPafterC->Draw("colz");
        canvas->SetName("AP After C");
        canvas->Write();

        pdecaylengthmag->SetStats(0);
        pdecaylengthmag->GetXaxis()->SetTitle("|Decay Length|");
        pdecaylengthmag->GetYaxis()->SetTitle("Count");
        pdecaylengthmag->Draw();
        pdecaylengthmag->Write();
        

        pXYvtxdensitytot->SetStats(0);
        pXYvtxdensitytot->GetXaxis()->SetTitle("X [cm]");
        pXYvtxdensitytot->GetYaxis()->SetTitle("Y [cm]");
        pXYvtxdensitytot->GetXaxis()->SetTitleSize(0.04);
        pXYvtxdensitytot->GetXaxis()->SetTitleOffset(1);
        pXYvtxdensitytot->GetYaxis()->SetTitleSize(0.05);
        pXYvtxdensitytot->Draw("colz");
        canvas->SetName("XY Vertex Density");
        canvas->Write();

        pZXvtxdensitytot->SetStats(0);
        pZXvtxdensitytot->GetXaxis()->SetTitle("Z [cm]");
        pZXvtxdensitytot->GetYaxis()->SetTitle("X [cm]");
        pZXvtxdensitytot->GetXaxis()->SetTitleSize(0.04);
        pZXvtxdensitytot->GetXaxis()->SetTitleOffset(1);
        pZXvtxdensitytot->GetYaxis()->SetTitleSize(0.05);
        pZXvtxdensitytot->Draw("colz");
        canvas->SetName("ZX Vertex Density");
        canvas->Write();

        pZYvtxdensitytot->SetStats(0);
        pZYvtxdensitytot->GetXaxis()->SetTitle("Z [cm]");
        pZYvtxdensitytot->GetYaxis()->SetTitle("Y [cm]");
        pZYvtxdensitytot->GetXaxis()->SetTitleSize(0.04);
        pZYvtxdensitytot->GetXaxis()->SetTitleOffset(1);
        pZYvtxdensitytot->GetYaxis()->SetTitleSize(0.05);
        pZYvtxdensitytot->Draw("colz");
        canvas->SetName("ZY Vertex Density");
        canvas->Write();

        
        //Write All Cut Tests
        for (int a = 0; a < 20; a++) {
            pionpTCutTest[a]->SetStats(0);
            pionpTCutTest[a]->SetLineColor(kBlack);
            pionpTCutTest[a]->GetXaxis()->SetTitle("Mass [Gev]");
            pionpTCutTest[a]->GetYaxis()->SetTitle("Count");
            pionpTCutTest[a]->Draw();
            pionpTCutTest[a]->Write();
        }
        for (int a = 0; a < 20; a++) {
            pionEtaCutTest[a]->SetStats(0);
            pionEtaCutTest[a]->SetLineColor(kBlack);
            pionEtaCutTest[a]->GetXaxis()->SetTitle("Mass [Gev]");
            pionEtaCutTest[a]->GetYaxis()->SetTitle("Count");
            pionEtaCutTest[a]->Draw();
            pionEtaCutTest[a]->Write();
        }
        for (int a = 0; a < 20; a++) {
            lambdaSDCApionCutTest[a]->SetStats(0);
            lambdaSDCApionCutTest[a]->SetLineColor(kBlack);
            lambdaSDCApionCutTest[a]->GetXaxis()->SetTitle("Mass [Gev]");
            lambdaSDCApionCutTest[a]->GetYaxis()->SetTitle("Count");
            lambdaSDCApionCutTest[a]->Draw();
            lambdaSDCApionCutTest[a]->Write();
        }
        for (int a = 0; a < 20; a++) {
            lambdaSDCAprotonCutTest[a]->SetStats(0);
            lambdaSDCAprotonCutTest[a]->SetLineColor(kBlack);
            lambdaSDCAprotonCutTest[a]->GetXaxis()->SetTitle("Mass [Gev]");
            lambdaSDCAprotonCutTest[a]->GetYaxis()->SetTitle("Count");
            lambdaSDCAprotonCutTest[a]->Draw();
            lambdaSDCAprotonCutTest[a]->Write();
        }
        for (int a = 0; a < 20; a++) {
            lambdaEtaCutTest[a]->SetStats(0);
            lambdaEtaCutTest[a]->SetLineColor(kBlack);
            lambdaEtaCutTest[a]->GetXaxis()->SetTitle("Mass [Gev]");
            lambdaEtaCutTest[a]->GetYaxis()->SetTitle("Count");
            lambdaEtaCutTest[a]->Draw();
            lambdaEtaCutTest[a]->Write();
        }
        for (int a = 0; a < 20; a++) {
            lambdapTCutTest[a]->SetStats(0);
            lambdapTCutTest[a]->SetLineColor(kBlack);
            lambdapTCutTest[a]->GetXaxis()->SetTitle("Mass [Gev]");
            lambdapTCutTest[a]->GetYaxis()->SetTitle("Count");
            lambdapTCutTest[a]->Draw();
            lambdapTCutTest[a]->Write();
        }

        
        //End Write all Cut Tests


       //Write all TTrees
       lambdaTotTree->Write();
       lambdaRegTree->Write();
       lambdaBarTree->Write();

       lambdaCTree->Write();
       //Write and close TTree file
       treefile.Write();
       treefile.Close();

       cout << "TTrees written to " << opts.GetOutput() << endl;

       return 0;

} //main loop