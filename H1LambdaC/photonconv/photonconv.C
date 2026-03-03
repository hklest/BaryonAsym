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
#include "H1Mods/H1PartConvPhoton.h"
#include "H1Mods/H1PartConvPhotonArrayPtr.h"

#include "H1Mods/H1PartLambda.h"
#include "H1Mods/H1SelVertex.h"
#include "H1Tracks/H1ReconstructedVertex.h"
#include "H1Tracks/H1CentralVertex.h"
#include "H1Tracks/H1CentralVertexArrayPtr.h"
#include "H1Steering/H1StdCmdLine.h"
#include "H1Tracks/H1RunVertex.h"

#include "H1Tracks/H1RunVertexPtr.h"


using namespace std;



int
main(int argc, char* argv[])
{
    //Output file name and path
   // string output = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/photonconv.root";
    // event counter
    Int_t eventCounter = 0;
    // parse the command line
    H1StdCmdLine opts;
    opts.Parse(&argc, argv);
    // needed for graphic, but MUST be AFTER Parse(...):
    //TApplication theApp("kaonfind_ods", &argc, argv);

    // Load mODS/HAT files
    H1Tree::Instance()->Open();            // this statement must be here

    

    //defining the 2D hists for vertex density
    TH2F* XYvtxdensity = new TH2F("Photon Coversion XY Vertices", "Photon Coversion XY Vertices", 2000, -100, 100, 2000, -100, 100);
    TH2F* ZRvtxdensity = new TH2F("Photon Coversion ZR Vertices", "Photon Coversion ZR Vertices", 2000, -100, 100, 2000, -100, 100);

    TH2F* ZXvtxdensity = new TH2F("Photon Coversion ZX Vertices", "Photon Coversion ZX Vertices", 2000, -100, 100, 2000, -100, 100);
    TH2F* ZYvtxdensity = new TH2F("Photon Coversion ZY Vertices", "Photon Coversion ZY Vertices", 2000, -100, 100, 2000, -100, 100);
  
  
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //Defining Root Canvas to work on
    TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 800);

   //
   // 
    // Pointer to vertices to loop over
    
    static H1PartConvPhotonArrayPtr PhotonCand;
    //static H1CentralFittedV0ArrayPtr v0;
   
    //static H1CentralFittedV0ArrayPtr v0;

    
    // Get the pion's mass
    Double_t mPion = TDatabasePDG::Instance()->GetParticle(211)->Mass();
    // Get proton mass
    Double_t mProton = TDatabasePDG::Instance()->GetParticle(2212)->Mass();
    // Get electron mass
    Double_t mElectron = TDatabasePDG::Instance()->GetParticle(11)->Mass();
    // Get Lambda mass
    Double_t mLambda = TDatabasePDG::Instance()->GetParticle(3122)->Mass();

    //static H1RunVertexPtr beam;

    const Double_t mElec = TDatabasePDG::Instance()->GetParticle(11)->Mass();


    TH1F* h1 = new TH1F("Photon", "Photon", 200, 0, 1);
    TH1F* h2 = new TH1F("Photon", "Photon", 200, 0, 1);
    TH1F* h3 = new TH1F("Photon", "Photon", 200, 0, 1);
    TH1F* h4 = new TH1F("Photon", "Photon", 200, 0, 1);
    TH1F* h5 = new TH1F("Photon", "Photon", 200, 0, 1);
    TH1F* h6 = new TH1F("Photon", "Photon", 200, 0, 1);
    TH1F* h7 = new TH1F("Photon", "Photon", 200, 0, 1);
    TH1F* h8 = new TH1F("Photon", "Photon", 200, 0, 1);
    TH1F* hfinal = new TH1F("Photon", "Photon", 200, 0, 1);

    THStack* hs = new THStack("hs", "Stacked Mass Plots");


    ///////////////BEGIN MAIN LOOP////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    // Loop as long as there's data
    while (H1Tree::Instance()->Next() && !opts.IsMaxEvent(eventCounter)) {

        // Loop over all Lambda candidates on MODS level as determined by rough cuts in H1FindLambda code
        for (Int_t loop = 0; loop < PhotonCand.GetEntries(); loop++) {
            
            Double_t invMass = (PhotonCand[loop]->GetElec1()->GetFourVector(mElec)
                + PhotonCand[loop]->GetElec2()->GetFourVector(mElec)).M();
            
            h1->Fill(invMass);

            //Doing SDCA cut to get rid of tracks from primary vertex
            //Getting H1PartSelTracks for daughters
            const H1PartSelTrack* E1PSTrack = PhotonCand[loop]->GetPosElec();
            const H1PartSelTrack* E2PSTrack = PhotonCand[loop]->GetNegElec();

            //Prepating DCA/dDCA information to get a histogram to help visualize this DCA Significance cut
            const H1CentralTrack* e1FittedNoV = PhotonCand[loop]->GetElec1()->GetCentralTrack();
            const H1Trajectory* e1Trajectory = e1FittedNoV->GetTrajectory();

            const H1CentralTrack* e2FittedNoV = PhotonCand[loop]->GetElec2()->GetCentralTrack();
            const H1Trajectory* e2Trajectory = e2FittedNoV->GetTrajectory();

            H1SelVertex* E1Vertex = (H1SelVertex*)PhotonCand[loop]->GetElec1();
            const H1SelVertex* E1PrimaryVertex = E1Vertex->GetPrimaryVertex();
            TVector3 pv = E1PrimaryVertex->GetPosition();//is same for both protons and pion daughters, so just using proton here and previous couple lines
            TVector2 V2PV(pv.X(), pv.Y());

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (abs((e1Trajectory->GetDca(V2PV)) / (e1Trajectory->GetdDca(V2PV))) < 1 || abs((e2Trajectory->GetDca(V2PV)) / (e2Trajectory->GetdDca(V2PV))) <1) {
                continue;
            }

            h2->Fill(invMass);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //cout << E1PSTrack->GetDedxLikelihood(H1Dedx::kElectron) << endl;
            //cout << E2PSTrack->GetDedxLikelihood(H1Dedx::kElectron) << endl;
            //if (E1PSTrack->GetDedxLikelihood(H1Dedx::kElectron) < 0.001 || E2PSTrack->GetDedxLikelihood(H1Dedx::kElectron) < 0.001) {continue;}
            /////////////////////////////////
            
            //h3->Fill(invMass);
          
            // After all cuts fill in vertex density
            if (invMass > 0.05) { continue; }

            h4->Fill(invMass);

            double vtxX = PhotonCand[loop]->GetDecayVtxX();
            double vtxY = PhotonCand[loop]->GetDecayVtxY();
            double vtxZ = PhotonCand[loop]->GetDecayVtxZ();

            XYvtxdensity->Fill(vtxX,vtxY);
            ZRvtxdensity->Fill(vtxZ, sqrt((vtxX*vtxX)+(vtxY*vtxY)));
            ZXvtxdensity->Fill(vtxZ,vtxX);
            ZYvtxdensity->Fill(vtxZ,vtxY);
         
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

    }

 
       TFile file(opts.GetOutput(), "RECREATE");
      
       XYvtxdensity->SetStats(0);
       XYvtxdensity->GetXaxis()->SetTitle("X");
       XYvtxdensity->GetYaxis()->SetTitle("Y");
       //XYvtxdensity->GetXaxis()->SetTitleSize(0.04);
       XYvtxdensity->GetXaxis()->SetTitleOffset(1);
       XYvtxdensity->GetYaxis()->SetTitleOffset(1.3);
       //XYvtxdensity->GetYaxis()->SetTitleSize(0.05);
       XYvtxdensity->Draw("colz");
       canvas->SetName("Photon Coversion XY Vertices");
       canvas->Write();

       ZRvtxdensity->SetStats(0);
       ZRvtxdensity->GetXaxis()->SetTitle("Z");
       ZRvtxdensity->GetYaxis()->SetTitle("R");
       //ZRvtxdensity->GetXaxis()->SetTitleSize(0.04);
       //ZRvtxdensity->GetXaxis()->SetTitleOffset(1);
       XYvtxdensity->GetYaxis()->SetTitleOffset(0);
       //ZRvtxdensity->GetYaxis()->SetTitleSize(0.05);
       ZRvtxdensity->Draw("colz");
       canvas->SetName("Photon Coversion ZR Vertices");
       canvas->Write();

       ZXvtxdensity->SetStats(0);
       ZXvtxdensity->GetXaxis()->SetTitle("Z");
       ZXvtxdensity->GetYaxis()->SetTitle("X");
       //ZXvtxdensity->GetXaxis()->SetTitleSize(0.04);
       //ZXvtxdensity->GetXaxis()->SetTitleOffset(1);
       XYvtxdensity->GetYaxis()->SetTitleOffset(0);
       //ZXvtxdensity->GetYaxis()->SetTitleSize(0.05);
       ZXvtxdensity->Draw("colz");
       canvas->SetName("Photon Coversion ZX Vertices");
       canvas->Write();

       ZYvtxdensity->SetStats(0);
       ZYvtxdensity->GetXaxis()->SetTitle("Z");
       ZYvtxdensity->GetYaxis()->SetTitle("Y");
       //ZYvtxdensity->GetXaxis()->SetTitleSize(0.04);
       //ZYvtxdensity->GetXaxis()->SetTitleOffset(1);
       XYvtxdensity->GetYaxis()->SetTitleOffset(0);
       //ZYvtxdensity->GetYaxis()->SetTitleSize(0.05);
       ZYvtxdensity->Draw("colz");
       canvas->SetName("Photon Coversion ZY Vertices");
       canvas->Write();
     

       h1->SetStats(0);
       h1->SetLineColor(kBlack);
       h1->GetXaxis()->SetTitle("Mass [Gev]");
       h1->GetYaxis()->SetTitle("Count");
       h1->Draw();
       h1->Write();

       h2->SetStats(0);
       h2->SetLineColor(kMagenta);
       h2->GetXaxis()->SetTitle("Mass [Gev]");
       h2->GetYaxis()->SetTitle("Count");
       h2->Draw();
       h2->Write();

       h3->SetStats(0);
       h3->SetLineColor(kRed);
       h3->GetXaxis()->SetTitle("Mass [Gev]");
       h3->GetYaxis()->SetTitle("Count");
       h3->Draw();
       h3->Write();

       h4->SetStats(0);
       h4->SetLineColor(kBlue);
       h4->GetXaxis()->SetTitle("Mass [Gev]");
       h4->GetYaxis()->SetTitle("Count");
       h4->Draw();
       h4->Write();

       h5->SetStats(0);
       h5->SetLineColor(kGreen);
       h5->GetXaxis()->SetTitle("Mass [Gev]");
       h5->GetYaxis()->SetTitle("Count");
       h5->Draw();
       h5->Write();

       h6->SetStats(0);
       h6->SetLineColor(kMagenta);
       h6->GetXaxis()->SetTitle("Mass [Gev]");
       h6->GetYaxis()->SetTitle("Count");
       h6->Draw();
       h6->Write();

       h7->SetStats(0);
       h7->SetLineColor(kMagenta);
       h7->GetXaxis()->SetTitle("Mass [Gev]");
       h7->GetYaxis()->SetTitle("Count");
       h7->Draw();
       h7->Write();

       h8->SetStats(0);
       h8->SetLineColor(kMagenta);
       h8->GetXaxis()->SetTitle("Mass [Gev]");
       h8->GetYaxis()->SetTitle("Count");
       h8->Draw();
       h8->Write();

       hfinal->SetStats(0);
       hfinal->SetLineColor(kBlack);
       hfinal->GetXaxis()->SetTitle("Mass [Gev]");
       hfinal->GetYaxis()->SetTitle("Count");
       hfinal->Draw();
       hfinal->Write();

       hs->Add(h1);
       hs->Add(h2);
       hs->Add(h3);
       hs->Add(h4);
       hs->Add(h5);
       hs->Add(h6);
       hs->Add(h7);
       hs->Add(h8);
       hs->Add(hfinal);
       hs->Draw("nostack");
       canvas->SetName("Cut Stack");
       canvas->Write();

       file.Write();
       file.Close();
       cout << "Histograms written to " << opts.GetOutput() << endl;


        return 0;
    }