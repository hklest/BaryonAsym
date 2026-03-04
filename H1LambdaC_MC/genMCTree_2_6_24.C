///////////////////////////////////////////////////////
//Loading in same MC data set as used for create_evlsMC
//and doing gen level counts for reconstruction efficiencies
///////////////////////////////////////////////////////
#include <stdlib.h>
#include <cmath>
#include <iostream>

// ROOT includes
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

// H1 OO includes
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

//#include "H1CalcBgTiming.h"
//#include "H1CalcCC.h"
//#include "H1CalcDiff.h"
//#include "H1CalcDiffKine.h"
//#include "H1CalcElec.h"
//#include "H1CalcEvent.h"
//#include "H1CalcFnc.h"
//#include "H1CalcForwDets.h"
//#include "H1CalcFs.h"
//#include "H1CalcHad.h"
//#include "H1CalcJet.h"
//#include "H1CalcK0.h"
//#include "H1CalcKine.h"
//#include "H1CalcMuon.h"
//#include "H1CalcPolarisation.h"
//#include "H1CalcSystematic.h"
//#include "H1CalcTaggers.h"
//#include "H1CalcTrack.h"
//#include "H1CalcTrackExtrapol.h"
//#include "H1CalcTrig.h"
//#include "H1CalcVertex.h"
//#include "H1CalcW.h"
//#include "H1CalcWeight.h"
//#include "H1CalcZ.h"


#include "H1Skeleton/H1Tree.h"

#include "H1Binning/H1BinMeas.h"
#include "H1Binning/H1ModelBinner.h"
#include "H1Binning/H1DataBinner.h"

#include "H1Analysis/H1AnalysisHistManager.h"
#include "H1Analysis/H1Analysis.h"
#include "H1Analysis/H1AnalysisSelectionResults.h"
#include "H1Analysis/H1AnalysisEventSelector.h"
using namespace std;

int
main(int argc, char* argv[])
{
    // parse the command line
    H1StdCmdLine opts;
    opts.Parse(&argc, argv);

      // Load mODS/HAT files
    H1Tree::Instance()->Open(); // this statement must be there!
    //H1Calculator* test = H1Calculator::Instance();
    //gH1Tree->Open();
    //gH1Calc = H1Calculator::Instance();
    //H1Calculator::Instance();
    //As the first thing inside your event loop call
    //    gH1Calc->Reset();

    //H1Tree::Instance();

    //H1Calculator::Instance();

     //These are the parameters used for MC selection on generator level
    //MAKE SURE THESE ARE SAME AS IN create_evlsMC! (but for generated values)
   
    //H1Tree::Instance()->SelectHat("Q2eGen>5.");
    //H1Tree::Instance()->SelectHat("Q2eGen<100.");
    //H1Tree::Instance()->SelectHat("YeGen>0.1");
    //H1Tree::Instance()->SelectHat("YeGen<0.6");
    H1Tree::Instance()->SelectHat("Q2sGen>5.");
    H1Tree::Instance()->SelectHat("Q2sGen<100.");
    H1Tree::Instance()->SelectHat("YsGen>0.1");
    H1Tree::Instance()->SelectHat("YsGen<0.6");

    //H1Tree::Instance()->Reset();
    //H1Calculator::Instance()->Open();

      // create new H1EventList
    //H1EventList *list= new H1EventList("mylist");

      // and enter interesting events into this list
    //Gen level
    //H1FloatPtr q2egen("Q2eGen");
    //H1FloatPtr yegen("YeGen");
    H1FloatPtr q2sgen("Q2sGen");
    H1FloatPtr ysgen("YsGen");
    H1FloatPtr GenPzHad("GenPzHad");
    H1FloatPtr GenEnHad("GenEnHad");
    H1FloatPtr GenPhElec("GenPhElec");

    H1FloatPtr GenEnElec("GenEnElec"); //Energy of scattered electron combined with photon energy for FSR
    H1FloatPtr GenThElec("GenThElec"); //Theta of scattered electron combined with photon for FSR
    H1FloatPtr EBeamE("EBeamE"); //Energy of electron beam from DMIS 
    H1FloatPtr EBeamP("EBeamP"); //Energy of proton beam from DMIS

    H1FloatPtr w1("Weight1");
    H1FloatPtr w2("Weight2");
    //H1FloatPtr vtxZgen("VtxZgen");
    H1FloatPtr vtxZ("VtxZ");
    H1ShortPtr RunType("RunType");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Defining TTrees to store Lambda gen info
    TTree* lambdaRegTree = new TTree("Lambda Reg", "Lambda Reg");
    TTree* lambdaBarTree = new TTree("Lambda Bar", "Lambda Bar");
    TTree* lambdaTotTree = new TTree("Lambda Tot", "Lambda Tot");
    //defining the variables I want to store in Lambda Trees
    double pT, Eta, x, Q2s, Ys, VtxZ, VtxZgen, weight1, weight2;
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

    lambdaBarTree->Branch("pT", &pT, "pT/D");
    lambdaBarTree->Branch("Eta", &Eta, "Eta/D");
    lambdaBarTree->Branch("x", &x, "x/D");
    lambdaBarTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaBarTree->Branch("Ys", &Ys, "Ys/D");
    lambdaBarTree->Branch("weight1", &weight1, "weight1/D");
    lambdaBarTree->Branch("weight2", &weight2, "weight2/D");

    lambdaTotTree->Branch("pT", &pT, "pT/D");
    lambdaTotTree->Branch("Eta", &Eta, "Eta/D");
    lambdaTotTree->Branch("x", &x, "x/D");
    lambdaTotTree->Branch("Q2s", &Q2s, "Q2s/D");
    lambdaTotTree->Branch("Ys", &Ys, "Ys/D");
    lambdaTotTree->Branch("weight1", &weight1, "weight1/D");
    lambdaTotTree->Branch("weight2", &weight2, "weight2/D");

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Defining TTrees to store K0s gen info
    TTree* K0sTree = new TTree("K0s", "K0s");
    
    //Defining Tree Branches
    K0sTree->Branch("pT", &pT, "pT/D");
    K0sTree->Branch("Eta", &Eta, "Eta/D");
    K0sTree->Branch("x", &x, "x/D");
    K0sTree->Branch("Q2s", &Q2s, "Q2s/D");
    K0sTree->Branch("Ys", &Ys, "Ys/D");
    K0sTree->Branch("weight1", &weight1, "weight1/D");
    K0sTree->Branch("weight2", &weight2, "weight2/D");

    //Defining TTrees to store K0s gen info
    TTree* K0lTree = new TTree("K0l", "K0l");

    //Defining Tree Branches
    K0lTree->Branch("pT", &pT, "pT/D");
    K0lTree->Branch("Eta", &Eta, "Eta/D");
    K0lTree->Branch("x", &x, "x/D");
    K0lTree->Branch("Q2s", &Q2s, "Q2s/D");
    K0lTree->Branch("Ys", &Ys, "Ys/D");

    //Defining TTrees to store K0s gen info
    TTree* K0Tree = new TTree("K0", "K0");

    //Defining Tree Branches
    K0Tree->Branch("pT", &pT, "pT/D");
    K0Tree->Branch("Eta", &Eta, "Eta/D");
    K0Tree->Branch("x", &x, "x/D");
    K0Tree->Branch("Q2s", &Q2s, "Q2s/D");
    K0Tree->Branch("Ys", &Ys, "Ys/D");

    //Defining TTrees to store K0bar gen info
    TTree* K0barTree = new TTree("K0bar", "K0bar");

    //Defining Tree Branches
    K0barTree->Branch("pT", &pT, "pT/D");
    K0barTree->Branch("Eta", &Eta, "Eta/D");
    K0barTree->Branch("x", &x, "x/D");
    K0barTree->Branch("Q2s", &Q2s, "Q2s/D");
    K0barTree->Branch("Ys", &Ys, "Ys/D");

    //Defining TTrees to store Kplus gen info
    TTree* KplusTree = new TTree("Kplus", "Kplus");

    //Defining Tree Branches
    KplusTree->Branch("pT", &pT, "pT/D");
    KplusTree->Branch("Eta", &Eta, "Eta/D");
    KplusTree->Branch("x", &x, "x/D");
    KplusTree->Branch("Q2s", &Q2s, "Q2s/D");
    KplusTree->Branch("Ys", &Ys, "Ys/D");

    //Defining TTrees to store Kminus gen info
    TTree* KminusTree = new TTree("Kminus", "Kminus");

    //Defining Tree Branches
    KminusTree->Branch("pT", &pT, "pT/D");
    KminusTree->Branch("Eta", &Eta, "Eta/D");
    KminusTree->Branch("x", &x, "x/D");
    KminusTree->Branch("Q2s", &Q2s, "Q2s/D");
    KminusTree->Branch("Ys", &Ys, "Ys/D");




    TTree* Xi0Tree = new TTree("Xi0", "Xi0");
    //Defining Tree Branches
    Xi0Tree->Branch("pT", &pT, "pT/D");
    Xi0Tree->Branch("Eta", &Eta, "Eta/D");
    Xi0Tree->Branch("x", &x, "x/D");
    Xi0Tree->Branch("Q2s", &Q2s, "Q2s/D");
    Xi0Tree->Branch("Ys", &Ys, "Ys/D");

    TTree* Xi0barTree = new TTree("Xi0bar", "Xi0bar");
    //Defining Tree Branches
    Xi0barTree->Branch("pT", &pT, "pT/D");
    Xi0barTree->Branch("Eta", &Eta, "Eta/D");
    Xi0barTree->Branch("x", &x, "x/D");
    Xi0barTree->Branch("Q2s", &Q2s, "Q2s/D");
    Xi0barTree->Branch("Ys", &Ys, "Ys/D");

    TTree* XiminusTree = new TTree("Ximinus", "Ximinus");
    //Defining Tree Branches
    XiminusTree->Branch("pT", &pT, "pT/D");
    XiminusTree->Branch("Eta", &Eta, "Eta/D");
    XiminusTree->Branch("x", &x, "x/D");
    XiminusTree->Branch("Q2s", &Q2s, "Q2s/D");
    XiminusTree->Branch("Ys", &Ys, "Ys/D");

    TTree* XiplusTree = new TTree("Xiplus", "Xiplus");
    //Defining Tree Branches
    XiplusTree->Branch("pT", &pT, "pT/D");
    XiplusTree->Branch("Eta", &Eta, "Eta/D");
    XiplusTree->Branch("x", &x, "x/D");
    XiplusTree->Branch("Q2s", &Q2s, "Q2s/D");
    XiplusTree->Branch("Ys", &Ys, "Ys/D");

    TTree* OmegaminusTree = new TTree("Omegaminus", "Omegaminus");
    //Defining Tree Branches
    OmegaminusTree->Branch("pT", &pT, "pT/D");
    OmegaminusTree->Branch("Eta", &Eta, "Eta/D");
    OmegaminusTree->Branch("x", &x, "x/D");
    OmegaminusTree->Branch("Q2s", &Q2s, "Q2s/D");
    OmegaminusTree->Branch("Ys", &Ys, "Ys/D");

    TTree* OmegaplusTree = new TTree("Omega", "Xi0");
    //Defining Tree Branches
    OmegaplusTree->Branch("pT", &pT, "pT/D");
    OmegaplusTree->Branch("Eta", &Eta, "Eta/D");
    OmegaplusTree->Branch("x", &x, "x/D");
    OmegaplusTree->Branch("Q2s", &Q2s, "Q2s/D");
    OmegaplusTree->Branch("Ys", &Ys, "Ys/D");



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
    double elecPhi, elecTheta, elecE, Zvtxgen, EmpzGen, Rtheta;  // x, Q2s, Ys; using these also, but they are already defined above
    //Defining Tree Branches
    EventTree->Branch("elecPhi", &elecPhi, "elecPhi/D");
    EventTree->Branch("elecTheta", &elecTheta, "elecTheta/D");
    EventTree->Branch("x", &x, "x/D");
    EventTree->Branch("Q2s", &Q2s, "Q2s/D");
    EventTree->Branch("Ys", &Ys, "Ys/D");
    EventTree->Branch("elecE", &elecE, "elecE/D");
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

    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    //For each event need to select proper pT and Eta ranges, so need pointer to H1PartMC
    static H1PartMCArrayPtr gtrdata;
    static H1SelVertexArrayPtr vertex;
    //static H1StrBankPtr strbank;
   
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
        //H1Calculator::Instance()->Reset();
        gH1Calc->Reset();
        //Getting weight of the event
        /*if (*genweight != 1 || *genweight2!=1) {
            nononeweightcount++;
            cout << "Weight 1: " << *genweight << endl;
            cout << "Weight 2: " << *genweight2 << endl;
        }*/

        //Double_t zGen = H1Calculator::Instance()->Vertex()->GetZGen();
        //H1Calculator::Instance();

        //Enforcing DIS kinematic range on Q2 and y on gen level
        toteventscheck++;
        if (*q2sgen <= 5 || *q2sgen >= 100 || *ysgen <= 0.1 || *ysgen >= 0.6) { //|| zGen <= -30 || zGen >= 30) {
            eventsrejected++;
            continue;
        }
        
        

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


        //Enforcing futher kinematic selections used on events in analysis on gen level for consistency
        if (gH1Calc->Fs()->GetEmpzGen() <= 35 || gH1Calc->Fs()->GetEmpzGen() >= 70) { continue; }
        if (*GenEnElec <= 11) { continue; }
        //if (abs((Zvtxgen + 160) * tan(*GenThElec)) < 9.1) { continue; } //R_Theta [cm] > 9.1


        if (havesimulatedvertex!=1) {
            //cout << "No Similated Vertex, Skipping Event" << endl;
            continue;
        }

        //Skipping if event simulated vertex is out of our range
        if (abs(Zvtxgen)>=30) {
            continue;
        }

        //Define all the variables I want to put into TTrees
         //double elecPhi, elecTheta, Epz, elecE  // x, Q2s, Ys; using these also, but they are already defined abov
        elecE = *GenEnElec;
        elecPhi = *GenPhElec;
        elecTheta = *GenThElec;
        //x = (*q2egen) / ((*yegen) * 920 * 27.6 * 4);
        x = ((*GenEnElec) / (*EBeamP)) * (cos((*GenThElec) / (2))) * (cos((*GenThElec) / (2))) / (*ysgen); //Bjorken x calculated from Sigma Method, ISR independent
        Q2s = *q2sgen;
        Ys = *ysgen;
        VtxZ = Zvtxgen;
        EmpzGen = gH1Calc->Fs()->GetEmpzGen();
        Rtheta = abs((Zvtxgen + 160) * tan(*GenThElec));
        weight1 = *w1;
        weight2 = *w2;

        //All the events that pass the basic gen level cuts analogous to ones used in the analysis are put into this tree
        EventTree->Fill();
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //cout << *RunType << endl;


        sel++;
        //double W = Calc.Weight()->GetWeightGen();
        //cout << "Weight 1: " << *w1 << endl;
        //cout << "Weight: " << W << endl;
        //cout << "Weight 2: " << *w2 << endl;
        //H1FloatPtr GenEnElec("GenEnElec");
        //H1FloatPtr GenPzHad("GenPzHad");
        //H1FloatPtr GenEnHad("GenEnHad");
        //H1FloatPtr GenThElec("GenThElec");
        //cout << "Epz: " << *GenEnElec + abs(*GenEnElec * cos(*GenThElec)) + (*GenEnHad) - (*GenPzHad) << endl;
        //Can do work with selected events here before the next loop which will dig through all the individual particles within the event
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
       
        
        
        //Counting particles by checking PDGs
        //Looping over H1PartMCArray associated with this event to grab all particles meeting conditions

        //reinitialize flag for checking for events both a lambda and an anti-lambda
        lambdantilambdacountvector = { 0,0 };

        for (Int_t i = 0; i < gtrdata.GetEntries(); i++) {
            H1PartMC* part = gtrdata[i];
            //making sure this is gen level event, H1PartMC in general also has STR bank with secondary particles from GEANT H1 detector simulations
            //if (part->IsGtr() == false || part->IsStable()==false) { continue; }
            if (part->IsGtr() == false) { continue; } //This excludes all detector level info in H1PartMC class
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

            //enforcing pT and Eta constraints on mother particles
            if (pT <= 0.5 || pT >= 3.5 || abs(Eta) >= 1.3) { continue; }

            


            //cout << "Mother 1: " << part->GetMother1() << endl;
            //cout << "Mother 2: " << part->GetMother2() << endl;


            //Now check PDG codes of generated particles and store into TTrees
            //Looking for: Lambda, K0s, LambdaC, D0
            
            particles++;


            //Lambda
            if (part->GetPDG() == 3122) {
                lambdaTotTree->Fill();
                lambdaRegTree->Fill();
               
                //cout << "Is GTR: " << part->IsGtr() << endl;

                lambdantilambdacountvector[0]++;

                PDGflag = 0;


                for (Int_t n = 0; n < LambdaMothers.size(); n++) {
                    if (gtrdata[part->GetMother1()]->GetPDG() == LambdaMothers[n][0]) {
                        LambdaMothers[n][1]++;
                        PDGflag = 1;
                    }
                }

                if (PDGflag == 0) {
                    LambdaMothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
                }

                //First entry put into arrays keeping track of particle mothers
                if (LambdaMothers.size() == 0) {
                    LambdaMothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
                }

                //cout << "Mother 1: " << part->GetMother1() << endl;
                //cout << "Mother 2: " << part->GetMother2() << endl;

                /*if (part->GetMother1() == -1) {
                    cout << "No Mother Particle" << endl;
                }

                if (part->GetMother1() != -1 && part->GetMother2() == -1) {
                    cout << "Single Mother PDG: " << gtrdata[part->GetMother1()]->GetPDG() << endl;
                }

                if (part->GetMother1() != -1 && part->GetMother2() != -1) {

                    for (Int_t m = part->GetMother1(); m <= part->GetMother2(); m++) {

                        cout << "Multiple Mother Particles" << endl;
                        cout << "Mother Particle " << m << " of " << part->GetMother2() - part->GetMother1() << ": " << gtrdata[m]->GetPDG() << endl;

                    }
                }*/
                
            }

            if (part->GetPDG() == -3122) {
                lambdaTotTree->Fill();
                lambdaBarTree->Fill();

                lambdantilambdacountvector[1]++;

              
                PDGflag = 0;


                for (Int_t n = 0; n < LambdaBarMothers.size(); n++) {
                    if (gtrdata[part->GetMother1()]->GetPDG() == LambdaBarMothers[n][0]) {
                        LambdaBarMothers[n][1]++;
                        PDGflag = 1;
                    }
                }

                if (PDGflag == 0) {
                    LambdaBarMothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
                }

                //First entry put into arrays keeping track of particle mothers
                if (LambdaBarMothers.size() == 0) {
                    LambdaBarMothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
                }

                /*if (part->GetMother1() == -1) {
                    cout << "No Mother Particle" << endl;
                }

                if (part->GetMother1() != -1 && part->GetMother2() == -1) {
                    cout << "Single Mother PDG: " << gtrdata[part->GetMother1()]->GetPDG() << endl;
                }

                if (part->GetMother1() != -1 && part->GetMother2() != -1) {

                    for (Int_t m = part->GetMother1(); m <= part->GetMother2(); m++) {

                        cout << "Multiple Mother Particles" << endl;
                        cout << "Mother Particle " << m << " of " << part->GetMother2() - part->GetMother1() << ": " << gtrdata[m]->GetPDG() << endl;

                    }
                }*/
                
            }

    

            //K0s, is its own anti particle
            if (part->GetPDG() == 310 || part->GetPDG() == -310) {
                K0sTree->Fill();

                PDGflag = 0;


                for (Int_t n = 0; n < K0shortMothers.size(); n++) {
                    if (gtrdata[part->GetMother1()]->GetPDG() == K0shortMothers[n][0]) {
                        K0shortMothers[n][1]++;
                        PDGflag = 1;
                    }
                }

                if (PDGflag == 0) {
                    K0shortMothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
                }

                //First entry put into arrays keeping track of particle mothers
                if (K0shortMothers.size() == 0) {
                    K0shortMothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
                }

                /*if (part->GetMother1() == -1) {
                    cout << "No Mother Particle" << endl;
                }

                if (part->GetMother1() != -1 && part->GetMother2() == -1) {
                    cout << "Single Mother PDG: " << gtrdata[part->GetMother1()]->GetPDG() << endl;
                }

                if (part->GetMother1() != -1 && part->GetMother2() != -1) {

                    for (Int_t m = part->GetMother1(); m <= part->GetMother2(); m++) {

                        cout << "Multiple Mother Particles" << endl;
                        cout << "Mother Particle " << m << " of " << part->GetMother2() - part->GetMother1() << ": " << gtrdata[m]->GetPDG() << endl;

                    }
                }*/

                
            }

            //K0l, is its own anti particle
            if (part->GetPDG() == 130 || part->GetPDG() == -130) {
                K0lTree->Fill();

                PDGflag = 0;


                for (Int_t n = 0; n < K0longMothers.size(); n++) {
                    if (gtrdata[part->GetMother1()]->GetPDG() == K0longMothers[n][0]) {
                        K0longMothers[n][1]++;
                        PDGflag = 1;
                    }
                }

                if (PDGflag == 0) {
                    K0longMothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
                }

                //First entry put into arrays keeping track of particle mothers
                if (K0longMothers.size() == 0) {
                    K0longMothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
                }

            }

            //K0
            if (part->GetPDG() == 311) {
                K0Tree->Fill();


                PDGflag = 0;


                for (Int_t n = 0; n < K0Daughters.size(); n++) {
                    if (gtrdata[part->GetDaughter1()]->GetPDG() == K0Daughters[n][0]) {
                        K0Daughters[n][1]++;
                        PDGflag = 1;
                    }
                }

                if (PDGflag == 0) {
                    K0Daughters.push_back({ gtrdata[part->GetDaughter1()]->GetPDG() , 1 });
                }

                //First entry put into arrays keeping track of particle daughters
                if (K0Daughters.size() == 0) {
                    K0Daughters.push_back({ gtrdata[part->GetDaughter1()]->GetPDG() , 1 });
                }



                PDGflag = 0;

                for (Int_t n = 0; n < K0Mothers.size(); n++) {
                    if (gtrdata[part->GetMother1()]->GetPDG() == K0Mothers[n][0]) {
                        K0Mothers[n][1]++;
                        PDGflag = 1;
                    }
                }

                if (PDGflag == 0) {
                    K0Mothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
                }

                //First entry put into arrays keeping track of particle mothers
                if (K0Mothers.size() == 0) {
                    K0Mothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
                }


                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                
                /*if (part->GetDaughter1() == -1) {
                    cout << "no daughter particle" << endl;
                }*/

                /*if (part->GetDaughter1() == part->GetDaughter2()) {
                    cout << "single daughter pdg: " << gtrdata[part->getdaughter1()]->getpdg() << endl;
                }*/

                if ((part->GetDaughter1() == part->GetDaughter2())) {
                    multipledaughterflag1++;
                    /*cout << "multiple daughter particles" << endl;
                    cout << "first index: " << part->GetDaughter1() << endl;
                    cout << "seconds index: " << part->GetDaughter2() << endl;
                    
                    for (Int_t m = part->GetDaughter1(); m <= part->GetDaughter2(); m++) {

                        cout << "daughter particle " << m << " of " << part->GetDaughter2() - part->GetDaughter1() << ": " << gtrdata[m]->GetPDG() << endl;

                    }*/
                }

                //gtrdata[part->GetDaughter1()]->GetPDG()
            }

            //K0bar
            if (part->GetPDG() == -311) {
                K0barTree->Fill();

                PDGflag = 0;


                for (Int_t n = 0; n < K0barDaughters.size(); n++) {
                    if (gtrdata[part->GetDaughter1()]->GetPDG() == K0barDaughters[n][0]) {
                        K0barDaughters[n][1]++;
                        PDGflag = 1;
                    }
                }

                if (PDGflag == 0) {
                    K0barDaughters.push_back({ gtrdata[part->GetDaughter1()]->GetPDG() , 1 });
                }

                //First entry put into arrays keeping track of particle daughters
                if (K0barDaughters.size() == 0) {
                    K0barDaughters.push_back({ gtrdata[part->GetDaughter1()]->GetPDG() , 1 });
                }

                if (part->GetDaughter1() != part->GetDaughter2()) {
                    multipledaughterflag2++;
                }



                PDGflag = 0;

                for (Int_t n = 0; n < K0barMothers.size(); n++) {
                    if (gtrdata[part->GetMother1()]->GetPDG() == K0barMothers[n][0]) {
                        K0barMothers[n][1]++;
                        PDGflag = 1;
                    }
                }

                if (PDGflag == 0) {
                    K0barMothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
                }

                //First entry put into arrays keeping track of particle mothers
                if (K0barMothers.size() == 0) {
                    K0barMothers.push_back({ gtrdata[part->GetMother1()]->GetPDG() , 1 });
                }



            }

            //K0plus
            if (part->GetPDG() == 321) {
                KplusTree->Fill();
            }

            //K0minus
            if (part->GetPDG() == -321) {
                KminusTree->Fill();
            }

            //LambdaC
            if (part->GetPDG() == 4122) {
                lambdaCTotTree->Fill();
                lambdaCRegTree->Fill();
            }

            if (part->GetPDG() == -4122) {
                lambdaCTotTree->Fill();
                lambdaCBarTree->Fill();
            }

            //D0
            if (part->GetPDG() == 421) {
                D0TotTree->Fill();
                D0RegTree->Fill();
            }

            if (part->GetPDG() == -421) {
                D0TotTree->Fill();
                D0BarTree->Fill();
                
            }


            //Xi0 aka Cascade Zero
            if (part->GetPDG() == 3322) {
                Xi0Tree->Fill();
                Xi0Tree->Fill();
                
            }

            if (part->GetPDG() == -3322) {
                Xi0barTree->Fill();
                Xi0barTree->Fill();
               
            }

            //Xi minus aka Cascade minus
            if (part->GetPDG() == 3312) {
                XiminusTree->Fill();
                XiminusTree->Fill();
                
            }

            if (part->GetPDG() == -3312) {
                XiplusTree->Fill();
                XiplusTree->Fill();
                
            }

            //Omega minus
            if (part->GetPDG() == 3334) {
                OmegaminusTree->Fill();
                OmegaminusTree->Fill();
               
            }

            if (part->GetPDG() == -3334) {
                OmegaplusTree->Fill();
                OmegaplusTree->Fill();
                
            }


            
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

    K0sTree->Write();
    K0lTree->Write();
    K0Tree->Write();
    K0barTree->Write();
    KplusTree->Write();
    KminusTree->Write();

    lambdaCTotTree->Write();
    lambdaCRegTree->Write();
    lambdaCBarTree->Write();

    D0TotTree->Write();
    D0RegTree->Write();
    D0BarTree->Write();

    Xi0Tree->Write();
    Xi0barTree->Write();
    XiminusTree->Write();
    XiplusTree->Write();
    OmegaminusTree->Write();
    OmegaplusTree->Write();


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
    
   
    return 0;
}












