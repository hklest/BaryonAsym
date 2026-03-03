//Analysis Code Root Macro 
//Importing TTrees
#include <iostream>
#include <stdlib.h>
#include <cmath>
using namespace std;


//Change these file paths and output for different years
//string path2005 = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2005.root";
//string path2006 = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2006.root";
//string path2007 = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2007.root";
//string pathCombined = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_All.root";
//string fileRAPGAPTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/RAPGAP31/2006/merged.root";
//string fileDJANGOHTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/DJANGOH14/2006/merged.root";

string outputfilename = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/TotalResults.root";

int Plot(string path2005, string path2006, string path2007, string pathCombined, TFile* file, string plotcanvasname, string plottitle, string xaxistitle, string yaxistitle, string TGraphTitle) {
	//TCanvas* c5 = new TCanvas(plotcanvasname.c_str(), plotcanvasname.c_str(), 10, 10, 800, 400);
	TFile* file2005 = new TFile(path2005.c_str(), "read");
	TFile* file2006 = new TFile(path2006.c_str(), "read");
	TFile* file2007 = new TFile(path2007.c_str(), "read");
	TFile* fileCombined = new TFile(pathCombined.c_str(), "read");

	fileCombined->cd();
	TGraphErrors* AsymCombined = (TGraphErrors*)fileCombined->Get(TGraphTitle.c_str());
	file2005->cd();
	TGraphErrors* Asym2005 = (TGraphErrors*)file2005->Get(TGraphTitle.c_str());
	file2006->cd();
	TGraphErrors* Asym2006 = (TGraphErrors*)file2006->Get(TGraphTitle.c_str());
	file2007->cd();
	TGraphErrors* Asym2007 = (TGraphErrors*)file2007->Get(TGraphTitle.c_str());

	AsymCombined->SetMarkerSize(0.01);
	Asym2005->SetMarkerSize(0.01);
	Asym2006->SetMarkerSize(0.01);
	Asym2007->SetMarkerSize(0.01);

	AsymCombined->SetMarkerStyle(21);
	Asym2005->SetMarkerStyle(21);
	Asym2006->SetMarkerStyle(21);
	Asym2007->SetMarkerStyle(21);

	AsymCombined->SetMarkerColor(kRed);
	Asym2005->SetMarkerColor(kRed);
	Asym2006->SetMarkerColor(kBlue);
	Asym2007->SetMarkerColor(kGreen);

	AsymCombined->SetLineColor(kRed);
	Asym2005->SetLineColor(kRed);
	Asym2006->SetLineColor(kBlue);
	Asym2007->SetLineColor(kGreen);

	TMultiGraph* mgcs = new TMultiGraph();
	mgcs->Add(AsymCombined, "AP");
	//mgcs->Add(Asym2005, "AP");
	//mgcs->Add(Asym2006, "AP");
	//mgcs->Add(Asym2007, "AP");

	mgcs->SetTitle(plottitle.c_str());
	mgcs->GetXaxis()->SetTitle(xaxistitle.c_str());
	mgcs->GetYaxis()->SetTitle(yaxistitle.c_str());

	TLegend* legend2 = new TLegend();
	legend2->AddEntry(AsymCombined, "2005-2007 HERAII DST7", "l");
	//legend2->AddEntry(Asym2005, "2005 HERAII DST7", "l");
	//legend2->AddEntry(Asym2006, "2006 HERAII DST7", "l");
	//legend2->AddEntry(Asym2007, "2007 HERAII DST7", "l");

	TCanvas* c5 = new TCanvas(plotcanvasname.c_str(), plotcanvasname.c_str(), 10, 10, 800, 400);
	c5->SetGridy();
	mgcs->Draw("AP");
	legend2->Draw("SAME");


	file->cd();
	c5->Write();
	//mgcs->Write();

	file->Write();
	file2005->Close();
	file2006->Close();
	file2007->Close();
	fileCombined->Close();

	return 0;
}


void TotalResults() {
	//create file to output plots to
	TFile* outputfile = new TFile(outputfilename.c_str(), "RECREATE");
	
	
	//TFile* file2005 = new TFile(path2005.c_str(), "read");
	//TFile* file2006 = new TFile(path2006.c_str(), "read");
	//TFile* file2007 = new TFile(path2007.c_str(), "read");
	//TFile* fileCombined = new TFile(pathCombined.c_str(), "read");
	//
	////// Check if source file is open
	////if (!fileCombined || fileCombined->IsZombie()) {
	////	cout << "Error: Could not open source file" << endl;
	////}
	//fileCombined->cd();
	//TGraphErrors* AsymCombined = (TGraphErrors*)fileCombined->Get("Lambda Asymm vs q2 TGraph");
	//file2005->cd();
	//TGraphErrors* Asym2005 = (TGraphErrors*)file2005->Get("Lambda Asymm vs q2 TGraph");
	//file2006->cd();
	//TGraphErrors* Asym2006 = (TGraphErrors*)file2006->Get("Lambda Asymm vs q2 TGraph");
	//file2007->cd();
	//TGraphErrors* Asym2007 = (TGraphErrors*)file2007->Get("Lambda Asymm vs q2 TGraph");

	//AsymCombined->SetMarkerSize(0.01);
	//Asym2005->SetMarkerSize(0.01);
	//Asym2006->SetMarkerSize(0.01);
	//Asym2007->SetMarkerSize(0.01);

	//AsymCombined->SetMarkerStyle(21);
	//Asym2005->SetMarkerStyle(21);
	//Asym2006->SetMarkerStyle(21);
	//Asym2007->SetMarkerStyle(21);

	//AsymCombined->SetMarkerColor(kBlack);
	//Asym2005->SetMarkerColor(kRed);
	//Asym2006->SetMarkerColor(kBlue);
	//Asym2007->SetMarkerColor(kGreen);

	//AsymCombined->SetLineColor(kBlack);
	//Asym2005->SetLineColor(kRed);
	//Asym2006->SetLineColor(kBlue);
	//Asym2007->SetLineColor(kGreen);

	//TMultiGraph* mgcs = new TMultiGraph();
	//mgcs->Add(AsymCombined, "AP");
	//mgcs->Add(Asym2005, "AP");
	//mgcs->Add(Asym2006, "AP");
	//mgcs->Add(Asym2007, "AP");

	//mgcs->SetTitle("[#sigma(ep #rightarrow e #Lambda X) - #sigma(ep #rightarrow e #bar{#Lambda} X)] / [#sigma(ep #rightarrow e #Lambda X) + #sigma(ep #rightarrow e #bar{#Lambda} X)]");
	//mgcs->GetXaxis()->SetTitle("Q^{2}");
	//mgcs->GetYaxis()->SetTitle("A_{#Lambda}");

	//TLegend* legend2 = new TLegend();
	//legend2->AddEntry(AsymCombined, "2005-2007 HERAII DST7", "l");
	//legend2->AddEntry(Asym2005, "2005 HERAII DST7", "l");
	//legend2->AddEntry(Asym2006, "2006 HERAII DST7", "l");
	//legend2->AddEntry(Asym2007, "2007 HERAII DST7", "l");

	//TCanvas* c5 = new TCanvas("Test1", "Test2", 10, 10, 800, 400);

	//mgcs->Draw("AP");
	//legend2->Draw("SAME");

	////// Check if graph exists
	////if (!Asym2005) {
	////	cout << "Error: Could not find graph in source file" << endl;
	////}


	//outputfile->cd();
	//c5->Write();
	//mgcs->Write();

	//int Plot(outputfile, string plotcanvasname, string plottitle, string xaxistitle, string yaxistitle, string TGraphTitle) {
	//string path2005 = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2005.root";
	//string path2006 = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2006.root";
	//string path2007 = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2007.root";
	//string pathCombined = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_All.root";
	Plot("/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2005.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2006.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2007.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_All.root", outputfile,"Asym vs Q2", "[#sigma(ep #rightarrow e #Lambda X) - #sigma(ep #rightarrow e #bar{ #Lambda } X)] / [#sigma(ep #rightarrow e #Lambda X) + #sigma(ep #rightarrow e #bar{ #Lambda } X)]", "Q^{2}", "A_{#Lambda}", "Lambda Asymm vs q2 TGraph");
	Plot("/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2005.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2006.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2007.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_All.root", outputfile, "Lambda CS vs Q2", "d#sigma(ep #rightarrow e [#Lambda + #bar{#Lambda}] X)/dQ^{2}", "Q^{2}", "d#sigma/dQ^{2} [nb]", "Lambda Cross Section vs q2 TGraph");
	Plot("/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2005.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2006.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2007.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_All.root", outputfile, "K0 CS vs Q2", "d#sigma(ep #rightarrow e K^{0}_{S} X)/dQ^{2}", "Q^{2}", "d#sigma/dQ^{2} [nb]", "K0 Cross Section vs q2 TGraph");
	Plot("/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2005.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2006.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2007.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisQ2_All.root", outputfile, "Baryon:Meson vs Q2", "[d#sigma(ep #rightarrow e #Lambda X)]/[d#sigma(ep #rightarrow e K^{0}_{S} X)]", "Q^{2}", "#Lambda/K^{0}_{S}", "Lambda Over K0 Cross Section vs q2 TGraph");

	Plot("/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisX_2005.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisX_2006.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisX_2007.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisX_All.root", outputfile, "Asym vs x", "[#sigma(ep #rightarrow e #Lambda X) - #sigma(ep #rightarrow e #bar{ #Lambda } X)] / [#sigma(ep #rightarrow e #Lambda X) + #sigma(ep #rightarrow e #bar{ #Lambda } X)]", "x", "A_{#Lambda}", "Lambda Asymm vs q2 TGraph");
	Plot("/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisX_2005.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisX_2006.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisX_2007.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisX_All.root", outputfile, "Lambda CS vs x", "d#sigma(ep #rightarrow e [#Lambda + #bar{#Lambda}] X)/dx", "x", "d#sigma/dQ^{2} [nb]", "Lambda Cross Section vs q2 TGraph");
	Plot("/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisX_2005.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisX_2006.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisX_2007.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisX_All.root", outputfile, "K0 CS vs x", "d#sigma(ep #rightarrow e K^{0}_{S} X)/dx", "x", "d#sigma/dx [nb]", "K0 Cross Section vs q2 TGraph");
	Plot("/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisX_2005.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisX_2006.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisX_2007.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisX_All.root", outputfile, "Baryon:Meson vs x", "[d#sigma(ep #rightarrow e #Lambda X)]/[d#sigma(ep #rightarrow e K^{0}_{S} X)]", "x", "#Lambda/K^{0}_{S}", "Lambda Over K0 Cross Section vs q2 TGraph");

	Plot("/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysispT_2005.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysispT_2006.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysispT_2007.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysispT_All.root", outputfile, "Asym vs pT", "[#sigma(ep #rightarrow e #Lambda X) - #sigma(ep #rightarrow e #bar{ #Lambda } X)] / [#sigma(ep #rightarrow e #Lambda X) + #sigma(ep #rightarrow e #bar{ #Lambda } X)]", "p_{T}", "A_{#Lambda}", "Lambda Asymm vs q2 TGraph");
	Plot("/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysispT_2005.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysispT_2006.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysispT_2007.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysispT_All.root", outputfile, "Lambda CS vs pT", "d#sigma(ep #rightarrow e [#Lambda + #bar{#Lambda}] X)/dQ^{2}", "p_{T}", "d#sigma/dp_{T} [nb]", "Lambda Cross Section vs q2 TGraph");
	Plot("/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysispT_2005.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysispT_2006.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysispT_2007.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysispT_All.root", outputfile, "K0 CS vs pT", "d#sigma(ep #rightarrow e K^{0}_{S} X)/dp_{T}", "p_{T}", "d#sigma/dp_{T} [nb]", "K0 Cross Section vs q2 TGraph");
	Plot("/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysispT_2005.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysispT_2006.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysispT_2007.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysispT_All.root", outputfile, "Baryon:Meson vs pT", "[d#sigma(ep #rightarrow e #Lambda X)]/[d#sigma(ep #rightarrow e K^{0}_{S} X)]", "dp_{T}", "#Lambda/K^{0}_{S}", "Lambda Over K0 Cross Section vs q2 TGraph");

	Plot("/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisEta_2005.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisEta_2006.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisEta_2007.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisEta_All.root", outputfile, "Asym vs Eta", "[#sigma(ep #rightarrow e #Lambda X) - #sigma(ep #rightarrow e #bar{ #Lambda } X)] / [#sigma(ep #rightarrow e #Lambda X) + #sigma(ep #rightarrow e #bar{ #Lambda } X)]", "#eta", "A_{#Lambda}", "Lambda Asymm vs q2 TGraph");
	Plot("/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisEta_2005.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisEta_2006.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisEta_2007.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisEta_All.root", outputfile, "Lambda CS vs Eta", "d#sigma(ep #rightarrow e [#Lambda + #bar{#Lambda}] X)/d#Eta", "#Eta", "d#sigma/d#Eta [nb]", "Lambda Cross Section vs q2 TGraph");
	Plot("/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisEta_2005.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisEta_2006.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisEta_2007.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisEta_All.root", outputfile, "K0 CS vs Eta", "d#sigma(ep #rightarrow e K^{0}_{S} X)/d#Eta", "#Eta", "d#sigma/dp_{T} [nb]", "K0 Cross Section vs q2 TGraph");
	Plot("/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisEta_2005.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisEta_2006.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisEta_2007.root", "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisEta_All.root", outputfile, "Baryon:Meson vs Eta", "[d#sigma(ep #rightarrow e #Lambda X)]/[d#sigma(ep #rightarrow e K^{0}_{S} X)]", "#Eta", "#Lambda/K^{0}_{S}", "Lambda Over K0 Cross Section vs q2 TGraph");


	//outputfile->Write();
	outputfile->Close();

	cout << "Done" << endl;
}