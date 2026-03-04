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
#include "H1Tools/H1RunList.h"
#include "H1Geom/H1DetectorStatus.h"

using namespace std;


int
main(int argc, char* argv[])
{
    //Parameters to change by hand before running: The number of events desired from the MC set specified in the steering
    //For combining the s,c, and b quark data sets, change these variables and run for each and then merge the root files into one event list
    //Then I will get event distributions for these individually to compare

    //double eventsneeded = 5336698; //nevents of c sample needed for int Lumi of 126.88 pb-1
    double eventsneeded = 92262; //nevents of b sample needed for int Lumi of 126.88 pb-1


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
    //H1Tree::Instance()->SelectHat("fIl1ac[61]>0");
    
      // create new H1EventList
    H1EventList *list= new H1EventList("mylist");

    
    double nevents = 0;

    Int_t sel=0;
    while (H1Tree::Instance()->Next()) {

        if (nevents == eventsneeded) { break; } //break out of the loop once we have the specific number of events we want

        
        list->Enter();

        nevents++;

    
    }
    

    cout << "Total Events: " << nevents << endl;

    // Write eventlist to file
    TFile file(opts.GetOutput(), "RECREATE");
    list->Write();
    list->Print();
    // close
    file.Write();
    file.Close();

    
    
    
    return 0;

    }



