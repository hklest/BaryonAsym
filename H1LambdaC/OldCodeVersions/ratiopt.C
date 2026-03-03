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
#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>
#include <TClonesArray.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TDatabasePDG.h>
#include <TApplication.h>
     //Added for fitting
#include <TMath.h>
#include <TF1.h>
#include <TLegend.h>
#include <TGraph.h>
#include <TGraphErrors.h>

// H1 OO includes
#include "H1Skeleton/H1Tree.h"
  // tracks
#include "H1Tracks/H1Track.h"
  // vertices
#include "H1Tracks/H1CentralVertex.h"
#include "H1Tracks/H1CentralVertexArrayPtr.h"
#include "H1Steering/H1StdCmdLine.h"

using namespace std;

//Defining functions to use for fitting data and background
double bgk(double* x, double* par) {
    if (x[0] > 0.44 && x[0] < 0.57) {
        TF1::RejectPoint();
        return 0;
    }
    return par[0] + par[1] * x[0] + par[2] * x[0] * x[0] + par[3]*x[0]*x[0]*x[0]+par[4]* x[0]* x[0]* x[0]* x[0]+ par[5] * x[0] * x[0] * x[0] * x[0]*x[0]+ par[6] * x[0] * x[0] * x[0] * x[0]*x[0]*x[0];
}

double bgl(double* x, double* par) {
    if (x[0] > 1.1 && x[0] < 1.13) {
        TF1::RejectPoint();
        return 0;
    }
    return par[0] + par[1] * x[0] + par[2] * x[0] * x[0] + par[3] * x[0] * x[0] * x[0] + par[4] * x[0] * x[0] * x[0] * x[0] + par[5] * x[0] * x[0] * x[0] * x[0] * x[0] + par[6] * x[0] * x[0] * x[0] * x[0] * x[0] * x[0];
}

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

    

    // Histograms to fill and defining their parameters
    Double_t kaonbins = 500;
    Double_t lambdabins = 1000;
    Double_t kaonupperbinrange = 1.0;
    Double_t kaonlowerbinrange = 0.0;
    Double_t lambdaupperbinrange = 1.5;
    Double_t lambdalowerbinrange = 1.0;
    Double_t kaonbinw = (kaonupperbinrange-kaonlowerbinrange) / kaonbins;
    Double_t lambdabinw = (lambdaupperbinrange-lambdalowerbinrange) / lambdabins;
    TH1F* hKaon = new TH1F("Kaon", "Kaon", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* hLambda = new TH1F("Lambda", "Lambda", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH2F* hAP = new TH2F("AP", "AP", 200, -1, 1, 200, 0, 0.3);
    TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 400);

    //Defining tranverse momemntum, pt, bins to fill
    Double_t lowerpt = 1;
    Double_t upperpt = 2;
    //This is the number of loops and kaon/lambda plots we will have to generate
    //and the number of data points on the pt dependence curve
    Double_t numptbins = 5;
    Double_t ptbinwidth = (upperpt - lowerpt) / numptbins;

    //define radial vertex cuts for distance of closest approach and beampipe
    const Float_t minDca = 2.;
    const Float_t maxDca = 40.;
    const Float_t lowerBeamPipe = 4.;
    const Float_t upperBeamPipe = 5.5;

    // define transverse momentum cuts on tracks
    const Float_t minPtPion = 0.18;
    //const Float_t minPtKZero = 0.15;
    const Float_t minTracklenPion = 15;
    //need to add separate ptrelcuts for different plots
    const Float_t ptrelCut = 0.12;
    const Float_t ptrelkexcludelow = 0.48;
    const Float_t ptrelkexcludehigh = 0.52;


    // Pointer to vertices to loop over
    static H1CentralVertexArrayPtr vertex;

    // Get the pion's mass
    Double_t mPion = TDatabasePDG::Instance()->GetParticle(211)->Mass();
    // Get proton mass
    Double_t mProton = TDatabasePDG::Instance()->GetParticle(2212)->Mass();

    ///////////////BEGIN MAIN LOOP////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    // Load mODS/HAT files
    H1Tree::Instance()->Open();            // this statement must be here

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

                   

                    H1Track* firstDaughter = (H1Track*)daughters->At(0);
                    H1Track* secondDaughter = (H1Track*)daughters->At(1);

                       // Breaks if the daughters are not opposite charges
                     if (firstDaughter->GetCharge() != (-1) * secondDaughter->GetCharge()) {
                            // DST bug: Link from vertex fitted track
                            // to non-vertex fitted track missing
                            break;
                     }

                     if ((firstDaughter->GetPt() < minPtPion
                         || secondDaughter->GetPt() < minPtPion)
                         //|| ((H1Track*)mothers->At(0))->GetPt() < minPtKZero
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

                     //Fill Armenteros-Podolanski Plot
                     hAP->Fill(alpha, pTrelOne);

                     //Getting Kaon invariant mass
                     Double_t invMasskaon = (firstDaughter->GetFourVector(mPion)
                         + secondDaughter->GetFourVector(mPion)).M();
                     //Implement ptrel cut and fill Kaon invariant mass if it passes
                     if (TMath::Min(pTrelOne, pTrelTwo) >= ptrelCut) {
                         //Comment this out if a specific pt range is not desired
                         if (((H1Track*)mothers->At(0))->GetPt() >= 1.6 && ((H1Track*)mothers->At(0))->GetPt()< 2.2) {
                             hKaon->Fill(invMasskaon);;
                         }
                       
                     }


                     //Implement pt cut for Lambdas and kill invariant mass if it passes
                     if (invMasskaon < ptrelkexcludelow || invMasskaon < ptrelkexcludehigh) {
                         if (qlOne > qlTwo && (chargeOne * alpha > 0.4 || chargeOne * alpha < 0.4)) {
                             Double_t invMasslambda = (firstDaughter->GetFourVector(mProton)
                                 + secondDaughter->GetFourVector(mPion)).M();
                             if (((H1Track*)mothers->At(0))->GetPt() >= 1.6 && ((H1Track*)mothers->At(0))->GetPt() < 2.2) {
                                 hLambda->Fill(invMasslambda);;
                             }
                             
                         }
                         else if (qlOne < qlTwo && (chargeTwo * alpha > 0.4 || chargeTwo * alpha < 0.4)) {
                             Double_t invMasslambda = (firstDaughter->GetFourVector(mPion)
                                 + secondDaughter->GetFourVector(mProton)).M();
                             if (((H1Track*)mothers->At(0))->GetPt() >= 1.6 && ((H1Track*)mothers->At(0))->GetPt() < 2.2) {
                                 hLambda->Fill(invMasslambda);;
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

        /////////////Fitting functions and trying to subtract backgrounds///
   
        //Fitting smooth background portions of Kaon and Lambda peaks
        //These ranges obtained from trial and error looking at plots of ~1000000 total events
        Double_t lowkaonfitbound = 0.38;
        Double_t upperkaonfitbound = 0.7;
        Double_t lowkaonplotbound = 0.38;
        Double_t upperkaonplotbound = 0.6;
        TF1* kaonfit = new TF1("kaonfit", bgk, lowkaonfitbound, upperkaonfitbound, 7);
        kaonfit->SetLineColor(kGreen);
        kaonfit->SetLineWidth(4);
        //Initialize paremeter values before fitting to data
        //kaonfit->SetParameters(1,1,1,30,0.5,0.02);
        //kaonfit->SetParLimits(0, -70000, 0);
        //kaonfit->SetParLimits(1, 0, 600000);
        //kaonfit->SetParLimits(2, -400000, 0);
        //kaonfit->SetParLimits(3, 1, 100);
        //kaonfit->SetParLimits(4, 0.3, 0.4);
        //kaonfit->SetParLimits(5, 0.001, 0.1);
        hKaon->Fit("kaonfit", "RQ0");
        Double_t pk0 = kaonfit->GetParameter(0);
        Double_t pk1 = kaonfit->GetParameter(1);
        Double_t pk2 = kaonfit->GetParameter(2);
        Double_t pk3 = kaonfit->GetParameter(3);
        Double_t pk4 = kaonfit->GetParameter(4);
        Double_t pk5 = kaonfit->GetParameter(5);
        Double_t pk6 = kaonfit->GetParameter(6);

        Double_t lowlambdafitbound = 1.09;
        Double_t upperlambdafitbound = 1.20;
        Double_t lowlambdaplotbound = 1.085;
        Double_t upperlambdaplotbound = 1.20;
        TF1* lambdafit = new TF1("lambdafit", bgl, lowlambdafitbound, upperlambdafitbound, 7);
        lambdafit->SetLineColor(kGreen);
        lambdafit->SetLineWidth(4);
        hLambda->Fit("lambdafit", "RQ0");
        Double_t pl0 = lambdafit->GetParameter(0);
        Double_t pl1 = lambdafit->GetParameter(1);
        Double_t pl2 = lambdafit->GetParameter(2);
        Double_t pl3 = lambdafit->GetParameter(3);
        Double_t pl4 = lambdafit->GetParameter(4);
        Double_t pl5 = lambdafit->GetParameter(5);
        Double_t pl6 = lambdafit->GetParameter(6);



        //oldfit  
        //hKaon->Fit("kaonfit", "RQ");//Plot the fit directly if desired, R->ranged fit, 0->no plot on graph     
        //TF1* lambdafit = new TF1("lambdafit", "pol1", 1.08, 1.11);
        //lambdafit->SetLineColor(kGreen);
        //hLambda->Fit("lambdafit", "RQ0");


        //Defining kaon background fit as a fuction and extending
        //it to range under particle bump
        Double_t lowerkaonbound = 0.459;
        Double_t upperkaonbound = 0.541;
        Double_t lowerlambdabound = 1.105;
        Double_t upperlambdabound = 1.125;
        
        //Defining plots of fits above and extending range to be under particle bump
        TF1* kaonbgext = new TF1("kaonbgext", "[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x+[6]*x*x*x*x*x*x",lowkaonplotbound,upperkaonplotbound);
        kaonbgext->SetParameters(pk0, pk1, pk2, pk3, pk4, pk5, pk6);

        TF1* kaonbgextsub = new TF1("kaonbgextsub", "[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x+[6]*x*x*x*x*x*x", lowerkaonbound, upperkaonbound);
        kaonbgextsub->SetParameters(pk0, pk1, pk2, pk3, pk4, pk5, pk6);
        kaonbgextsub->SetLineColor(kMagenta);

        TF1* lambdabgext = new TF1("lambdabgext", "[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x+[6]*x*x*x*x*x*x", lowlambdaplotbound, upperlambdaplotbound);
        lambdabgext->SetParameters(pl0, pl1, pl2, pl3, pl4, pl5, pl6);
        
        TF1* lambdabgextsub = new TF1("lambdabgextsub", "[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x+[6]*x*x*x*x*x*x", lowerlambdabound, upperlambdabound);
        lambdabgextsub->SetParameters(pl0, pl1, pl2, pl3, pl4, pl5, pl6);
        lambdabgextsub->SetLineColor(kMagenta);

        //Initializing counts
        Double_t kaontot = 0;
        Double_t kaonuncertainty = 0;
        Double_t lambdatot = 0;
        Double_t lambdauncertainty = 0;
        //New graphs to plot data with errors
        TGraphErrors *Gkaon = new TGraphErrors();
        TGraphErrors *Glambda = new TGraphErrors();
        //Initializing n =0 for graph, weird input needed for graphing in root
        int n = 0;
        //loop to subtract out background for bins under kaon peak and count
        for (Int_t i = hKaon->FindFixBin(lowerkaonbound); i <= hKaon->FindFixBin(upperkaonbound); i++) {
            Double_t x = hKaon->GetBinLowEdge(i);
            Double_t backgroundsubtractedbin = hKaon->GetBinContent(i) - (pk0 + pk1 * x + pk2 * x * x + pk3 * x * x * x + pk4 * x * x * x * x + pk5 * x * x * x * x * x + pk6 * x * x * x * x * x * x);
            if (backgroundsubtractedbin >= 0) {
                kaontot = kaontot + backgroundsubtractedbin;
                kaonuncertainty = kaonuncertainty + sqrt(backgroundsubtractedbin);
                n = Gkaon->GetN();
                Gkaon->SetPoint(n,hKaon->GetBinLowEdge(i)+kaonbinw/2, backgroundsubtractedbin);
                Gkaon->SetPointError(n,kaonbinw/2, sqrt(backgroundsubtractedbin));
            }
            else if (backgroundsubtractedbin < 0) {
                n = Gkaon->GetN();
                Gkaon->SetPoint(n, hKaon->GetBinLowEdge(i) + kaonbinw / 2, 0);
                Gkaon->SetPointError(n, kaonbinw / 2, 0);
            }
        }
        //Lambda loop to subtract background, different loop bc of different bins
        n = 0;
        for (Int_t i = hLambda->FindFixBin(lowerlambdabound); i <= hLambda->FindFixBin(upperlambdabound); i++) {
            Double_t x = hLambda->GetBinLowEdge(i);
            Double_t backgroundsubtractedbin = hLambda->GetBinContent(i) - (pl0 + pl1 * x + pl2 * x * x + pl3 * x * x * x + pl4 * x * x * x * x + pl5 * x * x * x * x * x + pl6 * x * x * x * x * x * x);

            if (backgroundsubtractedbin >= 0) {
                lambdatot = lambdatot + backgroundsubtractedbin;
                lambdauncertainty = lambdauncertainty + sqrt(backgroundsubtractedbin);
                n = Glambda->GetN();
                Glambda->SetPoint(n, hLambda->GetBinLowEdge(i) + lambdabinw / 2, backgroundsubtractedbin);
                Glambda->SetPointError(n, lambdabinw / 2, sqrt(backgroundsubtractedbin));
            }
            else if (backgroundsubtractedbin < 0) {
                n = Glambda->GetN();
                Glambda->SetPoint(n, hLambda->GetBinLowEdge(i) + lambdabinw / 2, 0);
                Glambda->SetPointError(n, lambdabinw / 2, 0);
            }
        }

        cout << "Kaon Total Count is " << kaontot << " +/- " << kaonuncertainty << endl;
        cout << "Lambda Total Count is " << lambdatot << " +/- " << lambdauncertainty << endl;
        //Adding Legend Entries
        TLegend* kleg = new TLegend(0.6, 0.7, 0.9, 0.9);
        kleg->AddEntry(hKaon,"Kaon Peak with Background", "l");
        kleg->AddEntry(kaonfit,"Background Fit", "l");
        kleg->AddEntry(kaonbgext,"Background Fit Extended Under Peak", "l");
        kleg->AddEntry(Gkaon, "Extracted Kaon Peak", "l");
        hKaon->SetStats(0);
        hLambda->SetStats(0);
        // Final draw
        hKaon->Draw();
        Gkaon->Draw("SAME");
        kaonbgext->Draw("SAME");
        kaonbgextsub->Draw("SAME");
        kleg->Draw("SAME");
        canvas->SaveAs("Kaon.pdf");
        TCanvas* canvas2 = new TCanvas("ODS2", "Plot", 10, 10, 800, 400);
        TLegend* lleg = new TLegend(0.6, 0.7, 0.9, 0.9);
        lleg->AddEntry(hLambda, "Lambda Peak with Background", "l");
        lleg->AddEntry(lambdafit, "Background Fit", "l");
        lleg->AddEntry(lambdabgext, "Background Fit Extended Under Peak", "l");
        lleg->AddEntry(Glambda, "Extracted Lambda Peak", "l");
        hLambda->Draw();
        Glambda->Draw("SAME");
        lambdabgext->Draw("SAME");
        lambdabgextsub->Draw("SAME");
        lleg->Draw("SAME");
        canvas2->SaveAs("Lambda.pdf");
        hAP->Draw();
        gPad->Update();
        //
        TFile file(opts.GetOutput(), "RECREATE");
        hKaon->Write();
        Gkaon->Write();
        Glambda->Write();
        kaonbgext->Write();
        lambdabgext->Write();
        hLambda->Write();
        hAP->Write();
        file.Write();
        file.Close();
        cout << "Histograms written to " << opts.GetOutput() << endl;
        return 0;
    }