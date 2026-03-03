///////////////////////////////////////////////////////
// 
// D0 and Lambda Finder
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
    string output = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/lambda.root";
    // event counter
    Int_t eventCounter = 0;
    // parse the command line
    H1StdCmdLine opts;
    opts.Parse(&argc, argv);
    // needed for graphic, but MUST be AFTER Parse(...):
    //TApplication theApp("kaonfind_ods", &argc, argv);

    // Load mODS/HAT files
    H1Tree::Instance()->Open();            // this statement must be here

    
    //Defining Root Canvas to work on
    TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 400);

   //
   // 
    // Pointer to vertices to loop over
    static H1CentralVertexArrayPtr vertex;
    static H1PartLambdaArrayPtr LambdaCand;
    static H1PartCandArrayPtr PartCand;
    static H1PartSelTrackArrayPtr PST;//loop over vertices now to try and get LambdaC peaks
    //static H1CentralFittedV0ArrayPtr v0;

    // load in some HAT data
    H1FloatPtr q2e("Q2e");
    H1FloatPtr ebeamE("EBeamE");
    H1FloatPtr ebeamP("EBeamP");
    H1FloatPtr ye("Ye");




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

        //Filling AP plot before any additional cuts, but after H1FindLambda code
        hAPbefore->Fill(alpha,pTrel);
        
        //Call H1PartLambda class data that references H1CentralFittedTrack for Pion and Proton
        const H1CentralFittedTrack* ProtonCFTrack = LambdaCand[loop]->GetProtonTrack();
        const H1CentralFittedTrack* PionCFTrack = LambdaCand[loop]->GetPionTrack();

        //dEdx plots of lambda ppion and proton daughters before any cuts, but after H1FindLambda
        hdEdxBothBefore->Fill(ProtonCFTrack->GetMomentum().Mag(), ProtonCFTrack->GetCorrectedDedx());
        hdEdxBothBefore->Fill(PionCFTrack->GetMomentum().Mag(), PionCFTrack->GetCorrectedDedx());

        hdEdxProtonBefore->Fill(ProtonCFTrack->GetMomentum().Mag(), ProtonCFTrack->GetCorrectedDedx());
        hdEdxPionBefore->Fill(PionCFTrack->GetMomentum().Mag(), PionCFTrack->GetCorrectedDedx());
        
        
        //Getting H1PartSelTracks for daughters
        const H1PartSelTrack* ProtonPSTrack = LambdaCand[loop]->GetProton();
        const H1PartSelTrack* PionPSTrack = LambdaCand[loop]->GetPion();

        //Plotting Likelihood to help visualize likelihood cut beofe any cuts
        hDedxLproton->Fill(ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton));
        hDedxLpion->Fill(PionPSTrack->GetDedxLikelihood(H1Dedx::kPion));
        
        //Prepating DCA/dDCA information to get a histogram to help visualize this DCA Significance cut
        const H1CentralTrack* protonFittedNoV = ProtonCFTrack->GetCentralTrack();
        const H1Trajectory* protonTrajectory = protonFittedNoV->GetTrajectory();

        const H1CentralTrack* pionFittedNoV = PionCFTrack->GetCentralTrack();
        const H1Trajectory* pionTrajectory = pionFittedNoV->GetTrajectory();

        H1SelVertex* ProtonVertex = (H1SelVertex*)LambdaCand[loop]->GetProtonTrack();
        const H1SelVertex* ProtonPrimaryVertex = ProtonVertex->GetPrimaryVertex();
        TVector3 pv = ProtonPrimaryVertex->GetPosition();//is same for both protons and pion daughters, so just using proton here and previous couple lines
        TVector2 V2PV(pv.X(), pv.Y());

        //Filling histograms for DCA,DCA,and DCA Significance for each lambda candidate daughters
        hdDCAproton->Fill((protonTrajectory->GetdDca(V2PV)));
        hdDCApion->Fill((pionTrajectory->GetdDca(V2PV)));
        hDCAproton->Fill((protonTrajectory->GetDca(V2PV)));
        hDCApion->Fill((pionTrajectory->GetDca(V2PV)));
        hDCAoverdDCAproton->Fill((protonTrajectory->GetDca(V2PV))/(protonTrajectory->GetdDca(V2PV)));
        hDCAoverdDCApion->Fill((pionTrajectory->GetDca(V2PV))/(pionTrajectory->GetdDca(V2PV)));

        //Defining pT before cuts and putting stuff into pt bins
        const H1CentralFittedV0* v0 = LambdaCand[loop]->GetV0();
        Double_t pT = v0->GetPt();

       
        //Raw Lambda Candidate data after our event selection defined in create_evls.C
        hLambda->Fill(invMasslambda);
        if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdaBar->Fill(invMasslambda); }
        if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaReg->Fill(invMasslambda); }

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
  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //PID Likelihood Cut for Proton
        if (ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton) < 0.001) { 
            continue;
        }

        hLambdaPIDproton->Fill(invMasslambda);
        if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdaBarPIDproton->Fill(invMasslambda); }
        if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaRegPIDproton->Fill(invMasslambda); }

        //Fill pT bins after PID Likelihood Cut for Proton
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //PID Likelihood Cut for Pion
        if (PionPSTrack->GetDedxLikelihood(H1Dedx::kPion) < 0.001) {
            continue; }

        hLambdaPIDpion->Fill(invMasslambda);
        if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdaBarPIDpion->Fill(invMasslambda); }
        if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaRegPIDpion->Fill(invMasslambda); }
        //Fill pT bins after PID Likelihood Cut for Pion
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////        
        //DCA over dDCA proton cut
        if (abs((protonTrajectory->GetDca(V2PV)) / (protonTrajectory->GetdDca(V2PV))) < 1) { 
            continue; }

        hLambdaDCAoverdDCAproton->Fill(invMasslambda);
        if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdaBarDCAoverdDCAproton->Fill(invMasslambda); }
        if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaRegDCAoverdDCAproton->Fill(invMasslambda); }
        
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //DCA over dDCA pion cut
        if (abs((pionTrajectory->GetDca(V2PV)) / (pionTrajectory->GetdDca(V2PV))) < 1) { 
            continue; }


        hpionDCAvsdDCAafter->Fill((pionTrajectory->GetdDca(V2PV)), (pionTrajectory->GetDca(V2PV)));
        hprotonDCAvsdDCAafter->Fill((protonTrajectory->GetdDca(V2PV)), (protonTrajectory->GetDca(V2PV)));

        hLambdaDCAoverdDCApion->Fill(invMasslambda);
        if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdaBarDCAoverdDCApion->Fill(invMasslambda); }
        if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaRegDCAoverdDCApion->Fill(invMasslambda); }
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


        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Cut against K0 contamination
        Double_t invMasskaonCont = (ProtonH1Track->GetFourVector(mPion)
            + PionH1Track->GetFourVector(mPion)).M();
        if (invMasskaonCont > 0.465 && invMasskaonCont < 0.53) { 
            continue; }
        hLambdaKaonCont->Fill(invMasslambda);
        if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdaBarKaonCont->Fill(invMasslambda); }
        if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaRegKaonCont->Fill(invMasslambda); }
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Cut against photon contamination
        Double_t invMassphotonCont = (ProtonH1Track->GetFourVector(mElectron)
            + PionH1Track->GetFourVector(mElectron)).M();
        if (invMassphotonCont < 0.05) { 
            continue; }
        hLambdaPhotonCont->Fill(invMasslambda);
        if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdaBarPhotonCont->Fill(invMasslambda); }
        if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaRegPhotonCont->Fill(invMasslambda); }
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

        ////////Get Raw pT and Eta distributions before implementing those cuts/////////////////////////////////////////////////////////////////////////////
        if (invMasslambda > lambdalowerbound && invMasslambda < lambdaupperbound) {
            //Fill in Lambda Candidate Distributions vs five kinematic variables

            if (LambdaCand[loop]->IsLambdaBar() == true) {
                hLambdabarVsBjorkenX->Fill((*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4));
                hLambdabarVspT->Fill(pT);
                hLambdabarVsEta->Fill(LambdaCand[loop]->GetFourVector().Vect().Eta());
                hLambdabarVsQ2->Fill(*q2e);
                hLambdabarVsy->Fill(*ye);
                hLambdabarVsphi->Fill(LambdaCand[loop]->GetFourVector().Vect().Phi());
            }

            if (LambdaCand[loop]->IsLambdaBar() != true) {
                hLambdaregVsBjorkenX->Fill((*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4));
                hLambdaregVspT->Fill(pT);
                hLambdaregVsEta->Fill(LambdaCand[loop]->GetFourVector().Vect().Eta());
                hLambdaregVsQ2->Fill(*q2e);
                hLambdaregVsy->Fill(*ye);
                hLambdaregVsphi->Fill(LambdaCand[loop]->GetFourVector().Vect().Phi());
            }
        }


        /*if (LambdaCand[loop]->IsLambdaBar() == true) {
            hMassVsBjorkenXlambdabar->Fill(invMasslambda, (*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4));
            hMassVspTlambdabar->Fill(invMasslambda, pT);
            hMassVsEtalambdabar->Fill(invMasslambda, LambdaCand[loop]->GetFourVector().Vect().Eta());
            hMassVsQ2lambdabar->Fill(invMasslambda, *q2e);
        }
        if (LambdaCand[loop]->IsLambdaBar() != true) {
            hMassVsBjorkenXlambdareg->Fill(invMasslambda, (*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4));
            hMassVspTlambdareg->Fill(invMasslambda, pT);
            hMassVsEtalambdareg->Fill(invMasslambda, LambdaCand[loop]->GetFourVector().Vect().Eta());
            hMassVsQ2lambdareg->Fill(invMasslambda, *q2e);
        }*/


        ////////Final Kinematic Cuts on pT and Eta////////////////////////////////////////////////////////////////////////////////////////////////////////
       /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////Eta Cut
        //abs(acosh((v0->GetMomentum().Mag()) / pT))
        if (abs(LambdaCand[loop]->GetFourVector().Vect().Eta()) >= 1.3) {
            continue; }
        hLambdaEta->Fill(invMasslambda);
        if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdaBarEta->Fill(invMasslambda); }
        if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaRegEta->Fill(invMasslambda); }
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//pT Range Cut
        if (pT<=0.5 || pT>=3.5) {
            continue;
        }
        hLambdaWholepTrange->Fill(invMasslambda);
        if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdaBarWholepTrange->Fill(invMasslambda); }
        if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaRegWholepTrange->Fill(invMasslambda); }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //After all cut plots to fill
        hAPafter->Fill(alpha, pTrel);
        hdEdxBothAfterAllcuts->Fill(ProtonCFTrack->GetMomentum().Mag(), ProtonCFTrack->GetCorrectedDedx());
        hdEdxBothAfterAllcuts->Fill(PionCFTrack->GetMomentum().Mag(), PionCFTrack->GetCorrectedDedx());

        hdEdxProtonAfterAllcuts->Fill(ProtonCFTrack->GetMomentum().Mag(), ProtonCFTrack->GetCorrectedDedx());
        hdEdxPionAfterAllcuts->Fill(PionCFTrack->GetMomentum().Mag(), PionCFTrack->GetCorrectedDedx());
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        //After All necessary cuts for kinematic range and for clean lambda sample, can fill different bins
        
        //Fill in plots for q2 dist
        if (*q2e >= 2 && *q2e < 2.5) {
            hLambdatotQ2bin1->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarQ2bin1->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregQ2bin1->Fill(invMasslambda); }
        }
        if (*q2e >= 2.5 && *q2e < 3.0) {
            hLambdatotQ2bin2->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarQ2bin2->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregQ2bin2->Fill(invMasslambda); }
        }
        if (*q2e >= 3.0 && *q2e < 4.0) {
            hLambdatotQ2bin3->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarQ2bin3->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregQ2bin3->Fill(invMasslambda); }
        }
        if (*q2e >= 4.0 && *q2e < 5.0) {
            hLambdatotQ2bin4->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarQ2bin4->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregQ2bin4->Fill(invMasslambda); }
        }
        if (*q2e >= 5.0 && *q2e < 7.0) {
            hLambdatotQ2bin5->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarQ2bin5->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregQ2bin5->Fill(invMasslambda); }
        }
        if(*q2e >= 7.0 && *q2e < 10.0) {
            hLambdatotQ2bin6->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarQ2bin6->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregQ2bin6->Fill(invMasslambda); }
        }
        if(*q2e >= 10.0 && *q2e < 15.0) {
            hLambdatotQ2bin7->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarQ2bin7->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregQ2bin7->Fill(invMasslambda); }
        }
        if(*q2e >= 15.0 && *q2e < 25.0) {
            hLambdatotQ2bin8->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarQ2bin8->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregQ2bin8->Fill(invMasslambda); }
        }
        if(*q2e >= 25.0 && *q2e < 100.0) {
            hLambdatotQ2bin9->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarQ2bin9->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregQ2bin9->Fill(invMasslambda); }
        }
        ///////////////////////////////
        //Fill in Plots for X distribution
        if ((*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4) >= 0.00004 && (*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4) < 0.0001) {
            hLambdatotXbin1->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarXbin1->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregXbin1->Fill(invMasslambda); }
        }
        if ((*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4) >= 0.0001 && (*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4) < 0.0002) {
            hLambdatotXbin2->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarXbin2->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregXbin2->Fill(invMasslambda); }
        }
        if ((*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4) >= 0.0002 && (*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4) < 0.0004) {
            hLambdatotXbin3->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarXbin3->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregXbin3->Fill(invMasslambda); }
        }
        if ((*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4) >= 0.0004 && (*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4) < 0.001) {
            hLambdatotXbin4->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarXbin4->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregXbin4->Fill(invMasslambda); }
        }
        if ((*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4) >= 0.001 && (*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4) < 0.01) {
            hLambdatotXbin5->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarXbin5->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregXbin5->Fill(invMasslambda); }
        }
        ////////////////////////////////////
        //Fill Eta
        if (LambdaCand[loop]->GetFourVector().Vect().Eta() >= -1.3 && LambdaCand[loop]->GetFourVector().Vect().Eta() < -1) {
            hLambdatotEtabin1->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarEtabin1->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregEtabin1->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Eta() >= -1 && LambdaCand[loop]->GetFourVector().Vect().Eta() < -0.75) {
            hLambdatotEtabin2->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarEtabin2->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregEtabin2->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Eta() >= -0.75 && LambdaCand[loop]->GetFourVector().Vect().Eta() < -0.5) {
            hLambdatotEtabin3->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarEtabin3->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregEtabin3->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Eta() >= -0.5 && LambdaCand[loop]->GetFourVector().Vect().Eta() < -0.25) {
            hLambdatotEtabin4->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarEtabin4->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregEtabin4->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Eta() >= -0.25 && LambdaCand[loop]->GetFourVector().Vect().Eta() < 0) {
            hLambdatotEtabin5->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarEtabin5->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregEtabin5->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Eta() >= 0 && LambdaCand[loop]->GetFourVector().Vect().Eta() < 0.25) {
            hLambdatotEtabin6->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarEtabin6->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregEtabin6->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Eta() >= 0.25 && LambdaCand[loop]->GetFourVector().Vect().Eta() < 0.5) {
            hLambdatotEtabin7->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarEtabin7->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregEtabin7->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Eta() >= 0.5 && LambdaCand[loop]->GetFourVector().Vect().Eta() < 0.75) {
            hLambdatotEtabin8->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarEtabin8->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregEtabin8->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Eta() >= 0.75 && LambdaCand[loop]->GetFourVector().Vect().Eta() < 1) {
            hLambdatotEtabin9->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarEtabin9->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregEtabin9->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Eta() >= 1 && LambdaCand[loop]->GetFourVector().Vect().Eta() < 1.3) {
            hLambdatotEtabin10->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarEtabin10->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregEtabin10->Fill(invMasslambda); }
        }
        ////////////////////////////////////////////////////
        //Fill pT
        if (pT >= 0.5 && pT < 0.6) {
            hLambdatotpTbin1->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarpTbin1->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregpTbin1->Fill(invMasslambda); }
        }
        if (pT >= 0.6 && pT < 0.7) {
            hLambdatotpTbin2->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarpTbin2->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregpTbin2->Fill(invMasslambda); }
        }
        if (pT >= 0.7 && pT < 0.8) {
            hLambdatotpTbin3->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarpTbin3->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregpTbin3->Fill(invMasslambda); }
        }
        if (pT >= 0.8 && pT < 0.9) {
            hLambdatotpTbin4->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarpTbin4->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregpTbin4->Fill(invMasslambda); }
        }
        if (pT >= 0.9 && pT < 1.1) {
            hLambdatotpTbin5->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarpTbin5->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregpTbin5->Fill(invMasslambda); }
        }
        if (pT >= 1.1 && pT < 1.3) {
            hLambdatotpTbin6->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarpTbin6->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregpTbin6->Fill(invMasslambda); }
        }
        if (pT >= 1.3 && pT < 1.6) {
            hLambdatotpTbin7->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarpTbin7->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregpTbin7->Fill(invMasslambda); }
        }
        if (pT >= 1.6 && pT < 2.2) {
            hLambdatotpTbin8->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarpTbin8->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregpTbin8->Fill(invMasslambda); }
        }
        if (pT >= 2.2 && pT < 3.5) {
            hLambdatotpTbin9->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarpTbin9->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregpTbin9->Fill(invMasslambda); }
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////
        // Fill Phi
        if (LambdaCand[loop]->GetFourVector().Vect().Phi() >= -M_PI && LambdaCand[loop]->GetFourVector().Vect().Phi() < (-5*M_PI)/(6)) {
            hLambdatotPhibin1->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarPhibin1->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregPhibin1->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Phi() >= (-5 * M_PI) / (6) && LambdaCand[loop]->GetFourVector().Vect().Phi() < (-4 * M_PI) / (6)) {
            hLambdatotPhibin2->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarPhibin2->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregPhibin2->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Phi() >= (-4 * M_PI) / (6) && LambdaCand[loop]->GetFourVector().Vect().Phi() < (-3 * M_PI) / (6)) {
            hLambdatotPhibin3->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarPhibin3->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregPhibin3->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Phi() >= (-3 * M_PI) / (6) && LambdaCand[loop]->GetFourVector().Vect().Phi() < (-2 * M_PI) / (6)) {
            hLambdatotPhibin4->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarPhibin4->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregPhibin4->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Phi() >= (-2 * M_PI) / (6) && LambdaCand[loop]->GetFourVector().Vect().Phi() < (-1 * M_PI) / (6)) {
            hLambdatotPhibin5->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarPhibin5->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregPhibin5->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Phi() >= (-1 * M_PI) / (6) && LambdaCand[loop]->GetFourVector().Vect().Phi() < 0) {
            hLambdatotPhibin6->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarPhibin6->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregPhibin6->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Phi() >= 0 && LambdaCand[loop]->GetFourVector().Vect().Phi() < (1 * M_PI) / (6)) {
            hLambdatotPhibin7->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarPhibin7->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregPhibin7->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Phi() >= (1 * M_PI) / (6) && LambdaCand[loop]->GetFourVector().Vect().Phi() < (2 * M_PI) / (6)) {
            hLambdatotPhibin8->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarPhibin8->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregPhibin8->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Phi() >= (2 * M_PI) / (6) && LambdaCand[loop]->GetFourVector().Vect().Phi() < (3 * M_PI) / (6)) {
            hLambdatotPhibin9->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarPhibin9->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregPhibin9->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Phi() >= (3 * M_PI) / (6) && LambdaCand[loop]->GetFourVector().Vect().Phi() < (4 * M_PI) / (6)) {
            hLambdatotPhibin10->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarPhibin10->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregPhibin10->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Phi() >= (4 * M_PI) / (6) && LambdaCand[loop]->GetFourVector().Vect().Phi() < (5 * M_PI) / (6)) {
            hLambdatotPhibin11->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarPhibin11->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregPhibin11->Fill(invMasslambda); }
        }
        if (LambdaCand[loop]->GetFourVector().Vect().Phi() >= (5 * M_PI) / (6) && LambdaCand[loop]->GetFourVector().Vect().Phi() < M_PI) {
            hLambdatotPhibin12->Fill(invMasslambda);
            if (LambdaCand[loop]->IsLambdaBar() == true) { hLambdabarPhibin12->Fill(invMasslambda); }
            if (LambdaCand[loop]->IsLambdaBar() != true) { hLambdaregPhibin12->Fill(invMasslambda); }
        }

       
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Getting peaks for doubly differential pT and Eta bins
        //TH1F* HistBins[9][10]; //HistBins[a][b] element corresponds to lambdas within pT bin a and Eta bin b
        
        //Filling hists
        for (int a = 0; a < 9; a++) {
            for (int b = 0; b < 10; b++) {
                if (pT > pTbins[a][0] && pT < pTbins[a][1] && LambdaCand[loop]->GetFourVector().Vect().Eta() > Etabins[b][0] && LambdaCand[loop]->GetFourVector().Vect().Eta() <Etabins[b][1]) {
                    if (LambdaCand[loop]->IsLambdaBar() == true) { HistBinsBar[a][b]->Fill(invMasslambda); }
                    if (LambdaCand[loop]->IsLambdaBar() != true) { HistBinsReg[a][b]->Fill(invMasslambda); }
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////





        //Sanity check to make sure my lambda sample makes it this far before I use it to try and get LambdaC
        hLambdasanitycheck->Fill(invMasslambda);







        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //LAMBDA C 
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Now starting new loop over H1PartCandArray to grab protons coming out of ventral vertex to get LambdaC candidates
        for (Int_t loop2 = 0; loop2 < PST.GetEntries(); loop2++) {

            
            if (LambdaCand[loop]->IsLambdaBar() == true && PST[loop2]->GetCharge() > 0) { continue; }

            if (LambdaCand[loop]->IsLambdaBar() != true && PST[loop2]->GetCharge() < 0) { continue; }

            if (PST[loop2]->IsFromPrimary() != true) { continue; }

            Double_t invMasslambdaC = (PST[loop2]->GetFourVector(mPion)
                 + LambdaCand[loop]->GetFourVector()).M();

            if (invMasslambdaC < 2.05 || invMasslambdaC > 2.55) { continue; }

            if (invMasslambda < 1.1 || invMasslambda > 1.135) { continue; }

            hC1->Fill(invMasslambdaC);

            hdEdxPionCBefore->Fill(PST[loop2]->GetMomentum().Mag(), PST[loop2]->GetDedx());

            if (PST[loop2]->GetDedxLikelihood(H1Dedx::kPion) < 0.001) {continue;}

            hdEdxPionCAfter->Fill(PST[loop2]->GetMomentum().Mag(), PST[loop2]->GetDedx());

            hC2->Fill(invMasslambdaC);

            //TObject* med = dynamic_cast<TObject*>(PST[loop2]);
            //H1Trajectory* CpionTrajectory = dynamic_cast<H1Trajectory*>(PST[loop2]);


            //if (!CpionTrajectory) { continue; }


            //const H1CentralTrack* pionFittedNoV = PionCFTrack->GetCentralTrack();
            //const H1Trajectory* pionTrajectory = pionFittedNoV->GetTrajectory();

            //H1SelVertex* ProtonVertex = (H1SelVertex*)LambdaCand[loop]->GetProtonTrack();
            //const H1SelVertex* ProtonPrimaryVertex = ProtonVertex->GetPrimaryVertex();
            //TVector3 pv = ProtonPrimaryVertex->GetPosition();//is same for both protons and pion daughters, so just using proton here and previous couple lines
            //TVector2 V2PV(pv.X(), pv.Y());
            //hdDCAproton->Fill((protonTrajectory->GetdDca(V2PV)));
            //hdDCApion->Fill((pionTrajectory->GetdDca(V2PV)));
            //hDCAproton->Fill((protonTrajectory->GetDca(V2PV)));
            //hDCApion->Fill((pionTrajectory->GetDca(V2PV)));
            //hDCAoverdDCAproton->Fill((protonTrajectory->GetDca(V2PV)) / (protonTrajectory->GetdDca(V2PV)));
            //hDCAoverdDCApion->Fill((pionTrajectory->GetDca(V2PV)) / (pionTrajectory->GetdDca(V2PV)));





            //if (abs((CpionTrajectory->GetDca(V2PV)) / (CpionTrajectory->GetdDca(V2PV))) < 1) { continue; }


            //hC3->Fill(invMasslambdaC);

            if (PST[loop2]->GetRadLength() < 10) {continue;}

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
            




            //if (LambdaCand[loop]->IsLambdaBar() == true && firstDaughter->GetCharge() > 0) { continue; }
            //if (LambdaCand[loop]->IsLambdaBar() != true && firstDaughter->GetCharge() < 0) { continue; }








            ////For Lambda want pi+, and antilambda want pi-
            //if (LambdaCand[loop]->IsLambdaBar() == true && PartCand[loop2]->GetCharge() > 0) { continue; }
            //if (LambdaCand[loop]->IsLambdaBar() != true && PartCand[loop2]->GetCharge() < 0) { continue; }

            ////PID Likelihood for Pion cut > 0.003
            ////const H1PartSelTrack* PionPairPSTrack = PartCand[loop2]->GetIDTrack();
            ////if (PionPairPSTrack->GetDedxLikelihood(H1Dedx::kPion) < 0.003) {
            ////    continue;
            ////}

            //if (!PartCand[loop2]->GetIDTrack()) { continue; } // Not all H1PartCand have ID tracks, must have this to avoid segmentation faults
            //

            //if(PartCand[loop2]->GetIDTrack()->IsCentralTrk()!=true) { continue; } // Want central tracks for pion to pair with Lambda
            //
            //Double_t invMasslambdaC = (PartCand[loop2]->GetFourVector(mPion)
            //    + LambdaCand[loop]->GetFourVector()).M();

            //Double_t invMasspionC = PartCand[loop2]->GetMass();
            ////Double_t invMasspionC = (PartCand[loop2]->GetFourVector(mPion)).M();

            ////cout << invMasslambdaC << endl;
            ////cout << invMasspionC << endl;


            //hCraw->Fill(invMasslambdaC);
            //hpraw->Fill(invMasspionC);


            ////H1CentralFittedTrack* PionPairCFT = (H1CentralFittedTrack*)PartCand[loop2]->GetIDTrack();
            ////if (!PionPairCFT) { continue; }

            //hdEdxPionCBefore->Fill(PartCand[loop2]->GetIDTrack()->GetMomentum().Mag(), PartCand[loop2]->GetIDTrack()->GetRawDedx());

            //hDedxLpionC->Fill(PartCand[loop2]->GetIDTrack()->GetDedxLikelihood(H1Dedx::kPion));
            ////PID Likelihood for Pion cut > 0.003
            //if (PartCand[loop2]->GetIDTrack()->GetDedxLikelihood(H1Dedx::kPion) < 0.003) {
            //    continue;
            //}
            //hdEdxPionCAfter->Fill(PartCand[loop2]->GetIDTrack()->GetMomentum().Mag(), PartCand[loop2]->GetIDTrack()->GetRawDedx());
            //

            //hCPID->Fill(invMasslambdaC);
            //hpPID->Fill(invMasspionC);


            //////SDCA cut
            ////////const H1CentralTrack* pionFittedNoV = PionCFTrack->GetCentralTrack();
            ////H1Trajectory* pionpairTrajectory = (H1Trajectory*)PartCand[loop2]->GetIDTrack();

            ////if (!pionpairTrajectory) { continue; }

            //////H1SelVertex* PionPairVertex = (H1SelVertex*)PartCand[loop2]->GetIDTrack();
            //////if (!PionPairVertex) { continue; }
            //////const H1SelVertex* PionPairPrimaryVertex = PionPairVertex->GetPrimaryVertex();
            //////TVector3 pv2 = PionPairPrimaryVertex->GetPosition();//is same for both protons and pion daughters, so just using proton here and previous couple lines
            //////TVector2 V2PVpair(pv2.X(), pv2.Y());
            //////if (!pionpairTrajectory->GetDca(V2PVpair)) { continue; }
            //////if (!pionpairTrajectory->GetdDca(V2PVpair)) { continue; }
            ////hDCAoverdDCApionC -> Fill((pionpairTrajectory->GetDca(V2PV)) / (pionpairTrajectory->GetdDca(V2PV)));
            ////if (abs((pionpairTrajectory->GetDca(V2PV)) / (pionpairTrajectory->GetdDca(V2PV))) < 1) {continue;}
            ////
            ////hCSDCA->Fill(invMasslambdaC);
            ////hpSDCA->Fill(invMasspionC);




            ////Pion Pair Rad Track Length > 10 cm cut
            //if (PartCand[loop2]->GetIDTrack()->GetRadLength() < 10) {
            //    continue;
            //}
            //hCTrackLength->Fill(invMasslambdaC);
            //hpTrackLength->Fill(invMasspionC);

            ////Pion pT>0.2GeV as in Steinhart thesis
            //if (PartCand[loop2]->GetPt() < 0.2) {
            //    continue;
            //}
            ////This is the plot for after all cuts
            //hCpionptcut->Fill(invMasslambdaC);
            //hpionptcut->Fill(invMasspionC);
           
            
        }
        //
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


       
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
       ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
       //Writing histograms for pT and Eta differential peaks
       for (int a = 0; a < 9; a++) {
           for (int b = 0; b < 10; b++) {
               HistBinsReg[a][b]->SetStats(0);
               HistBinsReg[a][b]->SetLineColor(kBlack);
               HistBinsReg[a][b]->GetXaxis()->SetTitle("Mass [Gev]");
               HistBinsReg[a][b]->GetYaxis()->SetTitle("Count");
               HistBinsReg[a][b]->Draw();
               HistBinsReg[a][b]->Write();

               HistBinsBar[a][b]->SetStats(0);
               HistBinsBar[a][b]->SetLineColor(kBlack);
               HistBinsBar[a][b]->GetXaxis()->SetTitle("Mass [Gev]");
               HistBinsBar[a][b]->GetYaxis()->SetTitle("Count");
               HistBinsBar[a][b]->Draw();
               HistBinsBar[a][b]->Write();
           }
       }
       ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
       //Roughing Lambda counts by enforcing invariant mass range since BG:Signal ratio is good and filling 2D plots
       double lambdaregcount = 0;
       double lambdabarcount = 0;

       for (int a = 0; a < 9; a++) {
           for (int b = 0; b < 10; b++) {
               double lambdaregcount = 0;
               double lambdabarcount = 0;

               //Getting the anti-lambda count for this particular (a,b)=(pT,Eta) bin
               for (int m = 1; m < HistBinsBar[a][b]->GetNbinsX() + 1; m++) {
                   if (HistBinsBar[a][b]->GetBinCenter(m) > lambdalowerbound && HistBinsBar[a][b]->GetBinCenter(m) < lambdaupperbound) {
                       lambdabarcount = lambdabarcount + HistBinsBar[a][b]->GetBinContent(m);
                   }
               }

               //Now setting 2D pT vs Eta anti lambda hist bin content for (a,b)=(pT,Eta)
               for (int l = 1; l < LambdaBar2D->GetNbinsX() + 1; l++) {
                   for (int k = 1; k < LambdaBar2D->GetNbinsY() + 1; k++) {
                       if (LambdaBar2D->GetXaxis()->GetBinCenter(l) > pTbins[a][0] && LambdaBar2D->GetXaxis()->GetBinCenter(l) < pTbins[a][1] && LambdaBar2D->GetYaxis()->GetBinCenter(k) > Etabins[b][0] && LambdaBar2D->GetYaxis()->GetBinCenter(k) < Etabins[b][1]) {
                           LambdaBar2D->SetBinContent(l, k, lambdabarcount);
                       }
                   }
               }

               //Getting the reg lambda count for this particular (a,b)=(pT,Eta) bin
               for (int m = 1; m < HistBinsReg[a][b]->GetNbinsX() + 1; m++) {
                   if (HistBinsReg[a][b]->GetBinCenter(m) > lambdalowerbound && HistBinsReg[a][b]->GetBinCenter(m) < lambdaupperbound) {
                       lambdaregcount = lambdaregcount + HistBinsReg[a][b]->GetBinContent(m);
                   }
               }

               //Now setting 2D pT vs Eta reg lambda hist bin content for (a,b)=(pT,Eta)
               for (int l = 1; l < LambdaReg2D->GetNbinsX() + 1; l++) {
                   for (int k = 1; k < LambdaReg2D->GetNbinsY() + 1; k++) {
                       if (LambdaReg2D->GetXaxis()->GetBinCenter(l) > pTbins[a][0] && LambdaReg2D->GetXaxis()->GetBinCenter(l) < pTbins[a][1] && LambdaReg2D->GetYaxis()->GetBinCenter(k) > Etabins[b][0] && LambdaReg2D->GetYaxis()->GetBinCenter(k) < Etabins[b][1]) {
                           LambdaReg2D->SetBinContent(l, k, lambdaregcount);
                           //Now at this point I have the reg and anti-lambda counts and can also fill the 2D Asymmetry pT vs Eta hist
                           Asymm2D->SetBinContent(l, k, (lambdaregcount- lambdabarcount)/(lambdaregcount + lambdabarcount));
                       }
                   }
               }

           }
       }
       //Write 2D hists
       LambdaBar2D->SetStats(0);
       LambdaBar2D->GetXaxis()->SetTitle("pT [GeV]");
       LambdaBar2D->GetYaxis()->SetTitle("Eta");
       LambdaBar2D->GetXaxis()->SetTitleSize(0.04);
       LambdaBar2D->GetXaxis()->SetTitleOffset(1);
       LambdaBar2D->GetYaxis()->SetTitleSize(0.05);
       LambdaBar2D->Draw("colz");
       canvas->SetName("Anti Lambda pT vs Eta");
       canvas->Write();

       LambdaReg2D->SetStats(0);
       LambdaReg2D->GetXaxis()->SetTitle("pT [GeV]");
       LambdaReg2D->GetYaxis()->SetTitle("Eta");
       LambdaReg2D->GetXaxis()->SetTitleSize(0.04);
       LambdaReg2D->GetXaxis()->SetTitleOffset(1);
       LambdaReg2D->GetYaxis()->SetTitleSize(0.05);
       LambdaReg2D->Draw("colz");
       canvas->SetName("Reg Lambda pT vs Eta");
       canvas->Write();

       Asymm2D->SetStats(0);
       Asymm2D->GetXaxis()->SetTitle("pT [GeV]");
       Asymm2D->GetYaxis()->SetTitle("Eta");
       Asymm2D->GetXaxis()->SetTitleSize(0.04);
       Asymm2D->GetXaxis()->SetTitleOffset(1);
       Asymm2D->GetYaxis()->SetTitleSize(0.05);
       Asymm2D->Draw("colz");
       canvas->SetName("Lambda Asymmetry pT vs Eta");
       canvas->Write();

       ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
       
       
       hLambdasanitycheck->SetStats(0);
       hLambdasanitycheck->SetLineColor(kBlack);
       hLambdasanitycheck->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdasanitycheck->GetYaxis()->SetTitle("Count");
       hLambdasanitycheck->Draw();
       hLambdasanitycheck->Write();
       
       
       
       //plots uploading
       hLambdatotQ2bin1->SetStats(0);
       hLambdatotQ2bin1->SetLineColor(kBlack);
       hLambdatotQ2bin1->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotQ2bin1->GetYaxis()->SetTitle("Count");
       hLambdatotQ2bin1->Draw();
       hLambdatotQ2bin1->Write();

       hLambdatotQ2bin2->SetStats(0);
       hLambdatotQ2bin2->SetLineColor(kBlack);
       hLambdatotQ2bin2->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotQ2bin2->GetYaxis()->SetTitle("Count");
       hLambdatotQ2bin2->Draw();
       hLambdatotQ2bin2->Write();

       hLambdatotQ2bin3->SetStats(0);
       hLambdatotQ2bin3->SetLineColor(kBlack);
       hLambdatotQ2bin3->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotQ2bin3->GetYaxis()->SetTitle("Count");
       hLambdatotQ2bin3->Draw();
       hLambdatotQ2bin3->Write();

       hLambdatotQ2bin4->SetStats(0);
       hLambdatotQ2bin4->SetLineColor(kBlack);
       hLambdatotQ2bin4->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotQ2bin4->GetYaxis()->SetTitle("Count");
       hLambdatotQ2bin4->Draw();
       hLambdatotQ2bin4->Write();

       hLambdatotQ2bin5->SetStats(0);
       hLambdatotQ2bin5->SetLineColor(kBlack);
       hLambdatotQ2bin5->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotQ2bin5->GetYaxis()->SetTitle("Count");
       hLambdatotQ2bin5->Draw();
       hLambdatotQ2bin5->Write();

       hLambdatotQ2bin6->SetStats(0);
       hLambdatotQ2bin6->SetLineColor(kBlack);
       hLambdatotQ2bin6->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotQ2bin6->GetYaxis()->SetTitle("Count");
       hLambdatotQ2bin6->Draw();
       hLambdatotQ2bin6->Write();

       hLambdatotQ2bin7->SetStats(0);
       hLambdatotQ2bin7->SetLineColor(kBlack);
       hLambdatotQ2bin7->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotQ2bin7->GetYaxis()->SetTitle("Count");
       hLambdatotQ2bin7->Draw();
       hLambdatotQ2bin7->Write();

       hLambdatotQ2bin8->SetStats(0);
       hLambdatotQ2bin8->SetLineColor(kBlack);
       hLambdatotQ2bin8->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotQ2bin8->GetYaxis()->SetTitle("Count");
       hLambdatotQ2bin8->Draw();
       hLambdatotQ2bin8->Write();

       hLambdatotQ2bin9->SetStats(0);
       hLambdatotQ2bin9->SetLineColor(kBlack);
       hLambdatotQ2bin9->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotQ2bin9->GetYaxis()->SetTitle("Count");
       hLambdatotQ2bin9->Draw();
       hLambdatotQ2bin9->Write();

       hLambdatotXbin1->SetStats(0);
       hLambdatotXbin1->SetLineColor(kBlack);
       hLambdatotXbin1->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotXbin1->GetYaxis()->SetTitle("Count");
       hLambdatotXbin1->Draw();
       hLambdatotXbin1->Write();

       hLambdatotXbin2->SetStats(0);
       hLambdatotXbin2->SetLineColor(kBlack);
       hLambdatotXbin2->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotXbin2->GetYaxis()->SetTitle("Count");
       hLambdatotXbin2->Draw();
       hLambdatotXbin2->Write();

       hLambdatotXbin3->SetStats(0);
       hLambdatotXbin3->SetLineColor(kBlack);
       hLambdatotXbin3->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotXbin3->GetYaxis()->SetTitle("Count");
       hLambdatotXbin3->Draw();
       hLambdatotXbin3->Write();

       hLambdatotXbin4->SetStats(0);
       hLambdatotXbin4->SetLineColor(kBlack);
       hLambdatotXbin4->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotXbin4->GetYaxis()->SetTitle("Count");
       hLambdatotXbin4->Draw();
       hLambdatotXbin4->Write();

       hLambdatotXbin5->SetStats(0);
       hLambdatotXbin5->SetLineColor(kBlack);
       hLambdatotXbin5->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotXbin5->GetYaxis()->SetTitle("Count");
       hLambdatotXbin5->Draw();
       hLambdatotXbin5->Write();

       hLambdatotEtabin1->SetStats(0);
       hLambdatotEtabin1->SetLineColor(kBlack);
       hLambdatotEtabin1->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotEtabin1->GetYaxis()->SetTitle("Count");
       hLambdatotEtabin1->Draw();
       hLambdatotEtabin1->Write();

       hLambdatotEtabin2->SetStats(0);
       hLambdatotEtabin2->SetLineColor(kBlack);
       hLambdatotEtabin2->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotEtabin2->GetYaxis()->SetTitle("Count");
       hLambdatotEtabin2->Draw();
       hLambdatotEtabin2->Write();

       hLambdatotEtabin3->SetStats(0);
       hLambdatotEtabin3->SetLineColor(kBlack);
       hLambdatotEtabin3->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotEtabin3->GetYaxis()->SetTitle("Count");
       hLambdatotEtabin3->Draw();
       hLambdatotEtabin3->Write();

       hLambdatotEtabin4->SetStats(0);
       hLambdatotEtabin4->SetLineColor(kBlack);
       hLambdatotEtabin4->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotEtabin4->GetYaxis()->SetTitle("Count");
       hLambdatotEtabin4->Draw();
       hLambdatotEtabin4->Write();

       hLambdatotEtabin5->SetStats(0);
       hLambdatotEtabin5->SetLineColor(kBlack);
       hLambdatotEtabin5->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotEtabin5->GetYaxis()->SetTitle("Count");
       hLambdatotEtabin5->Draw();
       hLambdatotEtabin5->Write();

       hLambdatotEtabin6->SetStats(0);
       hLambdatotEtabin6->SetLineColor(kBlack);
       hLambdatotEtabin6->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotEtabin6->GetYaxis()->SetTitle("Count");
       hLambdatotEtabin6->Draw();
       hLambdatotEtabin6->Write();

       hLambdatotEtabin7->SetStats(0);
       hLambdatotEtabin7->SetLineColor(kBlack);
       hLambdatotEtabin7->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotEtabin7->GetYaxis()->SetTitle("Count");
       hLambdatotEtabin7->Draw();
       hLambdatotEtabin7->Write();

       hLambdatotEtabin8->SetStats(0);
       hLambdatotEtabin8->SetLineColor(kBlack);
       hLambdatotEtabin8->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotEtabin8->GetYaxis()->SetTitle("Count");
       hLambdatotEtabin8->Draw();
       hLambdatotEtabin8->Write();

       hLambdatotEtabin9->SetStats(0);
       hLambdatotEtabin9->SetLineColor(kBlack);
       hLambdatotEtabin9->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotEtabin9->GetYaxis()->SetTitle("Count");
       hLambdatotEtabin9->Draw();
       hLambdatotEtabin9->Write();

       hLambdatotEtabin10->SetStats(0);
       hLambdatotEtabin10->SetLineColor(kBlack);
       hLambdatotEtabin10->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotEtabin10->GetYaxis()->SetTitle("Count");
       hLambdatotEtabin10->Draw();
       hLambdatotEtabin10->Write();

       hLambdatotPhibin1->SetStats(0);
       hLambdatotPhibin1->SetLineColor(kBlack);
       hLambdatotPhibin1->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotPhibin1->GetYaxis()->SetTitle("Count");
       hLambdatotPhibin1->Draw();
       hLambdatotPhibin1->Write();

       hLambdatotPhibin2->SetStats(0);
       hLambdatotPhibin2->SetLineColor(kBlack);
       hLambdatotPhibin2->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotPhibin2->GetYaxis()->SetTitle("Count");
       hLambdatotPhibin2->Draw();
       hLambdatotPhibin2->Write();

       hLambdatotPhibin3->SetStats(0);
       hLambdatotPhibin3->SetLineColor(kBlack);
       hLambdatotPhibin3->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotPhibin3->GetYaxis()->SetTitle("Count");
       hLambdatotPhibin3->Draw();
       hLambdatotPhibin3->Write();

       hLambdatotPhibin4->SetStats(0);
       hLambdatotPhibin4->SetLineColor(kBlack);
       hLambdatotPhibin4->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotPhibin4->GetYaxis()->SetTitle("Count");
       hLambdatotPhibin4->Draw();
       hLambdatotPhibin4->Write();

       hLambdatotPhibin5->SetStats(0);
       hLambdatotPhibin5->SetLineColor(kBlack);
       hLambdatotPhibin5->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotPhibin5->GetYaxis()->SetTitle("Count");
       hLambdatotPhibin5->Draw();
       hLambdatotPhibin5->Write();

       hLambdatotPhibin6->SetStats(0);
       hLambdatotPhibin6->SetLineColor(kBlack);
       hLambdatotPhibin6->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotPhibin6->GetYaxis()->SetTitle("Count");
       hLambdatotPhibin6->Draw();
       hLambdatotPhibin6->Write();

       hLambdatotPhibin7->SetStats(0);
       hLambdatotPhibin7->SetLineColor(kBlack);
       hLambdatotPhibin7->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotPhibin7->GetYaxis()->SetTitle("Count");
       hLambdatotPhibin7->Draw();
       hLambdatotPhibin7->Write();

       hLambdatotPhibin8->SetStats(0);
       hLambdatotPhibin8->SetLineColor(kBlack);
       hLambdatotPhibin8->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotPhibin8->GetYaxis()->SetTitle("Count");
       hLambdatotPhibin8->Draw();
       hLambdatotPhibin8->Write();

       hLambdatotPhibin9->SetStats(0);
       hLambdatotPhibin9->SetLineColor(kBlack);
       hLambdatotPhibin9->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotPhibin9->GetYaxis()->SetTitle("Count");
       hLambdatotPhibin9->Draw();
       hLambdatotPhibin9->Write();

       hLambdatotPhibin10->SetStats(0);
       hLambdatotPhibin10->SetLineColor(kBlack);
       hLambdatotPhibin10->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotPhibin10->GetYaxis()->SetTitle("Count");
       hLambdatotPhibin10->Draw();
       hLambdatotPhibin10->Write();

       hLambdatotPhibin11->SetStats(0);
       hLambdatotPhibin11->SetLineColor(kBlack);
       hLambdatotPhibin11->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotPhibin11->GetYaxis()->SetTitle("Count");
       hLambdatotPhibin11->Draw();
       hLambdatotPhibin11->Write();

       hLambdatotPhibin12->SetStats(0);
       hLambdatotPhibin12->SetLineColor(kBlack);
       hLambdatotPhibin12->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotPhibin12->GetYaxis()->SetTitle("Count");
       hLambdatotPhibin12->Draw();
       hLambdatotPhibin12->Write();

       hLambdatotpTbin1->SetStats(0);
       hLambdatotpTbin1->SetLineColor(kBlack);
       hLambdatotpTbin1->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotpTbin1->GetYaxis()->SetTitle("Count");
       hLambdatotpTbin1->Draw();
       hLambdatotpTbin1->Write();

       hLambdatotpTbin2->SetStats(0);
       hLambdatotpTbin2->SetLineColor(kBlack);
       hLambdatotpTbin2->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotpTbin2->GetYaxis()->SetTitle("Count");
       hLambdatotpTbin2->Draw();
       hLambdatotpTbin2->Write();

       hLambdatotpTbin3->SetStats(0);
       hLambdatotpTbin3->SetLineColor(kBlack);
       hLambdatotpTbin3->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotpTbin3->GetYaxis()->SetTitle("Count");
       hLambdatotpTbin3->Draw();
       hLambdatotpTbin3->Write();

       hLambdatotpTbin4->SetStats(0);
       hLambdatotpTbin4->SetLineColor(kBlack);
       hLambdatotpTbin4->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotpTbin4->GetYaxis()->SetTitle("Count");
       hLambdatotpTbin4->Draw();
       hLambdatotpTbin4->Write();

       hLambdatotpTbin5->SetStats(0);
       hLambdatotpTbin5->SetLineColor(kBlack);
       hLambdatotpTbin5->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotpTbin5->GetYaxis()->SetTitle("Count");
       hLambdatotpTbin5->Draw();
       hLambdatotpTbin5->Write();

       hLambdatotpTbin6->SetStats(0);
       hLambdatotpTbin6->SetLineColor(kBlack);
       hLambdatotpTbin6->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotpTbin6->GetYaxis()->SetTitle("Count");
       hLambdatotpTbin6->Draw();
       hLambdatotpTbin6->Write();

       hLambdatotpTbin7->SetStats(0);
       hLambdatotpTbin7->SetLineColor(kBlack);
       hLambdatotpTbin7->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotpTbin7->GetYaxis()->SetTitle("Count");
       hLambdatotpTbin7->Draw();
       hLambdatotpTbin7->Write();

       hLambdatotpTbin8->SetStats(0);
       hLambdatotpTbin8->SetLineColor(kBlack);
       hLambdatotpTbin8->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotpTbin8->GetYaxis()->SetTitle("Count");
       hLambdatotpTbin8->Draw();
       hLambdatotpTbin8->Write();

       hLambdatotpTbin9->SetStats(0);
       hLambdatotpTbin9->SetLineColor(kBlack);
       hLambdatotpTbin9->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdatotpTbin9->GetYaxis()->SetTitle("Count");
       hLambdatotpTbin9->Draw();
       hLambdatotpTbin9->Write();

       //
       hLambdaregQ2bin1->SetStats(0);
       hLambdaregQ2bin1->SetLineColor(kBlack);
       hLambdaregQ2bin1->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregQ2bin1->GetYaxis()->SetTitle("Count");
       hLambdaregQ2bin1->Draw();
       hLambdaregQ2bin1->Write();

       hLambdaregQ2bin2->SetStats(0);
       hLambdaregQ2bin2->SetLineColor(kBlack);
       hLambdaregQ2bin2->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregQ2bin2->GetYaxis()->SetTitle("Count");
       hLambdaregQ2bin2->Draw();
       hLambdaregQ2bin2->Write();

       hLambdaregQ2bin3->SetStats(0);
       hLambdaregQ2bin3->SetLineColor(kBlack);
       hLambdaregQ2bin3->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregQ2bin3->GetYaxis()->SetTitle("Count");
       hLambdaregQ2bin3->Draw();
       hLambdaregQ2bin3->Write();

       hLambdaregQ2bin4->SetStats(0);
       hLambdaregQ2bin4->SetLineColor(kBlack);
       hLambdaregQ2bin4->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregQ2bin4->GetYaxis()->SetTitle("Count");
       hLambdaregQ2bin4->Draw();
       hLambdaregQ2bin4->Write();

       hLambdaregQ2bin5->SetStats(0);
       hLambdaregQ2bin5->SetLineColor(kBlack);
       hLambdaregQ2bin5->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregQ2bin5->GetYaxis()->SetTitle("Count");
       hLambdaregQ2bin5->Draw();
       hLambdaregQ2bin5->Write();

       hLambdaregQ2bin6->SetStats(0);
       hLambdaregQ2bin6->SetLineColor(kBlack);
       hLambdaregQ2bin6->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregQ2bin6->GetYaxis()->SetTitle("Count");
       hLambdaregQ2bin6->Draw();
       hLambdaregQ2bin6->Write();

       hLambdaregQ2bin7->SetStats(0);
       hLambdaregQ2bin7->SetLineColor(kBlack);
       hLambdaregQ2bin7->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregQ2bin7->GetYaxis()->SetTitle("Count");
       hLambdaregQ2bin7->Draw();
       hLambdaregQ2bin7->Write();

       hLambdaregQ2bin8->SetStats(0);
       hLambdaregQ2bin8->SetLineColor(kBlack);
       hLambdaregQ2bin8->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregQ2bin8->GetYaxis()->SetTitle("Count");
       hLambdaregQ2bin8->Draw();
       hLambdaregQ2bin8->Write();

       hLambdaregQ2bin9->SetStats(0);
       hLambdaregQ2bin9->SetLineColor(kBlack);
       hLambdaregQ2bin9->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregQ2bin9->GetYaxis()->SetTitle("Count");
       hLambdaregQ2bin9->Draw();
       hLambdaregQ2bin9->Write();

       hLambdaregXbin1->SetStats(0);
       hLambdaregXbin1->SetLineColor(kBlack);
       hLambdaregXbin1->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregXbin1->GetYaxis()->SetTitle("Count");
       hLambdaregXbin1->Draw();
       hLambdaregXbin1->Write();

       hLambdaregXbin2->SetStats(0);
       hLambdaregXbin2->SetLineColor(kBlack);
       hLambdaregXbin2->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregXbin2->GetYaxis()->SetTitle("Count");
       hLambdaregXbin2->Draw();
       hLambdaregXbin2->Write();

       hLambdaregXbin3->SetStats(0);
       hLambdaregXbin3->SetLineColor(kBlack);
       hLambdaregXbin3->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregXbin3->GetYaxis()->SetTitle("Count");
       hLambdaregXbin3->Draw();
       hLambdaregXbin3->Write();

       hLambdaregXbin4->SetStats(0);
       hLambdaregXbin4->SetLineColor(kBlack);
       hLambdaregXbin4->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregXbin4->GetYaxis()->SetTitle("Count");
       hLambdaregXbin4->Draw();
       hLambdaregXbin4->Write();

       hLambdaregXbin5->SetStats(0);
       hLambdaregXbin5->SetLineColor(kBlack);
       hLambdaregXbin5->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregXbin5->GetYaxis()->SetTitle("Count");
       hLambdaregXbin5->Draw();
       hLambdaregXbin5->Write();

       hLambdaregEtabin1->SetStats(0);
       hLambdaregEtabin1->SetLineColor(kBlack);
       hLambdaregEtabin1->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregEtabin1->GetYaxis()->SetTitle("Count");
       hLambdaregEtabin1->Draw();
       hLambdaregEtabin1->Write();

       hLambdaregEtabin2->SetStats(0);
       hLambdaregEtabin2->SetLineColor(kBlack);
       hLambdaregEtabin2->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregEtabin2->GetYaxis()->SetTitle("Count");
       hLambdaregEtabin2->Draw();
       hLambdaregEtabin2->Write();

       hLambdaregEtabin3->SetStats(0);
       hLambdaregEtabin3->SetLineColor(kBlack);
       hLambdaregEtabin3->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregEtabin3->GetYaxis()->SetTitle("Count");
       hLambdaregEtabin3->Draw();
       hLambdaregEtabin3->Write();

       hLambdaregEtabin4->SetStats(0);
       hLambdaregEtabin4->SetLineColor(kBlack);
       hLambdaregEtabin4->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregEtabin4->GetYaxis()->SetTitle("Count");
       hLambdaregEtabin4->Draw();
       hLambdaregEtabin4->Write();

       hLambdaregEtabin5->SetStats(0);
       hLambdaregEtabin5->SetLineColor(kBlack);
       hLambdaregEtabin5->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregEtabin5->GetYaxis()->SetTitle("Count");
       hLambdaregEtabin5->Draw();
       hLambdaregEtabin5->Write();

       hLambdaregEtabin6->SetStats(0);
       hLambdaregEtabin6->SetLineColor(kBlack);
       hLambdaregEtabin6->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregEtabin6->GetYaxis()->SetTitle("Count");
       hLambdaregEtabin6->Draw();
       hLambdaregEtabin6->Write();

       hLambdaregEtabin7->SetStats(0);
       hLambdaregEtabin7->SetLineColor(kBlack);
       hLambdaregEtabin7->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregEtabin7->GetYaxis()->SetTitle("Count");
       hLambdaregEtabin7->Draw();
       hLambdaregEtabin7->Write();

       hLambdaregEtabin8->SetStats(0);
       hLambdaregEtabin8->SetLineColor(kBlack);
       hLambdaregEtabin8->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregEtabin8->GetYaxis()->SetTitle("Count");
       hLambdaregEtabin8->Draw();
       hLambdaregEtabin8->Write();

       hLambdaregEtabin9->SetStats(0);
       hLambdaregEtabin9->SetLineColor(kBlack);
       hLambdaregEtabin9->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregEtabin9->GetYaxis()->SetTitle("Count");
       hLambdaregEtabin9->Draw();
       hLambdaregEtabin9->Write();

       hLambdaregEtabin10->SetStats(0);
       hLambdaregEtabin10->SetLineColor(kBlack);
       hLambdaregEtabin10->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregEtabin10->GetYaxis()->SetTitle("Count");
       hLambdaregEtabin10->Draw();
       hLambdaregEtabin10->Write();

       hLambdaregPhibin1->SetStats(0);
       hLambdaregPhibin1->SetLineColor(kBlack);
       hLambdaregPhibin1->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregPhibin1->GetYaxis()->SetTitle("Count");
       hLambdaregPhibin1->Draw();
       hLambdaregPhibin1->Write();

       hLambdaregPhibin2->SetStats(0);
       hLambdaregPhibin2->SetLineColor(kBlack);
       hLambdaregPhibin2->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregPhibin2->GetYaxis()->SetTitle("Count");
       hLambdaregPhibin2->Draw();
       hLambdaregPhibin2->Write();

       hLambdaregPhibin3->SetStats(0);
       hLambdaregPhibin3->SetLineColor(kBlack);
       hLambdaregPhibin3->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregPhibin3->GetYaxis()->SetTitle("Count");
       hLambdaregPhibin3->Draw();
       hLambdaregPhibin3->Write();

       hLambdaregPhibin4->SetStats(0);
       hLambdaregPhibin4->SetLineColor(kBlack);
       hLambdaregPhibin4->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregPhibin4->GetYaxis()->SetTitle("Count");
       hLambdaregPhibin4->Draw();
       hLambdaregPhibin4->Write();

       hLambdaregPhibin5->SetStats(0);
       hLambdaregPhibin5->SetLineColor(kBlack);
       hLambdaregPhibin5->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregPhibin5->GetYaxis()->SetTitle("Count");
       hLambdaregPhibin5->Draw();
       hLambdaregPhibin5->Write();

       hLambdaregPhibin6->SetStats(0);
       hLambdaregPhibin6->SetLineColor(kBlack);
       hLambdaregPhibin6->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregPhibin6->GetYaxis()->SetTitle("Count");
       hLambdaregPhibin6->Draw();
       hLambdaregPhibin6->Write();

       hLambdaregPhibin7->SetStats(0);
       hLambdaregPhibin7->SetLineColor(kBlack);
       hLambdaregPhibin7->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregPhibin7->GetYaxis()->SetTitle("Count");
       hLambdaregPhibin7->Draw();
       hLambdaregPhibin7->Write();

       hLambdaregPhibin8->SetStats(0);
       hLambdaregPhibin8->SetLineColor(kBlack);
       hLambdaregPhibin8->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregPhibin8->GetYaxis()->SetTitle("Count");
       hLambdaregPhibin8->Draw();
       hLambdaregPhibin8->Write();

       hLambdaregPhibin9->SetStats(0);
       hLambdaregPhibin9->SetLineColor(kBlack);
       hLambdaregPhibin9->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregPhibin9->GetYaxis()->SetTitle("Count");
       hLambdaregPhibin9->Draw();
       hLambdaregPhibin9->Write();

       hLambdaregPhibin10->SetStats(0);
       hLambdaregPhibin10->SetLineColor(kBlack);
       hLambdaregPhibin10->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregPhibin10->GetYaxis()->SetTitle("Count");
       hLambdaregPhibin10->Draw();
       hLambdaregPhibin10->Write();

       hLambdaregPhibin11->SetStats(0);
       hLambdaregPhibin11->SetLineColor(kBlack);
       hLambdaregPhibin11->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregPhibin11->GetYaxis()->SetTitle("Count");
       hLambdaregPhibin11->Draw();
       hLambdaregPhibin11->Write();

       hLambdaregPhibin12->SetStats(0);
       hLambdaregPhibin12->SetLineColor(kBlack);
       hLambdaregPhibin12->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregPhibin12->GetYaxis()->SetTitle("Count");
       hLambdaregPhibin12->Draw();
       hLambdaregPhibin12->Write();

       hLambdaregpTbin1->SetStats(0);
       hLambdaregpTbin1->SetLineColor(kBlack);
       hLambdaregpTbin1->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregpTbin1->GetYaxis()->SetTitle("Count");
       hLambdaregpTbin1->Draw();
       hLambdaregpTbin1->Write();

       hLambdaregpTbin2->SetStats(0);
       hLambdaregpTbin2->SetLineColor(kBlack);
       hLambdaregpTbin2->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregpTbin2->GetYaxis()->SetTitle("Count");
       hLambdaregpTbin2->Draw();
       hLambdaregpTbin2->Write();

       hLambdaregpTbin3->SetStats(0);
       hLambdaregpTbin3->SetLineColor(kBlack);
       hLambdaregpTbin3->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregpTbin3->GetYaxis()->SetTitle("Count");
       hLambdaregpTbin3->Draw();
       hLambdaregpTbin3->Write();

       hLambdaregpTbin4->SetStats(0);
       hLambdaregpTbin4->SetLineColor(kBlack);
       hLambdaregpTbin4->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregpTbin4->GetYaxis()->SetTitle("Count");
       hLambdaregpTbin4->Draw();
       hLambdaregpTbin4->Write();

       hLambdaregpTbin5->SetStats(0);
       hLambdaregpTbin5->SetLineColor(kBlack);
       hLambdaregpTbin5->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregpTbin5->GetYaxis()->SetTitle("Count");
       hLambdaregpTbin5->Draw();
       hLambdaregpTbin5->Write();

       hLambdaregpTbin6->SetStats(0);
       hLambdaregpTbin6->SetLineColor(kBlack);
       hLambdaregpTbin6->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregpTbin6->GetYaxis()->SetTitle("Count");
       hLambdaregpTbin6->Draw();
       hLambdaregpTbin6->Write();

       hLambdaregpTbin7->SetStats(0);
       hLambdaregpTbin7->SetLineColor(kBlack);
       hLambdaregpTbin7->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregpTbin7->GetYaxis()->SetTitle("Count");
       hLambdaregpTbin7->Draw();
       hLambdaregpTbin7->Write();

       hLambdaregpTbin8->SetStats(0);
       hLambdaregpTbin8->SetLineColor(kBlack);
       hLambdaregpTbin8->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregpTbin8->GetYaxis()->SetTitle("Count");
       hLambdaregpTbin8->Draw();
       hLambdaregpTbin8->Write();

       hLambdaregpTbin9->SetStats(0);
       hLambdaregpTbin9->SetLineColor(kBlack);
       hLambdaregpTbin9->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaregpTbin9->GetYaxis()->SetTitle("Count");
       hLambdaregpTbin9->Draw();
       hLambdaregpTbin9->Write();
       
       //
       hLambdabarQ2bin1->SetStats(0);
       hLambdabarQ2bin1->SetLineColor(kBlack);
       hLambdabarQ2bin1->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarQ2bin1->GetYaxis()->SetTitle("Count");
       hLambdabarQ2bin1->Draw();
       hLambdabarQ2bin1->Write();

       hLambdabarQ2bin2->SetStats(0);
       hLambdabarQ2bin2->SetLineColor(kBlack);
       hLambdabarQ2bin2->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarQ2bin2->GetYaxis()->SetTitle("Count");
       hLambdabarQ2bin2->Draw();
       hLambdabarQ2bin2->Write();

       hLambdabarQ2bin3->SetStats(0);
       hLambdabarQ2bin3->SetLineColor(kBlack);
       hLambdabarQ2bin3->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarQ2bin3->GetYaxis()->SetTitle("Count");
       hLambdabarQ2bin3->Draw();
       hLambdabarQ2bin3->Write();

       hLambdabarQ2bin4->SetStats(0);
       hLambdabarQ2bin4->SetLineColor(kBlack);
       hLambdabarQ2bin4->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarQ2bin4->GetYaxis()->SetTitle("Count");
       hLambdabarQ2bin4->Draw();
       hLambdabarQ2bin4->Write();

       hLambdabarQ2bin5->SetStats(0);
       hLambdabarQ2bin5->SetLineColor(kBlack);
       hLambdabarQ2bin5->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarQ2bin5->GetYaxis()->SetTitle("Count");
       hLambdabarQ2bin5->Draw();
       hLambdabarQ2bin5->Write();

       hLambdabarQ2bin6->SetStats(0);
       hLambdabarQ2bin6->SetLineColor(kBlack);
       hLambdabarQ2bin6->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarQ2bin6->GetYaxis()->SetTitle("Count");
       hLambdabarQ2bin6->Draw();
       hLambdabarQ2bin6->Write();

       hLambdabarQ2bin7->SetStats(0);
       hLambdabarQ2bin7->SetLineColor(kBlack);
       hLambdabarQ2bin7->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarQ2bin7->GetYaxis()->SetTitle("Count");
       hLambdabarQ2bin7->Draw();
       hLambdabarQ2bin7->Write();

       hLambdabarQ2bin8->SetStats(0);
       hLambdabarQ2bin8->SetLineColor(kBlack);
       hLambdabarQ2bin8->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarQ2bin8->GetYaxis()->SetTitle("Count");
       hLambdabarQ2bin8->Draw();
       hLambdabarQ2bin8->Write();

       hLambdabarQ2bin9->SetStats(0);
       hLambdabarQ2bin9->SetLineColor(kBlack);
       hLambdabarQ2bin9->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarQ2bin9->GetYaxis()->SetTitle("Count");
       hLambdabarQ2bin9->Draw();
       hLambdabarQ2bin9->Write();

       hLambdabarXbin1->SetStats(0);
       hLambdabarXbin1->SetLineColor(kBlack);
       hLambdabarXbin1->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarXbin1->GetYaxis()->SetTitle("Count");
       hLambdabarXbin1->Draw();
       hLambdabarXbin1->Write();

       hLambdabarXbin2->SetStats(0);
       hLambdabarXbin2->SetLineColor(kBlack);
       hLambdabarXbin2->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarXbin2->GetYaxis()->SetTitle("Count");
       hLambdabarXbin2->Draw();
       hLambdabarXbin2->Write();

       hLambdabarXbin3->SetStats(0);
       hLambdabarXbin3->SetLineColor(kBlack);
       hLambdabarXbin3->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarXbin3->GetYaxis()->SetTitle("Count");
       hLambdabarXbin3->Draw();
       hLambdabarXbin3->Write();

       hLambdabarXbin4->SetStats(0);
       hLambdabarXbin4->SetLineColor(kBlack);
       hLambdabarXbin4->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarXbin4->GetYaxis()->SetTitle("Count");
       hLambdabarXbin4->Draw();
       hLambdabarXbin4->Write();

       hLambdabarXbin5->SetStats(0);
       hLambdabarXbin5->SetLineColor(kBlack);
       hLambdabarXbin5->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarXbin5->GetYaxis()->SetTitle("Count");
       hLambdabarXbin5->Draw();
       hLambdabarXbin5->Write();

       hLambdabarEtabin1->SetStats(0);
       hLambdabarEtabin1->SetLineColor(kBlack);
       hLambdabarEtabin1->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarEtabin1->GetYaxis()->SetTitle("Count");
       hLambdabarEtabin1->Draw();
       hLambdabarEtabin1->Write();

       hLambdabarEtabin2->SetStats(0);
       hLambdabarEtabin2->SetLineColor(kBlack);
       hLambdabarEtabin2->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarEtabin2->GetYaxis()->SetTitle("Count");
       hLambdabarEtabin2->Draw();
       hLambdabarEtabin2->Write();

       hLambdabarEtabin3->SetStats(0);
       hLambdabarEtabin3->SetLineColor(kBlack);
       hLambdabarEtabin3->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarEtabin3->GetYaxis()->SetTitle("Count");
       hLambdabarEtabin3->Draw();
       hLambdabarEtabin3->Write();

       hLambdabarEtabin4->SetStats(0);
       hLambdabarEtabin4->SetLineColor(kBlack);
       hLambdabarEtabin4->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarEtabin4->GetYaxis()->SetTitle("Count");
       hLambdabarEtabin4->Draw();
       hLambdabarEtabin4->Write();

       hLambdabarEtabin5->SetStats(0);
       hLambdabarEtabin5->SetLineColor(kBlack);
       hLambdabarEtabin5->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarEtabin5->GetYaxis()->SetTitle("Count");
       hLambdabarEtabin5->Draw();
       hLambdabarEtabin5->Write();

       hLambdabarEtabin6->SetStats(0);
       hLambdabarEtabin6->SetLineColor(kBlack);
       hLambdabarEtabin6->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarEtabin6->GetYaxis()->SetTitle("Count");
       hLambdabarEtabin6->Draw();
       hLambdabarEtabin6->Write();

       hLambdabarEtabin7->SetStats(0);
       hLambdabarEtabin7->SetLineColor(kBlack);
       hLambdabarEtabin7->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarEtabin7->GetYaxis()->SetTitle("Count");
       hLambdabarEtabin7->Draw();
       hLambdabarEtabin7->Write();

       hLambdabarEtabin8->SetStats(0);
       hLambdabarEtabin8->SetLineColor(kBlack);
       hLambdabarEtabin8->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarEtabin8->GetYaxis()->SetTitle("Count");
       hLambdabarEtabin8->Draw();
       hLambdabarEtabin8->Write();

       hLambdabarEtabin9->SetStats(0);
       hLambdabarEtabin9->SetLineColor(kBlack);
       hLambdabarEtabin9->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarEtabin9->GetYaxis()->SetTitle("Count");
       hLambdabarEtabin9->Draw();
       hLambdabarEtabin9->Write();

       hLambdabarEtabin10->SetStats(0);
       hLambdabarEtabin10->SetLineColor(kBlack);
       hLambdabarEtabin10->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarEtabin10->GetYaxis()->SetTitle("Count");
       hLambdabarEtabin10->Draw();
       hLambdabarEtabin10->Write();

       hLambdabarPhibin1->SetStats(0);
       hLambdabarPhibin1->SetLineColor(kBlack);
       hLambdabarPhibin1->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarPhibin1->GetYaxis()->SetTitle("Count");
       hLambdabarPhibin1->Draw();
       hLambdabarPhibin1->Write();

       hLambdabarPhibin2->SetStats(0);
       hLambdabarPhibin2->SetLineColor(kBlack);
       hLambdabarPhibin2->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarPhibin2->GetYaxis()->SetTitle("Count");
       hLambdabarPhibin2->Draw();
       hLambdabarPhibin2->Write();

       hLambdabarPhibin3->SetStats(0);
       hLambdabarPhibin3->SetLineColor(kBlack);
       hLambdabarPhibin3->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarPhibin3->GetYaxis()->SetTitle("Count");
       hLambdabarPhibin3->Draw();
       hLambdabarPhibin3->Write();

       hLambdabarPhibin4->SetStats(0);
       hLambdabarPhibin4->SetLineColor(kBlack);
       hLambdabarPhibin4->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarPhibin4->GetYaxis()->SetTitle("Count");
       hLambdabarPhibin4->Draw();
       hLambdabarPhibin4->Write();

       hLambdabarPhibin5->SetStats(0);
       hLambdabarPhibin5->SetLineColor(kBlack);
       hLambdabarPhibin5->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarPhibin5->GetYaxis()->SetTitle("Count");
       hLambdabarPhibin5->Draw();
       hLambdabarPhibin5->Write();

       hLambdabarPhibin6->SetStats(0);
       hLambdabarPhibin6->SetLineColor(kBlack);
       hLambdabarPhibin6->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarPhibin6->GetYaxis()->SetTitle("Count");
       hLambdabarPhibin6->Draw();
       hLambdabarPhibin6->Write();

       hLambdabarPhibin7->SetStats(0);
       hLambdabarPhibin7->SetLineColor(kBlack);
       hLambdabarPhibin7->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarPhibin7->GetYaxis()->SetTitle("Count");
       hLambdabarPhibin7->Draw();
       hLambdabarPhibin7->Write();

       hLambdabarPhibin8->SetStats(0);
       hLambdabarPhibin8->SetLineColor(kBlack);
       hLambdabarPhibin8->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarPhibin8->GetYaxis()->SetTitle("Count");
       hLambdabarPhibin8->Draw();
       hLambdabarPhibin8->Write();

       hLambdabarPhibin9->SetStats(0);
       hLambdabarPhibin9->SetLineColor(kBlack);
       hLambdabarPhibin9->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarPhibin9->GetYaxis()->SetTitle("Count");
       hLambdabarPhibin9->Draw();
       hLambdabarPhibin9->Write();

       hLambdabarPhibin10->SetStats(0);
       hLambdabarPhibin10->SetLineColor(kBlack);
       hLambdabarPhibin10->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarPhibin10->GetYaxis()->SetTitle("Count");
       hLambdabarPhibin10->Draw();
       hLambdabarPhibin10->Write();

       hLambdabarPhibin11->SetStats(0);
       hLambdabarPhibin11->SetLineColor(kBlack);
       hLambdabarPhibin11->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarPhibin11->GetYaxis()->SetTitle("Count");
       hLambdabarPhibin11->Draw();
       hLambdabarPhibin11->Write();

       hLambdabarPhibin12->SetStats(0);
       hLambdabarPhibin12->SetLineColor(kBlack);
       hLambdabarPhibin12->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarPhibin12->GetYaxis()->SetTitle("Count");
       hLambdabarPhibin12->Draw();
       hLambdabarPhibin12->Write();

       hLambdabarpTbin1->SetStats(0);
       hLambdabarpTbin1->SetLineColor(kBlack);
       hLambdabarpTbin1->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarpTbin1->GetYaxis()->SetTitle("Count");
       hLambdabarpTbin1->Draw();
       hLambdabarpTbin1->Write();

       hLambdabarpTbin2->SetStats(0);
       hLambdabarpTbin2->SetLineColor(kBlack);
       hLambdabarpTbin2->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarpTbin2->GetYaxis()->SetTitle("Count");
       hLambdabarpTbin2->Draw();
       hLambdabarpTbin2->Write();

       hLambdabarpTbin3->SetStats(0);
       hLambdabarpTbin3->SetLineColor(kBlack);
       hLambdabarpTbin3->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarpTbin3->GetYaxis()->SetTitle("Count");
       hLambdabarpTbin3->Draw();
       hLambdabarpTbin3->Write();

       hLambdabarpTbin4->SetStats(0);
       hLambdabarpTbin4->SetLineColor(kBlack);
       hLambdabarpTbin4->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarpTbin4->GetYaxis()->SetTitle("Count");
       hLambdabarpTbin4->Draw();
       hLambdabarpTbin4->Write();

       hLambdabarpTbin5->SetStats(0);
       hLambdabarpTbin5->SetLineColor(kBlack);
       hLambdabarpTbin5->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarpTbin5->GetYaxis()->SetTitle("Count");
       hLambdabarpTbin5->Draw();
       hLambdabarpTbin5->Write();

       hLambdabarpTbin6->SetStats(0);
       hLambdabarpTbin6->SetLineColor(kBlack);
       hLambdabarpTbin6->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarpTbin6->GetYaxis()->SetTitle("Count");
       hLambdabarpTbin6->Draw();
       hLambdabarpTbin6->Write();

       hLambdabarpTbin7->SetStats(0);
       hLambdabarpTbin7->SetLineColor(kBlack);
       hLambdabarpTbin7->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarpTbin7->GetYaxis()->SetTitle("Count");
       hLambdabarpTbin7->Draw();
       hLambdabarpTbin7->Write();

       hLambdabarpTbin8->SetStats(0);
       hLambdabarpTbin8->SetLineColor(kBlack);
       hLambdabarpTbin8->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarpTbin8->GetYaxis()->SetTitle("Count");
       hLambdabarpTbin8->Draw();
       hLambdabarpTbin8->Write();

       hLambdabarpTbin9->SetStats(0);
       hLambdabarpTbin9->SetLineColor(kBlack);
       hLambdabarpTbin9->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdabarpTbin9->GetYaxis()->SetTitle("Count");
       hLambdabarpTbin9->Draw();
       hLambdabarpTbin9->Write();
       //

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

       hLambdaReg->SetStats(0);
       hLambdaReg->SetLineColor(kBlack);
       hLambdaReg->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaReg->GetYaxis()->SetTitle("Count");
       hLambdaReg->Draw();
       hLambdaReg->Write();

       hLambdaBar->SetStats(0);
       hLambdaBar->SetLineColor(kBlack);
       hLambdaBar->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaBar->GetYaxis()->SetTitle("Count");
       hLambdaBar->Draw();
       hLambdaBar->Write();

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

       hLambdaWholepTrange->SetStats(0);
       hLambdaWholepTrange->SetLineColor(kBlack);
       hLambdaWholepTrange->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaWholepTrange->GetYaxis()->SetTitle("Count");
       hLambdaWholepTrange->Draw();
       hLambdaWholepTrange->Write();

       hLambdaBarWholepTrange->SetStats(0);
       hLambdaBarWholepTrange->SetLineColor(kBlack);
       hLambdaBarWholepTrange->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaBarWholepTrange->GetYaxis()->SetTitle("Count");
       hLambdaBarWholepTrange->Draw();
       hLambdaBarWholepTrange->Write();

       hLambdaRegWholepTrange->SetStats(0);
       hLambdaRegWholepTrange->SetLineColor(kBlack);
       hLambdaRegWholepTrange->GetXaxis()->SetTitle("Mass [Gev]");
       hLambdaRegWholepTrange->GetYaxis()->SetTitle("Count");
       hLambdaRegWholepTrange->Draw();
       hLambdaRegWholepTrange->Write();

       hAPbefore->SetStats(0);
       hAPbefore->GetXaxis()->SetTitle("\\alpha = (p^{+}_{L} - p^{-}_{L})/(p^{+}_{L} + p^{-}_{L})");
       hAPbefore->GetYaxis()->SetTitle("p_{T} [GeV]");
       hAPbefore->GetXaxis()->SetTitleSize(0.04);
       hAPbefore->GetXaxis()->SetTitleOffset(1);
       hAPbefore->GetYaxis()->SetTitleSize(0.05);
       hAPbefore->Draw("colz");
       canvas->SetName("AP Before");
       canvas->Write();

       hAPafter->SetStats(0);
       hAPafter->GetXaxis()->SetTitle("\\alpha = (p^{+}_{L} - p^{-}_{L})/(p^{+}_{L} + p^{-}_{L})");
       hAPafter->GetYaxis()->SetTitle("p_{T} [GeV]");
       hAPafter->GetXaxis()->SetTitleSize(0.04);
       hAPafter->GetXaxis()->SetTitleOffset(1);
       hAPafter->GetYaxis()->SetTitleSize(0.05);
       hAPafter->Draw("colz");
       canvas->SetName("AP After");
       canvas->Write();

       /*hMassVsBjorkenXlambdabar->SetStats(0);
       hMassVsBjorkenXlambdabar->GetXaxis()->SetTitle("Mass [GeV]");
       hMassVsBjorkenXlambdabar->GetYaxis()->SetTitle("Bjorken X");
       hMassVsBjorkenXlambdabar->GetXaxis()->SetTitleSize(0.04);
       hMassVsBjorkenXlambdabar->GetXaxis()->SetTitleOffset(1);
       hMassVsBjorkenXlambdabar->GetYaxis()->SetTitleSize(0.05);
       hMassVsBjorkenXlambdabar->Draw("colz");
       canvas->SetName("#bar{#Lambda} Bjorken x vs Mass");
       canvas->Write();

       hMassVspTlambdabar->SetStats(0);
       hMassVspTlambdabar->GetXaxis()->SetTitle("Mass [GeV]");
       hMassVspTlambdabar->GetYaxis()->SetTitle("p_{T} [GeV]");
       hMassVspTlambdabar->GetXaxis()->SetTitleSize(0.04);
       hMassVspTlambdabar->GetXaxis()->SetTitleOffset(1);
       hMassVspTlambdabar->GetYaxis()->SetTitleSize(0.05);
       hMassVspTlambdabar->Draw("colz");
       canvas->SetName("#bar{#Lambda} pT vs Mass");
       canvas->Write();

       hMassVsEtalambdabar->SetStats(0);
       hMassVsEtalambdabar->GetXaxis()->SetTitle("Mass [GeV]");
       hMassVsEtalambdabar->GetYaxis()->SetTitle("Eta");
       hMassVsEtalambdabar->GetXaxis()->SetTitleSize(0.04);
       hMassVsEtalambdabar->GetXaxis()->SetTitleOffset(1);
       hMassVsEtalambdabar->GetYaxis()->SetTitleSize(0.05);
       hMassVsEtalambdabar->Draw("colz");
       canvas->SetName("#bar{#Lambda} Eta vs Mass");
       canvas->Write();

       hMassVsQ2lambdabar->SetStats(0);
       hMassVsQ2lambdabar->GetXaxis()->SetTitle("Mass [GeV]");
       hMassVsQ2lambdabar->GetYaxis()->SetTitle("Q^2 [GeV^2]");
       hMassVsQ2lambdabar->GetXaxis()->SetTitleSize(0.04);
       hMassVsQ2lambdabar->GetXaxis()->SetTitleOffset(1);
       hMassVsQ2lambdabar->GetYaxis()->SetTitleSize(0.05);
       hMassVsQ2lambdabar->Draw("colz");
       canvas->SetName("#bar{#Lambda} Q^2 vs Mass");
       canvas->Write();

       hMassVsBjorkenXlambdareg->SetStats(0);
       hMassVsBjorkenXlambdareg->GetXaxis()->SetTitle("Mass [GeV]");
       hMassVsBjorkenXlambdareg->GetYaxis()->SetTitle("Bjorken X");
       hMassVsBjorkenXlambdareg->GetXaxis()->SetTitleSize(0.04);
       hMassVsBjorkenXlambdareg->GetXaxis()->SetTitleOffset(1);
       hMassVsBjorkenXlambdareg->GetYaxis()->SetTitleSize(0.05);
       hMassVsBjorkenXlambdareg->Draw("colz");
       canvas->SetName("#Lambda Bjorken x vs Mass");
       canvas->Write();

       hMassVspTlambdareg->SetStats(0);
       hMassVspTlambdareg->GetXaxis()->SetTitle("Mass [GeV]");
       hMassVspTlambdareg->GetYaxis()->SetTitle("p_{T} [GeV]");
       hMassVspTlambdareg->GetXaxis()->SetTitleSize(0.04);
       hMassVspTlambdareg->GetXaxis()->SetTitleOffset(1);
       hMassVspTlambdareg->GetYaxis()->SetTitleSize(0.05);
       hMassVspTlambdareg->Draw("colz");
       canvas->SetName("#Lambda pT vs Mass");
       canvas->Write();

       hMassVsEtalambdareg->SetStats(0);
       hMassVsEtalambdareg->GetXaxis()->SetTitle("Mass [GeV]");
       hMassVsEtalambdareg->GetYaxis()->SetTitle("Eta");
       hMassVsEtalambdareg->GetXaxis()->SetTitleSize(0.04);
       hMassVsEtalambdareg->GetXaxis()->SetTitleOffset(1);
       hMassVsEtalambdareg->GetYaxis()->SetTitleSize(0.05);
       hMassVsEtalambdareg->Draw("colz");
       canvas->SetName("#Lambda Eta vs Mass");
       canvas->Write();

       hMassVsQ2lambdareg->SetStats(0);
       hMassVsQ2lambdareg->GetXaxis()->SetTitle("Mass [GeV]");
       hMassVsQ2lambdareg->GetYaxis()->SetTitle("Q^2 [GeV^2]");
       hMassVsQ2lambdareg->GetXaxis()->SetTitleSize(0.04);
       hMassVsQ2lambdareg->GetXaxis()->SetTitleOffset(1);
       hMassVsQ2lambdareg->GetYaxis()->SetTitleSize(0.05);
       hMassVsQ2lambdareg->Draw("colz");
       canvas->SetName("#Lambda Q^2 vs Mass");
       canvas->Write();*/

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

       hLambdaBarPIDproton->SetStats(0);
       hLambdaBarPIDproton->SetLineColor(kRed);
       hLambdaBarPIDproton->Draw();
       hLambdaBarPIDproton->Write();

       hLambdaRegPIDproton->SetStats(0);
       hLambdaRegPIDproton->SetLineColor(kRed);
       hLambdaRegPIDproton->Draw();
       hLambdaRegPIDproton->Write();


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

       hLambdaBarPIDpion->SetStats(0);
       hLambdaBarPIDpion->SetLineColor(kOrange);
       hLambdaBarPIDpion->Draw();
       hLambdaBarPIDpion->Write();

       hLambdaRegPIDpion->SetStats(0);
       hLambdaRegPIDpion->SetLineColor(kOrange);
       hLambdaRegPIDpion->Draw();
       hLambdaRegPIDpion->Write();

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

       hLambdaBarDCAoverdDCAproton->SetStats(0);
       hLambdaBarDCAoverdDCAproton->SetLineColor(kCyan);
       hLambdaBarDCAoverdDCAproton->Draw();
       hLambdaBarDCAoverdDCAproton->Write();

       hLambdaRegDCAoverdDCAproton->SetStats(0);
       hLambdaRegDCAoverdDCAproton->SetLineColor(kCyan);
       hLambdaRegDCAoverdDCAproton->Draw();
       hLambdaRegDCAoverdDCAproton->Write();

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

       hLambdaBarDCAoverdDCApion->SetStats(0);
       hLambdaBarDCAoverdDCApion->SetLineColor(kCyan + 3);
       hLambdaBarDCAoverdDCApion->Draw();
       hLambdaBarDCAoverdDCApion->Write();

       hLambdaRegDCAoverdDCApion->SetStats(0);
       hLambdaRegDCAoverdDCApion->SetLineColor(kCyan +3);
       hLambdaRegDCAoverdDCApion->Draw();
       hLambdaRegDCAoverdDCApion->Write();

       //hLambdapt->SetStats(0);
       //hLambdapt->SetLineColor(kBlue);
       //h1Lambdapt->SetLineColor(kBlue);
       //h2Lambdapt->SetLineColor(kBlue);
       //h3Lambdapt->SetLineColor(kBlue);
       //h4Lambdapt->SetLineColor(kBlue);
       //h5Lambdapt->SetLineColor(kBlue);
       //h6Lambdapt->SetLineColor(kBlue);
       //h7Lambdapt->SetLineColor(kBlue);
       //h8Lambdapt->SetLineColor(kBlue);
       //h9Lambdapt->SetLineColor(kBlue);
       //hLambdapt->Draw();
       //hLambdapt->Write();


     /*  hLambdaBarpt->SetStats(0);
       hLambdaBarpt->SetLineColor(kBlue);
       hLambdaBarpt->Draw();
       hLambdaBarpt->Write();

       hLambdaRegpt->SetStats(0);
       hLambdaRegpt->SetLineColor(kBlue);
       hLambdaRegpt->Draw();
       hLambdaRegpt->Write();*/


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

       hLambdaBarEta->SetStats(0);
       hLambdaBarEta->SetLineColor(kGreen + 3);
       hLambdaBarEta->Draw();
       hLambdaBarEta->Write();

       hLambdaRegEta->SetStats(0);
       hLambdaRegEta->SetLineColor(kGreen + 3);
       hLambdaRegEta->Draw();
       hLambdaRegEta->Write();

       hLambdaKaonCont->SetStats(0);
       hLambdaKaonCont->Draw();
       hLambdaKaonCont->Write();

       hLambdaBarKaonCont->SetStats(0);
       hLambdaBarKaonCont->Draw();
       hLambdaBarKaonCont->Write();

       hLambdaRegKaonCont->SetStats(0);
       hLambdaRegKaonCont->Draw();
       hLambdaRegKaonCont->Write();

       hLambdaPhotonCont->SetStats(0);
       hLambdaPhotonCont->SetLineColor(kGreen);
       h1LambdaPhotonCont->SetLineColor(kGreen);
       h2LambdaPhotonCont->SetLineColor(kGreen);
       h3LambdaPhotonCont->SetLineColor(kGreen);
       h4LambdaPhotonCont->SetLineColor(kGreen);
       h5LambdaPhotonCont->SetLineColor(kGreen);
       h6LambdaPhotonCont->SetLineColor(kGreen);
       h7LambdaPhotonCont->SetLineColor(kGreen);
       h8LambdaPhotonCont->SetLineColor(kGreen);
       h9LambdaPhotonCont->SetLineColor(kGreen);
       hLambdaPhotonCont->Draw();
       hLambdaPhotonCont->Write();

       hLambdaBarPhotonCont->SetStats(0);
       hLambdaBarPhotonCont->SetLineColor(kGreen);
       hLambdaBarPhotonCont->Draw();
       hLambdaBarPhotonCont->Write();

       hLambdaRegPhotonCont->SetStats(0);
       hLambdaRegPhotonCont->SetLineColor(kGreen);
       hLambdaRegPhotonCont->Draw();
       hLambdaRegPhotonCont->Write();

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

       hDedxLpionC->SetStats(0);
       hDedxLpionC->SetFillColor(kRed);
       hDedxLpionC->GetXaxis()->SetTitle("Likelihood");
       hDedxLpionC->GetYaxis()->SetTitle("Count");
       hDedxLpionC->Draw();
       hDedxLpionC->Write();

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

       hDCAoverdDCApionC->SetStats(0);
       hDCAoverdDCApionC->SetFillColor(kRed);
       hDCAoverdDCApionC->GetXaxis()->SetTitle("DCA/dDCA");
       hDCAoverdDCApionC->GetYaxis()->SetTitle("Count");
       hDCAoverdDCApionC->Draw();
       hDCAoverdDCApionC->Write();

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
       canvas->SetName("Proton DCA vs dDCA Before");
       canvas->Write();

       hprotonDCAvsdDCAafter->SetStats(0);
       hprotonDCAvsdDCAafter->GetXaxis()->SetTitle("dDCA");
       hprotonDCAvsdDCAafter->GetYaxis()->SetTitle("DCA");
       hprotonDCAvsdDCAafter->Draw("colz");
       canvas->SetName("Proton DCA vs dDCA After");
       canvas->Write();

       hpionDCAvsdDCAbefore->SetStats(0);
       hpionDCAvsdDCAbefore->GetXaxis()->SetTitle("dDCA");
       hpionDCAvsdDCAbefore->GetYaxis()->SetTitle("DCA");
       hpionDCAvsdDCAbefore->Draw("colz");
       canvas->SetName("Pion DCA vs dDCA Before");
       canvas->Write();

       hpionDCAvsdDCAafter->SetStats(0);
       hpionDCAvsdDCAafter->GetXaxis()->SetTitle("dDCA");
       hpionDCAvsdDCAafter->GetYaxis()->SetTitle("DCA");
       hpionDCAvsdDCAafter->Draw("colz");
       canvas->SetName("Pion DCA vs dDCA After");
       canvas->Write();

       hpionDCAvspTbefore->SetStats(0);
       hpionDCAvspTbefore->GetXaxis()->SetTitle("pT");
       hpionDCAvspTbefore->GetYaxis()->SetTitle("DCA");
       hpionDCAvspTbefore->Draw("colz");
       canvas->SetName("Pion DCA vs pT Before");
       canvas->Write();

       hpiondDCAvspTbefore->SetStats(0);
       hpiondDCAvspTbefore->GetXaxis()->SetTitle("pT");
       hpiondDCAvspTbefore->GetYaxis()->SetTitle("dDCA");
       hpiondDCAvspTbefore->Draw("colz");
       canvas->SetName("Pion dDCA vs pT Before");
       canvas->Write();


       hprotonDCAvspTbefore->SetStats(0);
       hprotonDCAvspTbefore->GetXaxis()->SetTitle("pT");
       hprotonDCAvspTbefore->GetYaxis()->SetTitle("DCA");
       hprotonDCAvspTbefore->Draw("colz");
       canvas->SetName("Proton DCA vs pT Before");
       canvas->Write();


       hprotondDCAvspTbefore->SetStats(0);
       hprotondDCAvspTbefore->GetXaxis()->SetTitle("pT");
       hprotondDCAvspTbefore->GetYaxis()->SetTitle("dDCA");
       hprotondDCAvspTbefore->Draw("colz");
       canvas->SetName("Proton dDCA vs pT Before");
       canvas->Write();

       hprotonDCAoverdDCAvspT->SetStats(0);
       hprotonDCAoverdDCAvspT->GetXaxis()->SetTitle("pT");
       hprotonDCAoverdDCAvspT->GetYaxis()->SetTitle("DCA/dDCA");
       hprotonDCAoverdDCAvspT->Draw("colz");
       canvas->SetName("Proton DCA/dDCA vs pT");
       canvas->Write();

       hpionDCAoverdDCAvspT->SetStats(0);
       hpionDCAoverdDCAvspT->GetXaxis()->SetTitle("pT");
       hpionDCAoverdDCAvspT->GetYaxis()->SetTitle("DCA/dDCA");
       hpionDCAoverdDCAvspT->Draw("colz");
       canvas->SetName("Pion DCA/dDCA vs pT");
       canvas->Write();

       hdEdxBothBefore->SetStats(0);
       hdEdxBothBefore->GetXaxis()->SetTitle("p [GeV]");
       hdEdxBothBefore->GetYaxis()->SetTitle("dE/dx");
       hdEdxBothBefore->Draw("colz");
       canvas->SetName("dE/dx Both Before");
       canvas->Write();
       

       hdEdxProtonBefore->SetStats(0);
       hdEdxProtonBefore->GetXaxis()->SetTitle("p [GeV]");
       hdEdxProtonBefore->GetYaxis()->SetTitle("dE/dx");
       hdEdxProtonBefore->Draw("colz");
       canvas->SetName("dE/dx Proton Before");
       canvas->Write();

       hdEdxPionBefore->SetStats(0);
       hdEdxPionBefore->GetXaxis()->SetTitle("p [GeV]");
       hdEdxPionBefore->GetYaxis()->SetTitle("dE/dx");
       hdEdxPionBefore->Draw("colz");
       canvas->SetName("dE/dx Pion Before");
       canvas->Write();
       
       hdEdxBothAfterLcut->SetStats(0);
       hdEdxBothAfterLcut->GetXaxis()->SetTitle("p [GeV]");
       hdEdxBothAfterLcut->GetYaxis()->SetTitle("dE/dx");
       hdEdxBothAfterLcut->Draw("colz");
       canvas->SetName("dE/dx Both After Likelihood Cut");
       canvas->Write();

       hdEdxProtonAfterLcut->SetStats(0);
       hdEdxProtonAfterLcut->GetXaxis()->SetTitle("p [GeV]");
       hdEdxProtonAfterLcut->GetYaxis()->SetTitle("dE/dx");
       hdEdxProtonAfterLcut->Draw("colz");
       canvas->SetName("dE/dx Proton After Likelihood Cut");
       canvas->Write();

       hdEdxPionAfterLcut->SetStats(0);
       hdEdxPionAfterLcut->GetXaxis()->SetTitle("p [GeV]");
       hdEdxPionAfterLcut->GetYaxis()->SetTitle("dE/dx");
       hdEdxPionAfterLcut->Draw("colz");
       canvas->SetName("dE/dx Pion After Likelihood Cut");
       canvas->Write();

       hdEdxBothAfterAllcuts->SetStats(0);
       hdEdxBothAfterAllcuts->GetXaxis()->SetTitle("p [GeV]");
       hdEdxBothAfterAllcuts->GetYaxis()->SetTitle("dE/dx");
       hdEdxBothAfterAllcuts->Draw("colz");
       canvas->SetName("dE/dx Both After All Cuts");
       canvas->Write();

       hdEdxProtonAfterAllcuts->SetStats(0);
       hdEdxProtonAfterAllcuts->GetXaxis()->SetTitle("p [GeV]");
       hdEdxProtonAfterAllcuts->GetYaxis()->SetTitle("dE/dx");
       hdEdxProtonAfterAllcuts->Draw("colz");
       canvas->SetName("dE/dx Proton After All Cuts");
       canvas->Write();

       hdEdxPionAfterAllcuts->SetStats(0);
       hdEdxPionAfterAllcuts->GetXaxis()->SetTitle("p [GeV]");
       hdEdxPionAfterAllcuts->GetYaxis()->SetTitle("dE/dx");
       hdEdxPionAfterAllcuts->Draw("colz");
       canvas->SetName("dE/dx Pion After All Cuts");
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

       hs1->Add(h1Lambda);
       hs1->Add(h1LambdaPIDproton);
       hs1->Add(h1LambdaPIDpion);
       hs1->Add(h1LambdaDCAoverdDCAproton);
       hs1->Add(h1LambdaDCAoverdDCApion);
       hs1->Add(h1Lambdapt);
       hs1->Add(h1LambdaEta);
       hs1->Add(h1LambdaKaonCont);
       hs1->Add(h1LambdaPhotonCont);
       hs1->Draw("nostack");
       canvas->SetName("Lambda pT Bin 1 Cut Stack");
       canvas->Write();

       hs2->Add(h2Lambda);
       hs2->Add(h2LambdaPIDproton);
       hs2->Add(h2LambdaPIDpion);
       hs2->Add(h2LambdaDCAoverdDCAproton);
       hs2->Add(h2LambdaDCAoverdDCApion);
       hs2->Add(h2Lambdapt);
       hs2->Add(h2LambdaEta);
       hs2->Add(h2LambdaKaonCont);
       hs2->Add(h2LambdaPhotonCont);
       hs2->Draw("nostack");
       canvas->SetName("Lambda pT Bin 2 Cut Stack");
       canvas->Write();

       hs3->Add(h3Lambda);
       hs3->Add(h3LambdaPIDproton);
       hs3->Add(h3LambdaPIDpion);
       hs3->Add(h3LambdaDCAoverdDCAproton);
       hs3->Add(h3LambdaDCAoverdDCApion);
       hs3->Add(h3Lambdapt);
       hs3->Add(h3LambdaEta);
       hs3->Add(h3LambdaKaonCont);
       hs3->Add(h3LambdaPhotonCont);
       hs3->Draw("nostack");
       canvas->SetName("Lambda pT Bin 3 Cut Stack");
       canvas->Write();


       hs4->Add(h4Lambda);
       hs4->Add(h4LambdaPIDproton);
       hs4->Add(h4LambdaPIDpion);
       hs4->Add(h4LambdaDCAoverdDCAproton);
       hs4->Add(h4LambdaDCAoverdDCApion);
       hs4->Add(h4Lambdapt);
       hs4->Add(h4LambdaEta);
       hs4->Add(h4LambdaKaonCont);
       hs4->Add(h4LambdaPhotonCont);
       hs4->Draw("nostack");
       canvas->SetName("Lambda pT Bin 4 Cut Stack");
       canvas->Write();

       hs5->Add(h5Lambda);
       hs5->Add(h5LambdaPIDproton);
       hs5->Add(h5LambdaPIDpion);
       hs5->Add(h5LambdaDCAoverdDCAproton);
       hs5->Add(h5LambdaDCAoverdDCApion);
       hs5->Add(h5Lambdapt);
       hs5->Add(h5LambdaEta);
       hs5->Add(h5LambdaKaonCont);
       hs5->Add(h5LambdaPhotonCont);
       hs5->Draw("nostack");
       canvas->SetName("Lambda pT Bin 5 Cut Stack");
       canvas->Write();

       hs6->Add(h6Lambda);
       hs6->Add(h6LambdaPIDproton);
       hs6->Add(h6LambdaPIDpion);
       hs6->Add(h6LambdaDCAoverdDCAproton);
       hs6->Add(h6LambdaDCAoverdDCApion);
       hs6->Add(h6Lambdapt);
       hs6->Add(h6LambdaEta);
       hs6->Add(h6LambdaKaonCont);
       hs6->Add(h6LambdaPhotonCont);
       hs6->Draw("nostack");
       canvas->SetName("Lambda pT Bin 6 Cut Stack");
       canvas->Write();

       hs7->Add(h7Lambda);
       hs7->Add(h7LambdaPIDproton);
       hs7->Add(h7LambdaPIDpion);
       hs7->Add(h7LambdaDCAoverdDCAproton);
       hs7->Add(h7LambdaDCAoverdDCApion);
       hs7->Add(h7Lambdapt);
       hs7->Add(h7LambdaEta);
       hs7->Add(h7LambdaKaonCont);
       hs7->Add(h7LambdaPhotonCont);
       hs7->Draw("nostack");
       canvas->SetName("Lambda pT Bin 7 Cut Stack");
       canvas->Write();

       hs8->Add(h8Lambda);
       hs8->Add(h8LambdaPIDproton);
       hs8->Add(h8LambdaPIDpion);
       hs8->Add(h8LambdaDCAoverdDCAproton);
       hs8->Add(h8LambdaDCAoverdDCApion);
       hs8->Add(h8Lambdapt);
       hs8->Add(h8LambdaEta);
       hs8->Add(h8LambdaKaonCont);
       hs8->Add(h8LambdaPhotonCont);
       hs8->Draw("nostack");
       canvas->SetName("Lambda pT Bin 8 Cut Stack");
       canvas->Write();

       hs9->Add(h9Lambda);
       hs9->Add(h9LambdaPIDproton);
       hs9->Add(h9LambdaPIDpion);
       hs9->Add(h9LambdaDCAoverdDCAproton);
       hs9->Add(h9LambdaDCAoverdDCApion);
       hs9->Add(h9Lambdapt);
       hs9->Add(h9LambdaEta);
       hs9->Add(h9LambdaKaonCont);
       hs9->Add(h9LambdaPhotonCont);
       hs9->Draw("nostack");
       canvas->SetName("Lambda pT Bin 9 Cut Stack");
       canvas->Write();

       hs->Add(hLambda);
       hs->Add(hLambdaPIDproton);
       hs->Add(hLambdaPIDpion);
       hs->Add(hLambdaDCAoverdDCAproton);
       hs->Add(hLambdaDCAoverdDCApion);
       hs->Add(hLambdapt);
       hs->Add(hLambdaEta);
       hs->Add(hLambdaKaonCont);
       hs->Add(hLambdaPhotonCont);
       hs->Add(hLambdaWholepTrange);
       hs->Draw("nostack");
       canvas->SetName("Lambda Cut Stack");
       canvas->Write();

       hsBar->Add(hLambdaBar);
       hsBar->Add(hLambdaBarPIDproton);
       hsBar->Add(hLambdaBarPIDpion);
       hsBar->Add(hLambdaBarDCAoverdDCAproton);
       hsBar->Add(hLambdaBarDCAoverdDCApion);
       hsBar->Add(hLambdaBarpt);
       hsBar->Add(hLambdaBarEta);
       hsBar->Add(hLambdaBarKaonCont);
       hsBar->Add(hLambdaBarPhotonCont);
       hsBar->Add(hLambdaBarWholepTrange);
       hsBar->Draw("nostack");
       canvas->SetName("Lambda Bar Cut Stack");
       canvas->Write();

       hsReg->Add(hLambdaReg);
       hsReg->Add(hLambdaRegPIDproton);
       hsReg->Add(hLambdaRegPIDpion);
       hsReg->Add(hLambdaRegDCAoverdDCAproton);
       hsReg->Add(hLambdaRegDCAoverdDCApion);
       hsReg->Add(hLambdaRegpt);
       hsReg->Add(hLambdaRegEta);
       hsReg->Add(hLambdaRegKaonCont);
       hsReg->Add(hLambdaRegPhotonCont);
       hsReg->Add(hLambdaRegWholepTrange);
       hsReg->Draw("nostack");
       canvas->SetName("Lambda Reg Cut Stack");
       canvas->Write();

       hLambdabarVsBjorkenX->SetLineColor(kBlue);
       hLambdaregVsBjorkenX->SetLineColor(kRed);
       hsLambdaX->Add(hLambdaregVsBjorkenX);
       hsLambdaX->Add(hLambdabarVsBjorkenX);
       hsLambdaX->Draw("nostack");
       TLegend* legend = new TLegend();
       legend->AddEntry(hLambdabarVsBjorkenX, "#bar{#Lambda}", "l");
       legend->AddEntry(hLambdaregVsBjorkenX, "#Lambda", "l");
       legend->Draw("same");
       canvas->SetName("X Distributions");
       canvas->Write();

       hLambdabarVspT->SetLineColor(kBlue);
       hLambdaregVspT->SetLineColor(kRed);
       hsLambdapT->Add(hLambdaregVspT);
       hsLambdapT->Add(hLambdabarVspT);
       hsLambdapT->Draw("nostack");
       TLegend* legend2 = new TLegend();
       legend2->AddEntry(hLambdabarVspT, "#bar{#Lambda}", "l");
       legend2->AddEntry(hLambdaregVspT, "#Lambda", "l");
       legend2->Draw("same");
       canvas->SetName("pT Distributions");
       canvas->Write();

       hLambdabarVsphi->SetLineColor(kBlue);
       hLambdaregVsphi->SetLineColor(kRed);
       hsLambdaphi->Add(hLambdaregVsphi);
       hsLambdaphi->Add(hLambdabarVsphi);
       hsLambdaphi->Draw("nostack");
       TLegend* legendphi = new TLegend();
       legendphi->AddEntry(hLambdabarVsphi, "#bar{#Lambda}", "l");
       legendphi->AddEntry(hLambdaregVsphi, "#Lambda", "l");
       legendphi->Draw("same");
       canvas->SetName("Phi Distributions");
       canvas->Write();

       hLambdabarVsEta->SetLineColor(kBlue);
       hLambdaregVsEta->SetLineColor(kRed);
       hsLambdaEta->Add(hLambdaregVsEta);
       hsLambdaEta->Add(hLambdabarVsEta);
       hsLambdaEta->Draw("nostack");
       TLegend* legend3 = new TLegend();
       legend3->AddEntry(hLambdabarVsEta, "#bar{#Lambda}", "l");
       legend3->AddEntry(hLambdaregVsEta, "#Lambda", "l");
       legend3->Draw("same");
       canvas->SetName("Eta Distributions");
       canvas->Write();

       hLambdabarVsQ2->SetLineColor(kBlue);
       hLambdaregVsQ2->SetLineColor(kRed);
       hsLambdaQ2->Add(hLambdaregVsQ2);
       hsLambdaQ2->Add(hLambdabarVsQ2);
       hsLambdaQ2->Draw("nostack");
       TLegend* legend4 = new TLegend();
       legend4->AddEntry(hLambdabarVsQ2, "#bar{#Lambda}", "l");
       legend4->AddEntry(hLambdaregVsQ2, "#Lambda", "l");
       legend4->Draw("same");
       canvas->SetName("Q2 Distributions");
       canvas->Write();

       hLambdabarVsy->SetLineColor(kBlue);
       hLambdaregVsy->SetLineColor(kRed);
       hsLambday->Add(hLambdaregVsy);
       hsLambday->Add(hLambdabarVsy);
       hsLambday->Draw("nostack");
       TLegend* legend5 = new TLegend();
       legend5->AddEntry(hLambdabarVsy, "#bar{#Lambda}", "l");
       legend5->AddEntry(hLambdaregVsy, "#Lambda", "l");
       legend5->Draw("same");
       canvas->SetName("y Distributions");
       canvas->Write();

       

       file.Write();
       file.Close();
       cout << "Histograms written to " << output << endl;


        return 0;
    }