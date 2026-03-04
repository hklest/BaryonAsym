///////////////////////////////////////////////////////
//Loading in same MC data set as used for create_evlsMC
// and doing gen level counts for reconstruction efficiencies.
//This code also has its own Tree for Background particles on STR level to see
// where in the detector simulation BG particles are coming from. 
///////////////////////////////////////////////////////
#include <stdlib.h>
#include <cmath>
#include <iostream>

//ROOT includes
#include <TFile.h>
#include <TSystem.h>
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
#include "H1Skeleton/H1Tree.h"
#include "H1Skeleton/H1EventList.h"
#include "H1Pointers/H1FloatPtr.h"
#include "H1Steering/H1StdCmdLine.h"
#include "H1Mods/H1PartMCArrayPtr.h"
#include "H1Mods/H1PartMC.h"
#include "H1Tracks/H1CentralFittedV0.h"
#include "H1Tracks/H1CentralFittedV0ArrayPtr.h"
#include "H1OOBanks/ODS/H1Svx.h"
#include "H1OOBanks/ODS/H1StrBankPtr.h"
#include "H1OOBanks/ODS/H1Str.h"
#include "H1Calculator/H1CalcGenericInterface.h"
#include "H1Calculator/H1CalcGenericInterface.h"
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
#include "H1Steering/H1SteerManager.h" 
#include "H1HadronicCalibration/H1HadronicCalibration.h" 
#include "H1Skeleton/H1Tree.h"
#include "H1Binning/H1BinMeas.h"
#include "H1Binning/H1ModelBinner.h"
#include "H1Binning/H1DataBinner.h"
#include "H1Analysis/H1AnalysisHistManager.h"
#include "H1Analysis/H1Analysis.h"
#include "H1Analysis/H1AnalysisSelectionResults.h"
#include "H1Analysis/H1AnalysisEventSelector.h"
using namespace std;

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
    //parse the command line
    H1StdCmdLine opts;
    opts.Parse(&argc, argv);

    //Load mODS/HAT files
    H1Tree::Instance()->Open(); // this statement must be there!

    //These are the parameters used for MC selection on generator level
    //MAKE SURE THESE ARE SAME AS IN create_evlsMC! (but for generated values)
    //H1Tree::Instance()->SelectHat("Q2sGen>5.");
    //H1Tree::Instance()->SelectHat("Q2sGen<100.");
    //H1Tree::Instance()->SelectHat("YsGen>0.1");
    //H1Tree::Instance()->SelectHat("YsGen<0.6");

    //Defining pointers for event information
    H1FloatPtr q2sgen("Q2sGen");
    H1FloatPtr ysgen("YsGen");
    H1FloatPtr GenPzHad("GenPzHad");
    H1FloatPtr GenEnHad("GenEnHad");
    H1FloatPtr GenPhElec("GenPhElec");
    H1FloatPtr ElecTheta("ElecTheta"); //Theta of scattered electron
    H1FloatPtr ElecPhi("ElecPhi");
    H1FloatPtr ElecE("ElecE"); //Energy of scattered electron
    H1FloatPtr GenEnElec("GenEnElec"); //Energy of scattered electron combined with photon energy for FSR
    H1FloatPtr GenThElec("GenThElec"); //Theta of scattered electron combined with photon for FSR
    H1FloatPtr EBeamE("EBeamE"); //Energy of electron beam from DMIS 
    H1FloatPtr EBeamP("EBeamP"); //Energy of proton beam from DMIS

    H1FloatPtr w1("Weight1");
    H1FloatPtr w2("Weight2");
    H1FloatPtr vtxZ("VtxZ");
    H1ShortPtr RunType("RunType");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Defining TTrees to store Lambda gen info
    TTree* lambdaRegTree = new TTree("Lambda Reg", "Lambda Reg");
    TTree* lambdaBarTree = new TTree("Lambda Bar", "Lambda Bar");
    TTree* lambdaTotTree = new TTree("Lambda Tot", "Lambda Tot");

    TTree* lambdaRegBGTree = new TTree("Lambda Reg BG", "Lambda Reg BG");
    TTree* lambdaBarBGTree = new TTree("Lambda Bar BG", "Lambda Bar BG");
    TTree* lambdaTotBGTree = new TTree("Lambda Tot BG", "Lambda Tot BG");

    TTree* lambdaRegChannelTree = new TTree("Lambda Reg Channel", "Lambda Reg Channel");
    TTree* lambdaBarChannelTree = new TTree("Lambda Bar Channel", "Lambda Bar Channel");
    TTree* lambdaTotChannelTree = new TTree("Lambda Tot Channel", "Lambda Tot Channel");

    TTree* lambdaTotChannelConsistencyCheckTree = new TTree("Lambda Tot Cons Check Channel", "Lambda Tot Cons Check Channel");
    //defining the variables I want to store in Lambda Trees
    double pT, Eta, x, Q2s, Ys, VtxZ, VtxZgen, weight1, weight2, PDG1timesPDG2, Empz, GenElecEn, H1GenBool, DecayZVtx, DecayXVtx, DecayYVtx, genPhi, PX, PY, PZ;
    double elecPhi, elecTheta, elecEnergy, elecPhigen, elecThetagen, elecEnergygen, Zvtxgen, EmpzGen, Rtheta;  // x, Q2s, Ys; using these also, but they are already defined above
    //Defining Branches
    lambdaRegTree->Branch("pT", &pT, "pT/D");
    lambdaRegTree->Branch("Eta", &Eta, "Eta/D");
    lambdaRegTree->Branch("x", &x, "x/D");
    lambdaRegTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaRegTree->Branch("Ys", &Ys, "Ys/D");
    lambdaRegTree->Branch("VtxZ", &VtxZ, "VtxZ/D");
    lambdaRegTree->Branch("VtxZgen", &VtxZgen, "VtxZgen/D");
    lambdaRegTree->Branch("weight1", &weight1, "weight1/D");
    lambdaRegTree->Branch("weight2", &weight2, "weight2/D");
    lambdaRegTree->Branch("Empz", &Empz, "Empz/D");
    lambdaRegTree->Branch("GenElecEn", &GenElecEn, "GenElecEn/D");
    lambdaRegTree->Branch("H1GenBool", &H1GenBool, "H1GenBool/D");
    lambdaRegTree->Branch("DecayZVtx", &DecayZVtx, "DecayZVtx/D");
    lambdaRegTree->Branch("DecayXVtx", &DecayXVtx, "DecayXVtx/D");
    lambdaRegTree->Branch("DecayYVtx", &DecayYVtx, "DecayYVtx/D");
    lambdaRegTree->Branch("genPhi", &genPhi, "genPhi/D");

    lambdaRegBGTree->Branch("pT", &pT, "pT/D");
    lambdaRegBGTree->Branch("Eta", &Eta, "Eta/D");
    lambdaRegBGTree->Branch("x", &x, "x/D");
    lambdaRegBGTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaRegBGTree->Branch("Ys", &Ys, "Ys/D");
    lambdaRegBGTree->Branch("VtxZ", &VtxZ, "VtxZ/D");
    lambdaRegBGTree->Branch("VtxZgen", &VtxZgen, "VtxZgen/D");
    lambdaRegBGTree->Branch("weight1", &weight1, "weight1/D");
    lambdaRegBGTree->Branch("weight2", &weight2, "weight2/D");
    lambdaRegBGTree->Branch("Empz", &Empz, "Empz/D");
    lambdaRegBGTree->Branch("GenElecEn", &GenElecEn, "GenElecEn/D");
    lambdaRegBGTree->Branch("H1GenBool", &H1GenBool, "H1GenBool/D");
    lambdaRegBGTree->Branch("DecayZVtx", &DecayZVtx, "DecayZVtx/D");
    lambdaRegBGTree->Branch("DecayXVtx", &DecayXVtx, "DecayXVtx/D");
    lambdaRegBGTree->Branch("DecayYVtx", &DecayYVtx, "DecayYVtx/D");
    lambdaRegBGTree->Branch("genPhi", &genPhi, "genPhi/D");

    lambdaRegChannelTree->Branch("pT", &pT, "pT/D");
    lambdaRegChannelTree->Branch("Eta", &Eta, "Eta/D");
    lambdaRegChannelTree->Branch("x", &x, "x/D");
    lambdaRegChannelTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaRegChannelTree->Branch("Ys", &Ys, "Ys/D");
    lambdaRegChannelTree->Branch("VtxZ", &VtxZ, "VtxZ/D");
    lambdaRegChannelTree->Branch("VtxZgen", &VtxZgen, "VtxZgen/D");
    lambdaRegChannelTree->Branch("weight1", &weight1, "weight1/D");
    lambdaRegChannelTree->Branch("weight2", &weight2, "weight2/D");
    lambdaRegChannelTree->Branch("Empz", &Empz, "Empz/D");
    lambdaRegChannelTree->Branch("GenElecEn", &GenElecEn, "GenElecEn/D");
    lambdaRegChannelTree->Branch("H1GenBool", &H1GenBool, "H1GenBool/D");
    lambdaRegChannelTree->Branch("DecayZVtx", &DecayZVtx, "DecayZVtx/D");
    lambdaRegChannelTree->Branch("DecayXVtx", &DecayXVtx, "DecayXVtx/D");
    lambdaRegChannelTree->Branch("DecayYVtx", &DecayYVtx, "DecayYVtx/D");
    lambdaRegChannelTree->Branch("genPhi", &genPhi, "genPhi/D");

    lambdaBarTree->Branch("pT", &pT, "pT/D");
    lambdaBarTree->Branch("Eta", &Eta, "Eta/D");
    lambdaBarTree->Branch("x", &x, "x/D");
    lambdaBarTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaBarTree->Branch("Ys", &Ys, "Ys/D");
    lambdaBarTree->Branch("weight1", &weight1, "weight1/D");
    lambdaBarTree->Branch("weight2", &weight2, "weight2/D");
    lambdaBarTree->Branch("Empz", &Empz, "Empz/D");
    lambdaBarTree->Branch("GenElecEn", &GenElecEn, "GenElecEn/D");
    lambdaBarTree->Branch("H1GenBool", &H1GenBool, "H1GenBool/D");
    lambdaBarTree->Branch("VtxZgen", &VtxZgen, "VtxZgen/D");
    lambdaBarTree->Branch("DecayZVtx", &DecayZVtx, "DecayZVtx/D");
    lambdaBarTree->Branch("DecayXVtx", &DecayXVtx, "DecayXVtx/D");
    lambdaBarTree->Branch("DecayYVtx", &DecayYVtx, "DecayYVtx/D");
    lambdaBarTree->Branch("genPhi", &genPhi, "genPhi/D");

    lambdaBarBGTree->Branch("pT", &pT, "pT/D");
    lambdaBarBGTree->Branch("Eta", &Eta, "Eta/D");
    lambdaBarBGTree->Branch("x", &x, "x/D");
    lambdaBarBGTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaBarBGTree->Branch("Ys", &Ys, "Ys/D");
    lambdaBarBGTree->Branch("weight1", &weight1, "weight1/D");
    lambdaBarBGTree->Branch("weight2", &weight2, "weight2/D");
    lambdaBarBGTree->Branch("Empz", &Empz, "Empz/D");
    lambdaBarBGTree->Branch("GenElecEn", &GenElecEn, "GenElecEn/D");
    lambdaBarBGTree->Branch("H1GenBool", &H1GenBool, "H1GenBool/D");
    lambdaBarBGTree->Branch("VtxZgen", &VtxZgen, "VtxZgen/D");
    lambdaBarBGTree->Branch("DecayZVtx", &DecayZVtx, "DecayZVtx/D");
    lambdaBarBGTree->Branch("DecayXVtx", &DecayXVtx, "DecayXVtx/D");
    lambdaBarBGTree->Branch("DecayYVtx", &DecayYVtx, "DecayYVtx/D");
    lambdaBarBGTree->Branch("genPhi", &genPhi, "genPhi/D");

    lambdaBarChannelTree->Branch("pT", &pT, "pT/D");
    lambdaBarChannelTree->Branch("Eta", &Eta, "Eta/D");
    lambdaBarChannelTree->Branch("x", &x, "x/D");
    lambdaBarChannelTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaBarChannelTree->Branch("Ys", &Ys, "Ys/D");
    lambdaBarChannelTree->Branch("weight1", &weight1, "weight1/D");
    lambdaBarChannelTree->Branch("weight2", &weight2, "weight2/D");
    lambdaBarChannelTree->Branch("Empz", &Empz, "Empz/D");
    lambdaBarChannelTree->Branch("GenElecEn", &GenElecEn, "GenElecEn/D");
    lambdaBarChannelTree->Branch("H1GenBool", &H1GenBool, "H1GenBool/D");
    lambdaBarChannelTree->Branch("VtxZgen", &VtxZgen, "VtxZgen/D");
    lambdaBarChannelTree->Branch("DecayZVtx", &DecayZVtx, "DecayZVtx/D");
    lambdaBarChannelTree->Branch("DecayXVtx", &DecayXVtx, "DecayXVtx/D");
    lambdaBarChannelTree->Branch("DecayYVtx", &DecayYVtx, "DecayYVtx/D");
    lambdaBarChannelTree->Branch("genPhi", &genPhi, "genPhi/D");

    lambdaTotTree->Branch("pT", &pT, "pT/D");
    lambdaTotTree->Branch("Eta", &Eta, "Eta/D");
    lambdaTotTree->Branch("x", &x, "x/D");
    lambdaTotTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaTotTree->Branch("Ys", &Ys, "Ys/D");
    lambdaTotTree->Branch("weight1", &weight1, "weight1/D");
    lambdaTotTree->Branch("weight2", &weight2, "weight2/D");
    lambdaTotTree->Branch("Empz", &Empz, "Empz/D");
    lambdaTotTree->Branch("GenElecEn", &GenElecEn, "GenElecEn/D");
    lambdaTotTree->Branch("H1GenBool", &H1GenBool, "H1GenBool/D");
    lambdaTotTree->Branch("VtxZgen", &VtxZgen, "VtxZgen/D");
    lambdaTotTree->Branch("DecayZVtx", &DecayZVtx, "DecayZVtx/D");
    lambdaTotTree->Branch("DecayXVtx", &DecayXVtx, "DecayXVtx/D");
    lambdaTotTree->Branch("DecayYVtx", &DecayYVtx, "DecayYVtx/D");
    lambdaTotTree->Branch("genPhi", &genPhi, "genPhi/D");

    lambdaTotBGTree->Branch("pT", &pT, "pT/D");
    lambdaTotBGTree->Branch("Eta", &Eta, "Eta/D");
    lambdaTotBGTree->Branch("x", &x, "x/D");
    lambdaTotBGTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaTotBGTree->Branch("Ys", &Ys, "Ys/D");
    lambdaTotBGTree->Branch("weight1", &weight1, "weight1/D");
    lambdaTotBGTree->Branch("weight2", &weight2, "weight2/D");
    lambdaTotBGTree->Branch("Empz", &Empz, "Empz/D");
    lambdaTotBGTree->Branch("GenElecEn", &GenElecEn, "GenElecEn/D");
    lambdaTotBGTree->Branch("H1GenBool", &H1GenBool, "H1GenBool/D");
    lambdaTotBGTree->Branch("VtxZgen", &VtxZgen, "VtxZgen/D");
    lambdaTotBGTree->Branch("DecayZVtx", &DecayZVtx, "DecayZVtx/D");
    lambdaTotBGTree->Branch("DecayXVtx", &DecayXVtx, "DecayXVtx/D");
    lambdaTotBGTree->Branch("DecayYVtx", &DecayYVtx, "DecayYVtx/D");
    lambdaTotBGTree->Branch("genPhi", &genPhi, "genPhi/D");

    lambdaTotChannelTree->Branch("pT", &pT, "pT/D");
    lambdaTotChannelTree->Branch("Eta", &Eta, "Eta/D");
    lambdaTotChannelTree->Branch("x", &x, "x/D");
    lambdaTotChannelTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaTotChannelTree->Branch("Ys", &Ys, "Ys/D");
    lambdaTotChannelTree->Branch("weight1", &weight1, "weight1/D");
    lambdaTotChannelTree->Branch("weight2", &weight2, "weight2/D");
    lambdaTotChannelTree->Branch("Empz", &Empz, "Empz/D");
    lambdaTotChannelTree->Branch("GenElecEn", &GenElecEn, "GenElecEn/D");
    lambdaTotChannelTree->Branch("H1GenBool", &H1GenBool, "H1GenBool/D");
    lambdaTotChannelTree->Branch("VtxZgen", &VtxZgen, "VtxZgen/D");
    lambdaTotChannelTree->Branch("DecayZVtx", &DecayZVtx, "DecayZVtx/D");
    lambdaTotChannelTree->Branch("DecayXVtx", &DecayXVtx, "DecayXVtx/D");
    lambdaTotChannelTree->Branch("DecayYVtx", &DecayYVtx, "DecayYVtx/D");
    lambdaTotChannelTree->Branch("genPhi", &genPhi, "genPhi/D");
    //lambdaTotChannelConsistencyCheckTree->Branch("PDG1timesPDG2", &PDG1timesPDG2, "PDG1timesPDG2/D");

    lambdaTotChannelConsistencyCheckTree->Branch("pT", &pT, "pT/D");
    lambdaTotChannelConsistencyCheckTree->Branch("Eta", &Eta, "Eta/D");
    lambdaTotChannelConsistencyCheckTree->Branch("x", &x, "x/D");
    lambdaTotChannelConsistencyCheckTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaTotChannelConsistencyCheckTree->Branch("Ys", &Ys, "Ys/D");
    lambdaTotChannelConsistencyCheckTree->Branch("weight1", &weight1, "weight1/D");
    lambdaTotChannelConsistencyCheckTree->Branch("weight2", &weight2, "weight2/D");
    lambdaTotChannelConsistencyCheckTree->Branch("PDG1timesPDG2", &PDG1timesPDG2, "PDG1timesPDG2/D");
    lambdaTotChannelConsistencyCheckTree->Branch("genPhi", &genPhi, "genPhi/D");

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Defining TTrees to store K0s gen info
    TTree* K0sTree = new TTree("K0s", "K0s");
    TTree* K0sChannelTree = new TTree("K0s Channel", "K0s Channel");
    
    //Defining Tree Branches
    K0sTree->Branch("pT", &pT, "pT/D");
    K0sTree->Branch("Eta", &Eta, "Eta/D");
    K0sTree->Branch("x", &x, "x/D");
    K0sTree->Branch("Q2s", &Q2s, "Q2s/D");
    K0sTree->Branch("Ys", &Ys, "Ys/D");
    K0sTree->Branch("weight1", &weight1, "weight1/D");
    K0sTree->Branch("weight2", &weight2, "weight2/D");
    K0sTree->Branch("Empz", &Empz, "Empz/D");
    K0sTree->Branch("GenElecEn", &GenElecEn, "GenElecEn/D");
    K0sTree->Branch("H1GenBool", &H1GenBool, "H1GenBool/D");
    K0sTree->Branch("VtxZgen", &VtxZgen, "VtxZgen/D");
    K0sTree->Branch("genPhi", &genPhi, "genPhi/D");
    K0sTree->Branch("PX", &PX, "PX/D");
    K0sTree->Branch("PY", &PY, "PY/D");
    K0sTree->Branch("PZ", &PZ, "PZ/D");
    K0sTree->Branch("elecPhi", &elecPhi, "elecPhi/D");
    K0sTree->Branch("elecTheta", &elecTheta, "elecTheta/D");
    K0sTree->Branch("elecPhigen", &elecPhigen, "elecPhigen/D");
    K0sTree->Branch("elecThetagen", &elecThetagen, "elecThetagen/D");
    K0sTree->Branch("elecEnergy", &elecEnergy, "elecEnergy/D");
    K0sTree->Branch("elecEnergygen", &elecEnergygen, "elecEnergygen/D");
    //K0sTree->Branch("DecayZVtx", &DecayZVtx, "DecayZVtx/D");
    //K0sTree->Branch("DecayXVtx", &DecayXVtx, "DecayXVtx/D");
    //K0sTree->Branch("DecayYVtx", &DecayYVtx, "DecayYVtx/D");
    

    K0sChannelTree->Branch("pT", &pT, "pT/D");
    K0sChannelTree->Branch("Eta", &Eta, "Eta/D");
    K0sChannelTree->Branch("x", &x, "x/D");
    K0sChannelTree->Branch("Q2s", &Q2s, "Q2s/D");
    K0sChannelTree->Branch("Ys", &Ys, "Ys/D");
    K0sChannelTree->Branch("weight1", &weight1, "weight1/D");
    K0sChannelTree->Branch("weight2", &weight2, "weight2/D");
    K0sChannelTree->Branch("Empz", &Empz, "Empz/D");
    K0sChannelTree->Branch("GenElecEn", &GenElecEn, "GenElecEn/D");
    K0sChannelTree->Branch("H1GenBool", &H1GenBool, "H1GenBool/D");
    K0sChannelTree->Branch("VtxZgen", &VtxZgen, "VtxZgen/D");
    K0sChannelTree->Branch("genPhi", &genPhi, "genPhi/D");
    
    K0sChannelTree->Branch("elecPhi", &elecPhi, "elecPhi/D");
    K0sChannelTree->Branch("elecTheta", &elecTheta, "elecTheta/D");
    K0sChannelTree->Branch("elecPhigen", &elecPhigen, "elecPhigen/D");
    K0sChannelTree->Branch("elecThetagen", &elecThetagen, "elecThetagen/D");
    K0sChannelTree->Branch("elecEnergy", &elecEnergy, "elecEnergy/D");
    K0sChannelTree->Branch("elecEnergygen", &elecEnergygen, "elecEnergygen/D");
    //K0sChannelTree->Branch("DecayZVtx", &DecayZVtx, "DecayZVtx/D");
    //K0sChannelTree->Branch("DecayXVtx", &DecayXVtx, "DecayXVtx/D");
    //K0sChannelTree->Branch("DecayYVtx", &DecayYVtx, "DecayYVtx/D");
    //Defining TTrees to store K0s gen info
    TTree* K0lTree = new TTree("K0l", "K0l");

    


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Defining Trees to store Lambda C gen info
    TTree* lambdaCRegTree = new TTree("LambdaC Reg", "LambdaC Reg");
    TTree* lambdaCBarTree = new TTree("LambdaC Bar", "LambdaC Bar");
    TTree* lambdaCTotTree = new TTree("LambdaC Tot", "LambdaC Tot");

    lambdaCRegTree->Branch("pT", &pT, "pT/D");
    lambdaCRegTree->Branch("Eta", &Eta, "Eta/D");
    lambdaCRegTree->Branch("x", &x, "x/D");
    lambdaCRegTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaCRegTree->Branch("Ys", &Ys, "Ys/D");

    lambdaCBarTree->Branch("pT", &pT, "pT/D");
    lambdaCBarTree->Branch("Eta", &Eta, "Eta/D");
    lambdaCBarTree->Branch("x", &x, "x/D");
    lambdaCBarTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaCBarTree->Branch("Ys", &Ys, "Ys/D");

    lambdaCTotTree->Branch("pT", &pT, "pT/D");
    lambdaCTotTree->Branch("Eta", &Eta, "Eta/D");
    lambdaCTotTree->Branch("x", &x, "x/D");
    lambdaCTotTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaCTotTree->Branch("Ys", &Ys, "Ys/D");
    ///////////////////////////////////////////////////////////////////////////////////////////
    // Defining TTrees to store D0 gen info
    TTree* D0RegTree = new TTree("D0 Reg", "D0 Reg");
    TTree* D0BarTree = new TTree("D0 Bar", "D0 Bar");
    TTree* D0TotTree = new TTree("D0 Tot", "D0 Tot");

    D0RegTree->Branch("pT", &pT, "pT/D");
    D0RegTree->Branch("Eta", &Eta, "Eta/D");
    D0RegTree->Branch("x", &x, "x/D");
    D0RegTree->Branch("Q2s", &Q2s, "Q2s/D");
    D0RegTree->Branch("Ys", &Ys, "Ys/D");

    D0BarTree->Branch("pT", &pT, "pT/D");
    D0BarTree->Branch("Eta", &Eta, "Eta/D");
    D0BarTree->Branch("x", &x, "x/D");
    D0BarTree->Branch("Q2s", &Q2s, "Q2s/D");
    D0BarTree->Branch("Ys", &Ys, "Ys/D");

    D0TotTree->Branch("pT", &pT, "pT/D");
    D0TotTree->Branch("Eta", &Eta, "Eta/D");
    D0TotTree->Branch("x", &x, "x/D");
    D0TotTree->Branch("Q2s", &Q2s, "Q2s/D");
    D0TotTree->Branch("Ys", &Ys, "Ys/D");
    ///////////////////////////////////////////////////////////////////////////////
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Defining TTrees to event info
    TTree* EventTree = new TTree("Gen Event Tree", "Gen Event Tree");
    //double elecPhi, elecTheta, elecEnergy, elecPhigen, elecThetagen, elecEnergygen, Zvtxgen, EmpzGen, Rtheta;  // x, Q2s, Ys; using these also, but they are already defined above
    //Defining Tree Branches
    EventTree->Branch("elecPhi", &elecPhi, "elecPhi/D");
    EventTree->Branch("elecTheta", &elecTheta, "elecTheta/D");
    EventTree->Branch("elecPhigen", &elecPhigen, "elecPhigen/D");
    EventTree->Branch("elecThetagen", &elecThetagen, "elecThetagen/D");
    EventTree->Branch("x", &x, "x/D");
    EventTree->Branch("Q2s", &Q2s, "Q2s/D");
    EventTree->Branch("Ys", &Ys, "Ys/D");
    EventTree->Branch("elecEnergy", &elecEnergy, "elecEnergy/D");
    EventTree->Branch("elecEnergygen", &elecEnergygen, "elecEnergygen/D");
    EventTree->Branch("VtxZ", &VtxZ, "VtxZ/D");
    EventTree->Branch("EmpzGen", &EmpzGen, "EmpzGen/D");
    EventTree->Branch("Rtheta", &Rtheta, "Rtheta/D");
    EventTree->Branch("weight1", &weight1, "weight1/D");
    EventTree->Branch("weight2", &weight2, "weight2/D");
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    vector<vector<double>> LambdaMothers = {};
    vector<vector<double>> LambdaBarMothers = {};
    vector<vector<double>> K0shortMothers = {};
    vector<vector<double>> K0longMothers = {};

    vector<vector<double>> K0Daughters = {};
    vector<vector<double>> K0barDaughters = {};
    vector<vector<double>> K0Mothers = {};
    vector<vector<double>> K0barMothers = {};

    vector<double> lambdantilambdacountvector = {0,0};
    double lambdantilambdacount = 0;
    int totcountcheck = 0;

    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    //For each event need to select proper pT and Eta ranges, so need pointer to H1PartMC
    static H1PartMCArrayPtr gtrdata;
    static H1SelVertexArrayPtr vertex;
    static H1StrBankPtr strbank;
   
    TLorentzVector totgen4vec;//is initialized by zero four vector by default
    double particles = 0;
    Int_t sel = 0;
    double toteventscheck = 0;
    double eventsrejected = 0;
    
    int other = 0;
    int PDGflag = 0;
    int multipledaughterflag1 = 0;
    int multipledaughterflag2 = 0;
    

    //gH1Tree->Open();
    gH1Calc = H1Calculator::Instance();
    while (H1Tree::Instance()->Next()) {
        gH1Calc->Reset();

        //cout << "RunPeriod: " << gH1Constants->GetRunPeriod() << endl;
        //cout << "Run Number: " << gH1Calc->GetRunNumber() << endl;
        //gH1Constants->SetConstants(gH1Calc->GetRunNumber()); // SetConstants takes either period or run
      /*  int RunPeriod = gH1Constants->GetRunPeriod();

        cout << "Run Period                          = ";
        if (RunPeriod == H1Constants::eEplus9900)  cout << "e+ 99/00"
            << endl;
        else if (RunPeriod == H1Constants::eEplus0304)  cout << "e+ 03/04"
            << endl;
        else if (RunPeriod == H1Constants::eEminus0405) cout << "e- 04/05"
            << endl;
        else if (RunPeriod == H1Constants::eEminus06)   cout << "e- 06" <<
            endl;
        else if (RunPeriod == H1Constants::eEplus0607)  cout << "e+ 06/07"
            << endl;
        else {
            Error("AnalysisBase::InitialSettings", "Unkown Run Period. Please correct the steering.");
                exit(1);
            cout << RunPeriod << endl;
        }*/



        
        //Enforcing DIS kinematic range on Q2 and y on gen level
        toteventscheck++;
        //if (*q2sgen <= 5 || *q2sgen >= 100 || *ysgen <= 0.1 || *ysgen >= 0.6) { //|| zGen <= -30 || zGen >= 30) {
        //    eventsrejected++;
        //    cout << "Rejected" << endl;
        //    continue;
        //}

        

        //Now looping over H1SelVertex to get the generated Z vertex value
        int havesimulatedvertex = 0;
        //double Zvtxgen;
        for (int v = 0; v < vertex.GetEntries(); v++) {
            Int_t type = vertex[v]->GetVertexType();
            //if (type == 0) {
            //    havePrimaryVertex = true;
            //    primaryVertex = vertex[v]->GetPosition();
            //}
            if (type == 1) {
                havesimulatedvertex = 1;
                Zvtxgen = vertex[v]->GetPosition().Z();
            }
        }


        //Define all the variables I want to put into TTrees
         //double elecPhi, elecTheta, Epz, elecE  // x, Q2s, Ys; using these also, but they are already defined abov
        elecEnergygen = *GenEnElec;
        elecPhigen = *GenPhElec;
        elecThetagen = *GenThElec;
        elecEnergy = *ElecE;
        elecPhi = *ElecPhi;
        elecTheta = *ElecTheta;
        //x = (*q2egen) / ((*yegen) * 920 * 27.6 * 4);
        x = ((*GenEnElec) / (*EBeamP)) * (cos((*GenThElec) / (2))) * (cos((*GenThElec) / (2))) / (*ysgen); //Bjorken x calculated from Sigma Method, ISR independent
        Q2s = *q2sgen;
        Ys = *ysgen;
        VtxZ = Zvtxgen;
        EmpzGen = gH1Calc->Fs()->GetEmpzGen();
        Rtheta = abs((Zvtxgen + 160) * tan(*GenThElec));
        weight1 = *w1;
        weight2 = *w2;
        genPhi = *GenPhElec;
       
        
        //All the events that pass the basic gen level cuts analogous to ones used in the analysis are put into this tree
        EventTree->Fill();
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //cout << *RunType << endl;

        

        sel++;
        

        //Counting particles by checking PDGs
        //Going to llop over all MC particles produced in the event
        //Lambda and K0 particles are stable on the generator level by H1 convention
        //We are looking at decay channels: Lambda->pion/proton and K0->pion/pion
        //The decay daughters will only be Str particles coming from a decay vertex, and their mothers will only be Gtr particles
        //vector<int> LambdaDaughterIndices = {}; //initialize container for all stored daughter indices
        vector<int> LambdaChannelIndices = {}; //initialize container for all stored daughter indices
        vector<int> LambdaTotIndices = {}; //initialize container for all stored daughter indices

        vector<int> K0DaughterIndices = {}; //initialize container for all stored daughter indices

        lambdantilambdacountvector = { 0,0 };


        for (Int_t i = 0; i < gtrdata.GetEntries(); i++) {
            H1PartMC* part = gtrdata[i];
            
            //////////////////////////////////////////////////////////////////////////////////////////////////
            ////Counting feed down use this condition
            ////we don't want to exlude ALL non Gtr particles since there is a significant source of Lambdas coming from feed down decays are only allowed at Str level by H1 convention
            ////So if the particle is Str, we requie its vertex type be a decay vertex
            //if (part->IsGtr() == false) {
            //    if (strbank[part->GetStrIdx()]->GetSvx()->GetIvtype() != 2) { continue; } //Svx types should be 2, 2=decay vertex
            //}
            ////decays should only have 1 Mother, so GetMother2() should equal -1, if not, then skip
            //if (part->GetMother2()!=-1) { continue; }
            //
            ////Now, we have to make sure that if the particle is Str, that it's mother particle IS Gtr to avoid lambdas coming from decays of particles produced in non DIS interactions
            ////If the mother particle is STR, then again it must only be a decay vertex
            //if (part->IsGtr() == false) { // at this point, only decay Svx types are allowed
            //    H1PartMC* partMother = gtrdata[part->GetMother1()];
            //    if (partMother->IsGtr() == false) { 
            //        if (strbank[partMother->GetStrIdx()]->GetSvx()->GetIvtype() != 2) { continue; } //Svx types should be 2, 2=decay vertex
            //        } //if there is a mother particle and it is not Gtr and not Decay Vertex type, then skip
            //}
           
            ////We also have to check the grandmother
            ////Stable H1 particles by convention are: K0s, Lambda, Sigma+, Sigma-, Xi-, Xi0, Omega-
            ////There is a case in the branching ratios of "stable" particles that Omega->Xi->Lambda
            ////So grandmothers must all absolutely be Gtr
            //if (part->IsGtr() == false) { // at this point, only decay Svx types are allowed
            //    H1PartMC* partMother = gtrdata[part->GetMother1()];
            //    H1PartMC* partGrandMother = gtrdata[partMother->GetMother1()];
            //    if (partGrandMother->IsGtr() == false) { continue; }
            //}
            //////////////////////////////////////////////////////////////////////////////////////////////////
            
            //if (BGcheck(part, gtrdata, strbank ) == 1) { continue; }

            //if (part->IsStable() == true && sel==1) { 
            //    totgen4vec = totgen4vec + part->GetFourVector(); //Sanity check, for any given event the invariant mass is ~319 GeV
            //}
            //if (part->IsStable() == false) { continue; }
            //particles++;
            //cout << part->GetStatus() << endl;
            //First impose Generator Level HAT conditions from above
            //if (*q2sgen <= 5 || *q2sgen >= 100 || *ysgen <= 0.1 || *ysgen >= 0.6 ) { continue; }
            //Work here with all MC gen events passing gen level conditions
            //H1FloatPtr GenEnElec("GenEnElec");
            //H1FloatPtr GenPzHad("GenPzHad");
            //H1FloatPtr GenEnHad("GenEnHad");
            //H1FloatPtr GenThElec("GenThElec");
            
   
            //Define all the variables I want to store into Trees
            pT = part->GetPt();
            Eta = part->GetEta();
            //x = (*q2egen) / ((*yegen) * 920 * 27.6 * 4);
            x = ((*GenEnElec) / (*EBeamP)) * (cos((*GenThElec) / (2))) * (cos((*GenThElec) / (2))) / (*ysgen); //Bjorken x calculated from Sigma Method, ISR independent
            Q2s = *q2sgen;
            Ys = *ysgen;
            weight1 = *w1;
            weight2 = *w2;
            Empz = gH1Calc->Fs()->GetEmpzGen();
            GenElecEn = *GenEnElec;
            H1GenBool = part->IsGtr();
            VtxZgen = Zvtxgen;
            PX = part->GetPx();
            PY = part->GetPy();
            PZ = part->GetPz();
           
            //DecayZVtx = strbank[gtrdata[part->GetDaughter1()]->GetStrIdx()]->GetSvx()->GetZ();
            //cout << "Type: " << strbank[gtrdata[part->GetDaughter1()]->GetStrIdx()]->GetSvx()->GetIvtype() << endl;
            //cout << "1: " << strbank[gtrdata[part->GetDaughter1()]->GetStrIdx()]->GetSvx()->GetZ() << endl;
            //cout << "2: " << strbank[gtrdata[part->GetDaughter2()]->GetStrIdx()]->GetSvx()->GetZ() << endl;
            //DecayXVtx = strbank[part->GetStrIdx()]->GetSvx()->GetX();
            //DecayYVtx = strbank[part->GetStrIdx()]->GetSvx()->GetY();
            //enforcing pT and Eta constraints on mother particles
            //if (pT <= 0.5 || pT >= 3.5 || abs(Eta) >= 1.3) { continue; }

            


            //cout << "Mother 1: " << part->GetMother1() << endl;
            //cout << "Mother 2: " << part->GetMother2() << endl;


            //Now check PDG codes of generated particles and store into TTrees
            //Looking for: Lambda, K0s, LambdaC, D0
            
            particles++;

            //Lambda (NEW)
            if (abs(part->GetPDG()) == 3122) {
                //Fill tot lambdas within phase space for MC Truth calculations
                //DecayZVtx = strbank[gtrdata[part->GetDaughter1()]->GetStrIdx()]->GetSvx()->GetZ();
                /*if (part->GetPDG() == -3122) {
                    lambdaTotTree->Fill();
                    lambdaBarTree->Fill();
                }
                if (part->GetPDG() == 3122) {
                    lambdaTotTree->Fill();
                    lambdaRegTree->Fill();
                }*/
                //Now we have to check if the lambdas decay into the channel we are looking at
                //Need to look for daughters of this particle on STR level, so need a second loop over all GTR data
                totcountcheck++;
                vector<int> LambdaDaughterIndices = {}; //initialize container for all stored daughter indices
                for (Int_t j = 0; j < gtrdata.GetEntries(); j++) {
                    H1PartMC* part2 = gtrdata[j];
                    if (part2->IsStr() == false) { continue; } //Decay products of lambdas only exist on STR level, if particle is not STR then skip
                    if (part2->GetMother2() != -1) { continue; } //particles should only have 1 mother, second mother index then should be -1
                    if (part2->GetMother1()==i) { //If particle in secondary loop is claiming to come from the lambda we identified in the primary loop, then we can add it to LambdaDaughterIndices vector
                        LambdaDaughterIndices.push_back(j);
                    }
                }

                if (LambdaDaughterIndices.size() == 0) {
                    DecayZVtx = 0;
                    DecayXVtx = 0;
                    DecayYVtx = 0;
                }
                if (LambdaDaughterIndices.size() != 0) {
                    DecayZVtx = strbank[gtrdata[LambdaDaughterIndices[0]]->GetStrIdx()]->GetSvx()->GetZ();
                    DecayXVtx = strbank[gtrdata[LambdaDaughterIndices[0]]->GetStrIdx()]->GetSvx()->GetX();
                    DecayYVtx = strbank[gtrdata[LambdaDaughterIndices[0]]->GetStrIdx()]->GetSvx()->GetY();

                    //cout << "Z Decay : " << DecayZVtx << endl;
                    //cout << "X Decay : " << DecayXVtx << endl;
                    //cout << "Y Decay : " << DecayYVtx << endl;
                }
                //fill in normal Trees
                
                //if (BGcheck(part, gtrdata, strbank) == 1) { continue; }
                if (part->GetPDG() == -3122 && BGcheck(part, gtrdata, strbank) == 1) {
                    lambdaTotBGTree->Fill();
                    lambdaBarBGTree->Fill();
                }
                if (part->GetPDG() == 3122 && BGcheck(part, gtrdata, strbank) == 1) {
                    lambdaTotBGTree->Fill();
                    lambdaRegBGTree->Fill();
                }

                if (BGcheck(part, gtrdata, strbank) == 1) { continue; }
                
                if (part->GetPDG() == -3122) {
                    lambdaTotTree->Fill();
                    lambdaBarTree->Fill();
                }
                if (part->GetPDG() == 3122) {
                    lambdaTotTree->Fill();
                    lambdaRegTree->Fill();
                }
                
                //Now we checked all particles in this event on STR level, there should be 2 particles claiming to come from this lambda
                if (LambdaDaughterIndices.size() != 2) { continue; } //if there isn't two daughters, then continue

                //Making sure PDG codes of the two daughters are consistent with the decay channel we are looking at
                if (abs(gtrdata[LambdaDaughterIndices[0]]->GetPDG()) == abs(gtrdata[LambdaDaughterIndices[1]]->GetPDG())) { continue; }
                if (abs(gtrdata[LambdaDaughterIndices[0]]->GetPDG()) != 2212 && abs(gtrdata[LambdaDaughterIndices[0]]->GetPDG()) != 211) { continue; }
                if (abs(gtrdata[LambdaDaughterIndices[1]]->GetPDG()) != 2212 && abs(gtrdata[LambdaDaughterIndices[1]]->GetPDG()) != 211) { continue; }

                //cout << "Z Decay 1 : " << strbank[gtrdata[LambdaDaughterIndices[0]]->GetStrIdx()]->GetSvx()->GetZ() << endl;
                //cout << "Z Decay 2 : " << strbank[gtrdata[LambdaDaughterIndices[1]]->GetStrIdx()]->GetSvx()->GetZ() << endl;
            
                //Now fill in channel Trees
                if (part->GetPDG() == -3122) {
                    lambdaTotChannelTree->Fill();
                    lambdaBarChannelTree->Fill();
                }
                if (part->GetPDG() == 3122) {
                    lambdaTotChannelTree->Fill();
                    lambdaRegChannelTree->Fill();
                }
           
            }

           

           
            //K0_short (NEW)
            if (abs(part->GetPDG()) == 310) {

                if (BGcheck(part, gtrdata, strbank) == 1) { continue; }

                K0sTree->Fill();
                
                //Now we have to check if the particle decays into the channel we are looking at
                //Need to look for daughters of this particle on STR level, so need a second loop over all GTR data
                vector<int> K0DaughterIndices = {}; //initialize container for all stored daughter indices
                for (Int_t j = 0; j < gtrdata.GetEntries(); j++) {
                    H1PartMC* part2 = gtrdata[j];
                    if (part2->IsStr() == false) { continue; } //Decay products only exist on STR level, if particle is not STR then skip
                    if (part2->GetMother2() != -1) { continue; } //particles should only have 1 mother, second mother index then should be -1
                    if (part2->GetMother1() == i) { //If particle in secondary loop is claiming to come from the K0short we identified in the primary loop, then we can add it to K0DaughterIndices vector
                        K0DaughterIndices.push_back(j);
                    }
                }
                //Now we checked all particles in this event on STR level, there should be 2 particles claiming to come from this K0short
                if (K0DaughterIndices.size() != 2) { continue; } //if there isn't two daughters, then continue

                //Making sure PDG codes of the two daughters are consistent with the decay channel we are looking at
                if (abs(gtrdata[K0DaughterIndices[0]]->GetPDG()) != 211) { continue; }
                if (abs(gtrdata[K0DaughterIndices[1]]->GetPDG()) != 211) { continue; }

                K0sChannelTree->Fill();
            }

            ////K0s, is its own anti particle
            //if (part->GetPDG() == 310 || part->GetPDG() == -310) {
            //    K0sTree->Fill();

            //    PDGflag = 0;


            //    for (Int_t n = 0; n < K0shortMothers.size(); n++) {
            //        if (gtrdata[part->GetMother1()]->GetPDG() == K0shortMothers[n][0]) {
            //            K0shortMothers[n][1]++;
            //            PDGflag = 1;
            //        }
            //    }

            //    if (PDGflag == 0) {
            //        K0shortMothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
            //    }

            //    //First entry put into arrays keeping track of particle mothers
            //    if (K0shortMothers.size() == 0) {
            //        K0shortMothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
            //    }

            //    /*if (part->GetMother1() == -1) {
            //        cout << "No Mother Particle" << endl;
            //    }

            //    if (part->GetMother1() != -1 && part->GetMother2() == -1) {
            //        cout << "Single Mother PDG: " << gtrdata[part->GetMother1()]->GetPDG() << endl;
            //    }

            //    if (part->GetMother1() != -1 && part->GetMother2() != -1) {

            //        for (Int_t m = part->GetMother1(); m <= part->GetMother2(); m++) {

            //            cout << "Multiple Mother Particles" << endl;
            //            cout << "Mother Particle " << m << " of " << part->GetMother2() - part->GetMother1() << ": " << gtrdata[m]->GetPDG() << endl;

            //        }
            //    }*/

            //    
            //}

            ////K0l, is its own anti particle
            //if (part->GetPDG() == 130 || part->GetPDG() == -130) {
            //    K0lTree->Fill();

            //    PDGflag = 0;


            //    for (Int_t n = 0; n < K0longMothers.size(); n++) {
            //        if (gtrdata[part->GetMother1()]->GetPDG() == K0longMothers[n][0]) {
            //            K0longMothers[n][1]++;
            //            PDGflag = 1;
            //        }
            //    }

            //    if (PDGflag == 0) {
            //        K0longMothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
            //    }

            //    //First entry put into arrays keeping track of particle mothers
            //    if (K0longMothers.size() == 0) {
            //        K0longMothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
            //    }

            //}

            ////K0
            //if (part->GetPDG() == 311) {
            //    K0Tree->Fill();


            //    PDGflag = 0;


            //    for (Int_t n = 0; n < K0Daughters.size(); n++) {
            //        if (gtrdata[part->GetDaughter1()]->GetPDG() == K0Daughters[n][0]) {
            //            K0Daughters[n][1]++;
            //            PDGflag = 1;
            //        }
            //    }

            //    if (PDGflag == 0) {
            //        K0Daughters.push_back({ gtrdata[part->GetDaughter1()]->GetPDG() , 1 });
            //    }

            //    //First entry put into arrays keeping track of particle daughters
            //    if (K0Daughters.size() == 0) {
            //        K0Daughters.push_back({ gtrdata[part->GetDaughter1()]->GetPDG() , 1 });
            //    }



            //    PDGflag = 0;

            //    for (Int_t n = 0; n < K0Mothers.size(); n++) {
            //        if (gtrdata[part->GetMother1()]->GetPDG() == K0Mothers[n][0]) {
            //            K0Mothers[n][1]++;
            //            PDGflag = 1;
            //        }
            //    }

            //    if (PDGflag == 0) {
            //        K0Mothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
            //    }

            //    //First entry put into arrays keeping track of particle mothers
            //    if (K0Mothers.size() == 0) {
            //        K0Mothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
            //    }


            //    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //    
            //    /*if (part->GetDaughter1() == -1) {
            //        cout << "no daughter particle" << endl;
            //    }*/

            //    /*if (part->GetDaughter1() == part->GetDaughter2()) {
            //        cout << "single daughter pdg: " << gtrdata[part->getdaughter1()]->getpdg() << endl;
            //    }*/

            //    if ((part->GetDaughter1() == part->GetDaughter2())) {
            //        multipledaughterflag1++;
            //        /*cout << "multiple daughter particles" << endl;
            //        cout << "first index: " << part->GetDaughter1() << endl;
            //        cout << "seconds index: " << part->GetDaughter2() << endl;
            //        
            //        for (Int_t m = part->GetDaughter1(); m <= part->GetDaughter2(); m++) {

            //            cout << "daughter particle " << m << " of " << part->GetDaughter2() - part->GetDaughter1() << ": " << gtrdata[m]->GetPDG() << endl;

            //        }*/
            //    }

            //    //gtrdata[part->GetDaughter1()]->GetPDG()
            //}

            ////K0bar
            //if (part->GetPDG() == -311) {
            //    K0barTree->Fill();

            //    PDGflag = 0;


            //    for (Int_t n = 0; n < K0barDaughters.size(); n++) {
            //        if (gtrdata[part->GetDaughter1()]->GetPDG() == K0barDaughters[n][0]) {
            //            K0barDaughters[n][1]++;
            //            PDGflag = 1;
            //        }
            //    }

            //    if (PDGflag == 0) {
            //        K0barDaughters.push_back({ gtrdata[part->GetDaughter1()]->GetPDG() , 1 });
            //    }

            //    //First entry put into arrays keeping track of particle daughters
            //    if (K0barDaughters.size() == 0) {
            //        K0barDaughters.push_back({ gtrdata[part->GetDaughter1()]->GetPDG() , 1 });
            //    }

            //    if (part->GetDaughter1() != part->GetDaughter2()) {
            //        multipledaughterflag2++;
            //    }



            //    PDGflag = 0;

            //    for (Int_t n = 0; n < K0barMothers.size(); n++) {
            //        if (gtrdata[part->GetMother1()]->GetPDG() == K0barMothers[n][0]) {
            //            K0barMothers[n][1]++;
            //            PDGflag = 1;
            //        }
            //    }

            //    if (PDGflag == 0) {
            //        K0barMothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
            //    }

            //    //First entry put into arrays keeping track of particle mothers
            //    if (K0barMothers.size() == 0) {
            //        K0barMothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
            //    }



            //}

            ////K0plus
            //if (part->GetPDG() == 321) {
            //    KplusTree->Fill();
            //}

            ////K0minus
            //if (part->GetPDG() == -321) {
            //    KminusTree->Fill();
            //}

            //LambdaC
            if (part->GetPDG() == 4122) {
                if (BGcheck(part, gtrdata, strbank) == 1) { continue; }
                lambdaCTotTree->Fill();
                lambdaCRegTree->Fill();
            }

            if (part->GetPDG() == -4122) {
                if (BGcheck(part, gtrdata, strbank) == 1) { continue; }
                lambdaCTotTree->Fill();
                lambdaCBarTree->Fill();
            }

            //D0
            if (part->GetPDG() == 421) {
                if (BGcheck(part, gtrdata, strbank) == 1) { continue; }
                D0TotTree->Fill();
                D0RegTree->Fill();
            }

            if (part->GetPDG() == -421) {
                if (BGcheck(part, gtrdata, strbank) == 1) { continue; }
                D0TotTree->Fill();
                D0BarTree->Fill();
                
            }


            ////Xi0 aka Cascade Zero
            //if (part->GetPDG() == 3322) {
            //    Xi0Tree->Fill();
            //    Xi0Tree->Fill();
            //    
            //}

            //if (part->GetPDG() == -3322) {
            //    Xi0barTree->Fill();
            //    Xi0barTree->Fill();
            //   
            //}

            ////Xi minus aka Cascade minus
            //if (part->GetPDG() == 3312) {
            //    XiminusTree->Fill();
            //    XiminusTree->Fill();
            //    
            //}

            //if (part->GetPDG() == -3312) {
            //    XiplusTree->Fill();
            //    XiplusTree->Fill();
            //    
            //}

            ////Omega minus
            //if (part->GetPDG() == 3334) {
            //    OmegaminusTree->Fill();
            //    OmegaminusTree->Fill();
            //   
            //}

            //if (part->GetPDG() == -3334) {
            //    OmegaplusTree->Fill();
            //    OmegaplusTree->Fill();
            //    
            //}


            
        }

        if (lambdantilambdacountvector[0] != 0 && lambdantilambdacountvector[1] != 0) {lambdantilambdacount++ ;}
    
    
    }

    //This is a sanity check to make sure I am not including any detector level information
    //For only gen level, invariant mass of all particles should be ~319 GeV with no net transverse momentum
    cout << "Total Gen particles inv mass: " << totgen4vec.M() << endl;
    cout << "Particles: " << particles << endl;

    TFile f(opts.GetOutput(), "RECREATE");
    
    //Write Hists and Trees to File here
    lambdaTotTree->Write();
    lambdaRegTree->Write();
    lambdaBarTree->Write();

    lambdaTotBGTree->Write();
    lambdaRegBGTree->Write();
    lambdaBarBGTree->Write();

    lambdaTotChannelTree->Write();
    lambdaTotChannelConsistencyCheckTree->Write();
    lambdaRegChannelTree->Write();
    lambdaBarChannelTree->Write();

    K0sTree->Write();
    K0sChannelTree->Write();
    /*K0lTree->Write();
    K0Tree->Write();
    K0barTree->Write();
    KplusTree->Write();
    KminusTree->Write();*/

    lambdaCTotTree->Write();
    lambdaCRegTree->Write();
    lambdaCBarTree->Write();

    D0TotTree->Write();
    D0RegTree->Write();
    D0BarTree->Write();

    /*Xi0Tree->Write();
    Xi0barTree->Write();
    XiminusTree->Write();
    XiplusTree->Write();
    OmegaminusTree->Write();
    OmegaplusTree->Write();*/


    EventTree->Write();
    //

    f.Write();
    f.Close();

    cout << "TTrees written to " << opts.GetOutput() << endl;
    cout << "Consistency Checks:" << endl;
    cout << "Total Events: " << toteventscheck << endl;
    cout << "Events Rejected: " << eventsrejected << endl;
    cout << "Events Passed Conditions: " << sel << endl;

    cout << " " << endl;

    cout << "Lambda Reg Mothers: " << LambdaMothers.size() << " Sources" << endl;
    for (Int_t k = 0; k < LambdaMothers.size(); k++) {
        cout << "Particle: " << LambdaMothers[k][0] << " Counts: " << LambdaMothers[k][1] << endl;
    }

    cout << " " << endl;

    cout << "Lambda Bar Mothers: " << LambdaBarMothers.size() << " Sources" << endl;
    for (Int_t k = 0; k < LambdaBarMothers.size(); k++) {
        cout << "Particle: " << LambdaBarMothers[k][0] << " Counts: " << LambdaBarMothers[k][1] << endl;
    }

    cout << " " << endl;

    cout << "K0 Short Mothers: " << K0shortMothers.size() << " Sources" << endl;
    for (Int_t k = 0; k < K0shortMothers.size(); k++) {
        cout << "Particle: " << K0shortMothers[k][0] << " Counts: " << K0shortMothers[k][1] << endl;
    }

    cout << " " << endl;

    cout << "K0 Long Mothers: " << K0longMothers.size() << " Sources" << endl;
    for (Int_t k = 0; k < K0longMothers.size(); k++) {
        cout << "Particle: " << K0longMothers[k][0] << " Counts: " << K0longMothers[k][1] << endl;
    }

    cout << " " << endl;

    cout << "K0 Daughters: " << K0Daughters.size() << " Daughter Types" << endl;
    for (Int_t k = 0; k < K0Daughters.size(); k++) {
        cout << "Particle: " << K0Daughters[k][0] << " Counts: " << K0Daughters[k][1] << endl;
    }

    cout << "K0 with multiple daughters: " << multipledaughterflag1 << endl;

    cout << " " << endl;

    cout << "K0 Bar Daughters: " << K0barDaughters.size() << " Daughter Types" << endl;
    for (Int_t k = 0; k < K0barDaughters.size(); k++) {
        cout << "Particle: " << K0barDaughters[k][0] << " Counts: " << K0barDaughters[k][1] << endl;
    }

    cout << "K0 bar with multiple daughters: " << multipledaughterflag2 << endl;

    cout << " " << endl;


    cout << "K0 Mothers: " << K0Mothers.size() << " Mother Types" << endl;
    for (Int_t k = 0; k < K0Mothers.size(); k++) {
        cout << "Particle: " << K0Mothers[k][0] << " Counts: " << K0Mothers[k][1] << endl;
    }

    cout << " " << endl;


    cout << "K0 Bar Mothers: " << K0barMothers.size() << " Mother Types" << endl;
    for (Int_t k = 0; k < K0barMothers.size(); k++) {
        cout << "Particle: " << K0barMothers[k][0] << " Counts: " << K0barMothers[k][1] << endl;
    }

    cout << "Events with Lambda and Anti-Lambda: " << lambdantilambdacount << endl;
    cout << "Tot Lambda Check: " << totcountcheck << endl;
   
    return 0;
}












