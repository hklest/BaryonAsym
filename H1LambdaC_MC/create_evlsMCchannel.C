///////////////////////////////////////////////////////
//NEED TO RECOPY CREATE_EVLS.C CODE FROM LAMBDAC FOLDER, MODIFIED 
// TO ONLY FIND EVENTS WITH CHARM LAMBDA IN IT
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

    TTree* lambdaCTree = new TTree("LambdaC", "LambdaC");
    double pT, Eta, x, Q2e, Ye, Phi;
    //Defining Branches
    lambdaCTree->Branch("pT", &pT, "pT/D");
    lambdaCTree->Branch("Eta", &Eta, "Eta/D");
    lambdaCTree->Branch("x", &x, "x/D");
    lambdaCTree->Branch("Q2e", &Q2e, "Q2e/D");
    lambdaCTree->Branch("Ye", &Ye, "Ye/D");
    lambdaCTree->Branch("Phi", &Phi, "Phi/D");

      // Load mODS/HAT files
    H1Tree::Instance()->Open();            // this statement must be there!

     // do a selection
    H1Tree::Instance()->SelectHat("Q2e>5.");
    H1Tree::Instance()->SelectHat("Q2e<100.");
    H1Tree::Instance()->SelectHat("ElecE>10.");
    H1Tree::Instance()->SelectHat("Ye>0.1");
    H1Tree::Instance()->SelectHat("Ye<0.6");
    H1Tree::Instance()->SelectHat("Epz>35.");
    H1Tree::Instance()->SelectHat("Epz<70.");
    H1Tree::Instance()->SelectHat("fIl1rw[61]>0");
    
      // create new H1EventList
    H1EventList *list= new H1EventList("mylist");

      // and enter interesting events into this list
    H1FloatPtr q2e("Q2e");
    H1FloatPtr eE("ElecE");
    H1FloatPtr ye("Ye");
    H1FloatPtr vtxZ("VtxZ");
    H1FloatPtr epz("Epz");


    H1FloatPtr q2egen("Q2eGen");
    H1FloatPtr yegen("YeGen");

    static H1PartMCArrayPtr gtrdata;//delete later
    double lambdaCcount = 0;//delete later
    double lambdaCchannelcount = 0;
    double lambdaCchannelcountpT = 0;
    //Only selecting events that meet out typical analysis criteria,
    //then I am only selecting events that have LambdaC particles generated
    //I can't select specific LambdaC decay channels or only LambdaC products,
    //I can only ensure that every even has a lambdaC product
    int lambdaCFlag;
    int lambdaCspecificchannelFlag;
    Int_t sel=0;
    while (H1Tree::Instance()->Next()) {
      if (*ye < 0.6 && *ye>0.1 && *q2e > 5 && *q2e < 100 && *eE>10 && *epz > 35 && *epz < 70 && *vtxZ < 30 && *vtxZ > -30)    {
          lambdaCFlag = 0;//reset the LambdaC flag before looking for it again
          lambdaCspecificchannelFlag = 0;
          
          for (Int_t i = 0; i < gtrdata.GetEntries(); i++) {
              H1PartMC* part = gtrdata[i];
              if (part->IsGtr() == false) { continue; } //This excludes all detector level info in H1PartMC class
              if (part->GetPDG() == 4122 || part->GetPDG() == -4122) {
                  lambdaCcount++;
                  lambdaCFlag++;
                  //cout << "First Daughter Index: " << part->GetDaughter1() << endl;
                  //cout << "Second Daughter Index: " << part->GetDaughter1() << endl;
                  //selecting decay channels with only two Daughters
                  if (abs(part->GetDaughter1()- part->GetDaughter2())==1) {
                      if (part->GetPDG() == 4122) {
                          if ((gtrdata[part->GetDaughter1()]->GetPDG()==3122 && gtrdata[part->GetDaughter2()]->GetPDG()==211) || (gtrdata[part->GetDaughter1()]->GetPDG() == 211 && gtrdata[part->GetDaughter2()]->GetPDG() == 3122)) {
                              lambdaCchannelcount++;
                              if (part->GetPt() > 0.5) { lambdaCchannelcountpT++; }
                              lambdaCspecificchannelFlag++;
                              //cout << "Event Products: " << gtrdata.GetEntries() << endl;
                              //cout << "pT: " << part->GetPt() << endl;
                              //cout << "Daughter 1 PDG: " << gtrdata[part->GetDaughter1()]->GetPDG() << endl;
                              //cout << "Daughter 2 PDG: " << gtrdata[part->GetDaughter2()]->GetPDG() << endl;
                              pT = part->GetPt();
                              Eta = part->GetEta();
                              Phi = part->GetPhi();
                              x = (*q2egen) / ((*yegen) * 920 * 27.6 * 4);
                              Q2e = *q2egen;
                              Ye = *yegen;
                              lambdaCTree->Fill();
                          }
                      }
                      if (part->GetPDG() == -4122) {
                          if ((gtrdata[part->GetDaughter1()]->GetPDG() == -3122 && gtrdata[part->GetDaughter2()]->GetPDG() == -211) || (gtrdata[part->GetDaughter1()]->GetPDG() == -211 && gtrdata[part->GetDaughter2()]->GetPDG() == -3122)) {
                              lambdaCchannelcount++;
                              if (part->GetPt() > 0.5) { lambdaCchannelcountpT++; }
                              lambdaCspecificchannelFlag++;
                              //cout << "Event Products: " << gtrdata.GetEntries() << endl;
                              //cout << "pT: " << part->GetPt() << endl;
                              //cout << "Daughter 1 PDG: " << gtrdata[part->GetDaughter1()]->GetPDG() << endl;
                              //cout << "Daughter 2 PDG: " << gtrdata[part->GetDaughter2()]->GetPDG() << endl;
                              pT = part->GetPt();
                              Eta = part->GetEta();
                              Phi = part->GetPhi();
                              x = (*q2egen) / ((*yegen) * 920 * 27.6 * 4);
                              Q2e = *q2egen;
                              Ye = *yegen;
                              lambdaCTree->Fill();
                          }
                      }
                  }
              }//loop going through all event particles looking for LambdaC and adding to Flag
          }
          if (lambdaCspecificchannelFlag != 0) {
              sel++;
              list->Enter();
          }

      }
    }
    

      // Write eventlist to file
    TFile file(opts.GetOutput(), "RECREATE");
    list->Write();
    list->Print();
    // close
    file.Write();
    file.Close();

    TFile f("/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/lambdaCDists.root", "RECREATE");


    lambdaCTree->Write();

    f.Write();
    f.Close();

    cout << "Selected " << sel << " entries" << endl;
    cout << "LambdaC count: " << lambdaCcount << endl;
    cout << "LambdaC Channel count: " << lambdaCchannelcount << endl;
    cout << "LambdaC Channel count with pT > 0.5: " << lambdaCchannelcountpT << endl;
    cout << "Eventlist written to " << opts.GetOutput() << endl;
    return 0;
}


