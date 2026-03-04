//Analysis Code Root Macro
//Importing TTrees
#include <iostream>
#include <stdlib.h>
#include <cmath>
using namespace std;

//Change these file paths and output for different years
//string fileDataTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/EventLists/merged.root";
//string fileRAPGAPTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/EventLists/RAPGAP31/merged.root";
//string fileDJANGOHTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/EventLists/DJANGOH14/merged.root";

string fileDataTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/merged.root";
string fileRAPGAPTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/merged.root";
string fileDJANGOHTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/merged.root";

//string filePYTHIATree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/EventLists/PYTHIA62/merged.root";

string outputfilename = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/TriggerEff.root";

vector<vector<double>> AnalyzeTree(int idx, int idxvar, string plotcanvasname, string plottitle, string xaxistitle, string yaxistitle, TFile* Data, TFile* MC, TFile* Output, string TreeName, vector<double> binsvector, int binentries) {
	TTree* tree = (TTree*)Data->Get(TreeName.c_str());
	TTree* treeMC = (TTree*)MC->Get(TreeName.c_str());
	//Define the variables and banch addresses for data we want from the Event Tree entries
	double weight1, weight2, elecPhi, elecTheta, Epz, elecE, x, Q2s, Ys, Q2sGen, YsGen, RTheta, ClusterRadius, Zvertex, ProtonEnergy, ElectronEnergy, Xcluster, Ycluster, Zcluster, RunNumber, RunLumi, fposEX, fposEY, fRunType; //, RCluster,;
	double measuredCharge, epT;
	int s61, s1, s74, s75, s67, beamCharge, TrType, TrackCheck;
	int fMyRunYear;
	//Defining Tree Branches
	//tree->SetBranchAddress("elecPhi", &elecPhi);
	//tree->SetBranchAddress("elecTheta", &elecTheta);
	tree->SetBranchAddress("x", &x);
	tree->SetBranchAddress("Q2s", &Q2s);
	tree->SetBranchAddress("Ys", &Ys);
	//tree->SetBranchAddress("Q2sGen", &Q2sGen);
	//tree->SetBranchAddress("YsGen", &YsGen);
	//tree->SetBranchAddress("Epz", &Epz);
	//tree->SetBranchAddress("elecE", &elecE);
	//tree->SetBranchAddress("RTheta", &RTheta);
	//tree->SetBranchAddress("ClusterRadius", &ClusterRadius);
	//tree->SetBranchAddress("Zvertex", &Zvertex);
	//tree->SetBranchAddress("ProtonEnergy", &ProtonEnergy);
	//tree->SetBranchAddress("ElectronEnergy", &ElectronEnergy);
	//tree->SetBranchAddress("Xcluster", &Xcluster);
	//tree->SetBranchAddress("Ycluster", &Ycluster);
	//tree->SetBranchAddress("Zcluster", &Zcluster);
	//tree->SetBranchAddress("RunNumber", &RunNumber);
	//tree->SetBranchAddress("RunLumi", &RunLumi);
	//tree->SetBranchAddress("fRunType", &fRunType);
	//tree->SetBranchAddress("fMyRunYear", &fMyRunYear);
	//tree->SetBranchAddress("fposEX", &fposEX);
	tree->SetBranchAddress("fposEY", &fposEY);
	tree->SetBranchAddress("weight1", &weight1);
	tree->SetBranchAddress("weight2", &weight2);
	tree->SetBranchAddress("s61", &s61);
	tree->SetBranchAddress("s1", &s1);
	tree->SetBranchAddress("s74", &s74);
	tree->SetBranchAddress("s75", &s75);
	tree->SetBranchAddress("s67", &s67);
	//tree->SetBranchAddress("beamCharge", &beamCharge);
	//tree->SetBranchAddress("TrType", &TrType);
	//tree->SetBranchAddress("TrackCheck", &TrackCheck);
	//tree->SetBranchAddress("measuredCharge", &measuredCharge);
	//tree->SetBranchAddress("epT", &epT);

	//Defining MC Tree Branches
	//treeMC->SetBranchAddress("elecPhi", &elecPhi);
	//treeMC->SetBranchAddress("elecTheta", &elecTheta);
	treeMC->SetBranchAddress("x", &x);
	treeMC->SetBranchAddress("Q2s", &Q2s);
	treeMC->SetBranchAddress("Ys", &Ys);
	//treeMC->SetBranchAddress("Q2sGen", &Q2sGen);
	//treeMC->SetBranchAddress("YsGen", &YsGen);
	//treeMC->SetBranchAddress("Epz", &Epz);
	//treeMC->SetBranchAddress("elecE", &elecE);
	//treeMC->SetBranchAddress("RTheta", &RTheta);
	//treeMC->SetBranchAddress("ClusterRadius", &ClusterRadius);
	//treeMC->SetBranchAddress("Zvertex", &Zvertex);
	//treeMC->SetBranchAddress("ProtonEnergy", &ProtonEnergy);
	//treeMC->SetBranchAddress("ElectronEnergy", &ElectronEnergy);
	//treeMC->SetBranchAddress("Xcluster", &Xcluster);
	//treeMC->SetBranchAddress("Ycluster", &Ycluster);
	//treeMC->SetBranchAddress("Zcluster", &Zcluster);
	//treeMC->SetBranchAddress("RunNumber", &RunNumber);
	//treeMC->SetBranchAddress("RunLumi", &RunLumi);
	//treeMC->SetBranchAddress("fRunType", &fRunType);
	//treeMC->SetBranchAddress("fMyRunYear", &fMyRunYear);
	//treeMC->SetBranchAddress("fposEX", &fposEX);
	//treeMC->SetBranchAddress("fposEY", &fposEY);
	treeMC->SetBranchAddress("weight1", &weight1);
	treeMC->SetBranchAddress("weight2", &weight2);
	treeMC->SetBranchAddress("s61", &s61);
	treeMC->SetBranchAddress("s1", &s1);
	treeMC->SetBranchAddress("s74", &s74);
	treeMC->SetBranchAddress("s75", &s75);
	treeMC->SetBranchAddress("s67", &s67);
	/*treeMC->SetBranchAddress("beamCharge", &beamCharge);
	treeMC->SetBranchAddress("TrType", &TrType);
	treeMC->SetBranchAddress("TrackCheck", &TrackCheck);
	treeMC->SetBranchAddress("measuredCharge", &measuredCharge);
	treeMC->SetBranchAddress("epT", &epT);*/


	
	vector<double> tempeffcalcnum = {};
	vector<double> tempeffcalcdenom = {};
	vector<double> tempeff = {};
	vector<double> tempeffcalcnumMC = {};
	vector<double> tempeffcalcdenomMC = {};
	vector<double> tempeffMC = {};
	vector<double> ratio = {};
	vector<double> ratiodata = {};
	vector<double> X = {};
	vector<double> xerr = {};
	vector<double> yerr = {};
	vector<int> triggervector = {0,0};
	vector<double> varvector = {0,0};
	

	for (int d = 0; d < binentries; d++) {
		tempeffcalcnum.push_back(0);
		tempeffcalcdenom.push_back(0);
		tempeff.push_back(0);
		tempeffcalcnumMC.push_back(0);
		tempeffcalcdenomMC.push_back(0);
		tempeffMC.push_back(0);
		xerr.push_back(abs(binsvector[d + 1] - binsvector[d]) / 2.0);
		X.push_back(binsvector[d] + xerr[d]);
		yerr.push_back(0);
		ratio.push_back(0);
		ratiodata.push_back(0);
	}

	//Loop over Data Tree Entries
	int m;
	int j;
	int entries;//get number of entries in the Event Tree we just loaded in
	//entries = DataEventTree->GetEntries();
	entries = tree->GetEntries();
	for (m = 0; m < entries; m++) {
		tree->GetEntry(m);
		for (j = 0; j < binentries; j++) {
			if (Ys > 0.1 && Ys < 0.6) {
				if (Q2s > 5 && Q2s < 100) {
					varvector = { Q2s, x };
					if (varvector[idxvar] > binsvector[j] && varvector[idxvar] < binsvector[j + 1]) {
						//cout << "here" << endl;
						triggervector = {s1, s74};
						if (triggervector[idx] > 0) {
							tempeffcalcdenom[j] = tempeffcalcdenom[j] + weight1 * weight2;
						}
						if (s61 > 0 && triggervector[idx] > 0) {
							tempeffcalcnum[j] = tempeffcalcnum[j] + weight1 * weight2;
						}
					}
				}
			}
		}
	}

	// Loop over MC Tree Entries
	m=0;
	j=0;
	entries=0;//get number of entries in the Event Tree we just loaded in
	//entries = DataEventTree->GetEntries();
	entries = treeMC->GetEntries();
	for (m = 0; m < entries; m++) {
		treeMC->GetEntry(m);
		for (j = 0; j < binentries; j++) {
			if (Ys > 0.1 && Ys < 0.6) {
				if (Q2s > 5 && Q2s < 100) {
					varvector = { Q2s, x };
					if (varvector[idxvar] > binsvector[j] && varvector[idxvar] < binsvector[j + 1]) {
						triggervector = { s1, s74 };
						if (triggervector[idx] > 0) {
							tempeffcalcdenomMC[j] = tempeffcalcdenomMC[j] + weight1 * weight2;
						}
						if (s61 > 0 && triggervector[idx] > 0) {
							tempeffcalcnumMC[j] = tempeffcalcnumMC[j] + weight1 * weight2;
						}
					}
				}
			}
		}
	}
	
	//tempeff = (tempeffcalcnum) / (tempeffcalcdenom);
	for (int g = 0; g < binentries; g++) {
		tempeff[g] = (tempeffcalcnum[g])/(tempeffcalcdenom[g]);
		tempeffMC[g] = (tempeffcalcnumMC[g]) / (tempeffcalcdenomMC[g]);
		cout << "Bin " << g + 1 << ": " << tempeffcalcnum[g] << endl;
		cout << "Bin " << g + 1 << ": " << tempeffcalcdenom[g] << endl;
		ratio[g] = (tempeffMC[g])/(tempeff[g]);
		ratiodata[g] = (tempeff[g]) / (tempeff[g]);
	}

	TCanvas* canvas = new TCanvas(plotcanvasname.c_str(), plotcanvasname.c_str(), 10, 10, 800, 400);
	auto dataplot = new TGraphErrors(binentries, X.data(), tempeff.data(), xerr.data(), yerr.data());
	auto MCplot = new TGraphErrors(binentries, X.data(), tempeffMC.data(), xerr.data(), yerr.data());
	auto ratioplot = new TGraphErrors(binentries, X.data(), ratio.data(), xerr.data(), yerr.data());
	auto ratioplot2 = new TGraphErrors(binentries, X.data(), ratiodata.data(), xerr.data(), yerr.data());

	TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	legend->AddEntry(dataplot, "2005-2007 Data", "l");
	legend->AddEntry(MCplot, "2005-2007 RAPGAP", "l");


	dataplot->SetLineColor(kBlack);
	MCplot->SetLineColor(kRed);
	ratioplot->SetLineColor(kRed);
	ratioplot2->SetLineColor(kBlack);

	TMultiGraph* mg = new TMultiGraph();
	TMultiGraph* mgratio = new TMultiGraph();
	mg->SetTitle(plottitle.c_str());
	mg->GetXaxis()->SetTitle(xaxistitle.c_str());
	mg->GetYaxis()->SetTitle(yaxistitle.c_str());
	mgratio->SetTitle(" ");
	mgratio->GetXaxis()->SetTitle(xaxistitle.c_str());
	mgratio->GetYaxis()->SetTitle("MC/Data");

	/*mg->GetYaxis()->SetTitleFont(43);
	mg->GetYaxis()->SetTitleSize(20);
	mg->GetYaxis()->SetLabelFont(43);
	mg->GetYaxis()->SetLabelSize(15);
	mg->GetYaxis()->SetTitleOffset(1.55);*/

	


	mg->Add(dataplot,"AP");
	mg->Add(MCplot, "AP");
	mgratio->Add(ratioplot, "AP");
	mgratio->Add(ratioplot2, "AP");
	
	//
	//TPad* pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
	//canvas->cd();
	//pad1->SetBottomMargin(0.01);
	//pad1->SetGridx();
	//pad1->Draw();
	//pad1->cd();
	//

	mg->Draw("AP");
	//MCplot->Draw("SAME");
	legend->Draw("SAME");
	

	//
	//TPad* pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
	//canvas->cd();
	//pad2->SetTopMargin(0.03);
	//pad2->SetBottomMargin(0.4);
	////pad2->SetGridx();
	////mgratio->GetYaxis()->SetRangeUser(0, 2);
	//mgratio->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	//mgratio->GetXaxis()->SetLabelSize(15);
	//mgratio->GetYaxis()->SetLabelFont(43);
	//mgratio->GetYaxis()->SetLabelSize(15);
	//mgratio->GetXaxis()->SetTitleFont(43);
	//mgratio->GetXaxis()->SetTitleSize(20);
	//mgratio->GetXaxis()->SetTitleOffset(4.55);
	//pad2->Draw();
	//pad2->cd();
	//mgratio->Draw("AP");
	//
	
	Output->cd();
	canvas->Write();

	vector<vector<double>> out;
	out.push_back(tempeff);
	out.push_back(tempeffMC);
	return out;

}

void TriggerEff() {
	//create file to output plots to
	TFile* outputfile = new TFile(outputfilename.c_str(), "RECREATE");
	
	TFile* fileData = new TFile(fileDataTree.c_str(), "read");
	TFile* fileRAPGAP = new TFile(fileRAPGAPTree.c_str(), "read");
	TFile* fileDJANGOH = new TFile(fileDJANGOHTree.c_str(), "read");

	//Getting Event Tree from the Data file
	//TTree* DataEventTree = (TTree*)fileData->Get("Data Event Tree");
	//TTree* RAPGAPEventTree = (TTree*)fileRAPGAP->Get("Data Event Tree");
	//TTree* DJANGOHEventTree = (TTree*)fileDJANGOH->Get("Data Event Tree");
	//TTree* PYTHIAEventTree = (TTree*)PYTHIA->Get("Data Event Tree");


	vector<vector<double>> value = AnalyzeTree(1, 0, "S74 vs Q2","Monitoring S61 with S74","Q^{2} [GeV]","#frac{N_{MT&&TE}}{N_{MT}}", fileData, fileRAPGAP, outputfile, "Lambda Tot", {5.0, 7.0, 10.0, 15.0, 25.0, 100.0}, 5);
	
	vector<vector<double>> value2 = AnalyzeTree(0, 0, "S1 vs Q2", "Monitoring S61 with S01", "Q^{2} [GeV]", "#frac{N_{MT&&TE}}{N_{MT}}", fileData, fileRAPGAP, outputfile, "Lambda Tot", { 5.0, 7.0, 10.0, 15.0, 25.0, 100.0 }, 5);

	//vector<vector<double>> value = AnalyzeTree(1, 1, "S74 vs x", "Monitoring S61 with S74", "x [GeV]", "#frac{N_{MT&&TE}}{N_{MT}}", fileData, fileRAPGAP, outputfile, "Lambda Tot", { 0.0001, 0.0002, 0.0004, 0.001, 0.01 }, 4);
	
	//vector<vector<double>> value2 = AnalyzeTree(0, 1, "S1 vs x", "Monitoring S61 with S01", "x", "#frac{N_{MT&&TE}}{N_{MT}}", fileData, fileRAPGAP, outputfile, "Lambda Tot", { 0.0001, 0.0002, 0.0004, 0.001, 0.01 }, 4);
	
	//vector<vector<double>> value = AnalyzeTree(1, 1, "S74 vs x", "Monitoring S61 with S74", "x [GeV]", "#frac{N_{MT&&TE}}{N_{MT}}", fileData, fileRAPGAP, outputfile, "Data Event Tree", { 0.0001, 0.0002, 0.0004, 0.001, 0.01 }, 4);

	//vector<vector<double>> value2 = AnalyzeTree(0, 1, "S1 vs x", "Monitoring S61 with S01", "x", "#frac{N_{MT&&TE}}{N_{MT}}", fileData, fileRAPGAP, outputfile, "Data Event Tree", { 0.0001, 0.0002, 0.0004, 0.001, 0.01 }, 4);

	/*for (int d = 0; d < 5; d++) {
		cout << "Data Q2 Bin " << d + 1 << " : " << value[0][d] << endl;
		cout << "MC Q2 Bin " << d + 1 << " : " << value[1][d] << endl;
	}*/
	
	outputfile->Write();
	outputfile->Close();

	cout << "Done" << endl;
}