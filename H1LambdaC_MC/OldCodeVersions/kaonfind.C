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
#include "H1Tracks/H1CentralFittedTrack.h"
#include "H1Mods/H1PartSelTrack.h"
#include "H1Mods/H1PartSelTrackArrayPtr.h"
#include "H1Mods/H1PartJPsi.h"
#include "H1Mods/H1PartJPsiArrayPtr.h"
#include "H1Mods/H1PartCand.h"
#include "H1Mods/H1PartCandArrayPtr.h"
#include "H1Mods/H1PartMCArrayPtr.h"
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

    //Total Lambda plot
    TH1F* hLambda = new TH1F("Lambda", "Lambda", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    //Lambdas in pt bins
    TH1F* hLambda1 = new TH1F("Lambda1", "Lambda1", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda2 = new TH1F("Lambda2", "Lambda2", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda3 = new TH1F("Lambda3", "Lambda3", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda4 = new TH1F("Lambda4", "Lambda4", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda5 = new TH1F("Lambda5", "Lambda5", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda6 = new TH1F("Lambda6", "Lambda6", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda7 = new TH1F("Lambda7", "Lambda7", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda8 = new TH1F("Lambda8", "Lambda8", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* hLambda9 = new TH1F("Lambda9", "Lambda9", lambdabins, lambdalowerbinrange, lambdaupperbinrange);

    //Armenteros plot for particle ID lambdas vs kaons
    TH2F* hAP = new TH2F("AP", "Armenteros-Podolansky Plot", 200, -1, 1, 200, 0, 0.3);
    //dEdx plot for particle ID protons vs pions
    TH2D* hdEdxvP = new TH2D("dEdxvP", "dE/dx vs. p", 500, 0, 2, 500, 0, 10);
    TH2D* hdEdxvP2 = new TH2D("dEdxvP2", "dE/dx vs. p", 500, 0, 2, 500, 0, 10);
    TH1F* hDedxL = new TH1F("Likelihood", "Likelihood", 500, 0, 1);

   
    //Defining Root Canvas to work on
    TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 400);

    // define radial vertex cuts for distance of closest approach and beampipe
    const Float_t minDca = 2.;
    const Float_t maxDca = 40.;
    const Float_t lowerBeamPipe = 4.;
    const Float_t upperBeamPipe = 5.5;

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
    //static H1CentralFittedV0ArrayPtr v0;

    // Get the pion's mass
    Double_t mPion = TDatabasePDG::Instance()->GetParticle(211)->Mass();
    // Get proton mass
    Double_t mProton = TDatabasePDG::Instance()->GetParticle(2212)->Mass();

    ///////////////BEGIN MAIN LOOP////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    // Loop as long as there's data
    while (H1Tree::Instance()->Next() && !opts.IsMaxEvent(eventCounter)) {

        // Loop over all vertices found in the event
        for (Int_t loop = 0; loop < vertex.GetEntries(); loop++) {

            // In the following statements, some checks regarding
            // vertex type,
            // vertex position (exclude beam pipe),
            // number of mother and daughter particles and
            // momentum properties
            // are performed. If it's all ok, cut for pTrel
            // to exclue Lambdas and fill in the invariant mass.


            //If the vertex is a decay vertex, and the vertex position is within 
            //our DCA bounds and beampipe bounds, continue
            if (vertex[loop]->GetType() == H1Vertex::vtDecay
                && ((vertex[loop]->GetPosition().Perp() > minDca
             //       && vertex[loop]->GetPosition().Perp() < lowerBeamPipe)
             //       || (vertex[loop]->GetPosition().Perp() > upperBeamPipe
                        && vertex[loop]->GetPosition().Perp() < maxDca))) {

                //Get decay daughters, looking for 2 daughter processes
                const TObjArray* daughters = vertex[loop]->GetDaughters();
                if (daughters->GetEntries() == 2) {
                    const TObjArray* mothers = vertex[loop]->GetMothers();

                    if (mothers->GetEntries() != 1) {
                        // that shouldn't happen
                        break;
                    }

                    //
                    H1Track* firstDaughter = (H1Track*)daughters->At(0);
                    H1Track* secondDaughter = (H1Track*)daughters->At(1);

                    if (dynamic_cast<H1PartSelTrack*>(daughters->At(0))) {
                        cout << "GOOD H1PARTSELTRACK" << endl;
                    }
                    
                    /*H1PartSelTrack* firstDaughterPST = dynamic_cast<H1PartSelTrack*>(daughters->At(0));
                    H1PartSelTrack* secondDaughterPST = static_cast<H1PartSelTrack*>(daughters->At(1));
                    cout << "ID1 " << firstDaughterPST << endl;
                    cout << "ID2 " << secondDaughterPST << endl;*/


                    
                    //if (firstDaughterPST==0 || secondDaughterPST==0) {
                    //    cout << "ID1 " << firstDaughterPST << endl;
                    //    cout << "ID2 " << secondDaughterPST << endl;
                    //    badcount = badcount + 1;

                    //}
                    

                       // Breaks if the daughters are not opposite charges
                     if (firstDaughter->GetCharge() != (-1) * secondDaughter->GetCharge()) {
                            // DST bug: Link from vertex fitted track
                            // to non-vertex fitted track missing
                            break;
                     }

                     if ((firstDaughter->GetPt() < minPtPion
                         || secondDaughter->GetPt() < minPtPion) //minimum pt of daughters, value set to match 2009 H1 paper
                         || ((H1Track*)mothers->At(0))->GetPt() < minPtKZero //max and min pt for mother particle lambdas or kaons
                         || ((H1Track*)mothers->At(0))->GetPt() > maxPtKZero
                         || firstDaughter->GetLength() < minTracklenPion
                         || secondDaughter->GetLength() < minTracklenPion) {
                            break;
                     }
           
                     //Get 3-Momentum for pTrel/pLrel calcualtion
                     TVector3 vecPionOne = firstDaughter->GetFourVector(mPion).Vect();
                     TVector3 vecPionTwo = secondDaughter->GetFourVector(mPion).Vect();
                     TVector3 pKaon = vecPionOne + vecPionTwo;

                     //pTrel and pLrel calculation
                     // NOTE: These are independent of the assumed mPion mass input above since we
                     //divide by the magnitude of pKoan to get the daughters momentum
                     //directions with respect to the parent particle
                     Double_t pTrelOne = pKaon.Cross(vecPionOne).Mag() / pKaon.Mag();
                     Double_t pTrelTwo = pKaon.Cross(vecPionTwo).Mag() / pKaon.Mag();
                     Double_t qlOne = pKaon.Dot(vecPionOne) / pKaon.Mag();
                     Double_t qlTwo = pKaon.Dot(vecPionTwo) / pKaon.Mag();

                     //Get charges for alpha calcualtion, where by definition we 
                     //subtract negative particle in numerator
                     Double_t chargeOne = firstDaughter->GetCharge();
                     Double_t chargeTwo = secondDaughter->GetCharge();
                     Double_t alpha = (chargeOne*qlOne+chargeTwo*qlTwo)/(qlOne+qlTwo);

                     //Fill Armenteros-Podolanski plot
                     hAP->Fill(alpha, pTrelOne);
                     //Organizing data to extract dE/dx information
                     H1CentralFittedTrack* firstDaughterC = (H1CentralFittedTrack*)daughters->At(0);
                     H1CentralFittedTrack* secondDaughterC = (H1CentralFittedTrack*)daughters->At(1);
                     //Fill dE/dx vs p plot
                     hdEdxvP->Fill(secondDaughterC->GetMomentum().Mag(), secondDaughterC->GetCorrectedDedx());
                     hdEdxvP->Fill(firstDaughterC->GetMomentum().Mag(), firstDaughterC->GetCorrectedDedx());

                     //Extracting dEdx proton Likelihood using the H1Dedx class

                     //H1Dedx* firstDaughterL = (H1Dedx*)daughters->At(0);
                     //H1Dedx* secondDaughterL = (H1Dedx*)daughters->At(1);
                     H1Dedx* firstDaughterL = static_cast<H1Dedx*>(daughters->At(0));
                     H1Dedx* secondDaughterL = static_cast<H1Dedx*>(daughters->At(1));

                     H1CentralTrack* firstDaughterCT = static_cast<H1CentralTrack*>(daughters->At(0));
                     H1CentralTrack* secondDaughterCT = static_cast<H1CentralTrack*>(daughters->At(1));

                     H1PartSelTrack* partSel1 = static_cast<H1PartSelTrack*>
                         (daughters->At(0));
                     H1PartSelTrack* partSel2 = static_cast<H1PartSelTrack*>
                         (daughters->At(1));

                     if (!partSel1 || !partSel2) {
                         break;
                     }
                     
                     //Double_t dEdxLOne = firstDaughterL->GetLikelihood(H1Dedx::kProton, firstDaughterC->GetCorrectedDedx(), firstDaughterL->GetDedxResolution(firstDaughterC->GetNHitdEdx(), firstDaughter->GetPhi(), firstDaughter->GetCharge(), H1Dedx::kProton), firstDaughter->GetP(), firstDaughter->GetdP());
                     //Double_t dEdxLTwo = secondDaughterL->GetLikelihood(H1Dedx::kProton, secondDaughterC->GetCorrectedDedx(), secondDaughterL->GetDedxResolution(secondDaughterC->GetNHitdEdx(), secondDaughter->GetPhi(), secondDaughter->GetCharge(), H1Dedx::kProton), secondDaughter->GetP(), secondDaughter->GetdP());
                     Double_t dEdxLOne = firstDaughterL->GetLikelihood(H1Dedx::kProton, firstDaughterCT->GetdEdx(), 0.068, firstDaughter->GetP(), firstDaughter->GetdP());

                    //Now Plotting dEdx Proton Likelihood for all daughters to help visualize the proton cut
                    hDedxL->Fill(dEdxLOne);
                    //hDedxL->Fill(dEdxLTwo);

                    /*cout << "dEdx " << firstDaughterC->GetCorrectedDedx() << endl;
                    cout << "DdEdx " << firstDaughterL->GetDedxResolution(firstDaughterCT->GetNHitdEdx(), firstDaughter->GetPhi(), firstDaughter->GetCharge(), H1Dedx::kProton) << endl;
                    cout << "NHit " << firstDaughterC->GetNHitdEdx() << endl;
                    cout << "Phi " << firstDaughter->GetPhi() << endl;
                    cout << "Charge " << firstDaughter->GetCharge() << endl;

                    
                    
                    
                    cout << "p " << firstDaughter->GetP() << endl;
                    cout << "Dp " << firstDaughter->GetdP() << endl;
                    cout << "Likelihood " << partSel1->GetDedxLikelihood(kProton) << endl;*/

                     //Getting Kaon invariant mass
                     Double_t invMasskaon = (firstDaughter->GetFourVector(mPion)
                         + secondDaughter->GetFourVector(mPion)).M();
                     //Implement ptrel cut and fill Kaon invariant mass if it passes
                     if (((qlOne>qlTwo && (firstDaughter->GetFourVector(mProton)
                         + secondDaughter->GetFourVector(mPion)).M()>1.125)||(qlOne < qlTwo && (firstDaughter->GetFourVector(mPion)
                             + secondDaughter->GetFourVector(mProton)).M() > 1.125)) 
                         && pTrelOne>=ptrelCutK 
                         && pTrelTwo>=ptrelCutK) {

                             hKaon->Fill(invMasskaon);

                         if (((H1Track*)mothers->At(0))->GetPt() >= ptbin1l && ((H1Track*)mothers->At(0))->GetPt() < ptbin1u) {
                             hKaon1->Fill(invMasskaon);
                         }

                         else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin2l && ((H1Track*)mothers->At(0))->GetPt() < ptbin2u) {
                             hKaon2->Fill(invMasskaon);
                         }

                         else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin3l && ((H1Track*)mothers->At(0))->GetPt() < ptbin3u) {
                             hKaon3->Fill(invMasskaon);
                         }

                         else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin4l && ((H1Track*)mothers->At(0))->GetPt() < ptbin4u) {
                             hKaon4->Fill(invMasskaon);
                         }

                         else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin5l && ((H1Track*)mothers->At(0))->GetPt() < ptbin5u) {
                             hKaon5->Fill(invMasskaon);
                         }

                         else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin6l && ((H1Track*)mothers->At(0))->GetPt() < ptbin6u) {
                             hKaon6->Fill(invMasskaon);
                         }

                         else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin7l && ((H1Track*)mothers->At(0))->GetPt() < ptbin7u) {
                             hKaon7->Fill(invMasskaon);
                         }

                         else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin8l && ((H1Track*)mothers->At(0))->GetPt() < ptbin8u) {
                             hKaon8->Fill(invMasskaon);
                         }

                         else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin9l && ((H1Track*)mothers->At(0))->GetPt() < ptbin9u) {
                             hKaon9->Fill(invMasskaon);
                         }
                     }

                     //Now that Kaons are taken care of, can implement dE/dx cut to distinguish protons from kaons in two daughter decay for lambdas
                     //starting with proton cut
                     if ((qlOne > qlTwo && firstDaughterC->GetdEdxLubimov() < 0.003)
                         || (qlOne < qlTwo && secondDaughterC->GetdEdxLubimov() < 0.003)
                         //now the pion cut
                         //|| (qlOne > qlTwo && secondDaughterC->GetdEdxLubimov() > 0.03)
                         //|| (qlOne < qlTwo && firstDaughterC->GetCorrectedDedx() > 0.03)
                         
                         ) {
                         
                         break;
                     }

                     //Fill dE/dx vs p plot
                     hdEdxvP2->Fill(secondDaughterC->GetMomentum().Mag(), secondDaughterC->GetCorrectedDedx());
                     hdEdxvP2->Fill(firstDaughterC->GetMomentum().Mag(), firstDaughterC->GetCorrectedDedx());

                     //Chi2 cut setup
                     //H1TrackEvent* trackEvent = vertex[loop]->H1TrackEvent();
                     //TClonesArray* centralV0 = trackEvent->GetArray(H1CentralFittedV0::Class());
                     //H1CentralFittedV0* v0 = (H1CentralFittedV0*)centralV0->UncheckedAt(loop);

                     

                     //Implement pt cut for Lambdas and fill invariant mass if it passes
                     if (((firstDaughter->GetFourVector(mPion)
                         + secondDaughter->GetFourVector(mPion)).M()<0.46 || (firstDaughter->GetFourVector(mPion)
                             + secondDaughter->GetFourVector(mPion)).M()>0.54) 
                         && pTrelOne<=ptrelCutL 
                         && pTrelTwo<=ptrelCutL
                         && pTrelOne>=0.02
                         && pTrelTwo>=0.02){
                         //&& vertex[loop]->GetChi2Z() > 1
                         //&& vertex[loop]->GetChi2XY() > 1){
                         //&& v0->GetFitChi2() > 5) {
                         if (qlOne > qlTwo && (alpha > 0.4 || alpha < -0.4)) {
                             Double_t invMasslambda = (firstDaughter->GetFourVector(mProton)
                                 + secondDaughter->GetFourVector(mPion)).M();

                             hLambda->Fill(invMasslambda);

                             if (((H1Track*)mothers->At(0))->GetPt() >= ptbin1l && ((H1Track*)mothers->At(0))->GetPt() < ptbin1u) {
                                 hLambda1->Fill(invMasslambda);
                             }

                             else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin2l && ((H1Track*)mothers->At(0))->GetPt() < ptbin2u) {
                                 hLambda2->Fill(invMasslambda);
                             }

                             else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin3l && ((H1Track*)mothers->At(0))->GetPt() < ptbin3u) {
                                 hLambda3->Fill(invMasslambda);
                             }

                             else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin4l && ((H1Track*)mothers->At(0))->GetPt() < ptbin4u) {
                                 hLambda4->Fill(invMasslambda);
                             }

                             else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin5l && ((H1Track*)mothers->At(0))->GetPt() < ptbin5u) {
                                 hLambda5->Fill(invMasslambda);
                             }

                             else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin6l && ((H1Track*)mothers->At(0))->GetPt() < ptbin6u) {
                                 hLambda6->Fill(invMasslambda);
                             }

                             else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin7l && ((H1Track*)mothers->At(0))->GetPt() < ptbin7u) {
                                 hLambda7->Fill(invMasslambda);
                             }

                             else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin8l && ((H1Track*)mothers->At(0))->GetPt() < ptbin8u) {
                                 hLambda8->Fill(invMasslambda);
                             }

                             else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin9l && ((H1Track*)mothers->At(0))->GetPt() < ptbin9u) {
                                 hLambda9->Fill(invMasslambda);
                             }
                         }
                         else if (qlOne < qlTwo && (alpha > 0.4 || alpha < -0.4)) {
                             Double_t invMasslambda = (firstDaughter->GetFourVector(mPion)
                                 + secondDaughter->GetFourVector(mProton)).M();

                             hLambda->Fill(invMasslambda);

                             if (((H1Track*)mothers->At(0))->GetPt() >= ptbin1l && ((H1Track*)mothers->At(0))->GetPt() < ptbin1u) {
                                 hLambda1->Fill(invMasslambda);
                             }

                             else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin2l && ((H1Track*)mothers->At(0))->GetPt() < ptbin2u) {
                                 hLambda2->Fill(invMasslambda);
                             }

                             else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin3l && ((H1Track*)mothers->At(0))->GetPt() < ptbin3u) {
                                 hLambda3->Fill(invMasslambda);
                             }

                             else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin4l && ((H1Track*)mothers->At(0))->GetPt() < ptbin4u) {
                                 hLambda4->Fill(invMasslambda);
                             }

                             else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin5l && ((H1Track*)mothers->At(0))->GetPt() < ptbin5u) {
                                 hLambda5->Fill(invMasslambda);
                             }

                             else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin6l && ((H1Track*)mothers->At(0))->GetPt() < ptbin6u) {
                                 hLambda6->Fill(invMasslambda);
                             }

                             else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin7l && ((H1Track*)mothers->At(0))->GetPt() < ptbin7u) {
                                 hLambda7->Fill(invMasslambda);
                             }

                             else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin8l && ((H1Track*)mothers->At(0))->GetPt() < ptbin8u) {
                                 hLambda8->Fill(invMasslambda);
                             }

                             else if (((H1Track*)mothers->At(0))->GetPt() >= ptbin9l && ((H1Track*)mothers->At(0))->GetPt() < ptbin9u) {
                                 hLambda9->Fill(invMasslambda);
                             }
                         }

                     }
          
                   
                } // if #daughters == 2
            } // vertices' properties ok?
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


       //Defining cut functions to visualize cuts
       TF1* dEdxcutproton = new TF1("dEdxcutproton", "[0]+[1]*x", 0, 1.071);
       dEdxcutproton->SetParameter(0, 4.8);
       dEdxcutproton->SetParameter(1, -4.8);

       TF1* dEdxcutpion = new TF1("dEdxcutpion", "[0]", 0.2, 2);
       dEdxcutpion->SetParameter(0, 2.0);
       //dEdxcutpion->SetParameter(1, -15);
       dEdxcutpion->SetLineColor(kBlack);
       //Kaon plots uploading
       hKaon->SetStats(0);
       hKaon1->SetStats(0);
       hKaon2->SetStats(0);
       hKaon3->SetStats(0);
       hKaon4->SetStats(0);
       hKaon5->SetStats(0);
       hKaon6->SetStats(0);
       hKaon7->SetStats(0);
       hKaon8->SetStats(0);
       hKaon9->SetStats(0);
       hLambda->SetStats(0);
       hLambda1->SetStats(0);
       hLambda2->SetStats(0);
       hLambda3->SetStats(0);
       hLambda4->SetStats(0);
       hLambda5->SetStats(0);
       hLambda6->SetStats(0);
       hLambda7->SetStats(0);
       hLambda8->SetStats(0);
       hLambda9->SetStats(0);

       hKaon->Draw();
       hKaon1->Draw();
       hKaon2->Draw();
       hKaon3->Draw();
       hKaon4->Draw();
       hKaon5->Draw();
       hKaon6->Draw();
       hKaon7->Draw();
       hKaon8->Draw();
       hKaon9->Draw();
       hLambda->Draw();
       hLambda1->Draw();
       hLambda2->Draw();
       hLambda3->Draw();
       hLambda4->Draw();
       hLambda5->Draw();
       hLambda6->Draw();
       hLambda7->Draw();
       hLambda8->Draw();
       hLambda9->Draw();

       hAP->SetStats(0);
       hdEdxvP->SetStats(0);
       hdEdxvP2->SetStats(0);
       hDedxL->SetStats(0);
       TCanvas* c3 = new TCanvas("ODS", "Plot", 10, 10, 800, 400);
       //hdEdxvP->SetContour(1000);
       hdEdxvP->GetXaxis()->SetTitle("p [GeV]");
       hdEdxvP->GetYaxis()->SetTitle("dE/dx");
       hdEdxvP->GetXaxis()->SetTitleSize(0.05);
       hdEdxvP->GetXaxis()->SetTitleOffset(1);
       hdEdxvP->GetYaxis()->SetTitleSize(0.05);
       hdEdxvP->Draw("colz");
       dEdxcutproton->Draw("SAME");
       dEdxcutpion->Draw("SAME");
       c3->SaveAs("dEdx.pdf");
       //hAP->SetContour(1000);
       hAP->GetXaxis()->SetTitle("\\alpha = (p^{+}_{L} - p^{-}_{L})/(p^{+}_{L} + p^{-}_{L})");
       hAP->GetYaxis()->SetTitle("p_{T} [GeV]");
       hAP->GetXaxis()->SetTitleSize(0.04);
       hAP->GetXaxis()->SetTitleOffset(1);
       hAP->GetYaxis()->SetTitleSize(0.05);
       hAP->Draw("colz");
       c3->SaveAs("AP.pdf");

       hDedxL->Draw();
       hdEdxvP2->Draw();

       gPad->Update();
       TFile file(opts.GetOutput(), "RECREATE");

       hKaon->Write();
       hKaon1->Write();
       hKaon2->Write();
       hKaon3->Write();
       hKaon4->Write();
       hKaon5->Write();
       hKaon6->Write();
       hKaon7->Write();
       hKaon8->Write();
       hKaon9->Write();
       hLambda->Write();
       hLambda1->Write();
       hLambda2->Write();
       hLambda3->Write();
       hLambda4->Write();
       hLambda5->Write();
       hLambda6->Write();
       hLambda7->Write();
       hLambda8->Write();
       hLambda9->Write();
       hAP->Write();
       hdEdxvP->Write();
       hdEdxvP2->Write();
       hDedxL->Write();

       
       file.Write();
       file.Close();
       cout << "Histograms written to " << opts.GetOutput() << endl;




















        ///////////////Fitting functions and trying to subtract backgrounds///
   
        ////Fitting smooth background portions of Kaon and Lambda peaks
        ////These ranges obtained from trial and error looking at plots of ~1000000 total events
        //TF1* kaonfit = new TF1("kaonfit", "pol1", 0.59, 0.73);
        //kaonfit->SetLineColor(kGreen);
        //hKaon->Fit("kaonfit", "RQ");//Plot the fit directly if desired, R->ranged fit, 0->no plot on graph     
        //TF1* lambdafit = new TF1("lambdafit", "pol1", 1.08, 1.11);
        //lambdafit->SetLineColor(kGreen);
        //hLambda->Fit("lambdafit", "RQ");

        ////Extracting fit info
        //Double_t pk0 = kaonfit->GetParameter(0);
        //Double_t pk1 = kaonfit->GetParameter(1);
        //Double_t pl0 = lambdafit->GetParameter(0);
        //Double_t pl1 = lambdafit->GetParameter(1);

        ////Defining kaon background fit as a fuction and extending
        ////it to range under particle bump
        //Double_t lowerkaonbound = 0.44;
        //Double_t upperkaonbound = 0.56;
        //Double_t lowerlambdabound = 1.10;
        //Double_t upperlambdabound = 1.13;
        //
        ////Defining plots of fits above and extending range to be under particle bump
        //TF1* kaonbgext = new TF1("kaonbgext", "[0]+[1]*x",lowerkaonbound,upperkaonbound);
        //kaonbgext->SetParameters(pk0, pk1);
        //TF1* lambdabgext = new TF1("lambdabgext", "[0]+[1]*x", lowerlambdabound, upperlambdabound);
        //lambdabgext->SetParameters(pl0, pl1);

        //cout << "p0 is " << pl0 << endl;
        //cout << "p1 is " << pl1 << endl;
        //


        ////Initializing counts
        //Double_t kaontot = 0;
        //Double_t kaonuncertainty = 0;
        //Double_t lambdatot = 0;
        //Double_t lambdauncertainty = 0;
        ////New graphs to plot data with errors
        //TGraphErrors *Gkaon = new TGraphErrors();
        //TGraphErrors *Glambda = new TGraphErrors();
        ////Initializing n =0 for graph, weird input needed for graphing in root
        //int n = 0;
        ////loop to subtract out background for bins under kaon peak and count
        //for (Int_t i = hKaon->FindFixBin(lowerkaonbound); i <= hKaon->FindFixBin(upperkaonbound); i++) {
        //    Double_t backgroundsubtractedbin = hKaon->GetBinContent(i) - (pk0 + pk1*(hKaon->GetBinLowEdge(i)));
        //    if (backgroundsubtractedbin >= 0) {
        //        kaontot = kaontot + backgroundsubtractedbin;
        //        kaonuncertainty = kaonuncertainty + sqrt(backgroundsubtractedbin);
        //        n = Gkaon->GetN();
        //        Gkaon->SetPoint(n,hKaon->GetBinLowEdge(i)+kaonbinw/2, backgroundsubtractedbin);
        //        Gkaon->SetPointError(n,kaonbinw/2, sqrt(backgroundsubtractedbin));
        //    }
        //    else if (backgroundsubtractedbin < 0) {
        //        n = Gkaon->GetN();
        //        Gkaon->SetPoint(n, hKaon->GetBinLowEdge(i) + kaonbinw / 2, 0);
        //        Gkaon->SetPointError(n, kaonbinw / 2, 0);
        //    }
        //}
        ////Lambda loop to subtract background, different loop bc of different bins
        //n = 0;
        //for (Int_t i = hLambda->FindFixBin(lowerlambdabound); i <= hLambda->FindFixBin(upperlambdabound); i++) {
        //    Double_t x = hLambda->GetBinLowEdge(i);
        //    Double_t backgroundsubtractedbin = hLambda->GetBinContent(i) - (pl0 + pl1*x);
        //    if (backgroundsubtractedbin >= 0) {
        //        lambdatot = lambdatot + backgroundsubtractedbin;
        //        lambdauncertainty = lambdauncertainty + sqrt(backgroundsubtractedbin);
        //        n = Glambda->GetN();
        //        Glambda->SetPoint(n, hLambda->GetBinLowEdge(i) + lambdabinw / 2, backgroundsubtractedbin);
        //        Glambda->SetPointError(n, lambdabinw / 2, sqrt(backgroundsubtractedbin));
        //    }
        //    else if (backgroundsubtractedbin < 0) {
        //        n = Gkaon->GetN();
        //        Glambda->SetPoint(n, hLambda->GetBinLowEdge(i) + lambdabinw / 2, 0);
        //        Glambda->SetPointError(n, lambdabinw / 2, 0);
        //    }
        //}

        //cout << "Kaon Total Count is " << kaontot << " +/- " << kaonuncertainty << endl;
        //cout << "Lambda Total Count is " << lambdatot << " +/- " << lambdauncertainty << endl;
        ////Adding Legend Entries
        //TLegend* kleg = new TLegend(0.6, 0.7, 0.9, 0.9);
        //kleg->AddEntry(hKaon,"Kaon Peak with Background", "l");
        //kleg->AddEntry(kaonfit,"Background Fit", "l");
        //kleg->AddEntry(kaonbgext,"Background Fit Extended Under Peak", "l");
        //kleg->AddEntry(Gkaon, "Extracted Kaon Peak", "l");
        //hKaon->SetStats(0);
        //hLambda->SetStats(0);
        //// Final draw
        //hKaon->Draw();
        //Gkaon->Draw("SAME");
        //kaonbgext->Draw("SAME");
        //kleg->Draw("SAME");
        //canvas->SaveAs("Kaon.pdf");
        //TCanvas* canvas2 = new TCanvas("ODS2", "Plot", 10, 10, 800, 400);
        //TLegend* lleg = new TLegend(0.6, 0.7, 0.9, 0.9);
        //lleg->AddEntry(hLambda, "Lambda Peak with Background", "l");
        //lleg->AddEntry(lambdafit, "Background Fit", "l");
        //lleg->AddEntry(lambdabgext, "Background Fit Extended Under Peak", "l");
        //lleg->AddEntry(Glambda, "Extracted Lambda Peak", "l");
        //hLambda->Draw();
        //Glambda->Draw("SAME");
        //lambdabgext->Draw("SAME");
        //lleg->Draw("SAME");
        //canvas2->SaveAs("Lambda.pdf");
        //hAP->Draw();
        //gPad->Update();
        ////
        //TFile file(opts.GetOutput(), "RECREATE");
        //hKaon->Write();
        //Gkaon->Write();
        //Glambda->Write();
        //kaonbgext->Write();
        //lambdabgext->Write();
        //hLambda->Write();
        //hAP->Write();
        //file.Write();
        //file.Close();
        //cout << "Histograms written to " << opts.GetOutput() << endl;
        return 0;
    }