///////////////////////////////////////////////////////
// K0 short finder on ODS
// 
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
#include "H1Mods/H1PartK0ArrayPtr.h"
#include "H1Mods/H1PartK0.h"
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

    if (vec.size() == 0) { return -1; }

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
    H1Tree::Instance()->Open();            // this statement must be here

    // Histograms Binning, have some histograms filled directly from analysis that I use as a consistency check with the histograms I make from my Analysis code using the TTrees I create here
    Double_t kaonbins = 500;
    Double_t kaonupperbinrange = 1.0;
    Double_t kaonlowerbinrange = 0.0;

   //Hardcode Test
    TH1F* KaonCount = new TH1F("Kaon Count", "Kaon Count", kaonbins, kaonlowerbinrange, kaonupperbinrange);

    /////////////////////////////////////////////////////////////////////////
    //Creating Array of Kaon plots for cut progression

    THStack* hs = new THStack("hs", "Stacked K0 Mass Plots");

    TH1F* KaonCutProgression[16];
    for (int a = 0; a < 16; a++) {
        string kaoncutprogstring = "Kaon Cut " + to_string(a);
        KaonCutProgression[a] = new TH1F(kaoncutprogstring.c_str(), kaoncutprogstring.c_str(), kaonbins, kaonlowerbinrange, kaonupperbinrange);
    }

    /////////////////////////////////////////////////////////////////////////


    // load in some HAT data
    H1FloatPtr q2s("Q2s");//Q2 calcualted from sigma method
    H1FloatPtr ys("Ys"); //Inelasticity calculated from sigma method
    H1FloatPtr ElecTheta("ElecTheta"); //Theta of scattered electron
    H1FloatPtr ElecPhi("ElecPhi");
    H1FloatPtr ElecE("ElecE"); //Energy of scattered electron
    H1FloatPtr EBeamE("EBeamE"); //Energy of electron beam from DMIS 
    H1FloatPtr EBeamP("EBeamP"); //Energy of proton beam from DMIS
    H1FloatPtr w1("Weight1");
    H1FloatPtr w2("Weight2");

    // load in some gen Hat data for migration calculations
    H1FloatPtr q2sgen("Q2sGen");//Q2 calcualted from sigma method
    H1FloatPtr ysgen("YsGen"); //Inelasticity calculated from sigma method

    TH2F* hXYDecayVertex = new TH2F("Decay Vertex", "Decay Vertex", 1000, -80, 80, 1000, -80, 80);
    TH2F* hXYPrimaryVertex = new TH2F("Primary Vertex", "PrimaryVertex", 1000, -5, 5, 1000, -5, 5);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Defining TTrees to store K0 candidates and their associated kinematic variables
    TTree* K0Tree = new TTree("K0", "K0");
    //defining the variables I want to store in Lambda Trees
    double pT, Eta, x, invMasskaon, Q2s, Ys, Angle, weight1, weight2;
    //Defining Tree Branches
    K0Tree->Branch("invMasskaon", &invMasskaon, "invMasskaon/D");
    K0Tree->Branch("pT", &pT, "pT/D");
    K0Tree->Branch("Eta", &Eta, "Eta/D");
    K0Tree->Branch("x", &x, "x/D");
    K0Tree->Branch("Q2s", &Q2s, "Q2s/D");
    K0Tree->Branch("Ys", &Ys, "Ys/D");
    K0Tree->Branch("Angle", &Angle, "Angle/D");
    K0Tree->Branch("weight1", &weight1, "weight1/D");
    K0Tree->Branch("weight2", &weight2, "weight2/D");
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Defining TTrees to store some event information
    TTree* EventTree = new TTree("Events", "Events");
    //defining the variables I want to store in Lambda Trees
    double Q2sGen,YsGen,xGen;
    //Defining Tree Branches
    //K0Tree->Branch("invMasskaon", &invMasskaon, "invMasskaon/D");
    //K0Tree->Branch("pT", &pT, "pT/D");
    //K0Tree->Branch("Eta", &Eta, "Eta/D");
    EventTree->Branch("x", &x, "x/D");
    EventTree->Branch("Q2s", &Q2s, "Q2s/D");
    EventTree->Branch("Ys", &Ys, "Ys/D");
    EventTree->Branch("xGen", &xGen, "xGen/D");
    EventTree->Branch("Q2sGen", &Q2sGen, "Q2sGen/D");
    EventTree->Branch("YsGen", &YsGen, "YsGen/D");
    //K0Tree->Branch("Angle", &Angle, "Angle/D");
    EventTree->Branch("weight1", &weight1, "weight1/D");
    EventTree->Branch("weight2", &weight2, "weight2/D");
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Defining Root Canvas to work on
    TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 800);

    // Pointer to vertices to loop over
    static H1CentralVertexArrayPtr vertex;
    static H1PartK0ArrayPtr K0Cand;
   
    // Get the pion's mass
    Double_t mPion = TDatabasePDG::Instance()->GetParticle(211)->Mass();

    TH1F* RejectedDuplicatePosPionTracks = new TH1F("Candidates Rejected for Duplicate Pos Pion Tracks", "Candidates Rejected for Duplicate Pos Pion Tracks", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* RejectedDuplicateNegPionTracks = new TH1F("Candidates Rejected for Duplicate Neg Pion Tracks", "Candidates Rejected for Duplicate Neg Pion Tracks", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* RejectedDuplicateTotalTracks = new TH1F("Candidates Rejected for Duplicate Tracks", "Candidates Rejected for Duplicate Tracks", kaonbins, kaonlowerbinrange, kaonupperbinrange);
   
    TH1F* RejectedRadTrack = new TH1F("Candidates Rejected for CJC1 Requirement", "Candidates Rejected for CJC1 Requirement", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* RejectedTrackpT = new TH1F("Candidates Rejected for Track pT", "Candidates Rejected for Track pT", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* RejectedVertexFitted = new TH1F("Candidates Rejected for Vertex fitted", "Candidates Rejected for vertex fitted", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* RejectedNHit = new TH1F("Candidates Rejected for CJC1 NHit", "Candidates Rejected for CJC1 NHit", kaonbins, kaonlowerbinrange, kaonupperbinrange);
    TH1F* RejectedH1Track = new TH1F("Candidates Rejected for Duplicate H1 Tracks", "Candidates Rejected for Duplicate H1 Tracks", kaonbins, kaonlowerbinrange, kaonupperbinrange);


    ///////////////BEGIN MAIN LOOP////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    // Loop as long as there's data
    while (H1Tree::Instance()->Next() && !opts.IsMaxEvent(eventCounter)) {

        //Initializing items to help with consistency checks
        int breakloopcount = 0;
        int passedinitialcutscount = 0;
        int passedallcutscount = 0;
        int particlesinevent = 0;

        //Storing pT values of tracks, these will only be exactly identical if the same track is used twice
        vector<double> PosPionTrackCheck = {}; //pT values of proton tracks of lambda candidates will be stored here
        vector<double> NegPionTrackCheck = {}; //pT values of pion tracks of lambda candidates will be stored here
        
        //Storing lambda candidate loop value to later access H1Track class members of tracks
        vector<Int_t> PosPionH1TrackCheck = {};
        vector<Int_t> NegPionH1TrackCheck = {};

        // Loop over all vertices found in the event
        for (Int_t loop = 0; loop < K0Cand.GetEntries(); loop++) {

        //ensuring there are two stored daughters for particle candidate
        if (K0Cand[loop]->GetNumOfParticles() < 2) { 
            breakloopcount++;
            continue; 
        }

        //Getting invariant mass
        invMasskaon = (K0Cand[loop]->GetPosPionFourVector()
            + K0Cand[loop]->GetNegPionFourVector()).M();

        //Getting Vectors for Armenteros-Podolansky Plot
        H1Track* PosPionH1Track = (H1Track*)K0Cand[loop]->GetPosPionTrack();//This is casting H1CentralFittedTrack h1oo class as H1Track for daughter
        H1Track* NegPionH1Track = (H1Track*)K0Cand[loop]->GetNegPionTrack();//This is casting H1CentralFittedTrack h1oo class as H1Track for daughter


        TVector3 vecPosPion = K0Cand[loop]->GetPosPionFourVector().Vect(); //Pos pion 3 vector
        TVector3 vecNegPion = K0Cand[loop]->GetNegPionFourVector().Vect(); //Neg pion 3 vector
        TVector3 vecK0 = vecPosPion + vecNegPion; //K0 short 3 vector

        
        
        //Call H1PartK0 class data that references H1CentralFittedTrack for Pion and Proton
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

        //Primary Vertex Info
        H1SelVertex* PosPionVertex = (H1SelVertex*)K0Cand[loop]->GetPosPionTrack();
        const H1SelVertex* PosPionPrimaryVertex = PosPionVertex->GetPrimaryVertex();
        TVector3 pv = PosPionPrimaryVertex->GetPosition();//is same for both pions, so just using pos one here and previous couple lines
        TVector2 V2PV(pv.X(), pv.Y());

        //Decay Vertex Info
        const H1ReconstructedVertex* DecayVertex = v0->GetDecayVertex();
        TVector3 sv = DecayVertex->GetPosition();//is same for both pions, so just using pos one here and previous couple lines

        //Get the vector pointing from primary vertex to secondary vertex
        TVector3 PVtoSV = sv - pv;
        TVector3 KaonP = K0Cand[loop]->GetFourVector().Vect();
        double angle = (180 / (TMath::Pi()))*acos((PVtoSV.Dot(KaonP))/((PVtoSV.Mag())*(KaonP.Mag())));

        ///////////////////////////////////////////////////////////////////////////////
        //Defining pT before cuts and putting stuff into pt bins
        
        pT = v0->GetPt();
        Eta = K0Cand[loop]->GetFourVector().Vect().Eta();
        //Raw K0 Candidate data after our event selection defined in create_evls.C
        KaonCutProgression[0]->Fill(invMasskaon);
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //I want to first implement the cuts that have an intentional impact on the particle signal
        //ie: Eta Cut, pT Cut, Redundant Track Cut, 
        
        //Eta Cut
        if (abs(Eta) > 1.3) {
            breakloopcount++;
            continue;
        }

        KaonCutProgression[1]->Fill(invMasskaon);

        //pT Cut
        if (pT < 0.5 || pT > 3.5) {
            breakloopcount++;
            continue;
        }

        KaonCutProgression[2]->Fill(invMasskaon);

        //Need to Apply some further cuts on the tracks of the Lambda daughters
        //Cut on distance maximal radial distance of first CJC hit
        //requiring this to be < 35 cm will ensure our tracks are measureed in the CJC1 first
        //If either track fails this requirement we reject the candidate
        if (PosPionH1Track->GetStartRadius() > 35 || NegPionH1Track->GetStartRadius() > 35) {
            breakloopcount++;
            RejectedRadTrack->Fill(invMasskaon);
            continue;
        }

        KaonCutProgression[3]->Fill(invMasskaon);

        //Reject tracks that are primary vertex fitted with H1PartSelTrack
        if (PosPionPSTrack->IsFromSecondary() != true || NegPionPSTrack->IsFromSecondary() != true) {
            breakloopcount++;
            RejectedVertexFitted->Fill(invMasskaon);
            continue;
        }
        KaonCutProgression[4]->Fill(invMasskaon);

        //Check for duplicate tracks with H1Track class
        int H1TrackFlag = 0;
        for (Int_t a = 0; a < PosPionH1TrackCheck.size(); a++) {

            //Have to reget H1Track of previously selected candidate to compare with the current one
            H1Track* PosPionH1TrackPrevious = (H1Track*)K0Cand[PosPionH1TrackCheck[a]]->GetPosPionTrack(); //This is casting H1CentralFittedTrack h1oo class as H1Track for daughter
            H1Track* NegPionH1TrackPrevious = (H1Track*)K0Cand[NegPionH1TrackCheck[a]]->GetNegPionTrack(); //This is casting H1CentralFittedTrack h1oo class as H1Track for daughter

            //H1Track &ProtonH1TrackPreviousnonpointer = ProtonH1TrackPrevious;
            //H1Track &PionH1TrackPreviousnonpointer = PionH1TrackPrevious;

            if (PosPionH1Track->IsSameAs(*PosPionH1TrackPrevious) == true) { H1TrackFlag = 1; }
            if (NegPionH1Track->IsSameAs(*NegPionH1TrackPrevious) == true) { H1TrackFlag = 1; }

        }

        if (H1TrackFlag == 1) {
            breakloopcount++;
            RejectedH1Track->Fill(invMasskaon);
            continue;
        }
        KaonCutProgression[5]->Fill(invMasskaon);

   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //pT Track cut on both proton and pion track
        if (K0Cand[loop]->GetPosPionPt() < 0.12 || K0Cand[loop]->GetNegPionPt() < 0.12) {
            breakloopcount++;
            RejectedTrackpT->Fill(invMasskaon);
            continue;
        }
        KaonCutProgression[6]->Fill(invMasskaon);
   ////////////////////////////////////////////////////////////////////////////////////////////////
        //PID Likelihood Cut for Pions < 0.001
        if (PosPionPSTrack->GetDedxLikelihood(H1Dedx::kPion) < 0.001 || NegPionPSTrack->GetDedxLikelihood(H1Dedx::kPion) < 0.001) { 
            breakloopcount++;
            continue; 
        }
     
        KaonCutProgression[7]->Fill(invMasskaon);
        
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //DCA over dDCA proton cut
        if (abs((pospionTrajectory->GetDca(V2PV)) / (pospionTrajectory->GetdDca(V2PV))) < 1 || abs((negpionTrajectory->GetDca(V2PV)) / (negpionTrajectory->GetdDca(V2PV))) < 1) { 
            breakloopcount++; 
            continue;
        }

        KaonCutProgression[8]->Fill(invMasskaon);


        //define Tree information
        Eta = K0Cand[loop]->GetFourVector().Vect().Eta();
        //x = (*q2e) / ((*ye) * (*ebeamE) * (*ebeamP) * 4); //Bkorken X calculated from electron method
        x = ((*ElecE) / (*EBeamP)) * (cos((*ElecTheta) / (2))) * (cos((*ElecTheta) / (2))) / (*ys); //Bjorken x calculated from Sigma Method, ISR independent
        Q2s = *q2s;
        Ys = *ys;
        Angle = angle;
        weight1 = *w1;
        weight2 = *w2;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        //Eta Cut
//        if (abs(Eta) > 1.3) {
//            breakloopcount++;
//            continue;
//        }
//
//        KaonCutProgression[3]->Fill(invMasskaon);
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        //pT Cut
//        if (pT < 0.5 || pT > 3.5) {
//            breakloopcount++;
//            continue;
//        }
//
//        KaonCutProgression[4]->Fill(invMasskaon);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (PosPionH1Track->GetNHit(H1Track::tdCJC1) < 1 || NegPionH1Track->GetNHit(H1Track::tdCJC1) < 1) {
            breakloopcount++;
            RejectedNHit->Fill(invMasskaon);
            continue;
        }
        KaonCutProgression[9]->Fill(invMasskaon);

        PosPionH1TrackCheck.push_back(loop);
        NegPionH1TrackCheck.push_back(loop);

        //Cut on Redundant Pion tracks being used 
                 
        //We passed all of the initial cuts, now we have to check if we have already used the proton track
        int pospiontrackcheck = singlefind(PosPionTrackCheck, 0, PosPionTrackCheck.size(), K0Cand[loop]->GetPosPion()->GetPt());
        int negpiontrackcheck = singlefind(NegPionTrackCheck, 0, NegPionTrackCheck.size(), K0Cand[loop]->GetNegPion()->GetPt());

        //if both of these singlefindfunctions return -1, then these tracks have not been used
        // in the case these are the first particle accepted and the reference list is empty, it will also retun -1
        //if one of them gets a valus of -1 we should skip that event, because it is reusing a track
        particlesinevent++;

        ///HERE
       /* if (particlesinevent > 1) {
            cout << "milti particle event" << endl;
        }*/


        if (pospiontrackcheck != -1) {
            //cout << "Redundant proton track" << endl;
            RejectedDuplicateTotalTracks->Fill(invMasskaon);
            RejectedDuplicatePosPionTracks->Fill(invMasskaon);
        }
        if (negpiontrackcheck != -1) {
            //cout << "Redundant pion track" << endl; 
            RejectedDuplicateTotalTracks->Fill(invMasskaon);
            RejectedDuplicateNegPionTracks->Fill(invMasskaon);
        }
        if (pospiontrackcheck != -1 || negpiontrackcheck != -1) {
            breakloopcount++;
            continue;
        }



        KaonCutProgression[10]->Fill(invMasskaon);

        //now the particles have passed all checks, we need to update the track reference list
        PosPionTrackCheck.push_back(K0Cand[loop]->GetPosPion()->GetPt());
        NegPionTrackCheck.push_back(K0Cand[loop]->GetNegPion()->GetPt());




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



        if (pT > 0.5 && pT < 3.5) {
            if (abs(Eta) < 1.3) {
                if (Ys > 0.1 && Ys < 0.6) {
                    if (Q2s > 5 && Q2s < 100) {

                        hXYDecayVertex->Fill(sv.X(), sv.Y());
                        hXYPrimaryVertex->Fill(pv.X(), pv.Y());

                    }
                }
            }
        }
        
        //Hardcode a quick consistency for getting histogram counts
        if (pT > 0.5 && pT < 3.5) {
            if (abs(Eta) < 1.3) {
              
                KaonCount->Fill(invMasskaon);
              
            }
        }

        //
        passedallcutscount++;
        K0Tree->Fill();


        } // loop over K0 Candidate array

        if (passedallcutscount + breakloopcount != K0Cand.GetEntries()) {
            cout << "Bad Count" << endl;
        }
            eventCounter++;
            if (eventCounter % 1000 == 0) {
                cout << "Processing ODS event " << eventCounter << endl;

            }
    }//loop over events

        cout << "\n" << eventCounter << " events processed." << endl;
        ////////////////////////////////////////////////////////////////////
        //////////////END MAIN LOOP//////////////////////////////////////
        ////////////////////////////////////////////////////////////////////

       
 
       TFile treefile(opts.GetOutput(), "RECREATE");
       
       
       for (int a = 0; a < 16; a++) {
           //KaonCutProgression[a]->SetStats(0);
           if (a >= 0 && a <= 8) { KaonCutProgression[a]->SetLineColor(kBlack + a); }
           if (a > 8) { KaonCutProgression[a]->SetLineColor(kBlack + a - 9); }
           //KaonCutProgression[a]->SetLineColor(kBlack + a);
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

       //RejectedDuplicatePosPionTracks->SetStats(0);
       RejectedDuplicatePosPionTracks->SetLineColor(kBlack);
       RejectedDuplicatePosPionTracks->GetXaxis()->SetTitle("Mass [GeV]");
       RejectedDuplicatePosPionTracks->GetYaxis()->SetTitle("Count");
       RejectedDuplicatePosPionTracks->Draw();
       RejectedDuplicatePosPionTracks->Write();

       //RejectedDuplicatePionTracks->SetStats(0);
       RejectedDuplicateNegPionTracks->SetLineColor(kBlack);
       RejectedDuplicateNegPionTracks->GetXaxis()->SetTitle("Mass [GeV]");
       RejectedDuplicateNegPionTracks->GetYaxis()->SetTitle("Count");
       RejectedDuplicateNegPionTracks->Draw();
       RejectedDuplicateNegPionTracks->Write();

       //RejectedDuplicateTotalTracks->SetStats(0);
       RejectedDuplicateTotalTracks->SetLineColor(kBlack);
       RejectedDuplicateTotalTracks->GetXaxis()->SetTitle("Mass [GeV]");
       RejectedDuplicateTotalTracks->GetYaxis()->SetTitle("Count");
       RejectedDuplicateTotalTracks->Draw();
       RejectedDuplicateTotalTracks->Write();
       RejectedRadTrack->SetLineColor(kBlack);
       RejectedRadTrack->GetXaxis()->SetTitle("Mass [GeV]");
       RejectedRadTrack->GetYaxis()->SetTitle("Count");
       RejectedRadTrack->Draw();
       RejectedRadTrack->Write();

       RejectedTrackpT->SetLineColor(kBlack);
       RejectedTrackpT->GetXaxis()->SetTitle("Mass [GeV]");
       RejectedTrackpT->GetYaxis()->SetTitle("Count");
       RejectedTrackpT->Draw();
       RejectedTrackpT->Write();

       RejectedVertexFitted->SetLineColor(kBlack);
       RejectedVertexFitted->GetXaxis()->SetTitle("Mass [GeV]");
       RejectedVertexFitted->GetYaxis()->SetTitle("Count");
       RejectedVertexFitted->Draw();
       RejectedVertexFitted->Write();

       RejectedNHit->SetLineColor(kBlack);
       RejectedNHit->GetXaxis()->SetTitle("Mass [GeV]");
       RejectedNHit->GetYaxis()->SetTitle("Count");
       RejectedNHit->Draw();
       RejectedNHit->Write();

       //RejectedDuplicateProtonTracks->SetStats(0);
       RejectedDuplicatePosPionTracks->SetLineColor(kBlack);
       RejectedDuplicatePosPionTracks->GetXaxis()->SetTitle("Mass [GeV]");
       RejectedDuplicatePosPionTracks->GetYaxis()->SetTitle("Count");
       RejectedDuplicatePosPionTracks->Draw();
       RejectedDuplicatePosPionTracks->Write();

       //RejectedDuplicatePionTracks->SetStats(0);
       RejectedDuplicateNegPionTracks->SetLineColor(kBlack);
       RejectedDuplicateNegPionTracks->GetXaxis()->SetTitle("Mass [GeV]");
       RejectedDuplicateNegPionTracks->GetYaxis()->SetTitle("Count");
       RejectedDuplicateNegPionTracks->Draw();
       RejectedDuplicateNegPionTracks->Write();

       //RejectedDuplicateTotalTracks->SetStats(0);
       RejectedDuplicateTotalTracks->SetLineColor(kBlack);
       RejectedDuplicateTotalTracks->GetXaxis()->SetTitle("Mass [GeV]");
       RejectedDuplicateTotalTracks->GetYaxis()->SetTitle("Count");
       RejectedDuplicateTotalTracks->Draw();
       RejectedDuplicateTotalTracks->Write();

       //RejectedDuplicateTotalTracks->SetStats(0);
       RejectedH1Track->SetLineColor(kBlack);
       RejectedH1Track->GetXaxis()->SetTitle("Mass [GeV]");
       RejectedH1Track->GetYaxis()->SetTitle("Count");
       RejectedH1Track->Draw();
       RejectedH1Track->Write();

       //Write all TTrees 
       K0Tree->Write();
       //Write and close TTree file
       treefile.Write();
       treefile.Close();

       cout << "Histograms written to " << opts.GetOutput() << endl;


        return 0;
    }