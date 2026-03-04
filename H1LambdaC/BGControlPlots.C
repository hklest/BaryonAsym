//Analysis Code Root Macro 
//Importing TTrees
#include <iostream>
#include <stdlib.h>
#include <cmath>
using namespace std;


//Change these file paths and output for different years
string fileDataTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/merged.root";
string fileRAPGAPTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/merged.root";
string fileDJANGOHTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/merged.root";

string outputfilename = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/BGControlPlots.root";

double DataLumi = 297639.1; //nb-1
double RAPGAPLumi = 5293265.0; //nb-1
double DJANGOHLumi = 4108957.0; //nb-1
double PYTHIALumi = 14580680.0; //nb-1



void BGControlPlots() {
	//create file to output plots to
	TFile* outputfile = new TFile(outputfilename.c_str(), "RECREATE");
	//TFile* f = new TFile(dummyfile.c_str(), "RECREATE");

	//Getting Data File
	//TFile* Data = new TFile(fileDataTree.c_str(), "read");
	TFile* RAPGAP = new TFile(fileRAPGAPTree.c_str(), "read");
	TFile* DJANGOH = new TFile(fileDJANGOHTree.c_str(), "read");
	
	//Getting Lambda Tree from the Data file
	//TTree* DataEventTree = (TTree*)Data->Get("Data Event Tree");
	TTree* RAPGAPEventTree = (TTree*)RAPGAP->Get("Lambda Tot BG");
	TTree* DJANGOHEventTree = (TTree*)DJANGOH->Get("Lambda Tot BG");

	//Define the variables and banch addresses for data we want from the Event Tree entries
	double weight1, weight2, elecPhi, elecTheta, Epz, elecE, x, Q2s, Ys, Q2sGen, YsGen, RTheta, ClusterRadius, Zvertex, ProtonEnergy, ElectronEnergy, Xcluster, Ycluster, Zcluster, RunNumber, RunLumi, fposEX, fposEY, fRunType; //, RCluster,;
	double measuredCharge, epT, Phi;
	int s61, s1, s74, s75, s67, beamCharge, TrType, TrackCheck;
	int fMyRunYear;
	double invMasslambda, pT, Eta, Phu, Phigen, Angle, v0chi2, SVx, SVy, SVz, PVx, PVy, PVz, LambdaPX, LambdaPY, LambdaPZ, ystar, ylab, scatEX, scatEY, scatEZ;
	double pTgen, Etagen, xgen, Q2sgen, Ysgen, SVxgen, SVygen, SVzgen, PVzgen;
	
	RAPGAPEventTree->SetBranchAddress("invMasslambda", &invMasslambda);
	RAPGAPEventTree->SetBranchAddress("pT", &pT);
	RAPGAPEventTree->SetBranchAddress("Eta", &Eta);
	RAPGAPEventTree->SetBranchAddress("Phi", &Phi);
	RAPGAPEventTree->SetBranchAddress("Phigen", &Phigen);
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
	//Adding STR specific branches
	RAPGAPEventTree->SetBranchAddress("pTgen", &pTgen);
	RAPGAPEventTree->SetBranchAddress("Etagen", &Etagen);
	RAPGAPEventTree->SetBranchAddress("xgen", &xgen);
	RAPGAPEventTree->SetBranchAddress("Q2sgen", &Q2sgen);
	RAPGAPEventTree->SetBranchAddress("Ysgen", &Ysgen);
	RAPGAPEventTree->SetBranchAddress("SVxgen", &SVxgen);
	RAPGAPEventTree->SetBranchAddress("SVygen", &SVygen);
	RAPGAPEventTree->SetBranchAddress("SVzgen", &SVzgen);
	RAPGAPEventTree->SetBranchAddress("PVzgen", &PVzgen);
	
	DJANGOHEventTree->SetBranchAddress("invMasslambda", &invMasslambda);
	DJANGOHEventTree->SetBranchAddress("pT", &pT);
	DJANGOHEventTree->SetBranchAddress("Eta", &Eta);
	DJANGOHEventTree->SetBranchAddress("Phi", &Phi);
	DJANGOHEventTree->SetBranchAddress("Phigen", &Phigen);
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
	//Adding STR specific branches
	DJANGOHEventTree->SetBranchAddress("pTgen", &pTgen);
	DJANGOHEventTree->SetBranchAddress("Etagen", &Etagen);
	DJANGOHEventTree->SetBranchAddress("xgen", &xgen);
	DJANGOHEventTree->SetBranchAddress("Q2sgen", &Q2sgen);
	DJANGOHEventTree->SetBranchAddress("Ysgen", &Ysgen);
	DJANGOHEventTree->SetBranchAddress("SVxgen", &SVxgen);
	DJANGOHEventTree->SetBranchAddress("SVygen", &SVygen);
	DJANGOHEventTree->SetBranchAddress("SVzgen", &SVzgen);
	DJANGOHEventTree->SetBranchAddress("PVzgen", &PVzgen);

	
	//Creating Canvas and pads
	TCanvas* canvas = new TCanvas("Canvas", "Canvas", 10, 10, 800, 800);

	// Histograms Binning, have some histograms filled directly from analysis that I use as a consistency check with the histograms I make from my Analysis code using the TTrees I create here
	//Double_t bins = 100;
	//Double_t lowerbinrange = 0.1;
	//Double_t upperbinrange = 0.6;


	//Define the three hists: Data, RAPGAP, DJANGOH
	//TH1F* RAPGAPHist = new TH1F("RAPGAP", "RAPGAP", bins, lowerbinrange, upperbinrange);
	TH2F* XYRAPGAP = new TH2F("XY RAPGAP", "XY RAPGAP", 200, -30, 30, 200, -30, 30);
	TH2F* ZRRAPGAP = new TH2F("ZR RAPGAP", "ZR RAPGAP", 200, -30, 30, 200, 0, 30);
	TH2F* XYDJANGOH = new TH2F("XY DJANGOH", "XY DJANGOH", 200, -30, 30, 200, -30, 30);
	TH2F* ZRDJANGOH = new TH2F("ZR DJANGOH", "ZR DJANGOH", 200, -30, 30, 200, 0, 30);

	TH2F* XYRAPGAPgen = new TH2F("Gen XY RAPGAP", "XY RAPGAP", 200, -30, 30, 200, -30, 30);
	TH2F* XYDJANGOHgen = new TH2F("Gen XY DJANGOH", "XY DJANGOH", 200, -30, 30, 200, -30, 30);
	TH2F* ZRRAPGAPgen = new TH2F("Gen ZR RAPGAP", "Gen ZR RAPGAP", 200, -30, 30, 200, 0, 30);
	TH2F* ZRDJANGOHgen = new TH2F("Gen ZR DJANGOH", "Gen ZR DJANGOH", 200, -30, 30, 200, 0, 30);
	
	int entries;
	//Loop over RAPGAP Tree Entries
	int n;
	entries = RAPGAPEventTree->GetEntries();
	for (n = 0; n < entries; n++) {
		RAPGAPEventTree->GetEntry(n);
		if (invMasslambda > 1.095 && invMasslambda < 1.14) {
			XYRAPGAP->Fill(SVx, SVy, weight1 * weight2);
			XYRAPGAPgen->Fill(SVxgen, SVygen, weight1 * weight2);
			ZRRAPGAP->Fill(SVz, sqrt(SVx * SVx + SVy * SVy), weight1 * weight2);
			ZRRAPGAPgen->Fill(SVzgen, sqrt(SVxgen * SVxgen + SVygen * SVygen), weight1 * weight2);
		}
	}

	//Loop over DJANGOH Tree Entries
	int p;
	entries = DJANGOHEventTree->GetEntries();
	for (p = 0; p < entries; p++) {
		DJANGOHEventTree->GetEntry(p);
		if (invMasslambda > 1.095 && invMasslambda < 1.14) {
			XYDJANGOH->Fill(SVx, SVy, weight1 * weight2);
			XYDJANGOHgen->Fill(SVxgen, SVygen, weight1 * weight2);
			ZRDJANGOH->Fill(SVz, sqrt(SVx * SVx + SVy * SVy), weight1 * weight2);
			ZRDJANGOHgen->Fill(SVzgen, sqrt(SVxgen * SVxgen + SVygen * SVygen), weight1 * weight2);
		}
	}

	outputfile->cd();

	XYRAPGAP->SetStats(0);
	XYRAPGAP->GetXaxis()->SetTitle("SV_{x} [cm]");
	XYRAPGAP->GetYaxis()->SetTitle("SV_{y} [cm]");
	XYRAPGAP->SetTitle("XY Decay Vertices RAPGAP");
	//hAPbefore->GetXaxis()->SetTitleSize(0.04);
	//hAPbefore->GetXaxis()->SetTitleOffset(1);
	//hAPbefore->GetYaxis()->SetTitleSize(0.05);
	XYRAPGAP->Draw("colz");
	canvas->SetTitle("XY Decay Vertices RAPGAP");
	canvas->SetName("XY Decay Vertices RAPGAP");
	canvas->Write();

	XYRAPGAPgen->SetStats(0);
	XYRAPGAPgen->GetXaxis()->SetTitle("SV_{x} [cm]");
	XYRAPGAPgen->GetYaxis()->SetTitle("SV_{y} [cm]");
	XYRAPGAPgen->SetTitle("Gen XY Decay Vertices RAPGAP");
	XYRAPGAPgen->Draw("colz");
	canvas->SetTitle("Gen XY Decay Vertices RAPGAP");
	canvas->SetName("Gen XY Decay Vertices RAPGAP");
	canvas->Write();

	ZRRAPGAP->SetStats(0);
	ZRRAPGAP->GetXaxis()->SetTitle("SV_{z} [cm]");
	ZRRAPGAP->GetYaxis()->SetTitle("SV_{R} [cm]");
	ZRRAPGAP->SetTitle("ZR Decay Vertices RAPGAP");
	ZRRAPGAP->Draw("colz");
	canvas->SetTitle("ZR Decay Vertices RAPGAP");
	canvas->SetName("ZR Decay Vertices RAPGAP");
	canvas->Write();

	ZRRAPGAPgen->SetStats(0);
	ZRRAPGAPgen->GetXaxis()->SetTitle("SV_{z} [cm]");
	ZRRAPGAPgen->GetYaxis()->SetTitle("SV_{R} [cm]");
	ZRRAPGAPgen->SetTitle("Gen ZR Decay Vertices RAPGAP");
	ZRRAPGAPgen->Draw("colz");
	canvas->SetTitle("Gen ZR Decay Vertices RAPGAP");
	canvas->SetName("Gen ZR Decay Vertices RAPGAP");
	canvas->Write();

	
	XYDJANGOH->SetStats(0);
	XYDJANGOH->GetXaxis()->SetTitle("SV_{x} [cm]");
	XYDJANGOH->GetYaxis()->SetTitle("SV_{y} [cm]");
	XYDJANGOH->SetTitle("XY Decay Vertices DJANGOH");
	//hAPbefore->GetXaxis()->SetTitleSize(0.04);
	//hAPbefore->GetXaxis()->SetTitleOffset(1);
	//hAPbefore->GetYaxis()->SetTitleSize(0.05);
	XYDJANGOH->Draw("colz");
	canvas->SetTitle("XY Decay Vertices DJANGOH");
	canvas->SetName("XY Decay Vertices DJANGOH");
	canvas->Write();

	XYDJANGOHgen->SetStats(0);
	XYDJANGOHgen->GetXaxis()->SetTitle("SV_{x} [cm]");
	XYDJANGOHgen->GetYaxis()->SetTitle("SV_{y} [cm]");
	XYDJANGOHgen->SetTitle("Gen XY Decay Vertices DJANGOH");
	XYDJANGOHgen->Draw("colz");
	canvas->SetTitle("Gen XY Decay Vertices DJANGOH");
	canvas->SetName("Gen XY Decay Vertices DJANGOH");
	canvas->Write();

	ZRDJANGOH->SetStats(0);
	ZRDJANGOH->GetXaxis()->SetTitle("SV_{z} [cm]");
	ZRDJANGOH->GetYaxis()->SetTitle("SV_{R} [cm]");
	ZRDJANGOH->SetTitle("ZR Decay Vertices DJANGOH");
	ZRDJANGOH->Draw("colz");
	canvas->SetTitle("ZR Decay Vertices DJANGOH");
	canvas->SetName("ZR Decay Vertices DJANGOH");
	canvas->Write();

	ZRDJANGOHgen->SetStats(0);
	ZRDJANGOHgen->GetXaxis()->SetTitle("SV_{z} [cm]");
	ZRDJANGOHgen->GetYaxis()->SetTitle("SV_{R} [cm]");
	ZRDJANGOHgen->SetTitle("Gen ZR Decay Vertices DJANGOH");
	ZRDJANGOHgen->Draw("colz");
	canvas->SetTitle("Gen ZR Decay Vertices DJANGOH");
	canvas->SetName("Gen ZR Decay Vertices DJANGOH");
	canvas->Write();


	//Plot(outputfile, 27, 28, 500, 10, "Electron Energy", "Electron Energy", "E_{electron beam} [GeV]");
	//cout << "Electron Energy done" << endl;*/

	outputfile->cd();
	outputfile->Write();
	outputfile->Close();

	cout << "Done" << endl;
}