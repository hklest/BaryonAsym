//Analysis Code Root Macro 
//Importing TTrees
#include <iostream>
#include <stdlib.h>
#include <cmath>
using namespace std;


//Change these file paths and output for different years

string fileRAPGAPTree = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/merged.root";

string outputfilename = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/genrecAngleTest.root";

double DataLumi = 297639.1; //nb-1
double RAPGAPLumi = 5293265.0; //nb-1
double DJANGOHLumi = 4108957.0; //nb-1
double PYTHIALumi = 14580680.0; //nb-1


void genrecAngleTest() {
	//create file to output plots to
	TFile* outputfile = new TFile(outputfilename.c_str(), "RECREATE");
	//TFile* f = new TFile(dummyfile.c_str(), "RECREATE");

	//Plot(outputfile, 5, 100, 95, 0,"Q2 Distributions","Q^{2} Distributions","Q^{2}");
	//cout << "Q2 done" << endl;
	
	//Getting Data File
	//TFile* Data = new TFile(fileDataTree.c_str(), "read");
	TFile* RAPGAP = new TFile(fileRAPGAPTree.c_str(), "read");
	//TFile* DJANGOH = new TFile(fileDJANGOHTree.c_str(), "read");
	//TFile* PYTHIA = new TFile(filePYTHIATree.c_str(), "read");
	//Getting Event Tree from the Data file
	//TTree* DataTree = (TTree*)Data->Get("Data Event Tree");
	TTree* RAPGAPtree = (TTree*)RAPGAP->Get("Lambda Reg Gen");
	//TTree* DJANGOHTree = (TTree*)DJANGOH->Get("Data Event Tree");
	//TTree* PYTHIATree = (TTree*)PYTHIA->Get("Data Event Tree");


	int entries = RAPGAPtree->GetEntries();
	double invMass, pT, Eta, x, Q2s, Ys, Angle, weight1, weight2, Phi, SVx, SVy, SVz, Phigen, genrecAngle;
	//int s61;

	RAPGAPtree->SetBranchAddress("invMasslambda", &invMass);
	RAPGAPtree->SetBranchAddress("pT", &pT);
	RAPGAPtree->SetBranchAddress("Eta", &Eta);
	RAPGAPtree->SetBranchAddress("x", &x);
	RAPGAPtree->SetBranchAddress("Q2s", &Q2s);
	RAPGAPtree->SetBranchAddress("Ys", &Ys);
	//RAPGAPtree->SetBranchAddress("Angle", &Angle);
	RAPGAPtree->SetBranchAddress("weight1", &weight1);
	RAPGAPtree->SetBranchAddress("weight2", &weight2);
	//RAPGAPtree->SetBranchAddress("s61", &s61);
	RAPGAPtree->SetBranchAddress("Phi", &Phi);
	RAPGAPtree->SetBranchAddress("SVx", &SVx);
	RAPGAPtree->SetBranchAddress("SVy", &SVy);
	RAPGAPtree->SetBranchAddress("SVz", &SVz);
	RAPGAPtree->SetBranchAddress("genrecAngle", &genrecAngle);
	//RAPGAPtree->SetBranchAddress("SVz", &SVz);
	
	// Histograms Binning
	Double_t lambdabins = 1200;
	Double_t lambdaupperbinrange = 1.50014;
	Double_t lambdalowerbinrange = 1.00014;
	TH1F* Hist = new TH1F("Hist", "Hist", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
	TH1F* HistBG1 = new TH1F("Hist BG", "Hist BG", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
	TH1F* HistBG2 = new TH1F("Hist BG", "Hist BG", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
	TH1F* HistBG3 = new TH1F("Hist BG", "Hist BG", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
	TH1F* HistBG4 = new TH1F("Hist BG", "Hist BG", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
	TH1F* HistBG5 = new TH1F("Hist BG", "Hist BG", lambdabins, lambdalowerbinrange, lambdaupperbinrange);


	int m;
	//int j;
	//Have to enforce pT and Eta bounds here
	for (m = 0; m < entries; m++) {
		RAPGAPtree->GetEntry(m);
		//for (j = 0; j < binentries; j++) {
			//if (s61 > 0) {
				if (pT > 0.5 && pT < 3.5) {
					if (abs(Eta) < 1.3) {
						if (Ys > 0.1 && Ys < 0.6) {
							if (Q2s > 5 && Q2s < 100) {
								//distvariable = { Q2s,Q2s / (Ys * 101568),pT,Eta, Phi,sqrt((SVx) * (SVx)+(SVy) * (SVy)), SVz };
								if (abs(SVz) <= 30.0) {
									//if (distvariable[distvariableindex] > binsvector[j] && distvariable[distvariableindex] < binsvector[j + 1]) {
									Hist->Fill(invMass, weight1 * weight2);
									if (genrecAngle>5) {
										HistBG1->Fill(invMass, weight1 * weight2);
									}
									if (genrecAngle > 4) {
										HistBG2->Fill(invMass, weight1 * weight2);
									}
									if (genrecAngle > 3) {
										HistBG3->Fill(invMass, weight1 * weight2);
									}
									if (genrecAngle > 2) {
										HistBG4->Fill(invMass, weight1 * weight2);
									}
									if (genrecAngle > 1) {
										HistBG5->Fill(invMass, weight1 * weight2);
									}
										//cout << "Entry" << endl;
									//}
								}
							}
						}
					}
				}
			//}
		//}
	}

	
	TLorentzVector proton_target(0, 0, -sqrt((920) * (920) - 0.938 * 0.938), 920);
	TVector3 Boost = (proton_target).BoostVector();

	TLorentzVector lambdavector();

	//TLorentzVector Boostedlambda4vector = lambdavector;
	//Boostedlambda4vector.Boost(-Boost);
	//ylab = lambda4vector.Rapidity();
	//ycom = Boostedlambda4vector.Rapidity(); //Takes lambda four vector from lab frame into HCM frame and calculates the rapidity
	//cout << "neg" << endl;
	TVector3 beta_vec = proton_target.BoostVector();
	cout << "BETA: " << beta_vec.Mag() << endl;
	
	HistBG1->SetLineColor(kBlack);
	HistBG2->SetLineColor(kRed);
	HistBG3->SetLineColor(kBlue);
	HistBG4->SetLineColor(kGreen);
	HistBG5->SetLineColor(kCyan);

	//LEGEND
	TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	//yScatter3[k] = 100 * abs(LambdaRegDataCount[k][0] - LambdaRegDataCount[k][6]) / LambdaRegDataCount[k][0]; //Lambda
	legend->AddEntry(HistBG1, "genrecAngle>5");
	legend->AddEntry(HistBG2, "genrecAngle>4");
	legend->AddEntry(HistBG3, "genrecAngle>3");
	legend->AddEntry(HistBG4, "genrecAngle>2");
	legend->AddEntry(HistBG5, "genrecAngle>1");

	TCanvas* canvas = new TCanvas("c1", "multipads", 800, 800);
	canvas->SetName("Canvas");
	Hist->Draw("HIST");
	HistBG1->Draw("SAMEHIST");
	HistBG2->Draw("SAMEHIST");
	HistBG3->Draw("SAMEHIST");
	HistBG4->Draw("SAMEHIST");
	HistBG5->Draw("SAMEHIST");
	legend->Draw("SAME");
	outputfile->cd();
	Hist->Write();
	//HistBG->Write();
	canvas->Write();

	outputfile->cd();
	outputfile->Write();
	outputfile->Close();

	cout << "Done" << endl;
}