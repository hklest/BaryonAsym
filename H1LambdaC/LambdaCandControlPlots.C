//Analysis Code Root Macro 
//Importing TTrees
#include <iostream>
#include <stdlib.h>
#include <cmath>
using namespace std;



//Change these file paths and output for different years
string fileDataTree = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/LambdaTree/CutsApplied/merged.root";
string fileRAPGAPTree = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/CutsApplied/merged.root";
string fileDJANGOHTree = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/CutsApplied/merged.root";

string outputfilename = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/LambdaCandControlPlotsTest.root";

//double DataLumi = 297639.1; //nb-1
//double RAPGAPLumi = 5293265.0; //nb-1
//double DJANGOHLumi = 4108957.0; //nb-1
double DataLumi = 297639.1; //nb-1
double RAPGAPLumi = 5293265.0; //nb-1
double DJANGOHLumi = 3878866.0; //nb-1

// Get the pion's mass, in GeV
Double_t mPion = TDatabasePDG::Instance()->GetParticle(211)->Mass();
// Get proton mass, in GeV
Double_t mProton = TDatabasePDG::Instance()->GetParticle(2212)->Mass();

//Proton Momentum Bins - used for just a few of the control plots
double lowerprotonmomentumbin = 0;
double upperprotonmomentumbin = 0;


//For Signal Range
//double invmasslowerbound1 = 1.104;
//double invmassupperbound1 = 1.126;
//double invmasslowerbound2 = 100;
//double invmassupperbound2 = 100;
//string legendheader = "Signal Range Normalized Distributions";

//For All
//double invmasslowerbound1 = 0;
//double invmassupperbound1 = 100;
//double invmasslowerbound2 = 100;
//double invmassupperbound2 = 100;
//string legendheader = "Normalized Distributions";

//For Sidebands
double invmasslowerbound1 = 0;
double invmassupperbound1 = 1.104;
double invmasslowerbound2 = 1.126;
double invmassupperbound2 = 100;
string legendheader = "Sideband Normalized Distributions";

int Plot(TFile* output,double lowerbinrange, double upperbinrange, double bins, int variable, string treebranch, string canvasname, string plottitle, string xtitle, string ytitle) {
	//Getting Data File
	TFile* Data = new TFile(fileDataTree.c_str(), "read");
	TFile* RAPGAP = new TFile(fileRAPGAPTree.c_str(), "read");
	TFile* DJANGOH = new TFile(fileDJANGOHTree.c_str(), "read");
	//Getting Event Tree from the Data file
	TTree* DataEventTree = (TTree*)Data->Get(treebranch.c_str());
	TTree* RAPGAPEventTree = (TTree*)RAPGAP->Get(treebranch.c_str());
	TTree* DJANGOHEventTree = (TTree*)DJANGOH->Get(treebranch.c_str());

	//defining the variables in Trees
	double pT, Eta, x, invMasslambda, Q2s, Ys, Angle, v0chi2, weight1, weight2, SVx, SVy, SVz, PVx, PVy, PVz, LambdaPX, LambdaPY, LambdaPZ, ystar, ylab, scatEX, scatEY, scatEZ;
	double PionPt, ProtonPt, PionTrackStartRadius, ProtonTrackStartRadius, PionL, ProtonL, PionCharge, ProtonCharge, PionTrackLength, ProtonTrackLength;
	double PionDCA, ProtonDCA, PionSDCA, ProtonSDCA, Chi2, MassPionPion, MassElecElec, pTrelPion, pTrelProton, RadiusSVtoPV;
	double genIvxX, genIvxY, genIvxZ, decayrecX, decayrecY, decayrecZ, strIVtype, pionFRUNZ, protonFRUNZ;
	int fRunType, IsBar;
	double PionNuclCorrWeight;
	double ProtonTrackEndRadius, PionTrackEndRadius;
	double PionLProton, PionLelectron, ProtonLPion, ProtonLelectron;
	double CosThetaStar;
	//TVector3  ProtonTrackEndMomentum, ProtonTrackV0Momentum, PionTrackEndMomentum, PionTrackV0Momentum;
	TVector3* PionTrackV0Momentum = nullptr;
	TVector3* ProtonTrackV0Momentum = nullptr;
	TVector3* PionTrackEndMomentum = nullptr;
	TVector3* ProtonTrackEndMomentum = nullptr;
	//Defining Tree Branches
	DataEventTree->SetBranchAddress("invMasslambda", &invMasslambda);
	DataEventTree->SetBranchAddress("pT", &pT);
	DataEventTree->SetBranchAddress("Eta", &Eta);
	DataEventTree->SetBranchAddress("x", &x);
	DataEventTree->SetBranchAddress("Q2s", &Q2s);
	DataEventTree->SetBranchAddress("Ys", &Ys);
	DataEventTree->SetBranchAddress("Angle", &Angle);
	DataEventTree->SetBranchAddress("v0chi2", &v0chi2);
	DataEventTree->SetBranchAddress("weight1", &weight1);
	DataEventTree->SetBranchAddress("weight2", &weight2);
	DataEventTree->SetBranchAddress("SVx", &SVx);
	DataEventTree->SetBranchAddress("SVy", &SVy);
	DataEventTree->SetBranchAddress("SVz", &SVz);
	DataEventTree->SetBranchAddress("PVx", &PVx);
	DataEventTree->SetBranchAddress("PVy", &PVy);
	DataEventTree->SetBranchAddress("PVz", &PVz);
	DataEventTree->SetBranchAddress("LambdaPX", &LambdaPX);
	DataEventTree->SetBranchAddress("LambdaPY", &LambdaPY);
	DataEventTree->SetBranchAddress("LambdaPZ", &LambdaPZ);
	DataEventTree->SetBranchAddress("fRunType", &fRunType);
	DataEventTree->SetBranchAddress("ystar", &ystar);
	DataEventTree->SetBranchAddress("ylab", &ylab);
	DataEventTree->SetBranchAddress("scatEX", &scatEX);
	DataEventTree->SetBranchAddress("scatEY", &scatEY);
	DataEventTree->SetBranchAddress("scatEZ", &scatEZ);
	//Below is unique braches to the DataEventTree compared to Reg and Bar Trees
	DataEventTree->SetBranchAddress("PionCharge", &PionCharge);
	DataEventTree->SetBranchAddress("ProtonCharge", &ProtonCharge);
	//Candidate Control Variables
	DataEventTree->SetBranchAddress("pionFRUNZ", &pionFRUNZ);
	DataEventTree->SetBranchAddress("protonFRUNZ", &protonFRUNZ);
	DataEventTree->SetBranchAddress("PionPt", &PionPt);
	DataEventTree->SetBranchAddress("ProtonPt", &ProtonPt);
	DataEventTree->SetBranchAddress("PionTrackStartRadius", &PionTrackStartRadius);
	DataEventTree->SetBranchAddress("ProtonTrackStartRadius", &ProtonTrackStartRadius);
	DataEventTree->SetBranchAddress("PionL", &PionL);
	DataEventTree->SetBranchAddress("ProtonL", &ProtonL);
	DataEventTree->SetBranchAddress("PionTrackLength", &PionTrackLength);
	DataEventTree->SetBranchAddress("ProtonTrackLength", &ProtonTrackLength);
	DataEventTree->SetBranchAddress("PionDCA", &PionDCA);
	DataEventTree->SetBranchAddress("ProtonDCA", &ProtonDCA);
	DataEventTree->SetBranchAddress("PionSDCA", &PionSDCA);
	DataEventTree->SetBranchAddress("ProtonSDCA", &ProtonSDCA);
	DataEventTree->SetBranchAddress("Chi2", &Chi2);
	//DataEventTree->SetBranchAddress("Chi2Proton", &Chi2Proton);
	DataEventTree->SetBranchAddress("MassPionPion", &MassPionPion);
	DataEventTree->SetBranchAddress("MassElecElec", &MassElecElec);
	DataEventTree->SetBranchAddress("pTrelPion", &pTrelPion);
	DataEventTree->SetBranchAddress("pTrelProton", &pTrelProton);
	DataEventTree->SetBranchAddress("RadiusSVtoPV", &RadiusSVtoPV);
	DataEventTree->SetBranchAddress("IsBar", &IsBar);
	DataEventTree->SetBranchAddress("PionNuclCorrWeight", &PionNuclCorrWeight);
	DataEventTree->SetBranchAddress("ProtonTrackEndRadius", &ProtonTrackEndRadius);
	DataEventTree->SetBranchAddress("ProtonTrackEndMomentum", &ProtonTrackEndMomentum);
	DataEventTree->SetBranchAddress("PionTrackEndRadius", &PionTrackEndRadius);
	DataEventTree->SetBranchAddress("PionTrackEndMomentum", &PionTrackEndMomentum);
	DataEventTree->SetBranchAddress("ProtonTrackV0Momentum", &ProtonTrackV0Momentum);
	DataEventTree->SetBranchAddress("PionTrackV0Momentum", &PionTrackV0Momentum);
	DataEventTree->SetBranchAddress("PionLProton", &PionLProton);
	DataEventTree->SetBranchAddress("PionLelectron", &PionLelectron);
	DataEventTree->SetBranchAddress("ProtonLPion", &ProtonLPion);
	DataEventTree->SetBranchAddress("ProtonLelectron", &ProtonLelectron);
	DataEventTree->SetBranchAddress("CosThetaStar", &CosThetaStar);



	RAPGAPEventTree->SetBranchAddress("invMasslambda", &invMasslambda);
	RAPGAPEventTree->SetBranchAddress("pT", &pT);
	RAPGAPEventTree->SetBranchAddress("Eta", &Eta);
	RAPGAPEventTree->SetBranchAddress("x", &x);
	RAPGAPEventTree->SetBranchAddress("Q2s", &Q2s);
	RAPGAPEventTree->SetBranchAddress("Ys", &Ys);
	RAPGAPEventTree->SetBranchAddress("Angle", &Angle);
	RAPGAPEventTree->SetBranchAddress("v0chi2", &v0chi2);
	RAPGAPEventTree->SetBranchAddress("weight1", &weight1);
	RAPGAPEventTree->SetBranchAddress("weight2", &weight2);
	RAPGAPEventTree->SetBranchAddress("SVx", &SVx);
	RAPGAPEventTree->SetBranchAddress("SVy", &SVy);
	RAPGAPEventTree->SetBranchAddress("SVz", &SVz);
	RAPGAPEventTree->SetBranchAddress("PVx", &PVx);
	RAPGAPEventTree->SetBranchAddress("PVy", &PVy);
	RAPGAPEventTree->SetBranchAddress("PVz", &PVz);
	RAPGAPEventTree->SetBranchAddress("LambdaPX", &LambdaPX);
	RAPGAPEventTree->SetBranchAddress("LambdaPY", &LambdaPY);
	RAPGAPEventTree->SetBranchAddress("LambdaPZ", &LambdaPZ);
	RAPGAPEventTree->SetBranchAddress("fRunType", &fRunType);
	RAPGAPEventTree->SetBranchAddress("ystar", &ystar);
	RAPGAPEventTree->SetBranchAddress("ylab", &ylab);
	RAPGAPEventTree->SetBranchAddress("scatEX", &scatEX);
	RAPGAPEventTree->SetBranchAddress("scatEY", &scatEY);
	RAPGAPEventTree->SetBranchAddress("scatEZ", &scatEZ);
	//Below is unique braches to the RAPGAPEventTree compared to Reg and Bar Trees
	RAPGAPEventTree->SetBranchAddress("PionCharge", &PionCharge);
	RAPGAPEventTree->SetBranchAddress("ProtonCharge", &ProtonCharge);
	//Candidate Control Variables
	RAPGAPEventTree->SetBranchAddress("pionFRUNZ", &pionFRUNZ);
	RAPGAPEventTree->SetBranchAddress("protonFRUNZ", &protonFRUNZ);
	RAPGAPEventTree->SetBranchAddress("PionPt", &PionPt);
	RAPGAPEventTree->SetBranchAddress("ProtonPt", &ProtonPt);
	RAPGAPEventTree->SetBranchAddress("PionTrackStartRadius", &PionTrackStartRadius);
	RAPGAPEventTree->SetBranchAddress("ProtonTrackStartRadius", &ProtonTrackStartRadius);
	RAPGAPEventTree->SetBranchAddress("PionL", &PionL);
	RAPGAPEventTree->SetBranchAddress("ProtonL", &ProtonL);
	RAPGAPEventTree->SetBranchAddress("PionTrackLength", &PionTrackLength);
	RAPGAPEventTree->SetBranchAddress("ProtonTrackLength", &ProtonTrackLength);
	RAPGAPEventTree->SetBranchAddress("PionDCA", &PionDCA);
	RAPGAPEventTree->SetBranchAddress("ProtonDCA", &ProtonDCA);
	RAPGAPEventTree->SetBranchAddress("PionSDCA", &PionSDCA);
	RAPGAPEventTree->SetBranchAddress("ProtonSDCA", &ProtonSDCA);
	RAPGAPEventTree->SetBranchAddress("Chi2", &Chi2);
	//RAPGAPEventTree->SetBranchAddress("Chi2Proton", &Chi2Proton);
	RAPGAPEventTree->SetBranchAddress("MassPionPion", &MassPionPion);
	RAPGAPEventTree->SetBranchAddress("MassElecElec", &MassElecElec);
	RAPGAPEventTree->SetBranchAddress("pTrelPion", &pTrelPion);
	RAPGAPEventTree->SetBranchAddress("pTrelProton", &pTrelProton);
	RAPGAPEventTree->SetBranchAddress("RadiusSVtoPV", &RadiusSVtoPV);
	RAPGAPEventTree->SetBranchAddress("IsBar", &IsBar);
	RAPGAPEventTree->SetBranchAddress("PionNuclCorrWeight", &PionNuclCorrWeight);
	RAPGAPEventTree->SetBranchAddress("ProtonTrackEndRadius", &ProtonTrackEndRadius);
	RAPGAPEventTree->SetBranchAddress("ProtonTrackEndMomentum", &ProtonTrackEndMomentum);
	RAPGAPEventTree->SetBranchAddress("PionTrackEndRadius", &PionTrackEndRadius);
	RAPGAPEventTree->SetBranchAddress("PionTrackEndMomentum", &PionTrackEndMomentum);
	RAPGAPEventTree->SetBranchAddress("ProtonTrackV0Momentum", &ProtonTrackV0Momentum);
	RAPGAPEventTree->SetBranchAddress("PionTrackV0Momentum", &PionTrackV0Momentum);
	RAPGAPEventTree->SetBranchAddress("PionLProton", &PionLProton);
	RAPGAPEventTree->SetBranchAddress("PionLelectron", &PionLelectron);
	RAPGAPEventTree->SetBranchAddress("ProtonLPion", &ProtonLPion);
	RAPGAPEventTree->SetBranchAddress("ProtonLelectron", &ProtonLelectron);
	RAPGAPEventTree->SetBranchAddress("CosThetaStar", &CosThetaStar);



	DJANGOHEventTree->SetBranchAddress("invMasslambda", &invMasslambda);
	DJANGOHEventTree->SetBranchAddress("pT", &pT);
	DJANGOHEventTree->SetBranchAddress("Eta", &Eta);
	DJANGOHEventTree->SetBranchAddress("x", &x);
	DJANGOHEventTree->SetBranchAddress("Q2s", &Q2s);
	DJANGOHEventTree->SetBranchAddress("Ys", &Ys);
	DJANGOHEventTree->SetBranchAddress("Angle", &Angle);
	DJANGOHEventTree->SetBranchAddress("v0chi2", &v0chi2);
	DJANGOHEventTree->SetBranchAddress("weight1", &weight1);
	DJANGOHEventTree->SetBranchAddress("weight2", &weight2);
	DJANGOHEventTree->SetBranchAddress("SVx", &SVx);
	DJANGOHEventTree->SetBranchAddress("SVy", &SVy);
	DJANGOHEventTree->SetBranchAddress("SVz", &SVz);
	DJANGOHEventTree->SetBranchAddress("PVx", &PVx);
	DJANGOHEventTree->SetBranchAddress("PVy", &PVy);
	DJANGOHEventTree->SetBranchAddress("PVz", &PVz);
	DJANGOHEventTree->SetBranchAddress("LambdaPX", &LambdaPX);
	DJANGOHEventTree->SetBranchAddress("LambdaPY", &LambdaPY);
	DJANGOHEventTree->SetBranchAddress("LambdaPZ", &LambdaPZ);
	DJANGOHEventTree->SetBranchAddress("fRunType", &fRunType);
	DJANGOHEventTree->SetBranchAddress("ystar", &ystar);
	DJANGOHEventTree->SetBranchAddress("ylab", &ylab);
	DJANGOHEventTree->SetBranchAddress("scatEX", &scatEX);
	DJANGOHEventTree->SetBranchAddress("scatEY", &scatEY);
	DJANGOHEventTree->SetBranchAddress("scatEZ", &scatEZ);
	//Below is unique braches to the DJANGOHEventTree compared to Reg and Bar Trees
	DJANGOHEventTree->SetBranchAddress("PionCharge", &PionCharge);
	DJANGOHEventTree->SetBranchAddress("ProtonCharge", &ProtonCharge);
	//Candidate Control Variables
	DJANGOHEventTree->SetBranchAddress("pionFRUNZ", &pionFRUNZ);
	DJANGOHEventTree->SetBranchAddress("protonFRUNZ", &protonFRUNZ);
	DJANGOHEventTree->SetBranchAddress("PionPt", &PionPt);
	DJANGOHEventTree->SetBranchAddress("ProtonPt", &ProtonPt);
	DJANGOHEventTree->SetBranchAddress("PionTrackStartRadius", &PionTrackStartRadius);
	DJANGOHEventTree->SetBranchAddress("ProtonTrackStartRadius", &ProtonTrackStartRadius);
	DJANGOHEventTree->SetBranchAddress("PionL", &PionL);
	DJANGOHEventTree->SetBranchAddress("ProtonL", &ProtonL);
	DJANGOHEventTree->SetBranchAddress("PionTrackLength", &PionTrackLength);
	DJANGOHEventTree->SetBranchAddress("ProtonTrackLength", &ProtonTrackLength);
	DJANGOHEventTree->SetBranchAddress("PionDCA", &PionDCA);
	DJANGOHEventTree->SetBranchAddress("ProtonDCA", &ProtonDCA);
	DJANGOHEventTree->SetBranchAddress("PionSDCA", &PionSDCA);
	DJANGOHEventTree->SetBranchAddress("ProtonSDCA", &ProtonSDCA);
	DJANGOHEventTree->SetBranchAddress("Chi2", &Chi2);
	//DJANGOHEventTree->SetBranchAddress("Chi2Proton", &Chi2Proton);
	DJANGOHEventTree->SetBranchAddress("MassPionPion", &MassPionPion);
	DJANGOHEventTree->SetBranchAddress("MassElecElec", &MassElecElec);
	DJANGOHEventTree->SetBranchAddress("pTrelPion", &pTrelPion);
	DJANGOHEventTree->SetBranchAddress("pTrelProton", &pTrelProton);
	DJANGOHEventTree->SetBranchAddress("RadiusSVtoPV", &RadiusSVtoPV);
	DJANGOHEventTree->SetBranchAddress("IsBar", &IsBar);
	DJANGOHEventTree->SetBranchAddress("PionNuclCorrWeight", &PionNuclCorrWeight);
	DJANGOHEventTree->SetBranchAddress("ProtonTrackEndRadius", &ProtonTrackEndRadius);
	DJANGOHEventTree->SetBranchAddress("ProtonTrackEndMomentum", &ProtonTrackEndMomentum);
	DJANGOHEventTree->SetBranchAddress("PionTrackEndRadius", &PionTrackEndRadius);
	DJANGOHEventTree->SetBranchAddress("PionTrackEndMomentum", &PionTrackEndMomentum);
	DJANGOHEventTree->SetBranchAddress("ProtonTrackV0Momentum", &ProtonTrackV0Momentum);
	DJANGOHEventTree->SetBranchAddress("PionTrackV0Momentum", &PionTrackV0Momentum);
	DJANGOHEventTree->SetBranchAddress("PionLProton", &PionLProton);
	DJANGOHEventTree->SetBranchAddress("PionLelectron", &PionLelectron);
	DJANGOHEventTree->SetBranchAddress("ProtonLPion", &ProtonLPion);
	DJANGOHEventTree->SetBranchAddress("ProtonLelectron", &ProtonLelectron);
	DJANGOHEventTree->SetBranchAddress("CosThetaStar", &CosThetaStar);



	//Creating Canvas and pads
	TCanvas* canvas = new TCanvas("c1", "multipads", 800, 800);
	TPad* pad1 = new TPad("pad1", "pad1", 0, 0.35, 1, 1.0);
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

	DataHist->SetLineWidth(2);
	DataHist->SetMarkerSize(3);
	RAPGAPHist->SetLineWidth(2);
	DJANGOHHist->SetLineWidth(2);
	

	//Setting Stats to Not True
	DataHist->SetStats(0);
	RAPGAPHist->SetStats(0);
	DJANGOHHist->SetStats(0);

	cout << "Proton Mass: " << mProton << endl;

	//Loop over Data Tree Entries
	int m;
	int entries;//get number of entries in the Event Tree we just loaded in
	entries = DataEventTree->GetEntries();
	double numdataevents = 0;
	for (m = 0; m < entries; m++) {
		DataEventTree->GetEntry(m);
		if (pT > 0.5 && pT < 3.5 && Ys>0.1 && Ys < 0.6 && Q2s>5 && Q2s < 100 && abs(Eta) < 1.3) {
			if (variable == 0) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(invMasslambda, weight1 * weight2);
					//cout << PionTrackV0Momentum->Eta() << endl;
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}

			if (variable == 1) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(Eta, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 2) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(pT, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 3) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(pionFRUNZ, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 4) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(protonFRUNZ, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 5) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(PionPt, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 6) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(ProtonPt, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 7) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(PionTrackLength, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 8) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(ProtonTrackLength, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 9) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(pTrelPion, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 10) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(pTrelProton, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 11) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(Chi2, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 12) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(RadiusSVtoPV, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 13) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(PionDCA, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 14) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(ProtonDCA, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 15) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(PionSDCA, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 16) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(ProtonSDCA, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 17) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(PionL, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 18) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(ProtonL, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 19) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(SVx, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 20) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(SVy, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 21) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(SVz, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 22) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (abs(PionTrackV0Momentum->Eta()) < 0.75) {
						DataHist->Fill(PionTrackEndRadius, weight1 * weight2);
						numdataevents = numdataevents + 1 * weight1 * weight2;
					}
				}
			}
			if (variable == 23) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (abs(ProtonTrackV0Momentum->Eta()) < 0.75 && ProtonTrackEndMomentum->Mag() > 0.0 && ProtonTrackEndMomentum->Mag() < 0.6) {
						DataHist->Fill(ProtonTrackEndRadius, weight1 * weight2);
						numdataevents = numdataevents + 1 * weight1 * weight2;
					}
				}
			}
			if (variable == 24) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionTrackEndRadius < 45.0) {
						if (sqrt(SVx * SVx + SVy * SVy) < 20) {
							double InitialKE = sqrt(PionTrackV0Momentum->Mag() + mPion) - mPion;
							double FinalKE = sqrt(PionTrackEndMomentum->Mag() + mPion) - mPion;
							DataHist->Fill((InitialKE - FinalKE), weight1 * weight2);
							numdataevents = numdataevents + 1 * weight1 * weight2;
							//cout << InitialKE - FinalKE << endl;
						}
					}
				}
			}
			if (variable == 25) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (ProtonTrackEndRadius < 45.0) {
						if (sqrt(SVx * SVx + SVy * SVy) < 20) {
							double InitialKE = sqrt(ProtonTrackV0Momentum->Mag() + mProton) - mProton;
							double FinalKE = sqrt(ProtonTrackEndMomentum->Mag() + mProton) - mProton;
							DataHist->Fill((InitialKE - FinalKE), weight1 * weight2);
							numdataevents = numdataevents + 1 * weight1 * weight2;
							//cout << InitialKE - FinalKE << endl;
						}
					}
				}
			}
			if (variable == 26) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (abs(ProtonTrackV0Momentum->Eta()) < 0.75) {
						DataHist->Fill(ProtonTrackEndMomentum->Mag(), weight1 * weight2);
						numdataevents = numdataevents + 1 * weight1 * weight2;
					}
				}
			}
			if (variable == 27) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
						DataHist->Fill(Q2s, weight1 * weight2);
						numdataevents = numdataevents + 1 * weight1 * weight2;
					
				}
			}
			if (variable == 28) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(Ys, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;

				}
			}
			if (variable == 29) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(x, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;

				}
			}
			if (variable == 30) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(PVz, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 31) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(PionTrackStartRadius, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 32) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(ProtonTrackStartRadius, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 33) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(Angle, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 34) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(PionLelectron, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 35) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(ProtonLelectron, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 36) {
				DataEventTree->GetEntry(m);
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					DataHist->Fill(CosThetaStar, weight1 * weight2);
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
		if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
		if (pT > 0.5 && pT < 3.5 && Ys>0.1 && Ys < 0.6 && Q2s>5 && Q2s < 100 && abs(Eta) < 1.3) {
			if (variable == 0) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					//cout << "Nucl Corr: " << weight1 * weight2 * PionNuclCorrWeight << endl;
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 1) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(Eta, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 2) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(pT, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 3) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(pionFRUNZ, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 4) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(protonFRUNZ, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 5) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(PionPt, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 6) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(ProtonPt, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 7) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(PionTrackLength, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 8) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(ProtonTrackLength, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 9) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(pTrelPion, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 10) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(pTrelProton, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 11) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(Chi2, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 12) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(RadiusSVtoPV, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 13) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(PionDCA, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 14) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(ProtonDCA, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 15) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(PionSDCA, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 16) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(ProtonSDCA, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 17) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(PionL, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 18) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(ProtonL, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 19) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(SVx, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 20) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(SVy, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 21) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(SVz, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 22) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015 && abs(PionTrackV0Momentum->Eta()) < 0.75) {
						RAPGAPHist->Fill(PionTrackEndRadius, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 23) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015 && abs(ProtonTrackV0Momentum->Eta()) < 0.75 && ProtonTrackEndMomentum->Mag() > 0.0 && ProtonTrackEndMomentum->Mag() < 0.6) {
						//if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(ProtonTrackEndRadius, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 24) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015 && PionTrackEndRadius < 45.0) {
						if (sqrt(SVx * SVx + SVy * SVy) < 20) {
							double InitialKE = sqrt(PionTrackV0Momentum->Mag() + mPion) - mPion;
							double FinalKE = sqrt(PionTrackEndMomentum->Mag() + mPion) - mPion;
							RAPGAPHist->Fill((InitialKE - FinalKE), weight1 * weight2 * PionNuclCorrWeight);
							numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
							//cout << InitialKE - FinalKE << endl;
						}
					}
				}
			}
			if (variable == 25) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015 && ProtonTrackEndRadius < 45.0) {
						if (sqrt(SVx * SVx + SVy * SVy) < 20) {
							double InitialKE = sqrt(ProtonTrackV0Momentum->Mag() + mProton) - mProton;
							double FinalKE = sqrt(ProtonTrackEndMomentum->Mag() + mProton) - mProton;
							RAPGAPHist->Fill((InitialKE - FinalKE), weight1 * weight2 * PionNuclCorrWeight);
							numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
							//cout << InitialKE - FinalKE << endl;
						}
					}
				}
			}
			if (variable == 26) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015 && abs(ProtonTrackV0Momentum->Eta()) < 0.75) {
						//if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(ProtonTrackEndMomentum->Mag(), weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 27) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					//cout << "Nucl Corr: " << weight1 * weight2 * PionNuclCorrWeight << endl;
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(Q2s, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 28) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					//cout << "Nucl Corr: " << weight1 * weight2 * PionNuclCorrWeight << endl;
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(Ys, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 29) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					//cout << "Nucl Corr: " << weight1 * weight2 * PionNuclCorrWeight << endl;
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(x, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 30) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(PVz, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 31) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(PionTrackStartRadius, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 32) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(ProtonTrackStartRadius, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 33) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(Angle, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 34) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(PionLelectron, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 35) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(ProtonLelectron, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 36) {
				RAPGAPEventTree->GetEntry(n);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(CosThetaStar, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
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
		if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
		if (pT > 0.5 && pT < 3.5 && Ys>0.1 && Ys < 0.6 && Q2s>5 && Q2s < 100 && abs(Eta) < 1.3) {
			if (variable == 0) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 1) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(Eta, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 2) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(pT, weight1 * weight2)* PionNuclCorrWeight;
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 3) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(pionFRUNZ, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 4) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(protonFRUNZ, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 5) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(PionPt, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 6) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(ProtonPt, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 7) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(PionTrackLength, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 8) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(ProtonTrackLength, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 9) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(pTrelPion, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 10) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(pTrelProton, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 11) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(Chi2, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 12) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(RadiusSVtoPV, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 13) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(PionDCA, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 14) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(ProtonDCA, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 15) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(PionSDCA, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 16) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(ProtonSDCA, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 17) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(PionL, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 18) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(ProtonL, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 19) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(SVx, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 20) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(SVy, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 21) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(SVz, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 22) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015 && abs(PionTrackV0Momentum->Eta()) < 0.75) {
						DJANGOHHist->Fill(PionTrackEndRadius, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 23) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015 && abs(ProtonTrackV0Momentum->Eta()) < 0.75 && ProtonTrackEndMomentum->Mag() > 0.0 && ProtonTrackEndMomentum->Mag() < 0.6) {
						DJANGOHHist->Fill(ProtonTrackEndRadius, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 24) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015 && PionTrackEndRadius < 45.0) {
						if (sqrt(SVx * SVx + SVy * SVy) < 20) {
							double InitialKE = sqrt(PionTrackV0Momentum->Mag() + mPion) - mPion;
							double FinalKE = sqrt(PionTrackEndMomentum->Mag() + mPion) - mPion;
							DJANGOHHist->Fill((InitialKE - FinalKE), weight1 * weight2 * PionNuclCorrWeight);
							numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
							//cout << InitialKE - FinalKE << endl;
						}
					}
				}
			}
			if (variable == 25) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015 && ProtonTrackEndRadius < 45.0) {
						if (sqrt(SVx * SVx + SVy * SVy) < 20) {
							double InitialKE = sqrt(ProtonTrackV0Momentum->Mag() + mProton) - mProton;
							double FinalKE = sqrt(ProtonTrackEndMomentum->Mag() + mProton) - mProton;
							DJANGOHHist->Fill((InitialKE - FinalKE), weight1 * weight2 * PionNuclCorrWeight);
							numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
							//cout << InitialKE - FinalKE << endl;
						}
					}
				}
			}
			if (variable == 26) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015 && abs(ProtonTrackV0Momentum->Eta()) < 0.75) {
						DJANGOHHist->Fill(ProtonTrackEndMomentum->Mag(), weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 27) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(Q2s, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 28) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(Ys, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 29) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(x, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 30) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(PVz, weight1 * weight2* PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 31) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(PionTrackStartRadius, weight1 * weight2* PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 32) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(ProtonTrackStartRadius, weight1 * weight2* PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 33) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(Angle, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 34) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(PionLelectron, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 35) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(ProtonLelectron, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 36) {
				DJANGOHEventTree->GetEntry(p);
				if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
				if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(CosThetaStar, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
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
	DataHist->GetYaxis()->SetTitle(ytitle.c_str());


	TAxis* axis = DataHist->GetYaxis();
	axis->ChangeLabel(1, -1, -1, -1, -1, -1, " ");

	DataHist->GetYaxis()->SetTitleFont(43);
	DataHist->GetYaxis()->SetTitleSize(30);
	DataHist->GetYaxis()->SetLabelFont(43);
	DataHist->GetYaxis()->SetLabelSize(20);
	DataHist->GetYaxis()->SetTitleOffset(2.00);
	//DataHist->GetYaxis()->SetTitleSize(0.35);
	DataHist->Scale(1 / numdataevents);
	RAPGAPHist->Scale(1 / numRAPGAPevents);
	DJANGOHHist->Scale(1 / numDJANGOHevents);
	cout << "Num Data Events: " << numdataevents << endl;
	cout << "Num RAPGAP Events: " << numRAPGAPevents << endl;
	cout << "Num DJANGOH Events: " << numDJANGOHevents << endl;
	//DataHist->Scale(1 / DataLumi);
	//RAPGAPHist->Scale(1 / RAPGAPLumi);
	//DJANGOHHist->Scale(1 / DJANGOHLumi);

	TH2F* xaxis = (TH2F*)DataHist->Clone();
	xaxis->Reset("ICESM");
	xaxis->SetLineColor(kBlack);

	//DataHist->Scale(1);
	//RAPGAPHist->Scale(DataLumi / RAPGAPLumi);
	//DJANGOHHist->Scale(DataLumi / DJANGOHLumi);

	DataHist->Draw("e");
	RAPGAPHist->Draw("histsame");
	DJANGOHHist->Draw("histsame");
	xaxis->Draw("histsame");
	//DataHist->GetXaxis()->SetLabelOffset(999);
	DataHist->GetXaxis()->SetLabelSize(0);

	TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	//legend->SetHeader(Form("#splitline{Count- %.2f #pm %.2f (stat.)}{Stochastic MLP- %s Network Trained %ix}", kcount, kcounterror, network[particlecode].c_str(), trainnum[particlecode])); // option "C" allows to center the header

	//legend->AddEntry(DataHist, "2005-2007 Data", "l");
	//legend->AddEntry(RAPGAPHist, "#frac{L_{Data}}{L_{RAPGAP}} RAPGAP", "l");
	//legend->AddEntry(DJANGOHHist, "#frac{L_{Data}}{L_{DJANGOH}} DJANGOH", "l");

	legend->SetHeader(legendheader.c_str()); // option "C" allows to center the header
	legend->AddEntry(DataHist, "2005-2007 H1 Data", "l");
	legend->AddEntry(RAPGAPHist, "RAPGAP", "l");
	legend->AddEntry(DJANGOHHist, "DJANGOH", "l");
	legend->Draw("same");
	legend->SetTextSize(0.04);

	//Ratio plot will be in the lower pad
	// Go back to the main canvas before defining pad2
	//TPad* pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
	//TPad(const char* name, const char* title,
	//	Double_t xlow, Double_t ylow, Double_t xup, Double_t yup)
	TPad* pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.35);
	canvas->cd();
	pad2->SetTopMargin(0.03);
	pad2->SetBottomMargin(0.4);
	pad1->SetLeftMargin(0.17);
	pad2->SetLeftMargin(0.17);
	//pad2->SetGridx();
	pad2->Draw();
	pad2->cd();
	*DataRatioHist = (*DataHist) / (*DataHist);
	*RAPGAPRatioHist = (*RAPGAPHist) / (*DataHist);
	*DJANGOHRatioHist = (*DJANGOHHist) / (*DataHist);
	DataRatioHist->SetLineWidth(1);
	DataRatioHist->SetTitle("");
	DataRatioHist->GetXaxis()->SetTitle(xtitle.c_str());
	DataRatioHist->GetYaxis()->SetTitleOffset(1.0);
	//DataRatioHist->GetXaxis()->SetLabelOffset(0.015);
	DataRatioHist->GetYaxis()->SetTitle("Rec MC / Data");
	//DataRatioHist->GetYaxis()->SetTitleSize(0.35);
	//DataRatioHist->GetXaxis()->SetTitleSize(0.35);
	DataRatioHist->GetYaxis()->SetRangeUser(0, 2);
	DataRatioHist->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	DataRatioHist->GetXaxis()->SetLabelSize(20);
	DataRatioHist->GetYaxis()->SetLabelFont(43);
	DataRatioHist->GetYaxis()->SetLabelSize(15);
	DataRatioHist->GetXaxis()->SetTitleFont(43);
	DataRatioHist->GetXaxis()->SetTitleSize(40);
	DataRatioHist->GetYaxis()->SetTitleSize(22);
	DataRatioHist->GetXaxis()->SetTitleOffset(3.50);
	DataRatioHist->GetYaxis()->SetTitleOffset(2.50);


	TAxis* axis2 = DataRatioHist->GetYaxis();
	axis2->ChangeLabel(1, -1, -1, -1, -1, -1, " ");

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

void LambdaCandControlPlots() {
	//create file to output plots to
	TFile* outputfile = new TFile(outputfilename.c_str(), "RECREATE");
	//TFile* f = new TFile(dummyfile.c_str(), "RECREATE");

	//Plot(outputfile, 1.09, 1.15, 100, 0,"Mass Distributions","#Lambda + #bar{#Lambda} Mass Distributions","Mass [GeV]");
	
	
	
	
	
	
	//Plot(outputfile, 0, 0.3, 50, 24, "Lambda Reg", "Lambda Reg Pion Track Delta KE Beam Pipe CJC1 Distributions", "#Lambda Pion Track #Delta KE Beam Pipe CJC1", "#Delta KE [GeV]");
	//Plot(outputfile, 0,0.3, 50, 24, "Lambda Bar", "Lambda Bar Pion Track Delta KE Beam Pipe CJC1 Distributions", "#bar{#Lambda} Pion #Delta KE Beam Pipe CJC1", "#Delta KE [GeV]");
	//cout << "Pion Energy dKE Beam Pipe done" << endl;


	//Plot(outputfile, 0, 0.3, 50, 25, "Lambda Reg", "Lambda Reg Proton Track Delta KE Beam Pipe CJC1 Distributions", "#Lambda Proton Track #Delta KE Beam Pipe CJC1", "#Delta KE [GeV]");
	//Plot(outputfile, 0, 0.3, 50, 25, "Lambda Bar", "Lambda Bar Proton Track Delta KE Beam Pipe CJC1 Distributions", "#bar{#Lambda} Proton Track #Delta KE Beam Pipe CJC1", "#Delta KE [GeV]");
	//cout << "Proton Energy dKE Beam Pipe done" << endl;



	//Start Un comment here


	//Plot(outputfile, 1.08, 1.15, 100, 0, "Lambda Reg", "Lambda Reg Mass Distributions", "#Lambda Invariant Mass", "Mass [GeV]",  "1/N dN(#Lambda)/dm [GeV^{-1}]");
	//Plot(outputfile, 1.08, 1.15, 100, 0, "Lambda Bar", "Lambda Bar Mass Distributions", "#bar{#Lambda} Inavariant Mass", "Mass [GeV]",  "1/N dN(#bar{#Lambda})/dm [GeV^{-1}]");
	//cout << "Mass done" << endl;

	//Plot(outputfile, 0, 100, 100, 22, "Lambda Reg", "Lambda Reg Pion Track End Radius Distributions","Lambda Reg Pion Track End Radius Distributions", "R_{end}(#pi^{-}) [cm]", "1/N dN(#Lambda)/dR_{end}(#pi^{-}) [cm^{-1}]");
	//Plot(outputfile, 0, 100, 100, 22, "Lambda Bar", "Lambda Bar Pion Track End Radius Distributions","Lambda Bar Pion Track End Radius Distributions", "R_{end}(#pi^{+}) [cm]", "1/N dN(#bar{#Lambda})/dR_{end}(#pi^{+}) [cm^{-1}]");
	//cout << "Pion Track End Radius done" << endl;

	//Plot(outputfile, 0, 100, 50, 23, "Lambda Reg", "Lambda Reg Proton Track End Radius Distributions","Lambda Reg Proton Track End Radius Distributions", "R_{end}(p) [cm]", "1/N dN(#Lambda)/dR_{end}(p) [cm^{-1}]");
	//Plot(outputfile, 0, 100, 50, 23, "Lambda Bar", "Lambda Bar Proton Track End Radius Distributions","Lambda Bar Proton Track End Radius Distributions", "R_{end}(#bar{p}) [cm]", "1/N dN(#bar{#Lambda})/dR_{end}(#bar{p}) [cm^{-1}]");
	//cout << "Proton Track End Radius done" << endl;

	////Plot(outputfile, 0, 5.0, 100, 26, "Lambda Reg", "Lambda Reg Proton Track End Momentum Distributions", "p Daughter Track of #Lambda End Momentum", "Track End Momentum p_{p} [GeV]", "1/N dN(p)/p_{p} [GeV^{-1}]");
	////Plot(outputfile, 0, 5.0, 100, 26, "Lambda Bar", "Lambda Bar Proton Track End Momentum Distributions", "#bar{p} Daughter Track of #bar{#Lambda} End Momentum", "Track End Momentum p_{#bar{p}} [GeV]", "1/N dN(#bar{p})/p_{#bar{p}} [GeV^{-1}]");
	////cout << "Proton Track End Radius done" << endl;

	//Plot(outputfile, 5, 100, 95, 27, "Lambda Reg", "Lambda Reg Q2s Distributions", "#Lambda Q^{2}", "Q^{2} [GeV^{2}]", "1/N dN(#Lambda)/dQ^{2} [GeV^{-2}]");
	//Plot(outputfile, 5, 100, 95, 27, "Lambda Bar", "Lambda Bar Q2s Distributions", "#bar{#Lambda} Q^{2}", "Q^{2} [GeV^{2}]", "1/N dN(#bar{#Lambda})/dQ^{2} [GeV^{-2}]");
	//cout << "Q2s done" << endl;

	//Plot(outputfile, 0.0375, 0.6, 100, 28, "Lambda Reg", "Lambda Reg Y Distributions", "#Lambda Inelasticity", "y", "1/N dN(#Lambda)/dy");
	//Plot(outputfile, 0.0375, 0.6, 100, 28, "Lambda Bar", "Lambda Bar Y Distributions", "#bar{#Lambda} Inelasticity", "y", "1/N dN(#bar{#Lambda})/dy");
	//cout << "Ys done" << endl;


	//Plot(outputfile, -1.3, 1.3, 100, 1, "Lambda Reg", "Eta Distributions", "#Lambda Pseudorapidity", "#eta(#Lambda)", "1/N dN(#Lambda)/d#eta(#Lambda)");
	//Plot(outputfile, -1.3, 1.3, 100, 1, "Lambda Bar", "Eta Distributions", "#bar{#Lambda} Pseudorapidity", "#eta(#bar{#Lambda})", "1/N dN(#bar{#Lambda})/d#eta(#bar{#Lambda})");
	//cout << "Eta done" << endl;

	//Plot(outputfile, 0.5, 3.5, 100, 2, "Lambda Reg", "pT Distributions", "#Lambda Transverse Momentum", "p_{T}(#Lambda) [GeV]", "1/N dN(#Lambda)/dp_{T}(#Lambda) [GeV^{-1}]");
	//Plot(outputfile, 0.5, 3.5, 100, 2, "Lambda Bar", "pT Distributions", "#bar{#Lambda} Transverse Momentum", "p_{T}(#bar{#Lambda}) [GeV]", "1/N dN(#bar{#Lambda})/dp_{T}(#bar{#Lambda}) [GeV^{-1}]");
	//cout << "pT done" << endl;

	//Plot(outputfile, 0.0, 0.6, 100, 3, "Lambda Reg", "pion FRUNZ", "FRUNZ for Pion Daughters of #Lambda", "FRUNZ(#pi^{-})", "1/N dN(#Lambda)/dFRUNZ(#pi^{-})");
	//Plot(outputfile, 0.0, 0.6, 100, 3, "Lambda Bar", "pion FRUNZ", "FRUNZ for Pion Daughters of #bar{#Lambda}", "FRUNZ(#pi^{+})", "1/N dN(#bar{#Lambda})/dFRUNZ(#pi^{+})");
	//cout << "pion FRUNZ done" << endl;

	//Plot(outputfile, 0.0, 0.6, 100, 4, "Lambda Reg", "proton FRUNZ", "FRUNZ for Proton Daughters of #Lambda", "FRUNZ(p)", "1/N dN(#Lambda)/dFRUNZ(p)");
	//Plot(outputfile, 0.0, 0.6, 100, 4, "Lambda Bar", "proton FRUNZ", "FRUNZ for Proton Daughters of #bar{#Lambda}", "FRUNZ(#bar{p})", "1/N dN(#bar{#Lambda})/dFRUNZ(#bar{p})");
	//cout << "proton FRUNZ done" << endl;

	//Plot(outputfile, 0.0, 2.0, 100, 5, "Lambda Reg", "pion pT", "Pion Daughters of #Lambda p_{T} Transverse Momentum", "p_{T}(#pi^{-}) [GeV]", "1/N dN(#Lambda)/dp_{T}(#pi^{-}) [GeV^{-1}]");
	//Plot(outputfile, 0.0, 2.0, 100, 5, "Lambda Bar", "pion pT", "Pion Daughters of #bar{#Lambda} Transverse Momentum", "p_{T}(#pi^{+}) [GeV]", "1/N dN(#bar{#Lambda})/dp_{T}(#pi^{+}) [GeV^{-1}]");
	//cout << "pion pT done" << endl;

	//Plot(outputfile, 0.0, 3.5, 75, 6, "Lambda Reg", "proton pT", "Proton Daughters of #Lambda Transverse Momentum", "p_{T}(p) [GeV]", "1/N dN(#Lambda)/dp_{T}(p) [GeV^{-1}]");
	//Plot(outputfile, 0.0, 3.5, 75, 6, "Lambda Bar", "proton pT", "Proton Daughters of #bar{#Lambda} Transverse Momentum", "p_{T}(#bar{p}) [GeV]", "1/N dN(#bar{#Lambda})/dp_{T}(#bar{p}) [GeV^{-1}]");
	//cout << "proton pT done" << endl;

	//Plot(outputfile, 10, 65, 100, 7, "Lambda Reg", "pion Track Length", "Pion Daughters of #Lambda Track Length", "L_{track}(#pi^{-}) [cm]", "1/N dN(#Lambda)/dL_{track}(#pi^{-}) [cm^{-1}]");
	//Plot(outputfile, 10, 65, 100, 7, "Lambda Bar", "pion Track Length", "Pion Daughters of #bar{#Lambda} Track Length", "L_{track}(#pi^{+}) [cm]", "1/N dN(#bar{#Lambda})/dL_{track}(#pi^{+}) [cm^{-1}]");
	//cout << "poin track length done" << endl;

	//Plot(outputfile, 10, 65, 100, 8, "Lambda Reg", "proton Track Length", "Proton Daughters of #Lambda Track Length", "L_{track}(p) [cm]", "1/N dN(#Lambda)/dL_{track}(p) [cm^{-1}]");
	//Plot(outputfile, 10, 65, 100, 8, "Lambda Bar", "proton Track Length", "Proton Daughters of #bar{#Lambda} Track Length", "L_{track}(#bar{p}) [cm]", "1/N dN(#bar{#Lambda})/dL_{track}(#bar{p}) [cm^{-1}]");
	//cout << "proton track length done" << endl;

	//Plot(outputfile, 0.0, 0.2, 100, 9, "Lambda Reg", "pion pTrel", "Pion Daughters of #Lambda p_{T,rel}", "p_{T,rel}(#pi^{-})", "1/N dN(#Lambda)/dp_{T,rel}(#pi^{-}) [GeV^{-1}]");
	//Plot(outputfile, 0.0, 0.2, 100, 9, "Lambda Bar", "pion pTrel", "Pion Daughters of #bar{#Lambda} p_{T,rel}", "p_{T,rel}(#pi^{+})", "1/N dN(#bar{#Lambda})/dp_{T,rel}(#pi^{+}) [GeV^{-1}]");
	//cout << "pion pTrel" << endl;

	//Plot(outputfile, 0.0, 0.2, 100, 10, "Lambda Reg", "proton pTrel", "Proton Daughters of #Lambda p_{T,rel}", "p_{T,rel}(p)", "1/N dN(#Lambda)/dp_{T,rel}(p) [GeV^{-1}]");
	//Plot(outputfile, 0.0, 0.2, 100, 10, "Lambda Bar", "proton pTrel", "Proton Daughters of #bar{#Lambda} p_{T,rel}", "p_{T,rel}(#bar{p})", "1/N dN(#bar{#Lambda})/dp_{T,rel}(#bar{p}) [GeV^{-1}]");
	//cout << "proton pTrel" << endl;

	//Plot(outputfile, 0.0, 5, 100, 11, "Lambda Reg", "Chi2", "#Chi^{2} Secondary Vertex Fit of #Lambda Daughter Tracks", "#Chi^{2}", "1/N dN(#Lambda)/d#Chi^{2}");
	//Plot(outputfile, 0.0, 5, 100, 11, "Lambda Bar", "Chi2", "#Chi^{2} Secondary Vertex Fit of #bar{#Lambda} Daughter Tracks", "#Chi^{2}", "1/N dN(#bar{#Lambda})/d#Chi^{2}");
	//cout << "Chi 2 done" << endl;

	//Plot(outputfile, 0.0, 40, 100, 12, "Lambda Reg", "Radial Distance from SV to PV", "#Lambda Radial Distance from SV to PV", "R_{SV-PV} [cm]", "1/N dN(#Lambda)/dR_{SV-PV} [cm^{-1}]");
	//Plot(outputfile, 0.0, 40, 100, 12, "Lambda Bar", "Radial Distance from SV to PV", "#bar{#Lambda} Radial Distance from SV to PV", "R_{SV-PV} [cm]", "1/N dN(#bar{#Lambda})/dR_{SV-PV} [cm^{-1}]");
	//cout << "Radial Distance from SV to PV done" << endl;

	//Plot(outputfile, 0.0001, 0.01, 50, 29, "Lambda Reg", "Bjorken x", "#Lambda Bjorken x", "x", "1/N dN(#Lambda)/dx");
	//Plot(outputfile, 0.0001, 0.01, 50, 29, "Lambda Bar", "Bjorken x", "#bar{#Lambda} Bjorken x", "x", "1/N dN(#bar{#Lambda})/dx");
	//cout << "Bjorken X done" << endl;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////New ones
	//Plot(outputfile, -30.0, 30.0, 50, 13, "Lambda Reg", "Pion DCA", "#Lambda Pion DCA", "d_{ca}(#pi^{-}) [cm]", "1/N dN(#Lambda)/dd_{ca}(#pi^{-}) [cm^{-1}]");
	//Plot(outputfile, -30.0, 30.0, 50, 13, "Lambda Bar", "Pion DCA", "#bar{#Lambda} Pion DCA", "d_{ca}(#pi^{+}) [cm]", "1/N dN(#bar{#Lambda})/dd_{ca}(#pi^{+}) [cm^{-1}]");
	//cout << "Pion DCA done" << endl;

	//Plot(outputfile, -15.0, 15.0, 50, 14, "Lambda Reg", "Proton DCA", "#Lambda Proton DCA", "d_{ca}(p) [cm]", "1/N dN(#Lambda)/dd_{ca}(p) [cm^{-1}]");
	//Plot(outputfile, -15.0, 15.0, 50, 14, "Lambda Bar", "Proton DCA", "#bar{#Lambda} Proton DCA", "d_{ca}(#bar{p}) [cm]", "1/N dN(#bar{#Lambda})/dd_{ca}(#bar{p}) [cm^{-1}]");
	//cout << "Proton DCA done" << endl;

	//Plot(outputfile, 1.0, 100.0, 100, 15, "Lambda Reg", "Pion SDCA", "#Lambda Pion SDCA", "|S_{dca}(#pi^{-})|", "1/N dN(#Lambda)/d|S_{dca}(#pi^{-})|");
	//Plot(outputfile, 1.0, 100.0, 100, 15, "Lambda Bar", "Pion SDCA", "#bar{#Lambda} Pion SDCA", "|S_{dca}(#pi^{+})|", "1/N dN(#bar{#Lambda})/d|S_{dca}(#pi^{+})|");
	//cout << "Pion SDCA done" << endl;

	//Plot(outputfile, 1.0, 100.0, 100, 16, "Lambda Reg", "Proton SDCA", "#Lambda Proton SDCA", "|S_{dca}(p)|", "1/N dN(#Lambda)/d|S_{dca}(p)|");
	//Plot(outputfile, 1.0, 100.0, 100, 16, "Lambda Bar", "Proton SDCA", "#bar{#Lambda} proton SDCA", "|S_{dca}(#bar{p})|", "1/N dN(#bar{#Lambda})/d|S_{dca}(#bar{p})|");
	//cout << "Proton SDCA done" << endl;

	//Plot(outputfile, 0.001, 1.0, 100, 17, "Lambda Reg", "Pion Likelihood", "#Lambda Pion Likelihood", "L_{dE/dx}(#pi^{-})", "1/N dN(#Lambda)/dL_{dE/dx}(#pi^{-})");
	//Plot(outputfile, 0.001, 1.0, 100, 17, "Lambda Bar", "Pion Likelihood", "#bar{#Lambda} Pion Likelihood", "L_{dE/dx}(#pi^{+})", "1/N dN(#bar{#Lambda})/dL_{dE/dx}(#pi^{+})");
	//cout << "Pion Likelihood" << endl;

	//Plot(outputfile, 0.001, 1.0, 100, 18, "Lambda Reg", "Proton Likelihood", "#Lambda Proton Likelihood", "L_{dE/dx}(p)", "1/N dN(#Lambda)/dL_{dE/dx}(p)");
	//Plot(outputfile, 0.001, 1.0, 100, 18, "Lambda Bar", "Proton Likelihood", "#bar{#Lambda} Proton Likelihood", "L_{dE/dx}(#bar{p})", "1/N dN(#bar{#Lambda})/dL_{dE/dx}(#bar{p})");
	//cout << "Proton Likelihood" << endl;

	//Plot(outputfile, -35.0, 35.0, 100, 19, "Lambda Reg", "X Decay Vertex Distributions", "#Lambda Proton SV_{x}", "X_{SV} [cm]", "1/N dN(#Lambda)/dX_{SV} [cm^{-1}]");
	//Plot(outputfile, -35.0, 35.0, 100, 19, "Lambda Bar", "X Decay Vertex Distributions", "#bar{#Lambda} SV_{x}", "X_{SV} [cm]", "1/N dN(#bar{#Lambda})/dX_{SV} [cm^{-1}]");
	//cout << "SVX" << endl;

	//Plot(outputfile, -35.0, 35.0, 100, 20, "Lambda Reg", "Y Decay Vertex Distributions", "#Lambda Proton SV_{y}", "Y_{SV} [cm]", "1/N dN(#Lambda)/dY_{SV} [cm^{-1}]");
	//Plot(outputfile, -35.0, 35.0, 100, 20, "Lambda Bar", "Y Decay Vertex Distributions", "#bar{#Lambda} SV_{y}", "Y_{SV} [cm]", "1/N dN(#bar{#Lambda})/dY_{SV} [cm^{-1}]");
	//cout << "SVY" << endl;

	//Plot(outputfile, -35.0, 35.0, 100, 21, "Lambda Reg", "Z Decay Vertex Distributions", "#Lambda Proton SV_{z}", "Z_{SV} [cm]", "1/N dN(#Lambda)/dZ_{SV} [cm^{-1}]");
	//Plot(outputfile, -35.0, 35.0, 100, 21, "Lambda Bar", "Z Decay Vertex Distributions", "#bar{#Lambda} SV_{z}", "Z_{SV} [cm]", "1/N dN(#bar{#Lambda})/dZ_{SV} [cm^{-1}]");
	//cout << "SVZ" << endl;

	//Plot(outputfile, -35.0, 35.0, 100, 30, "Lambda Reg", "Z Primary Vertex Distributions", "#Lambda PV_{z}", "DIS Event Z_{vtx} [cm]", "1/N dN(#Lambda)/dZ_{vtx} [cm^{-1}]");
	//Plot(outputfile, -35.0, 35.0, 100, 30, "Lambda Bar", "Z Primary Vertex Distributions", "#bar{#Lambda} PV_{z}", "DIS Event Z_{vtx} [cm]", "1/N dN(#bar{#Lambda})/dZ_{vtx} [cm^{-1}]");
	//cout << "Event Z vertex" << endl;

	//Plot(outputfile, 15.0, 35.0, 100, 31, "Lambda Reg", "Pion Track Start Radius", "#Lambda Pion Track Start Radius", "R_{start}(#pi^{-}) [cm]", "1/N dN(#Lambda)/dR_{start}(#pi^{-}) [cm^{-1}]");
	//Plot(outputfile, 15.0, 35.0, 100, 31, "Lambda Bar", "Pion Track Start Radius", "#bar{#Lambda} Pion Track Start Radius", "R_{start}(#pi^{+}) [cm]", "1/N dN(#bar{#Lambda})/dR_{start}(#pi^{+}) [cm^{-1}]");
	//cout << "Pion Track Start Radius" << endl;

	//Plot(outputfile, 15.0, 35.0, 100, 32, "Lambda Reg", "Proton Track Start Radius", "#Lambda Proton Track Start Radius", "R_{start}(p) [cm]", "1/N dN(#Lambda)/dR_{start}(p) [cm^{-1}]");
	//Plot(outputfile, 15.0, 35.0, 100, 32, "Lambda Bar", "Proton Track Start Radius", "#bar{#Lambda} Proton Track Start Radius", "R_{start}(#bar{p}) [cm]", "1/N dN(#bar{#Lambda})/dR_{start}(#bar{p}) [cm^{-1}]");
	//cout << "Proton Track Start Radius" << endl;

	Plot(outputfile, 0, 1.0, 100, 34, "Lambda Reg", "Pion Likelihood Electron", "#Lambda Pion Likelihood Electron", "L", "1/N dN(#Lambda)/dL");
	Plot(outputfile, 0, 1.0, 100, 34, "Lambda Bar", "Pion Likelihood Electron", "#bar{#Lambda} Pion Likelihood Electron", "L", "1/N dN(#bar{#Lambda})/dL");
	cout << "Proton Track Start Radius" << endl;

	Plot(outputfile, 0, 1.0, 100, 35, "Lambda Reg", "Proton Likelihood Electron", "#Lambda Proton Likelihood Electron", "L", "1/N dN(#Lambda)/dL");
	Plot(outputfile, 0, 1.0, 100, 35, "Lambda Bar", "Proton Likelihood Electron", "#bar{#Lambda} Proton Likelihood Electron", "L", "1/N dN(#bar{#Lambda})/dL");
	cout << "Proton Track Start Radius" << endl;

	Plot(outputfile, -5, 5, 500, 36, "Lambda Reg", "Cos Theta Star", "#Lambda Cos Theta Star", "Cos Theta Star", "1/N dN(#Lambda)/dThetaStar");
	Plot(outputfile, -5, 5, 500, 36, "Lambda Bar", "Cos Theta Star", "#bar{#Lambda} Cos Theta Star", "Cos Theta Star", "1/N dN(#bar{#Lambda})/dThetaStar");
	cout << "Proton Track Start Radius" << endl;










	//Old Stuff


	//Angle Check
	//Plot(outputfile, 0, 100, 1000, 33, "Lambda Reg", "Angle", "#Lambda Angle", "Angle", "1/N dN(#Lambda)/dAngle [rad^{-1}]");
	//Plot(outputfile, 0, 100, 100, 33, "Lambda Bar", "Angle", "#bar{#Lambda} Angle", "Angle", "1/N dN(#bar{#Lambda})/dAgnle [rad^{-1}]");
	//cout << "Angle between reconstructed Lambda momentum and SV-PV line" << endl;







	/*Plot(outputfile, -1.3, 1.3, 100, 1, "Eta Distributions", "#Lambda + #bar{#Lambda} #eta Distributions", "#eta");
	cout << "Eta done" << endl;
	Plot(outputfile, 0.5, 3.5, 100, 2, "pT Distributions", "#Lambda + #bar{#Lambda} p_{T} Distributions", "p_{T} [GeV]");
	cout << "pT done" << endl;
	Plot(outputfile, 0.0, 0.6, 100, 3, "pion FRUNZ Distributions", "FRUNZ for Pion Daughters of #Lambda + #bar{#Lambda}", "FRUNZ");
	cout << "pion FRUNZ done" << endl;
	Plot(outputfile, 0.0, 0.6, 100, 4, "proton FRUNZ Distributions", "FRUNZ for Proton Daughters of #Lambda + #bar{#Lambda}", "FRUNZ");
	cout << "proton FRUNZ done" << endl;
	Plot(outputfile, 0.0, 2.0, 100, 5, "pion pT Distributions", "Pion Daughters of #Lambda + #bar{#Lambda} p_{T} Distributions", "p_{T} [GeV]");
	cout << "pion pT done" << endl;
	Plot(outputfile, 0.0, 3.5, 75, 6, "proton pT Distributions", "Proton Daughters of #Lambda + #bar{#Lambda} p_{T} Distributions", "p_{T} [GeV]");
	cout << "proton pT done" << endl;
	Plot(outputfile, 10, 65, 200, 7, "pion Track Length Distributions", "Pion Daughters of #Lambda + #bar{#Lambda} Track Length Distributions", "Track Length [cm]");
	cout << "poin track length done" << endl;
	Plot(outputfile, 10, 65, 200, 8, "proton Track Length Distributions", "Proton Daughters of #Lambda + #bar{#Lambda} Track Length Distributions", "Track Length [cm]");
	cout << "proton track length done" << endl;
	Plot(outputfile, 0.0, 0.2, 100, 9, "pion pTrel", "Pion Daughters of #Lambda + #bar{#Lambda} p_{T,rel}", "p_{T,rel}");
	cout << "pion pTrel" << endl;
	Plot(outputfile, 0.0, 0.2, 100, 10, "proton pTrel", "Proton Daughters of #Lambda + #bar{#Lambda} p_{T,rel}", "p_{T,rel}");
	cout << "proton pTrel" << endl;
	Plot(outputfile, 0.0, 5, 100, 11, "Chi2", "#Chi^{2} Secondary Vertex Fit of Daughter Tracks", "#Chi^{2}");
	cout << "Chi 2 done" << endl;
	Plot(outputfile, 0.0, 40, 200, 12, "Radial Distance from SV to PV", "Radial Distance from SV to PV", "R [cm]");
	cout << "Radial Distance from SV to PV done" << endl;
	*/


	/*Plot(outputfile, -30.0, 30, 50, 13, "Pion DCA", "Pion DCA", "DCA");
	cout << "Pion DCA done" << endl;
	Plot(outputfile, -15.0, 15.0, 50, 14, "Proton DCA", "Proton DCA", "DCA");
	cout << "Proton DCA done" << endl;
	Plot(outputfile, 0.0, 100, 100, 15, "Pion |SDCA|", "Pion |SDCA|", "|SDCA|");
	cout << "Pion SDCA done" << endl;
	Plot(outputfile, 0.0, 100, 100, 16, "Proton |SDCA|", "Proton |SDCA|", "|SDCA|");
	cout << "Proton SDCA done" << endl;
	Plot(outputfile, 0.0, 1.0, 100, 17, "Pion L", "Pion Likelihood", "Pion L_{#frac{dE}{dx}}");
	cout << "Pion L done" << endl;
	Plot(outputfile, 0.0, 1.0, 100, 18, "Proton L", "Proton Likelihood", "Proton L_{#frac{dE}{dx}}");
	cout << "Proton L done" << endl;*/
	//Plot(outputfile, -35, 35, 100, 19, "X Decay Vertex Distributions", "#Lambda + #bar{#Lambda} SV_{x} Distributions", "SV_{x} [cm]");
	//Plot(outputfile, -35, 35, 100, 19, "X Decay Vertex Distributions", "#bar{#Lambda} SV_{x} Distributions", "SV_{x} [cm]");
	//cout << "SVx" << endl;
	//Plot(outputfile, -35, 35, 100, 20, "Y Decay Vertex Distributions", "#Lambda + #bar{#Lambda} SV_{y} Distributions", "SV_{y} [cm]");
	//Plot(outputfile, -35, 35, 100, 20, "Y Decay Vertex Distributions", "#bar{#Lambda} SV_{y} Distributions", "SV_{y} [cm]");
	//cout << "SVy" << endl;
	//Plot(outputfile, -35, 35, 100, 21, "Z Decay Vertex Distributions", "#Lambda + #bar{#Lambda} SV_{z} Distributions", "SV_{z} [cm]");
	//Plot(outputfile, -35, 35, 100, 21, "Z Decay Vertex Distributions", "#bar{#Lambda} SV_{z} Distributions", "SV_{z} [cm]");
	//cout << "SVz" << endl;



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