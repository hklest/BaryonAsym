#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

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
#include <TSystem.h>

using namespace std;

int
main()
{
	//File to write hists to
	TFile file("/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/LambdaDists.root", "RECREATE");
	//File to load 2D hist in
	TFile* f = new TFile("/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/lambda.root");
	TCanvas* c3 = new TCanvas("canvas", "canvas", 10, 10, 800, 400);
	
	//load in
	TH2F* lambdaregpT2D = (TH2F*)f->Get("#Lambda pT vs Mass");
	TH2F* lambdabarpT2D = (TH2F*)f->Get("#bar{#Lambda} pT vs Mass");
	
	//Project to Y to get Lambda pT distribution
	//TH1D* lambdaregpT = lambdaregpT2D->ProjectionY();

	file.cd();
	lambdaregpT2D->Draw();
	lambdaregpT2D->Write();

	f->Close();
	file.Write();
	file.Close();

	if (c3) { c3->Close(); gSystem->ProcessEvents(); delete c3; c3 = 0; };

};