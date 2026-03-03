//Analysis Code Root Macro 
//Importing TTrees
#include <iostream>
#include <stdlib.h>
#include <cmath>
using namespace std;


//Change these file paths and output for different years
string fileDataTree = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/EventLists/merged.root";
string fileRAPGAPTree = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/EventLists/RAPGAP31/merged.root";
string fileDJANGOHTree = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/EventLists/DJANGOH14/merged.root";
string filePYTHIATree = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/EventLists/PYTHIA62/merged.root";

string outputfilename = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/DISControlPlotsTEST.root";

double DataLumi = 297639.1; //nb-1
double RAPGAPLumi = 5293265.0; //nb-1
double DJANGOHLumi = 4108957.0; //nb-1
double PYTHIALumi = 14580680.0; //nb-1

int Plot(TFile* output,double lowerbinrange, double upperbinrange, double bins, int variable, string canvasname, string plottitle, string xtitle, string yaxis) {
	//Getting Data File
	TFile* Data = new TFile(fileDataTree.c_str(), "read");
	TFile* RAPGAP = new TFile(fileRAPGAPTree.c_str(), "read");
	TFile* DJANGOH = new TFile(fileDJANGOHTree.c_str(), "read");
	TFile* PYTHIA = new TFile(filePYTHIATree.c_str(), "read");
	//Getting Event Tree from the Data file
	TTree* DataEventTree = (TTree*)Data->Get("Data Event Tree");
	TTree* RAPGAPEventTree = (TTree*)RAPGAP->Get("Data Event Tree");
	TTree* DJANGOHEventTree = (TTree*)DJANGOH->Get("Data Event Tree");
	TTree* PYTHIAEventTree = (TTree*)PYTHIA->Get("Data Event Tree");

	//Define the variables and banch addresses for data we want from the Event Tree entries
	double weight1, weight2, elecPhi, elecTheta, Epz, elecE, x, Q2s, Ys, Q2sGen, YsGen, RTheta, ClusterRadius, Zvertex, ProtonEnergy, ElectronEnergy, Xcluster, Ycluster, Zcluster, RunNumber, RunLumi, fposEX, fposEY, fRunType; //, RCluster,;
	double measuredCharge, epT;
	int s61, s1, s74, s75, s67, beamCharge, TrType, TrackCheck;
	int fMyRunYear;
	//Defining Tree Branches
	DataEventTree->SetBranchAddress("elecPhi", &elecPhi);
	DataEventTree->SetBranchAddress("elecTheta", &elecTheta);
	DataEventTree->SetBranchAddress("x", &x);
	DataEventTree->SetBranchAddress("Q2s", &Q2s);
	DataEventTree->SetBranchAddress("Ys", &Ys);
	DataEventTree->SetBranchAddress("Q2sGen", &Q2sGen);
	DataEventTree->SetBranchAddress("YsGen", &YsGen);
	DataEventTree->SetBranchAddress("Epz", &Epz);
	DataEventTree->SetBranchAddress("elecE", &elecE);
	DataEventTree->SetBranchAddress("RTheta", &RTheta);
	DataEventTree->SetBranchAddress("ClusterRadius", &ClusterRadius);
	DataEventTree->SetBranchAddress("Zvertex", &Zvertex);
	DataEventTree->SetBranchAddress("ProtonEnergy", &ProtonEnergy);
	DataEventTree->SetBranchAddress("ElectronEnergy", &ElectronEnergy);
	DataEventTree->SetBranchAddress("Xcluster", &Xcluster);
	DataEventTree->SetBranchAddress("Ycluster", &Ycluster);
	DataEventTree->SetBranchAddress("Zcluster", &Zcluster);
	DataEventTree->SetBranchAddress("RunNumber", &RunNumber);
	DataEventTree->SetBranchAddress("RunLumi", &RunLumi);
	//DataEventTree->SetBranchAddress("fRunType", &fRunType);
	DataEventTree->SetBranchAddress("fMyRunYear", &fMyRunYear);
	DataEventTree->SetBranchAddress("fposEX", &fposEX);
	DataEventTree->SetBranchAddress("fposEY", &fposEY);
	DataEventTree->SetBranchAddress("weight1", &weight1);
	DataEventTree->SetBranchAddress("weight2", &weight2);
	DataEventTree->SetBranchAddress("s61", &s61);
	DataEventTree->SetBranchAddress("s1", &s1);
	DataEventTree->SetBranchAddress("s74", &s74);
	DataEventTree->SetBranchAddress("s75", &s75);
	DataEventTree->SetBranchAddress("s67", &s67);
	DataEventTree->SetBranchAddress("beamCharge", &beamCharge);
	DataEventTree->SetBranchAddress("TrType", &TrType);
	DataEventTree->SetBranchAddress("TrackCheck", &TrackCheck);
	DataEventTree->SetBranchAddress("measuredCharge", &measuredCharge);
	DataEventTree->SetBranchAddress("epT", &epT);
	//LambdaBarTree->SetBranchAddress("invMasslambda", &invMasslambda);
	RAPGAPEventTree->SetBranchAddress("elecPhi", &elecPhi);
	RAPGAPEventTree->SetBranchAddress("elecTheta", &elecTheta);
	RAPGAPEventTree->SetBranchAddress("x", &x);
	RAPGAPEventTree->SetBranchAddress("Q2s", &Q2s);
	RAPGAPEventTree->SetBranchAddress("Ys", &Ys);
	RAPGAPEventTree->SetBranchAddress("Q2sGen", &Q2sGen);
	RAPGAPEventTree->SetBranchAddress("YsGen", &YsGen);
	RAPGAPEventTree->SetBranchAddress("Epz", &Epz);
	RAPGAPEventTree->SetBranchAddress("elecE", &elecE);
	RAPGAPEventTree->SetBranchAddress("RTheta", &RTheta);
	RAPGAPEventTree->SetBranchAddress("ClusterRadius", &ClusterRadius);
	RAPGAPEventTree->SetBranchAddress("Zvertex", &Zvertex);
	RAPGAPEventTree->SetBranchAddress("ProtonEnergy", &ProtonEnergy);
	RAPGAPEventTree->SetBranchAddress("ElectronEnergy", &ElectronEnergy);
	RAPGAPEventTree->SetBranchAddress("Xcluster", &Xcluster);
	RAPGAPEventTree->SetBranchAddress("Ycluster", &Ycluster);
	RAPGAPEventTree->SetBranchAddress("Zcluster", &Zcluster);
	RAPGAPEventTree->SetBranchAddress("RunNumber", &RunNumber);
	RAPGAPEventTree->SetBranchAddress("RunLumi", &RunLumi);
	//RAPGAPEventTree->SetBranchAddress("fRunType", &fRunType);
	RAPGAPEventTree->SetBranchAddress("fMyRunYear", &fMyRunYear);
	RAPGAPEventTree->SetBranchAddress("fposEX", &fposEX);
	RAPGAPEventTree->SetBranchAddress("fposEY", &fposEY);
	RAPGAPEventTree->SetBranchAddress("weight1", &weight1);
	RAPGAPEventTree->SetBranchAddress("weight2", &weight2);
	RAPGAPEventTree->SetBranchAddress("s61", &s61);
	RAPGAPEventTree->SetBranchAddress("s1", &s1);
	RAPGAPEventTree->SetBranchAddress("s74", &s74);
	RAPGAPEventTree->SetBranchAddress("s75", &s75);
	RAPGAPEventTree->SetBranchAddress("s67", &s67);
	RAPGAPEventTree->SetBranchAddress("beamCharge", &beamCharge);
	RAPGAPEventTree->SetBranchAddress("TrType", &TrType);
	RAPGAPEventTree->SetBranchAddress("TrackCheck", &TrackCheck);
	RAPGAPEventTree->SetBranchAddress("measuredCharge", &measuredCharge);
	RAPGAPEventTree->SetBranchAddress("epT", &epT);

	DJANGOHEventTree->SetBranchAddress("elecPhi", &elecPhi);
	DJANGOHEventTree->SetBranchAddress("elecTheta", &elecTheta);
	DJANGOHEventTree->SetBranchAddress("x", &x);
	DJANGOHEventTree->SetBranchAddress("Q2s", &Q2s);
	DJANGOHEventTree->SetBranchAddress("Ys", &Ys);
	DJANGOHEventTree->SetBranchAddress("Q2sGen", &Q2sGen);
	DJANGOHEventTree->SetBranchAddress("YsGen", &YsGen);
	DJANGOHEventTree->SetBranchAddress("Epz", &Epz);
	DJANGOHEventTree->SetBranchAddress("elecE", &elecE);
	DJANGOHEventTree->SetBranchAddress("RTheta", &RTheta);
	DJANGOHEventTree->SetBranchAddress("ClusterRadius", &ClusterRadius);
	DJANGOHEventTree->SetBranchAddress("Zvertex", &Zvertex);
	DJANGOHEventTree->SetBranchAddress("ProtonEnergy", &ProtonEnergy);
	DJANGOHEventTree->SetBranchAddress("ElectronEnergy", &ElectronEnergy);
	DJANGOHEventTree->SetBranchAddress("Xcluster", &Xcluster);
	DJANGOHEventTree->SetBranchAddress("Ycluster", &Ycluster);
	DJANGOHEventTree->SetBranchAddress("Zcluster", &Zcluster);
	DJANGOHEventTree->SetBranchAddress("RunNumber", &RunNumber);
	DJANGOHEventTree->SetBranchAddress("RunLumi", &RunLumi);
	//DJANGOHEventTree->SetBranchAddress("fRunType", &fRunType);
	DJANGOHEventTree->SetBranchAddress("fMyRunYear", &fMyRunYear);
	DJANGOHEventTree->SetBranchAddress("fposEX", &fposEX);
	DJANGOHEventTree->SetBranchAddress("fposEY", &fposEY);
	DJANGOHEventTree->SetBranchAddress("weight1", &weight1);
	DJANGOHEventTree->SetBranchAddress("weight2", &weight2);
	DJANGOHEventTree->SetBranchAddress("s61", &s61);
	DJANGOHEventTree->SetBranchAddress("s1", &s1);
	DJANGOHEventTree->SetBranchAddress("s74", &s74);
	DJANGOHEventTree->SetBranchAddress("s75", &s75);
	DJANGOHEventTree->SetBranchAddress("s67", &s67);
	DJANGOHEventTree->SetBranchAddress("beamCharge", &beamCharge);
	DJANGOHEventTree->SetBranchAddress("TrType", &TrType);
	DJANGOHEventTree->SetBranchAddress("TrackCheck", &TrackCheck);
	DJANGOHEventTree->SetBranchAddress("measuredCharge", &measuredCharge);
	DJANGOHEventTree->SetBranchAddress("epT", &epT);

	PYTHIAEventTree->SetBranchAddress("elecPhi", &elecPhi);
	PYTHIAEventTree->SetBranchAddress("elecTheta", &elecTheta);
	PYTHIAEventTree->SetBranchAddress("x", &x);
	PYTHIAEventTree->SetBranchAddress("Q2s", &Q2s);
	PYTHIAEventTree->SetBranchAddress("Ys", &Ys);
	PYTHIAEventTree->SetBranchAddress("Q2sGen", &Q2sGen);
	PYTHIAEventTree->SetBranchAddress("YsGen", &YsGen);
	PYTHIAEventTree->SetBranchAddress("Epz", &Epz);
	PYTHIAEventTree->SetBranchAddress("elecE", &elecE);
	PYTHIAEventTree->SetBranchAddress("RTheta", &RTheta);
	PYTHIAEventTree->SetBranchAddress("ClusterRadius", &ClusterRadius);
	PYTHIAEventTree->SetBranchAddress("Zvertex", &Zvertex);
	PYTHIAEventTree->SetBranchAddress("ProtonEnergy", &ProtonEnergy);
	PYTHIAEventTree->SetBranchAddress("ElectronEnergy", &ElectronEnergy);
	PYTHIAEventTree->SetBranchAddress("Xcluster", &Xcluster);
	PYTHIAEventTree->SetBranchAddress("Ycluster", &Ycluster);
	PYTHIAEventTree->SetBranchAddress("Zcluster", &Zcluster);
	PYTHIAEventTree->SetBranchAddress("RunNumber", &RunNumber);
	PYTHIAEventTree->SetBranchAddress("RunLumi", &RunLumi);
	//PYTHIAEventTree->SetBranchAddress("fRunType", &fRunType);
	PYTHIAEventTree->SetBranchAddress("fMyRunYear", &fMyRunYear);
	PYTHIAEventTree->SetBranchAddress("fposEX", &fposEX);
	PYTHIAEventTree->SetBranchAddress("fposEY", &fposEY);
	PYTHIAEventTree->SetBranchAddress("weight1", &weight1);
	PYTHIAEventTree->SetBranchAddress("weight2", &weight2);
	PYTHIAEventTree->SetBranchAddress("s61", &s61);
	PYTHIAEventTree->SetBranchAddress("s1", &s1);
	PYTHIAEventTree->SetBranchAddress("s74", &s74);
	PYTHIAEventTree->SetBranchAddress("s75", &s75);
	PYTHIAEventTree->SetBranchAddress("s67", &s67);
	PYTHIAEventTree->SetBranchAddress("beamCharge", &beamCharge);
	PYTHIAEventTree->SetBranchAddress("TrType", &TrType);
	PYTHIAEventTree->SetBranchAddress("TrackCheck", &TrackCheck);
	PYTHIAEventTree->SetBranchAddress("measuredCharge", &measuredCharge);
	PYTHIAEventTree->SetBranchAddress("epT", &epT);
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
	TH1F* PYTHIAHist = new TH1F("PYTHIA", "PYTHIA", bins, lowerbinrange, upperbinrange);

	TH1F* DataRatioHist = new TH1F();
	TH1F* RAPGAPRatioHist = new TH1F();
	TH1F* DJANGOHRatioHist = new TH1F();
	TH1F* PYTHIARatioHist = new TH1F();

	//Setting Histogram Colors
	DataHist->SetLineColor(kBlack);
	RAPGAPHist->SetLineColor(kRed);
	DJANGOHHist->SetLineColor(kBlue);
	PYTHIAHist->SetLineColor(kGreen);
	DataRatioHist->SetLineColor(kBlack);
	RAPGAPRatioHist->SetLineColor(kRed);
	DJANGOHRatioHist->SetLineColor(kBlue);
	PYTHIARatioHist->SetLineColor(kGreen);

	DataHist->SetLineWidth(2);
	DataHist->SetMarkerSize(3);
	RAPGAPHist->SetLineWidth(2);
	DJANGOHHist->SetLineWidth(2);
	PYTHIAHist->SetLineWidth(2);

	//Setting Stats to Not True
	DataHist->SetStats(0);
	RAPGAPHist->SetStats(0);
	DJANGOHHist->SetStats(0);
	PYTHIAHist->SetStats(0);

	
	
	//Loop over Data Tree Entries
	int m;
	int entries;//get number of entries in the Event Tree we just loaded in
	entries = DataEventTree->GetEntries();
	double numdataevents = 0;
	for (m = 0; m < entries; m++) {
		DataEventTree->GetEntry(m);
		if (Q2s > 5 && Q2s < 100 && Ys>0.0375 && Ys < 0.6) {
			if (s61 > 0) {
				if (variable == 0) {
					//DataEventTree->GetEntry(m);
					DataHist->Fill(Q2s, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
				if (variable == 1) {
					//DataEventTree->GetEntry(m);
					DataHist->Fill(Ys, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
				if (variable == 2) {
					//DataEventTree->GetEntry(m);
					DataHist->Fill(Zvertex, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
				if (variable == 3) {
					//DataEventTree->GetEntry(m);
					DataHist->Fill(Epz, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
				if (variable == 4) {
					//DataEventTree->GetEntry(m);
					DataHist->Fill(elecE, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
				if (variable == 5) {
					//DataEventTree->GetEntry(m);
					DataHist->Fill(elecPhi, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
				if (variable == 6) {
					//DataEventTree->GetEntry(m);
					DataHist->Fill(elecTheta, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
				if (variable == 7) {
					//DataEventTree->GetEntry(m);
					DataHist->Fill(ClusterRadius, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
				if (variable == 8 && TrackCheck > 0) {
					//DataEventTree->GetEntry(m);
					DataHist->Fill(1 / (epT * (measuredCharge) * (beamCharge)), weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
				if (variable == 9) {
					DataHist->Fill(ProtonEnergy, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
				if (variable == 10) {
					DataHist->Fill(ElectronEnergy, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
				if (variable == 11) {
					DataHist->Fill(Xcluster, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
				if (variable == 12) {
					DataHist->Fill(Ycluster, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
				if (variable == 13) {
					DataHist->Fill(Zcluster, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
				if (variable == 14) {
					DataHist->Fill(x, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
		}
	}

	//Loop over RAPGAP Tree Entries
	int n;
	entries = RAPGAPEventTree->GetEntries();
	double numRAPGAPevents = 0;
	for (n = 0; n < entries; n++) {
		RAPGAPEventTree->GetEntry(n);
		if (s61 > 0) {
			if (Q2s > 5 && Q2s < 100 && Ys>0.0375 && Ys < 0.6) {
				if (variable == 0) {
					//RAPGAPEventTree->GetEntry(n);
					RAPGAPHist->Fill(Q2s, weight1 * weight2);
					numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
				}
				if (variable == 1) {
					//RAPGAPEventTree->GetEntry(n);
					RAPGAPHist->Fill(Ys, weight1 * weight2);
					numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
				}
				if (variable == 2) {
					//RAPGAPEventTree->GetEntry(n);
					RAPGAPHist->Fill(Zvertex, weight1 * weight2);
					numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
				}
				if (variable == 3) {
					//RAPGAPEventTree->GetEntry(n);
					RAPGAPHist->Fill(Epz, weight1 * weight2);
					numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
				}
				if (variable == 4) {
					//RAPGAPEventTree->GetEntry(n);
					RAPGAPHist->Fill(elecE, weight1 * weight2);
					numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
				}
				if (variable == 5) {
					//RAPGAPEventTree->GetEntry(n);
					RAPGAPHist->Fill(elecPhi, weight1 * weight2);
					numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
				}
				if (variable == 6) {
					//RAPGAPEventTree->GetEntry(n);
					RAPGAPHist->Fill(elecTheta, weight1 * weight2);
					numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
				}
				if (variable == 7) {
					//RAPGAPEventTree->GetEntry(n);
					RAPGAPHist->Fill(ClusterRadius, weight1 * weight2);
					numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
				}
				if (variable == 8 && TrackCheck > 0) {
					//RAPGAPEventTree->GetEntry(m);
					RAPGAPHist->Fill(1 / (epT * (measuredCharge) * (beamCharge)), weight1 * weight2);
					numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
				}
				if (variable == 9) {
					RAPGAPHist->Fill(ProtonEnergy, weight1 * weight2);
					numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
				}
				if (variable == 10) {
					RAPGAPHist->Fill(ElectronEnergy, weight1 * weight2);
					numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
				}
				if (variable == 11) {
					RAPGAPHist->Fill(Xcluster, weight1 * weight2);
					numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
				}
				if (variable == 12) {
					RAPGAPHist->Fill(Ycluster, weight1 * weight2);
					numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
				}
				if (variable == 13) {
					RAPGAPHist->Fill(Zcluster, weight1 * weight2);
					numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
				}
				if (variable == 14) {
					RAPGAPHist->Fill(x, weight1 * weight2);
					numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
				}
			}
		}
	}

	//Loop over DJANGOH Tree Entries
	int p;
	entries = DJANGOHEventTree->GetEntries();
	double numDJANGOHevents = 0;
	for (p = 0; p < entries; p++) {
		DJANGOHEventTree->GetEntry(p);
		if (s61 > 0) {
			if (Q2s > 5 && Q2s < 100 && Ys>0.0375 && Ys < 0.6) {
				if (variable == 0) {
					//DJANGOHEventTree->GetEntry(p);
					DJANGOHHist->Fill(Q2s, weight1 * weight2);
					numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
				}
				if (variable == 1) {
					//DJANGOHEventTree->GetEntry(p);
					DJANGOHHist->Fill(Ys, weight1 * weight2);
					numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
				}
				if (variable == 2) {
					//DJANGOHEventTree->GetEntry(p);
					DJANGOHHist->Fill(Zvertex, weight1 * weight2);
					numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
				}
				if (variable == 3) {
					//DJANGOHEventTree->GetEntry(p);
					DJANGOHHist->Fill(Epz, weight1 * weight2);
					numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
				}
				if (variable == 4) {
					//DJANGOHEventTree->GetEntry(p);
					DJANGOHHist->Fill(elecE, weight1 * weight2);
					numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
				}
				if (variable == 5) {
					//DJANGOHEventTree->GetEntry(p);
					DJANGOHHist->Fill(elecPhi, weight1 * weight2);
					numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
				}
				if (variable == 6) {
					//DJANGOHEventTree->GetEntry(p);
					DJANGOHHist->Fill(elecTheta, weight1 * weight2);
					numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
				}
				if (variable == 7) {
					//DJANGOHEventTree->GetEntry(p);
					DJANGOHHist->Fill(ClusterRadius, weight1 * weight2);
					numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
				}
				if (variable == 8 && TrackCheck > 0) {
					//DJANGOHEventTree->GetEntry(m);
					DJANGOHHist->Fill(1 / (epT * (measuredCharge) * (beamCharge)), weight1 * weight2);
					numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
				}
				if (variable == 9) {
					DJANGOHHist->Fill(ProtonEnergy, weight1 * weight2);
					numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
				}
				if (variable == 10) {
					DJANGOHHist->Fill(ElectronEnergy, weight1 * weight2);
					numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
				}
				if (variable == 11) {
					DJANGOHHist->Fill(Xcluster, weight1 * weight2);
					numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
				}
				if (variable == 12) {
					DJANGOHHist->Fill(Ycluster, weight1 * weight2);
					numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
				}
				if (variable == 13) {
					DJANGOHHist->Fill(Zcluster, weight1 * weight2);
					numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
				}
				if (variable == 14) {
					DJANGOHHist->Fill(x, weight1 * weight2);
					numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
				}
			}
		}
	}

	//Loop over PYTHIA Tree Entries
	int k;
	entries = PYTHIAEventTree->GetEntries();
	double numPYTHIAevents = 0;
	for (k = 0; k < entries; k++) {
		PYTHIAEventTree->GetEntry(k);
		if (s61 > 0) {
			if (Q2s > 5 && Q2s < 100 && Ys>0.0375 && Ys < 0.6) {
				if (variable == 0) {
					//PYTHIAEventTree->GetEntry(k);
					PYTHIAHist->Fill(Q2s, weight1 * weight2);
					numPYTHIAevents = numPYTHIAevents + 1 * weight1 * weight2;
				}
				if (variable == 1) {
					//PYTHIAEventTree->GetEntry(k);
					PYTHIAHist->Fill(Ys, weight1 * weight2);
					numPYTHIAevents = numPYTHIAevents + 1 * weight1 * weight2;
				}
				if (variable == 2) {
					//PYTHIAEventTree->GetEntry(k);
					PYTHIAHist->Fill(Zvertex, weight1 * weight2);
					numPYTHIAevents = numPYTHIAevents + 1 * weight1 * weight2;
				}
				if (variable == 3) {
					//PYTHIAEventTree->GetEntry(k);
					PYTHIAHist->Fill(Epz, weight1 * weight2);
					numPYTHIAevents = numPYTHIAevents + 1 * weight1 * weight2;
				}
				if (variable == 4) {
					//PYTHIAEventTree->GetEntry(k);
					PYTHIAHist->Fill(elecE, weight1 * weight2);
					numPYTHIAevents = numPYTHIAevents + 1 * weight1 * weight2;
				}
				if (variable == 5) {
					//PYTHIAEventTree->GetEntry(k);
					PYTHIAHist->Fill(elecPhi, weight1 * weight2);
					numPYTHIAevents = numPYTHIAevents + 1 * weight1 * weight2;
				}
				if (variable == 6) {
					//PYTHIAEventTree->GetEntry(k);
					PYTHIAHist->Fill(elecTheta, weight1 * weight2);
					numPYTHIAevents = numPYTHIAevents + 1 * weight1 * weight2;
				}
				if (variable == 7) {
					//PYTHIAEventTree->GetEntry(k);
					PYTHIAHist->Fill(ClusterRadius, weight1 * weight2);
					numPYTHIAevents = numPYTHIAevents + 1 * weight1 * weight2;
				}
				if (variable == 8 && TrackCheck > 0) {
					//PYTHIAEventTree->GetEntry(k);
					PYTHIAHist->Fill(1 / (epT * (measuredCharge) * (beamCharge)), weight1 * weight2);
					numPYTHIAevents = numPYTHIAevents + 1 * weight1 * weight2;
				}
				if (variable == 9) {
					PYTHIAHist->Fill(ProtonEnergy, weight1 * weight2);
					numPYTHIAevents = numPYTHIAevents + 1 * weight1 * weight2;
				}
				if (variable == 10) {
					PYTHIAHist->Fill(ElectronEnergy, weight1 * weight2);
					numPYTHIAevents = numPYTHIAevents + 1 * weight1 * weight2;
				}
				if (variable == 11) {
					PYTHIAHist->Fill(Xcluster, weight1 * weight2);
					numPYTHIAevents = numPYTHIAevents + 1 * weight1 * weight2;
				}
				if (variable == 12) {
					PYTHIAHist->Fill(Ycluster, weight1 * weight2);
					numPYTHIAevents = numPYTHIAevents + 1 * weight1 * weight2;
				}
				if (variable == 13) {
					PYTHIAHist->Fill(Zcluster, weight1 * weight2);
					numPYTHIAevents = numPYTHIAevents + 1 * weight1 * weight2;
				}
				if (variable == 14) {
					PYTHIAHist->Fill(x, weight1 * weight2);
					numPYTHIAevents = numPYTHIAevents + 1 * weight1 * weight2;
				}
			}
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
	DataHist->GetYaxis()->SetTitle(yaxis.c_str());

	TAxis* axis = DataHist->GetYaxis();
	axis->ChangeLabel(1, -1, -1, -1, -1, -1, " ");
	
	DataHist->GetYaxis()->SetTitleFont(43);
	DataHist->GetYaxis()->SetTitleSize(20);
	DataHist->GetYaxis()->SetLabelFont(43);
	DataHist->GetYaxis()->SetLabelSize(15);
	DataHist->GetYaxis()->SetTitleOffset(1.55);
	//DataHist->GetYaxis()->SetTitleSize(0.35);
	DataHist->Scale(1 / numdataevents);
	RAPGAPHist->Scale(1 / numRAPGAPevents);
	DJANGOHHist->Scale(1 / numDJANGOHevents);
	PYTHIAHist->Scale(1 / numPYTHIAevents);
	//DataHist->Scale(1);
	//RAPGAPHist->Scale(numdataevents / numRAPGAPevents);
	//DJANGOHHist->Scale(numdataevents / numDJANGOHevents);
	//PYTHIAHist->Scale(numdataevents / numPYTHIAevents);
	//DataHist->Scale(1 / DataLumi);
	//RAPGAPHist->Scale(1 / RAPGAPLumi);
	//DJANGOHHist->Scale(1 / DJANGOHLumi);
	
	//DataHist->Scale(1);
	//RAPGAPHist->Scale(DataLumi / RAPGAPLumi);
	//DJANGOHHist->Scale(DataLumi / DJANGOHLumi);
	//PYTHIAHist->Scale(DataLumi / PYTHIALumi);
	
	DataHist->Draw("e");
	RAPGAPHist->Draw("histsame");
	DJANGOHHist->Draw("histsame");
	//PYTHIAHist->Draw("histsame");
	//DataHist->GetXaxis()->SetLabelOffset(999);
	DataHist->GetXaxis()->SetLabelSize(0);

	TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	//legend->SetHeader(Form("#splitline{Count- %.2f #pm %.2f (stat.)}{Stochastic MLP- %s Network Trained %ix}", kcount, kcounterror, network[particlecode].c_str(), trainnum[particlecode])); // option "C" allows to center the header
	legend->AddEntry(DataHist, "2005-2007 H1 Data", "l");
	legend->AddEntry(RAPGAPHist, "RAPGAP", "l");
	legend->AddEntry(DJANGOHHist, "DJANGOH", "l");
	//legend->AddEntry(PYTHIAHist, "Normalized PYTHIA", "l");

	//legend->AddEntry(DataHist, "2005-2007 Data", "l");
	//legend->AddEntry(RAPGAPHist, "RAPGAP", "l");
	//legend->AddEntry(DJANGOHHist, "DJANGOH", "l");
	//legend->AddEntry(PYTHIAHist, "PYTHIA", "l");
	legend->Draw("same");
	
	//Ratio plot will be in the lower pad
	// Go back to the main canvas before defining pad2
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
	*PYTHIARatioHist = (*PYTHIAHist) / (*DataHist);
	DataRatioHist->SetTitle("");
	DataRatioHist->GetXaxis()->SetTitle(xtitle.c_str());
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

	DataRatioHist->SetLineWidth(1);

	TAxis* axis2 = DataRatioHist->GetYaxis();
	axis->ChangeLabel(1, -1, -1, -1, -1, -1, " ");

	DataRatioHist->Draw("Hist][");
	RAPGAPRatioHist->Draw("same");
	DJANGOHRatioHist->Draw("same");
	//PYTHIARatioHist->Draw("same");

	canvas->Write();
	//legend->DeleteEntry();
	//legend->DeleteEntry();
	Data->Close();
	RAPGAP->Close();
	DJANGOH->Close();
	PYTHIA->Close();

	return 0;

};

void DISControlPlots() {
	//create file to output plots to
	TFile* outputfile = new TFile(outputfilename.c_str(), "RECREATE");
	//TFile* f = new TFile(dummyfile.c_str(), "RECREATE");

	//Plot(outputfile, 5, 100, 95, 0,"Q2 Distributions","Q^{2} Distributions","Q^{2} [GeV^{2}]", "1/N dN/dQ^{2} [GeV^{-2}]");
	//cout << "Q2 done" << endl;
	//Plot(outputfile, 0.0375, 0.6, 100, 1, "y Distributions", "y Distributions", "y", "1/N dN/dy");
	//cout << "Y done" << endl;
	//Plot(outputfile, -30, 30, 100, 2, "Z Vtx Distributions", "Z_{Vtx} Distributions", "Z_{vtx} [cm]", "1/N dN/dZ_{vtx} [cm^{-1}]");
	//cout << "Z Vtx done" << endl;
	//Plot(outputfile, 35, 70, 100, 3, "Epz Distributions", "Epz Distributions", "E-p_{z} [GeV]", "1/N dN/d(E-p_{z}) [GeV^{-1}]");
	//cout << "Epz done" << endl;
	//Plot(outputfile, 11, 32, 100, 4, "Ee Distributions", "E_{e} Distributions", "E^{'}_{e} [GeV]", "1/N dN/dE^{'}_{e} [GeV^{-1}]");
	//cout << "Ee done" << endl;
	//Plot(outputfile, -3.14, 3.14, 50, 5, "Phi Distributions", "#phi Distributions", "#phi_{e} [rad]", "1/N dN/d#phi_{e} [rad^{-1}]");
	//cout << "Phi done" << endl;
	//Plot(outputfile, 2.63, 3.08, 50, 6, "Theta Distributions", "#theta Distributions", "#theta_{e} [rad]", "1/N dN/d#theta_{e} [rad^{-1}]");
	//cout << "Theta done" << endl;
	//Plot(outputfile, 2, 3.52, 50, 7, "Ecra Distributions", "Ecra Distributions", "R_{cluster} [cm]", "1/N dN/dR_{cluster} [cm^{-1}]");
	//cout << "Ecra done" << endl;

	//Plot(outputfile, -70, 70, 140, 11, "Xcluster Distributions", "Xcluster Distributions", "X_{SpaCal} [cm]", "1/N dN/dX_{SpaCal} [cm^{-1}]");
	//cout << "Xcluster done" << endl;
	//Plot(outputfile, -70, 70, 140, 12, "Ycluster Distributions", "Ycluster Distributions", "Y_{SpaCal} [cm]", "1/N dN/dY_{SpaCal} [cm^{-1}]");
	//cout << "Xcluster done" << endl;
	//Plot(outputfile, -163, -160, 100, 13, "Zcluster Distributions", "Zcluster Distributions", "Z_{SpaCal} [cm]", "1/N dN/dZ_{SpaCal} [cm^{-1}]");
	//cout << "Xcluster done" << endl;
	
	//Plot(outputfile, -1, 1, 100, 8, "Scattered Lepton", "Scattered Lepton", "1/(p_{T} * Q_{Measured} * Q_{beam})");
	//cout << "1/pT*Q done" << endl;
	//Plot(outputfile, 919, 921, 500, 9, "Proton Energy", "Proton Energy", "E_{proton beam} [GeV]");
	//cout << "Proton Energy done" << endl;
	//Plot(outputfile, 27, 28, 500, 10, "Electron Energy", "Electron Energy", "E_{electron beam} [GeV]");
	//cout << "Electron Energy done" << endl;

	Plot(outputfile, 0.0, 0.1, 100, 14, "Bjorken x Distributions", "Bjorken x", "x", "1/N dN/dx");
	cout << "Bjorken x done" << endl;

	outputfile->cd();
	outputfile->Write();
	outputfile->Close();

	cout << "Done" << endl;
}