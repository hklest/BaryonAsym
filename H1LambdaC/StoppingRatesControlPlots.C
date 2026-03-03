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

string outputfilename = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/StoppingRatesControlPlots.root";

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
double invmasslowerbound1 = 1.104;
double invmassupperbound1 = 1.126;
double invmasslowerbound2 = 100;
double invmassupperbound2 = 100;
string legendheader = "Signal Range Normalized Distributions";

//For All
//double invmasslowerbound1 = 0;
//double invmassupperbound1 = 100;
//double invmasslowerbound2 = 100;
//double invmassupperbound2 = 100;
//string legendheader = "Normalized Distributions";

//For Sidebands
//double invmasslowerbound1 = 0;
//double invmassupperbound1 = 1.104;
//double invmasslowerbound2 = 1.126;
//double invmassupperbound2 = 100;
//string legendheader = "Sideband Normalized Distributions";

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
	int fRunType, IsBar, s61;
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
	DataEventTree->SetBranchAddress("s61", &s61);



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
	RAPGAPEventTree->SetBranchAddress("s61", &s61);


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
	DJANGOHEventTree->SetBranchAddress("s61", &s61);


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
			if (s61 > 0 && PionL > 0.05 && ProtonL > 0.05 && abs(ProtonTrackV0Momentum->Eta()) < 1.0) {
				if (variable == 0) {
					DataEventTree->GetEntry(m);
					if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
						//Calculate Ecm(p/pbar N), center of mass energy of proton/antiproton and at rest nucleon in their COM frame
						TLorentzVector p1, p2;
						double protonpmag = sqrt((ProtonTrackEndMomentum->X()*ProtonTrackEndMomentum->X()) + (ProtonTrackEndMomentum->Y()) * (ProtonTrackEndMomentum->Y()) + (ProtonTrackEndMomentum->Z()) * (ProtonTrackEndMomentum->Z()));
						double protonE = sqrt(protonpmag * protonpmag + mProton* mProton);
						p1.SetPxPyPzE(ProtonTrackEndMomentum->X(), ProtonTrackEndMomentum->Y(), ProtonTrackEndMomentum->Z(), protonE); //four momentum of proton/antiproton
						p2.SetPxPyPzE(0, 0, 0, 0.9315); //four momentum of what is assumed to be an at rest nucleon
						TLorentzVector system = p1 + p2;
						//cout << "Px " << ProtonTrackEndMomentum->X() << endl;
						//cout << "ECOM " << system.M() << endl;
						DataHist->Fill(system.M(), weight1 * weight2);
						//cout << PionTrackV0Momentum->Eta() << endl;
						numdataevents = numdataevents + 1 * weight1 * weight2;
					}
				}
				if(variable == 1) {
					DataEventTree->GetEntry(m);
					if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
						if (ProtonTrackEndRadius > 40 && ProtonTrackEndRadius < 55) {
							//Calculate Ecm(p/pbar N), center of mass energy of proton/antiproton and at rest nucleon in their COM frame
							TLorentzVector p1, p2;
							double protonpmag = sqrt((ProtonTrackEndMomentum->X() * ProtonTrackEndMomentum->X()) + (ProtonTrackEndMomentum->Y()) * (ProtonTrackEndMomentum->Y()) + (ProtonTrackEndMomentum->Z()) * (ProtonTrackEndMomentum->Z()));
							double protonE = sqrt(protonpmag * protonpmag + mProton * mProton);
							p1.SetPxPyPzE(ProtonTrackEndMomentum->X(), ProtonTrackEndMomentum->Y(), ProtonTrackEndMomentum->Z(), protonE); //four momentum of proton/antiproton
							p2.SetPxPyPzE(0, 0, 0, 0.9315); //four momentum of what is assumed to be an at rest nucleon
							TLorentzVector system = p1 + p2;
							//cout << "Px " << ProtonTrackEndMomentum->X() << endl;
							//cout << "ECOM " << system.M() << endl;
							DataHist->Fill(system.M(), weight1 * weight2);
							//cout << PionTrackV0Momentum->Eta() << endl;
							numdataevents = numdataevents + 1 * weight1 * weight2;
						}
					}
				}
			}
		}
	}



	//Calculating bin ranges for 10 evenly spaced bins in Ecm, limiting factor will be the interacting Lambda Sample, so using it to calculate 
	if (variable == 1 && treebranch == "Lambda Reg") {
		double nbins = 6;
		double goalcount = numdataevents / nbins;
		double minEcm = 1.9;
		double maxEcm = 3.1;
		double previouscounts = 0;
		std::vector<double> edges;
		std::vector<double> counts;
		edges.push_back(minEcm);

		double cumulative = 0;
		for (int i = 1; i <= DataHist->GetNbinsX(); i++) {
			cumulative += DataHist->GetBinContent(i);
			if (cumulative >= goalcount * edges.size()) {
				edges.push_back(DataHist->GetXaxis()->GetBinUpEdge(i));
				if (edges.size() == 2) {
					counts.push_back(cumulative);
					previouscounts = cumulative;
				}
				if (edges.size() != 2) {
					counts.push_back(cumulative - previouscounts);
					previouscounts = cumulative;
				}
			}
			if ((int)edges.size() == nbins) break;
		}
		edges.push_back(maxEcm);
		
		for (int t = 0; t < edges.size(); t++) {
			cout << "Bin " << t + 1 << ": " << edges[t] << " - " << edges[t + 1] << endl;
			//cout << "Count: " << counts[t] << " Goal: " << goalcount << endl;
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
			if (s61 > 0 && PionL > 0.05 && ProtonL > 0.05 && abs(ProtonTrackV0Momentum->Eta()) < 1.0) {
				if (variable == 0) {
					RAPGAPEventTree->GetEntry(n);
					if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
					if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
						//Calculate Ecm(p/pbar N), center of mass energy of proton/antiproton and at rest nucleon in their COM frame
						TLorentzVector p1, p2;
						double protonpmag = sqrt((ProtonTrackEndMomentum->X() * ProtonTrackEndMomentum->X()) + (ProtonTrackEndMomentum->Y()) * (ProtonTrackEndMomentum->Y()) + (ProtonTrackEndMomentum->Z()) * (ProtonTrackEndMomentum->Z()));
						double protonE = sqrt(protonpmag * protonpmag + mProton * mProton);
						p1.SetPxPyPzE(ProtonTrackEndMomentum->X(), ProtonTrackEndMomentum->Y(), ProtonTrackEndMomentum->Z(), protonE); //four momentum of proton/antiproton
						p2.SetPxPyPzE(0, 0, 0, 0.9315); //four momentum of what is assumed to be an at rest nucleon
						TLorentzVector system = p1 + p2;
						//cout << "Px " << ProtonTrackEndMomentum->X() << endl;
						//cout << "ECOM " << system.M() << endl;
						RAPGAPHist->Fill(system.M(), weight1 * weight2);
						//cout << PionTrackV0Momentum->Eta() << endl;
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
					}
				}
				if (variable == 1) {
					RAPGAPEventTree->GetEntry(n);
					if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
					if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
						//Calculate Ecm(p/pbar N), center of mass energy of proton/antiproton and at rest nucleon in their COM frame
						if (ProtonTrackEndRadius > 40 && ProtonTrackEndRadius < 55) {
							TLorentzVector p1, p2;
							double protonpmag = sqrt((ProtonTrackEndMomentum->X() * ProtonTrackEndMomentum->X()) + (ProtonTrackEndMomentum->Y()) * (ProtonTrackEndMomentum->Y()) + (ProtonTrackEndMomentum->Z()) * (ProtonTrackEndMomentum->Z()));
							double protonE = sqrt(protonpmag * protonpmag + mProton * mProton);
							p1.SetPxPyPzE(ProtonTrackEndMomentum->X(), ProtonTrackEndMomentum->Y(), ProtonTrackEndMomentum->Z(), protonE); //four momentum of proton/antiproton
							p2.SetPxPyPzE(0, 0, 0, 0.9315); //four momentum of what is assumed to be an at rest nucleon
							TLorentzVector system = p1 + p2;
							//cout << "Px " << ProtonTrackEndMomentum->X() << endl;
							//cout << "ECOM " << system.M() << endl;
							RAPGAPHist->Fill(system.M(), weight1 * weight2);
							//cout << PionTrackV0Momentum->Eta() << endl;
							numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2;
						}
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
			if (s61 > 0 && PionL > 0.05 && ProtonL > 0.05 && abs(ProtonTrackV0Momentum->Eta()) < 1.0) {
				if (variable == 0) {
					DJANGOHEventTree->GetEntry(p);
					if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
					if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
						//Calculate Ecm(p/pbar N), center of mass energy of proton/antiproton and at rest nucleon in their COM frame
						TLorentzVector p1, p2;
						double protonpmag = sqrt((ProtonTrackEndMomentum->X() * ProtonTrackEndMomentum->X()) + (ProtonTrackEndMomentum->Y()) * (ProtonTrackEndMomentum->Y()) + (ProtonTrackEndMomentum->Z()) * (ProtonTrackEndMomentum->Z()));
						double protonE = sqrt(protonpmag * protonpmag + mProton * mProton);
						p1.SetPxPyPzE(ProtonTrackEndMomentum->X(), ProtonTrackEndMomentum->Y(), ProtonTrackEndMomentum->Z(), protonE); //four momentum of proton/antiproton
						p2.SetPxPyPzE(0, 0, 0, 0.9315); //four momentum of what is assumed to be an at rest nucleon
						TLorentzVector system = p1 + p2;
						//cout << "ECOM " << system.M() << endl;
						DJANGOHHist->Fill(system.M(), weight1 * weight2);
						//cout << PionTrackV0Momentum->Eta() << endl;
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
					}
				}
				if (variable == 1) {
					DJANGOHEventTree->GetEntry(p);
					if (std::isnan(PionNuclCorrWeight) == 1) { PionNuclCorrWeight = 1; }
					if ((invMasslambda > invmasslowerbound1 && invMasslambda < invmassupperbound1) || invMasslambda > invmasslowerbound2 && invMasslambda < invmassupperbound2) {
						//Calculate Ecm(p/pbar N), center of mass energy of proton/antiproton and at rest nucleon in their COM frame
						if (ProtonTrackEndRadius > 40 && ProtonTrackEndRadius < 55) {
							TLorentzVector p1, p2;
							double protonpmag = sqrt((ProtonTrackEndMomentum->X() * ProtonTrackEndMomentum->X()) + (ProtonTrackEndMomentum->Y()) * (ProtonTrackEndMomentum->Y()) + (ProtonTrackEndMomentum->Z()) * (ProtonTrackEndMomentum->Z()));
							double protonE = sqrt(protonpmag * protonpmag + mProton * mProton);
							p1.SetPxPyPzE(ProtonTrackEndMomentum->X(), ProtonTrackEndMomentum->Y(), ProtonTrackEndMomentum->Z(), protonE); //four momentum of proton/antiproton
							p2.SetPxPyPzE(0, 0, 0, 0.9315); //four momentum of what is assumed to be an at rest nucleon
							TLorentzVector system = p1 + p2;
							//cout << "ECOM " << system.M() << endl;
							DJANGOHHist->Fill(system.M(), weight1 * weight2);
							//cout << PionTrackV0Momentum->Eta() << endl;
							numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2;
						}
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

void StoppingRatesControlPlots() {
	//create file to output plots to
	TFile* outputfile = new TFile(outputfilename.c_str(), "RECREATE");


	Plot(outputfile, 1.9, 3.1, 100, 0, "Lambda Reg", "Reference Lambda Reg Ecm(p N) Distributions", "#Lambda Ecm(p N) Reference Distributions", "Ecm(p N) [GeV]",  "1/N dN(#Lambda)/dEcm(p N) [GeV^{-1}]");
	Plot(outputfile, 1.9, 3.1, 100, 0, "Lambda Bar", "Reference Lambda Bar Ecm(p N) Distributions", "#bar{#Lambda} Ecm(p N) Reference Distributions", "Ecm(p N) [GeV]",  "1/N dN(#bar{#Lambda})/dEcm(p N) [GeV^{-1}]");
	cout << "Ecm Reference" << endl;

	Plot(outputfile, 1.9, 3.1, 100, 1, "Lambda Reg", "Interacting Lambda Reg Ecm(p N) Distributions", "#Lambda Ecm(p N) Interacting Distributions", "Ecm(p N) [GeV]", "1/N dN(#Lambda)/dEcm(p N) [GeV^{-1}]");
	Plot(outputfile, 1.9, 3.1, 100, 1, "Lambda Bar", "Interacting Lambda Bar Ecm(p N) Distributions", "#bar{#Lambda} Ecm(p N) Interacting Distributions", "Ecm(p N) [GeV]", "1/N dN(#bar{#Lambda})/dEcm(p N) [GeV^{-1}]");
	cout << "Ecm Interacting" << endl;

	outputfile->cd();
	outputfile->Write();
	outputfile->Close();

	cout << "Done" << endl;
}