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

string outputfilename = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/K0TotCandControlPlots.root";

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
//double invmasslowerbound1 = 0.45;
//double invmassupperbound1 = 0.55;
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
double invmassupperbound1 = 0.45;
double invmasslowerbound2 = 0.55;
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
	/*double pT, Eta, x, invMasslambda, Q2s, Ys, Angle, v0chi2, weight1, weight2, SVx, SVy, SVz, PVx, PVy, PVz, LambdaPX, LambdaPY, LambdaPZ, ystar, ylab, scatEX, scatEY, scatEZ;
	double PionPt, ProtonPt, PionTrackStartRadius, ProtonTrackStartRadius, PionL, ProtonL, PionCharge, ProtonCharge, PionTrackLength, ProtonTrackLength;
	double PionDCA, ProtonDCA, PionSDCA, ProtonSDCA, Chi2, MassPionPion, MassElecElec, pTrelPion, pTrelProton, RadiusSVtoPV;
	double genIvxX, genIvxY, genIvxZ, decayrecX, decayrecY, decayrecZ, strIVtype, pionFRUNZ, protonFRUNZ;
	int fRunType, IsBar;
	double PionNuclCorrWeight;
	double ProtonTrackEndRadius, PionTrackEndRadius;*/
	//defining the variables in Trees
	double pT, Eta, x, invMasskaon, Q2s, Ys, Angle, weight1, weight2;
	double SVx, SVy, SVz, PVx, PVy, PVz, K0PX, K0PY, K0PZ, ystar, ylab, scatEX, scatEY, scatEZ;
	double pospionFRUNZ, negpionFRUNZ, PosPionPt, NegPionPt, PosPionTrackStartRadius, NegPionTrackStartRadius;
	double PosPiondEdxLikelihood, NegPiondEdxLikelihood, PosPionTrackLength, NegPionTrackLength, PosPionDCA, NegPionDCA, PosPionSDCA, NegPionSDCA;
	double Chi2, MassPionProton, MassElecElec, pTrelPosPion, pTrelNegPion, RadiusSVtoPV, Phi;
	int fRunType, FoundGen, s61, s74, s1, s75, s67;
	double scatEenergy, scatEtheta, scatEphi, NegPionCJC1NHits, PosPionCJC1NHits;
	double PionNuclCorrWeight;

	//TVector3  ProtonTrackEndMomentum, ProtonTrackV0Momentum, PionTrackEndMomentum, PionTrackV0Momentum;
	//TVector3* PionTrackV0Momentum = nullptr;
	//TVector3* ProtonTrackV0Momentum = nullptr;
	//TVector3* PionTrackEndMomentum = nullptr;
	//TVector3* ProtonTrackEndMomentum = nullptr;
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
	DataEventTree->SetBranchAddress("PosPiondEdxLikelihood", &PosPiondEdxLikelihood);
	DataEventTree->SetBranchAddress("NegPiondEdxLikelihood", &NegPiondEdxLikelihood);
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
	DataEventTree->SetBranchAddress("FoundGen", &FoundGen);
	DataEventTree->SetBranchAddress("s61", &s61);
	DataEventTree->SetBranchAddress("s1", &s1);
	DataEventTree->SetBranchAddress("s74", &s74);
	DataEventTree->SetBranchAddress("s75", &s75);
	DataEventTree->SetBranchAddress("s67", &s67);
	DataEventTree->SetBranchAddress("PionNuclCorrWeight", &PionNuclCorrWeight);


	//
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
	RAPGAPEventTree->SetBranchAddress("PosPiondEdxLikelihood", &PosPiondEdxLikelihood);
	RAPGAPEventTree->SetBranchAddress("NegPiondEdxLikelihood", &NegPiondEdxLikelihood);
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
	RAPGAPEventTree->SetBranchAddress("FoundGen", &FoundGen);
	RAPGAPEventTree->SetBranchAddress("s61", &s61);
	RAPGAPEventTree->SetBranchAddress("s1", &s1);
	RAPGAPEventTree->SetBranchAddress("s74", &s74);
	RAPGAPEventTree->SetBranchAddress("s75", &s75);
	RAPGAPEventTree->SetBranchAddress("s67", &s67);
	RAPGAPEventTree->SetBranchAddress("PionNuclCorrWeight", &PionNuclCorrWeight);

	//
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
	DJANGOHEventTree->SetBranchAddress("PosPiondEdxLikelihood", &PosPiondEdxLikelihood);
	DJANGOHEventTree->SetBranchAddress("NegPiondEdxLikelihood", &NegPiondEdxLikelihood);
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
	DJANGOHEventTree->SetBranchAddress("FoundGen", &FoundGen);
	DJANGOHEventTree->SetBranchAddress("s61", &s61);
	DJANGOHEventTree->SetBranchAddress("s1", &s1);
	DJANGOHEventTree->SetBranchAddress("s74", &s74);
	DJANGOHEventTree->SetBranchAddress("s75", &s75);
	DJANGOHEventTree->SetBranchAddress("s67", &s67);
	DJANGOHEventTree->SetBranchAddress("PionNuclCorrWeight", &PionNuclCorrWeight);

	//


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
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(invMasskaon, weight1 * weight2);
					//cout << PionTrackV0Momentum->Eta() << endl;
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}

			if (variable == 1) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(Eta, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 2) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(pT, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 3) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(pospionFRUNZ, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 4) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(negpionFRUNZ, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 5) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(PosPionPt, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 6) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(NegPionPt, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 7) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(PosPionTrackLength, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 8) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(NegPionTrackLength, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 9) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(pTrelPosPion, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 10) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(pTrelNegPion, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 11) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(Chi2, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 12) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(RadiusSVtoPV, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 13) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(PosPionDCA, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 14) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(NegPionDCA, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 15) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(PosPionSDCA, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 16) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(NegPionSDCA, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 17) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(PosPiondEdxLikelihood, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 18) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(NegPiondEdxLikelihood, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 19) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(SVx, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 20) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(SVy, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 21) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(SVz, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			//if (variable == 22) {
			//	DataEventTree->GetEntry(m);
			//	if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
			//		if (abs(PionTrackV0Momentum->Eta()) < 0.75) {
			//			DataHist->Fill(PionTrackEndRadius, weight1 * weight2);
			//			numdataevents = numdataevents + 1 * weight1 * weight2;
			//		}
			//	}
			//}
			//if (variable == 23) {
			//	DataEventTree->GetEntry(m);
			//	if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
			//		if (abs(ProtonTrackV0Momentum->Eta()) < 0.75 && ProtonTrackEndMomentum->Mag() > 0.0 && ProtonTrackEndMomentum->Mag() < 0.6) {
			//			DataHist->Fill(ProtonTrackEndRadius, weight1 * weight2);
			//			numdataevents = numdataevents + 1 * weight1 * weight2;
			//		}
			//	}
			//}
			//if (variable == 24) {
			//	DataEventTree->GetEntry(m);
			//	if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
			//		if (PionTrackEndRadius < 45.0) {
			//			if (sqrt(SVx * SVx + SVy * SVy) < 20) {
			//				double InitialKE = sqrt(PionTrackV0Momentum->Mag() + mPion) - mPion;
			//				double FinalKE = sqrt(PionTrackEndMomentum->Mag() + mPion) - mPion;
			//				DataHist->Fill((InitialKE - FinalKE), weight1 * weight2);
			//				numdataevents = numdataevents + 1 * weight1 * weight2;
			//				//cout << InitialKE - FinalKE << endl;
			//			}
			//		}
			//	}
			//}
			//if (variable == 25) {
			//	DataEventTree->GetEntry(m);
			//	if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
			//		if (ProtonTrackEndRadius < 45.0) {
			//			if (sqrt(SVx * SVx + SVy * SVy) < 20) {
			//				double InitialKE = sqrt(ProtonTrackV0Momentum->Mag() + mProton) - mProton;
			//				double FinalKE = sqrt(ProtonTrackEndMomentum->Mag() + mProton) - mProton;
			//				DataHist->Fill((InitialKE - FinalKE), weight1 * weight2);
			//				numdataevents = numdataevents + 1 * weight1 * weight2;
			//				//cout << InitialKE - FinalKE << endl;
			//			}
			//		}
			//	}
			//}
			//if (variable == 26) {
			//	DataEventTree->GetEntry(m);
			//	if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
			//		if (abs(ProtonTrackV0Momentum->Eta()) < 0.75) {
			//			DataHist->Fill(ProtonTrackEndMomentum->Mag(), weight1 * weight2);
			//			numdataevents = numdataevents + 1 * weight1 * weight2;
			//		}
			//	}
			//}
			if (variable == 27) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
						DataHist->Fill(Q2s, weight1 * weight2);
						numdataevents = numdataevents + 1 * weight1 * weight2;
					
				}
			}
			if (variable == 28) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(Ys, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;

				}
			}
			if (variable == 29) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(x, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;

				}
			}
			if (variable == 30) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(PVz, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 31) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(PosPionTrackStartRadius, weight1 * weight2);
					numdataevents = numdataevents + 1 * weight1 * weight2;
				}
			}
			if (variable == 32) {
				DataEventTree->GetEntry(m);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					DataHist->Fill(NegPionTrackStartRadius, weight1 * weight2);
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
		if (pT > 0.5 && pT < 3.5 && Ys>0.1 && Ys < 0.6 && Q2s>5 && Q2s < 100 && abs(Eta) < 1.3) {
			if (variable == 0) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					//cout << "Nucl Corr: " << weight1 * weight2 * PionNuclCorrWeight << endl;
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(invMasskaon, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 1) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(Eta, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 2) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(pT, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 3) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(pospionFRUNZ, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 4) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(negpionFRUNZ, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 5) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(PosPionPt, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 6) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(NegPionPt, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 7) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(PosPionTrackLength, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 8) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(NegPionTrackLength, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 9) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(pTrelPosPion, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 10) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(pTrelNegPion, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 11) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(Chi2, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 12) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(RadiusSVtoPV, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 13) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(PosPionDCA, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 14) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(NegPionDCA, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 15) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(PosPionSDCA, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 16) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(NegPionSDCA, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 17) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(PosPiondEdxLikelihood, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 18) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(NegPiondEdxLikelihood, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 19) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(SVx, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 20) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(SVy, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 21) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(SVz, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			//if (variable == 22) {
			//	RAPGAPEventTree->GetEntry(n);
			//	if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
			//		if (PionNuclCorrWeight > 0.015 && abs(PionTrackV0Momentum->Eta()) < 0.75) {
			//			RAPGAPHist->Fill(PionTrackEndRadius, weight1 * weight2 * PionNuclCorrWeight);
			//			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
			//		}
			//	}
			//}
			//if (variable == 23) {
			//	RAPGAPEventTree->GetEntry(n);
			//	if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
			//		if (PionNuclCorrWeight > 0.015 && abs(ProtonTrackV0Momentum->Eta()) < 0.75 && ProtonTrackEndMomentum->Mag() > 0.0 && ProtonTrackEndMomentum->Mag() < 0.6) {
			//			//if (PionNuclCorrWeight > 0.015) {
			//			RAPGAPHist->Fill(ProtonTrackEndRadius, weight1 * weight2 * PionNuclCorrWeight);
			//			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
			//		}
			//	}
			//}
			//if (variable == 24) {
			//	RAPGAPEventTree->GetEntry(n);
			//	if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
			//		if (PionNuclCorrWeight > 0.015 && PionTrackEndRadius < 45.0) {
			//			if (sqrt(SVx * SVx + SVy * SVy) < 20) {
			//				double InitialKE = sqrt(PionTrackV0Momentum->Mag() + mPion) - mPion;
			//				double FinalKE = sqrt(PionTrackEndMomentum->Mag() + mPion) - mPion;
			//				RAPGAPHist->Fill((InitialKE - FinalKE), weight1 * weight2 * PionNuclCorrWeight);
			//				numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
			//				//cout << InitialKE - FinalKE << endl;
			//			}
			//		}
			//	}
			//}
			//if (variable == 25) {
			//	RAPGAPEventTree->GetEntry(n);
			//	if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
			//		if (PionNuclCorrWeight > 0.015 && ProtonTrackEndRadius < 45.0) {
			//			if (sqrt(SVx * SVx + SVy * SVy) < 20) {
			//				double InitialKE = sqrt(ProtonTrackV0Momentum->Mag() + mProton) - mProton;
			//				double FinalKE = sqrt(ProtonTrackEndMomentum->Mag() + mProton) - mProton;
			//				RAPGAPHist->Fill((InitialKE - FinalKE), weight1 * weight2 * PionNuclCorrWeight);
			//				numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
			//				//cout << InitialKE - FinalKE << endl;
			//			}
			//		}
			//	}
			//}
			//if (variable == 26) {
			//	RAPGAPEventTree->GetEntry(n);
			//	if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
			//		if (PionNuclCorrWeight > 0.015 && abs(ProtonTrackV0Momentum->Eta()) < 0.75) {
			//			//if (PionNuclCorrWeight > 0.015) {
			//			RAPGAPHist->Fill(ProtonTrackEndMomentum->Mag(), weight1 * weight2 * PionNuclCorrWeight);
			//			numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
			//		}
			//	}
			//}
			if (variable == 27) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					//cout << "Nucl Corr: " << weight1 * weight2 * PionNuclCorrWeight << endl;
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(Q2s, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 28) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					//cout << "Nucl Corr: " << weight1 * weight2 * PionNuclCorrWeight << endl;
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(Ys, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 29) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					//cout << "Nucl Corr: " << weight1 * weight2 * PionNuclCorrWeight << endl;
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(x, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 30) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(PVz, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 31) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(PosPionTrackStartRadius, weight1 * weight2 * PionNuclCorrWeight);
						numRAPGAPevents = numRAPGAPevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 32) {
				RAPGAPEventTree->GetEntry(n);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						RAPGAPHist->Fill(NegPionTrackStartRadius, weight1 * weight2 * PionNuclCorrWeight);
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
		if (pT > 0.5 && pT < 3.5 && Ys>0.1 && Ys < 0.6 && Q2s>5 && Q2s < 100 && abs(Eta) < 1.3) {
			if (variable == 0) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(invMasskaon, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 1) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(Eta, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 2) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(pT, weight1 * weight2)* PionNuclCorrWeight;
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 3) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(pospionFRUNZ, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 4) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(negpionFRUNZ, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 5) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(PosPionPt, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 6) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(NegPionPt, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 7) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(PosPionTrackLength, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 8) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(NegPionTrackLength, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 9) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(pTrelPosPion, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 10) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(pTrelNegPion, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 11) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(Chi2, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 12) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(RadiusSVtoPV, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 13) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(PosPionDCA, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 14) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(NegPionDCA, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 15) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(PosPionSDCA, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 16) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(NegPionSDCA, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 17) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(PosPiondEdxLikelihood, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 18) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(NegPiondEdxLikelihood, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 19) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(SVx, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 20) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(SVy, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 21) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(SVz, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			/*if (variable == 22) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015 && abs(PionTrackV0Momentum->Eta()) < 0.75) {
						DJANGOHHist->Fill(PionTrackEndRadius, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}*/
			/*if (variable == 23) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015 && abs(ProtonTrackV0Momentum->Eta()) < 0.75 && ProtonTrackEndMomentum->Mag() > 0.0 && ProtonTrackEndMomentum->Mag() < 0.6) {
						DJANGOHHist->Fill(ProtonTrackEndRadius, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}*/
			//if (variable == 24) {
			//	DJANGOHEventTree->GetEntry(p);
			//	if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
			//		if (PionNuclCorrWeight > 0.015 && PionTrackEndRadius < 45.0) {
			//			if (sqrt(SVx * SVx + SVy * SVy) < 20) {
			//				double InitialKE = sqrt(PionTrackV0Momentum->Mag() + mPion) - mPion;
			//				double FinalKE = sqrt(PionTrackEndMomentum->Mag() + mPion) - mPion;
			//				DJANGOHHist->Fill((InitialKE - FinalKE), weight1 * weight2 * PionNuclCorrWeight);
			//				numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
			//				//cout << InitialKE - FinalKE << endl;
			//			}
			//		}
			//	}
			//}
			//if (variable == 25) {
			//	DJANGOHEventTree->GetEntry(p);
			//	if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
			//		if (PionNuclCorrWeight > 0.015 && ProtonTrackEndRadius < 45.0) {
			//			if (sqrt(SVx * SVx + SVy * SVy) < 20) {
			//				double InitialKE = sqrt(ProtonTrackV0Momentum->Mag() + mProton) - mProton;
			//				double FinalKE = sqrt(ProtonTrackEndMomentum->Mag() + mProton) - mProton;
			//				DJANGOHHist->Fill((InitialKE - FinalKE), weight1 * weight2 * PionNuclCorrWeight);
			//				numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
			//				//cout << InitialKE - FinalKE << endl;
			//			}
			//		}
			//	}
			//}
			/*if (variable == 26) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015 && abs(ProtonTrackV0Momentum->Eta()) < 0.75) {
						DJANGOHHist->Fill(ProtonTrackEndMomentum->Mag(), weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}*/
			if (variable == 27) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(Q2s, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 28) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(Ys, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 29) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(x, weight1 * weight2 * PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 30) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(PVz, weight1 * weight2* PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 31) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(PosPionTrackStartRadius, weight1 * weight2* PionNuclCorrWeight);
						numDJANGOHevents = numDJANGOHevents + 1 * weight1 * weight2 * PionNuclCorrWeight;
					}
				}
			}
			if (variable == 32) {
				DJANGOHEventTree->GetEntry(p);
				if ((invMasskaon > invmasslowerbound1 && invMasskaon < invmassupperbound1) || invMasskaon > invmasslowerbound2 && invMasskaon < invmassupperbound2) {
					if (PionNuclCorrWeight > 0.015) {
						DJANGOHHist->Fill(NegPionTrackStartRadius, weight1 * weight2* PionNuclCorrWeight);
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

void K0TotCandControlPlots() {
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






	Plot(outputfile, 0.35, 0.7, 100, 0, "K0", "K0 Mass Distributions", "K^{0}_{s} Invariant Mass", "Mass [GeV]",  "1/N dN(K^{0}_{s})/dm [GeV^{-1}]");
	cout << "Mass done" << endl;

	//Plot(outputfile, 0, 100, 100, 22, "K0", "K0 Pion Track End Radius Distributions","K0 Pion Track End Radius Distributions", "R_{end}(#pi) [cm]", "1/N dN(K^{0}_{s})/dR_{end}(#pi^{-}) [cm^{-1}]");
	//cout << "Pion Track End Radius done" << endl;

	//Plot(outputfile, 0, 100, 50, 23, "K0", "K0 Proton Track End Radius Distributions","K0 Proton Track End Radius Distributions", "R_{end}(p) [cm]", "1/N dN(K^{0}_{s})/dR_{end}(p) [cm^{-1}]");
	//cout << "Proton Track End Radius done" << endl;
	// 
	//Plot(outputfile, 0, 5.0, 100, 26, "Lambda Reg", "Lambda Reg Proton Track End Momentum Distributions", "p Daughter Track of #Lambda End Momentum", "Track End Momentum p_{p} [GeV]", "1/N dN(p)/p_{p} [GeV^{-1}]");
	//Plot(outputfile, 0, 5.0, 100, 26, "Lambda Bar", "Lambda Bar Proton Track End Momentum Distributions", "#bar{p} Daughter Track of #bar{#Lambda} End Momentum", "Track End Momentum p_{#bar{p}} [GeV]", "1/N dN(#bar{p})/p_{#bar{p}} [GeV^{-1}]");
	//cout << "Proton Track End Radius done" << endl;

	Plot(outputfile, 5, 100, 95, 27, "K0", "K0 Q2s Distributions", "K^{0}_{s} Q^{2}", "Q^{2} [GeV^{2}]", "1/N dN(K^{0}_{s})/dQ^{2} [GeV^{-2}]");
	cout << "Q2s done" << endl;

	Plot(outputfile, 0.1, 0.6, 100, 28, "K0", "K0 Y Distributions", "K^{0}_{s} Inelasticity", "y", "1/N dN(K^{0}_{s})/dy");
	cout << "Ys done" << endl;


	Plot(outputfile, -1.3, 1.3, 100, 1, "K0", "Eta Distributions", "K^{0}_{s} Pseudorapidity", "#eta(K^{0}_{s})", "1/N dN(K^{0}_{s})/d#eta(K^{0}_{s})");
	cout << "Eta done" << endl;

	Plot(outputfile, 0.5, 3.5, 100, 2, "K0", "pT Distributions", "K^{0}_{s} Transverse Momentum", "p_{T}(K^{0}_{s}) [GeV]", "1/N dN(K^{0}_{s})/dp_{T}(K^{0}_{s}) [GeV^{-1}]");
	cout << "pT done" << endl;

	Plot(outputfile, 0.0, 0.6, 100, 3, "K0", "pos pion FRUNZ", "FRUNZ for Pos Pion Daughters of K^{0}_{s}", "FRUNZ(#pi^{+})", "1/N dN(K^{0}_{s})/dFRUNZ(#pi^{+})");
	cout << "pos pion FRUNZ done" << endl;

	Plot(outputfile, 0.0, 0.6, 100, 4, "K0", "neg pion FRUNZ", "FRUNZ for Neg Pion Daughters of K^{0}_{s}", "FRUNZ(#pi^{-})", "1/N dN(K^{0}_{s})/dFRUNZ(#pi^{-})");
	cout << "neg pion FRUNZ done" << endl;
	
	Plot(outputfile, 0.0, 2.0, 100, 5, "K0", "pos pion pT", "Pos Pion Daughters of K^{0}_{s} p_{T} Transverse Momentum", "p_{T}(#pi^{+}) [GeV]", "1/N dN(K^{0}_{s})/dp_{T}(#pi^{+}) [GeV^{-1}]");
	cout << "pos pion pT done" << endl;

	Plot(outputfile, 0.0, 3.5, 75, 6, "K0", "neg pion pT", "Neg Pion Daughters of K^{0}_{s} Transverse Momentum", "p_{T}(#pi^{-}) [GeV]", "1/N dN(K^{0}_{s})/dp_{T}(#pi^{-}) [GeV^{-1}]");
	cout << "neg pion pT done" << endl;

	Plot(outputfile, 10, 65, 100, 7, "K0", "pos pion Track Length", "Pos Pion Daughters of K^{0}_{s} Track Length", "L_{track}(#pi^{+}) [cm]", "1/N dN(K^{0}_{s})/dL_{track}(#pi^{+}) [cm^{-1}]");
	cout << "pos pion track length done" << endl;

	Plot(outputfile, 10, 65, 100, 8, "K0", "neg pion Track Length", "Neg Pion Daughters of K^{0}_{s} Track Length", "L_{track}(#pi^{-}) [cm]", "1/N dN(K^{0}_{s})/dL_{track}(#pi^{-}) [cm^{-1}]");
	cout << "neg pion track length done" << endl;

	Plot(outputfile, 0.0, 0.4, 100, 9, "K0", "pos pion pTrel", "Pos Pion Daughters of K^{0}_{s} p_{T,rel}", "p_{T,rel}(#pi^{+})", "1/N dN(K^{0}_{s})/dp_{T,rel}(#pi^{+}) [GeV^{-1}]");
	cout << "pos pion pTrel" << endl;

	Plot(outputfile, 0.0, 0.4, 100, 10, "K0", "neg pion pTrel", "Neg Pion Daughters of K^{0}_{s} p_{T,rel}", "p_{T,rel}(#pi^{-})", "1/N dN(K^{0}_{s})/dp_{T,rel}(#pi^{-}) [GeV^{-1}]");
	cout << "neg pion pTrel" << endl;

	Plot(outputfile, 0.0, 5, 100, 11, "K0", "Chi2", "#Chi^{2} Secondary Vertex Fit of K^{0}_{s} Daughter Tracks", "#Chi^{2}", "1/N dN(K^{0}_{s})/d#Chi^{2}");
	cout << "Chi 2 done" << endl;

	Plot(outputfile, 0.0, 40, 100, 12, "K0", "Radial Distance from SV to PV", "K^{0}_{s} Radial Distance from SV to PV", "R_{SV-PV} [cm]", "1/N dN(K^{0}_{s})/dR_{SV-PV} [cm^{-1}]");
	cout << "Radial Distance from SV to PV done" << endl;

	Plot(outputfile, 0.0001, 0.01, 50, 29, "K0", "Bjorken x", "K^{0}_{s} Bjorken x", "x", "1/N dN(K^{0}_{s})/dx");
	cout << "Bjorken X done" << endl;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//New ones
	Plot(outputfile, -30.0, 30.0, 50, 13, "K0", "Pos Pion DCA", "K^{0}_{s} Pos Pion DCA", "d_{ca}(#pi^{+}) [cm]", "1/N dN(K^{0}_{s})/dd_{ca}(#pi^{+}) [cm^{-1}]");
	cout << "Pos Pion DCA done" << endl;

	Plot(outputfile, -15.0, 15.0, 50, 14, "K0", "Neg Pion DCA", "K^{0}_{s} Neg Pion DCA", "d_{ca}(#pi^{-}) [cm]", "1/N dN(K^{0}_{s})/dd_{ca}(#pi^{-}) [cm^{-1}]");
	cout << "Neg Pion DCA done" << endl;

	Plot(outputfile, 1.0, 100.0, 100, 15, "K0", "Pos Pion SDCA", "K^{0}_{s} Pos Pion SDCA", "|S_{dca}(#pi^{+})|", "1/N dN(K^{0}_{s})/d|S_{dca}(#pi^{+})|");
	cout << "Pos Pion SDCA done" << endl;

	Plot(outputfile, 1.0, 100.0, 100, 16, "K0", "Neg Pion SDCA", "K^{0}_{s} Neg Pion SDCA", "|S_{dca}(#pi^{-})|", "1/N dN(K^{0}_{s})/d|S_{dca}(#pi^{-})|");
	cout << "Neg Pion SDCA done" << endl;

	Plot(outputfile, 0.001, 1.0, 100, 17, "K0", "Pos Pion Likelihood", "K^{0}_{s} Pos Pion Likelihood", "L_{dE/dx}(#pi^{+})", "1/N dN(K^{0}_{s})/dL_{dE/dx}(#pi^{+})");
	cout << "Pos Pion Likelihood" << endl;

	Plot(outputfile, 0.001, 1.0, 100, 18, "K0", "Neg Pion Likelihood", "K^{0}_{s} Neg Pion Likelihood", "L_{dE/dx}(#pi^{-})", "1/N dN(K^{0}_{s})/dL_{dE/dx}(#pi^{-})");
	cout << "Neg Pion Likelihood" << endl;

	Plot(outputfile, -35.0, 35.0, 100, 19, "K0", "X Decay Vertex Distributions", "K^{0}_{s} Proton SV_{x}", "X_{SV} [cm]", "1/N dN(K^{0}_{s})/dX_{SV} [cm^{-1}]");
	cout << "SVX" << endl;

	Plot(outputfile, -35.0, 35.0, 100, 20, "K0", "Y Decay Vertex Distributions", "K^{0}_{s} Proton SV_{y}", "Y_{SV} [cm]", "1/N dN(K^{0}_{s})/dY_{SV} [cm^{-1}]");
	cout << "SVY" << endl;

	Plot(outputfile, -35.0, 35.0, 100, 21, "K0", "Z Decay Vertex Distributions", "K^{0}_{s} Proton SV_{z}", "Z_{SV} [cm]", "1/N dN(K^{0}_{s})/dZ_{SV} [cm^{-1}]");
	cout << "SVZ" << endl;

	Plot(outputfile, -35.0, 35.0, 100, 30, "K0", "Z Primary Vertex Distributions", "K^{0}_{s} PV_{z}", "DIS Event Z_{vtx} [cm]", "1/N dN(K^{0}_{s})/dZ_{vtx} [cm^{-1}]");
	cout << "Event Z vertex" << endl;

	//Placeholder here
	Plot(outputfile, 15.0, 35.0, 100, 31, "K0", "Pos Pion Track Start Radius", "K^{0}_{s} Pos Pion Track Start Radius", "R_{start}(#pi^{+}) [cm]", "1/N dN(K^{0}_{s})/dR_{start}(#pi^{+}) [cm^{-1}]");
	cout << "Pos Pion Track Start Radius" << endl;

	Plot(outputfile, 15.0, 35.0, 100, 32, "K0", "Neg Pion Track Start Radius", "K^{0}_{s} Neg Pion Track Start Radius", "R_{start}(#pi^{-}) [cm]", "1/N dN(K^{0}_{s})/dR_{start}(#pi^{-}) [cm^{-1}]");
	cout << "Neg PionTrack Start Radius" << endl;









	outputfile->cd();
	outputfile->Write();
	outputfile->Close();

	cout << "Done" << endl;
}