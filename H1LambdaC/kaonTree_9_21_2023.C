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
    Double_t kaonupperbinrange = 1.0;
    Double_t kaonlowerbinrange = 0.0;

   //Hardcode Test
    TH1F* KaonCount = new TH1F("Kaon Count", "Kaon Count", kaonbins, kaonlowerbinrange, kaonupperbinrange);

    /////////////////////////////////////////////////////////////////////////
    //Creating Array of Kaon plots for cut progression

    THStack* hs = new THStack("hs", "Stacked K0 Mass Plots");

    TH1F* KaonCutProgression[9];
    for (int a = 0; a < 9; a++) {
        string kaoncutprogstring = "Kaon Cut " + to_string(a);
        KaonCutProgression[a] = new TH1F(kaoncutprogstring.c_str(), kaoncutprogstring.c_str(), kaonbins, kaonlowerbinrange, kaonupperbinrange);
    }

    /////////////////////////////////////////////////////////////////////////


    // load in some HAT data
    H1FloatPtr q2e("Q2e");
    H1FloatPtr ebeamE("EBeamE");
    H1FloatPtr ebeamP("EBeamP");
    H1FloatPtr ye("Ye");
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Defining TTrees to store K0 candidates and their associated kinematic variables
    TTree* K0Tree = new TTree("K0", "K0");
    //defining the variables I want to store in Lambda Trees
    double pT, Eta, x, invMasskaon, Q2e, Ye, Angle;
    //Defining Tree Branches
    K0Tree->Branch("invMasskaon", &invMasskaon, "invMasskaon/D");
    K0Tree->Branch("pT", &pT, "pT/D");
    K0Tree->Branch("Eta", &Eta, "Eta/D");
    K0Tree->Branch("x", &x, "x/D");
    K0Tree->Branch("Q2e", &Q2e, "Q2e/D");
    K0Tree->Branch("Ye", &Ye, "Ye/D");
    K0Tree->Branch("Angle", &Angle, "Angle/D");
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Defining Root Canvas to work on
    TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 800);

    // Pointer to vertices to loop over
    static H1CentralVertexArrayPtr vertex;
    static H1PartK0ArrayPtr K0Cand;
   
    // Get the pion's mass
    Double_t mPion = TDatabasePDG::Instance()->GetParticle(211)->Mass();
   
    ///////////////BEGIN MAIN LOOP////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    // Loop as long as there's data
    while (H1Tree::Instance()->Next() && !opts.IsMaxEvent(eventCounter)) {

        // Loop over all vertices found in the event
        for (Int_t loop = 0; loop < K0Cand.GetEntries(); loop++) {

        if (K0Cand[loop]->GetNumOfParticles() < 2) continue;

        invMasskaon = (K0Cand[loop]->GetPosPionFourVector()
            + K0Cand[loop]->GetNegPionFourVector()).M();

        //Getting Vectors for Armenteros-Podolansky Plot
        H1Track* PosPionH1Track = (H1Track*)K0Cand[loop]->GetPosPionTrack();
        H1Track* NegPionH1Track = (H1Track*)K0Cand[loop]->GetNegPionTrack();

        TVector3 vecPosPion = PosPionH1Track->GetFourVector(mPion).Vect();
        TVector3 vecNegPion = NegPionH1Track->GetFourVector(mPion).Vect();
        TVector3 vecK0 = vecPosPion + vecNegPion;

        
        
        //Call H1PartLambda class data that references H1CentralFittedTrack for Pion and Proton
        const H1CentralFittedTrack* PosPionCFTrack = K0Cand[loop]->GetPosPionTrack();
        const H1CentralFittedTrack* NegPionCFTrack = K0Cand[loop]->GetNegPionTrack();


        //Getting H1PartSelTracks for daughters
        const H1PartSelTrack* PosPionPSTrack = K0Cand[loop]->GetPosPion();
        const H1PartSelTrack* NegPionPSTrack = K0Cand[loop]->GetNegPion();

        
        //Prepating DCA/dDCA information to get a histogram to help visualize this cut
        const H1CentralFittedV0* v0 = K0Cand[loop]->GetV0();

        const H1CentralTrack* pospionFittedNoV = PosPionCFTrack->GetCentralTrack();
        const H1Trajectory* pospionTrajectory = pospionFittedNoV->GetTrajectory();

        const H1CentralTrack* negpionFittedNoV = NegPionCFTrack->GetCentralTrack();
        const H1Trajectory* negpionTrajectory = negpionFittedNoV->GetTrajectory();

        //PV Info
        H1SelVertex* PosPionVertex = (H1SelVertex*)K0Cand[loop]->GetPosPionTrack();
        const H1SelVertex* PosPionPrimaryVertex = PosPionVertex->GetPrimaryVertex();
        TVector3 pv = PosPionPrimaryVertex->GetPosition();//is same for both pions, so just using pos one here and previous couple lines
        TVector2 V2PV(pv.X(), pv.Y());

        //SV Info
        const H1ReconstructedVertex* DecayVertex = v0->GetDecayVertex();
        TVector3 sv = DecayVertex->GetPosition();//is same for both pions, so just using pos one here and previous couple lines

        //Get the vector pointing from primary vertex to secondary vertex
        TVector3 PVtoSV = sv - pv;
        TVector3 KaonP = K0Cand[loop]->GetFourVector().Vect();
        double angle = acos((PVtoSV.Dot(KaonP))/((PVtoSV.Mag())*(KaonP.Mag())));

       /* cout << "X 1: " << PVtoSV.X() << endl;
        cout << "X 2: " << KaonP.X() << endl;
        cout << "Y 1: " << PVtoSV.Y() << endl;
        cout << "Y 2: " << KaonP.Y() << endl;
        cout << "Z 1: " << PVtoSV.Z() << endl;
        cout << "Z 2: " << KaonP.Z() << endl;
        cout << "Dot: " << angle << endl;*/
        

        
        ///////////////////////////////////////////////////////////////////////////////
        //Defining pT before cuts and putting stuff into pt bins
        
        pT = v0->GetPt();

        //Raw K0 Candidate data after our event selection defined in create_evls.C
        KaonCutProgression[0]->Fill(invMasskaon);

       
   ////////////////////////////////////////////////////////////////////////////////////////////////
        //PID Likelihood Cut for Pions < 0.001
        if (PosPionPSTrack->GetDedxLikelihood(H1Dedx::kPion) < 0.001 || NegPionPSTrack->GetDedxLikelihood(H1Dedx::kPion) < 0.001) { continue; }
     
        KaonCutProgression[1]->Fill(invMasskaon);
        
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //DCA over dDCA proton cut
        if (abs((pospionTrajectory->GetDca(V2PV)) / (pospionTrajectory->GetdDca(V2PV))) < 1 || abs((negpionTrajectory->GetDca(V2PV)) / (negpionTrajectory->GetdDca(V2PV))) < 1) {  continue;}
        KaonCutProgression[2]->Fill(invMasskaon);


        //After all cuts, put information into TTrees
        Eta = K0Cand[loop]->GetFourVector().Vect().Eta();
        x = (*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4);
        Q2e = *q2e;
        Ye = *ye;
        Angle = angle;

        //cout << "pT from v0: " << pT << endl;
        //cout << "other pT: " << K0Cand[loop]->GetFourVector().Pt() << endl;
        //Hardcode a quit test for getting histogram counts
        if (pT > 0.5 && pT < 3.5) {
            if (abs(Eta) < 1.3) {
              
                KaonCount->Fill(invMasskaon);
              
            }
        }
        //
        K0Tree->Fill();


        } // loop over K0 Candidate array
            eventCounter++;
            if (eventCounter % 1000 == 0) {
                cout << "Processing ODS event " << eventCounter << endl;

                // Redraw the canvas every 1000 events
                //hKaon->Draw();
                //hLambda->Draw();
                //hAP->Draw();
                //gPad->Update();
            }
    }//loop over events

        cout << "\n" << eventCounter << " events processed." << endl;
        ////////////////////////////////////////////////////////////////////
        //////////////END MAIN LOOP//////////////////////////////////////
        ////////////////////////////////////////////////////////////////////

       
 
       TFile treefile(opts.GetOutput(), "RECREATE");
       
       
       for (int a = 0; a < 9; a++) {
           KaonCutProgression[a]->SetStats(0);
           KaonCutProgression[a]->SetLineColor(kBlack + a);
           KaonCutProgression[a]->GetXaxis()->SetTitle("Mass [GeV]");
           KaonCutProgression[a]->GetYaxis()->SetTitle("Count");
           KaonCutProgression[a]->Draw();
           KaonCutProgression[a]->Write();

           hs->Add(KaonCutProgression[a]);
       }
       hs->Draw("nostack");
       canvas->SetName("Kaon Cut Stack");
       canvas->Write();

       KaonCount->SetLineColor(kBlack);
       KaonCount->GetXaxis()->SetTitle("Mass [GeV]");
       KaonCount->GetYaxis()->SetTitle("Count");
       KaonCount->Draw();
       KaonCount->Write();

       //Write all TTrees 
       K0Tree->Write();
       //Write and close TTree file
       treefile.Write();
       treefile.Close();

       cout << "Histograms written to " << opts.GetOutput() << endl;


        return 0;
    }