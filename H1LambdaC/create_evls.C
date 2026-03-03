///////////////////////////////////////////////////////
// Creating an eventlist by entering events one by one
// and write it to a file afterwards.
// Also writes a separate file of various Event Distributions.
//
// Author     : Gage Tustin
///////////////////////////////////////////////////////

#include <stdlib.h>
#include <iostream>

// ROOT includes
#include <TFile.h>
#include <TROOT.h>
#include <THStack.h>
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
#include "H1Mods/H1PartEmArrayPtr.h"
#include "H1Mods/H1PartEm.h"
#include "H1Tools/H1RunList.h"
#include "H1Geom/H1DetectorStatus.h"

#include "H1Clusters/H1Cell.h"
#include "H1Geom/H1CellGeometry.h"
#include "H1Geom/H1SpaCalCellGeo.h"
#include "H1Geom/H1CaloGeometry.h"
#include "H1Geom/H1SpacalGeometry.h"
#include "H1Geom/H1DBManager.h"

using namespace std;

int
main(int argc, char* argv[])
{
    // parse the command line
    H1StdCmdLine opts;
    opts.Parse(&argc, argv);

    // Load mODS/HAT files
    H1Tree::Instance()->Open();            // this statement must be there!

     // do a selection
    // Can implement some selections here, I chose to use H1FloatPtr instead to variables and do a check later
     
    //H1Tree::Instance()->SelectHat("Q2e>5.");
    //H1Tree::Instance()->SelectHat("Q2e<100.");
    //H1Tree::Instance()->SelectHat("ElecE>10.");
    //H1Tree::Instance()->SelectHat("Ye>0.1");
    //H1Tree::Instance()->SelectHat("Ye<0.6");
    //H1Tree::Instance()->SelectHat("Epz>35.");
    //H1Tree::Instance()->SelectHat("Epz<70.");
    H1Tree::Instance()->SelectHat("fIl1ac[61]>0");
    
      // create new H1EventList
    H1EventList *list= new H1EventList("mylist");

      // and enter interesting events into this list
    //H1FloatPtr q2e("Q2e"); //Q2 variable aquired using electron method
    H1FloatPtr q2s("Q2s"); //Q2 variable aquired using sigma method
    //H1FloatPtr q2egen("Q2eGen");//Q2 gen level variable aquired using electron method
    H1FloatPtr q2sgen("Q2sGen"); //Q2 gen level variable aquired using sigma method
    H1FloatPtr eE("ElecE"); //Energy of electron as deposited in SpaCal
    //H1FloatPtr ye("Ye"); //Inelasticity aquired through sigma method
    H1FloatPtr ys("Ys"); //Inelasticity aquired through sigma method
    H1FloatPtr vtxZ("VtxZ");
    H1FloatPtr epz("Epz");

    //Values for Epz calculation
    H1FloatPtr ElecTheta("ElecTheta"); //Theta of scattered electron
    H1FloatPtr ElecPhi("ElecPhi");
    H1FloatPtr ElecE("ElecE"); //Energy of scattered electron
    H1FloatPtr EBeamE("EBeamE"); //Energy of electron beam from DMIS 
    H1FloatPtr EBeamP("EBeamP"); //Energy of proton beam from DMIS
    H1FloatPtr HadPzX("HadPzX");
    H1FloatPtr HadEX("HadEX");
    H1FloatPtr hfsE("FullHfsCombE");
    H1FloatPtr hfsTheta("FullHfsCombTheta");

    //Defining Root Canvas to work on
    TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 800);

    TH2F* hXYclusterBefore = new TH2F("XY Cluster Position Before", "XY Cluster Position Before", 1000, -80, 80, 1000, -80, 80);
    TH2F* hXYclusterAfter = new TH2F("XY Cluster Position After", "XY Cluster Position After", 1000, -80, 80, 1000, -80, 80);

    static H1PartEmArrayPtr PartEm;


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Consistenct with Lumi calculation using SelectedRuns.root oolumi output file
    TString goodRunFileName("/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/SelectedRuns.root");
    TFile goodRunFile(goodRunFileName);
    if (!goodRunFile.IsOpen()) {
        cerr << "Error: could not open file " << goodRunFileName << "\n";
        return 2;
    }

    H1RunList* goodRunList
        = (H1RunList*)goodRunFile.Get("H1RunList");

    if (!goodRunList) {
        cerr << "Error: no runlist in file - return!\n";
        return 2;
    }

    H1DetectorStatus* detectorStatus
        = (H1DetectorStatus*)goodRunFile.Get("MyDetectorStatus");

    if (!detectorStatus) {
        cerr << "Error: no detector status in file - return!\n";
        return 3;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Defining TTrees to syore event info
    TTree* EventTree = new TTree("Data Event Tree", "Data Event Tree");
    double elecPhi, elecTheta, Epz, elecE, x, Q2s, Ys, RTheta, ClusterRadius, Zvertex, ProtonEnergy, ElectronEnergy, Xcluster, Ycluster;
    //Defining Tree Branches
    EventTree->Branch("elecPhi", &elecPhi, "elecPhi/D");
    EventTree->Branch("elecTheta", &elecTheta, "elecTheta/D");
    EventTree->Branch("x", &x, "x/D");
    EventTree->Branch("Q2s", &Q2s, "Q2s/D");
    EventTree->Branch("Ys", &Ys, "Ys/D");
    EventTree->Branch("Epz", &Epz, "Epz/D");
    EventTree->Branch("elecE", &elecE, "elecE/D");
    EventTree->Branch("RTheta", &RTheta, "RTheta/D");
    EventTree->Branch("ClusterRadius", &ClusterRadius, "ClusterRadius/D");
    EventTree->Branch("Zvertex", &Zvertex, "Zvertex/D");
    EventTree->Branch("ProtonEnergy", &ProtonEnergy, "ProtonEnergy/D");
    EventTree->Branch("ElectronEnergy", &ElectronEnergy, "ElectronEnergy/D");
    EventTree->Branch("Xcluster", &Xcluster, "Xcluster/D");
    EventTree->Branch("Ycluster", &Ycluster, "Ycluster/D");
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //initializing counts, counting events after each selection just as a consistency check
    double TriggerEvents = 0;
    double Q2Events = 0;
    double yEvents = 0;
    double eEnergyEvents = 0;
    double zvtxEvents = 0;
    double ClusterREvents = 0;
    double NoScattPartEm = 0;
    double EnergyBalanceEvents = 0;
    double SpaCalEvents = 0;
    double GoodDetectorEvents = 0;
    double GoodRunEvents = 0;
    //double DeadCellEvents = 0;
    //double DeadTriggerEvents = 0;
    //double RadClusterEvents = 0;
    double BoxCutEvents = 0;
    double ElipseCutEvents = 0;

    //initializing PartEM checks
    double scatelCount = 0;

    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Initializing SpaCal dead cells and triggers for scattered electron cut
    //Valid for 2006 data

    

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Int_t sel=0;
    while (H1Tree::Instance()->Next()) {
        TriggerEvents++;

        int RunNumber = H1Tree::Instance()->GetRunNumber();
        //cout << "Run Number: " << RunNumber << endl;


       scatelCount = 0;

        for (Int_t loop = 0; loop < PartEm.GetEntries(); loop++) {
            
            if (PartEm[loop]->IsScatElec() == true) { 
                scatelCount++;
                ClusterRadius = PartEm[loop]->GetRadiusFromVectors();
                //ClusterRadius = PartEm[loop]->GetEcra();
                Xcluster = PartEm[loop]->GetXClus();
                Ycluster = PartEm[loop]->GetYClus();
            }

        }

        //cout << "Scattered e count: " << scatelCount << endl;

        

        
        if (*q2s <= 5 || *q2s >= 100) { continue; } // 5 < Q2 [GeV] < 100
        Q2Events++;
        
        if (*ys <= 0.1 || *ys >= 0.6) { continue; } // 0.1 < y < 0.6
        yEvents++;

        if (*eE <= 11) { continue; } //E_electron [GeV] > 11
        eEnergyEvents++;

        if (*vtxZ<=-30 || *vtxZ>=30) { continue; } //-30 < Z_vertex [cm] < 30
        zvtxEvents++;

        if (*epz<=35 || *epz>=70) { continue; } // 35 < E-pz [GeV] < 70
        EnergyBalanceEvents++;

        if (abs((*vtxZ + 160) * tan(*ElecTheta)) < 9.1) { continue; } //R_Theta [cm] > 9.1
        SpaCalEvents++;

        if (scatelCount != 1) {
            //cout << "Scattered Electron Count != 1" << endl;
            continue;
        } //skip event if scattered electron is not found in PartEm class
        NoScattPartEm++;

        if (ClusterRadius > 3.5) { continue; } // e Cluster Radius [cm] < 3.5
        ClusterREvents++;

        // skip runs not in list of good runs
        if (!goodRunList->FindRun(H1Tree::Instance()->GetRunNumber())) { continue; }
        GoodRunEvents++;
        // skip data events with bad detector status
        if (!detectorStatus->IsOn()) { continue; }
        GoodDetectorEvents++;
        


        hXYclusterBefore->Fill(Xcluster,Ycluster);


     ///////////////////////////////////////////////////////////////////////////////////////
        //SpaCal box cut for scattered electron
        if ((Xcluster >= 9.0 && Xcluster <= 16.24) && (Ycluster >= -4.105 && Ycluster <= 4.377)) { continue; }
        BoxCutEvents++;


        //SpaCal elipse cut 
        if ((Xcluster + 78.4461) * (Xcluster + 78.4461) / ((11.027) * (11.027)) + (Ycluster) * (Ycluster) / ((20.5) * (20.5)) < 1.0) { continue; }
        ElipseCutEvents++;

     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        hXYclusterAfter->Fill(Xcluster, Ycluster);



      //if (*ye < 0.6 && *ye>0.1 && *q2s > 5 && *q2s < 100 && *eE>10 && *epz > 35 && *epz < 70 && *vtxZ < 30 && *vtxZ > -30)    {
        //cout << "E: " << *Echeck << endl;
        //cout << "EPZ: " << *epz << endl;
        //cout << "EPZ calc: " <<  ( * ElecE) + abs((*ElecE)*cos(*ElecTheta)) + (*HadEX)-(*HadPzX) << endl;
        //cout << "EPZ calc: " <<  ( * ElecE) + abs((*ElecE)*cos(*ElecTheta)) + (*hfsE) - abs((*hfsE) * cos(*hfsTheta)) << endl;
          
          elecE = *eE;
          Epz = *epz;
          elecPhi = *ElecPhi;
          elecTheta = *ElecTheta;
          //x = (*q2e) / ((*ye) * 920 * 27.6 * 4); //Bjorken X calculated from Electron Method
          x = ((*eE) / (*EBeamP)) * (cos((*ElecTheta) / (2))) * (cos((*ElecTheta) / (2))) / (*ys);
          Q2s = *q2s;
          Ys = *ys;
          RTheta = abs((*vtxZ + 160) * tan(*ElecTheta));
          Zvertex = *vtxZ;
          ProtonEnergy = *EBeamP;
          ElectronEnergy = *EBeamE;

          EventTree->Fill();

          sel++;
          list->Enter();
      //}
    }
    cout << "Selected "<<sel<<" entries"<<endl;

    cout << "check" << endl;
      // Write eventlist to file
    TFile file(opts.GetOutput(), "RECREATE");
    list->Write();
    list->Print();
    // close
    file.Write();
    file.Close();

    //Write Event Distributions to File
    TFile fileEventDist("/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/EventDists06.root","RECREATE");
    EventTree->Write();

    hXYclusterBefore->SetStats(0);
    hXYclusterBefore->GetXaxis()->SetTitle("X");
    hXYclusterBefore->GetYaxis()->SetTitle("Y");
    //XYvtxdensitytot->GetXaxis()->SetTitleSize(0.03);
    //XYvtxdensitytot->GetXaxis()->SetTitleOffset(1);
    //XYvtxdensitytot->GetYaxis()->SetTitleOffset(1);
    //XYvtxdensitytot->GetYaxis()->SetTitleSize(0.03);
    hXYclusterBefore->Draw("colz");
    canvas->SetName("SpaCal XY Positions Before");
    canvas->Write();

    hXYclusterAfter->SetStats(0);
    hXYclusterAfter->GetXaxis()->SetTitle("X");
    hXYclusterAfter->GetYaxis()->SetTitle("Y");
    //XYvtxdensitytot->GetXaxis()->SetTitleSize(0.03);
    //XYvtxdensitytot->GetXaxis()->SetTitleOffset(1);
    //XYvtxdensitytot->GetYaxis()->SetTitleOffset(1);
    //XYvtxdensitytot->GetYaxis()->SetTitleSize(0.03);
    hXYclusterAfter->Draw("colz");
    canvas->SetName("SpaCal XY Positions After");
    canvas->Write();

    fileEventDist.Write();
    fileEventDist.Close();

    cout << "Eventlist written to " << opts.GetOutput() << endl;
    cout << "Event Dists written to " << "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/EventDists06.root" << endl;

    //initial amount of events in 2006 e+p and e-p: 
    cout << "Events After Trigger: " << TriggerEvents << endl;
    cout << "Events After Q2: " << Q2Events << endl;
    cout << "Events After y: " << yEvents << endl;
    cout << "Events After eE: " << eEnergyEvents << endl;
    cout << "Events After zvtx: " << zvtxEvents << endl;
    cout << "Events After Epz: " << EnergyBalanceEvents << endl;
    cout << "Events After RTheta: " << SpaCalEvents << endl;
    cout << "Events After Requiring Part Em Scattered electron: " << NoScattPartEm << endl;
    cout << "Events After Cluster Radius: " << ClusterREvents << endl;
    cout << "Events Run Check: " << GoodRunEvents << endl;
    cout << "Events After Detector Check: " << GoodDetectorEvents << endl;
    //cout << "Events After Dead Cell Check: " << DeadCellEvents << endl;
    //cout << "Events After Dead Trigger Check: " << DeadTriggerEvents << endl;
    //cout << "Events After Radial Cluster Pos Check: " << RadClusterEvents << endl;
    //cout << "Events After Trigger: " << TriggerEvents << endl;
    cout << "Events After Box Cut: " << BoxCutEvents << endl;
    cout << "Events After Elipse Cut: " << ElipseCutEvents << endl;


    //double TriggerEvents = 0;
    //double Q2Events = 0;
    //double yEvents = 0;
    //double eEnergyEvents = 0;
    //double zvtxEvents = 0;
    //double ClusterREvents = 0;
    //double EnergyBalanceEvents = 0;
    //double SpaCalEvents = 0;
    
    return 0;
}


