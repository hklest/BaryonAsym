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


//minimum finder function
//returns the index value of the smallest member of vector
//I am not worried about the case where a minimum appears multiple times since this vector will just output the index of one the instances of the minimum
int minfinder(vector<double> vec) {
    double smallestvalue = 0; //initialize smallest value
    int index = 0;

    for (Int_t a = 0; a < vec.size(); a++) {

        if (a == 0) { smallestvalue = vec[0]; }

        if (a != 0) {
            if (vec[a] < smallestvalue) {
                smallestvalue = vec[a];
                index = a;
            }
        }

    }
    return index;
}


//Finder function, looks for values in a vector
//It only finds the first instance, if it finds the first instance of the value then it returns the index in the array the value is located at
//If the function does not find the value, it returns -1
int singlefind(vector<double> vec, int begin_index, int end_index, double value) {

    for (Int_t a = begin_index; a < end_index; a++) {

        if (vec[a] == value) { return a; }

    }

    return -1;

}


int intsinglefind(vector<int> vec, int begin_index, int end_index, double value) {

    for (Int_t a = begin_index; a < end_index; a++) {

        if (vec[a] == value) { return a; }

    }

    return -1;

}


//This finder calls the single find, it outputs a vector containing indices of all instances of the value we are looking for
vector<int> find(vector<double> vec, double value) {
    //intializing vector for storing output indices
    vector<int> output = {};
    //initializing findvalue
    int findvalue = 0;
    //initializing index for while loop
    int index = 0;

    while (findvalue != -1 && index < vec.size()) {

        findvalue = singlefind(vec, index, vec.size(), value);
        if (findvalue != -1) { output.push_back(findvalue); }
        index = findvalue + 1;

    }

    return output;

}

//This finder will search a vector for duplicate entries
//if it finds dupicate entries, it will collect the indices where they appear
//If there are multuple duplicate entries, it will save this as a seperate vector
//So final output is a vector of vectors
vector<vector<int>> findduplicates(vector<double> vec) {

    vector<double> duplicatevalueslareadyfound = {}; //if we find a value that is duplicated, we will store it here, then we will have a check look into this array to make sure we don't double count things

    vector<int> value;

    vector<vector<int>> output = {};

    for (Int_t a = 0; a < vec.size(); a++) {

        value = find(vec, vec[a]);

        //if the finder didn't find a duplicate, then it only finds itself and size of vector is 1, if it is <=1 we continue
        if (value.size() <= 1) { continue; }

        //If the code gets here it found a duplicate, in this case the duplicatevaluesalreadyfound vector is empty, must fill for first entry
        if (find(duplicatevalueslareadyfound, vec[a]).size() == 0) {
            output.push_back(value);
            duplicatevalueslareadyfound.push_back(vec[a]);
        }


    }

    return output;

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
    //TApplication theApp("kaonfind_ods", &argc, argv);

    // Load mODS/HAT files
    H1Tree::Instance()->Open();// this statement must be here

    // Histograms Binning, have some histograms filled directly from analysis that I use as a consistency check with the histograms I make from my Analysis code using the TTrees I create here
    Double_t lambdabins = 1000;
    Double_t lambdaupperbinrange = 1.5;
    Double_t lambdalowerbinrange = 1.0;
    
    // load in some HAT data
    H1FloatPtr q2s("Q2s");//Q2 calcualted from sigma method
    H1FloatPtr ys("Ys"); //Inelasticity calculated from sigma method
    H1FloatPtr ElecTheta("ElecTheta"); //Theta of scattered electron
    H1FloatPtr ElecPhi("ElecPhi");
    H1FloatPtr ElecE("ElecE"); //Energy of scattered electron
    H1FloatPtr EBeamE("EBeamE"); //Energy of electron beam from DMIS 
    H1FloatPtr EBeamP("EBeamP"); //Energy of proton beam from DMIS
    H1FloatPtr vtxZ("VtxZ");
    H1FloatPtr epz("Epz");

    TH2F* hXYDecayVertex = new TH2F("Decay Vertex", "Decay Vertex", 1000, -80, 80, 1000, -80, 80);
    TH2F* hXYPrimaryVertex = new TH2F("Primary Vertex", "PrimaryVertex", 1000, -5, 5, 1000, -5, 5);
    
    //Defining TTrees to store Lambda, Lambda Bar, and Combined Lambda candidates and their associated kinematic variables
    TTree* lambdaRegTree = new TTree("Lambda Reg", "Lambda Reg");
    TTree* lambdaBarTree = new TTree("Lambda Bar", "Lambda Bar");
    TTree* lambdaTotTree = new TTree("Lambda Tot", "Lambda Tot");
    //defining the variables I want to store in Lambda Trees, will save the TTrees and access branches later in a separate analysis code
    double pT, Eta, x, invMasslambda, Q2s, Ys, Angle, v0chi2;
    //Defining Tree Branches for Combined Lambdas, and seperate ones for Regular Lambdas vs Lambda Bars
    lambdaRegTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaRegTree->Branch("pT",&pT,"pT/D");
    lambdaRegTree->Branch("Eta", &Eta, "Eta/D");
    lambdaRegTree->Branch("x", &x, "x/D");
    lambdaRegTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaRegTree->Branch("Ys", &Ys, "Ys/D");
    lambdaRegTree->Branch("Angle", &Angle, "Angle/D");
    lambdaRegTree->Branch("v0chi2", &v0chi2, "v0chi2/D");

    lambdaBarTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaBarTree->Branch("pT", &pT, "pT/D");
    lambdaBarTree->Branch("Eta", &Eta, "Eta/D");
    lambdaBarTree->Branch("x", &x, "x/D");
    lambdaBarTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaBarTree->Branch("Ys", &Ys, "Ys/D");
    lambdaBarTree->Branch("Angle", &Angle, "Angle/D");
    lambdaBarTree->Branch("v0chi2", &v0chi2, "v0chi2/D");

    lambdaTotTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaTotTree->Branch("pT", &pT, "pT/D");
    lambdaTotTree->Branch("Eta", &Eta, "Eta/D");
    lambdaTotTree->Branch("x", &x, "x/D");
    lambdaTotTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaTotTree->Branch("Ys", &Ys, "Ys/D");
    lambdaTotTree->Branch("Angle", &Angle, "Angle/D");
    lambdaTotTree->Branch("v0chi2", &v0chi2, "v0chi2/D");
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  Defining Cut Progression Histograms

    THStack* hs = new THStack("hs", "Stacked Total Lambda Mass Plots");

    TH1F* LambdaCutProgression[12];
    for (int a = 0; a < 12; a++) {
        string lambdacutprogstring = "Lambda Cut " + to_string(a);
        LambdaCutProgression[a] = new TH1F(lambdacutprogstring.c_str(), lambdacutprogstring.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    }

    //Hard coding lambda peaks as a double check
    TH1F* LambdaBoth = new TH1F("Lambda Both", "Lambda Both", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* LambdaReg = new TH1F("Lambda Reg", "Lambda Reg", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* LambdaBar = new TH1F("Lambda Bar", "Lambda Bar", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    //

    //Histograms to visualize v0 cit Chi2 distributions
    TH1F* v0chi2bothhist = new TH1F("V0 Chi2 Dist Both", "V0 Chi2 Dist Both", 1000, 0, 10);
    TH1F* v0chi2reghist = new TH1F("V0 Chi2 Dist Reg", "V0 Chi2 Dist Reg", 1000, 0, 10);
    TH1F* v0chi2barhist = new TH1F("V0 Chi2 Dist Bar", "V0 Chi2 Dist Bar", 1000, 0, 10);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    //  Defining Root Canvas to work on
    TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 800);

   
    //  Pointer to vertices to loop over
    static H1CentralVertexArrayPtr vertex;
    static H1PartLambdaArrayPtr LambdaCand;
    
    
    // Get the pion's mass
    Double_t mPion = TDatabasePDG::Instance()->GetParticle(211)->Mass();
    // Get proton mass
    Double_t mProton = TDatabasePDG::Instance()->GetParticle(2212)->Mass();
    // Get electron mass
    Double_t mElectron = TDatabasePDG::Instance()->GetParticle(11)->Mass();

    
    //Initializing some consistency checks
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
        // Loop over all Lambda candidates on MODS level as determined by rough cuts in H1FindLambda.C code
        for (Int_t loop = 0; loop < LambdaCand.GetEntries(); loop++) {

            if (LambdaCand[loop]->GetNumOfParticles() < 2) { //Ensuring there are 2 stored daughters for candidates
                breakloopcount++;
                continue;
            }
        
        //Getting candidate invariant mass using reconstructed daughters
        invMasslambda = (LambdaCand[loop]->GetProtonFourVector()
            + LambdaCand[loop]->GetPionFourVector()).M();

        //Getting Vectors for Armenteros-Podolansky Plot
        H1Track* ProtonH1Track = (H1Track*)LambdaCand[loop]->GetProtonTrack(); //This is casting H1CentralFittedTrack h1oo class as H1Track for daughter
        H1Track* PionH1Track = (H1Track*)LambdaCand[loop]->GetPionTrack(); //This is casting H1CentralFittedTrack h1oo class as H1Track for daughter

        TVector3 vecProton = LambdaCand[loop]->GetProtonFourVector().Vect(); //Proton 3 vector
        TVector3 vecPion = LambdaCand[loop]->GetPionFourVector().Vect();  //Pion 3 vector
        TVector3 vecLambda = LambdaCand[loop]->GetPionFourVector().Vect();  //Lambda 3 vector
        
        //Call H1PartLambda class data that references H1CentralFittedTrack for Pion and Proton
        const H1CentralFittedTrack* ProtonCFTrack = LambdaCand[loop]->GetProtonTrack();
        const H1CentralFittedTrack* PionCFTrack = LambdaCand[loop]->GetPionTrack();

        //Getting H1PartSelTracks for daughters
        const H1PartSelTrack* ProtonPSTrack = LambdaCand[loop]->GetProton();
        const H1PartSelTrack* PionPSTrack = LambdaCand[loop]->GetPion();

        //Prepating DCA/dDCA information to get a histogram to help visualize this DCA Significance cut
        const H1CentralFittedV0* v0 = LambdaCand[loop]->GetV0();

        const H1CentralTrack* protonFittedNoV = ProtonCFTrack->GetCentralTrack();
        const H1Trajectory* protonTrajectory = protonFittedNoV->GetTrajectory();

        const H1CentralTrack* pionFittedNoV = PionCFTrack->GetCentralTrack();
        const H1Trajectory* pionTrajectory = pionFittedNoV->GetTrajectory();

        //Primary Vertex position
        H1SelVertex* ProtonVertex = (H1SelVertex*)LambdaCand[loop]->GetProtonTrack();
        const H1SelVertex* ProtonPrimaryVertex = ProtonVertex->GetPrimaryVertex();
        TVector3 pv = ProtonPrimaryVertex->GetPosition();//is same for both protons and pion daughters, so just using proton here and previous couple lines
        TVector2 V2PV(pv.X(), pv.Y()); //2 Vector for the primary vertex position

        //Decay Vertex Position
        const H1ReconstructedVertex* DecayVertex = v0->GetDecayVertex();
        TVector3 sv = DecayVertex->GetPosition();

        //Get the vector pointing from primary vertex to secondary vertex
        TVector3 PVtoSV = sv - pv;//vector pointing from primary event vertex to decay vertex
        TVector3 LambdaP = LambdaCand[loop]->GetFourVector().Vect();
        double angle = (180/(TMath::Pi()))*acos((PVtoSV.Dot(LambdaP)) / ((PVtoSV.Mag()) * (LambdaP.Mag()))); //Angle between reconstructed particle momentum and vector from PV to SV
     
        //Defining pT before cuts and putting stuff into pt bins
        
        pT = v0->GetPt();

        LambdaCutProgression[0]->Fill(invMasslambda); //Raw Lambda Candidates from rough cuts in H1FindLambda

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
        // Defining TTree variables to store into branches
        Eta = LambdaCand[loop]->GetFourVector().Vect().Eta();
        //x = (*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4);
        x = ((*ElecE) / (*EBeamP)) * (cos((*ElecTheta) / (2))) * (cos((*ElecTheta) / (2))) / (*ys); //Bjorken x calculated from Sigma Method, ISR independent
        Q2s = *q2s;
        Ys = *ys;
        pT = v0->GetPt();
        invMasslambda = (LambdaCand[loop]->GetProtonFourVector()
            + LambdaCand[loop]->GetPionFourVector()).M();
        Angle = angle;
        v0chi2 = v0->GetFitChi2();


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  I want to implement a few more cuts to have the full cut progression stack plot of my peak
//  Eta, pT, and angle cuts (angle is angle between Lambda candidate momentum and Vector from PV to DV to keep only primary vertex particles)
//  For these if the cut condition is passed, I will only fill LambdaCutProgression[index_value], and will not break the loop because in my TTree I want to see the fill pT, Eta, and Angle Distributions
//  I will fully implement this cut in the Analysis part of the code using the values stored in the TTree
        

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        //Eta Cut
//        if (abs(Eta) < 1.3) {
//            LambdaCutProgression[7]->Fill(invMasslambda);
//        }
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        //pT Cut
//        if (pT > 0.5 && pT < 3.5) {
//            LambdaCutProgression[8]->Fill(invMasslambda);
//        }
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////Eta Cut
        //if (abs(Eta) > 1.3) {
        //    breakloopcount++;
        //    continue;
        //}

        //LambdaCutProgression[7]->Fill(invMasslambda);


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //        //pT Cut
        //if (pT < 0.5 || pT > 3.5) {
        //    breakloopcount++;
        //    continue;
        //}

        //LambdaCutProgression[8]->Fill(invMasslambda);
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // STORE LAMBDA CANDIDATES INTO TTREES
        passedallcutscount++;
        lambdaTotTree->Fill();
        
        if (pT > 0.5 && pT < 3.5) {
            if (abs(Eta) < 1.3) {
                if (Ys > 0.1 && Ys < 0.6) {
                    if (Q2s > 5 && Q2s < 100) {
                        
                     LambdaBoth->Fill(invMasslambda);
                     v0chi2bothhist->Fill(v0chi2);

                     hXYDecayVertex->Fill(sv.X(),sv.Y());
                     hXYPrimaryVertex->Fill(pv.X(), pv.Y());
                        
                    }
                }
            }
        }


        if (LambdaCand[loop]->IsLambdaBar() == true) { 
            lambdaBarTree->Fill(); 
            if (pT > 0.5 && pT < 3.5) {
                if (abs(Eta) < 1.3) {
                    if (Ys > 0.1 && Ys < 0.6) {
                        if (Q2s > 5 && Q2s < 100) {

                            LambdaBar->Fill(invMasslambda);
                            v0chi2barhist->Fill(v0chi2);

                        }
                    }
                }
            }
        }

        if (LambdaCand[loop]->IsLambdaBar() != true) { 
            lambdaRegTree->Fill(); 
            if (pT > 0.5 && pT < 3.5) {
                if (abs(Eta) < 1.3) {
                    if (Ys > 0.1 && Ys < 0.6) {
                        if (Q2s > 5 && Q2s < 100) {

                            LambdaReg->Fill(invMasslambda);
                            v0chi2reghist->Fill(v0chi2);

                        }
                    }
                }
            }
        }

       
    
            
        }//loop over lambda candidate array
                
        //Outputting some consistency checks for passing and rejecting lambdas vs total entries
        //cout << "Passed All Cuts Count: " << passedallcutscount << endl;
        //cout << "Loop Break Counts: " << breakloopcount << endl;
        //cout << "Total Number of Lambda Entries: " << LambdaCand.GetEntries() << endl;
        
        if (passedallcutscount + breakloopcount != LambdaCand.GetEntries()) {
            cout << "Bad Count" << endl;
        }
            eventCounter++;
            if (eventCounter % 1000 == 0) {
                cout << "Processing ODS event " << eventCounter << endl;
            }

    } // loop over events

        cout << "\n" << eventCounter << " events processed." << endl;
        ////////////////////////////////////////////////////////////////////
        //////////////END MAIN LOOP//////////////////////////////////////
        ////////////////////////////////////////////////////////////////////

      
       /////////////////////////////////////////////////////////////////////////////////////////////////////////////
       //Output File just for Lambda TTrees and few odd histograms as sanity checks
       
        TFile treefile(opts.GetOutput(), "RECREATE");
        
        
        for (int a = 0; a < 12; a++) {
            LambdaCutProgression[a]->SetStats(0);
            if (a == 9) { LambdaCutProgression[a]->SetLineColor(kBlack + 1); }
            if (a != 9) { LambdaCutProgression[a]->SetLineColor(kBlack + a); }
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
        
        v0chi2bothhist->SetStats(0);
        v0chi2bothhist->SetLineColor(kBlack);
        v0chi2bothhist->GetXaxis()->SetTitle("Chi2");
        v0chi2bothhist->GetYaxis()->SetTitle("Count");
        v0chi2bothhist->Draw();
        v0chi2bothhist->Write();

        v0chi2reghist->SetStats(0);
        v0chi2reghist->SetLineColor(kBlack);
        v0chi2reghist->GetXaxis()->SetTitle("Chi2");
        v0chi2reghist->GetYaxis()->SetTitle("Count");
        v0chi2reghist->Draw();
        v0chi2reghist->Write();

        v0chi2barhist->SetStats(0);
        v0chi2barhist->SetLineColor(kBlack);
        v0chi2barhist->GetXaxis()->SetTitle("Chi2");
        v0chi2barhist->GetYaxis()->SetTitle("Count");
        v0chi2barhist->Draw();
        v0chi2barhist->Write();

        
       
        hXYDecayVertex->GetXaxis()->SetTitle("X");
        hXYDecayVertex->GetYaxis()->SetTitle("Y");
        hXYDecayVertex->Draw("colz");
        canvas->SetName("XY Decay Vertices");
        canvas->Write();

     
        hXYPrimaryVertex->GetXaxis()->SetTitle("X");
        hXYPrimaryVertex->GetYaxis()->SetTitle("Y");
        hXYPrimaryVertex->Draw("colz");
        canvas->SetName("XY Primary Vertices");
        canvas->Write();
       
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