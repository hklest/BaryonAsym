///////////////////////////////////////////////////////
// Example of 
// how to create an eventlist by entering events one by one
// and write it to a file afterwards
//
// Author     : Ursula Berthon (berthon@in2p3.fr)
// Created    : 11.12.2002// Last update: $Date: 2003/10/10 21:15:29 $
//          by: $Author: wessling $
//
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
#include "H1Mods/H1PartMCArrayPtr.h"
#include "H1Mods/H1PartMC.h"

using namespace std;

int
main(int argc, char* argv[])
{
    // parse the command line
    H1StdCmdLine opts;
    opts.Parse(&argc, argv);

      // Load mODS/HAT files
    H1Tree::Instance()->Open();            // this statement must be there!

     //These are the parameters used for MC selection on generator level
    H1Tree::Instance()->SelectHat("Q2eGen>5.");
    H1Tree::Instance()->SelectHat("YeGen>0.01");
    H1Tree::Instance()->SelectHat("YeGen<0.7");

     // do a selection, these are the parameters used for reconsturcted MC data selection
    H1Tree::Instance()->SelectHat("Q2e>5.");
    H1Tree::Instance()->SelectHat("ElecE>10.");
    H1Tree::Instance()->SelectHat("Ye>0.01");
    H1Tree::Instance()->SelectHat("Ye<0.7");
    H1Tree::Instance()->SelectHat("Epz>35.");
    H1Tree::Instance()->SelectHat("Epz<70.");
    H1Tree::Instance()->SelectHat("fIl1rw[61]>0");
    
      // create new H1EventList
    H1EventList *list= new H1EventList("mylist");

      // and enter interesting events into this list
    //Gen level
    H1FloatPtr q2egen("Q2eGen");
    H1FloatPtr yegen("YeGen");
    //Reconstructed
    H1FloatPtr q2e("Q2e");
    H1FloatPtr eE("ElecE");
    H1FloatPtr ye("Ye");
    H1FloatPtr vtxZ("VtxZ");
    H1FloatPtr epz("Epz");


    //H1FloatPtr N_L("NumLambda");
    //H1FloatPtr N_K("NumK0");

    //For each event need to select proper pT and Eta ranges, so need pointer to H1PartMC
    static H1PartMCArrayPtr gtrdata;

    //Initialize particle counts
    Int_t LambdaCount = 0;
    Int_t LambdaBarCount = 0;
    Int_t K0Count = 0;
    Int_t LambdaCCount = 0;
    Int_t LambdaCBarCount = 0;
    Int_t D0Count = 0;
    Int_t D0BarCount = 0;

    Int_t sel=0;
    while (H1Tree::Instance()->Next()) {
        //Counting particles by checking PDGs
        //Looping over H1PartMCArray associated with this event to grab all particles meeting conditions
        for (Int_t i = 0; i < gtrdata.GetEntries(); i++) {
            //First impose Generator Level HAT conditions from above
            if (*q2egen <= 5 || *yegen <= 0.01 || *yegen >= 0.7) { continue; }
            //Now check pT and Eta bounds and count if it passes
            //Make sure entries are particles with pT and Eta info
            H1PartMC* part = gtrdata[i];
            //if (!part->GetPt()) { continue; }
            //if (!part->GetEta()) { continue; }
            //if (!part->GetPDG()) { continue; }
            
            //cout << part->GetPt() << endl;
            //cout << part->GetEta() << endl;
            if (part->GetPt() > 0.5 && part->GetPt() < 3.5){//&& abs(part->GetEta()) < 1.3) {
                //cout << part->GetPDG() << endl;
                if (part->GetPDG() == 3122) { LambdaCount++; }
                if (part->GetPDG() == -3122) { LambdaBarCount++; }
                if (part->GetPDG() == 310) { K0Count++; }
                if (part->GetPDG() == 4122) { LambdaCCount++; }
                if (part->GetPDG() == -4122) { LambdaCBarCount++; }
                if (part->GetPDG() == 421) { D0Count++; }
                if (part->GetPDG() == -421) { D0BarCount++; }


            }
        }

        //Entering values into data list
      if (*ye<0.7 && *ye>0.01 && *q2e>5 && *eE>10 && *vtxZ<30 && *vtxZ>-30 && *epz>35 && *epz<70)    {
        sel++;
        list->Enter();
      }
    }
    cout << "Selected "<<sel<<" entries"<<endl;


      // Write eventlist to file
    TFile file(opts.GetOutput(), "RECREATE");
    list->Write();
    list->Print();
    // close
    file.Write();
    file.Close();

    cout << "Eventlist written to " << opts.GetOutput() << endl;
    cout << "Lambda Count: " << LambdaCount << endl;
    cout << "Lambda Bar Count: " << LambdaBarCount << endl;
    cout << "K0 Count: " << K0Count << endl;
    cout << "LambdaC Count: " << LambdaCCount << endl;
    cout << "LambdaC Bar Count: " << LambdaCBarCount << endl;
    cout << "D0 Count: " << LambdaCBarCount << endl;
    cout << "D0 Bar Count: " << LambdaCBarCount << endl;
    return 0;
}












