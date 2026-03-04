///////////////////////////////////////////////////////
// Example of 
// how to create an eventlist by entering events one by one
// and write it to a file afterwards
//
// Author     : Ursula Berthon (berthon@in2p3.fr)
// Created    : 11.12.2002// Last update: $Date: 2003/10/10 21:15:29 $
//          by: $Author: wessling $
// 
// Modified for Charm Particle Production Analysis
///////////////////////////////////////////////////////
#include <stdlib.h>
#include <iostream>

// ROOT includes
#include <TFile.h>
#include <TROOT.h>

// H1 OO includes
#include "H1Skeleton/H1Tree.h"
#include "H1Skeleton/H1EventList.h"
#include "H1Pointers/H1FloatPtr.h"
#include "H1Steering/H1StdCmdLine.h"
#include "H1Mods/H1PartEmArrayPtr.h"
#include "H1Mods/H1PartEm.h"
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
    H1FloatPtr q2e("Q2e");
    H1FloatPtr q2egen("Q2eGen");
    H1FloatPtr eE("ElecE");
    H1FloatPtr ye("Ye");
    H1FloatPtr vtxZ("VtxZ");
    H1FloatPtr epz("Epz");

    //Values for Epz calculation
    H1FloatPtr ElecTheta("ElecTheta");
    H1FloatPtr ElecPhi("ElecPhi");
    H1FloatPtr ElecE("ElecE");
    H1FloatPtr HadPzX("HadPzX");
    H1FloatPtr HadEX("HadEX");
    H1FloatPtr hfsE("FullHfsCombE");
    H1FloatPtr hfsTheta("FullHfsCombTheta");

    static H1PartEmArrayPtr PartEm;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Defining TTrees to event info
    TTree* EventTree = new TTree("MC Rec Event Tree", "MC Rec Event Tree");
    double elecPhi, elecTheta, Epz, elecE, x, Q2e, Ye, RTheta, ClusterRadius; 
    //Defining Tree Branches
    EventTree->Branch("elecPhi", &elecPhi, "elecPhi/D");
    EventTree->Branch("elecTheta", &elecTheta, "elecTheta/D");
    EventTree->Branch("x", &x, "x/D");
    EventTree->Branch("Q2e", &Q2e, "Q2e/D");
    EventTree->Branch("Ye", &Ye, "Ye/D");
    EventTree->Branch("Epz", &Epz, "Epz/D");
    EventTree->Branch("elecE", &elecE, "elecE/D");
    EventTree->Branch("RTheta", &RTheta, "RTheta/D");
    EventTree->Branch("ClusterRadius", &ClusterRadius, "ClusterRadius/D");
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //initializing counts
    double TriggerEvents = 0;
    double Q2Events = 0;
    double yEvents = 0;
    double eEnergyEvents = 0;
    double zvtxEvents = 0;
    double ClusterREvents = 0;
    double EnergyBalanceEvents = 0;
    double SpaCalEvents = 0;

    //initializing PartEM checks
    double scatelCount = 0;
    

    Int_t sel=0;
    while (H1Tree::Instance()->Next()) {
        TriggerEvents++;

        //cout << "Event " << TriggerEvents << endl;

        scatelCount = 0;

        for (Int_t loop = 0; loop < PartEm.GetEntries(); loop++) {
            
            if (PartEm[loop]->IsScatElec() == true) { 
                scatelCount++;
                ClusterRadius = PartEm[loop]->GetRadiusFromVectors();
            }

        }

        if (scatelCount!=1) { continue; }

        //cout << "Num scattered electrons: " << scatelCount << endl;

        //cout << "Cluster Radius: " << ClusterRadius << endl;
        
        if (*q2e < 2 || *q2e>100) { continue; }
        Q2Events++;
        
        if (*ye<0.1 || *ye>0.6) { continue; }
        yEvents++;

        if (*eE<11) { continue; }
        eEnergyEvents++;

        if (*vtxZ<-30 || *vtxZ>30) { continue; }
        zvtxEvents++;

        if (*epz<35 || *epz>70) { continue; }
        EnergyBalanceEvents++;

        if (abs((*vtxZ + 160) * tan(*ElecTheta)) < 9.1) { continue; }
        SpaCalEvents++;

        if (ClusterRadius > 3.5) { continue; }
        ClusterREvents++;

      //if (*ye < 0.6 && *ye>0.1 && *q2e > 5 && *q2e < 100 && *eE>10 && *epz > 35 && *epz < 70 && *vtxZ < 30 && *vtxZ > -30)    {
        //cout << "E: " << *Echeck << endl;
        //cout << "EPZ: " << *epz << endl;
        //cout << "EPZ calc: " <<  ( * ElecE) + abs((*ElecE)*cos(*ElecTheta)) + (*HadEX)-(*HadPzX) << endl;
        //cout << "EPZ calc: " <<  ( * ElecE) + abs((*ElecE)*cos(*ElecTheta)) + (*hfsE) - abs((*hfsE) * cos(*hfsTheta)) << endl;
          elecE = *eE;
          Epz = *epz;
          elecPhi = *ElecPhi;
          elecTheta = *ElecTheta;
          x = (*q2e) / ((*ye) * 920 * 27.6 * 4);
          Q2e = *q2e;
          Ye = *ye;
          RTheta = abs((*vtxZ + 160) * tan(*ElecTheta));

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
    TFile fileEventDist("/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/EventDistsMC06e.root","RECREATE");
    EventTree->Write();
    fileEventDist.Write();
    fileEventDist.Close();

    cout << "Eventlist written to " << opts.GetOutput() << endl;
    cout << "Event Dists written to " << "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/EventDistsMC06e.root" << endl;

    cout << "Events After Trigger: " << TriggerEvents << endl;
    cout << "Events After Q2: " << Q2Events << endl;
    cout << "Events After y: " << yEvents << endl;
    cout << "Events After eE: " << eEnergyEvents << endl;
    cout << "Events After zvtx: " << zvtxEvents << endl;
    cout << "Events After Epz: " << EnergyBalanceEvents << endl;
    cout << "Events After RTheta: " << SpaCalEvents << endl;
    cout << "Events After Cluster Radius: " << ClusterREvents << endl;
    //cout << "Events After Trigger: " << TriggerEvents << endl;

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


