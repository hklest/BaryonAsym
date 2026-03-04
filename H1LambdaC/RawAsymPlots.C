//Analysis Code Root Macro 
//Importing TTrees
#include <iostream>
#include <stdlib.h>
#include <cmath>
using namespace std;



string fileLambdaTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/merged.root";

string outputfilename = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/RawAsymPlots.root";


// Histograms Binning
Double_t lambdabins = 1000;
Double_t lambdaupperbinrange = 1.5;
Double_t lambdalowerbinrange = 1.0;
Double_t kaonbins = 500;
Double_t kaonupperbinrange = 1.0;
Double_t kaonlowerbinrange = 0.0;






void RawAsymPlots() {
	
	//Canvas
	TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 800);
	
	// Histograms Binning, have some histograms filled directly from analysis that I use as a consistency check with the histograms I make from my Analysis code using the TTrees I create here
	Double_t lambdabins = 1000;
	Double_t lambdaupperbinrange = 1.5;
	Double_t lambdalowerbinrange = 1.0;
	double lowerlambdabin = 1.1;
	double upperlambdabin = 1.132;

	

	TH1F* LambdaRegHist = new TH1F("Lambda Reg", "Lambda Reg", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
	TH1F* LambdaBarHist = new TH1F("Lambda Bar", "Lambda Bar", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
	TH1F* LambdaOverlapHist = new TH1F("Lambda", "Lambda", lambdabins, lambdalowerbinrange, lambdaupperbinrange);

	TH1F* LambdaRegRadDecayVtxHist = new TH1F("Lambda Reg Rad Decay Vtx", "Lambda Reg Rad Decay Vtx", 30, 0, 40);
	TH1F* LambdaBarRadDecayVtxHist = new TH1F("Lambda Bar Rad Decay Vtx", "Lambda Bar Rad Decay Vtx", 30, 0, 40);

	TH1F* LambdaRegZDecayVtxHist = new TH1F("Lambda Reg Z Decay Vtx", "Lambda Reg Decay Z Vtx", 20, -40, 40);
	TH1F* LambdaBarZDecayVtxHist = new TH1F("Lambda Bar Z Decay Vtx", "Lambda Bar Decay Z Vtx", 20, -40, 40);

	//TH1F* LambdaRegPhiHist = new TH1F("Lambda Reg Phi", "Lambda Reg Phi", 10, -3.14159, 3.14159);
	//TH1F* LambdaBarPhiHist = new TH1F("Lambda Bar Phi", "Lambda Bar Phi", 10, -3.14159, 3.14159);

	TH1F* LambdaRegPhiHist = new TH1F("Lambda Reg Phi", "Lambda Reg Phi", 10, -3.14159, 3.14159);
	TH1F* LambdaBarPhiHist = new TH1F("Lambda Bar Phi", "Lambda Bar Phi", 10, -3.14159, 3.14159);

	TH1F* LambdaRegystarHist = new TH1F("Lambda Reg ystar", "Lambda Reg ystar", 40, 0, 5);
	TH1F* LambdaBarystarHist = new TH1F("Lambda Bar ystar", "Lambda Bar ystar", 40, 0, 5);

	TH2F* LambdaRegXYDecayVertex = new TH2F("Lambda Reg Decay Vertex", "Lambda Reg Decay Vertex", 100, -40, 40, 100, -40, 40);
	TH2F* LambdaBarXYDecayVertex = new TH2F("Lambda Bar Decay Vertex", "Lambda Bar Decay Vertex", 100, -40, 40, 100, -40, 40);
	TH2F* LambdaTotXYDecayVertex = new TH2F("Lambda Tot Decay Vertex", "Lambda Tot Decay Vertex", 100, -40, 40, 100, -40, 40);

	//Data File
	TFile* Data = new TFile(fileLambdaTree.c_str(), "read");
	//Getting Event Tree from the Data file
	
	TTree* LambdaRegTree = (TTree*)Data->Get("Lambda Reg");
	TTree* LambdaBarTree = (TTree*)Data->Get("Lambda Bar");
	
	

	//Define the variables and banch addresses for data we want from the Event Tree entries
	double pT, Eta, x, invMasslambda, Q2s, Ys, Angle, v0chi2, weight1, weight2, SVx, SVy, SVz, PVx, PVy, PVz, LambdaPX, LambdaPY, LambdaPZ, ystar;

	LambdaRegTree->SetBranchAddress("invMasslambda", &invMasslambda);
	LambdaRegTree->SetBranchAddress("pT", &pT);
	LambdaRegTree->SetBranchAddress("Eta", &Eta);
	LambdaRegTree->SetBranchAddress("x", &x);
	LambdaRegTree->SetBranchAddress("Q2s", &Q2s);
	LambdaRegTree->SetBranchAddress("Ys", &Ys);
	LambdaRegTree->SetBranchAddress("Angle", &Angle);
	LambdaRegTree->SetBranchAddress("v0chi2", &v0chi2);
	LambdaRegTree->SetBranchAddress("weight1", &weight1);
	LambdaRegTree->SetBranchAddress("weight2", &weight2);
	LambdaRegTree->SetBranchAddress("SVx", &SVx);
	LambdaRegTree->SetBranchAddress("SVy", &SVy);
	LambdaRegTree->SetBranchAddress("SVz", &SVz);
	LambdaRegTree->SetBranchAddress("PVx", &PVx);
	LambdaRegTree->SetBranchAddress("PVy", &PVy);
	LambdaRegTree->SetBranchAddress("PVz", &PVz);
	LambdaRegTree->SetBranchAddress("LambdaPX", &LambdaPX);
	LambdaRegTree->SetBranchAddress("LambdaPY", &LambdaPY);
	LambdaRegTree->SetBranchAddress("LambdaPZ", &LambdaPZ);
	LambdaRegTree->SetBranchAddress("ystar", &ystar);

	

	LambdaBarTree->SetBranchAddress("invMasslambda", &invMasslambda);
	LambdaBarTree->SetBranchAddress("pT", &pT);
	LambdaBarTree->SetBranchAddress("Eta", &Eta);
	LambdaBarTree->SetBranchAddress("x", &x);
	LambdaBarTree->SetBranchAddress("Q2s", &Q2s);
	LambdaBarTree->SetBranchAddress("Ys", &Ys);
	LambdaBarTree->SetBranchAddress("Angle", &Angle);
	LambdaBarTree->SetBranchAddress("v0chi2", &v0chi2);
	LambdaBarTree->SetBranchAddress("weight1", &weight1);
	LambdaBarTree->SetBranchAddress("weight2", &weight2);
	LambdaBarTree->SetBranchAddress("SVx", &SVx);
	LambdaBarTree->SetBranchAddress("SVy", &SVy);
	LambdaBarTree->SetBranchAddress("SVz", &SVz);
	LambdaBarTree->SetBranchAddress("PVx", &PVx);
	LambdaBarTree->SetBranchAddress("PVy", &PVy);
	LambdaBarTree->SetBranchAddress("PVz", &PVz);
	LambdaBarTree->SetBranchAddress("LambdaPX", &LambdaPX);
	LambdaBarTree->SetBranchAddress("LambdaPY", &LambdaPY);
	LambdaBarTree->SetBranchAddress("LambdaPZ", &LambdaPZ);
	LambdaBarTree->SetBranchAddress("ystar", &ystar);
	

	//Loop over Lambda Reg Tree Entries
	int m;
	int entries;//get number of entries in the Event Tree we just loaded in
	entries = LambdaRegTree->GetEntries();
	double redundanteventcheck=0;
	double eventregcount = 0;
	double eventbarcount = 0;
	double eventregcountcheck = 0;
	double eventbarcountcheck = 0;
	for (m = 0; m < entries; m++) {
		LambdaRegTree->GetEntry(m);
		
		if (invMasslambda > lowerlambdabin && invMasslambda < upperlambdabin) {
			LambdaRegHist->Fill(invMasslambda, weight1 * weight2);
			//LambdaRegHist->Fill(invMasslambda);
			LambdaRegRadDecayVtxHist->Fill(sqrt(SVx * SVx + SVy * SVy), weight1 * weight2);
			LambdaRegZDecayVtxHist->Fill(SVz, weight1 * weight2);
			LambdaRegXYDecayVertex->Fill(SVx, SVy, weight1 * weight2);
			LambdaTotXYDecayVertex->Fill(SVx, SVy, weight1 * weight2);
			LambdaRegPhiHist->Fill(atan2(LambdaPY,LambdaPX), weight1 * weight2);
			LambdaRegystarHist->Fill(ystar, weight1 * weight2);
			if (redundanteventcheck != Q2s) { eventregcount++; }
			redundanteventcheck = Q2s;
			eventregcountcheck++;
		}
	}

	redundanteventcheck = 0;

	//Loop over Lambda Bar Tree Entries
	int n;
	//int entries;//get number of entries in the Event Tree we just loaded in
	entries = LambdaBarTree->GetEntries();
	
	for (n = 0; n < entries; n++) {
		LambdaBarTree->GetEntry(n);

		if (invMasslambda > lowerlambdabin && invMasslambda < upperlambdabin) {
			LambdaBarHist->Fill(invMasslambda, weight1 * weight2);
			//LambdaBarHist->Fill(invMasslambda);
			LambdaBarRadDecayVtxHist->Fill(sqrt(SVx * SVx + SVy * SVy), weight1 * weight2);
			LambdaBarZDecayVtxHist->Fill(SVz, weight1 * weight2);
			LambdaBarXYDecayVertex->Fill(SVx, SVy, weight1 * weight2);
			LambdaTotXYDecayVertex->Fill(SVx, SVy, weight1 * weight2);
			LambdaBarPhiHist->Fill(atan2(LambdaPY,LambdaPX), weight1 * weight2);
			LambdaBarystarHist->Fill(ystar, weight1 * weight2);
			if (redundanteventcheck != Q2s) { eventbarcount++; }
			redundanteventcheck = Q2s;
			eventbarcountcheck++;
		}
	}

	cout << "Lambda Reg Events: " << eventregcount << " Check: " << eventregcountcheck << endl;

	cout << "Lambda Bar Events: " << eventbarcount << " Check: " << eventbarcountcheck << endl;

	//Got All my data and filled Hists, so close data file
	//Data->Close();
	
	//Placeholder Asymm calculator hists
	TH1F* AsymNum = new TH1F();
	TH1F* AsymDenom = new TH1F();
	//Asymm plots
	TH1F* AsymRadDecayVertex = new TH1F();
	TH1F* AsymZDecayVertex = new TH1F();
	TH1F* AsymPhi = new TH1F();
	TH1F* NetLambdaystar = new TH1F();

	*AsymNum = *LambdaRegRadDecayVtxHist - *LambdaBarRadDecayVtxHist;
	*AsymDenom = *LambdaRegRadDecayVtxHist + *LambdaBarRadDecayVtxHist;
	*AsymRadDecayVertex = (*AsymNum) / (*AsymDenom);

	//Reset placeholder hists for next asymm calc
	AsymNum->Reset("ICESM");
	AsymDenom->Reset("ICESM");

	*AsymNum = *LambdaRegZDecayVtxHist - *LambdaBarZDecayVtxHist;
	*AsymDenom = *LambdaRegZDecayVtxHist + *LambdaBarZDecayVtxHist;
	*AsymZDecayVertex = (*AsymNum) / (*AsymDenom);

	//Reset placeholder hists for next asymm calc
	AsymNum->Reset("ICESM");
	AsymDenom->Reset("ICESM");

	*AsymNum = *LambdaRegPhiHist - *LambdaBarPhiHist;
	*AsymDenom = *LambdaRegPhiHist + *LambdaBarPhiHist;
	*AsymPhi = (*AsymNum) / (*AsymDenom);

	//Reset placeholder hists for next asymm calc
	AsymNum->Reset("ICESM");
	AsymDenom->Reset("ICESM");

	TH1F* LambdaRegystarHistNormalized = (TH1F*)LambdaRegystarHist->Clone();
	TH1F* LambdaBarystarHistNormalized = (TH1F*)LambdaBarystarHist->Clone();

	LambdaRegystarHistNormalized->Scale(1/eventregcount);
	LambdaBarystarHistNormalized->Scale(1/eventregcount);

    *NetLambdaystar = *LambdaRegystarHistNormalized - *LambdaBarystarHistNormalized;
	//*NetLambdaystar = *LambdaRegystarHist - *LambdaBarystarHist;

	//create file to output plots to
	TFile outputfile(outputfilename.c_str(), "RECREATE");

	//NetLambda ystar plots
	canvas->SetName("Lambda Reg ystar Dist");
	LambdaRegystarHist->Draw("hist");
	canvas->Write();

	canvas->SetName("Lambda Bar ystar Dist");
	LambdaBarystarHist->Draw("hist");
	canvas->Write();

	canvas->SetName("Lambda Reg ystar Normalized Dist");
	LambdaRegystarHistNormalized->Draw("hist");
	canvas->Write();

	canvas->SetName("Lambda Bar ystar Normalized Dist");
	LambdaBarystarHistNormalized->Draw("hist");
	canvas->Write();

	TF1* f1 = new TF1("f1", "[0]*exp(-[1]*x)", 2.445, 4.8);
	f1->SetParameter(0.1, 0.1);
	//f1->SetParameter(0.1);
	NetLambdaystar->Fit("f1","R");

	canvas->SetName("Net Lambda vs y*");
	NetLambdaystar->SetName("Net Lambda vs y*");
	NetLambdaystar->SetTitle("Net Lambda vs y*");
	NetLambdaystar->GetYaxis()->SetTitle("(1/N) (dN_Lambda - dN_LambdaBar)/(dy*)");
	NetLambdaystar->GetXaxis()->SetTitle("y*");
	//NetLambdaystar->SetStats(0);
	NetLambdaystar->Draw();
	f1->Draw("same");
	canvas->Write();


	//Draw Histograms and write to output file
	canvas->SetName("Lambda Reg XY Decay Vertex");
	canvas->SetTitle("#Lambda XY Vertices");
	LambdaRegXYDecayVertex->SetTitle("#Lambda XY Vertices");
	LambdaRegXYDecayVertex->GetYaxis()->SetTitle("Y");
	LambdaRegXYDecayVertex->GetXaxis()->SetTitle("X");
	LambdaRegXYDecayVertex->SetStats(0);
	LambdaRegXYDecayVertex->Draw("colz");
	canvas->Write();

	canvas->SetName("Lambda Bar XY Decay Vertex");
	canvas->SetTitle("#bar{#Lambda} XY Vertices");
	LambdaBarXYDecayVertex->SetTitle("#bar{#Lambda} XY Vertices");
	LambdaBarXYDecayVertex->GetYaxis()->SetTitle("Y");
	LambdaBarXYDecayVertex->GetXaxis()->SetTitle("X");
	LambdaBarXYDecayVertex->SetStats(0);
	LambdaBarXYDecayVertex->Draw("colz");
	canvas->Write();

	canvas->SetName("Lambda Tot XY Decay Vertex");
	LambdaTotXYDecayVertex->Draw("colz");
	canvas->Write();

	canvas->SetName("Lambda Peak");
	LambdaRegHist->SetName("#Lambda");
	//LambdaRegHist->SetTitle("#Lambda");
	LambdaRegHist->GetXaxis()->SetTitle("Mass [GeV]");
	LambdaRegHist->GetYaxis()->SetTitle("Count");
	LambdaRegHist->Draw("hist");
	canvas->Write();

	canvas->SetName("Lambda Bar Peak");
	LambdaBarHist->SetName("#bar{#Lambda}");
	//LambdaBarHist->SetTitle("#bar{#Lambda}");
	LambdaBarHist->GetXaxis()->SetTitle("Mass [GeV]");
	LambdaBarHist->GetYaxis()->SetTitle("Count");
	LambdaBarHist->Draw("hist");
	canvas->Write();

	canvas->SetName("Lambda and Lambda Bar Peaks");
	LambdaRegHist->SetLineColor(kRed);
	LambdaBarHist->SetLineColor(kBlack);
	LambdaRegHist->Draw();
	LambdaBarHist->Draw("same");
	TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	//legend->SetHeader(Form("#splitline{Count- %.2f #pm %.2f (stat.)}{Stochastic MLP- %s Network Trained %ix}", kcount, kcounterror, network[particlecode].c_str(), trainnum[particlecode])); // option "C" allows to center the header
	legend->AddEntry(LambdaRegHist, "#Lambda", "l");
	legend->AddEntry(LambdaBarHist, "#bar{#Lambda}", "l");
	legend->Draw("same");
	canvas->Write();
	legend->DeleteEntry();
	legend->DeleteEntry();

	canvas->SetName("Lambda Reg Rad Vtx Dist");
	LambdaRegRadDecayVtxHist->Draw("hist");
	canvas->Write();

	canvas->SetName("Lambda Bar Rad Vtx Dist");
	LambdaBarRadDecayVtxHist->Draw("hist");
	canvas->Write();

	canvas->SetName("Lambda Reg Z Vtx Dist");
	LambdaRegZDecayVtxHist->Draw("hist");
	canvas->Write();

	canvas->SetName("Lambda Bar Z Vtx Dist");
	LambdaBarZDecayVtxHist->Draw("hist");
	canvas->Write();

	canvas->SetName("Lambda Phi Dist");
	LambdaRegPhiHist->SetName("#Lambda Dist. in #Phi");
	LambdaRegPhiHist->SetTitle("#Lambda Dist. in #Phi");
	LambdaRegPhiHist->GetXaxis()->SetTitle("#Phi");
	LambdaRegPhiHist->GetYaxis()->SetTitle("Count");
	LambdaRegPhiHist->Draw("E");
	canvas->SetName("Lambda Phi Dist");
	canvas->Write();

	canvas->SetName("Lambda Bar Phi Dist");
	LambdaBarPhiHist->SetName("#bar{#Lambda} Dist. in #Phi");
	LambdaBarPhiHist->SetTitle("#bar{#Lambda} Dist. in #Phi");
	LambdaBarPhiHist->GetXaxis()->SetTitle("#Phi");
	LambdaBarPhiHist->GetYaxis()->SetTitle("Count");
	LambdaBarPhiHist->Draw("E");
	canvas->SetName("Lambda Bar Phi Dist");
	canvas->Write();


	THStack* phistack = new THStack("Lambda and Lambda Bar vs Phi", "Lambda and Lambda Bar vs Phi");
	canvas->SetName("Lambda and Lambda Bar vs Phi");
	LambdaRegPhiHist->SetLineColor(kRed);
	LambdaBarPhiHist->SetLineColor(kBlack);
	LambdaRegPhiHist->SetOption("E");
	LambdaBarPhiHist->SetOption("E");
	phistack->Add(LambdaRegPhiHist);
	phistack->Add(LambdaBarPhiHist);
	phistack->Draw("nostack");
	phistack->GetXaxis()->SetTitle("#Phi");
	phistack->GetYaxis()->SetTitle("Count");
	//TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	legend->AddEntry(LambdaRegHist, "#Lambda Dist. in #Phi", "l");
	legend->AddEntry(LambdaBarHist, "#bar{#Lambda} Dist. in #Phi", "l");
	legend->Draw("same");
	canvas->Write();
	legend->DeleteEntry();
	legend->DeleteEntry();

	THStack* radstack = new THStack("Lambda and Lambda Bar vs Rad Decay", "Lambda and Lambda Bar vs Rad Decay");
	canvas->SetName("Lambda and Lambda Bar vs Rad Decay");
	LambdaRegRadDecayVtxHist->SetLineColor(kRed);
	LambdaBarRadDecayVtxHist->SetLineColor(kBlack);
	LambdaRegRadDecayVtxHist->SetOption("E");
	LambdaBarRadDecayVtxHist->SetOption("E");
	radstack->Add(LambdaRegRadDecayVtxHist);
	radstack->Add(LambdaBarRadDecayVtxHist);
	radstack->Draw("nostack");
	radstack->GetXaxis()->SetTitle("r_{Decay Vtx}");
	radstack->GetYaxis()->SetTitle("Count");
	//TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	legend->AddEntry(LambdaRegRadDecayVtxHist, "#Lambda Dist. in r_{Decay Vtx}", "l");
	legend->AddEntry(LambdaBarRadDecayVtxHist, "#bar{#Lambda} Dist. in r_{Decay Vtx}", "l");
	legend->Draw("same");
	canvas->Write();
	legend->DeleteEntry();
	legend->DeleteEntry();

	THStack* Zstack = new THStack("Lambda and Lambda Bar vs Z Decay", "Lambda and Lambda Bar vs Z Decay");
	canvas->SetName("Lambda and Lambda Bar vs Z Decay");
	LambdaRegZDecayVtxHist->SetLineColor(kRed);
	LambdaBarZDecayVtxHist->SetLineColor(kBlack);
	LambdaRegZDecayVtxHist->SetOption("E");
	LambdaBarZDecayVtxHist->SetOption("E");
	Zstack->Add(LambdaRegZDecayVtxHist);
	Zstack->Add(LambdaBarZDecayVtxHist);
	Zstack->Draw("nostack");
	Zstack->GetXaxis()->SetTitle("Z_{Decay Vtx}");
	Zstack->GetYaxis()->SetTitle("Count");
	//TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	legend->AddEntry(LambdaRegZDecayVtxHist, "#Lambda Dist. in Z_{Decay Vtx}", "l");
	legend->AddEntry(LambdaBarZDecayVtxHist, "#bar{#Lambda} Dist. in Z_{Decay Vtx}", "l");
	legend->Draw("same");
	canvas->Write();
	legend->DeleteEntry();
	legend->DeleteEntry();
	

	

	/*canvas->SetName("AsymNum");
	AsymNum->SetName("Lambda - Lambda Bar");
	AsymNum->SetTitle("Lambda - Lambda Bar");
	AsymNum->Draw("hist");
	canvas->Write();

	canvas->SetName("AsymDenom");
	AsymDenom->SetName("Lambda + Lambda Bar");
	AsymDenom->SetTitle("Lambda + Lambda Bar");
	AsymDenom->Draw("hist");
	canvas->Write();*/

	

	canvas->SetName("AsymRadDecayVertex");
	AsymRadDecayVertex->SetName("Raw A_{#Lambda} = #frac{#Lambda - #bar{#Lambda}}{#Lambda + #bar{#Lambda}}");
	AsymRadDecayVertex->SetStats(0);
	AsymRadDecayVertex->SetTitle("Raw A_{#Lambda} = #frac{#Lambda - #bar{#Lambda}}{#Lambda + #bar{#Lambda}}");
	AsymRadDecayVertex->GetXaxis()->SetTitle("r_{Decay Vtx}");
	AsymRadDecayVertex->GetYaxis()->SetTitle("Raw A_{#Lambda}");
	AsymRadDecayVertex->Draw("E");
	canvas->Write();


	canvas->SetName("AsymZDecayVertex");
	AsymZDecayVertex->SetName("Raw A_{#Lambda} = #frac{#Lambda - #bar{#Lambda}}{#Lambda + #bar{#Lambda}}");
	AsymZDecayVertex->SetStats(0);
	AsymZDecayVertex->SetTitle("Raw A_{#Lambda} = #frac{#Lambda - #bar{#Lambda}}{#Lambda + #bar{#Lambda}}");
	AsymZDecayVertex->GetXaxis()->SetTitle("Z_{Decay Vtx}");
	AsymZDecayVertex->GetYaxis()->SetTitle("Raw A_{#Lambda}");
	AsymZDecayVertex->Draw("E");
	canvas->Write();


	canvas->SetName("AsymPhi");
	AsymPhi->SetName("Raw A_{#Lambda} = #frac{#Lambda - #bar{#Lambda}}{#Lambda + #bar{#Lambda}}");
	AsymPhi->SetStats(0);
	AsymPhi->SetTitle("Raw A_{#Lambda} = #frac{#Lambda - #bar{#Lambda}}{#Lambda + #bar{#Lambda}}");
	AsymPhi->GetXaxis()->SetTitle("#Phi");
	AsymPhi->GetYaxis()->SetTitle("Raw A_{#Lambda}");
	AsymPhi->Draw("E");
	canvas->Write();

	outputfile.Write();
	outputfile.Close();
	Data->Close();

	cout << "Done" << endl;

};