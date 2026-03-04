//Analysis Code Root Macro 
//Importing TTrees
#include <iostream>
#include <stdlib.h>
#include <cmath>
using namespace std;


//Change these file paths and output for different years
string fileDataTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/merged.root";
string fileRAPGAPTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/merged.root";
string fileDJANGOHTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/DJANGOH14/merged.root";

string outputfilename = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonCandControlPlotsAllLumi.root";

//double DataLumi = 297639.1; //nb-1
//double RAPGAPLumi = 5293265.0; //nb-1
double DJANGOHLumi = 4108957.0; //nb-1
double DataLumi = 297639.1; //nb-1
double RAPGAPLumi = 5293265.0; //nb-1
//double DJANGOHLumi = 3878866.0; //nb-1

int Plot(TFile* output,double lowerbinrange, double upperbinrange, double bins, int variable, string canvasname, string plottitle, string xtitle) {
	//Getting Data File
	TFile* Data = new TFile(fileDataTree.c_str(), "read");
	TFile* RAPGAP = new TFile(fileRAPGAPTree.c_str(), "read");
	TFile* DJANGOH = new TFile(fileDJANGOHTree.c_str(), "read");
	//Getting Event Tree from the Data file
	TTree* DataEventTree = (TTree*)Data->Get("K0");
	TTree* RAPGAPEventTree = (TTree*)RAPGAP->Get("K0");
	TTree* DJANGOHEventTree = (TTree*)DJANGOH->Get("K0");

	//defining the variables in Trees
	double pT, Eta, x, invMasskaon, Q2s, Ys, Angle, weight1, weight2;
	double SVx, SVy, SVz, PVx, PVy, PVz, K0PX, K0PY, K0PZ, ystar, ylab, scatEX, scatEY, scatEZ;
	double pospionFRUNZ, negpionFRUNZ, PosPionPt, NegPionPt, PosPionTrackStartRadius, NegPionTrackStartRadius;
	double PosPionL, NegPionL, PosPionTrackLength, NegPionTrackLength, PosPionDCA, NegPionDCA, PosPionSDCA, NegPionSDCA;
	double Chi2, MassPionProton, MassElecElec, pTrelPosPion, pTrelNegPion, RadiusSVtoPV;
	int fRunType;
	//Defining Tree Branches

	DataEventTree->SetBranchAddress("invMasskaon", &invMasskaon);
	DataEventTree->SetBranchAddress("pT", &pT);
	DataEventTree->SetBranchAddress("Eta", &Eta);
	DataEventTree->SetBranchAddress("x", &x);
	DataEventTree->SetBranchAddress("Q2s", &Q2s);
	DataEventTree->SetBranchAddress("Ys", &Ys);
	DataEventTree->SetBranchAddress("Angle", &Angle);
	DataEventTree->SetBranchAddress("weight1", &weight1);
	DataEventTree->SetBranchAddress("weight2", &weight2);
	//Candidate Control Plots
	DataEventTree->SetBranchAddress("SVx", &SVx);
	DataEventTree->SetBranchAddress("SVy", &SVy);
	DataEventTree->SetBranchAddress("SVz", &SVz);
	DataEventTree->SetBranchAddress("PVx", &PVx);
	DataEventTree->SetBranchAddress("PVy", &PVy);
	DataEventTree->SetBranchAddress("PVz", &PVz);
	DataEventTree->SetBranchAddress("K0PX", &K0PX);
	DataEventTree->SetBranchAddress("K0PY", &K0PY);
	DataEventTree->SetBranchAddress("K0PZ", &K0PZ);
	DataEventTree->SetBranchAddress("fRunType", &fRunType);
	DataEventTree->SetBranchAddress("ystar", &ystar);
	DataEventTree->SetBranchAddress("ylab", &ylab);
	DataEventTree->SetBranchAddress("scatEX", &scatEX);
	DataEventTree->SetBranchAddress("scatEY", &scatEY);
	DataEventTree->SetBranchAddress("scatEZ", &scatEZ);
	DataEventTree->SetBranchAddress("pospionFRUNZ", &pospionFRUNZ);
	DataEventTree->SetBranchAddress("negpionFRUNZ", &negpionFRUNZ);
	DataEventTree->SetBranchAddress("PosPionPt", &PosPionPt);
	DataEventTree->SetBranchAddress("NegPionPt", &NegPionPt);
	DataEventTree->SetBranchAddress("PosPionTrackStartRadius", &PosPionTrackStartRadius);
	DataEventTree->SetBranchAddress("NegPionTrackStartRadius", &NegPionTrackStartRadius);
	DataEventTree->SetBranchAddress("PosPionL", &PosPionL);
	DataEventTree->SetBranchAddress("NegPionL", &NegPionL);
	DataEventTree->SetBranchAddress("PosPionTrackLength", &PosPionTrackLength);
	DataEventTree->SetBranchAddress("NegPionTrackLength", &NegPionTrackLength);
	DataEventTree->SetBranchAddress("PosPionDCA", &PosPionDCA);
	DataEventTree->SetBranchAddress("NegPionDCA", &NegPionDCA);
	DataEventTree->SetBranchAddress("PosPionSDCA", &PosPionSDCA);
	DataEventTree->SetBranchAddress("NegPionSDCA", &NegPionSDCA);
	DataEventTree->SetBranchAddress("Chi2", &Chi2);
	//DataEventTree->SetBranchAddress("Chi2Proton", &Chi2Proton);
	DataEventTree->SetBranchAddress("MassPionProton", &MassPionProton);
	DataEventTree->SetBranchAddress("MassElecElec", &MassElecElec);
	DataEventTree->SetBranchAddress("pTrelPosPion", &pTrelPosPion);
	DataEventTree->SetBranchAddress("pTrelNegPion", &pTrelNegPion);
	DataEventTree->SetBranchAddress("RadiusSVtoPV", &RadiusSVtoPV);


	RAPGAPEventTree->SetBranchAddress("invMasskaon", &invMasskaon);
	RAPGAPEventTree->SetBranchAddress("pT", &pT);
	RAPGAPEventTree->SetBranchAddress("Eta", &Eta);
	RAPGAPEventTree->SetBranchAddress("x", &x);
	RAPGAPEventTree->SetBranchAddress("Q2s", &Q2s);
	RAPGAPEventTree->SetBranchAddress("Ys", &Ys);
	RAPGAPEventTree->SetBranchAddress("Angle", &Angle);
	RAPGAPEventTree->SetBranchAddress("weight1", &weight1);
	RAPGAPEventTree->SetBranchAddress("weight2", &weight2);
	//Candidate Control Plots
	RAPGAPEventTree->SetBranchAddress("SVx", &SVx);
	RAPGAPEventTree->SetBranchAddress("SVy", &SVy);
	RAPGAPEventTree->SetBranchAddress("SVz", &SVz);
	RAPGAPEventTree->SetBranchAddress("PVx", &PVx);
	RAPGAPEventTree->SetBranchAddress("PVy", &PVy);
	RAPGAPEventTree->SetBranchAddress("PVz", &PVz);
	RAPGAPEventTree->SetBranchAddress("K0PX", &K0PX);
	RAPGAPEventTree->SetBranchAddress("K0PY", &K0PY);
	RAPGAPEventTree->SetBranchAddress("K0PZ", &K0PZ);
	RAPGAPEventTree->SetBranchAddress("fRunType", &fRunType);
	RAPGAPEventTree->SetBranchAddress("ystar", &ystar);
	RAPGAPEventTree->SetBranchAddress("ylab", &ylab);
	RAPGAPEventTree->SetBranchAddress("scatEX", &scatEX);
	RAPGAPEventTree->SetBranchAddress("scatEY", &scatEY);
	RAPGAPEventTree->SetBranchAddress("scatEZ", &scatEZ);
	RAPGAPEventTree->SetBranchAddress("pospionFRUNZ", &pospionFRUNZ);
	RAPGAPEventTree->SetBranchAddress("negpionFRUNZ", &negpionFRUNZ);
	RAPGAPEventTree->SetBranchAddress("PosPionPt", &PosPionPt);
	RAPGAPEventTree->SetBranchAddress("NegPionPt", &NegPionPt);
	RAPGAPEventTree->SetBranchAddress("PosPionTrackStartRadius", &PosPionTrackStartRadius);
	RAPGAPEventTree->SetBranchAddress("NegPionTrackStartRadius", &NegPionTrackStartRadius);
	RAPGAPEventTree->SetBranchAddress("PosPionL", &PosPionL);
	RAPGAPEventTree->SetBranchAddress("NegPionL", &NegPionL);
	RAPGAPEventTree->SetBranchAddress("PosPionTrackLength", &PosPionTrackLength);
	RAPGAPEventTree->SetBranchAddress("NegPionTrackLength", &NegPionTrackLength);
	RAPGAPEventTree->SetBranchAddress("PosPionDCA", &PosPionDCA);
	RAPGAPEventTree->SetBranchAddress("NegPionDCA", &NegPionDCA);
	RAPGAPEventTree->SetBranchAddress("PosPionSDCA", &PosPionSDCA);
	RAPGAPEventTree->SetBranchAddress("NegPionSDCA", &NegPionSDCA);
	RAPGAPEventTree->SetBranchAddress("Chi2", &Chi2);
	//RAPGAPEventTree->SetBranchAddress("Chi2Proton", &Chi2Proton);
	RAPGAPEventTree->SetBranchAddress("MassPionProton", &MassPionProton);
	RAPGAPEventTree->SetBranchAddress("MassElecElec", &MassElecElec);
	RAPGAPEventTree->SetBranchAddress("pTrelPosPion", &pTrelPosPion);
	RAPGAPEventTree->SetBranchAddress("pTrelNegPion", &pTrelNegPion);
	RAPGAPEventTree->SetBranchAddress("RadiusSVtoPV", &RadiusSVtoPV);



	DJANGOHEventTree->SetBranchAddress("invMasskaon", &invMasskaon);
	DJANGOHEventTree->SetBranchAddress("pT", &pT);
	DJANGOHEventTree->SetBranchAddress("Eta", &Eta);
	DJANGOHEventTree->SetBranchAddress("x", &x);
	DJANGOHEventTree->SetBranchAddress("Q2s", &Q2s);
	DJANGOHEventTree->SetBranchAddress("Ys", &Ys);
	DJANGOHEventTree->SetBranchAddress("Angle", &Angle);
	DJANGOHEventTree->SetBranchAddress("weight1", &weight1);
	DJANGOHEventTree->SetBranchAddress("weight2", &weight2);
	//Candidate Control Plots
	DJANGOHEventTree->SetBranchAddress("SVx", &SVx);
	DJANGOHEventTree->SetBranchAddress("SVy", &SVy);
	DJANGOHEventTree->SetBranchAddress("SVz", &SVz);
	DJANGOHEventTree->SetBranchAddress("PVx", &PVx);
	DJANGOHEventTree->SetBranchAddress("PVy", &PVy);
	DJANGOHEventTree->SetBranchAddress("PVz", &PVz);
	DJANGOHEventTree->SetBranchAddress("K0PX", &K0PX);
	DJANGOHEventTree->SetBranchAddress("K0PY", &K0PY);
	DJANGOHEventTree->SetBranchAddress("K0PZ", &K0PZ);
	DJANGOHEventTree->SetBranchAddress("fRunType", &fRunType);
	DJANGOHEventTree->SetBranchAddress("ystar", &ystar);
	DJANGOHEventTree->SetBranchAddress("ylab", &ylab);
	DJANGOHEventTree->SetBranchAddress("scatEX", &scatEX);
	DJANGOHEventTree->SetBranchAddress("scatEY", &scatEY);
	DJANGOHEventTree->SetBranchAddress("scatEZ", &scatEZ);
	DJANGOHEventTree->SetBranchAddress("pospionFRUNZ", &pospionFRUNZ);
	DJANGOHEventTree->SetBranchAddress("negpionFRUNZ", &negpionFRUNZ);
	DJANGOHEventTree->SetBranchAddress("PosPionPt", &PosPionPt);
	DJANGOHEventTree->SetBranchAddress("NegPionPt", &NegPionPt);
	DJANGOHEventTree->SetBranchAddress("PosPionTrackStartRadius", &PosPionTrackStartRadius);
	DJANGOHEventTree->SetBranchAddress("NegPionTrackStartRadius", &NegPionTrackStartRadius);
	DJANGOHEventTree->SetBranchAddress("PosPionL", &PosPionL);
	DJANGOHEventTree->SetBranchAddress("NegPionL", &NegPionL);
	DJANGOHEventTree->SetBranchAddress("PosPionTrackLength", &PosPionTrackLength);
	DJANGOHEventTree->SetBranchAddress("NegPionTrackLength", &NegPionTrackLength);
	DJANGOHEventTree->SetBranchAddress("PosPionDCA", &PosPionDCA);
	DJANGOHEventTree->SetBranchAddress("NegPionDCA", &NegPionDCA);
	DJANGOHEventTree->SetBranchAddress("PosPionSDCA", &PosPionSDCA);
	DJANGOHEventTree->SetBranchAddress("NegPionSDCA", &NegPionSDCA);
	DJANGOHEventTree->SetBranchAddress("Chi2", &Chi2);
	//DJANGOHEventTree->SetBranchAddress("Chi2Proton", &Chi2Proton);
	DJANGOHEventTree->SetBranchAddress("MassPionProton", &MassPionProton);
	DJANGOHEventTree->SetBranchAddress("MassElecElec", &MassElecElec);
	DJANGOHEventTree->SetBranchAddress("pTrelPosPion", &pTrelPosPion);
	DJANGOHEventTree->SetBranchAddress("pTrelNegPion", &pTrelNegPion);
	DJANGOHEventTree->SetBranchAddress("RadiusSVtoPV", &RadiusSVtoPV);





	//Creating Canvas and pads
	TCanvas* canvas = new TCanvas("c1", "multipads", 800, 800);
	TPad* pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
	// Histograms Binning, have some histograms filled directly from analysis that I use as a consistency check with the histograms I make from my Analysis code using the TTrees I create here
	//Double_t bins = 100;
	//Double_t lowerbinrange = 0.1;
	//Double_t upperbinrange = 0.6;

	
	//Define the three hists: Data, RAPGAP, DJANGOH
	TH1F* DataHist = new TH1F("Data", "Data", bins, lowerbinrange, upperbinrange);
	TH1F* RAPGAPHist = new TH1F("RAPGAP", "RAPGAP", bins, lowerbinrange, upperbinrange);
	TH1F* DJANGOHHist = new TH1F("DJANGOH", "DJANGOH", bins, lowerbinrange, upperbinrange);

	TH1F* DataRatioHist = new TH1F();
	TH1F* RAPGAPRatioHist = new TH1F();
	TH1F* DJANGOHRatioHist = new TH1F();

	//Setting Histogram Colors
	DataHist->SetLineColor(kBlack);
	RAPGAPHist->SetLineColor(kRed);
	DJANGOHHist->SetLineColor(kBlue);
	DataRatioHist->SetLineColor(kBlack);
	RAPGAPRatioHist->SetLineColor(kRed);
	DJANGOHRatioHist->SetLineColor(kBlue);

	//Setting Stats to Not True
	DataHist->SetStats(0);
	RAPGAPHist->SetStats(0);
	DJANGOHHist->SetStats(0);

	
	
	//Loop over Data Tree Entries
	int m;
	int entries;//get number of entries in the Event Tree we just loaded in
	entries = DataEventTree->GetEntries();
	double numdataevents = 0;
	for (m = 0; m < entries; m++) {
		if (variable == 0) {
			DataEventTree->GetEntry(m);
			DataHist->Fill(invMasskaon, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;
		}
		if (variable == 1) {
			DataEventTree->GetEntry(m);
			DataHist->Fill(Eta, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;
		}
		if (variable == 2) {
			DataEventTree->GetEntry(m);
			DataHist->Fill(pT, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;
		}
		if (variable == 3) {
			DataEventTree->GetEntry(m);
			DataHist->Fill(pospionFRUNZ, weight1 * weight2);
			DataHist->Fill(negpionFRUNZ, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;
		}
		if (variable == 4) {
			/*DataEventTree->GetEntry(m);
			DataHist->Fill(protonFRUNZ, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;*/
		}
		if (variable == 5) {
			DataEventTree->GetEntry(m);
			DataHist->Fill(PosPionPt, weight1 * weight2);
			DataHist->Fill(NegPionPt, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;
		}
		if (variable == 6) {
			/*DataEventTree->GetEntry(m);
			DataHist->Fill(ProtonPt, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;*/
		}
		if (variable == 7) {
			DataEventTree->GetEntry(m);
			DataHist->Fill(PosPionTrackLength, weight1 * weight2);
			DataHist->Fill(NegPionTrackLength, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;
		}
		if (variable == 8) {
			/*DataEventTree->GetEntry(m);
			DataHist->Fill(ProtonTrackLength, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;*/
		}
		if (variable == 9) {
			DataEventTree->GetEntry(m);
			DataHist->Fill(pTrelPosPion, weight1 * weight2);
			DataHist->Fill(pTrelNegPion, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;
		}
		if (variable == 10) {
			/*DataEventTree->GetEntry(m);
			DataHist->Fill(pTrelProton, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;*/
		}
		if (variable == 11) {
			DataEventTree->GetEntry(m);
			DataHist->Fill(Chi2, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;
		}
		if (variable == 12) {
			DataEventTree->GetEntry(m);
			DataHist->Fill(RadiusSVtoPV, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;
		}
		if (variable == 13) {
			DataEventTree->GetEntry(m);
			DataHist->Fill(PosPionDCA, weight1 * weight2);
			DataHist->Fill(NegPionDCA, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;
		}
		if (variable == 14) {
			/*DataEventTree->GetEntry(m);
			DataHist->Fill(ProtonDCA, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;*/
		}
		if (variable == 15) {
			DataEventTree->GetEntry(m);
			DataHist->Fill(PosPionSDCA, weight1 * weight2);
			DataHist->Fill(NegPionSDCA, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;
		}
		if (variable == 16) {
			/*DataEventTree->GetEntry(m);
			DataHist->Fill(ProtonSDCA, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;*/
		}
		if (variable == 17) {
			DataEventTree->GetEntry(m);
			DataHist->Fill(PosPionL, weight1 * weight2);
			DataHist->Fill(NegPionL, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;
		}
		if (variable == 18) {
			/*DataEventTree->GetEntry(m);
			DataHist->Fill(ProtonL, weight1 * weight2);
			numdataevents = numdataevents + 1 * weight1 * weight2;*/
		}
	}

	//Loop over RAPGAP Tree Entries
	int n;
	entries = RAPGAPEventTree->GetEntries();
	double numRAPGAPevents = 0;
	for (n = 0; n < entries; n++) {
		if (variable == 0) {
			RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(invMasskaon, weight1 * weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
		}
		if (variable == 1) {
			RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(Eta, weight1 * weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
		}
		if (variable == 2) {
			RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(pT, weight1 * weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
		}
		if (variable == 3) {
			RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(pospionFRUNZ, weight1 * weight2);
			RAPGAPHist->Fill(negpionFRUNZ, weight1* weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
		}
		if (variable == 4) {
			/*RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(protonFRUNZ, weight1 * weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;*/
		}
		if (variable == 5) {
			RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(PosPionPt, weight1 * weight2);
			RAPGAPHist->Fill(NegPionPt, weight1* weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
		}
		if (variable == 6) {
			/*RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(ProtonPt, weight1 * weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;*/
		}
		if (variable == 7) {
			RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(PosPionTrackLength, weight1 * weight2);
			RAPGAPHist->Fill(NegPionTrackLength, weight1* weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
		}
		if (variable == 8) {
			/*RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(ProtonTrackLength, weight1 * weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;*/
		}
		if (variable == 9) {
			RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(pTrelPosPion, weight1 * weight2);
			RAPGAPHist->Fill(pTrelNegPion, weight1* weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
		}
		if (variable == 10) {
			/*RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(pTrelProton, weight1 * weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;*/
		}
		if (variable == 11) {
			RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(Chi2, weight1 * weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
		}
		if (variable == 12) {
			RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(RadiusSVtoPV, weight1 * weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
		}
		if (variable == 13) {
			RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(PosPionDCA, weight1 * weight2);
			RAPGAPHist->Fill(NegPionDCA, weight1* weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
		}
		if (variable == 14) {
			/*RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(ProtonDCA, weight1 * weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;*/
		}
		if (variable == 15) {
			RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(PosPionSDCA, weight1 * weight2);
			RAPGAPHist->Fill(NegPionSDCA, weight1* weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
		}
		if (variable == 16) {
			/*RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(ProtonSDCA, weight1 * weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;*/
		}
		if (variable == 17) {
			RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(PosPionL, weight1 * weight2);
			RAPGAPHist->Fill(NegPionL, weight1 * weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
		}
		if (variable == 18) {
		/*	RAPGAPEventTree->GetEntry(n);
			RAPGAPHist->Fill(ProtonL, weight1 * weight2);
			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;*/
		}
	}

	//Loop over DJANGOH Tree Entries
	int p;
	entries = DJANGOHEventTree->GetEntries();
	double numDJANGOHevents = 0;
	for (p = 0; p < entries; p++) {
		if (variable == 0) {
			DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(invMasskaon, weight1 * weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
		}
		if (variable == 1) {
			DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(Eta, weight1 * weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
		}
		if (variable == 2) {
			DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(pT, weight1 * weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
		}
		if (variable == 3) {
			DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(pospionFRUNZ, weight1 * weight2);
			DJANGOHHist->Fill(negpionFRUNZ, weight1* weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
		}
		if (variable == 4) {
			/*DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(protonFRUNZ, weight1* weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;*/
		}
		if (variable == 5) {
			DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(PosPionPt, weight1* weight2);
			DJANGOHHist->Fill(NegPionPt, weight1* weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
		}
		if (variable == 6) {
			/*DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(ProtonPt, weight1* weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;*/
		}
		if (variable == 7) {
			DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(PosPionTrackLength, weight1* weight2);
			DJANGOHHist->Fill(NegPionTrackLength, weight1* weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
		}
		if (variable == 8) {
		/*	DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(ProtonTrackLength, weight1* weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;*/
		}
		if (variable == 9) {
			DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(pTrelPosPion, weight1 * weight2);
			DJANGOHHist->Fill(pTrelNegPion, weight1* weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
		}
		if (variable == 10) {
		/*	DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(pTrelProton, weight1 * weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;*/
		}
		if (variable == 11) {
			DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(Chi2, weight1 * weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
		}
		if (variable == 12) {
			DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(RadiusSVtoPV, weight1 * weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
		}
		if (variable == 13) {
			DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(PosPionDCA, weight1 * weight2);
			DJANGOHHist->Fill(NegPionDCA, weight1* weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
		}
		if (variable == 14) {
			/*DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(ProtonDCA, weight1 * weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;*/
		}
		if (variable == 15) {
			DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(PosPionSDCA, weight1 * weight2);
			DJANGOHHist->Fill(NegPionSDCA, weight1* weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
		}
		if (variable == 16) {
			/*DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(ProtonSDCA, weight1 * weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;*/
		}
		if (variable == 17) {
			DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(PosPionL, weight1 * weight2);
			DJANGOHHist->Fill(NegPionL, weight1 * weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
		}
		if (variable == 18) {
			/*DJANGOHEventTree->GetEntry(p);
			DJANGOHHist->Fill(ProtonL, weight1 * weight2);
			numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;*/
		}
	}

	//create file to output plots to
	//TFile outputfile(outputfilename.c_str(), "RECREATE");
	output->cd();
	canvas->SetName(canvasname.c_str());
	DataHist->SetTitle(plottitle.c_str());
	//DataHist->GetXaxis()->SetTitle("Q^{2}");
	//change to pad1 before drawing
	canvas->cd();
	pad1->SetBottomMargin(0.01);
	//pad1->SetGridx();
	pad1->Draw();
	pad1->cd();
	DataHist->GetYaxis()->SetTitle("Entries");

	TAxis* axis = DataHist->GetYaxis();
	axis->ChangeLabel(1, -1, -1, -1, -1, -1, " ");
	
	DataHist->GetYaxis()->SetTitleFont(43);
	DataHist->GetYaxis()->SetTitleSize(20);
	DataHist->GetYaxis()->SetLabelFont(43);
	DataHist->GetYaxis()->SetLabelSize(15);
	DataHist->GetYaxis()->SetTitleOffset(1.55);
	//DataHist->GetYaxis()->SetTitleSize(0.35);
	//DataHist->Scale(1 / numdataevents);
	//RAPGAPHist->Scale(1 / numRAPGAPevents);
	//DJANGOHHist->Scale(1 / numDJANGOHevents);
	//DataHist->Scale(1 / DataLumi);
	//RAPGAPHist->Scale(1 / RAPGAPLumi);
	//DJANGOHHist->Scale(1 / DJANGOHLumi);
	DataHist->Scale(1);
	RAPGAPHist->Scale(DataLumi / RAPGAPLumi);
	DJANGOHHist->Scale(DataLumi / DJANGOHLumi);
	DataHist->Draw("e");
	RAPGAPHist->Draw("histsame");
	DJANGOHHist->Draw("histsame");
	//DataHist->GetXaxis()->SetLabelOffset(999);
	DataHist->GetXaxis()->SetLabelSize(0);

	TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	//legend->SetHeader(Form("#splitline{Count- %.2f #pm %.2f (stat.)}{Stochastic MLP- %s Network Trained %ix}", kcount, kcounterror, network[particlecode].c_str(), trainnum[particlecode])); // option "C" allows to center the header
	legend->AddEntry(DataHist, "2005-2007 Data", "l");
	legend->AddEntry(RAPGAPHist, "#frac{L_{Data}}{L_{RAPGAP}} RAPGAP", "l");
	legend->AddEntry(DJANGOHHist, "#frac{L_{Data}}{L_{DJANGOH}} DJANGOH", "l");
	legend->Draw("same");
	
	//Ratio plot will be in the lower pad
	// Go back to the main canvas before defining pad2
	//TPad* pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
	TPad* pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
	canvas->cd();
	pad2->SetTopMargin(0.03);
	pad2->SetBottomMargin(0.4);
	//pad2->SetGridx();
	pad2->Draw();
	pad2->cd();
	*DataRatioHist = (*DataHist) / (*DataHist);
	*RAPGAPRatioHist = (*RAPGAPHist) / (*DataHist);
	*DJANGOHRatioHist = (*DJANGOHHist) / (*DataHist);
	DataRatioHist->SetTitle("");
	DataRatioHist->GetXaxis()->SetTitle(xtitle.c_str());
	DataRatioHist->GetXaxis()->SetTitleOffset(2.0);
	//DataRatioHist->GetXaxis()->SetLabelOffset(0.015);
	DataRatioHist->GetYaxis()->SetTitle("Rec MC / Data");
	//DataRatioHist->GetYaxis()->SetTitleSize(0.35);
	//DataRatioHist->GetXaxis()->SetTitleSize(0.35);
	DataRatioHist->GetYaxis()->SetRangeUser(0, 2);
	DataRatioHist->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	DataRatioHist->GetXaxis()->SetLabelSize(15);
	DataRatioHist->GetYaxis()->SetLabelFont(43);
	DataRatioHist->GetYaxis()->SetLabelSize(15);
	DataRatioHist->GetXaxis()->SetTitleFont(43);
	DataRatioHist->GetXaxis()->SetTitleSize(20);
	DataRatioHist->GetXaxis()->SetTitleOffset(4.55);

	TAxis* axis2 = DataRatioHist->GetYaxis();
	axis->ChangeLabel(1, -1, -1, -1, -1, -1, " ");

	DataRatioHist->Draw("Hist][");
	RAPGAPRatioHist->Draw("same");
	DJANGOHRatioHist->Draw("same");

	canvas->Write();
	//legend->DeleteEntry();
	//legend->DeleteEntry();
	Data->Close();
	RAPGAP->Close();
	DJANGOH->Close();

	return 0;

};

void KaonCandControlPlots() {
	//create file to output plots to
	TFile* outputfile = new TFile(outputfilename.c_str(), "RECREATE");
	//TFile* f = new TFile(dummyfile.c_str(), "RECREATE");

	Plot(outputfile, 0.4, 0.58, 100, 0,"Mass Distributions","K^{0}_{s} Mass Distributions","Mass[GeV]");
	cout << "Mass done" << endl;
	Plot(outputfile, -1.3, 1.3, 100, 1, "Eta Distributions", "K^{0}_{s} #eta Distributions", "#eta");
	cout << "Eta done" << endl;
	Plot(outputfile, 0.5, 3.5, 100, 2, "pT Distributions", "K^{0}_{s} p_{T} Distributions", "p_{T} [GeV]");
	cout << "pT done" << endl;
	Plot(outputfile, 0.0, 0.6, 100, 3, "pion FRUNZ Distributions", "FRUNZ for Pion Daughters of K^{0}_{s}", "FRUNZ");
	cout << "pion FRUNZ done" << endl;
	/*Plot(outputfile, 0.0, 0.6, 100, 4, "proton FRUNZ Distributions", "FRUNZ for Proton Daughters of #Lambda + #bar{#Lambda}", "FRUNZ");
	cout << "proton FRUNZ done" << endl;*/
	Plot(outputfile, 0.0, 2.0, 100, 5, "pion pT Distributions", "Pion Daughters of K^{0}_{s} p_{T} Distributions", "p_{T} [GeV]");
	cout << "pion pT done" << endl;
	/*Plot(outputfile, 0.0, 3.5, 100, 6, "proton pT Distributions", "Proton Daughters of #Lambda + #bar{#Lambda} p_{T} Distributions", "p_{T} [GeV]");
	cout << "proton pT done" << endl;*/
	Plot(outputfile, 10, 65, 200, 7, "pion Track Length Distributions", "Pion Daughters of K^{0}_{s} Track Length Distributions", "Track Length [cm]");
	cout << "poin track length done" << endl;
	/*Plot(outputfile, 0.0, 65, 200, 8, "proton Track Length Distributions", "Proton Daughters of #Lambda + #bar{#Lambda} Track Length Distributions", "Track Length [cm]");
	cout << "proton track length done" << endl;*/
	Plot(outputfile, 0.0, 0.35, 100, 9, "pion pTrel", "Pion Daughters of K^{0}_{s} p_{T,rel}", "p_{T,rel}");
	cout << "pion pTrel" << endl;
	/*Plot(outputfile, 0.0, 0.2, 100, 10, "proton pTrel", "Proton Daughters of #Lambda + #bar{#Lambda} p_{T,rel}", "p_{T,rel}");
	cout << "proton pTrel" << endl;*/
	Plot(outputfile, 0.0, 5, 100, 11, "Chi2", "#Chi^{2} Secondary Vertex Fit of Daughter Tracks", "#Chi^{2}");
	cout << "Chi 2 done" << endl;
	Plot(outputfile, 0.0, 40, 200, 12, "Radial Distance from SV to PV", "Radial Distance from SV to PV", "R [cm]");
	cout << "Radial Distance from SV to PV done" << endl;
	


	Plot(outputfile, -10.0, 10, 100, 13, "Pion DCA", "Pion DCA", "DCA");
	cout << "Pion DCA done" << endl;
	/*Plot(outputfile, -15.0, 15.0, 250, 14, "Proton DCA", "Proton DCA", "DCA");
	cout << "Proton DCA done" << endl;*/
	Plot(outputfile, 0.0, 100, 100, 15, "Pion |SDCA|", "Pion |SDCA|", "|SDCA|");
	cout << "Pion SDCA done" << endl;
	/*Plot(outputfile, 0.0, 100, 500, 16, "Proton |SDCA|", "Proton |SDCA|", "|SDCA|");
	cout << "Proton SDCA done" << endl;*/
	Plot(outputfile, 0.0, 1.0, 100, 17, "Pion L", "Pion Likelihood", "Pion L_{#frac{dE}{dx}}");
	cout << "Pion L done" << endl;
	/*Plot(outputfile, 0.0, 1.0, 100, 18, "Proton L", "Proton Likelihood", "Proton L_{#frac{dE}{dx}}");
	cout << "Proton L done" << endl;*/

	/*Plot(outputfile, 0.1, 0.6, 100, 1, "y Distributions", "y Distributions", "y");
	cout << "Y done" << endl;
	Plot(outputfile, -30, 30, 100, 2, "Z Vtx Distributions", "Z_{Vtx} Distributions", "Z_{Vtx} [cm]");
	cout << "Z Vtx done" << endl;
	Plot(outputfile, 35, 70, 100, 3, "Epz Distributions", "Epz Distributions", "E-pz");
	cout << "Epz done" << endl;
	Plot(outputfile, 11, 32, 100, 4, "Ee Distributions", "E_{e} Distributions", "E_{e} [GeV]");
	cout << "Ee done" << endl;
	Plot(outputfile, -4, 4, 50, 5, "Phi Distributions", "#phi Distributions", "#phi");
	cout << "Phi done" << endl;
	Plot(outputfile, 2.5, 3.5, 50, 6, "Theta Distributions", "#theta Distributions", "#theta");
	cout << "Theta done" << endl;
	Plot(outputfile, 2, 4, 50, 7, "Ecra Distributions", "Ecra Distributions", "Ecra [cm]");
	cout << "Ecra done" << endl;*/

	outputfile->cd();
	outputfile->Write();
	outputfile->Close();

	cout << "Done" << endl;
}