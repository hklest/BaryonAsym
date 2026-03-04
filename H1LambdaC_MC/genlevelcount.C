///////////////////////////////////////////////////////
//Loading in same MC data set as used for create_evlsMC
//and doing gen level counts for reconstruction efficiencies
///////////////////////////////////////////////////////
#include <stdlib.h>
#include <cmath>
#include <iostream>

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

using namespace std;
//Defining histograms
TH1F* hgenlambdaregpT = new TH1F("Lambda Reg Gen Level pT Dist", "Lambda Reg Gen Level pT Dist", 2000, 0, 20);
TH1F* hgenlambdaregEta = new TH1F("Lambda Reg Gen Level Eta Dist", "Lambda Reg Gen Level Eta Dist", 2200, -10, 12);
TH1F* hgenlambdabarpT = new TH1F("Lambda Bar Gen Level pT Dist", "Lambda Bar Gen Level pT Dist", 2000, 0, 20);
TH1F* hgenlambdabarEta = new TH1F("Lambda Bar Gen Level Eta Dist", "Lambda Bar Gen Level Eta Dist", 2200, -10, 12);

TH1F* hgenlambdaregq2 = new TH1F("Lambda Reg Gen Level pT Dist", "Lambda Reg Gen Level pT Dist", 1500, 0, 150);
TH1F* hgenlambdaregy = new TH1F("Lambda Reg Gen Level Eta Dist", "Lambda Reg Gen Level Eta Dist", 100, 0, 1);
TH1F* hgenlambdabarq2 = new TH1F("Lambda Bar Gen Level pT Dist", "Lambda Bar Gen Level pT Dist", 1500, 0, 150);
TH1F* hgenlambdabary = new TH1F("Lambda Bar Gen Level Eta Dist", "Lambda Bar Gen Level Eta Dist", 100, 0, 1);

TH1F* hlambdaregcountpT = new TH1F("Lambda Reg Count vs pT", "Lambda Reg Count vs pT", 1000, 0, 10);
TH1F* hlambdaregcountEta = new TH1F("Lambda Reg Count vs Eta", "Lambda Reg Count vs Eta", 800, -4, 4);
TH1F* hlambdabarcountpT = new TH1F("Lambda Bar Count vs pT", "Lambda Bar Count vs pT", 1000, 0, 10);
TH1F* hlambdabarcountEta = new TH1F("Lambda Bar Count vs Eta", "Lambda Bar Count vs Eta", 800, -4, 4);

TH1F* hlambdaregcountq2 = new TH1F("Lambda Reg Count vs q2", "Lambda Reg Count vs q2", 1500, 0, 150);
TH1F* hlambdaregcounty = new TH1F("Lambda Reg Count vs y", "Lambda Reg Count vs y", 100, 0, 1);
TH1F* hlambdabarcountq2 = new TH1F("Lambda Bar Count vs q2", "Lambda Bar Count vs q2", 1500, 0, 150);
TH1F* hlambdabarcounty = new TH1F("Lambda Bar Count vs y", "Lambda Bar Count vs y", 100, 0, 1);

TH1F* hlambdaregcountX = new TH1F("Lambda Reg Count vs Bjorken X", "Lambda Reg Count vs Bjorken X", 5000, 0, 0.05);
TH1F* hlambdabarcountX = new TH1F("Lambda Bar Count vs Bjorken X", "Lambda Bar Count vs Bjorken X", 5000, 0, 0.05);

TH1F* hlambdaregcountphi = new TH1F("Lambda Reg Count vs Phi", "Lambda Reg Count vs Phi", 1400, -7, 7);
TH1F* hlambdabarcountphi = new TH1F("Lambda Bar Count vs Phi", "Lambda Bar Count vs Phi", 1400, -7, 7);

TH1F* hlambdaregcounttheta = new TH1F("Lambda Reg Count vs Theta", "Lambda Reg Count vs Theta", 1400, -7, 7);
TH1F* hlambdabarcounttheta = new TH1F("Lambda Bar Count vs Theta", "Lambda Bar Count vs Theta", 1400, -7, 7);
///////////////Gen level Pions, Kaons, and protons to help understand Lambda Distributions
TH1F* hgenlambdaregX = new TH1F("Lambda Reg Count vs Bjorken X", "Lambda Reg Count vs Bjorken X", 5000, 0, 0.05);
TH1F* hgenlambdabarX = new TH1F("Lambda Bar Count vs Bjorken X", "Lambda Bar Count vs Bjorken X", 5000, 0, 0.05);


TH1F* hgenprotonregpT = new TH1F("Proton Gen Level pT Dist", "Proton Gen Level pT Dist", 2000, 0, 20);
TH1F* hgenprotonregEta = new TH1F("Proton Gen Level Eta Dist", "Proton Gen Level Eta Dist", 2200, -10, 12);
TH1F* hgenprotonbarpT = new TH1F("Anti Proton Gen Level pT Dist", "Anti Proton Gen Level pT Dist", 2000, 0, 20);
TH1F* hgenprotonbarEta = new TH1F("Anti Proton Gen Level Eta Dist", "Anti Proton Gen Level Eta Dist", 2200, -10, 12);
TH1F* hgenprotonregX = new TH1F("Proton Gen Level X Dist", "Proton Gen Level X Dist", 5000, 0, 0.05);
TH1F* hgenprotonbarX = new TH1F("Anti Proton Gen Level X Dist", "Anti Proton Gen Level X Dist", 5000, 0, 0.05);

TH1F* hgenprotonregq2 = new TH1F("Proton Gen Level Q2 Dist", "Proton Gen Level Q2 Dist", 1500, 0, 150);
TH1F* hgenprotonregy = new TH1F("Proton Gen Level Eta Dist", "Proton Gen Level Eta Dist", 100, 0, 1);
TH1F* hgenprotonbarq2 = new TH1F("Anti Proton Gen Level Q2 Dist", "Anti Proton Gen Level Q2 Dist", 1500, 0, 150);
TH1F* hgenprotonbary = new TH1F("Anti Proton Gen Level Eta Dist", "Anti Proton Gen Level Eta Dist", 100, 0, 1);

TH1F* hgenpipluspT = new TH1F("Pi Plus Gen Level pT Dist", "Pi Plus Gen Level pT Dist", 2000, 0, 20);
TH1F* hgenpiplusEta = new TH1F("Pi Plus Gen Level Eta Dist", "Pi Plus Gen Level Eta Dist", 2200, -10, 12);
TH1F* hgenpiminuspT = new TH1F("Pi Minus Gen Level pT Dist", "Pi Minus Gen Level pT Dist", 2000, 0, 20);
TH1F* hgenpiminusEta = new TH1F("Pi Minus Gen Level Eta Dist", "Pi Minus Gen Level Eta Dist", 2200, -10, 12);
TH1F* hgenpiplusX = new TH1F("Pi Minus Gen Level X Dist", "Pi Minus Gen Level X Dist", 5000, 0, 0.05);
TH1F* hgenpiminusX = new TH1F("Pi Plus Gen Level X Dist", "Pi Plus Gen Level X Dist", 5000, 0, 0.05);

TH1F* hgenpiplusq2 = new TH1F("Pi Plus Gen Level Q2 Dist", "Pi Plus Gen Level Q2 Dist", 1500, 0, 150);
TH1F* hgenpiplusy = new TH1F("Pi Plus Gen Level Eta Dist", "Pi Plus Gen Level Eta Dist", 100, 0, 1);
TH1F* hgenpiminusq2 = new TH1F("Pi Minus Gen Level Q2 Dist", "Pi Minus Gen Level Q2 Dist", 1500, 0, 150);
TH1F* hgenpiminusy = new TH1F("Pi Minus Gen Level Eta Dist", "Pi Minus Gen Level Eta Dist", 100, 0, 1);

TH1F* hgenKpluspT = new TH1F("Pi Plus Gen Level pT Dist", "Pi Plus Gen Level pT Dist", 2000, 0, 20);
TH1F* hgenKplusEta = new TH1F("Pi Plus Gen Level Eta Dist", "Pi Plus Gen Level Eta Dist", 2200, -10, 12);
TH1F* hgenKminuspT = new TH1F("Pi Minus Gen Level pT Dist", "Pi Minus Gen Level pT Dist", 2000, 0, 20);
TH1F* hgenKminusEta = new TH1F("Pi Minus Gen Level Eta Dist", "Pi Minus Gen Level Eta Dist", 2200, -10, 12);
TH1F* hgenKplusX = new TH1F("K Minus Gen Level X Dist", "K Minus Gen Level X Dist", 5000, 0, 0.05);
TH1F* hgenKminusX = new TH1F("K Plus Gen Level X Dist", "K Plus Gen Level X Dist", 5000, 0, 0.05);

TH1F* hgenKplusq2 = new TH1F("K Plus Gen Level Q2 Dist", "K Plus Gen Level Q2 Dist", 1500, 0, 150);
TH1F* hgenKplusy = new TH1F("K Plus Gen Level Eta Dist", "K Plus Gen Level Eta Dist", 100, 0, 1);
TH1F* hgenKminusq2 = new TH1F("K Minus Gen Level Q2 Dist", "K Minus Gen Level Q2 Dist", 1500, 0, 150);
TH1F* hgenKminusy = new TH1F("K Minus Gen Level Eta Dist", "K Minus Gen Level Eta Dist", 100, 0, 1);

////////////////////////////diff hists
TH1F* hgenKdiffy = new TH1F("K+ - K- Gen Level y Dist", "K+ - K- Gen Level y Dist", 100, 0, 1);
TH1F* hgenKdiffX = new TH1F("K+ - K- Gen Level X Dist", "K+ - K- Gen Level X Dist", 5000, 0, 0.05);
TH1F* hgenKdiffpT = new TH1F("K+ - K- Gen Level pT Dist", "K+ - K- Gen Level pT Dist", 2000, 0, 20);
TH1F* hgenKdiffEta = new TH1F("K+ - K- Gen Level Eta Dist", "K+ - K- Gen Level Eta Dist", 2200, -10, 12);
TH1F* hgenKdiffq2 = new TH1F("K+ - K- Gen Level q2 Dist", "K+ - K- Gen Level q2 Dist", 1500, 0, 150);

TH1F* hgenLdiffy = new TH1F("#Lambda - #bar{#Lambda} Gen Level y Dist", "#Lambda - #bar{#Lambda} Gen Level y Dist", 100, 0, 1);
TH1F* hgenLdiffX = new TH1F("#Lambda - #bar{#Lambda} Gen Level X Dist", "#Lambda - #bar{#Lambda} Gen Level X Dist", 5000, 0, 0.05);
TH1F* hgenLdiffpT = new TH1F("#Lambda - #bar{#Lambda} Gen Level pT Dist", "#Lambda - #bar{#Lambda} Gen Level pT Dist", 2000, 0, 20);
TH1F* hgenLdiffEta = new TH1F("#Lambda - #bar{#Lambda} Gen Level Eta Dist", "#Lambda - #bar{#Lambda} - Gen Level Eta Dist", 2200, -10, 12);
TH1F* hgenLdiffq2 = new TH1F("#Lambda - #bar{#Lambda} Gen Level q2 Dist", "#Lambda - #bar{#Lambda} Gen Level q2 Dist", 1500, 0, 150);


////////////////////////////////////////////////////////////////////////////////////////////////////////

THStack* hspT = new THStack("Generator Level pT Distributions", "Generator Level pT Distributions");
THStack* hsEta = new THStack("Generator Level #eta Distributions", "Generator Level #eta Distributions");
THStack* hsq2 = new THStack("Generator Level Q#^{2} Distributions", "Generator Level Q#^{2} Distributions");
THStack* hsy = new THStack("Generator Level y Distributions", "Generator Level y Distributions");
THStack* hsphi = new THStack("Generator Level Phi Distributions", "Generator Level Phi Distributions");
THStack* hstheta = new THStack("Generator Level Theta Distributions", "Generator Level Theta Distributions");

THStack* hscountpT = new THStack("Generator Level pT Distributions", "Generator Level pT Distributions");
THStack* hscountEta = new THStack("Generator Level #eta Distributions", "Generator Level #eta Distributions");
THStack* hscountq2 = new THStack("Generator Level Q#^{2} Distributions", "Generator Level Q#^{2} Distributions");
THStack* hscounty = new THStack("Generator Level y Distributions", "Generator Level y Distributions");
THStack* hscountX = new THStack("Generator Level Bjorken x Distributions", "Generator Level Bjorken x Distributions");

THStack* hsPiX = new THStack("Generator Level Bjorken x Distributions", "Generator Level Bjorken x Distributions");
THStack* hsPipT = new THStack("Generator Level pT Distributions", "Generator Level pT Distributions");
THStack* hsPiq2 = new THStack("Generator Level q2 Distributions", "Generator Level q2 Distributions");
THStack* hsPiEta = new THStack("Generator Level Eta Distributions", "Generator Level Eta Distributions");
THStack* hsPiy = new THStack("Generator Level y Distributions", "Generator Level y Distributions");

THStack* hsKX = new THStack("Generator Level Bjorken x Distributions", "Generator Level Bjorken x Distributions");
THStack* hsKpT = new THStack("Generator Level pT Distributions", "Generator Level pT Distributions");
THStack* hsKq2 = new THStack("Generator Level q2 Distributions", "Generator Level q2 Distributions");
THStack* hsKEta = new THStack("Generator Level Eta Distributions", "Generator Level Eta Distributions");
THStack* hsKy = new THStack("Generator Level y Distributions", "Generator Level y Distributions");

THStack* hsProtonX = new THStack("Generator Level Bjorken x Distributions", "Generator Level Bjorken x Distributions");
THStack* hsProtonpT = new THStack("Generator Level pT Distributions", "Generator Level pT Distributions");
THStack* hsProtonq2 = new THStack("Generator Level q2 Distributions", "Generator Level q2 Distributions");
THStack* hsProtonEta = new THStack("Generator Level Eta Distributions", "Generator Level Eta Distributions");
THStack* hsProtony = new THStack("Generator Level y Distributions", "Generator Level y Distributions");

THStack* hsKandLcompX = new THStack("Generator Level X Distributions", "Generator Level X Distributions");
THStack* hsKandLcomppT = new THStack("Generator Level pT Distributions", "Generator Level pT Distributions");
THStack* hsKandLcompq2 = new THStack("Generator Level q2 Distributions", "Generator Level q2 Distributions");
THStack* hsKandLcompEta = new THStack("Generator Level Eta Distributions", "Generator Level Eta Distributions");
THStack* hsKandLcompy = new THStack("Generator Level y Distributions", "Generator Level y Distributions");

TH1F* hlambdaAsymmpT = new TH1F("Lambda Asymm vs pT", "Lambda Asymm vs pT", 1000, 0, 10);
TH1F* hlambdaAsymmEta = new TH1F("Lambda Asymm vs Eta", "Lambda Asymm vs Eta", 800, -4, 4);
TH1F* hlambdaAsymmq2 = new TH1F("Lambda Asymm vs q2", "Lambda Asymm vs q2", 1500, 0, 150);
TH1F* hlambdaAsymmX = new TH1F("Lambda Asymm vs X", "Lambda Asymm vs X", 5000, 0, 0.05);
TH1F* hlambdaAsymmphi = new TH1F("Lambda Asymm vs Phi", "Lambda Asymm vs Phi", 800, -4, 4);

TH1F* hlambdaregsigmapT = new TH1F("Lambda Reg pT Cross Section", "Lambda Reg pT Cross Section", 1000, 0, 10);
TH1F* hlambdaregsigmaEta = new TH1F("Lambda Reg Eta Cross Section", "Lambda Reg Eta Cross Section", 800, -4, 4);
TH1F* hlambdaregsigmaq2 = new TH1F("Lambda Reg q2 Cross Section", "Lambda Reg q2 Cross Section", 1500, 0, 150);
TH1F* hlambdaregsigmaX = new TH1F("Lambda Reg X Cross Section", "Lambda Reg X Cross Section", 5000, 0, 0.05);
TH1F* hlambdaregsigmaphi = new TH1F("Lambda Reg Phi Cross Section", "Lambda Reg Phi Cross Section", 800, -4, 4);

TH1F* hlambdabarsigmapT = new TH1F("Lambda Bar pT Cross Section", "Lambda Bar pT Cross Section", 1000, 0, 10);
TH1F* hlambdabarsigmaEta = new TH1F("Lambda Bar Eta Cross Section", "Lambda Bar Eta Cross Section", 800, -4, 4);
TH1F* hlambdabarsigmaq2 = new TH1F("Lambda Bar q2 Cross Section", "Lambda Bar q2 Cross Section", 1500, 0, 150);
TH1F* hlambdabarsigmaX = new TH1F("Lambda Bar X Cross Section", "Lambda Bar X Cross Section", 5000, 0, 0.05);
TH1F* hlambdabarsigmaphi = new TH1F("Lambda Bar Phi Cross Section", "Lambda Bar Phi Cross Section", 800, -4, 4);

TH1F* hlambdatotsigmapT = new TH1F("Lambda Tot pT Cross Section", "Lambda Tot pT Cross Section", 1000, 0, 10);
TH1F* hlambdatotsigmaEta = new TH1F("Lambda Tot Eta Cross Section", "Lambda Tot Eta Cross Section", 800, -4, 4);
TH1F* hlambdatotsigmaq2 = new TH1F("Lambda Tot q2 Cross Section", "Lambda Tot q2 Cross Section", 1500, 0, 150);
TH1F* hlambdatotsigmaX = new TH1F("Lambda Tot X Cross Section", "Lambda Tot X Cross Section", 5000, 0, 0.05);
TH1F* hlambdatotsigmaphi = new TH1F("Lambda Tot Phi Cross Section", "Lambda Tot Phi Cross Section", 800, -4, 4);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH1F* hEtaTest = new TH1F("Eta Test", "Eta Test", 800, -4, 4);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//defining a vector with bounds for pT and Eta bins
vector<vector<double>> pTbins = { {0.5,0.6},{0.6,0.7},{0.7,0.8},{0.8,0.9},{0.9,1.1},{1.1,1.3},{1.3,1.6},{1.6,2.2},{2.2,3.5} };
vector<vector<double>> Etabins = { {-1.3,-1},{-1,-0.75},{-0.75,-0.5},{-0.5,-0.25},{-0.25,0},{0,0.25},{0.25,0.50},{0.50,0.75},{0.75,1},{1,1.3} };
double HistBinsReg[9][10]; //HistBins[a][b] element corresponds to lambdas within pT bin a and Eta bin b
double HistBinsBar[9][10]; //HistBins[a][b] element corresponds to lambdas within pT bin a and Eta bin b
//initializing all counts to zero

///////////////////////////////////////////////////////////////
double MCTotalLumi = 2*63.44;//used two MC sets, int lumi addded for both
///////////////////////////////////////////////////////////////

TCanvas* canvas = new TCanvas("Plot", "Plot", 10, 10, 800, 800);

int
main(int argc, char* argv[])
{
    // parse the command line
    H1StdCmdLine opts;
    opts.Parse(&argc, argv);

      // Load mODS/HAT files
    H1Tree::Instance()->Open();            // this statement must be there!

     //These are the parameters used for MC selection on generator level
    //MAKE SURE THESE ARE SAME AS IN create_evlsMC! (but for generated values)
   
    H1Tree::Instance()->SelectHat("Q2eGen>5.");
    H1Tree::Instance()->SelectHat("Q2eGen<100.");
    H1Tree::Instance()->SelectHat("YeGen>0.1");
    H1Tree::Instance()->SelectHat("YeGen<0.6");

     
      // create new H1EventList
    //H1EventList *list= new H1EventList("mylist");

      // and enter interesting events into this list
    //Gen level
    H1FloatPtr q2egen("Q2eGen");
    H1FloatPtr yegen("YeGen");
    H1FloatPtr genweight("Weight1");
    H1FloatPtr genweight2("Weight2");
    H1FloatPtr vtxZ("VtxZ");
    

    //For each event need to select proper pT and Eta ranges, so need pointer to H1PartMC
    static H1PartMCArrayPtr gtrdata;
    static H1StrBankPtr strbank;

    double pTcheck = 0;
    //Initialize particle counts
    double LambdaCount = 0;
    double LambdaBarCount = 0;
    double K0Count = 0;
    double LambdaCCount = 0;
    double LambdaCBarCount = 0;
    double D0Count = 0;
    double D0BarCount = 0;
    //pt bin counting
    double ptbin1reg = 0;
    double ptbin2reg = 0;
    double ptbin3reg = 0;
    double ptbin4reg = 0;
    double ptbin5reg = 0;
    double ptbin6reg = 0;
    double ptbin7reg = 0;
    double ptbin8reg = 0;
    double ptbin9reg = 0;

    double ptbin1bar = 0;
    double ptbin2bar = 0;
    double ptbin3bar = 0;
    double ptbin4bar = 0;
    double ptbin5bar = 0;
    double ptbin6bar = 0;
    double ptbin7bar = 0;
    double ptbin8bar = 0;
    double ptbin9bar = 0;
   //Eta bin counting
    double Etabin1reg = 0;
    double Etabin2reg = 0;
    double Etabin3reg = 0;
    double Etabin4reg = 0;
    double Etabin5reg = 0;
    double Etabin6reg = 0;
    double Etabin7reg = 0;
    double Etabin8reg = 0;
    double Etabin9reg = 0;
    double Etabin10reg = 0;

    double Etabin1bar = 0;
    double Etabin2bar = 0;
    double Etabin3bar = 0;
    double Etabin4bar = 0;
    double Etabin5bar = 0;
    double Etabin6bar = 0;
    double Etabin7bar = 0;
    double Etabin8bar = 0;
    double Etabin9bar = 0;
    double Etabin10bar = 0;
    //Xbincounting
    double Xbin1reg = 0;
    double Xbin2reg = 0;
    double Xbin3reg = 0;
    double Xbin4reg = 0;
    double Xbin5reg = 0;

    double Xbin1bar = 0;
    double Xbin2bar = 0;
    double Xbin3bar = 0;
    double Xbin4bar = 0;
    double Xbin5bar = 0;
    //q2 bin counting
    double q2bin1reg = 0;
    double q2bin2reg = 0;
    double q2bin3reg = 0;
    double q2bin4reg = 0;
    double q2bin5reg = 0;
    double q2bin6reg = 0;
    double q2bin7reg = 0;
    double q2bin8reg = 0;
    double q2bin9reg = 0;

    double q2bin1bar = 0;
    double q2bin2bar = 0;
    double q2bin3bar = 0;
    double q2bin4bar = 0;
    double q2bin5bar = 0;
    double q2bin6bar = 0;
    double q2bin7bar = 0;
    double q2bin8bar = 0;
    double q2bin9bar = 0;
    //phi bin counting
    double phibin1reg = 0;
    double phibin2reg = 0;
    double phibin3reg = 0;
    double phibin4reg = 0;
    double phibin5reg = 0;
    double phibin6reg = 0;
    double phibin7reg = 0;
    double phibin8reg = 0;
    double phibin9reg = 0;
    double phibin10reg = 0;
    double phibin11reg = 0;
    double phibin12reg = 0;

    double phibin1bar = 0;
    double phibin2bar = 0;
    double phibin3bar = 0;
    double phibin4bar = 0;
    double phibin5bar = 0;
    double phibin6bar = 0;
    double phibin7bar = 0;
    double phibin8bar = 0;
    double phibin9bar = 0;
    double phibin10bar = 0;
    double phibin11bar = 0;
    double phibin12bar = 0;
   

    for (int a = 0; a < 9; a++) {
        for (int b = 0; b < 10; b++) {
            HistBinsReg[a][b] = 0;
            HistBinsBar[a][b] = 0;
        }
    }

    //defining the 2D hists to store lambda and anti-lambda counts in 2D pT vs Eta bins
    TH2F* LambdaReg2D = new TH2F("Lambda Reg Counts 2D", "Lambda Reg Counts 2D", 30, 0.5, 3.5, 52, -1.3, 1.3);
    TH2F* LambdaBar2D = new TH2F("Lambda Bar Counts 2D", "Lambda Bar Counts 2D", 30, 0.5, 3.5, 52, -1.3, 1.3);
    TH2F* Asymm2D = new TH2F("Asymmetry Counts 2D", "Asymmetry Counts 2D", 30, 0.5, 3.5, 52, -1.3, 1.3);


    //2D hists for vertex densities
    TH2F* XYvtxdensityreg = new TH2F("Reg Lambda XY Vertices", "Reg Lambda XY Vertices", 2000, -100, 100, 2000, -100, 100);
    TH2F* ZXvtxdensityreg = new TH2F("Reg Lambda ZX Vertices", "Reg Lambda ZX Vertices", 8000, -4000, 4000, 2000, -100, 100);
    TH2F* ZYvtxdensityreg = new TH2F("Reg Lambda ZY Vertices", "Reg Lambda ZY Vertices", 8000, -4000, 4000, 2000, -100, 100);

    TH2F* XYvtxdensitybar = new TH2F("Lambda Bar XY Vertices", "Lambda Bar XY Vertices", 2000, -100, 100, 2000, -100, 100);
    TH2F* ZXvtxdensitybar = new TH2F("Lambda Bar ZX Vertices", "Lambda Bar ZX Vertices", 8000, -4000, 4000, 2000, -100, 100);
    TH2F* ZYvtxdensitybar = new TH2F("Lambda Bar ZY Vertices", "Lambda Bar ZY Vertices", 8000, -4000, 4000, 2000, -100, 100);

    //2D hists for vertex densities
    TH2F* rXYvtxdensityreg = new TH2F("Range Reg Lambda XY Vertices", "Range Reg Lambda XY Vertices", 2000, -100, 100, 2000, -100, 100);
    TH2F* rZXvtxdensityreg = new TH2F("Range Reg Lambda ZX Vertices", "Range Reg Lambda ZX Vertices", 8000, -4000, 4000, 2000, -100, 100);
    TH2F* rZYvtxdensityreg = new TH2F("Range Reg Lambda ZY Vertices", "Range Reg Lambda ZY Vertices", 8000, -4000, 4000, 2000, -100, 100);

    TH2F* rXYvtxdensitybar = new TH2F("Range Lambda Bar XY Vertices", "Range Lambda Bar XY Vertices", 2000, -100, 100, 2000, -100, 100);
    TH2F* rZXvtxdensitybar = new TH2F("Range Lambda Bar ZX Vertices", "Range Lambda Bar ZX Vertices", 8000, -4000, 4000, 2000, -100, 100);
    TH2F* rZYvtxdensitybar = new TH2F("Range Lambda Bar ZY Vertices", "Range Lambda Bar ZY Vertices", 8000, -4000, 4000, 2000, -100, 100);

    //2D hists for vertex densities
    TH2F* dXYvtxdensityreg = new TH2F("Decay Reg Lambda XY Vertices", "Decay Reg Lambda XY Vertices", 2000, -100, 100, 2000, -100, 100);
    TH2F* dZXvtxdensityreg = new TH2F("Decay Reg Lambda ZX Vertices", "Decay Reg Lambda ZX Vertices", 8000, -4000, 4000, 2000, -100, 100);
    TH2F* dZYvtxdensityreg = new TH2F("Decay Reg Lambda ZY Vertices", "Decay Reg Lambda ZY Vertices", 8000, -4000, 4000, 2000, -100, 100);

    TH2F* dXYvtxdensitybar = new TH2F("Decay Lambda Bar XY Vertices", "Decay Lambda Bar XY Vertices", 2000, -100, 100, 2000, -100, 100);
    TH2F* dZXvtxdensitybar = new TH2F("Decay Lambda Bar ZX Vertices", "Decay Lambda Bar ZX Vertices", 8000, -4000, 4000, 2000, -100, 100);
    TH2F* dZYvtxdensitybar = new TH2F("Decay Lambda Bar ZY Vertices", "Decay Lambda Bar ZY Vertices", 8000, -4000, 4000, 2000, -100, 100);



    double nononeweightcount;
    Int_t sel=0;
    TLorentzVector totgen4vec;//is initialized by zero four vector by default
    double px = 0;
    double particles = 0;

    int ivtype1 = 0;
    int ivtype2 = 0;
    int ivtype3 = 0;
    int ivtype4 = 0;
    int ivtype5 = 0;

    int mivtype1 = 0;
    int mivtype2 = 0;
    int mivtype3 = 0;
    int mivtype4 = 0;
    int mivtype5 = 0;


    while (H1Tree::Instance()->Next()) {
        //Getting weight of the event
        if (*genweight != 1 || *genweight2!=1) {
            nononeweightcount++;
            cout << "Weight 1: " << *genweight << endl;
            cout << "Weight 2: " << *genweight2 << endl;
        }
        //Counting particles by checking PDGs
        //Looping over H1PartMCArray associated with this event to grab all particles meeting conditions
        for (Int_t i = 0; i < gtrdata.GetEntries(); i++) {
            H1PartMC* part = gtrdata[i];
            //making sure this is gen level event, H1PartMC in general also has STR bank with secondary particles from GEANT H1 detector simulations
            //if (part->IsGtr() == false || part->IsStable()==false) { continue; }
            if (part->IsGtr() == false) { continue; }
            H1SvxRow* svx = strbank[part->GetStrIdx()]->GetSvx();
            
            //cout <<  svx->GetX() << endl;
            //H1SelVertex* Vertex = (H1SelVertex*)part;
            //if (Vertex) { cout << "vertex" endl; }
            if (sel==0) {
                cout << "Inv Mass: " << part->GetFourVector().M() << endl;
                cout << "PDG Mass: " << part->GetMassPDG() << endl;
                totgen4vec = totgen4vec + part->GetFourVector();
                cout << totgen4vec.M() << endl;
                px = px+part->GetMomentum().X();
                particles++;
            }//adding up all gen particle four vectors, invariant mass of this should be 319 GeV with zero transverse momentum
            //First impose Generator Level HAT conditions from above
            if (*q2egen <= 5 || *q2egen >= 100 || *yegen <= 0.1 || *yegen >= 0.6 ) { continue; }
            
            //Before enfrocing pT and Eta ranges, will fill some particle distributions for some kinematic variables
            //DELETE BELOW LATER FOR CHECK
            /*H1CentralFittedV0* v0 = (H1CentralFittedV0*)part;
            if (!v0) { cout << "Reconstrcuted pT: " << "No Conversion" << endl; }
            if (v0) {
                Double_t pT = v0->GetPt();
                cout << "Reconstrcuted pT: " << pT << endl; 
            }
            cout << "Generated pT: " << part->GetPt() << endl;*/
            //DELETE ABOVE///////////////////////
            if (part->GetPDG() == 3122) {
                if (part->GetPt() > 0.5 && part->GetPt() < 0.6) {
                    pTcheck++;
                }
                hgenlambdaregpT->Fill(part->GetPt());
                hgenlambdaregEta->Fill(part->GetEta());
                hEtaTest->Fill(atanh((part->GetPz())/(part->GetMomentum().Mag())));
                //cout << gtrdata[part->GetMother1()]->GetPDG() << endl;
                //cout << part->GetMother2() << endl;
                hgenlambdaregq2->Fill(*q2egen);
                hgenlambdaregy->Fill(*yegen);
                //hlambdaregcountphi->Fill(part->GetPhi());
                hlambdaregcountphi->Fill(part->GetFourVector().Vect().Phi());
                //hlambdaregcounttheta->Fill(part->GetTheta());
                hlambdaregcounttheta->Fill(part->GetFourVector().Vect().Theta());
                hgenlambdaregX->Fill((*q2egen) / ((*yegen) * 4 * 920 * 27.6));
                XYvtxdensityreg->Fill(svx->GetX(), svx->GetY());
                ZXvtxdensityreg->Fill(svx->GetZ(), svx->GetX());
                ZYvtxdensityreg->Fill(svx->GetZ(), svx->GetY());
                if (part->GetPt() > 0 && part->GetPt() < 4) {
                    rXYvtxdensityreg->Fill(svx->GetX(), svx->GetY());
                    rZXvtxdensityreg->Fill(svx->GetZ(), svx->GetX());
                    rZYvtxdensityreg->Fill(svx->GetZ(), svx->GetY());
                }
                if (svx->GetIvtype() == 1) { ivtype1++; }
                if (svx->GetIvtype() == 2) { ivtype2++; }
                if (svx->GetIvtype() == 3) { ivtype3++; }
                if (svx->GetIvtype() == 4) { ivtype4++; }
                if (svx->GetIvtype() == 5) { ivtype5++; }
                if (svx->GetIvtype() == 2) {
                    //cout << gtrdata[part->GetMother1()]->GetPDG() << endl;
                    //cout << part->GetMother1() << endl;
                    //cout << "Mother: " << gtrdata[part->GetMother1()]->GetStrIdx() << endl;
                    if (gtrdata[part->GetMother1()]->GetStrIdx() == -1) {
                        dXYvtxdensityreg->Fill(svx->GetX(), svx->GetY());
                        dZXvtxdensityreg->Fill(svx->GetZ(), svx->GetX());
                        dZYvtxdensityreg->Fill(svx->GetZ(), svx->GetY());
                    }
                }
            }
            if (part->GetPDG() == -3122) {
                if (part->GetPt() > 0.5 && part->GetPt() < 0.6) {
                    pTcheck++;
                }
                hgenlambdabarpT->Fill(part->GetPt());
                hgenlambdabarEta->Fill(part->GetEta());
                hgenlambdabarq2->Fill(*q2egen);
                hgenlambdabary->Fill(*yegen);
                //hlambdabarcountphi->Fill(part->GetPhi());
                hlambdabarcountphi->Fill(part->GetFourVector().Vect().Phi());
                //hlambdabarcounttheta->Fill(part->GetTheta());
                hlambdabarcounttheta->Fill(part->GetFourVector().Vect().Theta());
                hgenlambdabarX->Fill((*q2egen) / ((*yegen) * 4 * 920 * 27.6));
                XYvtxdensitybar->Fill(svx->GetX(), svx->GetY());
                ZXvtxdensitybar->Fill(svx->GetZ(), svx->GetX());
                ZYvtxdensitybar->Fill(svx->GetZ(), svx->GetY());
                if (part->GetPt() > 0 && part->GetPt() < 4) {
                    rXYvtxdensitybar->Fill(svx->GetX(), svx->GetY());
                    rZXvtxdensitybar->Fill(svx->GetZ(), svx->GetX());
                    rZYvtxdensitybar->Fill(svx->GetZ(), svx->GetY());
                }
                if (svx->GetIvtype() == 1) { ivtype1++; }
                if (svx->GetIvtype() == 2) { ivtype2++; }
                if (svx->GetIvtype() == 3) { ivtype3++; }
                if (svx->GetIvtype() == 4) { ivtype4++; }
                if (svx->GetIvtype() == 5) { ivtype5++; }
                if (svx->GetIvtype() == 2) {
                    //cout << gtrdata[part->GetMother1()]->GetPDG() << endl;
                    //cout << part->GetMother1() << endl;
                    //cout << "Mother: " << gtrdata[part->GetMother1()]->GetStrIdx() << endl;
                    if (gtrdata[part->GetMother1()]->GetStrIdx() == -1) {
                        dXYvtxdensitybar->Fill(svx->GetX(), svx->GetY());
                        dZXvtxdensitybar->Fill(svx->GetZ(), svx->GetX());
                        dZYvtxdensitybar->Fill(svx->GetZ(), svx->GetY());
                    }
                }
                
            }
            //Now getting dists for Pi+/-,Proton/antiproton, and K+/K- to help understand Lambda Distributions
            
            if (part->GetPDG() == 211) {
                hgenpipluspT->Fill(part->GetPt());
                hgenpiplusEta->Fill(part->GetEta());
                hgenpiplusq2->Fill(*q2egen);
                hgenpiplusy->Fill(*yegen);
                hgenpiplusX->Fill((*q2egen) / ((*yegen) * 4 * 920 * 27.6));
            }
            if (part->GetPDG() == -211) {
                hgenpiminuspT->Fill(part->GetPt());
                hgenpiminusEta->Fill(part->GetEta());
                hgenpiminusq2->Fill(*q2egen);
                hgenpiminusy->Fill(*yegen);
                hgenpiminusX->Fill((*q2egen) / ((*yegen) * 4 * 920 * 27.6));
            }
            if (part->GetPDG() == 321) {
                hgenKpluspT->Fill(part->GetPt());
                hgenKplusEta->Fill(part->GetEta());
                hgenKplusq2->Fill(*q2egen);
                hgenKplusy->Fill(*yegen);
                hgenKplusX->Fill((*q2egen) / ((*yegen) * 4 * 920 * 27.6));
            }
            if (part->GetPDG() == -321) {
                hgenKminuspT->Fill(part->GetPt());
                hgenKminusEta->Fill(part->GetEta());
                hgenKminusq2->Fill(*q2egen);
                hgenKminusy->Fill(*yegen);
                hgenKminusX->Fill((*q2egen) / ((*yegen) * 4 * 920 * 27.6));
            }
            if (part->GetPDG() == 2212) {
                hgenprotonregpT->Fill(part->GetPt());
                hgenprotonregEta->Fill(part->GetEta());
                hgenprotonregq2->Fill(*q2egen);
                hgenprotonregy->Fill(*yegen);
                hgenprotonregX->Fill((*q2egen) / ((*yegen) * 4 * 920 * 27.6));
            }
            if (part->GetPDG() == -2212) {
                hgenprotonbarpT->Fill(part->GetPt());
                hgenprotonbarEta->Fill(part->GetEta());
                hgenprotonbarq2->Fill(*q2egen);
                hgenprotonbary->Fill(*yegen);
                hgenprotonbarX->Fill((*q2egen) / ((*yegen) * 4 * 920 * 27.6));

            }
            

            /////////////////////////////////////////////////////////////////////////////////////////////////
            //Now enforcing generator level pT and Eta ranges then counting particles that are produced in this range


            if (part->GetPt() > 0.5 && part->GetPt() < 3.5 && abs(part->GetEta()) < 1.3) {
                //cout << part->GetPDG() << endl;
                if (part->GetPDG() == 3122) { 
                    LambdaCount++;
                    hlambdaregcountpT->Fill(part->GetPt());
                    hlambdaregcountEta->Fill(part->GetEta());
                    hlambdaregcountq2->Fill(*q2egen);
                    hlambdaregcounty->Fill(*yegen);
                    hlambdaregcountX->Fill((*q2egen)/((*yegen)*4*920*27.6));
                    //pT
                    if (part->GetPt() > 0.5 && part->GetPt() < 0.6) { ptbin1reg++; }
                    if (part->GetPt() >= 0.6 && part->GetPt() < 0.7) { ptbin2reg++; }
                    if (part->GetPt() >= 0.7 && part->GetPt() < 0.8) { ptbin3reg++; }
                    if (part->GetPt() >= 0.8 && part->GetPt() < 0.9) { ptbin4reg++; }
                    if (part->GetPt() >= 0.9 && part->GetPt() < 1.1) { ptbin5reg++; }
                    if (part->GetPt() >= 1.1 && part->GetPt() < 1.3) { ptbin6reg++; }
                    if (part->GetPt() >= 1.3 && part->GetPt() < 1.6) { ptbin7reg++; }
                    if (part->GetPt() >= 1.6 && part->GetPt() < 2.2) { ptbin8reg++; }
                    if (part->GetPt() >= 2.2 && part->GetPt() < 3.5) { ptbin9reg++; }
                    //Eta
                    if (part->GetEta() > -1.3 && part->GetEta() < -1) { Etabin1reg = Etabin1reg + 1; }
                    if (part->GetEta() >= -1 && part->GetEta() < -0.75) { Etabin2reg = Etabin2reg + 1; }
                    if (part->GetEta() >= -0.75 && part->GetEta() < -0.5) { Etabin3reg = Etabin3reg + 1; }
                    if (part->GetEta() >= -0.5 && part->GetEta() < -0.25) { Etabin4reg = Etabin4reg + 1; }
                    if (part->GetEta() >= -0.25 && part->GetEta() < 0) { Etabin5reg = Etabin5reg + 1; }
                    if (part->GetEta() >= 0 && part->GetEta() < 0.25) { Etabin6reg = Etabin6reg + 1; }
                    if (part->GetEta() >= 0.25 && part->GetEta() < 0.5) { Etabin7reg = Etabin7reg + 1; }
                    if (part->GetEta() >= 0.5 && part->GetEta() < 0.75) { Etabin8reg = Etabin8reg + 1; }
                    if (part->GetEta() >= 0.75 && part->GetEta() < 1) { Etabin9reg = Etabin9reg + 1; }
                    if (part->GetEta() >= 1 && part->GetEta() < 1.3) { Etabin10reg = Etabin10reg + 1; }
                    //X
                    if ((*q2egen) / ((*yegen) * 4 * 920 * 27.6) > 0.00004 && (*q2egen) / ((*yegen) * 4 * 920 * 27.6) < 0.0001) { Xbin1reg = Xbin1reg + 1; }
                    if ((*q2egen) / ((*yegen) * 4 * 920 * 27.6) >= 0.0001 && (*q2egen) / ((*yegen) * 4 * 920 * 27.6) < 0.0002) { Xbin2reg = Xbin2reg + 1; }
                    if ((*q2egen) / ((*yegen) * 4 * 920 * 27.6) >= 0.0002 && (*q2egen) / ((*yegen) * 4 * 920 * 27.6) < 0.0004) { Xbin3reg = Xbin3reg + 1; }
                    if ((*q2egen) / ((*yegen) * 4 * 920 * 27.6) >= 0.0004 && (*q2egen) / ((*yegen) * 4 * 920 * 27.6) < 0.001) { Xbin4reg = Xbin4reg + 1; }
                    if ((*q2egen) / ((*yegen) * 4 * 920 * 27.6) >= 0.001 && (*q2egen) / ((*yegen) * 4 * 920 * 27.6) < 0.01) { Xbin5reg = Xbin5reg + 1; }
                    //Q2
                    if (*q2egen > 2 && *q2egen < 2.5) { q2bin1reg = q2bin1reg + 1; }
                    if (*q2egen >= 2.5 && *q2egen < 3) { q2bin2reg = q2bin2reg + 1; }
                    if (*q2egen >= 3 && *q2egen < 4) { q2bin3reg = q2bin3reg + 1; }
                    if (*q2egen >= 4 && *q2egen < 5) { q2bin4reg = q2bin4reg + 1; }
                    if (*q2egen >= 5 && *q2egen < 7) { q2bin5reg = q2bin5reg + 1; }
                    if (*q2egen >= 7 && *q2egen < 10) { q2bin6reg = q2bin6reg + 1; }
                    if (*q2egen >= 10 && *q2egen < 15) { q2bin7reg = q2bin7reg + 1; }
                    if (*q2egen >= 15 && *q2egen < 25) { q2bin8reg = q2bin8reg + 1; }
                    if (*q2egen >= 25 && *q2egen < 100) { q2bin9reg = q2bin9reg + 1; }
                    //Phi
                    if (part->GetFourVector().Vect().Phi() > -M_PI && part->GetFourVector().Vect().Phi() < -(5 * M_PI) / (6)) { phibin1reg++; }
                    if (part->GetFourVector().Vect().Phi() >= -(5 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < -(4 * M_PI) / (6)) { phibin2reg++; }
                    if (part->GetFourVector().Vect().Phi() >= -(4 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < -(3 * M_PI) / (6)) { phibin3reg++; }
                    if (part->GetFourVector().Vect().Phi() >= -(3 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < -(2 * M_PI) / (6)) { phibin4reg++; }
                    if (part->GetFourVector().Vect().Phi() >= -(2 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < -(1 * M_PI) / (6)) { phibin5reg++; }
                    if (part->GetFourVector().Vect().Phi() >= -(1 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < 0) { phibin6reg++; }
                    if (part->GetFourVector().Vect().Phi() >= 0 && part->GetFourVector().Vect().Phi() < (1 * M_PI) / (6)) { phibin7reg++; }
                    if (part->GetFourVector().Vect().Phi() >= (1 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < (2 * M_PI) / (6)) { phibin8reg++; }
                    if (part->GetFourVector().Vect().Phi() >= (2 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < (3 * M_PI) / (6)) { phibin9reg++; }
                    if (part->GetFourVector().Vect().Phi() >= (3 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < (4 * M_PI) / (6)) { phibin10reg++; }
                    if (part->GetFourVector().Vect().Phi() >= (4 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < (5 * M_PI) / (6)) { phibin11reg++; }
                    if (part->GetFourVector().Vect().Phi() >= (5 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < M_PI) { phibin12reg++; }
                    //Filling Doubly differential count plot
                    for (int a = 0; a < 9; a++) {
                        for (int b = 0; b < 10; b++) {
                            if (part->GetPt() > pTbins[a][0] && part->GetPt() < pTbins[a][1]  && part->GetEta() > Etabins[b][0] && part->GetEta() < Etabins[b][1]) {
                                HistBinsReg[a][b] = HistBinsReg[a][b] + 1;
                            }
                        }
                    }
                }
                if (part->GetPDG() == -3122) { 
                    LambdaBarCount++;
                    hlambdabarcountpT->Fill(part->GetPt());
                    hlambdabarcountEta->Fill(part->GetEta());
                    hlambdabarcountq2->Fill(*q2egen);
                    hlambdabarcounty->Fill(*yegen);
                    hlambdabarcountX->Fill((*q2egen) / ((*yegen) * 4 * 920 * 27.6));
                    //pT
                    if (part->GetPt() > 0.5 && part->GetPt() < 0.6) { ptbin1bar = ptbin1bar + 1; }
                    if (part->GetPt() >= 0.6 && part->GetPt() < 0.7) { ptbin2bar = ptbin2bar + 1; }
                    if (part->GetPt() >= 0.7 && part->GetPt() < 0.8) { ptbin3bar = ptbin3bar + 1; }
                    if (part->GetPt() >= 0.8 && part->GetPt() < 0.9) { ptbin4bar = ptbin4bar + 1; }
                    if (part->GetPt() >= 0.9 && part->GetPt() < 1.1) { ptbin5bar = ptbin5bar + 1; }
                    if (part->GetPt() >= 1.1 && part->GetPt() < 1.3) { ptbin6bar = ptbin6bar + 1; }
                    if (part->GetPt() >= 1.3 && part->GetPt() < 1.6) { ptbin7bar = ptbin7bar + 1; }
                    if (part->GetPt() >= 1.6 && part->GetPt() < 2.2) { ptbin8bar = ptbin8bar + 1; }
                    if (part->GetPt() >= 2.2 && part->GetPt() < 3.5) { ptbin9bar = ptbin9bar + 1; }
                    //Eta
                    if (part->GetEta() > -1.3 && part->GetEta() < -1) { Etabin1bar = Etabin1bar + 1; }
                    if (part->GetEta() >= -1 && part->GetEta() < -0.75) { Etabin2bar = Etabin2bar + 1; }
                    if (part->GetEta() >= -0.75 && part->GetEta() < -0.5) { Etabin3bar = Etabin3bar + 1; }
                    if (part->GetEta() >= -0.5 && part->GetEta() < -0.25) { Etabin4bar = Etabin4bar + 1; }
                    if (part->GetEta() >= -0.25 && part->GetEta() < 0) { Etabin5bar = Etabin5bar + 1; }
                    if (part->GetEta() >= 0 && part->GetEta() < 0.25) { Etabin6bar = Etabin6bar + 1; }
                    if (part->GetEta() >= 0.25 && part->GetEta() < 0.5) { Etabin7bar = Etabin7bar + 1; }
                    if (part->GetEta() >= 0.5 && part->GetEta() < 0.75) { Etabin8bar = Etabin8bar + 1; }
                    if (part->GetEta() >= 0.75 && part->GetEta() < 1) { Etabin9bar = Etabin9bar + 1; }
                    if (part->GetEta() >= 1 && part->GetEta() < 1.3) { Etabin10bar = Etabin10bar + 1; }
                    //X
                    if ((*q2egen) / ((*yegen) * 4 * 920 * 27.6) > 0.00004 && (*q2egen) / ((*yegen) * 4 * 920 * 27.6) < 0.0001) { Xbin1bar = Xbin1bar + 1; }
                    if ((*q2egen) / ((*yegen) * 4 * 920 * 27.6) >= 0.0001 && (*q2egen) / ((*yegen) * 4 * 920 * 27.6) < 0.0002) { Xbin2bar = Xbin2bar + 1; }
                    if ((*q2egen) / ((*yegen) * 4 * 920 * 27.6) >= 0.0002 && (*q2egen) / ((*yegen) * 4 * 920 * 27.6) < 0.0004) { Xbin3bar = Xbin3bar + 1; }
                    if ((*q2egen) / ((*yegen) * 4 * 920 * 27.6) >= 0.0004 && (*q2egen) / ((*yegen) * 4 * 920 * 27.6) < 0.001) { Xbin4bar = Xbin4bar + 1; }
                    if ((*q2egen) / ((*yegen) * 4 * 920 * 27.6) >= 0.001 && (*q2egen) / ((*yegen) * 4 * 920 * 27.6) < 0.01) { Xbin5bar = Xbin5bar + 1; }
                    //Q2
                    if (*q2egen > 2 && *q2egen < 2.5) { q2bin1bar = q2bin1bar + 1; }
                    if (*q2egen >= 2.5 && *q2egen < 3) { q2bin2bar = q2bin2bar + 1; }
                    if (*q2egen >= 3 && *q2egen < 4) { q2bin3bar = q2bin3bar + 1; }
                    if (*q2egen >= 4 && *q2egen < 5) { q2bin4bar = q2bin4bar + 1; }
                    if (*q2egen >= 5 && *q2egen < 7) { q2bin5bar = q2bin5bar + 1; }
                    if (*q2egen >= 7 && *q2egen < 10) { q2bin6bar = q2bin6bar + 1; }
                    if (*q2egen >= 10 && *q2egen < 15) { q2bin7bar = q2bin7bar + 1; }
                    if (*q2egen >= 15 && *q2egen < 25) { q2bin8bar = q2bin8bar + 1; }
                    if (*q2egen >= 25 && *q2egen < 100) { q2bin9bar = q2bin9bar + 1; }
                    //Phi
                    if (part->GetFourVector().Vect().Phi() > -M_PI && part->GetFourVector().Vect().Phi() < -(5 * M_PI) / (6)) { phibin1bar++; }
                    if (part->GetFourVector().Vect().Phi() >= -(5 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < -(4 * M_PI) / (6)) { phibin2bar++; }
                    if (part->GetFourVector().Vect().Phi() >= -(4 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < -(3 * M_PI) / (6)) { phibin3bar++; }
                    if (part->GetFourVector().Vect().Phi() >= -(3 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < -(2 * M_PI) / (6)) { phibin4bar++; }
                    if (part->GetFourVector().Vect().Phi() >= -(2 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < -(1 * M_PI) / (6)) { phibin5bar++; }
                    if (part->GetFourVector().Vect().Phi() >= -(1 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < 0) { phibin6bar++; }
                    if (part->GetFourVector().Vect().Phi() >= 0 && part->GetFourVector().Vect().Phi() < (1 * M_PI) / (6)) { phibin7bar++; }
                    if (part->GetFourVector().Vect().Phi() >= (1 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < (2 * M_PI) / (6)) { phibin8bar++; }
                    if (part->GetFourVector().Vect().Phi() >= (2 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < (3 * M_PI) / (6)) { phibin9bar++; }
                    if (part->GetFourVector().Vect().Phi() >= (3 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < (4 * M_PI) / (6)) { phibin10bar++; }
                    if (part->GetFourVector().Vect().Phi() >= (4 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < (5 * M_PI) / (6)) { phibin11bar++; }
                    if (part->GetFourVector().Vect().Phi() >= (5 * M_PI) / (6) && part->GetFourVector().Vect().Phi() < M_PI) { phibin12bar++; }
                    //Filling Doubly differential count plot
                    for (int a = 0; a < 9; a++) {
                        for (int b = 0; b < 10; b++) {
                            if (part->GetPt() > pTbins[a][0] && part->GetPt() < pTbins[a][1] && part->GetEta() > Etabins[b][0] && part->GetEta() < Etabins[b][1]) {
                                HistBinsBar[a][b] = HistBinsBar[a][b] + 1;
                            }
                        }
                    }
                }
                if (part->GetPDG() == 310) { K0Count++; }
                if (part->GetPDG() == 4122) { LambdaCCount++; }
                if (part->GetPDG() == -4122) { LambdaCBarCount++; }
                if (part->GetPDG() == 421) { D0Count++; }
                if (part->GetPDG() == -421) { D0BarCount++; }
            }
        }
        sel++;
    }


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     Done Counting, now filling plots
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (int a = 0; a < 9; a++) {
        for (int b = 0; b < 10; b++) {


            //Now setting 2D pT vs Eta anti lambda hist bin content for (a,b)=(pT,Eta)
            for (int l = 1; l < LambdaBar2D->GetNbinsX() + 1; l++) {
                for (int k = 1; k < LambdaBar2D->GetNbinsY() + 1; k++) {
                    if (LambdaBar2D->GetXaxis()->GetBinCenter(l) > pTbins[a][0] && LambdaBar2D->GetXaxis()->GetBinCenter(l) < pTbins[a][1] && LambdaBar2D->GetYaxis()->GetBinCenter(k) > Etabins[b][0] && LambdaBar2D->GetYaxis()->GetBinCenter(k) < Etabins[b][1]) {
                        LambdaBar2D->SetBinContent(l, k, HistBinsBar[a][b]);
                        LambdaReg2D->SetBinContent(l, k, HistBinsReg[a][b]);
                        Asymm2D->SetBinContent(l, k, (HistBinsReg[a][b] - HistBinsBar[a][b]) / (HistBinsReg[a][b] + HistBinsBar[a][b]));

                    }
                }
            }

        }
    }
    

  //////////////////////////Getting Difference Plots
    for (int i = 1; i < (int)hgenKpluspT->GetNbinsX() + 1; i++) {
        hgenKdiffpT->SetBinContent(i,(hgenKpluspT->GetBinContent(i))-(hgenKminuspT->GetBinContent(i)));
    }
    for (int i = 1; i < (int)hgenlambdabarpT->GetNbinsX() + 1; i++) {
        hgenLdiffpT->SetBinContent(i,(hgenlambdaregpT->GetBinContent(i)) - (hgenlambdabarpT->GetBinContent(i)));
    }

    for (int i = 1; i < (int)hgenKplusEta->GetNbinsX() + 1; i++) {
        hgenKdiffEta->SetBinContent(i, (hgenKplusEta->GetBinContent(i)) - (hgenKminusEta->GetBinContent(i)));
    }
    for (int i = 1; i < (int)hgenlambdabarEta->GetNbinsX() + 1; i++) {
        hgenLdiffEta->SetBinContent(i, (hgenlambdaregEta->GetBinContent(i)) - (hgenlambdabarEta->GetBinContent(i)));
    }

    for (int i = 1; i < (int)hgenKplusX->GetNbinsX() + 1; i++) {
        hgenKdiffX->SetBinContent(i, (hgenKplusX->GetBinContent(i)) - (hgenKminusX->GetBinContent(i)));
    }
    for (int i = 1; i < (int)hgenlambdabarX->GetNbinsX() + 1; i++) {
        hgenLdiffX->SetBinContent(i, (hgenlambdaregX->GetBinContent(i)) - (hgenlambdabarX->GetBinContent(i)));
    }

    for (int i = 1; i < (int)hgenKplusy->GetNbinsX() + 1; i++) {
        hgenKdiffy->SetBinContent(i, (hgenKplusy->GetBinContent(i)) - (hgenKminusy->GetBinContent(i)));
    }
    for (int i = 1; i < (int)hgenlambdabary->GetNbinsX() + 1; i++) {
        hgenLdiffy->SetBinContent(i, (hgenlambdaregy->GetBinContent(i)) - (hgenlambdabary->GetBinContent(i)));
    }

    for (int i = 1; i < (int)hgenKplusq2->GetNbinsX() + 1; i++) {
        hgenKdiffq2->SetBinContent(i, (hgenKplusq2->GetBinContent(i)) - (hgenKminusq2->GetBinContent(i)));
    }
    for (int i = 1; i < (int)hgenlambdabarq2->GetNbinsX() + 1; i++) {
        hgenLdiffq2->SetBinContent(i, (hgenlambdaregq2->GetBinContent(i)) - (hgenlambdabarq2->GetBinContent(i)));
    }



/////////////////////////////


    cout << "Lambda Count: " << LambdaCount << endl;
    cout << "Lambda Bar Count: " << LambdaBarCount << endl;
    cout << "K0 Count: " << K0Count << endl;
    cout << "LambdaC Count: " << LambdaCCount << endl;
    cout << "LambdaC Bar Count: " << LambdaCBarCount << endl;
    cout << "D0 Count: " << D0Count << endl;
    cout << "D0 Bar Count: " << D0BarCount << endl;
    cout << "none one weight count " << nononeweightcount << endl;
    cout << "Total Gen particles inv mass: " << totgen4vec.M() << endl;
    cout << "Total pX: " << px << endl;
    cout << "Total Lambda in pT Bin 1: " << pTcheck << endl;
    cout << particles << endl;

    cout << "primary: " << ivtype1 << endl;
    cout << "decay: " << ivtype2 << endl;
    cout << "secondary: " << ivtype3 << endl;
    cout << "pair prod: " << ivtype4 << endl;
    cout << "else: " << ivtype5 << endl;

    TFile f("/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/genlevelcount.root", "RECREATE");
    //just using this as ref to see hist names
    /*TH1F* hlambdaregcountpT = new TH1F("Lambda Reg Count vs pT", "Lambda Reg Count vs pT", 1000, 0, 10);
    TH1F* hlambdaregcountEta = new TH1F("Lambda Reg Count vs Eta", "Lambda Reg Count vs Eta", 800, -4, 4);
    TH1F* hlambdabarcountpT = new TH1F("Lambda Bar Count vs pT", "Lambda Bar Count vs pT", 1000, 0, 10);
    TH1F* hlambdabarcountEta = new TH1F("Lambda Bar Count vs Eta", "Lambda Bar Count vs Eta", 800, -4, 4);

    TH1F* hlambdaregcountq2 = new TH1F("Lambda Reg Count vs q2", "Lambda Reg Count vs q2", 15000, 0, 150);
    TH1F* hlambdaregcounty = new TH1F("Lambda Reg Count vs q2", "Lambda Reg Count vs q2", 100, 0, 1);
    TH1F* hlambdabarcountq2 = new TH1F("Lambda Bar Count vs q2", "Lambda Bar Count vs q2", 15000, 0, 150);
    TH1F* hlambdabarcounty = new TH1F("Lambda Bar Count vs q2", "Lambda Bar Count vs q2", 100, 0, 1);

    TH1F* hlambdaregcountX = new TH1F("Lambda Reg Count vs Bjorken X", "Lambda Reg Count vs Bjorken X", 5000, 0, 0.05);
    TH1F* hlambdabarcountX = new TH1F("Lambda Bar Count vs Bjorken X", "Lambda Bar Count vs Bjorken X", 5000, 0, 0.05);*/
    //

    //Now calculate the generator level assymtry with very fine binning, and with the binning needed for 09 paper comparison
    
    //pt
    for (int i = 1; i < (int)hlambdaregcountpT->GetNbinsX() + 1; i++) {
        
        double tempx = hlambdaregcountpT->GetBinCenter(i);
        
        if (tempx > 0.5 && tempx < 0.6) {
            double tempy = (ptbin1reg - ptbin1bar)/(ptbin1reg + ptbin1bar);
            hlambdaAsymmpT->SetBinContent(i, tempy);
            hlambdaregsigmapT->SetBinContent(i, ptbin1reg);
            hlambdabarsigmapT->SetBinContent(i, ptbin1bar);
            hlambdatotsigmapT->SetBinContent(i, (ptbin1reg + ptbin1bar) );
        }
        if (tempx > 0.6 && tempx < 0.7) {
            double tempy = (ptbin2reg - ptbin2bar) / (ptbin2reg + ptbin2bar);
            hlambdaAsymmpT->SetBinContent(i, tempy);
            hlambdaregsigmapT->SetBinContent(i, ptbin2reg);
            hlambdabarsigmapT->SetBinContent(i, ptbin2bar);
            hlambdatotsigmapT->SetBinContent(i, (ptbin2reg+ptbin2bar));
        }
        if (tempx > 0.7 && tempx < 0.8) {
            double tempy = (ptbin3reg - ptbin3bar) / (ptbin3reg + ptbin3bar);
            hlambdaAsymmpT->SetBinContent(i, tempy);
            hlambdaregsigmapT->SetBinContent(i, ptbin3reg);
            hlambdabarsigmapT->SetBinContent(i, ptbin3bar);
            hlambdatotsigmapT->SetBinContent(i, (ptbin3reg + ptbin3bar));
        }
        if (tempx > 0.8 && tempx < 0.9) {
            double tempy = (ptbin4reg - ptbin4bar) / (ptbin4reg + ptbin4bar);
            hlambdaAsymmpT->SetBinContent(i, tempy);
            hlambdaregsigmapT->SetBinContent(i, ptbin4reg);
            hlambdabarsigmapT->SetBinContent(i, ptbin4bar );
            hlambdatotsigmapT->SetBinContent(i, (ptbin4reg + ptbin4bar));
        }
        if (tempx > 0.9 && tempx < 1.1) {
            double tempy = (ptbin5reg - ptbin5bar) / (ptbin5reg + ptbin5bar);
            hlambdaAsymmpT->SetBinContent(i, tempy);
            hlambdaregsigmapT->SetBinContent(i, ptbin5reg);
            hlambdabarsigmapT->SetBinContent(i, ptbin5bar);
            hlambdatotsigmapT->SetBinContent(i, (ptbin5reg + ptbin5bar));
        }
        if (tempx > 1.1 && tempx < 1.3) {
            double tempy = (ptbin6reg - ptbin6bar) / (ptbin6reg + ptbin6bar);
            hlambdaAsymmpT->SetBinContent(i, tempy);
            hlambdaregsigmapT->SetBinContent(i, ptbin6reg);
            hlambdabarsigmapT->SetBinContent(i, ptbin6bar);
            hlambdatotsigmapT->SetBinContent(i, (ptbin6reg + ptbin6bar));
        }
        if (tempx > 1.3 && tempx < 1.6) {
            double tempy = (ptbin7reg - ptbin7bar) / (ptbin7reg + ptbin7bar);
            hlambdaAsymmpT->SetBinContent(i, tempy);
            hlambdaregsigmapT->SetBinContent(i, ptbin7reg);
            hlambdabarsigmapT->SetBinContent(i, ptbin7bar);
            hlambdatotsigmapT->SetBinContent(i, (ptbin7reg + ptbin7bar));
        }
        if (tempx > 1.6 && tempx < 2.2) {
            double tempy = (ptbin8reg - ptbin8bar) / (ptbin8reg + ptbin8bar);
            hlambdaAsymmpT->SetBinContent(i, tempy);
            hlambdaregsigmapT->SetBinContent(i, ptbin8reg);
            hlambdabarsigmapT->SetBinContent(i, ptbin8bar);
            hlambdatotsigmapT->SetBinContent(i, (ptbin8reg + ptbin8bar));
        }
        if (tempx > 2.2 && tempx < 3.5) {
            double tempy = (ptbin9reg - ptbin9bar) / (ptbin9reg + ptbin9bar);
            hlambdaAsymmpT->SetBinContent(i, tempy);
            hlambdaregsigmapT->SetBinContent(i, ptbin9reg);
            hlambdabarsigmapT->SetBinContent(i, ptbin9bar);
            hlambdatotsigmapT->SetBinContent(i, (ptbin9reg + ptbin9bar));
        }

    }
    //Eta
    for (int i = 1; i < (int)hlambdaregcountEta->GetNbinsX() + 1; i++) {

        double tempx = hlambdaregcountEta->GetBinCenter(i);

        if (tempx > -1.3 && tempx < -1) {
            double tempy = (Etabin1reg - Etabin1bar) / (Etabin1reg + Etabin1bar);
            hlambdaAsymmEta->SetBinContent(i, tempy);
            hlambdaregsigmaEta->SetBinContent(i, Etabin1reg);
            hlambdabarsigmaEta->SetBinContent(i, Etabin1bar);
            hlambdatotsigmaEta->SetBinContent(i, (Etabin1reg + Etabin1bar));
        }
        if (tempx > -1 && tempx < -0.75) {
            double tempy = (Etabin2reg - Etabin2bar) / (Etabin2reg + Etabin2bar);
            hlambdaAsymmEta->SetBinContent(i, tempy);
            hlambdaregsigmaEta->SetBinContent(i, Etabin2reg);
            hlambdabarsigmaEta->SetBinContent(i, Etabin2bar);
            hlambdatotsigmaEta->SetBinContent(i, (Etabin2reg + Etabin2bar));
        }
        if (tempx > -0.75 && tempx < -0.5) {
            double tempy = (Etabin3reg - Etabin3bar) / (Etabin3reg + Etabin3bar);
            hlambdaAsymmEta->SetBinContent(i, tempy);
            hlambdaregsigmaEta->SetBinContent(i, Etabin3reg);
            hlambdabarsigmaEta->SetBinContent(i, Etabin3bar);
            hlambdatotsigmaEta->SetBinContent(i, (Etabin3reg + Etabin3bar));
        }
        if (tempx > -0.5 && tempx < -0.25) {
            double tempy = (Etabin4reg - Etabin4bar) / (Etabin4reg + Etabin4bar);
            hlambdaAsymmEta->SetBinContent(i, tempy);
            hlambdaregsigmaEta->SetBinContent(i, Etabin4reg);
            hlambdabarsigmaEta->SetBinContent(i, Etabin4bar);
            hlambdatotsigmaEta->SetBinContent(i, (Etabin4reg + Etabin4bar));
        }
        if (tempx > -0.25 && tempx < 0) {
            double tempy = (Etabin5reg - Etabin5bar) / (Etabin5reg + Etabin5bar);
            hlambdaAsymmEta->SetBinContent(i, tempy);
            hlambdaregsigmaEta->SetBinContent(i, Etabin5reg);
            hlambdabarsigmaEta->SetBinContent(i, Etabin5bar);
            hlambdatotsigmaEta->SetBinContent(i, (Etabin5reg + Etabin5bar));
        }
        if (tempx > 0 && tempx < 0.25) {
            double tempy = (Etabin6reg - Etabin6bar) / (Etabin6reg + Etabin6bar);
            hlambdaAsymmEta->SetBinContent(i, tempy);
            hlambdaregsigmaEta->SetBinContent(i, Etabin6reg);
            hlambdabarsigmaEta->SetBinContent(i, Etabin6bar);
            hlambdatotsigmaEta->SetBinContent(i, (Etabin6reg + Etabin6bar));
        }
        if (tempx > 0.25 && tempx < 0.5) {
            double tempy = (Etabin7reg - Etabin7bar) / (Etabin7reg + Etabin7bar);
            hlambdaAsymmEta->SetBinContent(i, tempy);
            hlambdaregsigmaEta->SetBinContent(i, Etabin7reg);
            hlambdabarsigmaEta->SetBinContent(i, Etabin7bar);
            hlambdatotsigmaEta->SetBinContent(i, (Etabin7reg + Etabin7bar));
        }
        if (tempx > 0.5 && tempx < 0.75) {
            double tempy = (Etabin8reg - Etabin8bar) / (Etabin8reg + Etabin8bar);
            hlambdaAsymmEta->SetBinContent(i, tempy);
            hlambdaregsigmaEta->SetBinContent(i, Etabin8reg);
            hlambdabarsigmaEta->SetBinContent(i, Etabin8bar);
            hlambdatotsigmaEta->SetBinContent(i, (Etabin8reg + Etabin8bar));
        }
        if (tempx > 0.75 && tempx < 1) {
            double tempy = (Etabin9reg - Etabin9bar) / (Etabin9reg + Etabin9bar);
            hlambdaAsymmEta->SetBinContent(i, tempy);
            hlambdaregsigmaEta->SetBinContent(i, Etabin9reg);
            hlambdabarsigmaEta->SetBinContent(i, Etabin9bar);
            hlambdatotsigmaEta->SetBinContent(i, (Etabin9reg + Etabin9bar));
        }
        if (tempx > 1 && tempx < 1.3) {
            double tempy = (Etabin10reg - Etabin10bar) / (Etabin10reg + Etabin10bar);
            hlambdaAsymmEta->SetBinContent(i, tempy);
            hlambdaregsigmaEta->SetBinContent(i, Etabin10reg);
            hlambdabarsigmaEta->SetBinContent(i, Etabin10bar);
            hlambdatotsigmaEta->SetBinContent(i, (Etabin10reg + Etabin10bar));
        }

    }
    //X
    for (int i = 1; i < (int)hlambdaregcountX->GetNbinsX() + 1; i++) {

        double tempx = hlambdaregcountX->GetBinCenter(i);

        if (tempx > 0.00004 && tempx < 0.0001) {
            double tempy = (Xbin1reg - Xbin1bar) / (Xbin1reg + Xbin1bar);
            hlambdaAsymmX->SetBinContent(i, tempy);
            hlambdaregsigmaX->SetBinContent(i, Xbin1reg);
            hlambdabarsigmaX->SetBinContent(i, Xbin1bar);
            hlambdatotsigmaX->SetBinContent(i, (Xbin1bar + Xbin1reg));
        }
        if (tempx > 0.0001 && tempx < 0.0002) {
            double tempy = (Xbin2reg - Xbin2bar) / (Xbin2reg + Xbin2bar);
            hlambdaAsymmX->SetBinContent(i, tempy);
            hlambdaregsigmaX->SetBinContent(i, Xbin2reg);
            hlambdabarsigmaX->SetBinContent(i, Xbin2bar);
            hlambdatotsigmaX->SetBinContent(i, (Xbin2bar + Xbin2reg));
        }
        if (tempx > 0.0002 && tempx < 0.0004) {
            double tempy = (Xbin3reg - Xbin3bar) / (Xbin3reg + Xbin3bar);
            hlambdaAsymmX->SetBinContent(i, tempy);
            hlambdaregsigmaX->SetBinContent(i, Xbin3reg);
            hlambdabarsigmaX->SetBinContent(i, Xbin3bar);
            hlambdatotsigmaX->SetBinContent(i, (Xbin3bar + Xbin3reg));
        }
        if (tempx > 0.0004 && tempx < 0.001) {
            double tempy = (Xbin4reg - Xbin4bar) / (Xbin4reg + Xbin4bar);
            hlambdaAsymmX->SetBinContent(i, tempy);
            hlambdaregsigmaX->SetBinContent(i, Xbin4reg);
            hlambdabarsigmaX->SetBinContent(i, Xbin4bar);
            hlambdatotsigmaX->SetBinContent(i, (Xbin4bar + Xbin4reg));
        }
        if (tempx > 0.001 && tempx < 0.01) {
            double tempy = (Xbin5reg - Xbin5bar) / (Xbin5reg + Xbin5bar);
            hlambdaAsymmX->SetBinContent(i, tempy);
            hlambdaregsigmaX->SetBinContent(i, Xbin5reg);
            hlambdabarsigmaX->SetBinContent(i, Xbin5bar);
            hlambdatotsigmaX->SetBinContent(i, (Xbin5bar + Xbin5reg));
        }
    }
    //Q2
    for (int i = 1; i < (int)hlambdaregcountq2->GetNbinsX() + 1; i++) {

        double tempx = hlambdaregcountq2->GetBinCenter(i);

        if (tempx > 2 && tempx < 2.5) {
            double tempy = (q2bin1reg - q2bin1bar) / (q2bin1reg + q2bin1bar);
            hlambdaAsymmq2->SetBinContent(i, tempy);
            hlambdaregsigmaq2->SetBinContent(i, q2bin1reg);
            hlambdabarsigmaq2->SetBinContent(i, q2bin1bar);
            hlambdatotsigmaq2->SetBinContent(i, (q2bin1bar + q2bin1reg));
        }
        if (tempx > 2.5 && tempx < 3) {
            double tempy = (q2bin2reg - q2bin2bar) / (q2bin2reg + q2bin2bar);
            hlambdaAsymmq2->SetBinContent(i, tempy);
            hlambdaregsigmaq2->SetBinContent(i, q2bin2reg);
            hlambdabarsigmaq2->SetBinContent(i, q2bin2bar);
            hlambdatotsigmaq2->SetBinContent(i, (q2bin2bar + q2bin2reg));
        }
        if (tempx > 3 && tempx < 4) {
            double tempy = (q2bin3reg - q2bin3bar) / (q2bin3reg + q2bin3bar);
            hlambdaAsymmq2->SetBinContent(i, tempy);
            hlambdaregsigmaq2->SetBinContent(i, q2bin3reg);
            hlambdabarsigmaq2->SetBinContent(i, q2bin3bar);
            hlambdatotsigmaq2->SetBinContent(i, (q2bin3bar + q2bin3reg));
        }
        if (tempx > 4 && tempx < 5) {
            double tempy = (q2bin4reg - q2bin4bar) / (q2bin4reg + q2bin4bar);
            hlambdaAsymmq2->SetBinContent(i, tempy);
            hlambdaregsigmaq2->SetBinContent(i, q2bin4reg);
            hlambdabarsigmaq2->SetBinContent(i, q2bin4bar);
            hlambdatotsigmaq2->SetBinContent(i, (q2bin4bar + q2bin4reg));
        }
        if (tempx > 5 && tempx < 7) {
            double tempy = (q2bin5reg - q2bin5bar) / (q2bin5reg + q2bin5bar);
            hlambdaAsymmq2->SetBinContent(i, tempy);
            hlambdaregsigmaq2->SetBinContent(i, q2bin5reg);
            hlambdabarsigmaq2->SetBinContent(i, q2bin5bar);
            hlambdatotsigmaq2->SetBinContent(i, (q2bin5bar + q2bin5reg));
        }
        if (tempx > 7 && tempx < 10) {
            double tempy = (q2bin6reg - q2bin6bar) / (q2bin6reg + q2bin6bar);
            hlambdaAsymmq2->SetBinContent(i, tempy);
            hlambdaregsigmaq2->SetBinContent(i, q2bin6reg);
            hlambdabarsigmaq2->SetBinContent(i, q2bin6bar);
            hlambdatotsigmaq2->SetBinContent(i, (q2bin6bar + q2bin6reg));
        }
        if (tempx > 10 && tempx < 15) {
            double tempy = (q2bin7reg - q2bin7bar) / (q2bin7reg + q2bin7bar);
            hlambdaAsymmq2->SetBinContent(i, tempy);
            hlambdaregsigmaq2->SetBinContent(i, q2bin7reg);
            hlambdabarsigmaq2->SetBinContent(i, q2bin7bar);
            hlambdatotsigmaq2->SetBinContent(i, (q2bin7bar + q2bin7reg));
        }
        if (tempx > 15 && tempx < 25) {
            double tempy = (q2bin8reg - q2bin8bar) / (q2bin8reg + q2bin8bar);
            hlambdaAsymmq2->SetBinContent(i, tempy);
            hlambdaregsigmaq2->SetBinContent(i, q2bin8reg);
            hlambdabarsigmaq2->SetBinContent(i, q2bin8bar);
            hlambdatotsigmaq2->SetBinContent(i, (q2bin8bar + q2bin8reg));
        }
        if (tempx > 25 && tempx < 100) {
            double tempy = (q2bin9reg - q2bin9bar) / (q2bin9reg + q2bin9bar);
            hlambdaAsymmq2->SetBinContent(i, tempy);
            hlambdaregsigmaq2->SetBinContent(i, q2bin9reg);
            hlambdabarsigmaq2->SetBinContent(i, q2bin9bar);
            hlambdatotsigmaq2->SetBinContent(i, (q2bin9bar + q2bin9reg));
        }
    }
    //Phi
    for (int i = 1; i < (int)hlambdaregcountphi->GetNbinsX() + 1; i++) {

        double tempx = hlambdaregcountphi->GetBinCenter(i);

        if (tempx > -M_PI && tempx < (-5 * M_PI) / (6)) {
            double tempy = (phibin1reg - phibin1bar) / (phibin1reg + phibin1bar);
            hlambdaAsymmphi->SetBinContent(i, tempy);
            hlambdaregsigmaphi->SetBinContent(i, phibin1reg);
            hlambdabarsigmaphi->SetBinContent(i, phibin1bar);
            hlambdatotsigmaphi->SetBinContent(i, (phibin1reg + phibin1bar));
        }
        if (tempx > (-5 * M_PI) / (6) && tempx < (-4 * M_PI) / (6)) {
            double tempy = (phibin2reg - phibin2bar) / (phibin2reg + phibin2bar);
            hlambdaAsymmphi->SetBinContent(i, tempy);
            hlambdaregsigmaphi->SetBinContent(i, phibin2reg);
            hlambdabarsigmaphi->SetBinContent(i, phibin2bar);
            hlambdatotsigmaphi->SetBinContent(i, (phibin2reg + phibin2bar));
        }
        if (tempx > (-4 * M_PI) / (6) && tempx < (-3 * M_PI) / (6)) {
            double tempy = (phibin3reg - phibin3bar) / (phibin3reg + phibin3bar);
            hlambdaAsymmphi->SetBinContent(i, tempy);
            hlambdaregsigmaphi->SetBinContent(i, phibin3reg);
            hlambdabarsigmaphi->SetBinContent(i, phibin3bar);
            hlambdatotsigmaphi->SetBinContent(i, (phibin3reg + phibin3bar));
        }
        if (tempx > (-3 * M_PI) / (6) && tempx < (-2 * M_PI) / (6)) {
            double tempy = (phibin4reg - phibin4bar) / (phibin4reg + phibin4bar);
            hlambdaAsymmphi->SetBinContent(i, tempy);
            hlambdaregsigmaphi->SetBinContent(i, phibin4reg);
            hlambdabarsigmaphi->SetBinContent(i, phibin4bar);
            hlambdatotsigmaphi->SetBinContent(i, (phibin4reg + phibin4bar));
        }
        if (tempx > (-2 * M_PI) / (6) && tempx < (-1 * M_PI) / (6)) {
            double tempy = (phibin5reg - phibin5bar) / (phibin5reg + phibin5bar);
            hlambdaAsymmphi->SetBinContent(i, tempy);
            hlambdaregsigmaphi->SetBinContent(i, phibin5reg);
            hlambdabarsigmaphi->SetBinContent(i, phibin5bar);
            hlambdatotsigmaphi->SetBinContent(i, (phibin5reg + phibin5bar));
        }
        if (tempx > (-1 * M_PI) / (6) && tempx < 0) {
            double tempy = (phibin6reg - phibin6bar) / (phibin6reg + phibin6bar);
            hlambdaAsymmphi->SetBinContent(i, tempy);
            hlambdaregsigmaphi->SetBinContent(i, phibin6reg);
            hlambdabarsigmaphi->SetBinContent(i, phibin6bar);
            hlambdatotsigmaphi->SetBinContent(i, (phibin6reg + phibin6bar));
        }
        if (tempx > 0 && tempx < (1 * M_PI) / (6)) {
            double tempy = (phibin7reg - phibin7bar) / (phibin7reg + phibin7bar);
            hlambdaAsymmphi->SetBinContent(i, tempy);
            hlambdaregsigmaphi->SetBinContent(i, phibin7reg);
            hlambdabarsigmaphi->SetBinContent(i, phibin7bar);
            hlambdatotsigmaphi->SetBinContent(i, (phibin7reg + phibin7bar));
        }
        if (tempx > (1 * M_PI) / (6) && tempx < (2 * M_PI) / (6)) {
            double tempy = (phibin8reg - phibin8bar) / (phibin8reg + phibin8bar);
            hlambdaAsymmphi->SetBinContent(i, tempy);
            hlambdaregsigmaphi->SetBinContent(i, phibin8reg);
            hlambdabarsigmaphi->SetBinContent(i, phibin8bar);
            hlambdatotsigmaphi->SetBinContent(i, (phibin8reg + phibin8bar));
        }
        if (tempx > (2 * M_PI) / (6) && tempx < (3 * M_PI) / (6)) {
            double tempy = (phibin9reg - phibin9bar) / (phibin9reg + phibin9bar);
            hlambdaAsymmphi->SetBinContent(i, tempy);
            hlambdaregsigmaphi->SetBinContent(i, phibin9reg);
            hlambdabarsigmaphi->SetBinContent(i, phibin9bar);
            hlambdatotsigmaphi->SetBinContent(i, (phibin9reg + phibin9bar));
        }
        if (tempx > (3 * M_PI) / (6) && tempx < (4 * M_PI) / (6)) {
            double tempy = (phibin10reg - phibin10bar) / (phibin10reg + phibin10bar);
            hlambdaAsymmphi->SetBinContent(i, tempy);
            hlambdaregsigmaphi->SetBinContent(i, phibin10reg);
            hlambdabarsigmaphi->SetBinContent(i, phibin10bar);
            hlambdatotsigmaphi->SetBinContent(i, (phibin10reg + phibin10bar));
        }
        if (tempx > (4 * M_PI) / (6) && tempx < (5 * M_PI) / (6)) {
            double tempy = (phibin11reg - phibin11bar) / (phibin11reg + phibin11bar);
            hlambdaAsymmphi->SetBinContent(i, tempy);
            hlambdaregsigmaphi->SetBinContent(i, phibin11reg);
            hlambdabarsigmaphi->SetBinContent(i, phibin11bar);
            hlambdatotsigmaphi->SetBinContent(i, (phibin11reg + phibin11bar));
        }
        if (tempx > (5 * M_PI) / (6) && tempx < M_PI) {
            double tempy = (phibin12reg - phibin12bar) / (phibin12reg + phibin12bar);
            hlambdaAsymmphi->SetBinContent(i, tempy);
            hlambdaregsigmaphi->SetBinContent(i, phibin12reg);
            hlambdabarsigmaphi->SetBinContent(i, phibin12bar);
            hlambdatotsigmaphi->SetBinContent(i, (phibin12reg + phibin12bar));
        }
    }

    //Write 2D hists
    LambdaBar2D->SetStats(0);
    LambdaBar2D->GetXaxis()->SetTitle("pT [GeV]");
    LambdaBar2D->GetYaxis()->SetTitle("Eta");
    LambdaBar2D->GetXaxis()->SetTitleSize(0.04);
    LambdaBar2D->GetXaxis()->SetTitleOffset(1);
    LambdaBar2D->GetYaxis()->SetTitleSize(0.05);
    LambdaBar2D->Draw("colz");
    canvas->SetName("Anti Lambda pT vs Eta");
    canvas->Write();

    LambdaReg2D->SetStats(0);
    LambdaReg2D->GetXaxis()->SetTitle("pT [GeV]");
    LambdaReg2D->GetYaxis()->SetTitle("Eta");
    LambdaReg2D->GetXaxis()->SetTitleSize(0.04);
    LambdaReg2D->GetXaxis()->SetTitleOffset(1);
    LambdaReg2D->GetYaxis()->SetTitleSize(0.05);
    LambdaReg2D->Draw("colz");
    canvas->SetName("Reg Lambda pT vs Eta");
    canvas->Write();

    Asymm2D->SetStats(0);
    Asymm2D->GetXaxis()->SetTitle("pT [GeV]");
    Asymm2D->GetYaxis()->SetTitle("Eta");
    Asymm2D->GetXaxis()->SetTitleSize(0.04);
    Asymm2D->GetXaxis()->SetTitleOffset(1);
    Asymm2D->GetYaxis()->SetTitleSize(0.05);
    Asymm2D->Draw("colz");
    canvas->SetName("Lambda Asymmetry pT vs Eta");
    canvas->Write();


    hlambdaAsymmpT->SetStats(0);
    hlambdaAsymmpT->SetTitle("#Lambda Asymmetry vs pT");
    hlambdaAsymmpT->GetXaxis()->SetTitle("pT");
    hlambdaAsymmpT->GetYaxis()->SetTitle("Asymm");
    hlambdaAsymmpT->SetLineColor(kBlue);
    hlambdaAsymmpT->Draw();
    hlambdaAsymmpT->Write();

    hlambdaAsymmphi->SetStats(0);
    hlambdaAsymmphi->SetTitle("#Lambda Asymmetry vs Phi");
    hlambdaAsymmphi->GetXaxis()->SetTitle("Phi");
    hlambdaAsymmphi->GetYaxis()->SetTitle("Asymm");
    hlambdaAsymmphi->SetLineColor(kBlue);
    hlambdaAsymmphi->Draw();
    hlambdaAsymmphi->Write();

    hlambdaAsymmEta->SetStats(0);
    hlambdaAsymmEta->SetTitle("#Lambda Asymmetry vs Eta");
    hlambdaAsymmEta->GetXaxis()->SetTitle("Eta");
    hlambdaAsymmEta->GetYaxis()->SetTitle("Asymm");
    hlambdaAsymmEta->SetLineColor(kBlue);
    hlambdaAsymmEta->Draw();
    hlambdaAsymmEta->Write();

    hlambdaAsymmX->SetStats(0);
    hlambdaAsymmX->SetTitle("#Lambda Asymmetry vs X");
    hlambdaAsymmX->GetXaxis()->SetTitle("X");
    hlambdaAsymmX->GetYaxis()->SetTitle("Asymm");
    hlambdaAsymmX->SetLineColor(kBlue);
    hlambdaAsymmX->Draw();
    hlambdaAsymmX->Write();

    hlambdaAsymmq2->SetStats(0);
    hlambdaAsymmq2->SetTitle("#Lambda Asymmetry vs Q2");
    hlambdaAsymmq2->GetXaxis()->SetTitle("Q2");
    hlambdaAsymmq2->GetYaxis()->SetTitle("Asymm");
    hlambdaAsymmq2->SetLineColor(kBlue);
    hlambdaAsymmq2->Draw();
    hlambdaAsymmq2->Write();

  /*  TH1F* hlambdaregsigmapT = new TH1F("Lambda Reg pT Cross Section", "Lambda Reg pT Cross Section", 1000, 0, 10);
    TH1F* hlambdaregsigmaEta = new TH1F("Lambda Reg Eta Cross Section", "Lambda Reg Eta Cross Section", 800, -4, 4);
    TH1F* hlambdaregsigmaq2 = new TH1F("Lambda Reg q2 Cross Section", "Lambda Reg q2 Cross Section", 15000, 0, 150);
    TH1F* hlambdaregsigmaX = new TH1F("Lambda Reg X Cross Section", "Lambda Reg X Cross Section", 5000, 0, 0.05);

    TH1F* hlambdabarsigmapT = new TH1F("Lambda Bar pT Cross Section", "Lambda Bar pT Cross Section", 1000, 0, 10);
    TH1F* hlambdabarsigmaEta = new TH1F("Lambda Bar Eta Cross Section", "Lambda Bar Eta Cross Section", 800, -4, 4);
    TH1F* hlambdabarsigmaq2 = new TH1F("Lambda Bar q2 Cross Section", "Lambda Bar q2 Cross Section", 15000, 0, 150);
    TH1F* hlambdabarsigmaX = new TH1F("Lambda Bar X Cross Section", "Lambda Bar X Cross Section", 5000, 0, 0.05);*/

    //hlambdatest->Draw();
    //hlambdatest->Write();

    hlambdaregsigmapT->SetStats(0);
    hlambdaregsigmapT->SetTitle("#Lambda pT Cross Section");
    hlambdaregsigmapT->GetXaxis()->SetTitle("pT");
    hlambdaregsigmapT->GetYaxis()->SetTitle("Sigma");
    hlambdaregsigmapT->SetLineColor(kBlue);
    hlambdaregsigmapT->Draw();
    hlambdaregsigmapT->Write();

    hlambdabarsigmapT->SetStats(0);
    hlambdabarsigmapT->SetTitle("#bar{#Lambda} pT Cross Section");
    hlambdabarsigmapT->GetXaxis()->SetTitle("pT");
    hlambdabarsigmapT->GetYaxis()->SetTitle("Sigma");
    hlambdabarsigmapT->SetLineColor(kBlue);
    hlambdabarsigmapT->Draw();
    hlambdabarsigmapT->Write();

    hlambdatotsigmapT->SetStats(0);
    hlambdatotsigmapT->SetTitle("Total #Lambda pT Cross Section");
    hlambdatotsigmapT->GetXaxis()->SetTitle("pT");
    hlambdatotsigmapT->GetYaxis()->SetTitle("Sigma");
    hlambdatotsigmapT->SetLineColor(kBlue);
    hlambdatotsigmapT->Draw();
    hlambdatotsigmapT->Write();

    hlambdaregsigmaphi->SetStats(0);
    hlambdaregsigmaphi->SetTitle("#Lambda Phi Cross Section");
    hlambdaregsigmaphi->GetXaxis()->SetTitle("Phi");
    hlambdaregsigmaphi->GetYaxis()->SetTitle("Sigma");
    hlambdaregsigmaphi->SetLineColor(kBlue);
    hlambdaregsigmaphi->Draw();
    hlambdaregsigmaphi->Write();

    hlambdabarsigmaphi->SetStats(0);
    hlambdabarsigmaphi->SetTitle("#bar{#Lambda} Phi Cross Section");
    hlambdabarsigmaphi->GetXaxis()->SetTitle("Phi");
    hlambdabarsigmaphi->GetYaxis()->SetTitle("Sigma");
    hlambdabarsigmaphi->SetLineColor(kBlue);
    hlambdabarsigmaphi->Draw();
    hlambdabarsigmaphi->Write();

    hlambdatotsigmaphi->SetStats(0);
    hlambdatotsigmaphi->SetTitle("Total #Lambda Phi Cross Section");
    hlambdatotsigmaphi->GetXaxis()->SetTitle("Phi");
    hlambdatotsigmaphi->GetYaxis()->SetTitle("Sigma");
    hlambdatotsigmaphi->SetLineColor(kBlue);
    hlambdatotsigmaphi->Draw();
    hlambdatotsigmaphi->Write();

    hlambdaregsigmaEta->SetStats(0);
    hlambdaregsigmaEta->SetTitle("#Lambda Eta Cross Section");
    hlambdaregsigmaEta->GetXaxis()->SetTitle("Eta");
    hlambdaregsigmaEta->GetYaxis()->SetTitle("Sigma");
    hlambdaregsigmaEta->SetLineColor(kBlue);
    hlambdaregsigmaEta->Draw();
    hlambdaregsigmaEta->Write();

    hlambdabarsigmaEta->SetStats(0);
    hlambdabarsigmaEta->SetTitle("#bar{#Lambda} Eta Cross Section");
    hlambdabarsigmaEta->GetXaxis()->SetTitle("Eta");
    hlambdabarsigmaEta->GetYaxis()->SetTitle("Sigma");
    hlambdabarsigmaEta->SetLineColor(kBlue);
    hlambdabarsigmaEta->Draw();
    hlambdabarsigmaEta->Write();

    hlambdatotsigmaEta->SetStats(0);
    hlambdatotsigmaEta->SetTitle("Total #Lambda Eta Cross Section");
    hlambdatotsigmaEta->GetXaxis()->SetTitle("Eta");
    hlambdatotsigmaEta->GetYaxis()->SetTitle("Sigma");
    hlambdatotsigmaEta->SetLineColor(kBlue);
    hlambdatotsigmaEta->Draw();
    hlambdatotsigmaEta->Write();

    hlambdaregsigmaX->SetStats(0);
    hlambdaregsigmaX->SetTitle("#Lambda X Cross Section");
    hlambdaregsigmaX->GetXaxis()->SetTitle("X");
    hlambdaregsigmaX->GetYaxis()->SetTitle("Sigma");
    hlambdaregsigmaX->SetLineColor(kBlue);
    hlambdaregsigmaX->Draw();
    hlambdaregsigmaX->Write();

    hlambdabarsigmaX->SetStats(0);
    hlambdabarsigmaX->SetTitle("#bar{#Lambda} X Cross Section");
    hlambdabarsigmaX->GetXaxis()->SetTitle("X");
    hlambdabarsigmaX->GetYaxis()->SetTitle("Sigma");
    hlambdabarsigmaX->SetLineColor(kBlue);
    hlambdabarsigmaX->Draw();
    hlambdabarsigmaX->Write();

    hlambdatotsigmaX->SetStats(0);
    hlambdatotsigmaX->SetTitle("Total #Lambda X Cross Section");
    hlambdatotsigmaX->GetXaxis()->SetTitle("X");
    hlambdatotsigmaX->GetYaxis()->SetTitle("Sigma");
    hlambdatotsigmaX->SetLineColor(kBlue);
    hlambdatotsigmaX->Draw();
    hlambdatotsigmaX->Write();

    hlambdaregsigmaq2->SetStats(0);
    hlambdaregsigmaq2->SetTitle("#Lambda q2 Cross Section");
    hlambdaregsigmaq2->GetXaxis()->SetTitle("q2");
    hlambdaregsigmaq2->GetYaxis()->SetTitle("Sigma");
    hlambdaregsigmaq2->SetLineColor(kBlue);
    hlambdaregsigmaq2->Draw();
    hlambdaregsigmaq2->Write();

    hlambdabarsigmaq2->SetStats(0);
    hlambdabarsigmaq2->SetTitle("#bar{#Lambda} q2 Cross Section");
    hlambdabarsigmaq2->GetXaxis()->SetTitle("q2");
    hlambdabarsigmaq2->GetYaxis()->SetTitle("Sigma");
    hlambdabarsigmaq2->SetLineColor(kBlue);
    hlambdabarsigmaq2->Draw();
    hlambdabarsigmaq2->Write();


    hlambdatotsigmaq2->SetStats(0);
    hlambdatotsigmaq2->SetTitle("Total #Lambda q2 Cross Section");
    hlambdatotsigmaq2->GetXaxis()->SetTitle("q2");
    hlambdatotsigmaq2->GetYaxis()->SetTitle("Sigma");
    hlambdatotsigmaq2->SetLineColor(kBlue);
    hlambdatotsigmaq2->Draw();
    hlambdatotsigmaq2->Write();


    hgenlambdaregpT->SetStats(0);
    hgenlambdaregpT->SetTitle("#Lambda Raw Generator Level pT Distribution");
    hgenlambdaregpT->GetXaxis()->SetTitle("pT [GeV]");
    hgenlambdaregpT->GetYaxis()->SetTitle("Count");
    hgenlambdaregpT->SetLineColor(kRed);
    hgenlambdaregpT->Draw();
    hgenlambdaregpT->Write();

    //hgenlambdaregEta->SetStats(0);
    hgenlambdaregEta->SetTitle("#Lambda Raw Generator Level #eta Distribution");
    hgenlambdaregEta->GetXaxis()->SetTitle("#eta");
    hgenlambdaregEta->GetYaxis()->SetTitle("Count");
    hgenlambdaregEta->SetLineColor(kRed);
    hgenlambdaregEta->Draw();
    hgenlambdaregEta->Write();

    hgenlambdaregq2->SetStats(0);
    hgenlambdaregq2->SetTitle("#Lambda Raw Generator Level q2 Distribution");
    hgenlambdaregq2->GetXaxis()->SetTitle("q2");
    hgenlambdaregq2->GetYaxis()->SetTitle("Count");
    hgenlambdaregq2->SetLineColor(kRed);
    hgenlambdaregq2->Draw();
    hgenlambdaregq2->Write();

    hgenlambdaregy->SetStats(0);
    hgenlambdaregy->SetTitle("#Lambda Raw Generator Level y Distribution");
    hgenlambdaregy->GetXaxis()->SetTitle("y");
    hgenlambdaregy->GetYaxis()->SetTitle("Count");
    hgenlambdaregy->SetLineColor(kRed);
    hgenlambdaregy->Draw();
    hgenlambdaregy->Write();

    hgenlambdabarpT->SetStats(0);
    hgenlambdabarpT->SetTitle("#bar{#Lambda} Raw Generator Level pT Distribution");
    hgenlambdabarpT->GetXaxis()->SetTitle("pT [GeV]");
    hgenlambdabarpT->GetYaxis()->SetTitle("Count");
    hgenlambdabarpT->SetLineColor(kBlue);
    hgenlambdabarpT->Draw();
    hgenlambdabarpT->Write();

    //hgenlambdabarEta->SetStats(0);
    hgenlambdabarEta->SetTitle("#bar{#Lambda} Raw Generator Level #eta Distribution");
    hgenlambdabarEta->GetXaxis()->SetTitle("#eta");
    hgenlambdabarEta->GetYaxis()->SetTitle("Count");
    hgenlambdabarEta->SetLineColor(kBlue);
    hgenlambdabarEta->Draw();
    hgenlambdabarEta->Write();

    hgenlambdabarq2->SetStats(0);
    hgenlambdabarq2->SetTitle("#bar{#Lambda} Raw Generator Level #eta Distribution");
    hgenlambdabarq2->GetXaxis()->SetTitle("q2");
    hgenlambdabarq2->GetYaxis()->SetTitle("Count");
    hgenlambdabarq2->SetLineColor(kBlue);
    hgenlambdabarq2->Draw();
    hgenlambdabarq2->Write();

    hgenlambdabary->SetStats(0);
    hgenlambdabary->SetTitle("#bar{#Lambda} Raw Generator Level y Distribution");
    hgenlambdabary->GetXaxis()->SetTitle("y");
    hgenlambdabary->GetYaxis()->SetTitle("Count");
    hgenlambdabary->SetLineColor(kBlue);
    hgenlambdabary->Draw();
    hgenlambdabary->Write();

    //hlambdaregcountpT->SetStats(0);
    hlambdaregcountpT->SetTitle("#Lambda Counts After Generator Selection vs pT");
    hlambdaregcountpT->GetXaxis()->SetTitle("pT");
    hlambdaregcountpT->GetYaxis()->SetTitle("Count");
    hlambdaregcountpT->SetLineColor(kRed);
    hlambdaregcountpT->Draw();
    hlambdaregcountpT->Write();

    //hlambdaregcountEta->SetStats(0);
    hlambdaregcountEta->SetTitle("#Lambda Counts After Generator Selection vs #eta");
    hlambdaregcountEta->GetXaxis()->SetTitle("#eta");
    hlambdaregcountEta->GetYaxis()->SetTitle("Count");
    hlambdaregcountEta->SetLineColor(kRed);
    hlambdaregcountEta->Draw();
    hlambdaregcountEta->Write();

    //hlambdaregcountEta->SetStats(0);
    hlambdaregcountq2->SetTitle("#Lambda Counts After Generator Selection vs q2");
    hlambdaregcountq2->GetXaxis()->SetTitle("q2");
    hlambdaregcountq2->GetYaxis()->SetTitle("Count");
    hlambdaregcountq2->SetLineColor(kRed);
    hlambdaregcountq2->Draw();
    hlambdaregcountq2->Write();

    //hlambdaregcountEta->SetStats(0);
    hlambdaregcounty->SetTitle("#Lambda Counts After Generator Selection vs y");
    hlambdaregcounty->GetXaxis()->SetTitle("y");
    hlambdaregcounty->GetYaxis()->SetTitle("Count");
    hlambdaregcounty->SetLineColor(kRed);
    hlambdaregcounty->Draw();
    hlambdaregcounty->Write();

    //hlambdaregcountX->SetStats(0);
    hlambdaregcountX->SetTitle("#Lambda Counts After Generator Selection vs Bjorken X");
    hlambdaregcountX->GetXaxis()->SetTitle("Bjorken X");
    hlambdaregcountX->GetYaxis()->SetTitle("Count");
    hlambdaregcountX->SetLineColor(kRed);
    hlambdaregcountX->Draw();
    hlambdaregcountX->Write();

    //hlambdabarcountpT->SetStats(0);
    hlambdabarcountpT->SetTitle("#bar{#Lambda} Counts After Generator Selection vs pT");
    hlambdabarcountpT->GetXaxis()->SetTitle("pT");
    hlambdabarcountpT->GetYaxis()->SetTitle("Count");
    hlambdabarcountpT->SetLineColor(kBlue);
    hlambdabarcountpT->Draw();
    hlambdabarcountpT->Write();

    //hlambdabarcountEta->SetStats(0);
    hlambdabarcountEta->SetTitle("#bar{#Lambda} Counts After Generator Selection vs #eta");
    hlambdabarcountEta->GetXaxis()->SetTitle("#eta");
    hlambdabarcountEta->GetYaxis()->SetTitle("Count");
    hlambdabarcountEta->SetLineColor(kBlue);
    hlambdabarcountEta->Draw();
    hlambdabarcountEta->Write();

    //hlambdabarcountEta->SetStats(0);
    hlambdabarcountq2->SetTitle("#bar{#Lambda} Counts After Generator Selection vs q2");
    hlambdabarcountq2->GetXaxis()->SetTitle("q2");
    hlambdabarcountq2->GetYaxis()->SetTitle("Count");
    hlambdabarcountq2->SetLineColor(kBlue);
    hlambdabarcountq2->Draw();
    hlambdabarcountq2->Write();

    //hlambdabarcountEta->SetStats(0);
    hlambdabarcounty->SetTitle("#bar{#Lambda} Counts After Generator Selection vs y");
    hlambdabarcounty->GetXaxis()->SetTitle("y");
    hlambdabarcounty->GetYaxis()->SetTitle("Count");
    hlambdabarcounty->SetLineColor(kBlue);
    hlambdabarcounty->Draw();
    hlambdabarcounty->Write();

    //hlambdabarcountX->SetStats(0);
    hlambdabarcountX->SetTitle("#bar{#Lambda} Counts After Generator Selection vs Bjorken X");
    hlambdabarcountX->GetXaxis()->SetTitle("Bjorken X");
    hlambdabarcountX->GetYaxis()->SetTitle("Count");
    hlambdabarcountX->SetLineColor(kBlue);
    hlambdabarcountX->Draw();
    hlambdabarcountX->Write();

    hlambdaregcountphi->SetLineColor(kRed);
    hlambdabarcountphi->SetLineColor(kBlue);
    hsphi->Add(hlambdaregcountphi);
    hsphi->Add(hlambdabarcountphi);
    hsphi->Draw("nostack");
    TLegend* legendphi = new TLegend();
    legendphi->AddEntry(hlambdabarcountphi, "#bar{#Lambda}", "l");
    legendphi->AddEntry(hlambdaregcountphi, "#Lambda", "l");
    legendphi->Draw("same");
    canvas->SetName("Phi Distributions");
    canvas->Write();

    hlambdaregcounttheta->SetLineColor(kRed);
    hlambdabarcounttheta->SetLineColor(kBlue);
    hstheta->Add(hlambdaregcounttheta);
    hstheta->Add(hlambdabarcounttheta);
    hstheta->Draw("nostack");
    TLegend* legendtheta = new TLegend();
    legendtheta->AddEntry(hlambdabarcounttheta, "#bar{#Lambda}", "l");
    legendtheta->AddEntry(hlambdaregcounttheta, "#Lambda", "l");
    legendtheta->Draw("same");
    canvas->SetName("Theta Distributions");
    canvas->Write();

    hspT->Add(hgenlambdaregpT);
    hspT->Add(hgenlambdabarpT);
    hspT->Draw("nostack");
    TLegend* legend = new TLegend();
    legend->AddEntry(hgenlambdabarpT, "#bar{#Lambda}", "l");
    legend->AddEntry(hgenlambdaregpT, "#Lambda", "l");
    legend->Draw("same");
    canvas->SetName("pT Distributions");
    canvas->Write();

    hsEta->Add(hgenlambdaregEta);
    hsEta->Add(hgenlambdabarEta);
    hsEta->Draw("nostack");
    TLegend* legend2 = new TLegend();
    legend2->AddEntry(hgenlambdabarEta, "#bar{#Lambda}", "l");
    legend2->AddEntry(hgenlambdaregEta, "#Lambda", "l");
    legend2->Draw("same");
    canvas->SetName("#eta Distributions");
    canvas->Write();

    hscountq2->Add(hlambdaregcountq2);
    hscountq2->Add(hlambdabarcountq2);
    hscountq2->Draw("nostack");
    TLegend* legend3 = new TLegend();
    legend3->AddEntry(hlambdabarcountq2, "#bar{#Lambda}", "l");
    legend3->AddEntry(hlambdaregcountq2, "#Lambda", "l");
    legend3->Draw("same");
    canvas->SetName("q2 Distributions");
    canvas->Write();

    hscounty->Add(hlambdaregcounty);
    hscounty->Add(hlambdabarcounty);
    hscounty->Draw("nostack");
    TLegend* legend4 = new TLegend();
    legend4->AddEntry(hlambdabarcountq2, "#bar{#Lambda}", "l");
    legend4->AddEntry(hlambdaregcountq2, "#Lambda", "l");
    legend4->Draw("same");
    canvas->SetName("y Distributions");
    canvas->Write();

    hscountpT->Add(hlambdaregcountpT);
    hscountpT->Add(hlambdabarcountpT);
    hscountpT->Draw("nostack");
    TLegend* legend5 = new TLegend();
    legend5->AddEntry(hlambdabarcountpT, "#bar{#Lambda}", "l");
    legend5->AddEntry(hlambdaregcountpT, "#Lambda", "l");
    legend5->Draw("same");
    canvas->SetName("pT Distributions");
    canvas->Write();

    hscountEta->Add(hlambdaregcountEta);
    hscountEta->Add(hlambdabarcountEta);
    hscountEta->Draw("nostack");
    TLegend* legend6 = new TLegend();
    legend6->AddEntry(hlambdabarcountEta, "#bar{#Lambda}", "l");
    legend6->AddEntry(hlambdaregcountEta, "#Lambda", "l");
    legend6->Draw("same");
    canvas->SetName("#eta Distributions");
    canvas->Write();

    hscountq2->Add(hlambdaregcountq2);
    hscountq2->Add(hlambdabarcountq2);
    hscountq2->Draw("nostack");
    TLegend* legend7 = new TLegend();
    legend7->AddEntry(hlambdabarcountq2, "#bar{#Lambda}", "l");
    legend7->AddEntry(hlambdaregcountq2, "#Lambda", "l");
    legend7->Draw("same");
    canvas->SetName("q2 Distributions");
    canvas->Write();

    hscounty->Add(hlambdaregcounty);
    hscounty->Add(hlambdabarcounty);
    hscounty->Draw("nostack");
    TLegend* legend8 = new TLegend();
    legend8->AddEntry(hlambdabarcounty, "#bar{#Lambda}", "l");
    legend8->AddEntry(hlambdaregcounty, "#Lambda", "l");
    legend8->Draw("same");
    canvas->SetName("y Distributions");
    canvas->Write();

    hscountX->Add(hlambdaregcountX);
    hscountX->Add(hlambdabarcountX);
    hscountX->Draw("nostack");
    TLegend* legend9 = new TLegend();
    legend9->AddEntry(hlambdabarcountX, "#bar{#Lambda}", "l");
    legend9->AddEntry(hlambdaregcountX, "#Lambda", "l");
    legend9->Draw("same");
    canvas->SetName("X Distributions");
    canvas->Write();

    ////////////////////////////////////////////////////////////////////////

    hEtaTest->SetStats(0);
    hEtaTest->SetTitle("Eta Test");
    hEtaTest->GetXaxis()->SetTitle("Eta");
    hEtaTest->GetYaxis()->SetTitle("Count");
    hEtaTest->SetLineColor(kBlue);
    hEtaTest->Draw();
    hEtaTest->Write();



    hgenpiplusX->SetLineColor(kRed);
    hgenpiminusX->SetLineColor(kBlue);
    hsPiX->Add(hgenpiplusX);
    hsPiX->Add(hgenpiminusX);
    hsPiX->Draw("nostack");
    TLegend* legend10 = new TLegend();
    legend10->AddEntry(hgenpiplusX, "#Pi+", "l");
    legend10->AddEntry(hgenpiminusX, "#Pi-", "l");
    legend10->Draw("same");
    canvas->SetName("Pion X Distributions");
    canvas->Write();

    hgenpipluspT->SetLineColor(kRed);
    hgenpiminuspT->SetLineColor(kBlue);
    hsPipT->Add(hgenpipluspT);
    hsPipT->Add(hgenpiminuspT);
    hsPipT->Draw("nostack");
    TLegend* legend11 = new TLegend();
    legend11->AddEntry(hgenpipluspT, "#Pi+", "l");
    legend11->AddEntry(hgenpiminuspT, "#Pi-", "l");
    legend11->Draw("same");
    canvas->SetName("Pion pT Distributions");
    canvas->Write();

    hgenpiplusq2->SetLineColor(kRed);
    hgenpiminusq2->SetLineColor(kBlue);
    hsPiq2->Add(hgenpiplusq2);
    hsPiq2->Add(hgenpiminusq2);
    hsPiq2->Draw("nostack");
    TLegend* legend12 = new TLegend();
    legend12->AddEntry(hgenpiplusq2, "#Pi+", "l");
    legend12->AddEntry(hgenpiminusq2, "#Pi-", "l");
    legend12->Draw("same");
    canvas->SetName("Pion q2 Distributions");
    canvas->Write();

    hgenpiplusy->SetLineColor(kRed);
    hgenpiminusy->SetLineColor(kBlue);
    hsPiy->Add(hgenpiplusy);
    hsPiy->Add(hgenpiminusy);
    hsPiy->Draw("nostack");
    TLegend* legend13 = new TLegend();
    legend13->AddEntry(hgenpiplusy, "#Pi+", "l");
    legend13->AddEntry(hgenpiminusy, "#Pi-", "l");
    legend13->Draw("same");
    canvas->SetName("Pion y Distributions");
    canvas->Write();

    hgenpiplusEta->SetLineColor(kRed);
    hgenpiminusEta->SetLineColor(kBlue);
    hsPiEta->Add(hgenpiplusEta);
    hsPiEta->Add(hgenpiminusEta);
    hsPiEta->Draw("nostack");
    TLegend* legend14 = new TLegend();
    legend14->AddEntry(hgenpiplusEta, "#Pi+", "l");
    legend14->AddEntry(hgenpiminusEta, "#Pi-", "l");
    legend14->Draw("same");
    canvas->SetName("Pion Eta Distributions");
    canvas->Write();

    //
    hgenKplusX->SetLineColor(kRed);
    hgenKminusX->SetLineColor(kBlue);
    hsKX->Add(hgenKplusX);
    hsKX->Add(hgenKminusX);
    hsKX->Draw("nostack");
    TLegend* legend15 = new TLegend();
    legend15->AddEntry(hgenKplusX, "K+", "l");
    legend15->AddEntry(hgenKminusX, "K-", "l");
    legend15->Draw("same");
    canvas->SetName("Kaon X Distributions");
    canvas->Write();

    hgenKpluspT->SetLineColor(kRed);
    hgenKminuspT->SetLineColor(kBlue);
    hsKpT->Add(hgenKpluspT);
    hsKpT->Add(hgenKminuspT);
    hsKpT->Draw("nostack");
    TLegend* legend16 = new TLegend();
    legend16->AddEntry(hgenKpluspT, "K+", "l");
    legend16->AddEntry(hgenKminuspT, "K-", "l");
    legend16->Draw("same");
    canvas->SetName("Kaon pT Distributions");
    canvas->Write();

    hgenKplusq2->SetLineColor(kRed);
    hgenKminusq2->SetLineColor(kBlue);
    hsKq2->Add(hgenKplusq2);
    hsKq2->Add(hgenKminusq2);
    hsKq2->Draw("nostack");
    TLegend* legend17 = new TLegend();
    legend17->AddEntry(hgenKplusq2, "K+", "l");
    legend17->AddEntry(hgenKminusq2, "K-", "l");
    legend17->Draw("same");
    canvas->SetName("Kaon q2 Distributions");
    canvas->Write();

    hgenKplusy->SetLineColor(kRed);
    hgenKminusy->SetLineColor(kBlue);
    hsKy->Add(hgenKplusy);
    hsKy->Add(hgenKminusy);
    hsKy->Draw("nostack");
    TLegend* legend18 = new TLegend();
    legend18->AddEntry(hgenKplusy, "K+", "l");
    legend18->AddEntry(hgenKminusy, "K-", "l");
    legend18->Draw("same");
    canvas->SetName("Kaon y Distributions");
    canvas->Write();

    hgenKplusEta->SetLineColor(kRed);
    hgenKminusEta->SetLineColor(kBlue);
    hsKEta->Add(hgenKplusEta);
    hsKEta->Add(hgenKminusEta);
    hsKEta->Draw("nostack");
    TLegend* legend19 = new TLegend();
    legend19->AddEntry(hgenKplusEta, "K+", "l");
    legend19->AddEntry(hgenKminusEta, "K-", "l");
    legend19->Draw("same");
    canvas->SetName("Kaon Eta Distributions");
    canvas->Write();
    //
    hgenprotonregX->SetLineColor(kRed);
    hgenprotonbarX->SetLineColor(kBlue);
    hsProtonX->Add(hgenprotonregX);
    hsProtonX->Add(hgenprotonbarX);
    hsProtonX->Draw("nostack");
    TLegend* legend20 = new TLegend();
    legend20->AddEntry(hgenprotonregX, "Proton", "l");
    legend20->AddEntry(hgenprotonbarX, "Anti Proton", "l");
    legend20->Draw("same");
    canvas->SetName("Proton X Distributions");
    canvas->Write();

    hgenprotonregpT->SetLineColor(kRed);
    hgenprotonbarpT->SetLineColor(kBlue);
    hsProtonpT->Add(hgenprotonregpT);
    hsProtonpT->Add(hgenprotonbarpT);
    hsProtonpT->Draw("nostack");
    TLegend* legend21 = new TLegend();
    legend21->AddEntry(hgenprotonregpT, "Proton", "l");
    legend21->AddEntry(hgenprotonbarpT, "Anti Proton", "l");
    legend21->Draw("same");
    canvas->SetName("Proton pT Distributions");
    canvas->Write();

    hgenprotonregq2->SetLineColor(kRed);
    hgenprotonbarq2->SetLineColor(kBlue);
    hsProtonq2->Add(hgenprotonregq2);
    hsProtonq2->Add(hgenprotonbarq2);
    hsProtonq2->Draw("nostack");
    TLegend* legend22 = new TLegend();
    legend22->AddEntry(hgenprotonregq2, "Proton", "l");
    legend22->AddEntry(hgenprotonbarq2, "Anti Proton", "l");
    legend22->Draw("same");
    canvas->SetName("Proton q2 Distributions");
    canvas->Write();

    hgenprotonregy->SetLineColor(kRed);
    hgenprotonbary->SetLineColor(kBlue);
    hsProtony->Add(hgenprotonregy);
    hsProtony->Add(hgenprotonbary);
    hsProtony->Draw("nostack");
    TLegend* legend23 = new TLegend();
    legend23->AddEntry(hgenprotonregy, "Proton", "l");
    legend23->AddEntry(hgenprotonbary, "Anti Proton", "l");
    legend23->Draw("same");
    canvas->SetName("Proton y Distributions");
    canvas->Write();

    hgenprotonregEta->SetLineColor(kRed);
    hgenprotonbarEta->SetLineColor(kBlue);
    hsProtonEta->Add(hgenprotonregEta);
    hsProtonEta->Add(hgenprotonbarEta);
    hsProtonEta->Draw("nostack");
    TLegend* legend24 = new TLegend();
    legend24->AddEntry(hgenprotonregEta, "Proton", "l");
    legend24->AddEntry(hgenprotonbarEta, "Anti Proton", "l");
    legend24->Draw("same");
    canvas->SetName("Proton Eta Distributions");
    canvas->Write();

    hgenKdiffpT->SetLineColor(kRed);
    hgenLdiffpT->SetLineColor(kBlue);
    hsKandLcomppT->Add(hgenKdiffpT);
    hsKandLcomppT->Add(hgenLdiffpT);
    hsKandLcomppT->Draw("nostack");
    TLegend* legend25 = new TLegend();
    legend25->AddEntry(hgenKdiffpT, "K^{+} - K^{-}", "l");
    legend25->AddEntry(hgenLdiffpT, "#Lambda-#bar{#Lambda}", "l");
    legend25->Draw("same");
    canvas->SetName("Particle Difference pT Distributions");
    canvas->Write();

    hgenKdiffEta->SetLineColor(kRed);
    hgenLdiffEta->SetLineColor(kBlue);
    hsKandLcompEta->Add(hgenKdiffEta);
    hsKandLcompEta->Add(hgenLdiffEta);
    hsKandLcompEta->Draw("nostack");
    TLegend* legend26 = new TLegend();
    legend26->AddEntry(hgenKdiffEta, "K^{+} - K^{-}", "l");
    legend26->AddEntry(hgenLdiffEta, "#Lambda-#bar{#Lambda}", "l");
    legend26->Draw("same");
    canvas->SetName("Particle Difference Eta Distributions");
    canvas->Write();

    hgenKdiffX->SetLineColor(kRed);
    hgenLdiffX->SetLineColor(kBlue);
    hsKandLcompX->Add(hgenKdiffX);
    hsKandLcompX->Add(hgenLdiffX);
    hsKandLcompX->Draw("nostack");
    TLegend* legend27 = new TLegend();
    legend27->AddEntry(hgenKdiffX, "K^{+} - K^{-}", "l");
    legend27->AddEntry(hgenLdiffX, "#Lambda-#bar{#Lambda}", "l");
    legend27->Draw("same");
    canvas->SetName("Particle Difference X Distributions");
    canvas->Write();

    hgenKdiffy->SetLineColor(kRed);
    hgenLdiffy->SetLineColor(kBlue);
    hsKandLcompy->Add(hgenKdiffy);
    hsKandLcompy->Add(hgenLdiffy);
    hsKandLcompy->Draw("nostack");
    TLegend* legend28 = new TLegend();
    legend28->AddEntry(hgenKdiffy, "K^{+} - K^{-}", "l");
    legend28->AddEntry(hgenLdiffy, "#Lambda-#bar{#Lambda}", "l");
    legend28->Draw("same");
    canvas->SetName("Particle Difference y Distributions");
    canvas->Write();

    hgenKdiffq2->SetLineColor(kRed);
    hgenLdiffq2->SetLineColor(kBlue);
    hsKandLcompq2->Add(hgenKdiffq2);
    hsKandLcompq2->Add(hgenLdiffq2);
    hsKandLcompq2->Draw("nostack");
    TLegend* legend29 = new TLegend();
    legend29->AddEntry(hgenKdiffq2, "K^{+} - K^{-}", "l");
    legend29->AddEntry(hgenLdiffq2, "#Lambda-#bar{#Lambda}", "l");
    legend29->Draw("same");
    canvas->SetName("Particle Difference q2 Distributions");
    canvas->Write();


    XYvtxdensityreg->SetStats(0);
    XYvtxdensityreg->GetXaxis()->SetTitle("X");
    XYvtxdensityreg->GetYaxis()->SetTitle("Y");
    XYvtxdensityreg->GetXaxis()->SetTitleSize(0.04);
    XYvtxdensityreg->GetXaxis()->SetTitleOffset(1);
    XYvtxdensityreg->GetYaxis()->SetTitleSize(0.05);
    XYvtxdensityreg->Draw("colz");
    canvas->SetName("Reg Lambda XY Vertices");
    //canvas->SetRealAspectRatio();
    canvas->Write();

    ZXvtxdensityreg->SetStats(0);
    ZXvtxdensityreg->GetXaxis()->SetTitle("Z");
    ZXvtxdensityreg->GetYaxis()->SetTitle("X");
    ZXvtxdensityreg->GetXaxis()->SetTitleSize(0.04);
    ZXvtxdensityreg->GetXaxis()->SetTitleOffset(1);
    ZXvtxdensityreg->GetYaxis()->SetTitleSize(0.05);
    ZXvtxdensityreg->Draw("colz");
    canvas->SetName("Reg Lambda ZX Vertices");
    canvas->Write();

    ZYvtxdensityreg->SetStats(0);
    ZYvtxdensityreg->GetXaxis()->SetTitle("Z");
    ZYvtxdensityreg->GetYaxis()->SetTitle("Y");
    ZYvtxdensityreg->GetXaxis()->SetTitleSize(0.04);
    ZYvtxdensityreg->GetXaxis()->SetTitleOffset(1);
    ZYvtxdensityreg->GetYaxis()->SetTitleSize(0.05);
    ZYvtxdensityreg->Draw("colz");
    canvas->SetName("Reg Lambda ZY Vertices");
    canvas->Write();

    XYvtxdensitybar->SetStats(0);
    XYvtxdensitybar->GetXaxis()->SetTitle("X");
    XYvtxdensitybar->GetYaxis()->SetTitle("Y");
    XYvtxdensitybar->GetXaxis()->SetTitleSize(0.04);
    XYvtxdensitybar->GetXaxis()->SetTitleOffset(1);
    XYvtxdensitybar->GetYaxis()->SetTitleSize(0.05);
    XYvtxdensitybar->Draw("colz");
    canvas->SetName("Lambda Bar XY Vertices");
    canvas->Write();

    ZXvtxdensitybar->SetStats(0);
    ZXvtxdensitybar->GetXaxis()->SetTitle("Z");
    ZXvtxdensitybar->GetYaxis()->SetTitle("X");
    ZXvtxdensitybar->GetXaxis()->SetTitleSize(0.04);
    ZXvtxdensitybar->GetXaxis()->SetTitleOffset(1);
    ZXvtxdensitybar->GetYaxis()->SetTitleSize(0.05);
    ZXvtxdensitybar->Draw("colz");
    canvas->SetName("Lambda Bar ZX Vertices");
    canvas->Write();

    ZYvtxdensitybar->SetStats(0);
    ZYvtxdensitybar->GetXaxis()->SetTitle("Z");
    ZYvtxdensitybar->GetYaxis()->SetTitle("Y");
    ZYvtxdensitybar->GetXaxis()->SetTitleSize(0.04);
    ZYvtxdensitybar->GetXaxis()->SetTitleOffset(1);
    ZYvtxdensitybar->GetYaxis()->SetTitleSize(0.05);
    ZYvtxdensitybar->Draw("colz");
    canvas->SetName("Lambda Bar ZY Vertices");
    canvas->Write();










    rXYvtxdensityreg->SetStats(0);
    rXYvtxdensityreg->GetXaxis()->SetTitle("X");
    rXYvtxdensityreg->GetYaxis()->SetTitle("Y");
    rXYvtxdensityreg->GetXaxis()->SetTitleSize(0.04);
    rXYvtxdensityreg->GetXaxis()->SetTitleOffset(1);
    rXYvtxdensityreg->GetYaxis()->SetTitleSize(0.05);
    rXYvtxdensityreg->Draw("colz");
    canvas->SetName("Range Reg Lambda XY Vertices");
    //canvas->SetRealAspectRatio();
    canvas->Write();

    rZXvtxdensityreg->SetStats(0);
    rZXvtxdensityreg->GetXaxis()->SetTitle("Z");
    rZXvtxdensityreg->GetYaxis()->SetTitle("X");
    rZXvtxdensityreg->GetXaxis()->SetTitleSize(0.04);
    rZXvtxdensityreg->GetXaxis()->SetTitleOffset(1);
    rZXvtxdensityreg->GetYaxis()->SetTitleSize(0.05);
    rZXvtxdensityreg->Draw("colz");
    canvas->SetName("Range Reg Lambda ZX Vertices");
    canvas->Write();

    rZYvtxdensityreg->SetStats(0);
    rZYvtxdensityreg->GetXaxis()->SetTitle("Z");
    rZYvtxdensityreg->GetYaxis()->SetTitle("Y");
    rZYvtxdensityreg->GetXaxis()->SetTitleSize(0.04);
    rZYvtxdensityreg->GetXaxis()->SetTitleOffset(1);
    rZYvtxdensityreg->GetYaxis()->SetTitleSize(0.05);
    rZYvtxdensityreg->Draw("colz");
    canvas->SetName("Range Reg Lambda ZY Vertices");
    canvas->Write();

    rXYvtxdensitybar->SetStats(0);
    rXYvtxdensitybar->GetXaxis()->SetTitle("X");
    rXYvtxdensitybar->GetYaxis()->SetTitle("Y");
    rXYvtxdensitybar->GetXaxis()->SetTitleSize(0.04);
    rXYvtxdensitybar->GetXaxis()->SetTitleOffset(1);
    rXYvtxdensitybar->GetYaxis()->SetTitleSize(0.05);
    rXYvtxdensitybar->Draw("colz");
    canvas->SetName("Range Lambda Bar XY Vertices");
    canvas->Write();

    rZXvtxdensitybar->SetStats(0);
    rZXvtxdensitybar->GetXaxis()->SetTitle("Z");
    rZXvtxdensitybar->GetYaxis()->SetTitle("X");
    rZXvtxdensitybar->GetXaxis()->SetTitleSize(0.04);
    rZXvtxdensitybar->GetXaxis()->SetTitleOffset(1);
    rZXvtxdensitybar->GetYaxis()->SetTitleSize(0.05);
    rZXvtxdensitybar->Draw("colz");
    canvas->SetName("Range Lambda Bar ZX Vertices");
    canvas->Write();

    rZYvtxdensitybar->SetStats(0);
    rZYvtxdensitybar->GetXaxis()->SetTitle("Z");
    rZYvtxdensitybar->GetYaxis()->SetTitle("Y");
    rZYvtxdensitybar->GetXaxis()->SetTitleSize(0.04);
    rZYvtxdensitybar->GetXaxis()->SetTitleOffset(1);
    rZYvtxdensitybar->GetYaxis()->SetTitleSize(0.05);
    rZYvtxdensitybar->Draw("colz");
    canvas->SetName("Range Lambda Bar ZY Vertices");
    canvas->Write();
    //
    dXYvtxdensityreg->SetStats(0);
    dXYvtxdensityreg->GetXaxis()->SetTitle("X");
    dXYvtxdensityreg->GetYaxis()->SetTitle("Y");
    dXYvtxdensityreg->GetXaxis()->SetTitleSize(0.04);
    dXYvtxdensityreg->GetXaxis()->SetTitleOffset(1);
    dXYvtxdensityreg->GetYaxis()->SetTitleSize(0.05);
    dXYvtxdensityreg->Draw("colz");
    canvas->SetName("Decay Reg Lambda XY Vertices");
    //canvas->SetRealAspectRatio();
    canvas->Write();

    dZXvtxdensityreg->SetStats(0);
    dZXvtxdensityreg->GetXaxis()->SetTitle("Z");
    dZXvtxdensityreg->GetYaxis()->SetTitle("X");
    dZXvtxdensityreg->GetXaxis()->SetTitleSize(0.04);
    dZXvtxdensityreg->GetXaxis()->SetTitleOffset(1);
    dZXvtxdensityreg->GetYaxis()->SetTitleSize(0.05);
    dZXvtxdensityreg->Draw("colz");
    canvas->SetName("Decay Reg Lambda ZX Vertices");
    canvas->Write();

    dZYvtxdensityreg->SetStats(0);
    dZYvtxdensityreg->GetXaxis()->SetTitle("Z");
    dZYvtxdensityreg->GetYaxis()->SetTitle("Y");
    dZYvtxdensityreg->GetXaxis()->SetTitleSize(0.04);
    dZYvtxdensityreg->GetXaxis()->SetTitleOffset(1);
    dZYvtxdensityreg->GetYaxis()->SetTitleSize(0.05);
    dZYvtxdensityreg->Draw("colz");
    canvas->SetName("Decay Reg Lambda ZY Vertices");
    canvas->Write();

    dXYvtxdensitybar->SetStats(0);
    dXYvtxdensitybar->GetXaxis()->SetTitle("X");
    dXYvtxdensitybar->GetYaxis()->SetTitle("Y");
    dXYvtxdensitybar->GetXaxis()->SetTitleSize(0.04);
    dXYvtxdensitybar->GetXaxis()->SetTitleOffset(1);
    dXYvtxdensitybar->GetYaxis()->SetTitleSize(0.05);
    dXYvtxdensitybar->Draw("colz");
    canvas->SetName("Decay Lambda Bar XY Vertices");
    canvas->Write();

    dZXvtxdensitybar->SetStats(0);
    dZXvtxdensitybar->GetXaxis()->SetTitle("Z");
    dZXvtxdensitybar->GetYaxis()->SetTitle("X");
    dZXvtxdensitybar->GetXaxis()->SetTitleSize(0.04);
    dZXvtxdensitybar->GetXaxis()->SetTitleOffset(1);
    dZXvtxdensitybar->GetYaxis()->SetTitleSize(0.05);
    dZXvtxdensitybar->Draw("colz");
    canvas->SetName("Decay Lambda Bar ZX Vertices");
    canvas->Write();

    dZYvtxdensitybar->SetStats(0);
    dZYvtxdensitybar->GetXaxis()->SetTitle("Z");
    dZYvtxdensitybar->GetYaxis()->SetTitle("Y");
    dZYvtxdensitybar->GetXaxis()->SetTitleSize(0.04);
    dZYvtxdensitybar->GetXaxis()->SetTitleOffset(1);
    dZYvtxdensitybar->GetYaxis()->SetTitleSize(0.05);
    dZYvtxdensitybar->Draw("colz");
    canvas->SetName("Decay Lambda Bar ZY Vertices");
    canvas->Write();

    f.Write();
    f.Close();
    return 0;
}












