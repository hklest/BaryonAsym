///////////////////////////////////////////////////////
// 
// Lambda Finder 
// Additional Selections building on H1FindLambda.C selections
// 
// Author     : gtustin
// 
///////////////////////////////////////////////////////

//General Includes
#include <iostream>
#include <stdlib.h>
#include <cmath>

//ROOT includes
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

//h1oo includes
#include "H1Pointers/H1FloatPtr.h"
#include "H1Pointers/H1IntPtr.h"
#include "H1Skeleton/H1EventList.h"
#include "H1Skeleton/H1FpackIndexManager.h"
#include "H1Skeleton/H1Tree.h"
#include "H1Geom/H1DBManager.h"
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
#include "H1Mods/H1PartMC.h"
#include "H1Mods/H1PartLambdaArrayPtr.h"
#include "H1Mods/H1PartLambda.h"
#include "H1Mods/H1SelVertex.h"
#include "H1Tracks/H1ReconstructedVertex.h"
#include "H1Tracks/H1CentralVertex.h"
#include "H1Tracks/H1CentralVertexArrayPtr.h"
#include "H1Steering/H1StdCmdLine.h"
#include "H1OOBanks/ODS/H1Str.h"
#include "H1OOBanks/ODS/H1StrBankPtr.h"
#include "H1OOBanks/ODS/H1Svx.h"
#include "H1Calculator/H1CalcGenericInterface.h"
#include "H1Tools/H1LumiCalculator.h"
#include "H1Calculator/H1Calculator.h"
#include "H1Calculator/H1CalcTrig.h"
#include "H1Calculator/H1CalcWeight.h"
#include "H1Calculator/H1CalcVertex.h"
#include "H1Calculator/H1CalcEvent.h"
#include "H1Calculator/H1CalcKine.h"
#include "H1Calculator/H1CalcElec.h"
#include "H1Calculator/H1CalcFs.h"
#include "H1Calculator/H1CalcHad.h"
#include "H1Calculator/H1CalcTrack.h"
#include "H1Calculator/H1CalcBgTiming.h"
#include "H1Calculator/H1CalcSystematic.h"
#include "H1PhysUtils/H1NuclIACor.h"

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



//This function is used to check for BG particles and returns 1 if BG is TRUE and 0 if the particle is not BG
int BGcheck(H1PartMC* part, H1PartMCArrayPtr gtrdata, H1StrBankPtr strbank) {

    //Counting feed down use this condition
    //we don't want to exlude ALL non Gtr particles since there is a significant source of Lambdas coming from feed down decays are only allowed to decay at the STR level by H1 convention
    //So if the particle is STR, we requie its vertex type be a decay vertex
    if (part->IsGtr() == false) {
        if (strbank[part->GetStrIdx()]->GetSvx()->GetIvtype() != 2) { return 1; } //SVX types should be 2, 2=decay vertex
    }
    //decays should only have 1 Mother, so GetMother2() should equal -1, if not, then skip
    if (part->GetMother2() != -1) { return 1; }

    //Now, we have to make sure that if the particle is STR, that it's mother particle IS Gtr to avoid lambdas coming from decays of particles produced in secondary interactions
    //If the mother particle is STR, then again it must only be a decay vertex
    if (part->IsGtr() == false) { // at this point, only decay Svx types are allowed
        H1PartMC* partMother = gtrdata[part->GetMother1()];
        if (partMother->IsGtr() == false) {
            if (strbank[partMother->GetStrIdx()]->GetSvx()->GetIvtype() != 2) { return 1; } //Svx types should be 2, 2=decay vertex
        }
    }

    //We also have to check the grandmother
    //Stable H1 particles by convention are: K0s, Lambda, Sigma+, Sigma-, Xi-, Xi0, Omega-
    //There is a 2 generation case in the branching ratios of "stable" particles such as Omega->Xi->Lambda
    //So grandmothers must all absolutely be Gtr
    if (part->IsGtr() == false) { // at this point, only decay Svx types are allowed
        H1PartMC* partMother = gtrdata[part->GetMother1()];
        H1PartMC* partGrandMother = gtrdata[partMother->GetMother1()];
        if (partGrandMother->IsGtr() == false) { return 1; }
    }

    //If all the previous checks are passed then we can return 0, this partilce should be counted for the detector efficiency
    return 0;

}


int
main(int argc, char* argv[])
{
    
    // event counter
    Int_t eventCounter = 0;
    // event num of lambdas
    Int_t LambdaCount = 0;
    // number of event with multiple lambda candidates
    Int_t eventswithMultipleLambdas = 0;
    Int_t eventswithOneLambda = 0;
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

    Double_t lambdaCbins = 50;
    Double_t lambdaCupperbinrange = 3.317;
    Double_t lambdaClowerbinrange = 2.017;
    
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
    H1FloatPtr w1("Weight1");
    H1FloatPtr w2("Weight2");
    H1IntPtr RunType("RunType"); //Run type, 0=data, else is not data
    //MC Pointers
    H1FloatPtr q2sgen("Q2sGen");
    H1FloatPtr ysgen("YsGen");
    H1FloatPtr GenPzHad("GenPzHad");
    H1FloatPtr GenEnHad("GenEnHad");
    H1FloatPtr GenPhElec("GenPhElec");
    H1FloatPtr GenEnElec("GenEnElec"); //Energy of scattered electron combined with photon energy for FSR
    H1FloatPtr GenThElec("GenThElec"); //Theta of scattered electron combined with photon for FSR

    //Armenteros-Podolansky plot for charmed lambdas
    TH2F* hAPbeforeC = new TH2F("Charm AP", "Charm Armenteros-Podolansky Plot Before pT_rel Cut", 200, -1, 1, 500, 0, 1);
    TH2F* hAPafterC = new TH2F("Charm AP", "Charm Armenteros-Podolansky Plot Before pT_rel Cut", 200, -1, 1, 500, 0, 1);

    TH2D* hdEdxPionCBefore = new TH2D("LambdaC Pion dEdxvP Before", "LambdaC Pion dEdxvP Before", 500, 0, 2, 500, 0, 10);
    TH2D* hdEdxPionCAfter = new TH2D("LambdaC Pion dEdxvP After", "LambdaC Pion dEdxvP After", 500, 0, 2, 500, 0, 10);
    //Other LambdaC plots
    TH2F* pXYvtxdensitytot = new TH2F("Total Lambda XY Vertices", "Total Lambda XY Vertices", 200, -10, 10, 200, -10, 10);
    TH2F* pZXvtxdensitytot = new TH2F("Total Lambda ZX Vertices", "Total Lambda ZX Vertices", 500, -40, 40, 200, -10, 10);
    TH2F* pZYvtxdensitytot = new TH2F("Total Lambda ZY Vertices", "Total Lambda ZY Vertices", 500, -40, 40, 200, -10, 10);
    TH1F* pdecaylengthmag = new TH1F("Lambda Decay Length Magnitude", "Lambda Decay Length Magnitude", 200, 0, 20);
    //

    TH2F* hXYDecayVertex = new TH2F("Decay Vertex", "Decay Vertex", 1000, -80, 80, 1000, -80, 80);
    TH2F* hXYPrimaryVertex = new TH2F("Primary Vertex", "PrimaryVertex", 1000, -5, 5, 1000, -5, 5);
    
    //Defining TTrees to store Lambda, Lambda Bar, and Combined Lambda candidates and their associated kinematic variables
    TTree* lambdaRegTree = new TTree("Lambda Reg", "Lambda Reg");
    TTree* lambdaBarTree = new TTree("Lambda Bar", "Lambda Bar");
    TTree* lambdaTotTree = new TTree("Lambda Tot", "Lambda Tot");

    //TTree* lambdaTotPionDaughterTree = new TTree("Lambda Tot Pion Daughters", "Lambda Tot Pion Daughters");
    //TTree* lambdaTotProtonDaughterTree = new TTree("Lambda Tot Proton Daughters", "Lambda Tot Proton Daughters");

    TTree* lambdaCTree = new TTree("LambdaC", "LambdaC");
    double invMasslambdaC;
    lambdaCTree->Branch("invMasslambdaC", &invMasslambdaC, "invMasslambdaC/D");

    TTree* lambdaRegGenTree = new TTree("Lambda Reg Gen", "Lambda Reg Gen");
    TTree* lambdaBarGenTree = new TTree("Lambda Bar Gen", "Lambda Bar Gen");
    TTree* lambdaTotGenTree = new TTree("Lambda Tot Gen", "Lambda Tot Gen");

    TTree* lambdaRegBGTree = new TTree("Lambda Reg BG", "Lambda Reg BG");
    TTree* lambdaBarBGTree = new TTree("Lambda Bar BG", "Lambda Bar BG");
    TTree* lambdaTotBGTree = new TTree("Lambda Tot BG", "Lambda Tot BG");
    //defining the variables I want to store in Lambda Trees, will save the TTrees and access branches later in a separate analysis code
    double pT, Eta, x, invMasslambda, Q2s, Ys, Angle, v0chi2, weight1, weight2, SVx, SVy, SVz, PVx, PVy, PVz, LambdaPX, LambdaPY, LambdaPZ, ystar, ylab, scatEX, scatEY, scatEZ;
    double PionPt, ProtonPt, PionTrackStartRadius, ProtonTrackStartRadius, PionL, PionLProton, PionLelectron, ProtonL, ProtonLPion, ProtonLelectron, PionCharge, ProtonCharge, PionTrackLength, ProtonTrackLength, CosThetaStar;
    double PionNHitsdEdx, ProtonNHitsdEdx, PiondEdx, ProtondEdx, invMasskaonCont, invMassphotonCont;
    double PionDCA, ProtonDCA, PionSDCA, ProtonSDCA, Chi2, MassPionPion, MassElecElec, pTrelPion, pTrelProton, RadiusSVtoPV, DecayLength, DeltaDCA;
    //defining variables needed only for MC Lambdas
    double genIvxX, genIvxY, genIvxZ, decayrecX, decayrecY, decayrecZ, strIVtype, pionFRUNZ, protonFRUNZ, Ws, Phi;
    double pTgen, Etagen, xgen, Q2sgen, Ysgen, SVxgen, SVygen, SVzgen, PVzgen, Phigen, genrecAngle;
    int IsBar, FoundGen, s61, s74, s1, s75, s67;
    Bool_t fRunType;
    double PionNuclCorrWeight;
    double ProtonTrackEndRadius, PionTrackEndRadius;
    TVector3  ProtonTrackEndMomentum, ProtonTrackV0Momentum, PionTrackEndMomentum, PionTrackV0Momentum;
    //TVector3* ProtonTrackEndMomentum = nullptr;
    //Default PionNuclCorrWeight to 1.0, that way the data correction is always 1
    PionNuclCorrWeight = 1.0;

    //Defining Tree Branches for Combined Lambdas, and seperate ones for Regular Lambdas vs Lambda Bars
    lambdaRegTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaRegTree->Branch("invMasskaonCont", &invMasskaonCont, "invMasskaonCont/D");
    lambdaRegTree->Branch("invMassphotonCont", &invMassphotonCont, "invMassphotonCont/D");
    lambdaRegTree->Branch("pT",&pT,"pT/D");
    lambdaRegTree->Branch("Eta", &Eta, "Eta/D");
    lambdaRegTree->Branch("Phi", &Phi, "Phi/D");
    lambdaRegTree->Branch("x", &x, "x/D");
    lambdaRegTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaRegTree->Branch("Ys", &Ys, "Ys/D");
    lambdaRegTree->Branch("Angle", &Angle, "Angle/D");
    lambdaRegTree->Branch("v0chi2", &v0chi2, "v0chi2/D");
    lambdaRegTree->Branch("weight1", &weight1, "weight1/D");
    lambdaRegTree->Branch("weight2", &weight2, "weight2/D");
    lambdaRegTree->Branch("SVx", &SVx, "SVx/D");
    lambdaRegTree->Branch("SVy", &SVy, "SVy/D");
    lambdaRegTree->Branch("SVz", &SVz, "SVz/D");
    lambdaRegTree->Branch("PVx", &PVx, "PVx/D");
    lambdaRegTree->Branch("PVy", &PVy, "PVy/D");
    lambdaRegTree->Branch("PVz", &PVz, "PVz/D");
    lambdaRegTree->Branch("LambdaPX", &LambdaPX, "LambdaPX/D");
    lambdaRegTree->Branch("LambdaPY", &LambdaPY, "LambdaPY/D");
    lambdaRegTree->Branch("LambdaPZ", &LambdaPZ, "LambdaPZ/D");
    lambdaRegTree->Branch("fRunType", &fRunType, "fRunType/O");
    lambdaRegTree->Branch("CosThetaStar", &CosThetaStar, "CosThetaStar/D");
    lambdaRegTree->Branch("ystar", &ystar, "ystar/D");
    lambdaRegTree->Branch("ylab", &ylab, "ylab/D");
    lambdaRegTree->Branch("scatEX", &scatEX, "scatEX/D");
    lambdaRegTree->Branch("scatEY", &scatEY, "scatEY/D");
    lambdaRegTree->Branch("scatEZ", &scatEZ, "scatEZ/D");
    lambdaRegTree->Branch("FoundGen", &FoundGen, "FoundGen/I");
    lambdaRegTree->Branch("s61", &s61, "s61/I");
    lambdaRegTree->Branch("s1", &s1, "s1/I");
    lambdaRegTree->Branch("s74", &s74, "s74/I");
    lambdaRegTree->Branch("s75", &s75, "s75/I");
    lambdaRegTree->Branch("s67", &s67, "s67/I");
    //Below is unique braches to the LambdaTotTree compared to Reg and Bar Trees
    lambdaRegTree->Branch("PionCharge", &PionCharge, "PionCharge/D");
    lambdaRegTree->Branch("ProtonCharge", &ProtonCharge, "ProtonCharge/D");
    //Candidate Control Variables
    lambdaRegTree->Branch("pionFRUNZ", &pionFRUNZ, "pionFRUNZ/D");
    lambdaRegTree->Branch("protonFRUNZ", &protonFRUNZ, "protonFRUNZ/D");
    lambdaRegTree->Branch("PionPt", &PionPt, "PionPt/D");
    lambdaRegTree->Branch("ProtonPt", &ProtonPt, "ProtonPt/D");
    lambdaRegTree->Branch("PionTrackStartRadius", &PionTrackStartRadius, "PionTrackStartRadius/D");
    lambdaRegTree->Branch("ProtonTrackStartRadius", &ProtonTrackStartRadius, "ProtonTrackStartRadius/D");
    lambdaRegTree->Branch("PionNHitsdEdx", &PionNHitsdEdx, "PionNHitsdEdx/D");
    lambdaRegTree->Branch("ProtonNHitsdEdx", &ProtonNHitsdEdx, "ProtonNHitsdEdx/D");
    lambdaRegTree->Branch("PiondEdx", &PiondEdx, "PiondEdx/D");
    lambdaRegTree->Branch("ProtondEdx", &ProtondEdx, "ProtondEdx/D");
    lambdaRegTree->Branch("PionL", &PionL, "PionL/D");
    lambdaRegTree->Branch("PionLProton", &PionLProton, "PionLProton/D");
    lambdaRegTree->Branch("PionLelectron", &PionLelectron, "PionLelectron/D");
    lambdaRegTree->Branch("ProtonL", &ProtonL, "ProtonL/D");
    lambdaRegTree->Branch("ProtonLPion", &ProtonLPion, "ProtonLPion/D");
    lambdaRegTree->Branch("ProtonLelectron", &ProtonLelectron, "ProtonLelectron/D");
    lambdaRegTree->Branch("PionTrackLength", &PionTrackLength, "PionTrackLength/D");
    lambdaRegTree->Branch("ProtonTrackLength", &ProtonTrackLength, "ProtonTrackLength/D");
    lambdaRegTree->Branch("PionDCA", &PionDCA, "PionDCA/D");
    lambdaRegTree->Branch("ProtonDCA", &ProtonDCA, "ProtonDCA/D");
    lambdaRegTree->Branch("PionSDCA", &PionSDCA, "PionSDCA/D");
    lambdaRegTree->Branch("ProtonSDCA", &ProtonSDCA, "ProtonSDCA/D");
    lambdaRegTree->Branch("Chi2", &Chi2, "Chi2/D");
    //lambdaTotTree->Branch("Chi2Proton", &Chi2Proton, "Chi2Proton/D");
    lambdaRegTree->Branch("MassPionPion", &MassPionPion, "MassPionPion/D");
    lambdaRegTree->Branch("MassElecElec", &MassElecElec, "MassElecElec/D");
    lambdaRegTree->Branch("pTrelPion", &pTrelPion, "pTrelPion/D");
    lambdaRegTree->Branch("pTrelProton", &pTrelProton, "pTrelProton/D");
    lambdaRegTree->Branch("RadiusSVtoPV", &RadiusSVtoPV, "RadiusSVtoPV/D");
    lambdaRegTree->Branch("IsBar", &IsBar, "IsBar/I");
    lambdaRegTree->Branch("PionNuclCorrWeight", &PionNuclCorrWeight, "PionNuclCorrWeight/D");
    lambdaRegTree->Branch("ProtonTrackEndRadius", &ProtonTrackEndRadius, "ProtonTrackEndRadius/D");
    lambdaRegTree->Branch("ProtonTrackEndMomentum", "TVector3", &ProtonTrackEndMomentum);
    lambdaRegTree->Branch("PionTrackEndRadius", &PionTrackEndRadius, "PionTrackEndRadius/D");
    lambdaRegTree->Branch("PionTrackEndMomentum","TVector3", &PionTrackEndMomentum);
    lambdaRegTree->Branch("ProtonTrackV0Momentum", "TVector3", &ProtonTrackV0Momentum);
    lambdaRegTree->Branch("PionTrackV0Momentum", "TVector3", &PionTrackV0Momentum);
    lambdaRegTree->Branch("DecayLength", &DecayLength, "DecayLength/D");
    lambdaRegTree->Branch("DeltaDCA", &DeltaDCA, "DeltaDCA/D");

    lambdaBarTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaBarTree->Branch("pT", &pT, "pT/D");
    lambdaBarTree->Branch("Eta", &Eta, "Eta/D");
    lambdaBarTree->Branch("Phi", &Phi, "Phi/D");
    lambdaBarTree->Branch("x", &x, "x/D");
    lambdaBarTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaBarTree->Branch("Ys", &Ys, "Ys/D");
    lambdaBarTree->Branch("Angle", &Angle, "Angle/D");
    lambdaBarTree->Branch("v0chi2", &v0chi2, "v0chi2/D");
    lambdaBarTree->Branch("weight1", &weight1, "weight1/D");
    lambdaBarTree->Branch("weight2", &weight2, "weight2/D");
    lambdaBarTree->Branch("SVx", &SVx, "SVx/D");
    lambdaBarTree->Branch("SVy", &SVy, "SVy/D");
    lambdaBarTree->Branch("SVz", &SVz, "SVz/D");
    lambdaBarTree->Branch("PVx", &PVx, "PVx/D");
    lambdaBarTree->Branch("PVy", &PVy, "PVy/D");
    lambdaBarTree->Branch("PVz", &PVz, "PVz/D");
    lambdaBarTree->Branch("LambdaPX", &LambdaPX, "LambdaPX/D");
    lambdaBarTree->Branch("LambdaPY", &LambdaPY, "LambdaPY/D");
    lambdaBarTree->Branch("LambdaPZ", &LambdaPZ, "LambdaPZ/D");
    lambdaBarTree->Branch("fRunType", &fRunType, "fRunType/O");
    lambdaBarTree->Branch("CosThetaStar", &CosThetaStar, "CosThetaStar/D");
    lambdaBarTree->Branch("ystar", &ystar, "ystar/D");
    lambdaBarTree->Branch("ylab", &ylab, "ylab/D");
    lambdaBarTree->Branch("scatEX", &scatEX, "scatEX/D");
    lambdaBarTree->Branch("scatEY", &scatEY, "scatEY/D");
    lambdaBarTree->Branch("scatEZ", &scatEZ, "scatEZ/D");
    lambdaBarTree->Branch("FoundGen", &FoundGen, "FoundGen/I");
    lambdaBarTree->Branch("s61", &s61, "s61/I");
    lambdaBarTree->Branch("s1", &s1, "s1/I");
    lambdaBarTree->Branch("s74", &s74, "s74/I");
    lambdaBarTree->Branch("s75", &s75, "s75/I");
    lambdaBarTree->Branch("s67", &s67, "s67/I");
    //Below is unique braches to the LambdaTotTree compared to Reg and Bar Trees
    lambdaBarTree->Branch("PionCharge", &PionCharge, "PionCharge/D");
    lambdaBarTree->Branch("ProtonCharge", &ProtonCharge, "ProtonCharge/D");
    //Candidate Control Variables
    lambdaBarTree->Branch("pionFRUNZ", &pionFRUNZ, "pionFRUNZ/D");
    lambdaBarTree->Branch("protonFRUNZ", &protonFRUNZ, "protonFRUNZ/D");
    lambdaBarTree->Branch("PionPt", &PionPt, "PionPt/D");
    lambdaBarTree->Branch("ProtonPt", &ProtonPt, "ProtonPt/D");
    lambdaBarTree->Branch("PionTrackStartRadius", &PionTrackStartRadius, "PionTrackStartRadius/D");
    lambdaBarTree->Branch("ProtonTrackStartRadius", &ProtonTrackStartRadius, "ProtonTrackStartRadius/D");
    lambdaBarTree->Branch("PionNHitsdEdx", &PionNHitsdEdx, "PionNHitsdEdx/D");
    lambdaBarTree->Branch("ProtonNHitsdEdx", &ProtonNHitsdEdx, "ProtonNHitsdEdx/D");
    lambdaBarTree->Branch("PiondEdx", &PiondEdx, "PiondEdx/D");
    lambdaBarTree->Branch("ProtondEdx", &ProtondEdx, "ProtondEdx/D");
    lambdaBarTree->Branch("PionL", &PionL, "PionL/D");
    lambdaBarTree->Branch("PionLProton", &PionLProton, "PionLProton/D");
    lambdaBarTree->Branch("PionLelectron", &PionLelectron, "PionLelectron/D");
    lambdaBarTree->Branch("ProtonL", &ProtonL, "ProtonL/D");
    lambdaBarTree->Branch("ProtonLPion", &ProtonLPion, "ProtonLPion/D");
    lambdaBarTree->Branch("ProtonLelectron", &ProtonLelectron, "ProtonLelectron/D");
    lambdaBarTree->Branch("PionTrackLength", &PionTrackLength, "PionTrackLength/D");
    lambdaBarTree->Branch("ProtonTrackLength", &ProtonTrackLength, "ProtonTrackLength/D");
    lambdaBarTree->Branch("PionDCA", &PionDCA, "PionDCA/D");
    lambdaBarTree->Branch("ProtonDCA", &ProtonDCA, "ProtonDCA/D");
    lambdaBarTree->Branch("PionSDCA", &PionSDCA, "PionSDCA/D");
    lambdaBarTree->Branch("ProtonSDCA", &ProtonSDCA, "ProtonSDCA/D");
    lambdaBarTree->Branch("Chi2", &Chi2, "Chi2/D");
    //lambdaTotTree->Branch("Chi2Proton", &Chi2Proton, "Chi2Proton/D");
    lambdaBarTree->Branch("MassPionPion", &MassPionPion, "MassPionPion/D");
    lambdaBarTree->Branch("MassElecElec", &MassElecElec, "MassElecElec/D");
    lambdaBarTree->Branch("pTrelPion", &pTrelPion, "pTrelPion/D");
    lambdaBarTree->Branch("pTrelProton", &pTrelProton, "pTrelProton/D");
    lambdaBarTree->Branch("RadiusSVtoPV", &RadiusSVtoPV, "RadiusSVtoPV/D");
    lambdaBarTree->Branch("IsBar", &IsBar, "IsBar/I");
    lambdaBarTree->Branch("PionNuclCorrWeight", &PionNuclCorrWeight, "PionNuclCorrWeight/D");
    lambdaBarTree->Branch("ProtonTrackEndRadius", &ProtonTrackEndRadius, "ProtonTrackEndRadius/D");
    lambdaBarTree->Branch("ProtonTrackEndMomentum", "TVector3", &ProtonTrackEndMomentum);
    lambdaBarTree->Branch("PionTrackEndRadius", &PionTrackEndRadius, "PionTrackEndRadius/D");
    lambdaBarTree->Branch("PionTrackEndMomentum", "TVector3", &PionTrackEndMomentum);
    lambdaBarTree->Branch("ProtonTrackV0Momentum", "TVector3", &ProtonTrackV0Momentum);
    lambdaBarTree->Branch("PionTrackV0Momentum", "TVector3", &PionTrackV0Momentum);
    lambdaBarTree->Branch("DecayLength", &DecayLength, "DecayLength/D");
    lambdaBarTree->Branch("DeltaDCA", &DeltaDCA, "DeltaDCA/D");

    lambdaTotTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaTotTree->Branch("pT", &pT, "pT/D");
    lambdaTotTree->Branch("Eta", &Eta, "Eta/D");
    lambdaTotTree->Branch("Phi", &Phi, "Phi/D");
    lambdaTotTree->Branch("x", &x, "x/D");
    lambdaTotTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaTotTree->Branch("Ys", &Ys, "Ys/D");
    lambdaTotTree->Branch("Ws", &Ws, "Ws/D"); //Weight of hadronic final state
    lambdaTotTree->Branch("Angle", &Angle, "Angle/D");
    lambdaTotTree->Branch("v0chi2", &v0chi2, "v0chi2/D");
    lambdaTotTree->Branch("weight1", &weight1, "weight1/D");
    lambdaTotTree->Branch("weight2", &weight2, "weight2/D");
    lambdaTotTree->Branch("SVx", &SVx, "SVx/D");
    lambdaTotTree->Branch("SVy", &SVy, "SVy/D");
    lambdaTotTree->Branch("SVz", &SVz, "SVz/D");
    lambdaTotTree->Branch("PVx", &PVx, "PVx/D");
    lambdaTotTree->Branch("PVy", &PVy, "PVy/D");
    lambdaTotTree->Branch("PVz", &PVz, "PVz/D");
    lambdaTotTree->Branch("LambdaPX", &LambdaPX, "LambdaPX/D");
    lambdaTotTree->Branch("LambdaPY", &LambdaPY, "LambdaPY/D");
    lambdaTotTree->Branch("LambdaPZ", &LambdaPZ, "LambdaPZ/D");
    lambdaTotTree->Branch("fRunType", &fRunType, "fRunType/O");
    lambdaTotTree->Branch("ystar", &ystar, "ystar/D");
    lambdaTotTree->Branch("CosThetaStar", &CosThetaStar, "CosThetaStar/D");
    lambdaTotTree->Branch("ylab", &ylab, "ylab/D");
    lambdaTotTree->Branch("scatEX", &scatEX, "scatEX/D");
    lambdaTotTree->Branch("scatEY", &scatEY, "scatEY/D");
    lambdaTotTree->Branch("scatEZ", &scatEZ, "scatEZ/D");
    //Below is unique braches to the LambdaTotTree compared to Reg and Bar Trees
    lambdaTotTree->Branch("PionCharge", &PionCharge, "PionCharge/D");
    lambdaTotTree->Branch("ProtonCharge", &ProtonCharge, "ProtonCharge/D");
    //Candidate Control Variables
    lambdaTotTree->Branch("pionFRUNZ", &pionFRUNZ, "pionFRUNZ/D");
    lambdaTotTree->Branch("protonFRUNZ", &protonFRUNZ, "protonFRUNZ/D");
    lambdaTotTree->Branch("PionPt", &PionPt, "PionPt/D");
    lambdaTotTree->Branch("ProtonPt", &ProtonPt, "ProtonPt/D");
    lambdaTotTree->Branch("PionTrackStartRadius", &PionTrackStartRadius, "PionTrackStartRadius/D");
    lambdaTotTree->Branch("ProtonTrackStartRadius", &ProtonTrackStartRadius, "ProtonTrackStartRadius/D");
    lambdaTotTree->Branch("PionNHitsdEdx", &PionNHitsdEdx, "PionNHitsdEdx/D");
    lambdaTotTree->Branch("ProtonNHitsdEdx", &ProtonNHitsdEdx, "ProtonNHitsdEdx/D");
    lambdaTotTree->Branch("PiondEdx", &PiondEdx, "PiondEdx/D");
    lambdaTotTree->Branch("ProtondEdx", &ProtondEdx, "ProtondEdx/D");
    lambdaTotTree->Branch("PionL", &PionL, "PionL/D");
    lambdaTotTree->Branch("PionLProton", &PionLProton, "PionLProton/D");
    lambdaTotTree->Branch("PionLelectron", &PionLelectron, "PionLelectron/D");
    lambdaTotTree->Branch("ProtonL", &ProtonL, "ProtonL/D");
    lambdaTotTree->Branch("ProtonLPion", &ProtonLPion, "ProtonLPion/D");
    lambdaTotTree->Branch("ProtonLelectron", &ProtonLelectron, "ProtonLelectron/D");
    lambdaTotTree->Branch("PionTrackLength", &PionTrackLength, "PionTrackLength/D");
    lambdaTotTree->Branch("ProtonTrackLength", &ProtonTrackLength, "ProtonTrackLength/D");
    lambdaTotTree->Branch("PionDCA", &PionDCA, "PionDCA/D");
    lambdaTotTree->Branch("ProtonDCA", &ProtonDCA, "ProtonDCA/D");
    lambdaTotTree->Branch("PionSDCA", &PionSDCA, "PionSDCA/D");
    lambdaTotTree->Branch("ProtonSDCA", &ProtonSDCA, "ProtonSDCA/D");
    lambdaTotTree->Branch("Chi2", &Chi2, "Chi2/D");
    //lambdaTotTree->Branch("Chi2Proton", &Chi2Proton, "Chi2Proton/D");
    lambdaTotTree->Branch("MassPionPion", &MassPionPion, "MassPionPion/D");
    lambdaTotTree->Branch("MassElecElec", &MassElecElec, "MassElecElec/D");
    lambdaTotTree->Branch("pTrelPion", &pTrelPion, "pTrelPion/D");
    lambdaTotTree->Branch("pTrelProton", &pTrelProton, "pTrelProton/D");
    lambdaTotTree->Branch("RadiusSVtoPV", &RadiusSVtoPV, "RadiusSVtoPV/D");
    lambdaTotTree->Branch("IsBar", &IsBar, "IsBar/I");
    lambdaTotTree->Branch("FoundGen", &FoundGen, "FoundGen/I");
    lambdaTotTree->Branch("s61", &s61, "s61/I");
    lambdaTotTree->Branch("s1", &s1, "s1/I");
    lambdaTotTree->Branch("s74", &s74, "s74/I");
    lambdaTotTree->Branch("s75", &s75, "s75/I");
    lambdaTotTree->Branch("s67", &s67, "s67/I");
    lambdaTotTree->Branch("PionNuclCorrWeight", &PionNuclCorrWeight, "PionNuclCorrWeight/D");
    lambdaTotTree->Branch("ProtonTrackEndRadius", &ProtonTrackEndRadius, "ProtonTrackEndRadius/D");
    lambdaTotTree->Branch("ProtonTrackEndMomentum", "TVector3", &ProtonTrackEndMomentum);
    lambdaTotTree->Branch("PionTrackEndRadius", &PionTrackEndRadius, "PionTrackEndRadius/D");
    lambdaTotTree->Branch("PionTrackEndMomentum", "TVector3", &PionTrackEndMomentum);
    lambdaTotTree->Branch("ProtonTrackV0Momentum", "TVector3", &ProtonTrackV0Momentum);
    lambdaTotTree->Branch("PionTrackV0Momentum", "TVector3", &PionTrackV0Momentum);
    lambdaTotTree->Branch("DecayLength", &DecayLength, "DecayLength/D");
    lambdaTotTree->Branch("DeltaDCA", &DeltaDCA, "DeltaDCA/D");

    //double pTgen, Etagen, xgen, Q2sgen, Ysgen, SVxgen, SVygen, SVzgen, PVzgen, Phigen, genrecAngle;
    //STR Trees
    lambdaRegGenTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaRegGenTree->Branch("pT", &pT, "pT/D");
    lambdaRegGenTree->Branch("Eta", &Eta, "Eta/D");
    lambdaRegGenTree->Branch("Phi", &Phi, "Phi/D");
    lambdaRegGenTree->Branch("Phigen", &Phigen, "Phigen/D");
    lambdaRegGenTree->Branch("x", &x, "x/D");
    lambdaRegGenTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaRegGenTree->Branch("Ys", &Ys, "Ys/D");
    lambdaRegGenTree->Branch("Angle", &Angle, "Angle/D");
    lambdaRegGenTree->Branch("v0chi2", &v0chi2, "v0chi2/D");
    lambdaRegGenTree->Branch("weight1", &weight1, "weight1/D");
    lambdaRegGenTree->Branch("weight2", &weight2, "weight2/D");
    lambdaRegGenTree->Branch("SVx", &SVx, "SVx/D");
    lambdaRegGenTree->Branch("SVy", &SVy, "SVy/D");
    lambdaRegGenTree->Branch("SVz", &SVz, "SVz/D");
    lambdaRegGenTree->Branch("PVx", &PVx, "PVx/D");
    lambdaRegGenTree->Branch("PVy", &PVy, "PVy/D");
    lambdaRegGenTree->Branch("PVz", &PVz, "PVz/D");
    lambdaRegGenTree->Branch("LambdaPX", &LambdaPX, "LambdaPX/D");
    lambdaRegGenTree->Branch("LambdaPY", &LambdaPY, "LambdaPY/D");
    lambdaRegGenTree->Branch("LambdaPZ", &LambdaPZ, "LambdaPZ/D");
    lambdaRegGenTree->Branch("fRunType", &fRunType, "fRunType/O");
    lambdaRegGenTree->Branch("ystar", &ystar, "ystar/D");
    lambdaRegGenTree->Branch("ylab", &ylab, "ylab/D");
    lambdaRegGenTree->Branch("scatEX", &scatEX, "scatEX/D");
    lambdaRegGenTree->Branch("scatEY", &scatEY, "scatEY/D");
    lambdaRegGenTree->Branch("scatEZ", &scatEZ, "scatEZ/D");
    lambdaRegGenTree->Branch("FoundGen", &FoundGen, "FoundGen/I");
    //Adding STR specific branches
    lambdaRegGenTree->Branch("pTgen", &pTgen, "pTgen/D");
    lambdaRegGenTree->Branch("Etagen", &Etagen, "Etagen/D");
    lambdaRegGenTree->Branch("xgen", &xgen, "xgen/D");
    lambdaRegGenTree->Branch("Q2sgen", &Q2sgen, "Q2sgen/D");
    lambdaRegGenTree->Branch("Ysgen", &Ysgen, "Ysgen/D");
    lambdaRegGenTree->Branch("SVxgen", &SVxgen, "SVxgen/D");
    lambdaRegGenTree->Branch("SVygen", &SVygen, "SVygen/D");
    lambdaRegGenTree->Branch("SVzgen", &SVzgen, "SVzgen/D");
    lambdaRegGenTree->Branch("PVzgen", &PVzgen, "PVzgen/D");
    lambdaRegGenTree->Branch("genrecAngle", &genrecAngle, "genrecAngle/D");
    lambdaRegGenTree->Branch("PionNuclCorrWeight", &PionNuclCorrWeight, "PionNuclCorrWeight/D");

    lambdaBarGenTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaBarGenTree->Branch("pT", &pT, "pT/D");
    lambdaBarGenTree->Branch("Eta", &Eta, "Eta/D");
    lambdaBarGenTree->Branch("Phi", &Phi, "Phi/D");
    lambdaBarGenTree->Branch("Phigen", &Phigen, "Phigen/D");
    lambdaBarGenTree->Branch("x", &x, "x/D");
    lambdaBarGenTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaBarGenTree->Branch("Ys", &Ys, "Ys/D");
    lambdaBarGenTree->Branch("Angle", &Angle, "Angle/D");
    lambdaBarGenTree->Branch("v0chi2", &v0chi2, "v0chi2/D");
    lambdaBarGenTree->Branch("weight1", &weight1, "weight1/D");
    lambdaBarGenTree->Branch("weight2", &weight2, "weight2/D");
    lambdaBarGenTree->Branch("SVx", &SVx, "SVx/D");
    lambdaBarGenTree->Branch("SVy", &SVy, "SVy/D");
    lambdaBarGenTree->Branch("SVz", &SVz, "SVz/D");
    lambdaBarGenTree->Branch("PVx", &PVx, "PVx/D");
    lambdaBarGenTree->Branch("PVy", &PVy, "PVy/D");
    lambdaBarGenTree->Branch("PVz", &PVz, "PVz/D");
    lambdaBarGenTree->Branch("LambdaPX", &LambdaPX, "LambdaPX/D");
    lambdaBarGenTree->Branch("LambdaPY", &LambdaPY, "LambdaPY/D");
    lambdaBarGenTree->Branch("LambdaPZ", &LambdaPZ, "LambdaPZ/D");
    lambdaBarGenTree->Branch("fRunType", &fRunType, "fRunType/O");
    lambdaBarGenTree->Branch("ystar", &ystar, "ystar/D");
    lambdaBarGenTree->Branch("ylab", &ylab, "ylab/D");
    lambdaBarGenTree->Branch("scatEX", &scatEX, "scatEX/D");
    lambdaBarGenTree->Branch("scatEY", &scatEY, "scatEY/D");
    lambdaBarGenTree->Branch("scatEZ", &scatEZ, "scatEZ/D");
    lambdaBarGenTree->Branch("FoundGen", &FoundGen, "FoundGen/I");
    //Adding STR specific branches
    lambdaBarGenTree->Branch("pTgen", &pTgen, "pTgen/D");
    lambdaBarGenTree->Branch("Etagen", &Etagen, "Etagen/D");
    lambdaBarGenTree->Branch("xgen", &xgen, "xgen/D");
    lambdaBarGenTree->Branch("Q2sgen", &Q2sgen, "Q2sgen/D");
    lambdaBarGenTree->Branch("Ysgen", &Ysgen, "Ysgen/D");
    lambdaBarGenTree->Branch("SVxgen", &SVxgen, "SVxgen/D");
    lambdaBarGenTree->Branch("SVygen", &SVygen, "SVygen/D");
    lambdaBarGenTree->Branch("SVzgen", &SVzgen, "SVzgen/D");
    lambdaBarGenTree->Branch("PVzgen", &PVzgen, "PVzgen/D");
    lambdaBarGenTree->Branch("genrecAngle", &genrecAngle, "genrecAngle/D");
    lambdaBarGenTree->Branch("PionNuclCorrWeight", &PionNuclCorrWeight, "PionNuclCorrWeight/D");

    lambdaTotGenTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaTotGenTree->Branch("pT", &pT, "pT/D");
    lambdaTotGenTree->Branch("Eta", &Eta, "Eta/D");
    lambdaTotGenTree->Branch("Phi", &Phi, "Phi/D");
    lambdaTotGenTree->Branch("Phigen", &Phigen, "Phigen/D");
    lambdaTotGenTree->Branch("x", &x, "x/D");
    lambdaTotGenTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaTotGenTree->Branch("Ys", &Ys, "Ys/D");
    lambdaTotGenTree->Branch("Angle", &Angle, "Angle/D");
    lambdaTotGenTree->Branch("v0chi2", &v0chi2, "v0chi2/D");
    lambdaTotGenTree->Branch("weight1", &weight1, "weight1/D");
    lambdaTotGenTree->Branch("weight2", &weight2, "weight2/D");
    lambdaTotGenTree->Branch("SVx", &SVx, "SVx/D");
    lambdaTotGenTree->Branch("SVy", &SVy, "SVy/D");
    lambdaTotGenTree->Branch("SVz", &SVz, "SVz/D");
    lambdaTotGenTree->Branch("PVx", &PVx, "PVx/D");
    lambdaTotGenTree->Branch("PVy", &PVy, "PVy/D");
    lambdaTotGenTree->Branch("PVz", &PVz, "PVz/D");
    lambdaTotGenTree->Branch("LambdaPX", &LambdaPX, "LambdaPX/D");
    lambdaTotGenTree->Branch("LambdaPY", &LambdaPY, "LambdaPY/D");
    lambdaTotGenTree->Branch("LambdaPZ", &LambdaPZ, "LambdaPZ/D");
    lambdaTotGenTree->Branch("fRunType", &fRunType, "fRunType/O");
    lambdaTotGenTree->Branch("ystar", &ystar, "ystar/D");
    lambdaTotGenTree->Branch("ylab", &ylab, "ylab/D");
    lambdaTotGenTree->Branch("scatEX", &scatEX, "scatEX/D");
    lambdaTotGenTree->Branch("scatEY", &scatEY, "scatEY/D");
    lambdaTotGenTree->Branch("scatEZ", &scatEZ, "scatEZ/D");
    lambdaTotGenTree->Branch("FoundGen", &FoundGen, "FoundGen/I");
    //Adding STR specific branches
    lambdaTotGenTree->Branch("pTgen", &pTgen, "pTgen/D");
    lambdaTotGenTree->Branch("Etagen", &Etagen, "Etagen/D");
    lambdaTotGenTree->Branch("xgen", &xgen, "xgen/D");
    lambdaTotGenTree->Branch("Q2sgen", &Q2sgen, "Q2sgen/D");
    lambdaTotGenTree->Branch("Ysgen", &Ysgen, "Ysgen/D");
    lambdaTotGenTree->Branch("SVxgen", &SVxgen, "SVxgen/D");
    lambdaTotGenTree->Branch("SVygen", &SVygen, "SVygen/D");
    lambdaTotGenTree->Branch("SVzgen", &SVzgen, "SVzgen/D");
    lambdaTotGenTree->Branch("PVzgen", &PVzgen, "PVzgen/D");
    lambdaTotGenTree->Branch("genrecAngle", &genrecAngle, "genrecAngle/D");
    lambdaTotGenTree->Branch("PionNuclCorrWeight", &PionNuclCorrWeight, "PionNuclCorrWeight/D");

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    lambdaRegBGTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaRegBGTree->Branch("pT", &pT, "pT/D");
    lambdaRegBGTree->Branch("Eta", &Eta, "Eta/D");
    lambdaRegBGTree->Branch("Phi", &Phi, "Phi/D");
    lambdaRegBGTree->Branch("Phigen", &Phigen, "Phigen/D");
    lambdaRegBGTree->Branch("x", &x, "x/D");
    lambdaRegBGTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaRegBGTree->Branch("Ys", &Ys, "Ys/D");
    lambdaRegBGTree->Branch("Angle", &Angle, "Angle/D");
    lambdaRegBGTree->Branch("v0chi2", &v0chi2, "v0chi2/D");
    lambdaRegBGTree->Branch("weight1", &weight1, "weight1/D");
    lambdaRegBGTree->Branch("weight2", &weight2, "weight2/D");
    lambdaRegBGTree->Branch("SVx", &SVx, "SVx/D");
    lambdaRegBGTree->Branch("SVy", &SVy, "SVy/D");
    lambdaRegBGTree->Branch("SVz", &SVz, "SVz/D");
    lambdaRegBGTree->Branch("PVx", &PVx, "PVx/D");
    lambdaRegBGTree->Branch("PVy", &PVy, "PVy/D");
    lambdaRegBGTree->Branch("PVz", &PVz, "PVz/D");
    lambdaRegBGTree->Branch("LambdaPX", &LambdaPX, "LambdaPX/D");
    lambdaRegBGTree->Branch("LambdaPY", &LambdaPY, "LambdaPY/D");
    lambdaRegBGTree->Branch("LambdaPZ", &LambdaPZ, "LambdaPZ/D");
    lambdaRegBGTree->Branch("fRunType", &fRunType, "fRunType/O");
    lambdaRegBGTree->Branch("ystar", &ystar, "ystar/D");
    lambdaRegBGTree->Branch("ylab", &ylab, "ylab/D");
    lambdaRegBGTree->Branch("scatEX", &scatEX, "scatEX/D");
    lambdaRegBGTree->Branch("scatEY", &scatEY, "scatEY/D");
    lambdaRegBGTree->Branch("scatEZ", &scatEZ, "scatEZ/D");
    lambdaRegBGTree->Branch("FoundGen", &FoundGen, "FoundGen/I");
    //Adding STR specific branches
    lambdaRegBGTree->Branch("pTgen", &pTgen, "pTgen/D");
    lambdaRegBGTree->Branch("Etagen", &Etagen, "Etagen/D");
    lambdaRegBGTree->Branch("xgen", &xgen, "xgen/D");
    lambdaRegBGTree->Branch("Q2sgen", &Q2sgen, "Q2sgen/D");
    lambdaRegBGTree->Branch("Ysgen", &Ysgen, "Ysgen/D");
    lambdaRegBGTree->Branch("SVxgen", &SVxgen, "SVxgen/D");
    lambdaRegBGTree->Branch("SVygen", &SVygen, "SVygen/D");
    lambdaRegBGTree->Branch("SVzgen", &SVzgen, "SVzgen/D");
    lambdaRegBGTree->Branch("PVzgen", &PVzgen, "PVzgen/D");
    lambdaRegBGTree->Branch("PionNuclCorrWeight", &PionNuclCorrWeight, "PionNuclCorrWeight/D");

    lambdaBarBGTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaBarBGTree->Branch("pT", &pT, "pT/D");
    lambdaBarBGTree->Branch("Eta", &Eta, "Eta/D");
    lambdaBarBGTree->Branch("Phi", &Phi, "Phi/D");
    lambdaBarBGTree->Branch("Phigen", &Phigen, "Phigen/D");
    lambdaBarBGTree->Branch("x", &x, "x/D");
    lambdaBarBGTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaBarBGTree->Branch("Ys", &Ys, "Ys/D");
    lambdaBarBGTree->Branch("Angle", &Angle, "Angle/D");
    lambdaBarBGTree->Branch("v0chi2", &v0chi2, "v0chi2/D");
    lambdaBarBGTree->Branch("weight1", &weight1, "weight1/D");
    lambdaBarBGTree->Branch("weight2", &weight2, "weight2/D");
    lambdaBarBGTree->Branch("SVx", &SVx, "SVx/D");
    lambdaBarBGTree->Branch("SVy", &SVy, "SVy/D");
    lambdaBarBGTree->Branch("SVz", &SVz, "SVz/D");
    lambdaBarBGTree->Branch("PVx", &PVx, "PVx/D");
    lambdaBarBGTree->Branch("PVy", &PVy, "PVy/D");
    lambdaBarBGTree->Branch("PVz", &PVz, "PVz/D");
    lambdaBarBGTree->Branch("LambdaPX", &LambdaPX, "LambdaPX/D");
    lambdaBarBGTree->Branch("LambdaPY", &LambdaPY, "LambdaPY/D");
    lambdaBarBGTree->Branch("LambdaPZ", &LambdaPZ, "LambdaPZ/D");
    lambdaBarBGTree->Branch("fRunType", &fRunType, "fRunType/O");
    lambdaBarBGTree->Branch("ystar", &ystar, "ystar/D");
    lambdaBarBGTree->Branch("ylab", &ylab, "ylab/D");
    lambdaBarBGTree->Branch("scatEX", &scatEX, "scatEX/D");
    lambdaBarBGTree->Branch("scatEY", &scatEY, "scatEY/D");
    lambdaBarBGTree->Branch("scatEZ", &scatEZ, "scatEZ/D");
    lambdaBarBGTree->Branch("FoundGen", &FoundGen, "FoundGen/I");
    //Adding STR specific branches
    lambdaBarBGTree->Branch("pTgen", &pTgen, "pTgen/D");
    lambdaBarBGTree->Branch("Etagen", &Etagen, "Etagen/D");
    lambdaBarBGTree->Branch("xgen", &xgen, "xgen/D");
    lambdaBarBGTree->Branch("Q2sgen", &Q2sgen, "Q2sgen/D");
    lambdaBarBGTree->Branch("Ysgen", &Ysgen, "Ysgen/D");
    lambdaBarBGTree->Branch("SVxgen", &SVxgen, "SVxgen/D");
    lambdaBarBGTree->Branch("SVygen", &SVygen, "SVygen/D");
    lambdaBarBGTree->Branch("SVzgen", &SVzgen, "SVzgen/D");
    lambdaBarBGTree->Branch("PVzgen", &PVzgen, "PVzgen/D");
    lambdaBarBGTree->Branch("PionNuclCorrWeight", &PionNuclCorrWeight, "PionNuclCorrWeight/D");

    lambdaTotBGTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaTotBGTree->Branch("pT", &pT, "pT/D");
    lambdaTotBGTree->Branch("Eta", &Eta, "Eta/D");
    lambdaTotBGTree->Branch("Phi", &Phi, "Phi/D");
    lambdaTotBGTree->Branch("Phigen", &Phigen, "Phigen/D");
    lambdaTotBGTree->Branch("x", &x, "x/D");
    lambdaTotBGTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaTotBGTree->Branch("Ys", &Ys, "Ys/D");
    lambdaTotBGTree->Branch("Angle", &Angle, "Angle/D");
    lambdaTotBGTree->Branch("v0chi2", &v0chi2, "v0chi2/D");
    lambdaTotBGTree->Branch("weight1", &weight1, "weight1/D");
    lambdaTotBGTree->Branch("weight2", &weight2, "weight2/D");
    lambdaTotBGTree->Branch("SVx", &SVx, "SVx/D");
    lambdaTotBGTree->Branch("SVy", &SVy, "SVy/D");
    lambdaTotBGTree->Branch("SVz", &SVz, "SVz/D");
    lambdaTotBGTree->Branch("PVx", &PVx, "PVx/D");
    lambdaTotBGTree->Branch("PVy", &PVy, "PVy/D");
    lambdaTotBGTree->Branch("PVz", &PVz, "PVz/D");
    lambdaTotBGTree->Branch("LambdaPX", &LambdaPX, "LambdaPX/D");
    lambdaTotBGTree->Branch("LambdaPY", &LambdaPY, "LambdaPY/D");
    lambdaTotBGTree->Branch("LambdaPZ", &LambdaPZ, "LambdaPZ/D");
    lambdaTotBGTree->Branch("fRunType", &fRunType, "fRunType/O");
    lambdaTotBGTree->Branch("ystar", &ystar, "ystar/D");
    lambdaTotBGTree->Branch("ylab", &ylab, "ylab/D");
    lambdaTotBGTree->Branch("scatEX", &scatEX, "scatEX/D");
    lambdaTotBGTree->Branch("scatEY", &scatEY, "scatEY/D");
    lambdaTotBGTree->Branch("scatEZ", &scatEZ, "scatEZ/D");
    lambdaTotBGTree->Branch("FoundGen", &FoundGen, "FoundGen/I");
    //Adding STR specific branches
    lambdaTotBGTree->Branch("pTgen", &pTgen, "pTgen/D");
    lambdaTotBGTree->Branch("Etagen", &Etagen, "Etagen/D");
    lambdaTotBGTree->Branch("xgen", &xgen, "xgen/D");
    lambdaTotBGTree->Branch("Q2sgen", &Q2sgen, "Q2sgen/D");
    lambdaTotBGTree->Branch("Ysgen", &Ysgen, "Ysgen/D");
    lambdaTotBGTree->Branch("SVxgen", &SVxgen, "SVxgen/D");
    lambdaTotBGTree->Branch("SVygen", &SVygen, "SVygen/D");
    lambdaTotBGTree->Branch("SVzgen", &SVzgen, "SVzgen/D");
    lambdaTotBGTree->Branch("PVzgen", &PVzgen, "PVzgen/D");
    lambdaTotBGTree->Branch("PionNuclCorrWeight", &PionNuclCorrWeight, "PionNuclCorrWeight/D");


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  Defining Cut Progression Histograms

    THStack* hs = new THStack("hs", "Stacked Total Lambda Mass Plots");
    THStack* hsC = new THStack("hsC", "Stacked Total LambdaC Mass Plots");

    TH1F* LambdaCutProgression[16];
    for (int a = 0; a < 16; a++) {
        string lambdacutprogstring = "Lambda Cut " + to_string(a);
        LambdaCutProgression[a] = new TH1F(lambdacutprogstring.c_str(), lambdacutprogstring.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    }

    TH1F* LambdaCCutProgression[16];
    for (int a = 0; a < 16; a++) {
        string lambdaCcutprogstring = "LambdaC Cut " + to_string(a);
        LambdaCCutProgression[a] = new TH1F(lambdaCcutprogstring.c_str(), lambdaCcutprogstring.c_str(), lambdaCbins, lambdaClowerbinrange, lambdaCupperbinrange);
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    //  Defining Root Canvas to work on
    TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 800);

   
    //  Pointer to vertices to loop over
    static H1CentralVertexArrayPtr vertex;
    static H1PartMCArrayPtr gtrdata;
    static H1PartLambdaArrayPtr LambdaCand;
    static H1StrBankPtr StrBank;
    static H1PartSelTrackArrayPtr PST;
    static H1SelVertexArrayPtr vertexgen;
    


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
    Int_t RunPeriod;
    gH1Calc = H1Calculator::Instance();
    while (H1Tree::Instance()->Next() && !opts.IsMaxEvent(eventCounter)) {
        gH1Calc->Reset();
        //gH1Constants->SetConstants(gH1Calc->GetRunNumber()); // SetConstants takes either period or run

        //RunPeriod = gH1Constants->GetRunPeriod();

        //cout << "Run Period                          = ";
        //if (RunPeriod == H1Constants::eEplus9900)  cout << "e+ 99/00"
        //    << endl;
        //else if (RunPeriod == H1Constants::eEplus0304)  cout << "e+ 03/04"
        //    << endl;
        //else if (RunPeriod == H1Constants::eEminus0405) cout << "e- 04/05"
        //    << endl;
        //else if (RunPeriod == H1Constants::eEminus06)   cout << "e- 06" <<
        //    endl;
        //else if (RunPeriod == H1Constants::eEplus0607)  cout << "e+ 06/07"
        //    << endl;
        //else {
        //    Error("AnalysisBase::InitialSettings", "Unkown Run Period. Please correct the steering.");
        //        exit(1);
        //    cout << RunPeriod << endl;
        //}
        LambdaCount = 0; // initialize this for every event
        int breakloopcount = 0;
        int passedinitialcutscount = 0;
        int passedallcutscount = 0;
        int particlesinevent = 0;
        //Initializing items to help with consistency checks
        vector<int> LambdaIndices = {}; //store indices of lambda candidates in H1PartLambdaArray that pass all cuts
        vector<double> LambdaInvMasses = {}; //store invariantmasses of lambda candidates in H1PartLambdaArray that pass all cuts
        vector<int> LambdaExcludedIndices = {}; //will check for doubly assigned tracks to candidates and excluded indices will be stored here

        //Storing pT values of tracks, these will only be exactly identical if the same track is used twice
        vector<double> ProtonTrackCheck = {}; //pT values of proton tracks of lambda candidates will be stored here
        vector<double> PionTrackCheck = {}; //pT values of pion tracks of lambda candidates will be stored here

        vector<double> PIDProtonTracks = {}; //DeDx Likelihood of proton tracks of lambda candidates will be stored here
        vector<double> PIDPionTracks = {}; //DeDx Likelihood ofof pion tracks of lambda candidates will be stored here
        
        //Storing lambda candidate loop value to later access H1Track class members of tracks
        vector<Int_t> ProtonH1TrackCheck = {};
        vector<Int_t> PionH1TrackCheck = {};
        //vector<Int_t> PionCH1TrackCheck = {};
                                           
        //Tests for my duplicate vector entry finder functions
        /*vector<vector<int>> TestIndexValues = { {0,1}, {0,1} };
        vector<double> testvec = {1.2,1.8,1.1,1.2,1.3};

        int testfind = singlefind(testvec,0,testvec.size(),1.3);

        cout << "Test Find: " << testfind << endl;*/




        //Initialize some vectors for storing Lambda generator level
        vector<double> GtrAngleLambdaReg = {};
        vector<double> GtrAngleLambdaBar = {};
        vector<double> GtrAngleNegPionLambdaReg = {};
        vector<double> GtrAnglePosPionLambdaBar = {};

        vector<double> GtrLambdaRegIndex = {};
        vector<double> GtrLambdaBarIndex = {};

        vector<double> PionNucIntCorrLambdaRegIndex = {};
        vector<double> PionNucIntCorrLambdaBarIndex = {};

        //vector<vector<double>> PionNucIntCorrLambdaRegWeightIndex = {};
        //vector<vector<double>> PionNucIntCorrLambdaBarWeightIndex = {};

        double EventPionNuclearCorrectionWeight = 0;
                                                                                                                                                                                                                                                                            
        int fRunType = gH1Calc->IsMC(); //0 = data, else is MC
        //cout << "Run Type: " << fRunType << endl;
        if (fRunType != 0) {
            int havesimulatedvertex = 0;
            PVzgen = 0;
            //double Zvtxgen;
            for (int v = 0; v < vertexgen.GetEntries(); v++) {
                Int_t type = vertexgen[v]->GetVertexType();
                //if (type == 0) {
                //    havePrimaryVertex = true;
                //    primaryVertex = vertex[v]->GetPosition();
                //}
                if (type == 1) {
                    havesimulatedvertex = 1;
                    PVzgen = vertexgen[v]->GetPosition().Z();
                }
            }
            //Loop over generator data
            for (Int_t gtrloop = 0; gtrloop < gtrdata.GetEntries(); gtrloop++) {
                H1PartMC* part = gtrdata[gtrloop];

                //Lambdas do not decay on H1 gen level, so need to have a separate check for Pion tracks
                //-211 is for negative pion, so this would be potentially be coming for a Lambda
                if ((part->GetPDG() == -211) && (part->GetMother2()<0) && (part->IsStr()==1)) {
                    if (gtrdata[part->GetMother1()]->GetPDG() == 3122) {
                        //If this pions mother is a Lambda, this is a pion we could potentially reconstruct, so I save its index in this array to reference later
                        //Calculate the pion particles weight contribution (correction from Karin for more accurate detector simulation)

                        //  Float_t GetWeight(int ipart, float charge, float pt, float phi, float theta, float dca, float z0)
                        //The integer 1 in the input of H1NuclIACor is strange, this correction should only be for pions (positive or negative), so putting a 1 there makes the calculation go or else you get a wrong particle type, I have already ensured this with my PDG checks so hardcoding a 1 there
                        //double tempweight = H1NuclIACor::GetWeight(1, -1.0, part->GetFourVector().Vect().Pt(), part->GetFourVector().Vect().Phi(), part->GetFourVector().Vect().Theta(), 0.0, PVzgen);
                        //cout << "Weight: " << tempweight << endl;
                        //PionNucIntCorrLambdaRegWeightIndex.push_back({ gtrloop, tempweight });
                        PionNucIntCorrLambdaRegIndex.push_back(gtrloop);
                        //cout << "Lambda Reg Pion" << endl;
                    }
                }

                //211 is for a positive pion, so this would be potentially be coming for a Lambda Bar
                //For this, I am only counting pions that come from Lambdas, so all these pions must be on STR level and must only have one mother particle
                if ((part->GetPDG() == 211) && (part->GetMother2() < 0) && (part->IsStr() == 1)) {
                    if (gtrdata[part->GetMother1()]->GetPDG() == -3122) {
                        //If this pions mother is a Lambda, this is a pion we could potentially reconstruct, so I save its index in this array to reference later
                        //Calculate the pion particles weight contribution (correction from Karin for more accurate detector simulation)
                        
                        //  Float_t GetWeight(int ipart, float charge, float pt, float phi, float theta, float dca, float z0)
                        //The integer 1 in the input of H1NuclIACor is strange, this correction should only be for pions (positive or negative), so putting a 1 there makes the calculation go or else you get a wrong particle type, I have already ensured this with my PDG checks so hardcoding a 1 there
                        //double tempweight = H1NuclIACor::GetWeight(1, 1.0, part->GetFourVector().Vect().Pt(), part->GetFourVector().Vect().Phi(), part->GetFourVector().Vect().Theta(),0.0, PVzgen);
                        //cout << "Weight: " << tempweight << endl;
                        //PionNucIntCorrLambdaBarWeightIndex.push_back({ gtrloop, tempweight });
                        PionNucIntCorrLambdaBarIndex.push_back(gtrloop);
                        //cout << "Lambda Bar Pion" << endl;
                    }
                }

                if (part->GetPDG() == -3122) {
                    GtrLambdaBarIndex.push_back(gtrloop);
                    
                }

                if (part->GetPDG() == 3122) {
                    GtrLambdaRegIndex.push_back(gtrloop);
                
                }
            }
        }
       
        // Loop over all Lambda candidates on MODS level as determined by rough cuts in H1FindLambda.C code
        //This loop quickly saves data from all Lambda Candidates in vectors to use in the primary loop
        //Need to access data for all candidates to properly reject duplicate tracks, we want to only keep the Lambda particles who's daughters best fit the secondary vertex
        //for (Int_t preloop = 0; preloop < LambdaCand.GetEntries(); preloop++) {
            //H1NuclIACor::GetWeight
            //(part, myEvent.typeChgREC[iREC] > 0 ? 1 : -1,
            //    myEvent.momREC[iREC].Pt(),
            //    myEvent.momREC[iREC].Phi(), myEvent.momREC[iREC].Theta(),
            //    0.0 /* dca */, H1SelVertex::GetPrimaryVertex()->Z());
        //}


        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Loop over all Lambda candidates on MODS level as determined by rough cuts in H1FindLambda.C code
        // storing all Chi2 of secondary vertex fits for candidates and then reordering from lowest to highest Chi2
        // I have built in checks to reject redundant tracks (multiple secondary fit hypotheses)
        // with this re ordering I will ensure we keep the candidate with lowest Chi2 fit for secondary vertex hypothesis
        //cout << "Number of Entries: " << LambdaCand.GetEntries() << endl;
        vector<vector<double>> preorderedvector = {};
        for (Int_t orderingloop = 0; orderingloop < LambdaCand.GetEntries(); orderingloop++) {
            //Chi2 = LambdaCand[loop]->GetV0()->GetFitChi2();
            //Save the Chi2 V0 fit and candidate loop number for all candidates into this preorderedarray
            preorderedvector.push_back({ LambdaCand[orderingloop]->GetV0()->GetFitChi2() , orderingloop});
            //cout << "Entry " << orderingloop << ": " << preorderedvector[orderingloop][0] << " " << preorderedvector[orderingloop][1] << endl;
        }
        //Now I will reorder the entries of this array from lowest to highest Chi2 values with the std::sort function
        std::sort(preorderedvector.begin(), preorderedvector.end(), [](const std::vector<double>& a, const std::vector<double>& b) {
            return a[0] < b[0];  // Compare first components
            });
        //for (Int_t orderingloopcheck = 0; orderingloopcheck < LambdaCand.GetEntries(); orderingloopcheck++) {
        //    cout << "Ordered Entry " << orderingloopcheck << ": " << preorderedvector[orderingloopcheck][0] << " " << preorderedvector[orderingloopcheck][1] << endl;
        //}
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Loop over all Lambda candidates on MODS level as determined by rough cuts in H1FindLambda.C code
        for (Int_t entryint = 0; entryint < LambdaCand.GetEntries(); entryint++) {

            //I want to access the Lambda candidates from lowest to highest Chi2 V0 hypothesis values so that the duplicate tracks I reject are ones with higher Chi2 values
            //So, I get the ordering from the preordervector in the following line
            int loop = preorderedvector[entryint][1];


            if (LambdaCand[loop]->GetNumOfParticles() != 2) { //Ensuring there are 2 stored daughters for candidates
                breakloopcount++;
                continue;
            }
         
        //Getting candidate invariant mass using reconstructed daughters
        invMasslambda = (LambdaCand[loop]->GetProtonFourVector()
            + LambdaCand[loop]->GetPionFourVector()).M();

        ////////////////////////////////////////////////////////////////////
        //H1FpackIndexManager* sourceManager;
        //H1FpackIndex* source;
        //if (!sourceManager) { cout << "No source found" << endl; }
        //TObjString* fileName = (TObjString*)sourceManager->fFileNames.At(source->fFileIndex);
        //cout << strncmp(fileName.Data()) << endl;
        ////////////////////////////////////////////////////////////////////
        //Getting Vectors for Armenteros-Podolansky Plot
        H1Track* ProtonH1Track = (H1Track*)LambdaCand[loop]->GetProtonTrack(); //This is casting H1CentralFittedTrack h1oo class as H1Track for daughter
        //cout << "HERE" << endl; Code seg faults here when trying to access track information
        H1Track* PionH1Track = (H1Track*)LambdaCand[loop]->GetPionTrack(); //This is casting H1CentralFittedTrack h1oo class as H1Track for daughter

        TVector3 vecProton = LambdaCand[loop]->GetProtonFourVector().Vect(); //Proton 3 vector
        TVector3 vecPion = LambdaCand[loop]->GetPionFourVector().Vect();  //Pion 3 vector
        //TVector3 vecLambda = LambdaCand[loop]->GetLambdaFourVector().Vect();  //Lambda 3 vector
        
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
        Eta = LambdaCand[loop]->GetFourVector().Vect().Eta();

        LambdaCutProgression[0]->Fill(invMasslambda); //Raw Lambda Candidates from rough cuts in H1FindLambda


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //I want to first implement the cuts that have an intentional impact on the particle signal
        //ie: Eta Cut, pT Cut, Redundant Track Cut,
        

        //Eta Cut
        if (abs(Eta) > 1.3) {
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[1]->Fill(invMasslambda);

        //pT Cut
        if (pT < 0.5 || pT > 3.5) {
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[2]->Fill(invMasslambda);


        //Need to Apply some further cuts on the tracks of the Lambda daughters
        //Cut on distance maximal radial distance of first CJC hit
        //requiring this to be < 35 cm will ensure our tracks are measureed in the CJC1 first
        //If either track fails this requirement we reject the candidate
        
        if (ProtonH1Track->GetStartRadius() > 35 || PionH1Track->GetStartRadius() > 35) {
            breakloopcount++;
            continue;
        }
        LambdaCutProgression[3]->Fill(invMasslambda);

        
        
        
        //Reject tracks that are primary vertex fitted with H1PartSelTrack
        //if (ProtonPSTrack->IsFromSecondary() != true || PionPSTrack->IsFromSecondary() != true) {
            //breakloopcount++;
            //continue;
        //}
        //LambdaCutProgression[4]->Fill(invMasslambda);





        //Check for duplicate tracks with H1Track class
        int H1TrackFlag = 0;
        for (Int_t a = 0; a < ProtonH1TrackCheck.size(); a++) {

            //Have to reget H1Track of previously selected candidate to compare with the current one
            H1Track* ProtonH1TrackPrevious = (H1Track*)LambdaCand[ProtonH1TrackCheck[a]]->GetProtonTrack(); //This is casting H1CentralFittedTrack h1oo class as H1Track for daughter
            H1Track* PionH1TrackPrevious = (H1Track*)LambdaCand[PionH1TrackCheck[a]]->GetPionTrack(); //This is casting H1CentralFittedTrack h1oo class as H1Track for daughter

            //H1Track &ProtonH1TrackPreviousnonpointer = ProtonH1TrackPrevious;
            //H1Track &PionH1TrackPreviousnonpointer = PionH1TrackPrevious;

            if (ProtonH1Track->IsSameAs(*ProtonH1TrackPrevious) == true) { H1TrackFlag = 1; }
            if (PionH1Track->IsSameAs(*PionH1TrackPrevious) == true) { H1TrackFlag = 1; }

        }

        if (H1TrackFlag == 1) {
            breakloopcount++;
            continue;
        }
        LambdaCutProgression[5]->Fill(invMasslambda);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //pT Track cut on both proton and pion track
        /*if (LambdaCand[loop]->GetProtonPt() < 0.12 || LambdaCand[loop]->GetPionPt() < 0.12) {
            breakloopcount++;
            continue;
        }
        LambdaCutProgression[6]->Fill(invMasslambda);*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //PID Likelihood Cut for Proton
        /*if (ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton) < 0.001) { 
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[7]->Fill(invMasslambda);*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //PID Likelihood Cut for Pion
       /* if (PionPSTrack->GetDedxLikelihood(H1Dedx::kPion) < 0.001) {
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[8]->Fill(invMasslambda);*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////        
        //DCA over dDCA proton cut
        /*if (abs((protonTrajectory->GetDca(V2PV)) / (protonTrajectory->GetdDca(V2PV))) < 1) { 
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[9]->Fill(invMasslambda);*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //DCA over dDCA pion cut
        /*if (abs((pionTrajectory->GetDca(V2PV)) / (pionTrajectory->GetdDca(V2PV))) < 1) { 
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[10]->Fill(invMasslambda);*/

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Cut against K0 contamination
        invMasskaonCont = (ProtonH1Track->GetFourVector(mPion)
            + PionH1Track->GetFourVector(mPion)).M();
       /* if (invMasskaonCont > 0.465 && invMasskaonCont < 0.53) { 
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[11]->Fill(invMasslambda);*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Cut against photon contamination
        invMassphotonCont = (ProtonH1Track->GetFourVector(mElectron)
            + PionH1Track->GetFourVector(mElectron)).M();
       /* if (invMassphotonCont < 0.05) {
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[12]->Fill(invMasslambda);*/


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
        weight1 = *w1;
        weight2 = *w2;
        SVx = sv.X();
        SVy = sv.Y();
        SVz = sv.Z();
        PVx = pv.X();
        PVy = pv.Y();
        PVz = pv.Z();
        LambdaPX = LambdaP.X();
        LambdaPY = LambdaP.Y();
        LambdaPZ = LambdaP.Z();
        s61 = gH1Calc->Trig()->GetL1ac(61);
        s1 = gH1Calc->Trig()->GetL1ac(1);
        s74 = gH1Calc->Trig()->GetL1ac(74);
        s74 = gH1Calc->Trig()->GetL1ac(75);
        s74 = gH1Calc->Trig()->GetL1ac(67);
        Phi = *ElecPhi;
        
        ProtonTrackEndRadius = LambdaCand[loop]->GetProtonTrack()->GetCentralTrack()->GetEndRadius();
        PionTrackEndRadius = LambdaCand[loop]->GetPionTrack()->GetCentralTrack()->GetEndRadius();
        ProtonTrackEndMomentum = LambdaCand[loop]->GetProtonTrack()->GetCentralTrack()->GetTrajectoryEnd()->GetMomentum();
        PionTrackEndMomentum = LambdaCand[loop]->GetPionTrack()->GetCentralTrack()->GetTrajectoryEnd()->GetMomentum();
        ProtonTrackV0Momentum = LambdaCand[loop]->GetProtonFourVector().Vect();
        PionTrackV0Momentum = LambdaCand[loop]->GetPionFourVector().Vect();
        //cout << "Electron Mass: " << mElectron << endl;
        //cout << "Proton Mass: " << mProton << endl;
        //cout << "Electron Beam: " << *EBeamE << endl;
        //cout << "Proton Beam: " << *EBeamP << endl;
        
        //TLorentzVector (x,y,z,t)
        //Redefining H1 coordinates, they have proton in +z direction by convention
        //To make comparison with theories I will make electron +z direction and proton -z direction
        TLorentzVector e_initial(0,0,sqrt((*EBeamE) * (*EBeamE) - mElectron * mElectron), *EBeamE); // e beam four vector in lab frame
        //cout << "E p: " << sqrt((*EBeamE) * (*EBeamE) - mElectron * mElectron) << endl;
        //cout << "E E: " << *EBeamE << endl;
        TLorentzVector e_final(-(* ElecE)*sin(*ElecTheta)*cos(*ElecPhi), -(*ElecE)* sin(*ElecTheta)*sin(*ElecPhi), -(*ElecE)* cos(*ElecTheta), *ElecE); //measured scattered electron four vector in lab frame
        scatEX = -(*ElecE) * sin(*ElecTheta) * cos(*ElecPhi);
        scatEY = -(*ElecE) * sin(*ElecTheta) * sin(*ElecPhi);
        scatEZ = -(*ElecE) * cos(*ElecTheta);
        TLorentzVector photon = e_initial - e_final;// virtual photon four vector in lab frame
        TLorentzVector proton_target(0, 0, -sqrt((*EBeamP) * (*EBeamP) - mProton * mProton), *EBeamP); // Proton beam in the lab frame        
        //cout << "Proton Z: " << proton_target.Z() << endl;
        //cout << "Proton E: " << proton_target.E() << endl;
        TVector3 Boost = (photon + proton_target).BoostVector();
        //TLorentzVector lambda4vector = LambdaCand[loop]->GetFourVector();
        TLorentzVector lambda4vector(LambdaCand[loop]->GetFourVector().X(), LambdaCand[loop]->GetFourVector().Y(),-1*( LambdaCand[loop]->GetFourVector().Z()), LambdaCand[loop]->GetFourVector().E());
        //cout << lambda4vector.Z() << endl;
        TLorentzVector Boostedlambda4vector = lambda4vector;
        Boostedlambda4vector.Boost(-Boost);
        ylab = lambda4vector.Rapidity();
        ystar = Boostedlambda4vector.Rapidity(); //Takes lambda four vector from lab frame into HCM frame and calculates the rapidity
        //cout << "neg" << endl;
        

        //Below is unique braches to the LambdaTotTree compared to Reg and Bar Trees
        //lambdaTotTree->Branch("PionCharge", &PionCharge, "PionCharge/D");
        PionCharge = LambdaCand[loop]->GetPionCharge();
        //lambdaTotTree->Branch("ProtonCharge", &ProtonCharge, "ProtonCharge/D");
        ProtonCharge = LambdaCand[loop]->GetProtonCharge();
        //lambdaTotTree->Branch("FRUNZ", &FRUNZ, "FRUNZ/D");
        pionFRUNZ = LambdaCand[loop]->GetPionTrack()->GetUnusedZHitFraction();
        protonFRUNZ = LambdaCand[loop]->GetProtonTrack()->GetUnusedZHitFraction();
        //lambdaTotTree->Branch("PionPt", &PionPt, "PionPt/D");
        PionPt = LambdaCand[loop]->GetPionPt();
        //lambdaTotTree->Branch("ProtonPt", &ProtonPt, "ProtonPt/D");
        ProtonPt = LambdaCand[loop]->GetProtonPt();
        //lambdaTotTree->Branch("PionTrackStartRadius", &PionTrackStartRadius, "PionTrackStartRadius/D");
        PionTrackStartRadius = PionH1Track->GetStartRadius();
        //lambdaTotTree->Branch("ProtonTrackStartRadius", &ProtonTrackStartRadius, "ProtonTrackStartRadius/D");
        ProtonTrackStartRadius = ProtonH1Track->GetStartRadius();
        //lambdaTotTree->Branch("PionL", &PionL, "PionL/D");
        PionNHitsdEdx = LambdaCand[loop]->GetPionNHitsdEdx();
        ProtonNHitsdEdx = LambdaCand[loop]->GetProtonNHitsdEdx();
        PiondEdx = LambdaCand[loop]->GetPiondEdx();
        ProtondEdx = LambdaCand[loop]->GetProtondEdx();
        //
        PionL = PionPSTrack->GetDedxLikelihood(H1Dedx::kPion);
        PionLProton = PionPSTrack->GetDedxLikelihood(H1Dedx::kProton);
        PionLelectron = PionPSTrack->GetDedxLikelihood(H1Dedx::kElectron);
        //lambdaTotTree->Branch("ProtonL", &ProtonL, "ProtonL/D");
        ProtonL = ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton);
        ProtonLPion = ProtonPSTrack->GetDedxLikelihood(H1Dedx::kPion);
        ProtonLelectron = ProtonPSTrack->GetDedxLikelihood(H1Dedx::kElectron);
        //lambdaTotTree->Branch("PionTrackLength", &PionTrackLength, "PionTrackLength/D");
        //lambdaTotTree->Branch("ProtonTrackLength", &ProtonTrackLength, "ProtonTrackLength/D");
        PionTrackLength = LambdaCand[loop]->GetPionTrack()->GetLength();
        ProtonTrackLength = LambdaCand[loop]->GetProtonTrack()->GetLength();
        //lambdaTotTree->Branch("PionDCA", &PionDCA, "PionDCA/D");
        //lambdaTotTree->Branch("ProtonDCA", &ProtonDCA, "ProtonDCA/D");
        PionDCA = pionTrajectory->GetDca(V2PV);
        ProtonDCA = protonTrajectory->GetDca(V2PV);
        //lambdaTotTree->Branch("PionSDCA", &PionSDCA, "PionSDCA/D");
        //lambdaTotTree->Branch("ProtonSDCA", &ProtonSDCA, "ProtonSDCA/D");
        PionSDCA = abs((pionTrajectory->GetDca(V2PV)) / (pionTrajectory->GetdDca(V2PV)));
        ProtonSDCA = abs((protonTrajectory->GetDca(V2PV)) / (protonTrajectory->GetdDca(V2PV)));
        //lambdaTotTree->Branch("Chi2", &Chi2, "Chi2/D");
        Chi2 = LambdaCand[loop]->GetV0()->GetFitChi2();
        //lambdaTotTree->Branch("MassPionPion", &MassPionPion, "MassPionPion/D");
        //lambdaTotTree->Branch("MassElecElec", &MassElecElec, "MassElecElec/D");
        MassPionPion = invMasskaonCont;
        MassElecElec = invMassphotonCont;
        //lambdaTotTree->Branch("pTrelPion", &pTrelPion, "pTrelPion/D");
        //lambdaTotTree->Branch("pTrelProton", &pTrelProton, "pTrelProton/D");
        pTrelPion = LambdaCand[loop]->GetPionPtRel();
        pTrelProton = LambdaCand[loop]->GetProtonPtRel();
        //lambdaTotTree->Branch("RadiusSVtoPV", &RadiusSVtoPV, "RadiusSVtoPV/D");
        RadiusSVtoPV = LambdaCand[loop]->GetV0()->GetLength();
        //lambdaTotTree->Branch("IsBar", &IsBar, "IsBar/I");
        IsBar = LambdaCand[loop]->IsLambdaBar();
        DeltaDCA= LambdaCand[loop]->GetDeltaDca();
        DecayLength= LambdaCand[loop]->GetDecayLength();
        CosThetaStar = LambdaCand[loop]->GetCosThetaStar();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        //Eta Cut
//        if (abs(Eta) > 1.3) {
//            breakloopcount++;
//            continue;
//        }
//        
//        LambdaCutProgression[7]->Fill(invMasslambda);
//        
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        //pT Cut
//        if (pT < 0.5 || pT > 3.5) {
//            breakloopcount++;
//            continue;
//        }
//            
//        LambdaCutProgression[8]->Fill(invMasslambda);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Need to Apply some further cuts on the tracks of the Lambda daughters
        //Cut on distance maximal radial distance of first CJC hit
        //requiring this to be < 35 cm will ensure our tracks are measureed in the CJC1 first
        //If either track fails this requirement we reject the candidate
        /*if (ProtonH1Track->GetStartRadius()>35 || PionH1Track->GetStartRadius()>35) {
            breakloopcount++;
            RejectedRadTrack->Fill(invMasslambda);
            continue;
        }



        LambdaCutProgression[9]->Fill(invMasslambda);*/

        //Strengthening our track pT requirement, require pT of all tracks to be >0.12GeV as in 2009 measurement we are replicating
        //Defualt is 0.2GeV for proton and 0.1GeV for pion
        //This should have a small effect, will only reject some pion tracks
        /*if (LambdaCand[loop]->GetProtonPt()<0.12 || LambdaCand[loop]->GetPionPt()<0.12) {
            breakloopcount++;
            RejectedTrackpT->Fill(invMasslambda);
            continue;
        }
        LambdaCutProgression[10]->Fill(invMasslambda);*/

        ////Reject tracks that are primary vertex fitted with H1PartSelTrack
        //if (ProtonPSTrack->IsFromSecondary()!=true || PionPSTrack->IsFromSecondary()!=true) {
        //    breakloopcount++;
        //    RejectedVertexFitted->Fill(invMasslambda);
        //    continue;
        //}
        //LambdaCutProgression[11]->Fill(invMasslambda);

        //Reject tracks that are primary vertex fitted with H1PartSelTrack
        //Try casting Tracks to H1NonVertexFittedTrack class
        /*H1NonVertexFittedTrack* ProtonH1NonFittedVertexTrack = (H1NonVertexFittedTrack*)LambdaCand[loop]->GetProtonTrack();
        H1NonVertexFittedTrack* PionH1NonFittedVertexTrack = (H1NonVertexFittedTrack*)LambdaCand[loop]->GetPionTrack();

        if (!ProtonH1NonFittedVertexTrack || !PionH1NonFittedVertexTrack) {
            breakloopcount++;
            RejectedVertexFitted->Fill(invMasslambda);
            continue;
        }
        LambdaCutProgression[11]->Fill(invMasslambda);*/


        //Make sure CJC1 has at least a hit
        // This is just a sanity check cut, this should reject nothing, the work is already done by requring the first track radial hit to be within 35 cm
        //cout << "Proton Track CJC1 Hits: " << ProtonH1Track->GetNHit(H1Track::tdCJC1) << endl;
        //cout << "Pion Track CJC1 Hits: " << PionH1Track->GetNHit(H1Track::tdCJC1) << endl;
        /*if (ProtonH1Track->GetNHit(H1Track::tdCJC1) < 1 || PionH1Track->GetNHit(H1Track::tdCJC1) < 1) {
            breakloopcount++;
            continue;
        }*/
        LambdaCutProgression[13]->Fill(invMasslambda);

        ////Check for duplicate tracks with H1Track class
        //int H1TrackFlag = 0;
        //for (Int_t a = 0; a < ProtonH1TrackCheck.size(); a++) {

        //    //Have to reget H1Track of previously selected candidate to compare with the current one
        //    H1Track* ProtonH1TrackPrevious = (H1Track*)LambdaCand[ProtonH1TrackCheck[a]]->GetProtonTrack(); //This is casting H1CentralFittedTrack h1oo class as H1Track for daughter
        //    H1Track* PionH1TrackPrevious = (H1Track*)LambdaCand[PionH1TrackCheck[a]]->GetPionTrack(); //This is casting H1CentralFittedTrack h1oo class as H1Track for daughter

        //    //H1Track &ProtonH1TrackPreviousnonpointer = ProtonH1TrackPrevious;
        //    //H1Track &PionH1TrackPreviousnonpointer = PionH1TrackPrevious;

        //    if (ProtonH1Track->IsSameAs(*ProtonH1TrackPrevious)==true) { H1TrackFlag = 1; }
        //    if (PionH1Track->IsSameAs(*PionH1TrackPrevious)==true) { H1TrackFlag = 1; }

        //}

        //if (H1TrackFlag == 1) {
        //    breakloopcount++;
        //    RejectedH1Track->Fill(invMasslambda);
        //    continue;
        //}
        //LambdaCutProgression[13]->Fill(invMasslambda);



        //passed all cuts except for final sanity check, pass H1Tracks into vector of initial tracks
        ProtonH1TrackCheck.push_back(loop);
        PionH1TrackCheck.push_back(loop);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //We passed all of the initial cuts, now we have to check if we have already used the proton track
        //this is a sanity check, these split tracks should be rejected by the CJC1 requirement
        //This is a sanity check, this cut should reject nothing
        int protontrackcheck = singlefind(ProtonTrackCheck, 0, ProtonTrackCheck.size(), LambdaCand[loop]->GetProton()->GetPt());
        int piontrackcheck = singlefind(PionTrackCheck, 0, PionTrackCheck.size(), LambdaCand[loop]->GetPion()->GetPt());

        //if both of these singlefindfunctions return -1, then these tracks have not been used
        //in the case these are the first particle accepted and the reference list is empty, it will also retun -1
        //if one of them gets a valus of -1 we should skip that event, because it is reusing a track
        particlesinevent++;

        ///HERE
       /* if (particlesinevent > 1) {
            cout << "milti particle event" << endl;
        }*/


        if (protontrackcheck != -1){
            //cout << "Redundant proton track" << endl;
            
        }
        if (piontrackcheck != -1) {
            //cout << "Redundant pion track" << endl; 
            
        }
        if (protontrackcheck != -1 || piontrackcheck != -1) {
            //cout << "Duplicate Track Used!" << endl;
            breakloopcount++;
            continue; 
        }



        LambdaCutProgression[14]->Fill(invMasslambda);

        //Cut on Z position of Lambda Decay vertex
        //if (abs(sv.Z())>30) {
        //    //cout << "Duplicate Track Used!" << endl;
        //    breakloopcount++;
        //    continue;
        //}
        LambdaCutProgression[15]->Fill(invMasslambda);

        //now the particles have passed all checks, we need to update the track reference list
        ProtonTrackCheck.push_back(LambdaCand[loop]->GetProton()->GetPt());
        PionTrackCheck.push_back(LambdaCand[loop]->GetPion()->GetPt());

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//         Candidates Passed all cuts, so now we can compare reconstructed pT of candidate with generated pTs to try and see which generated Lambda we reconstructed
//         The candidate with measured pT closest to generated pT is assumed to be the correct one
        FoundGen = 0; //Initialize at zero, if this is MC and we can assocaite reconstructed lambda with a gen one we can tag this as true
        //RunType == 0 is data, anything else is MC
        if (fRunType != 0) {
            //cout << "Event: " << eventCounter << endl;
//cout << "There are " << GtrLambdaRegIndex.size() << " Reg Lambdas Generated in this Event: " << endl;
//cout << "There are " << GtrLambdaBarIndex.size() << " Lambda Bars Generated in this Event: " << endl;
            GtrAngleLambdaBar = {};//initializing these vectors
            GtrAngleLambdaReg = {};
//If this candidate is a Lambda Bar, and the event has a generated lambda bar, try to find which generated lambda we found by comparing reconstructed and generated momentums
            if (LambdaCand[loop]->IsLambdaBar() == true && GtrLambdaBarIndex.size() != 0) {
                for (Int_t k = 0; k < GtrLambdaBarIndex.size(); k++) {
                    H1PartMC* part = gtrdata[GtrLambdaBarIndex[k]];
                    GtrAngleLambdaBar.push_back((180 / (TMath::Pi())) * acos((part->GetMomentum().Dot(LambdaP)) / ((part->GetMomentum().Mag()) * (LambdaP.Mag()))));
                    //cout << "Lambda Bar Angles: " << (180 / (TMath::Pi())) * acos((part->GetMomentum().Dot(LambdaP)) / ((part->GetMomentum().Mag()) * (LambdaP.Mag()))) << endl;
                }
                //cout << "The generated lambda bar angle is: " << GtrAngleLambdaBar[minfinder(GtrAngleLambdaBar)] << endl;
                //cout << "The generated lambda bar angle is: " << (180 / (TMath::Pi())) * acos((gtrdata[GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)]]->GetMomentum().Dot(LambdaP)) / ((gtrdata[GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)]]->GetMomentum().Mag()) * (LambdaP.Mag()))) << endl;
                int genLambdaIndex = GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)];
                //
                vector<int> LambdaDaughterIndices = {}; //initialize container for all stored daughter indices
                for (Int_t j = 0; j < gtrdata.GetEntries(); j++) {
                    H1PartMC* part2 = gtrdata[j];
                    if (part2->IsStr() == false) { continue; } //Decay products of lambdas only exist on STR level, if particle is not STR then skip
                    if (part2->GetMother2() != -1) { continue; } //particles should only have 1 mother, second mother index then should be -1
                    if (part2->GetMother1() == genLambdaIndex) { //If particle in secondary loop is claiming to come from the lambda we identified in the primary loop, then we can add it to LambdaDaughterIndices vector
                        LambdaDaughterIndices.push_back(j);
                    }
                }

                if (LambdaDaughterIndices.size() == 0) {
                    SVzgen = 0;
                    SVxgen = 0;
                    SVygen = 0;
                }
                if (LambdaDaughterIndices.size() != 0) {
                    SVzgen = StrBank[gtrdata[LambdaDaughterIndices[0]]->GetStrIdx()]->GetSvx()->GetZ();
                    SVxgen = StrBank[gtrdata[LambdaDaughterIndices[0]]->GetStrIdx()]->GetSvx()->GetX();
                    SVygen = StrBank[gtrdata[LambdaDaughterIndices[0]]->GetStrIdx()]->GetSvx()->GetY();

                    //cout << "Z Decay : " << DecayZVtx << endl;
                    //cout << "X Decay : " << DecayXVtx << endl;
                    //cout << "Y Decay : " << DecayYVtx << endl;
                }

                int havesimulatedvertex = 0;
                PVzgen = 0;
                //double Zvtxgen;
                for (int v = 0; v < vertexgen.GetEntries(); v++) {
                    Int_t type = vertexgen[v]->GetVertexType();
                    //if (type == 0) {
                    //    havePrimaryVertex = true;
                    //    primaryVertex = vertex[v]->GetPosition();
                    //}
                    if (type == 1) {
                        havesimulatedvertex = 1;
                        PVzgen = vertexgen[v]->GetPosition().Z();
                    }
                }
              
                pTgen = gtrdata[genLambdaIndex]->GetPt();
                Etagen = gtrdata[genLambdaIndex]->GetEta();
                Q2sgen = *q2sgen;
                Ysgen = *ysgen;
                xgen = ((*GenEnElec) / (*EBeamP)) * (cos((*GenThElec) / (2))) * (cos((*GenThElec) / (2))) / (*ysgen);
                genrecAngle = GtrAngleLambdaBar[minfinder(GtrAngleLambdaBar)];

                //cout << "Rec Lambda Bar pT: " << pT << endl;
                //cout << "Gen Lambda Bar pT: " << pTgen << endl;

                lambdaBarGenTree->Fill();
                lambdaTotGenTree->Fill();

                //if (BGcheck(part, gtrdata, strbank) == 1) { continue; }
                //Now I want to check which Lambdas we reconstruct that pass all cuts are coming from detector interactions, these lambdas will fill Lambda BG
                if (BGcheck(gtrdata[genLambdaIndex], gtrdata, StrBank) == 1) {
                    lambdaBarBGTree->Fill();
                    lambdaTotBGTree->Fill();
                }
                    
                
            }

            //If this candidate is a Regular Lambda, and the event has a generated regular lambda, try to find which generated lambda we found by comparing reconstructed and generated momentums
            if (LambdaCand[loop]->IsLambdaBar() != true && GtrLambdaRegIndex.size() != 0) {
                for (Int_t k = 0; k < GtrLambdaRegIndex.size(); k++) {
                    H1PartMC* part = gtrdata[GtrLambdaRegIndex[k]];
                    GtrAngleLambdaReg.push_back((180 / (TMath::Pi())) * acos((part->GetMomentum().Dot(LambdaP)) / ((part->GetMomentum().Mag()) * (LambdaP.Mag()))));
                    //cout << "Lambda Reg Angles: " << (180 / (TMath::Pi())) * acos((part->GetMomentum().Dot(LambdaP)) / ((part->GetMomentum().Mag()) * (LambdaP.Mag()))) << endl;
                }
                //cout << "The generated lambda reg angle is: " << GtrAngleLambdaReg[minfinder(GtrAngleLambdaReg)] << endl;
                //cout << "The generated lambda reg angle is: " << (180 / (TMath::Pi())) * acos((gtrdata[GtrLambdaRegIndex[minfinder(GtrAngleLambdaReg)]]->GetMomentum().Dot(LambdaP)) / ((gtrdata[GtrLambdaRegIndex[minfinder(GtrAngleLambdaReg)]]->GetMomentum().Mag()) * (LambdaP.Mag()))) << endl;
                int genLambdaIndex = GtrLambdaRegIndex[minfinder(GtrAngleLambdaReg)];
                //
                vector<int> LambdaDaughterIndices = {}; //initialize container for all stored daughter indices
                for (Int_t j = 0; j < gtrdata.GetEntries(); j++) {
                    H1PartMC* part2 = gtrdata[j];

                    if (part2->IsStr() == false) { continue; } //Decay products of lambdas only exist on STR level, if particle is not STR then skip
                    if (part2->GetMother2() != -1) { continue; } //particles should only have 1 mother, second mother index then should be -1
                    if (part2->GetMother1() == genLambdaIndex) { //If particle in secondary loop is claiming to come from the lambda we identified in the primary loop, then we can add it to LambdaDaughterIndices vector
                        LambdaDaughterIndices.push_back(j);
                    }
                }

                if (LambdaDaughterIndices.size() == 0) {
                    SVzgen = 0;
                    SVxgen = 0;
                    SVygen = 0;
                }
                if (LambdaDaughterIndices.size() != 0) {
                    SVzgen = StrBank[gtrdata[LambdaDaughterIndices[0]]->GetStrIdx()]->GetSvx()->GetZ();
                    SVxgen = StrBank[gtrdata[LambdaDaughterIndices[0]]->GetStrIdx()]->GetSvx()->GetX();
                    SVygen = StrBank[gtrdata[LambdaDaughterIndices[0]]->GetStrIdx()]->GetSvx()->GetY();

                    //cout << "Z Decay : " << DecayZVtx << endl;
                    //cout << "X Decay : " << DecayXVtx << endl;
                    //cout << "Y Decay : " << DecayYVtx << endl;
                }

                int havesimulatedvertex = 0;
                PVzgen = 0;
                //double Zvtxgen;
                for (int v = 0; v < vertexgen.GetEntries(); v++) {
                    Int_t type = vertexgen[v]->GetVertexType();
                    //if (type == 0) {
                    //    havePrimaryVertex = true;
                    //    primaryVertex = vertex[v]->GetPosition();
                    //}
                    if (type == 1) {
                        havesimulatedvertex = 1;
                        PVzgen = vertexgen[v]->GetPosition().Z();
                    }
                }

                pTgen = gtrdata[genLambdaIndex]->GetPt();
                Etagen = gtrdata[genLambdaIndex]->GetEta();
                Q2sgen = *q2sgen;
                Ysgen = *ysgen;
                xgen = ((*GenEnElec) / (*EBeamP)) * (cos((*GenThElec) / (2))) * (cos((*GenThElec) / (2))) / (*ysgen);
                Phigen = *GenPhElec;
                //cout << "Rec Lambda Reg pT: " << pT << endl;
                //cout << "Gen Lambda Reg pT: " << pTgen << endl;
                genrecAngle = GtrAngleLambdaReg[minfinder(GtrAngleLambdaReg)];

                lambdaRegGenTree->Fill();
                lambdaTotGenTree->Fill();

                //if (BGcheck(part, gtrdata, strbank) == 1) { continue; }
                //Now I want to check which Lambdas we reconstruct that pass all cuts are coming from detector interactions, these lambdas will fill Lambda BG
                if (BGcheck(gtrdata[genLambdaIndex], gtrdata, StrBank) == 1) {
                    lambdaRegBGTree->Fill();
                    lambdaTotBGTree->Fill();
                }

            }
        }
        
        PionNuclCorrWeight = 1.0; //initialize this
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Now before I add data to the Trees, I want to find the H1NuclIntACorr weight to add for the pion daughter track (Correction for proton tracks does not exist currently) 
        if (fRunType != 0) { //If this is MC Data
            //cout << "Event: " << eventCounter << endl;
//cout << "There are " << GtrLambdaRegIndex.size() << " Reg Lambdas Generated in this Event: " << endl;
//cout << "There are " << GtrLambdaBarIndex.size() << " Lambda Bars Generated in this Event: " << endl;
            GtrAnglePosPionLambdaBar = {};//initializing these vectors
            GtrAngleNegPionLambdaReg = {};

            int havesimulatedvertex = 0;
            PVzgen = 0;
            //double Zvtxgen;
            for (int v = 0; v < vertexgen.GetEntries(); v++) {
                Int_t type = vertexgen[v]->GetVertexType();
                //if (type == 0) {
                //    havePrimaryVertex = true;
                //    primaryVertex = vertex[v]->GetPosition();
                //}
                if (type == 1) {
                    havesimulatedvertex = 1;
                    PVzgen = vertexgen[v]->GetPosition().Z();
                }
            }

            if (LambdaCand[loop]->IsLambdaBar() == true && PionNucIntCorrLambdaBarIndex.size() != 0) {

                for (Int_t k = 0; k < PionNucIntCorrLambdaBarIndex.size(); k++) {
                    H1PartMC* part = gtrdata[PionNucIntCorrLambdaBarIndex[k]];
                    GtrAnglePosPionLambdaBar.push_back((180 / (TMath::Pi())) * acos((part->GetMomentum().Dot(vecPion)) / ((part->GetMomentum().Mag()) * (vecPion.Mag()))));
                }
                //cout << "The generated lambda bar angle is: " << GtrAngleLambdaBar[minfinder(GtrAngleLambdaBar)] << endl;
                //cout << "The generated lambda bar angle is: " << (180 / (TMath::Pi())) * acos((gtrdata[GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)]]->GetMomentum().Dot(LambdaP)) / ((gtrdata[GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)]]->GetMomentum().Mag()) * (LambdaP.Mag()))) << endl;
                int genPosPionLambdaBarIndex = PionNucIntCorrLambdaBarIndex[minfinder(GtrAnglePosPionLambdaBar)];
                //Float_t GetWeight(int ipart, float charge, float pt, float phi, float theta, float dca, float z0)
                double tempweight = H1NuclIACor::GetWeight(1, 1.0, gtrdata[genPosPionLambdaBarIndex]->GetFourVector().Vect().Pt(), gtrdata[genPosPionLambdaBarIndex]->GetFourVector().Vect().Phi(), gtrdata[genPosPionLambdaBarIndex]->GetFourVector().Vect().Theta(), 0.0, PVzgen);
                PionNuclCorrWeight = tempweight;
                if (std::isnan(PionNuclCorrWeight) == 1) { cout << "Bad Weight" << endl; }
                //cout << "Pos Pion Rec pT: " << vecPion.Pt() << endl;
                //cout << "Pos Pion Gen pT: " << gtrdata[genPosPionLambdaBarIndex]->GetPt() << endl;
                //cout << "Pos Pion Weight: " << tempweight << endl;
            }

            if (LambdaCand[loop]->IsLambdaBar() != true && PionNucIntCorrLambdaRegIndex.size() != 0) {

                for (Int_t k = 0; k < PionNucIntCorrLambdaRegIndex.size(); k++) {
                    H1PartMC* part = gtrdata[PionNucIntCorrLambdaRegIndex[k]];
                    GtrAngleNegPionLambdaReg.push_back((180 / (TMath::Pi())) * acos((part->GetMomentum().Dot(vecPion)) / ((part->GetMomentum().Mag()) * (vecPion.Mag()))));
                }
                //cout << "The generated lambda bar angle is: " << GtrAngleLambdaBar[minfinder(GtrAngleLambdaBar)] << endl;
                //cout << "The generated lambda bar angle is: " << (180 / (TMath::Pi())) * acos((gtrdata[GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)]]->GetMomentum().Dot(LambdaP)) / ((gtrdata[GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)]]->GetMomentum().Mag()) * (LambdaP.Mag()))) << endl;
                int genNegPionLambdaRegIndex = PionNucIntCorrLambdaRegIndex[minfinder(GtrAngleNegPionLambdaReg)];
               //Float_t GetWeight(int ipart, float charge, float pt, float phi, float theta, float dca, float z0)
                double tempweight = H1NuclIACor::GetWeight(1, -1.0, gtrdata[genNegPionLambdaRegIndex]->GetFourVector().Vect().Pt(), gtrdata[genNegPionLambdaRegIndex]->GetFourVector().Vect().Phi(), gtrdata[genNegPionLambdaRegIndex]->GetFourVector().Vect().Theta(), 0.0, PVzgen);
                PionNuclCorrWeight = tempweight;
                if (std::isnan(PionNuclCorrWeight) == 1) { cout << "Bad Weight" << endl; }
                //cout << "Neg Pion Rec pT: " << vecPion.Pt() << endl;
                //cout << "Neg Pion Gen pT: " << gtrdata[genNegPionLambdaRegIndex]->GetPt() << endl;
                //cout << "Pos Pion Weight: " << tempweight << endl;
            }

        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // STORE LAMBDA CANDIDATES INTO TTREES
        passedallcutscount++;
        LambdaCount++;
        lambdaTotTree->Fill();

        if (LambdaCand[loop]->IsLambdaBar() == true) { 
            lambdaBarTree->Fill(); 
        }

        if (LambdaCand[loop]->IsLambdaBar() != true) { 
            lambdaRegTree->Fill(); 
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //LAMBDA C 
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        vector<Int_t> PionCH1TrackCheck = {};
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

            // Armenteros Plot and pT_rel cut calculations

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
            double qlLambdaC = vecLambdaC.Dot(LambdaCand[loop]->GetFourVector().Vect()) / vecLambdaC.Mag();
            double alphaC = (2 * (qlPionC)) / (vecLambdaC.Mag()) - 1;

            //pT cut for Lambda C
            //if (PST[loop2]->GetPt() < 0.2) { continue; }
            //if ((sqrt(vecLambdaC.X() * vecLambdaC.X() + vecLambdaC.Y() * vecLambdaC.Y())) < 1.7) { continue; }

            if (PST[loop2]->GetDcaPrime() > 0.25) { continue; }

            //hC4->Fill(invMasslambdaC);
            LambdaCCutProgression[7]->Fill(invMasslambdaC);


            //ensuring the larger longitudinal momentum with respect to charm lambda is the lambda, skip if not
            //if (qlPionC > qlLambdaC) { continue; }
            //hC5->Fill(invMasslambdaC);

            if (abs(PST[loop2]->GetEta()) > 1.8) { continue; }

            LambdaCCutProgression[8]->Fill(invMasslambdaC);

            hAPbeforeC->Fill(alphaC, pTrelC);

            //Calculations to cut on Lambda Candidate vertex position
            //we only care about lambdas that come from the primary vertex
            H1SelVertex* ProtonVertex = (H1SelVertex*)LambdaCand[loop]->GetProtonTrack();//Get the proton secondary vertex, which should be the spot the Lambda decay took place
            TVector3 ProtonVertexPosVec = ProtonVertex->GetPosition();
            double ProtonPosX = sv.X();
            double ProtonPosY = sv.Y();
            double ProtonPosZ = sv.Z();


            //cout << "Total Decay Length: " << sqrt((pv.X() - ProtonVertexPosVec.X()) * (pv.X() - ProtonVertexPosVec.X()) + (pv.Y() - ProtonVertexPosVec.Y()) * (pv.Y() - ProtonVertexPosVec.Y()) + (pv.Z() - ProtonVertexPosVec.Z()) * (pv.Z() - ProtonVertexPosVec.Z())) << endl;

            pdecaylengthmag->Fill(sqrt((pv.X() - ProtonPosX) * (pv.X() - ProtonPosX) + (pv.Y() - ProtonPosY) * (pv.Y() - ProtonPosY) + (pv.Z() - ProtonPosZ) * (pv.Z() - ProtonPosX)));

            //if (sqrt((pv.X() - ProtonPosX) * (pv.X() - ProtonPosX) + (pv.Y() - ProtonPosY) * (pv.Y() - ProtonPosY) + (pv.Z() - ProtonPosZ) * (pv.Z() - ProtonPosX)) > 8) { continue; }

            //cout << "X: " << ProtonPosX << endl;
            //cout << "Y: " << ProtonPosY << endl;
            //cout << "Z: " << ProtonPosZ << endl;

            pXYvtxdensitytot->Fill(pv.X() - ProtonPosX, pv.Y() - ProtonPosY);
            pZXvtxdensitytot->Fill(pv.Z() - ProtonPosZ, pv.X() - ProtonPosX);
            pZYvtxdensitytot->Fill(pv.Z() - ProtonPosZ, pv.Y() - ProtonPosY);

            //if (pTrelC < 0.3 || pTrelC > 0.88) { continue; } //pT,rel cut for Armenteros Plot
            if (pT < 0.4) { continue; }
            LambdaCCutProgression[10]->Fill(invMasslambdaC);

            //if (pTrelC < 0.3) { continue; } //final cut
            if (PST[loop2]->GetPt() < 0.6) { continue; }
            LambdaCCutProgression[11]->Fill(invMasslambdaC);

            //If track is not within CJC1 reject this requirement we reject the candidate
            if (PST[loop2]->GetParticle()->GetTrack()->GetStartRadius() > 35) {
                continue;
            }
            LambdaCCutProgression[12]->Fill(invMasslambdaC);

            
            //Check for duplicate tracks within H1Track class
            int H1TrackFlag = 0;
            for (Int_t a = 0; a < PionCH1TrackCheck.size(); a++) {

                //Have to reget H1Track of previously selected candidate to compare with the current one
                //H1Track* PionCH1TrackPrevious = (H1Track*)LambdaCand[ProtonH1TrackCheck[a]]->GetProtonTrack();//edit this
                const H1Track* PionCH1TrackPrevious = PST[PionCH1TrackCheck[a]]->GetParticle()->GetTrack();

                if (PST[loop2]->GetParticle()->GetTrack()->IsSameAs(*PionCH1TrackPrevious) == true) { H1TrackFlag = 1; }
               
            }

            if (H1TrackFlag == 1) {
                continue;
            }

            LambdaCCutProgression[13]->Fill(invMasslambdaC);
            //end check for duplicate H1Track

            hAPafterC->Fill(alphaC, pTrelC);

            lambdaCTree->Fill();

            PionCH1TrackCheck.push_back(loop2);

        }//Secondary LambdaC loop
    
            
        }//loop over lambda candidate array
                
        //Outputting some consistency checks for passing and rejecting lambdas vs total entries
        //cout << "Passed All Cuts Count: " << passedallcutscount << endl;
        //cout << "Loop Break Counts: " << breakloopcount << endl;
        //cout << "Total Number of Lambda Entries: " << LambdaCand.GetEntries() << endl;
        
        if (passedallcutscount + breakloopcount != LambdaCand.GetEntries()) {
            cout << "Bad Count" << endl;
        }
        if (LambdaCount > 1) { eventswithMultipleLambdas++; }
        if (LambdaCount == 1) { eventswithOneLambda++; }

            eventCounter++;
            if (eventCounter % 1000 == 0) {
                cout << "Processing ODS event " << eventCounter << endl;
            }

    } // loop over events

        cout << "\n" << eventCounter << " events processed." << endl;
        cout << "Events with One Lambda Candidate Passing All Cuts: " << eventswithOneLambda << endl;
        cout << "Events with Multiple Lambda Candidates Passing All Cuts: " << eventswithMultipleLambdas << endl;
        ////////////////////////////////////////////////////////////////////
        //////////////END MAIN LOOP//////////////////////////////////////
        ////////////////////////////////////////////////////////////////////

      
       /////////////////////////////////////////////////////////////////////////////////////////////////////////////
       //Output File just for Lambda TTrees and few odd histograms as sanity checks
       
        TFile treefile(opts.GetOutput(), "RECREATE");
        
        
        for (int a = 0; a < 16; a++) {
            //LambdaCutProgression[a]->SetStats(0);
            //if (a == 9) { LambdaCutProgression[a]->SetLineColor(kBlack + 1); }
            //if (a != 9) { LambdaCutProgression[a]->SetLineColor(kBlack + a); }
            if(a>=0 && a<=8) { LambdaCutProgression[a]->SetLineColor(kBlack + a); }
            if (a>8) { LambdaCutProgression[a]->SetLineColor(kBlack + a - 9); }
            LambdaCutProgression[a]->GetXaxis()->SetTitle("Mass [GeV]");
            LambdaCutProgression[a]->GetYaxis()->SetTitle("Count");
            LambdaCutProgression[a]->Draw();
            LambdaCutProgression[a]->Write();

            hs->Add(LambdaCutProgression[a]);
        }

        for (int a = 0; a < 16; a++) {
            LambdaCCutProgression[a]->SetStats(0);
            LambdaCCutProgression[a]->SetLineColor(kBlack + a);
            LambdaCCutProgression[a]->GetXaxis()->SetTitle("Mass [GeV]");
            LambdaCCutProgression[a]->GetYaxis()->SetTitle("Count");
            LambdaCCutProgression[a]->Draw();
            LambdaCCutProgression[a]->Write();

            hsC->Add(LambdaCCutProgression[a]);
        }

        hs->Draw("nostack");
        canvas->SetName("Lambda Cut Stack");
        canvas->Write();

        hsC->Draw("nostack");
        canvas->SetName("LambdaC Cut Stack");
        canvas->Write();
        
       
       
       //Write all TTrees 
       lambdaTotTree->Write();
       lambdaRegTree->Write();
       lambdaBarTree->Write();
       lambdaCTree->Write();
       lambdaTotGenTree->Write();
       lambdaRegGenTree->Write();
       lambdaBarGenTree->Write();
       lambdaTotBGTree->Write();
       lambdaRegBGTree->Write();
       lambdaBarBGTree->Write();
       //Write and close TTree file
       treefile.Write();
       treefile.Close();

       cout << "TTrees written to " << opts.GetOutput() << endl;

       return 0;

} //main loop