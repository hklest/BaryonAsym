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
    H1FloatPtr eE("ElecE");
    H1FloatPtr ye("Ye");
    H1FloatPtr vtxZ("VtxZ");
    H1FloatPtr epz("Epz");
    H1FloatPtr elecE("ElecE");

    //Getting Beam energies
    H1FloatPtr Ee("fEBeamE");
    H1FloatPtr Ep("fEBeamP");

    //Values for Epz calculation
    H1FloatPtr ElecTheta("ElecTheta");
    H1FloatPtr ElecPhi("ElecPhi");
    H1FloatPtr ElecE("ElecE");
    H1FloatPtr HadPzX("HadPzX");
    H1FloatPtr HadEX("HadEX");
    H1FloatPtr hfsE("FullHfsCombE");
    H1FloatPtr hfsTheta("FullHfsCombTheta");

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Defining TTrees to event info
    TTree* EventTree = new TTree("Data Event Tree", "Data Event Tree");
    double elecPhi, elecTheta, Epz, elecE, x, Q2e, Ye, VtxZ; 
    //Defining Tree Branches
    EventTree->Branch("elecPhi", &elecPhi, "elecPhi/D");
    EventTree->Branch("elecTheta", &elecTheta, "elecTheta/D");
    EventTree->Branch("x", &x, "x/D");
    EventTree->Branch("Q2e", &Q2e, "Q2e/D");
    EventTree->Branch("Ye", &Ye, "Ye/D");
    EventTree->Branch("Epz", &Epz, "Epz/D");
    EventTree->Branch("elecE", &elecE, "elecE/D");
    EventTree->Branch("vtxZ", &vtxZ, "vtxZ/D");
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    Int_t sel=0;
    while (H1Tree::Instance()->Next()) {
      if (*ye < 0.6 && *ye>0.1 && *q2e > 5 && *q2e < 100 && *eE>11 && *epz > 35 && *epz < 70 && *vtxZ < 30 && *vtxZ > -30)    {
        //cout << "E: " << *Echeck << endl;
        //cout << "EPZ: " << *epz << endl;
        //cout << "EPZ calc: " <<  ( * ElecE) + abs((*ElecE)*cos(*ElecTheta)) + (*HadEX)-(*HadPzX) << endl;
        //cout << "EPZ calc: " <<  ( * ElecE) + abs((*ElecE)*cos(*ElecTheta)) + (*hfsE) - abs((*hfsE) * cos(*hfsTheta)) << endl;
          elecE = *eE;
          Epz = *epz;
          elecPhi = *ElecPhi;
          elecTheta = *ElecTheta;
          x = (*q2e) / ((*ye) * (*Ee) * (*Ep) * 4);
          Q2e = *q2e;
          Ye = *ye;
          VtxZ = *vtxZ;

          EventTree->Fill();

          sel++;
          list->Enter();
      }
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
    TFile fileEventDist("/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/EventDists06test.root","RECREATE");
    EventTree->Write();
    fileEventDist.Write();
    fileEventDist.Close();

    cout << "Eventlist written to " << opts.GetOutput() << endl;
    cout << "Event Dists written to " << "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/EventDists06test.root" << endl;
    
    return 0;
}


