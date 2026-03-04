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
#include "H1Pointers/H1IntPtr.h"
#include "H1Skeleton/H1EventList.h"
#include "H1Skeleton/H1FpackIndexManager.h"
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

    TTree* lambdaRegStrTree = new TTree("Lambda Reg STR", "Lambda Reg STR");
    TTree* lambdaBarStrTree = new TTree("Lambda Bar STR", "Lambda Bar STR");
    TTree* lambdaTotStrTree = new TTree("Lambda Tot STR", "Lambda Tot STR");
    //defining the variables I want to store in Lambda Trees, will save the TTrees and access branches later in a separate analysis code
    double pT, Eta, x, invMasslambda, Q2s, Ys, Angle, v0chi2, weight1, weight2, SVx, SVy, SVz, PVx, PVy, PVz, LambdaPX, LambdaPY, LambdaPZ, ystar, ylab, scatEX, scatEY, scatEZ;
    double PionPt, ProtonPt, PionTrackStartRadius, ProtonTrackStartRadius, PionL, ProtonL, PionCharge, ProtonCharge, PionTrackLength, ProtonTrackLength;
    double PionDCA, ProtonDCA, PionSDCA, ProtonSDCA, Chi2, MassPionPion, MassElecElec, pTrelPion, pTrelProton, RadiusSVtoPV;
    //defining variables needed only for MC Lambdas
    double genIvxX, genIvxY, genIvxZ, decayrecX, decayrecY, decayrecZ, strIVtype, FRUNZ;
    int fRunType;
    //Defining Tree Branches for Combined Lambdas, and seperate ones for Regular Lambdas vs Lambda Bars
    lambdaRegTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaRegTree->Branch("pT",&pT,"pT/D");
    lambdaRegTree->Branch("Eta", &Eta, "Eta/D");
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
    lambdaRegTree->Branch("fRunType", &fRunType, "fRunType/I");
    lambdaRegTree->Branch("ystar", &ystar, "ystar/D");
    lambdaRegTree->Branch("ylab", &ylab, "ylab/D");
    lambdaRegTree->Branch("scatEX", &scatEX, "scatEX/D");
    lambdaRegTree->Branch("scatEY", &scatEY, "scatEY/D");
    lambdaRegTree->Branch("scatEZ", &scatEZ, "scatEZ/D");


    lambdaBarTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaBarTree->Branch("pT", &pT, "pT/D");
    lambdaBarTree->Branch("Eta", &Eta, "Eta/D");
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
    lambdaBarTree->Branch("fRunType", &fRunType, "fRunType/I");
    lambdaBarTree->Branch("ystar", &ystar, "ystar/D");
    lambdaBarTree->Branch("ylab", &ylab, "ylab/D");
    lambdaBarTree->Branch("scatEX", &scatEX, "scatEX/D");
    lambdaBarTree->Branch("scatEY", &scatEY, "scatEY/D");
    lambdaBarTree->Branch("scatEZ", &scatEZ, "scatEZ/D");
    lambdaBarTree->Branch("FRUNZ", &FRUNZ, "FRUNZ/D");

    lambdaTotTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaTotTree->Branch("pT", &pT, "pT/D");
    lambdaTotTree->Branch("Eta", &Eta, "Eta/D");
    lambdaTotTree->Branch("x", &x, "x/D");
    lambdaTotTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaTotTree->Branch("Ys", &Ys, "Ys/D");
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
    lambdaTotTree->Branch("fRunType", &fRunType, "fRunType/I");
    lambdaTotTree->Branch("ystar", &ystar, "ystar/D");
    lambdaTotTree->Branch("ylab", &ylab, "ylab/D");
    lambdaTotTree->Branch("scatEX", &scatEX, "scatEX/D");
    lambdaTotTree->Branch("scatEY", &scatEY, "scatEY/D");
    lambdaTotTree->Branch("scatEZ", &scatEZ, "scatEZ/D");
    //Below is unique braches to the LambdaTotTree compared to Reg and Bar Trees
    lambdaTotTree->Branch("PionCharge", &PionCharge, "PionCharge/D");
    lambdaTotTree->Branch("ProtonCharge", &ProtonCharge, "ProtonCharge/D");
    //Candidate Control Variables
    lambdaTotTree->Branch("FRUNZ", &FRUNZ, "FRUNZ/D");
    lambdaTotTree->Branch("PionPt", &PionPt, "PionPt/D");
    lambdaTotTree->Branch("ProtonPt", &ProtonPt, "ProtonPt/D");
    lambdaTotTree->Branch("PionTrackStartRadius", &PionTrackStartRadius, "PionTrackStartRadius/D");
    lambdaTotTree->Branch("ProtonTrackStartRadius", &ProtonTrackStartRadius, "ProtonTrackStartRadius/D");
    lambdaTotTree->Branch("PionL", &PionL, "PionL/D");
    lambdaTotTree->Branch("ProtonL", &ProtonL, "ProtonL/D");
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
    //
    //lambdaTotPionDaughterTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    //lambdaTotPionDaughterTree->Branch("pT", &pT, "pT/D");
    //lambdaTotPionDaughterTree->Branch("Eta", &Eta, "Eta/D");
    //lambdaTotPionDaughterTree->Branch("x", &x, "x/D");
    //lambdaTotPionDaughterTree->Branch("Q2s", &Q2s, "Q2s/D");
    //lambdaTotPionDaughterTree->Branch("Ys", &Ys, "Ys/D");
    //lambdaTotPionDaughterTree->Branch("v0chi2", &v0chi2, "v0chi2/D");
    //lambdaTotPionDaughterTree->Branch("weight1", &weight1, "weight1/D");
    //lambdaTotPionDaughterTree->Branch("weight2", &weight2, "weight2/D");
    //lambdaTotPionDaughterTree->Branch("fRunType", &fRunType, "fRunType/I");
    //lambdaTotPionDaughterTree->Branch("ystar", &ystar, "ystar/D");
    //lambdaTotPionDaughterTree->Branch("ylab", &ylab, "ylab/D");
    //lambdaTotPionDaughterTree->Branch("PionPt", &PionPt, "PionPt/D");
    //lambdaTotPionDaughterTree->Branch("PionTrackStartRadius", &PionTrackStartRadius, "PionTrackStartRadius/D");
    //lambdaTotPionDaughterTree->Branch("PionL", &PionL, "PionL/D");
    //lambdaTotPionDaughterTree->Branch("PionCharge", &PionCharge, "PionCharge/D");
    //lambdaTotPionDaughterTree->Branch("PionTrackLength", &PionTrackLength, "PionTrackLength/D");
    //lambdaTotPionDaughterTree->Branch("PionDCA", &PionDCA, "PionDCA/D");
    //lambdaTotPionDaughterTree->Branch("PionSDCA", &PionSDCA, "PionSDCA/D");
    ////

    lambdaRegStrTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaRegStrTree->Branch("pT", &pT, "pT/D");
    lambdaRegStrTree->Branch("Eta", &Eta, "Eta/D");
    lambdaRegStrTree->Branch("x", &x, "x/D");
    lambdaRegStrTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaRegStrTree->Branch("Ys", &Ys, "Ys/D");
    lambdaRegStrTree->Branch("Angle", &Angle, "Angle/D");
    lambdaRegStrTree->Branch("v0chi2", &v0chi2, "v0chi2/D");
    lambdaRegStrTree->Branch("weight1", &weight1, "weight1/D");
    lambdaRegStrTree->Branch("weight2", &weight2, "weight2/D");
    lambdaRegStrTree->Branch("genIvxX", &genIvxX, "genIvxX/D");
    lambdaRegStrTree->Branch("genIvxY", &genIvxY, "genIvxY/D");
    lambdaRegStrTree->Branch("genIvxZ", &genIvxZ, "genIvxZ/D");
    lambdaRegStrTree->Branch("decayrecX", &decayrecX, "decayrecX/D");
    lambdaRegStrTree->Branch("decayrecY", &decayrecY, "decayrecY/D");
    lambdaRegStrTree->Branch("decayrecZ", &decayrecZ, "decayrecZ/D");
    lambdaRegStrTree->Branch("strIVtype", &strIVtype, "strIVtype/D");

    lambdaBarStrTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaBarStrTree->Branch("pT", &pT, "pT/D");
    lambdaBarStrTree->Branch("Eta", &Eta, "Eta/D");
    lambdaBarStrTree->Branch("x", &x, "x/D");
    lambdaBarStrTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaBarStrTree->Branch("Ys", &Ys, "Ys/D");
    lambdaBarStrTree->Branch("Angle", &Angle, "Angle/D");
    lambdaBarStrTree->Branch("v0chi2", &v0chi2, "v0chi2/D");
    lambdaBarStrTree->Branch("weight1", &weight1, "weight1/D");
    lambdaBarStrTree->Branch("weight2", &weight2, "weight2/D");
    lambdaBarStrTree->Branch("genIvxX", &genIvxX, "genIvxX/D");
    lambdaBarStrTree->Branch("genIvxY", &genIvxY, "genIvxY/D");
    lambdaBarStrTree->Branch("genIvxZ", &genIvxZ, "genIvxZ/D");
    lambdaBarStrTree->Branch("decayrecX", &decayrecX, "decayrecX/D");
    lambdaBarStrTree->Branch("decayrecY", &decayrecY, "decayrecY/D");
    lambdaBarStrTree->Branch("decayrecZ", &decayrecZ, "decayrecZ/D");
    lambdaBarStrTree->Branch("strIVtype", &strIVtype, "strIVtype/D");

    lambdaTotStrTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
    lambdaTotStrTree->Branch("pT", &pT, "pT/D");
    lambdaTotStrTree->Branch("Eta", &Eta, "Eta/D");
    lambdaTotStrTree->Branch("x", &x, "x/D");
    lambdaTotStrTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaTotStrTree->Branch("Ys", &Ys, "Ys/D");
    lambdaTotStrTree->Branch("Angle", &Angle, "Angle/D");
    lambdaTotStrTree->Branch("v0chi2", &v0chi2, "v0chi2/D");
    lambdaTotStrTree->Branch("weight1", &weight1, "weight1/D");
    lambdaTotStrTree->Branch("weight2", &weight2, "weight2/D");
    lambdaTotStrTree->Branch("genIvxX", &genIvxX, "genIvxX/D");
    lambdaTotStrTree->Branch("genIvxY", &genIvxY, "genIvxY/D");
    lambdaTotStrTree->Branch("genIvxZ", &genIvxZ, "genIvxZ/D");
    lambdaTotStrTree->Branch("decayrecX", &decayrecX, "decayrecX/D");
    lambdaTotStrTree->Branch("decayrecY", &decayrecY, "decayrecY/D");
    lambdaTotStrTree->Branch("decayrecZ", &decayrecZ, "decayrecZ/D");
    lambdaTotStrTree->Branch("strIVtype", &strIVtype, "strIVtype/D");

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

    

    //Hard coding lambda peaks as a double check
    TH1F* LambdaBoth = new TH1F("Lambda Both", "Lambda Both", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* LambdaReg = new TH1F("Lambda Reg", "Lambda Reg", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* LambdaBar = new TH1F("Lambda Bar", "Lambda Bar", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    //

    TH1F* RejectedDuplicateProtonTracks = new TH1F("Candidates Rejected for Duplicate Proton Tracks", "Candidates Rejected for Duplicate Proton Tracks", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* RejectedDuplicatePionTracks = new TH1F("Candidates Rejected for Duplicate Pionn Tracks", "Candidates Rejected for Duplicate Pion Tracks", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* RejectedDuplicateTotalTracks = new TH1F("Candidates Rejected for Duplicate Tracks", "Candidates Rejected for Duplicate Tracks", lambdabins, lambdalowerbinrange, lambdaupperbinrange);

    TH1F* RejectedRadTrack = new TH1F("Candidates Rejected for CJC1 Requirement", "Candidates Rejected for CJC1 Requirement", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* RejectedTrackpT = new TH1F("Candidates Rejected for Track pT", "Candidates Rejected for Track pT", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* RejectedVertexFitted = new TH1F("Candidates Rejected for Vertex fitted", "Candidates Rejected for vertex fitted", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* RejectedNHit = new TH1F("Candidates Rejected for CJC1 NHit", "Candidates Rejected for CJC1 NHit", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
    TH1F* RejectedH1Track= new TH1F("Candidates Rejected for Duplicate H1 Tracks", "Candidates Rejected for Duplicate H1 Tracks", lambdabins, lambdalowerbinrange, lambdaupperbinrange);

    //Histograms to visualize v0 cit Chi2 distributions
    TH1F* v0chi2bothhist = new TH1F("V0 Chi2 Dist Both", "V0 Chi2 Dist Both", 1000, 0, 10);
    TH1F* v0chi2reghist = new TH1F("V0 Chi2 Dist Reg", "V0 Chi2 Dist Reg", 1000, 0, 10);
    TH1F* v0chi2barhist = new TH1F("V0 Chi2 Dist Bar", "V0 Chi2 Dist Bar", 1000, 0, 10);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    //  Defining Root Canvas to work on
    TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 800);

   
    //  Pointer to vertices to loop over
    static H1CentralVertexArrayPtr vertex;
    static H1PartMCArrayPtr gtrdata;
    static H1PartLambdaArrayPtr LambdaCand;
    static H1StrBankPtr StrBank;
    static H1PartSelTrackArrayPtr PST;
    

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

        vector<double> GtrLambdaRegIndex = {};
        vector<double> GtrLambdaBarIndex = {};
                                           
        int fRunType = gH1Calc->IsMC(); //0 = data, else is MC
        if (fRunType != 0) {
            //Loop over generator data
            for (Int_t gtrloop = 0; gtrloop < gtrdata.GetEntries(); gtrloop++) {
                H1PartMC* part = gtrdata[gtrloop];

                if (part->GetPDG() == -3122) {
                    GtrLambdaBarIndex.push_back(gtrloop);
                }

                if (part->GetPDG() == 3122) {
                    GtrLambdaRegIndex.push_back(gtrloop);
                }
            }
        }

        // Loop over all Lambda candidates on MODS level as determined by rough cuts in H1FindLambda.C code
        for (Int_t loop = 0; loop < LambdaCand.GetEntries(); loop++) {

            if (LambdaCand[loop]->GetNumOfParticles() < 2) { //Ensuring there are 2 stored daughters for candidates
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
            RejectedRadTrack->Fill(invMasslambda);
            continue;
        }

        LambdaCutProgression[3]->Fill(invMasslambda);

        //Reject tracks that are primary vertex fitted with H1PartSelTrack
        //if (ProtonPSTrack->IsFromSecondary() != true || PionPSTrack->IsFromSecondary() != true) {
        //    breakloopcount++;
        //    RejectedVertexFitted->Fill(invMasslambda);
        //    continue;
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
            RejectedH1Track->Fill(invMasslambda);
            continue;
        }
        LambdaCutProgression[5]->Fill(invMasslambda);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //pT Track cut on both proton and pion track
        if (LambdaCand[loop]->GetProtonPt() < 0.12 || LambdaCand[loop]->GetPionPt() < 0.12) {
            breakloopcount++;
            RejectedTrackpT->Fill(invMasslambda);
            continue;
        }
        LambdaCutProgression[6]->Fill(invMasslambda);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //PID Likelihood Cut for Proton
        if (ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton) < 0.001) { 
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[7]->Fill(invMasslambda);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //PID Likelihood Cut for Pion
        if (PionPSTrack->GetDedxLikelihood(H1Dedx::kPion) < 0.001) {
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[8]->Fill(invMasslambda);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////        
        //DCA over dDCA proton cut
        if (abs((protonTrajectory->GetDca(V2PV)) / (protonTrajectory->GetdDca(V2PV))) < 1) { 
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[9]->Fill(invMasslambda);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //DCA over dDCA pion cut
        if (abs((pionTrajectory->GetDca(V2PV)) / (pionTrajectory->GetdDca(V2PV))) < 1) { 
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[10]->Fill(invMasslambda);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Cut against K0 contamination
        Double_t invMasskaonCont = (ProtonH1Track->GetFourVector(mPion)
            + PionH1Track->GetFourVector(mPion)).M();
        if (invMasskaonCont > 0.465 && invMasskaonCont < 0.53) { 
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[11]->Fill(invMasslambda);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Cut against photon contamination
        Double_t invMassphotonCont = (ProtonH1Track->GetFourVector(mElectron)
            + PionH1Track->GetFourVector(mElectron)).M();
        if (invMassphotonCont < 0.05) {
            breakloopcount++;
            continue;
        }

        LambdaCutProgression[12]->Fill(invMasslambda);


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
        PVx = pv.Y();
        PVz = pv.Z();
        LambdaPX = LambdaP.X();
        LambdaPY = LambdaP.Y();
        LambdaPZ = LambdaP.Z();
       /* PionCharge = LambdaCand[loop]->GetPionCharge();
        ProtonCharge = LambdaCand[loop]->GetProtonCharge();
        FRUNZ = LambdaCand[loop]->GetPionTrack()->GetUnusedZHitFraction();
        PionPt = LambdaCand[loop]->GetPionTrack()->GetPt();
        ProtonPt = LambdaCand[loop]->GetProtonTrack()->GetPt();
        PionTrackStartRadius = PionH1Track->GetStartRadius();
        ProtonTrackStartRadius = ProtonH1Track->GetStartRadius();
        PionL = PionPSTrack->GetDedxLikelihood(H1Dedx::kPion);
        ProtonL = ProtonPSTrack->GetDedxLikelihood(H1Dedx::kProton);
        PionTrackLength = LambdaCand[loop]->GetPionTrack()->GetLength();
        ProtonTrackLength = LambdaCand[loop]->GetProtonTrack()->GetLength();
        PionDCA = pionTrajectory->GetDca(V2PV);
        ProtonDCA = protonTrajectory->GetDca(V2PV);
        PionSDCA = abs((pionTrajectory->GetDca(V2PV)) / (pionTrajectory->GetdDca(V2PV)));
        ProtonSDCA = abs((protonTrajectory->GetDca(V2PV)) / (protonTrajectory->GetdDca(V2PV)));
        Chi2 = LambdaCand[loop]->GetV0()->GetFitChi2();
        MassPionPion = invMasskaonCont;
        MassElecElec = invMassphotonCont;
        pTrelPion = LambdaCand[loop]->GetPionPtRel();
        pTrelProton = LambdaCand[loop]->GetProtonPtRel();
        RadiusSVtoPV = LambdaCand[loop]->GetV0()->GetLength();*/

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
        TLorentzVector e_final(-(* ElecE)*sin(*ElecTheta)*cos(*ElecPhi), -(*ElecE)* sin(*ElecTheta)*sin(*ElecPhi), -(*ElecE)* cos(*ElecTheta), *ElecE);// measured scattered electron four vector in lab frame
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
        if (ProtonH1Track->GetNHit(H1Track::tdCJC1) < 1 || PionH1Track->GetNHit(H1Track::tdCJC1) < 1) {
            breakloopcount++;
            RejectedNHit->Fill(invMasslambda);
            continue;
        }
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
            RejectedDuplicateTotalTracks->Fill(invMasslambda);
            RejectedDuplicateProtonTracks->Fill(invMasslambda);
        }
        if (piontrackcheck != -1) {
            //cout << "Redundant pion track" << endl; 
            RejectedDuplicateTotalTracks->Fill(invMasslambda);
            RejectedDuplicatePionTracks->Fill(invMasslambda);
        }
        if (protontrackcheck != -1 || piontrackcheck != -1) {
            //cout << "Duplicate Track Used!" << endl;
            breakloopcount++;
            continue; 
        }



        LambdaCutProgression[14]->Fill(invMasslambda);

        //Cut on Z position of Lambda Decay vertex
        if (abs(sv.Z())>30) {
            //cout << "Duplicate Track Used!" << endl;
            breakloopcount++;
            continue;
        }
        LambdaCutProgression[15]->Fill(invMasslambda);

        //now the particles have passed all checks, we need to update the track reference list
        ProtonTrackCheck.push_back(LambdaCand[loop]->GetProton()->GetPt());
        PionTrackCheck.push_back(LambdaCand[loop]->GetPion()->GetPt());

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//         Candidates Passed all cuts, so now we can compare reconstructed pT of candidate with generated pTs to try and see which generated Lambda we reconstructed
//         The candidate with measured pT closest to generated pT is assumed to be the correct one
        if (fRunType != 0) {
            //cout << "Event: " << eventCounter << endl;
//cout << "There are " << GtrLambdaRegIndex.size() << " Reg Lambdas Generated in this Event: " << endl;
//cout << "There are " << GtrLambdaBarIndex.size() << " Lambda Bars Generated in this Event: " << endl;

//If this candidate is a Lambda Bar, and the event has a generated lambda bar, try to find which generated lambda we found by comparing reconstructed and generated momentums
            if (LambdaCand[loop]->IsLambdaBar() == true && GtrLambdaBarIndex.size() != 0) {
                for (Int_t k = 0; k < GtrLambdaBarIndex.size(); k++) {
                    H1PartMC* part = gtrdata[GtrLambdaBarIndex[k]];
                    GtrAngleLambdaBar.push_back((180 / (TMath::Pi())) * acos((part->GetMomentum().Dot(LambdaP)) / ((part->GetMomentum().Mag()) * (LambdaP.Mag()))));
                    //cout << "Lambda Bar Angles: " << (180 / (TMath::Pi())) * acos((part->GetMomentum().Dot(LambdaP)) / ((part->GetMomentum().Mag()) * (LambdaP.Mag()))) << endl;
                }
                //cout << "The generated lambda bar angle is: " << GtrAngleLambdaBar[minfinder(GtrAngleLambdaBar)] << endl;
                //cout << "The generated lambda bar angle is: " << (180 / (TMath::Pi())) * acos((gtrdata[GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)]]->GetMomentum().Dot(LambdaP)) / ((gtrdata[GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)]]->GetMomentum().Mag()) * (LambdaP.Mag()))) << endl;
                if (gtrdata[GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)]]->IsStr() == true) {
                    //if (StrBank[gtrdata[GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)]]->GetStrIdx()]->GetSvx()->GetIvtype() != 2) {
                    strIVtype = StrBank[gtrdata[GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)]]->GetStrIdx()]->GetSvx()->GetIvtype();
                    //cout << "Itrhis: " << StrBank[gtrdata[GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)]]->GetStrIdx()]->GetItrhis() << endl;
                    //cout << "Mother is Gtr: " << gtrdata[gtrdata[GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)]]->GetMother1()]->IsGtr() << endl;
                    Angle = GtrAngleLambdaBar[minfinder(GtrAngleLambdaBar)];
                    genIvxX = StrBank[gtrdata[GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)]]->GetStrIdx()]->GetSvx()->GetX();
                    genIvxY = StrBank[gtrdata[GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)]]->GetStrIdx()]->GetSvx()->GetY();
                    genIvxZ = StrBank[gtrdata[GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)]]->GetStrIdx()]->GetSvx()->GetZ();
                    decayrecX = sv.X();
                    decayrecY = sv.Y();
                    decayrecZ = sv.Z();
                    //cout << "IsStr and SVX Type is:  " << StrBank[gtrdata[GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)]]->GetStrIdx()]->GetSvx()->GetIvtype() << endl;
                    //cout << "STR " << StrBank[gtrdata[GtrLambdaBarIndex[minfinder(GtrAngleLambdaBar)]]->GetStrIdx()]->GetPx() << endl;
                    lambdaBarStrTree->Fill();
                    lambdaTotStrTree->Fill();
                    //}
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
                if (gtrdata[GtrLambdaRegIndex[minfinder(GtrAngleLambdaReg)]]->IsStr() == true) {
                    //if (StrBank[gtrdata[GtrLambdaRegIndex[minfinder(GtrAngleLambdaReg)]]->GetStrIdx()]->GetSvx()->GetIvtype()!=2) {
                    strIVtype = StrBank[gtrdata[GtrLambdaRegIndex[minfinder(GtrAngleLambdaReg)]]->GetStrIdx()]->GetSvx()->GetIvtype();
                    //cout << "Itrhis: " << StrBank[gtrdata[GtrLambdaRegIndex[minfinder(GtrAngleLambdaReg)]]->GetStrIdx()]->GetItrhis() << endl;
                    //cout << "Mother Is Gtr: " << gtrdata[gtrdata[GtrLambdaRegIndex[minfinder(GtrAngleLambdaReg)]]->GetMother1()]->IsGtr() << endl;
                    Angle = GtrAngleLambdaReg[minfinder(GtrAngleLambdaReg)];
                    genIvxX = StrBank[gtrdata[GtrLambdaRegIndex[minfinder(GtrAngleLambdaReg)]]->GetStrIdx()]->GetSvx()->GetX();
                    genIvxY = StrBank[gtrdata[GtrLambdaRegIndex[minfinder(GtrAngleLambdaReg)]]->GetStrIdx()]->GetSvx()->GetY();
                    genIvxZ = StrBank[gtrdata[GtrLambdaRegIndex[minfinder(GtrAngleLambdaReg)]]->GetStrIdx()]->GetSvx()->GetZ();
                    decayrecX = sv.X();
                    decayrecY = sv.Y();
                    decayrecZ = sv.Z();
                    //cout << "IsStr and SVX Type is:  " << StrBank[gtrdata[GtrLambdaRegIndex[minfinder(GtrAngleLambdaReg)]]->GetStrIdx()]->GetSvx()->GetIvtype() << endl;
                    //cout << "STR " << StrBank[gtrdata[GtrLambdaRegIndex[minfinder(GtrAngleLambdaReg)]]->GetStrIdx()]->GetPx() << endl;
                    lambdaRegStrTree->Fill();
                    lambdaTotStrTree->Fill();
                    //}
                }
            }
        }

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
        RejectedDuplicateProtonTracks->SetLineColor(kBlack);
        RejectedDuplicateProtonTracks->GetXaxis()->SetTitle("Mass [GeV]");
        RejectedDuplicateProtonTracks->GetYaxis()->SetTitle("Count");
        RejectedDuplicateProtonTracks->Draw();
        RejectedDuplicateProtonTracks->Write();

        //RejectedDuplicatePionTracks->SetStats(0);
        RejectedDuplicatePionTracks->SetLineColor(kBlack);
        RejectedDuplicatePionTracks->GetXaxis()->SetTitle("Mass [GeV]");
        RejectedDuplicatePionTracks->GetYaxis()->SetTitle("Count");
        RejectedDuplicatePionTracks->Draw();
        RejectedDuplicatePionTracks->Write();

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
       lambdaCTree->Write();
       lambdaTotStrTree->Write();
       lambdaRegStrTree->Write();
       lambdaBarStrTree->Write();
       //Write and close TTree file
       treefile.Write();
       treefile.Close();

       cout << "TTrees written to " << opts.GetOutput() << endl;

       return 0;

} //main loop