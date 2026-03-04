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
    
    // load in some HAT data
    H1FloatPtr q2e("Q2e");
    H1FloatPtr ebeamE("EBeamE");
    H1FloatPtr ebeamP("EBeamP");
    H1FloatPtr ye("Ye");
    
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
    
    //////////////////////////////////////////////////////////////////////////
    // Defining Cut Progression Histograms

    THStack* hs = new THStack("hs", "Stacked Total Lambda Mass Plots");

    TH1F* LambdaCutProgression[9];
    for (int a = 0; a < 9; a++) {
        string lambdacutprogstring = "Lambda Cut " + to_string(a);
        LambdaCutProgression[a] = new TH1F(lambdacutprogstring.c_str(), lambdacutprogstring.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    }

    //Hard coding lambda peaks as a double check
    TH1F* LambdaBoth = new TH1F("Lambda Both", "Lambda Both", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* LambdaReg = new TH1F("Lambda Reg", "Lambda Reg", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* LambdaBar = new TH1F("Lambda Bar", "Lambda Bar", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    //

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Defining Root Canvas to work on
    TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 800);

   //
   // 
    // Pointer to vertices to loop over
    static H1CentralVertexArrayPtr vertex;
    static H1PartLambdaArrayPtr LambdaCand;
    
    
    // Get the pion's mass
    Double_t mPion = TDatabasePDG::Instance()->GetParticle(211)->Mass();
    // Get proton mass
    Double_t mProton = TDatabasePDG::Instance()->GetParticle(2212)->Mass();
    // Get electron mass
    Double_t mElectron = TDatabasePDG::Instance()->GetParticle(11)->Mass();

    
    double breakloopcount = 0;
    double passedallcutscount = 0;



    ///////////////BEGIN MAIN LOOP////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    // Loop as long as there's data
    while (H1Tree::Instance()->Next() && !opts.IsMaxEvent(eventCounter)) {
        //cout << "Get Entries count: " << LambdaCand.GetEntries() << endl;
        //cout << "sizeof count: " << LambdaCand.size() << endl;
        breakloopcount = 0;
        passedallcutscount = 0;
        // Loop over all Lambda candidates on MODS level as determined by rough cuts in H1FindLambda code
        for (Int_t loop = 0; loop < LambdaCand.GetEntries(); loop++) {

            if (LambdaCand[loop]->GetNumOfParticles() < 2) { 
                breakloopcount++;
                continue;
            }

        invMasslambda = (LambdaCand[loop]->GetProtonFourVector()
            + LambdaCand[loop]->GetPionFourVector()).M();

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

        H1SelVertex* ProtonVertex = (H1SelVertex*)LambdaCand[loop]->GetProtonTrack();
        const H1SelVertex* ProtonPrimaryVertex = ProtonVertex->GetPrimaryVertex();
        TVector3 pv = ProtonPrimaryVertex->GetPosition();//is same for both protons and pion daughters, so just using proton here and previous couple lines
        TVector2 V2PV(pv.X(), pv.Y());

        //Defining pT before cuts and putting stuff into pt bins
        const H1CentralFittedV0* v0 = LambdaCand[loop]->GetV0();
        pT = v0->GetPt();

        LambdaCutProgression[0]->Fill(invMasslambda); //Raw Lambda Candidates

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //PID Likelihood Cut for Proton
        if (ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton) < 0.001) { 
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[1]->Fill(invMasslambda);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //PID Likelihood Cut for Pion
        if (PionPSTrack->GetDedxLikelihood(H1Dedx::kPion) < 0.001) {
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[2]->Fill(invMasslambda);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////        
        //DCA over dDCA proton cut
        if (abs((protonTrajectory->GetDca(V2PV)) / (protonTrajectory->GetdDca(V2PV))) < 1) { 
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[3]->Fill(invMasslambda);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //DCA over dDCA pion cut
        if (abs((pionTrajectory->GetDca(V2PV)) / (pionTrajectory->GetdDca(V2PV))) < 1) { 
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[4]->Fill(invMasslambda);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Cut against K0 contamination
        Double_t invMasskaonCont = (ProtonH1Track->GetFourVector(mPion)
            + PionH1Track->GetFourVector(mPion)).M();
        if (invMasskaonCont > 0.465 && invMasskaonCont < 0.53) { 
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[5]->Fill(invMasslambda);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Cut against photon contamination
        Double_t invMassphotonCont = (ProtonH1Track->GetFourVector(mElectron)
            + PionH1Track->GetFourVector(mElectron)).M();
        if (invMassphotonCont < 0.05) {
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[6]->Fill(invMasslambda);

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // STORE LAMBDA CANDIDATES INTO TTREES AFTER ALL CUTS EXCEPT THE PT AND ETA CUT HERE
        Eta = LambdaCand[loop]->GetFourVector().Vect().Eta();
        x = (*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4);
        Q2e = *q2e;
        Ye = *ye;
        pT = v0->GetPt();
        invMasslambda = (LambdaCand[loop]->GetProtonFourVector()
            + LambdaCand[loop]->GetPionFourVector()).M();

        passedallcutscount++;
        lambdaTotTree->Fill();
        
        if (pT > 0.5 && pT < 3.5) {
            if (abs(Eta) < 1.3) {
                if (Ye > 0.1 && Ye < 0.6) {
                    if (Q2e > 5 && Q2e < 100) {
                        
                     LambdaBoth->Fill(invMasslambda);
                        
                    }
                }
            }
        }


        if (LambdaCand[loop]->IsLambdaBar() == true) { 
            lambdaBarTree->Fill(); 
            if (pT > 0.5 && pT < 3.5) {
                if (abs(Eta) < 1.3) {
                    if (Ye > 0.1 && Ye < 0.6) {
                        if (Q2e > 5 && Q2e < 100) {

                            LambdaBar->Fill(invMasslambda);

                        }
                    }
                }
            }
        }

        if (LambdaCand[loop]->IsLambdaBar() != true) { 
            lambdaRegTree->Fill(); 
            if (pT > 0.5 && pT < 3.5) {
                if (abs(Eta) < 1.3) {
                    if (Ye > 0.1 && Ye < 0.6) {
                        if (Q2e > 5 && Q2e < 100) {

                            LambdaReg->Fill(invMasslambda);

                        }
                    }
                }
            }
        }
    
            
        }//loop over lambda candidate array
                
        //cout << "Passed All Cuts Count: " << passedallcutscount << endl;
        //cout << "Loop Break Counts: " << breakloopcount << endl;
        //cout << "Total Number of Lambda Entries: " << LambdaCand.GetEntries() << endl;
        if (passedallcutscount + breakloopcount != LambdaCand.GetEntries()) {
            cout << "Bad Count" << endl;
        }
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
        
        LambdaBoth->SetStats(0);
        LambdaBoth->SetLineColor(kBlack);
        LambdaBoth->GetXaxis()->SetTitle("Mass [GeV]");
        LambdaBoth->GetYaxis()->SetTitle("Count");
        LambdaBoth->Draw();
        LambdaBoth->Write();

        LambdaReg->SetStats(0);
        LambdaReg->SetLineColor(kBlack);
        LambdaReg->GetXaxis()->SetTitle("Mass [GeV]");
        LambdaReg->GetYaxis()->SetTitle("Count");
        LambdaReg->Draw();
        LambdaReg->Write();

        LambdaBar->SetStats(0);
        LambdaBar->SetLineColor(kBlack);
        LambdaBar->GetXaxis()->SetTitle("Mass [GeV]");
        LambdaBar->GetYaxis()->SetTitle("Count");
        LambdaBar->Draw();
        LambdaBar->Write();
        
        
        
       
       //Write all TTrees 
       lambdaTotTree->Write();
       lambdaRegTree->Write();
       lambdaBarTree->Write();
       //Write and close TTree file
       treefile.Write();
       treefile.Close();

       cout << "TTrees written to " << opts.GetOutput() << endl;

       return 0;

} //main loop