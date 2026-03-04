//Analysis Code Root Macro 
//Importing TTrees
#include <iostream>
#include <stdlib.h>
#include <cmath>
using namespace std;



string fileDataTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/EventLists/merged.root";
string fileRAPGAPTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/EventLists/RAPGAP06/merged.root";
string fileDJANGOHTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/EventLists/DJANGOH06/merged.root";

string outputfilename = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/ControlPlots.root";



void ControlPlots() {
	
	//Creating Canvas and pads
	TCanvas* canvas = new TCanvas("c1", "multipads", 800, 800);
	TPad* pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
	// Histograms Binning, have some histograms filled directly from analysis that I use as a consistency check with the histograms I make from my Analysis code using the TTrees I create here
	Double_t bins = 100;
	Double_t lowerbinrange = 0.1;
	Double_t upperbinrange = 0.6;

	
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

	//Getting Data File
	TFile* Data = new TFile(fileDataTree.c_str(), "read");
	TFile* RAPGAP = new TFile(fileRAPGAPTree.c_str(), "read");
	TFile* DJANGOH = new TFile(fileDJANGOHTree.c_str(), "read");
	//Getting Event Tree from the Data file
	TTree* DataEventTree = (TTree*)Data->Get("Data Event Tree");
	TTree* RAPGAPEventTree = (TTree*)RAPGAP->Get("Data Event Tree");
	TTree* DJANGOHEventTree = (TTree*)DJANGOH->Get("Data Event Tree");

	//Define the variables and banch addresses for data we want from the Event Tree entries
	double weight1, weight2, elecPhi, elecTheta, Epz, elecE, x, Q2s, Ys, Q2sGen, YsGen, RTheta, ClusterRadius, Zvertex, ProtonEnergy, ElectronEnergy, Xcluster, Ycluster, Zcluster, RunNumber, RunLumi, fposEX, fposEY, fRunType; //, RCluster,;
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
	DataEventTree->SetBranchAddress("fRunType", &fRunType);
	DataEventTree->SetBranchAddress("fMyRunYear", &fMyRunYear);
	DataEventTree->SetBranchAddress("fposEX", &fposEX);
	DataEventTree->SetBranchAddress("fposEY", &fposEY);
	DataEventTree->SetBranchAddress("weight1", &weight1);
	DataEventTree->SetBranchAddress("weight2", &weight2);
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
	RAPGAPEventTree->SetBranchAddress("fRunType", &fRunType);
	RAPGAPEventTree->SetBranchAddress("fMyRunYear", &fMyRunYear);
	RAPGAPEventTree->SetBranchAddress("fposEX", &fposEX);
	RAPGAPEventTree->SetBranchAddress("fposEY", &fposEY);
	RAPGAPEventTree->SetBranchAddress("weight1", &weight1);
	RAPGAPEventTree->SetBranchAddress("weight2", &weight2);

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
	DJANGOHEventTree->SetBranchAddress("fRunType", &fRunType);
	DJANGOHEventTree->SetBranchAddress("fMyRunYear", &fMyRunYear);
	DJANGOHEventTree->SetBranchAddress("fposEX", &fposEX);
	DJANGOHEventTree->SetBranchAddress("fposEY", &fposEY);
	DJANGOHEventTree->SetBranchAddress("weight1", &weight1);
	DJANGOHEventTree->SetBranchAddress("weight2", &weight2);
	
	//Loop over Data Tree Entries
	int m;
	int entries;//get number of entries in the Event Tree we just loaded in
	entries = DataEventTree->GetEntries();
	double numdataevents = 0;
	for (m = 0; m < entries; m++) {

		DataEventTree->GetEntry(m);
		DataHist->Fill(Ys, weight1 * weight2);
		numdataevents= numdataevents+1* weight1 * weight2;
		//LambdaRegHist->Fill(invMasslambda, weight1 * weight2);
	}

	//Loop over RAPGAP Tree Entries
	int n;
	entries = RAPGAPEventTree->GetEntries();
	double numRAPGAPevents = 0;
	for (n = 0; n < entries; n++) {

		RAPGAPEventTree->GetEntry(n);
		RAPGAPHist->Fill(Ys, weight1 * weight2);
		numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
	}

	//Loop over DJANGOH Tree Entries
	int p;
	entries = DJANGOHEventTree->GetEntries();
	double numDJANGOHevents = 0;
	for (p = 0; p < entries; p++) {

		DJANGOHEventTree->GetEntry(p);
		DJANGOHHist->Fill(Ys, weight1 * weight2);
		numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
	}

	//create file to output plots to
	TFile outputfile(outputfilename.c_str(), "RECREATE");
	canvas->SetName("Q2 Distributions");
	DataHist->SetTitle("Q^{2} Distributions");
	//DataHist->GetXaxis()->SetTitle("Q^{2}");
	//change to pad1 before drawing
	canvas->cd();
	pad1->SetBottomMargin(0.01);
	//pad1->SetGridx();
	pad1->Draw();
	pad1->cd();
	DataHist->GetYaxis()->SetTitle("Entries (Normalized)");

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
	DataHist->Draw("hist");
	RAPGAPHist->Draw("esame");
	DJANGOHHist->Draw("esame");
	//DataHist->GetXaxis()->SetLabelOffset(999);
	DataHist->GetXaxis()->SetLabelSize(0);

	TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	//legend->SetHeader(Form("#splitline{Count- %.2f #pm %.2f (stat.)}{Stochastic MLP- %s Network Trained %ix}", kcount, kcounterror, network[particlecode].c_str(), trainnum[particlecode])); // option "C" allows to center the header
	legend->AddEntry(DataHist, "2006 Data", "l");
	legend->AddEntry(RAPGAPHist, "RAPGAP", "l");
	legend->AddEntry(DJANGOHHist, "DJANGOH", "l");
	legend->Draw("same");
	
	//Ratio plot will be in the lower pad
	// Go back to the main canvas before defining pad2
	TPad* pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
	canvas->cd();
	pad2->SetTopMargin(0.03);
	pad2->SetBottomMargin(0.2);
	//pad2->SetGridx();
	pad2->Draw();
	pad2->cd();
	*DataRatioHist = (*DataHist) / (*DataHist);
	*RAPGAPRatioHist = (*RAPGAPHist) / (*DataHist);
	*DJANGOHRatioHist = (*DJANGOHHist) / (*DataHist);
	DataRatioHist->SetTitle("");
	DataRatioHist->GetXaxis()->SetTitle("Q^{2}");
	DataRatioHist->GetYaxis()->SetTitle("Rec MC / Data");
	//DataRatioHist->GetYaxis()->SetTitleSize(0.35);
	//DataRatioHist->GetXaxis()->SetTitleSize(0.35);
	DataRatioHist->GetYaxis()->SetRangeUser(0.9, 1.3);
	DataRatioHist->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	DataRatioHist->GetXaxis()->SetLabelSize(15);
	DataRatioHist->GetYaxis()->SetLabelFont(43);
	DataRatioHist->GetYaxis()->SetLabelSize(15);
	DataRatioHist->GetXaxis()->SetTitleFont(43);
	DataRatioHist->GetXaxis()->SetTitleSize(20);
	DataRatioHist->GetXaxis()->SetTitleOffset(1.55);

	TAxis* axis2 = DataRatioHist->GetYaxis();
	axis->ChangeLabel(1, -1, -1, -1, -1, -1, " ");

	DataRatioHist->Draw("Hist][");
	RAPGAPRatioHist->Draw("same");
	DJANGOHRatioHist->Draw("same");

	canvas->Write();
	//legend->DeleteEntry();
	//legend->DeleteEntry();

	outputfile.Write();
	outputfile.Close();
	Data->Close();
	RAPGAP->Close();
	DJANGOH->Close();

	cout << "Done" << endl;

};

//void ControlPlots() {
//
//
//
//}