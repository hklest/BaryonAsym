//Analysis Code Root Macro 
//Importing TTrees

using namespace std;

////////////////////////////////////////////////
// User Inputs in this section
////////////////////////////////////////////////

//Lambda and Kaon Extraction May require slightly different parameters
//Convecntion here: element[0] = Lambda ; element[1] = Kaon

vector<int> trainnum = { 100,10 }; //number of time NN is trained
vector<string> network = { "Mass:350:Count", "Mass:350:Count" }; //defining NN

//Vectors of fit subtraction values for lambda=0 and k0=1 particles,
//vector<double> leftfitbound1 = { 1.082, 0.40 };
//vector<double> leftfitbound2 = { 1.1, 0.435 };
//vector<double> rightfitbound1 = { 1.128, 0.555 };
//vector<double> rightfitbound2 = { 1.15, 0.68 };
//vector<double> leftsubbound = { 1.1, 0.44 };
//vector<double> rightsubbound = { 1.128, 0.55 };

vector<double> leftfitbound1 = { 1.082, 0.40 };
vector<double> leftfitbound2 = { 1.094, 0.425 };
vector<double> rightfitbound1 = { 1.132, 0.59 };
vector<double> rightfitbound2 = { 1.16, 0.68 };
vector<double> leftsubbound = { 1.094, 0.43 };
vector<double> rightsubbound = { 1.132, 0.58 };

//defining vectors of the binning definitions for each of the kinematic variables we are looking at
//when playing with binning, have to adjust these vectors and hand specify the number of bins for defining Hist arrays
vector<double> pTbins = {0.5, 0.6, 0.7, 0.8, 0.9, 1.1, 1.3, 1.6, 2.2, 3.5};
vector<double> Etabins = {-1.3, -1.0, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1.0, 1.3};
vector<double> xbins = {0.00004, 0.0001, 0.0002, 0.0004, 0.001, 0.01};
//vector<double> q2bins = {2.0, 2.5, 3.0, 4.0, 5.0, 7.0, 10.0, 15.0, 25.0, 100.0};
vector<double> q2bins = { 5.0, 7.0, 10.0, 15.0, 25.0, 100.0 };
const int pTentries = 9;
const int Etaentries = 10;
const int xentries = 5;
const int q2entries = 5;

string filelambdaTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/merged.root";
string filekaonTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/merged.root";

string filelambdaRAPGAPTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/merged.root";
string filekaonRAPGAPTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/merged.root";
string filelambdaDJANGOHTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/merged.root";
string filekaonDJANGOHTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/merged.root";

string outputfilename = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/CutStack.root";


// Histograms Binning
Double_t lambdabins = 1000;
Double_t lambdaupperbinrange = 1.5;
Double_t lambdalowerbinrange = 1.0;

Double_t lambdaCbins = 50;
Double_t lambdaCupperbinrange = 3.317;
Double_t lambdaClowerbinrange = 2.017;

Double_t kaonbins = 500;
Double_t kaonupperbinrange = 1.0;
Double_t kaonlowerbinrange = 0.0;






void CutStack() {
	//TFile* f = new TFile("pi0_pp200.root"); 				//read in your data file
	//TH1F* inHist = (TH1F*)f->Get("fg2d_ggmass_sameArm");   //read in the histogram of interest

	//inHist->AddDirectory(0);

	//TH1F* checkHist = (TH1F*)inHist->Clone();
	//Canvas
	TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 800);
	canvas->SetLeftMargin(0.15);
	//Data File
	TFile* Data = new TFile(filelambdaTree.c_str());
	//Getting Histograms
	TH1F* finalDataLambdaHist = (TH1F*)Data->Get("Lambda Cut 15");
	finalDataLambdaHist->SetLineColor(kBlack);



	TH1F* hist0 = (TH1F*)Data->Get("Lambda Cut 0");
	TH1F* hist1 = (TH1F*)Data->Get("Lambda Cut 1");
	TH1F* hist2 = (TH1F*)Data->Get("Lambda Cut 2");
	TH1F* hist3 = (TH1F*)Data->Get("Lambda Cut 3");
	TH1F* hist4 = (TH1F*)Data->Get("Lambda Cut 4");
	TH1F* hist5 = (TH1F*)Data->Get("Lambda Cut 5");
	TH1F* hist6 = (TH1F*)Data->Get("Lambda Cut 6");
	TH1F* hist7 = (TH1F*)Data->Get("Lambda Cut 7");
	TH1F* hist8 = (TH1F*)Data->Get("Lambda Cut 8");
	TH1F* hist9 = (TH1F*)Data->Get("Lambda Cut 9");
	TH1F* hist10 = (TH1F*)Data->Get("Lambda Cut 10");
	TH1F* hist11 = (TH1F*)Data->Get("Lambda Cut 11");
	TH1F* hist12 = (TH1F*)Data->Get("Lambda Cut 12");
	TH1F* hist13 = (TH1F*)Data->Get("Lambda Cut 13");
	TH1F* hist14 = (TH1F*)Data->Get("Lambda Cut 14");
	TH1F* hist15 = (TH1F*)Data->Get("Lambda Cut 15");

	TFile* DataKaon = new TFile(filekaonTree.c_str());
	TH1F* finalDataKaonHist = (TH1F*)DataKaon->Get("Kaon Cut 10");

	TH1F* histC0 = (TH1F*)DataKaon->Get("LambdaC Cut 0");
	TH1F* histC1 = (TH1F*)DataKaon->Get("LambdaC Cut 1");
	TH1F* histC2 = (TH1F*)DataKaon->Get("LambdaC Cut 2");
	TH1F* histC3 = (TH1F*)DataKaon->Get("LambdaC Cut 3");
	TH1F* histC4 = (TH1F*)DataKaon->Get("LambdaC Cut 4");
	TH1F* histC5 = (TH1F*)DataKaon->Get("LambdaC Cut 5");
	TH1F* histC6 = (TH1F*)DataKaon->Get("LambdaC Cut 6");
	TH1F* histC7 = (TH1F*)DataKaon->Get("LambdaC Cut 7");
	TH1F* histC8 = (TH1F*)DataKaon->Get("LambdaC Cut 8");
	TH1F* histC9 = (TH1F*)DataKaon->Get("LambdaC Cut 9");
	TH1F* histC10 = (TH1F*)DataKaon->Get("LambdaC Cut 10");
	TH1F* histC11 = (TH1F*)DataKaon->Get("LambdaC Cut 11");
	TH1F* histC12 = (TH1F*)DataKaon->Get("LambdaC Cut 12");
	TH1F* histC13 = (TH1F*)DataKaon->Get("LambdaC Cut 13");
	TH1F* histC14 = (TH1F*)DataKaon->Get("LambdaC Cut 14");
	TH1F* histC15 = (TH1F*)DataKaon->Get("LambdaC Cut 15");

	//TFile* DataKaon = new TFile(filekaonTree.c_str());
	//TH1F* finalDataKaonHist = (TH1F*)DataKaon->Get("Kaon Cut 10");
	TH1F* histk0 = (TH1F*)DataKaon->Get("Kaon Cut 0");
	TH1F* histk1 = (TH1F*)DataKaon->Get("Kaon Cut 1");
	TH1F* histk2 = (TH1F*)DataKaon->Get("Kaon Cut 2");
	TH1F* histk3 = (TH1F*)DataKaon->Get("Kaon Cut 3");
	TH1F* histk4 = (TH1F*)DataKaon->Get("Kaon Cut 4");
	TH1F* histk5 = (TH1F*)DataKaon->Get("Kaon Cut 5");
	TH1F* histk6 = (TH1F*)DataKaon->Get("Kaon Cut 6");
	TH1F* histk7 = (TH1F*)DataKaon->Get("Kaon Cut 7");
	TH1F* histk8 = (TH1F*)DataKaon->Get("Kaon Cut 8");
	TH1F* histk9 = (TH1F*)DataKaon->Get("Kaon Cut 9");
	TH1F* histk10 = (TH1F*)DataKaon->Get("Kaon Cut 10");

	//Getting Final Hists for MC
	TFile* RAPGAPLambda = new TFile(filelambdaRAPGAPTree.c_str());
	TH1F* finalRAPGAPLambdaHist = (TH1F*)RAPGAPLambda->Get("Lambda Cut 15");
	finalRAPGAPLambdaHist->SetLineColor(kRed);

	TFile* RAPGAPKaon = new TFile(filekaonRAPGAPTree.c_str());
	TH1F* finalRAPGAPKaonHist = (TH1F*)RAPGAPKaon->Get("Kaon Cut 10");
	finalRAPGAPKaonHist->SetLineColor(kRed);

	TFile* DJANGOHLambda = new TFile(filelambdaDJANGOHTree.c_str());
	TH1F* finalDJANGOHLambdaHist = (TH1F*)DJANGOHLambda->Get("Lambda Cut 15");
	finalDJANGOHLambdaHist->SetLineColor(kBlue);

	TFile* DJANGOHKaon = new TFile(filekaonDJANGOHTree.c_str());
	TH1F* finalDJANGOHKaonHist = (TH1F*)DJANGOHKaon->Get("Kaon Cut 10");
	finalDJANGOHKaonHist->SetLineColor(kBlue);
	
	//creating file to output histograms to
	TFile outputfile(outputfilename.c_str(), "RECREATE");
	int lw = 2;

	hist0->SetLineWidth(lw);
	hist0->Draw();
	hist0->Write();

	hist1->SetLineWidth(lw);
	hist1->SetLineColor(kRed);
	hist1->Draw();
	hist1->Write();

	hist2->SetLineWidth(lw);
	hist2->SetLineColor(kGreen+1);
	hist2->Draw();
	hist2->Write();

	hist3->SetLineWidth(lw);
	hist3->SetLineColor(kBlue);
	hist3->Draw();
	hist3->Write();

	hist4->SetLineWidth(lw);
	hist4->SetLineColor(kOrange+7);
	hist4->Draw();
	hist4->Write();

	hist5->SetLineWidth(lw);
	hist5->SetLineColor(kBlack);
	hist5->Draw();
	hist5->Write();

	hist6->SetLineWidth(lw);
	hist6->SetLineColor(kRed);
	hist6->Draw();
	hist6->Write();

	hist7->SetLineWidth(lw);
	hist7->SetLineColor(kGreen+1);
	hist7->Draw();
	hist7->Write();

	hist8->SetLineWidth(lw);
	hist8->SetLineColor(kBlue);
	hist8->Draw();
	hist8->Write();

	hist9->SetLineWidth(lw);
	hist9->SetLineColor(kMagenta+1);
	hist9->Draw();
	hist9->Write();

	hist10->SetLineWidth(lw);
	hist10->SetLineColor(kRed);
	hist10->Draw();
	hist10->Write();

	hist11->SetLineWidth(lw);
	hist11->SetLineColor(kYellow+1);
	hist11->Draw();
	hist11->Write();

	hist12->SetLineWidth(lw);
	hist12->SetLineColor(kBlack);
	hist12->Draw();
	hist12->Write();

	hist13->SetLineWidth(lw);
	hist13->Draw();
	hist13->Write();

	hist14->SetLineWidth(lw);
	hist14->SetLineColor(kBlack);
	hist14->Draw();
	hist14->Write();

	hist15->SetLineWidth(lw);
	hist15->SetLineColor(kOrange);
	hist15->Draw();
	hist15->Write();

	//LambdaC
	histC0->SetLineWidth(lw);
	histC0->Draw();
	histC0->Write();

	histC1->SetLineWidth(lw);
	histC1->SetLineColor(kRed);
	histC1->Draw();
	histC1->Write();

	histC2->SetLineWidth(lw);
	histC2->SetLineColor(kGreen + 1);
	histC2->Draw();
	histC2->Write();

	histC3->SetLineWidth(lw);
	histC3->SetLineColor(kBlue);
	histC3->Draw();
	histC3->Write();

	histC4->SetLineWidth(lw);
	histC4->SetLineColor(kOrange + 7);
	histC4->Draw();
	histC4->Write();

	histC5->SetLineWidth(lw);
	histC5->SetLineColor(kBlack);
	histC5->Draw();
	histC5->Write();

	histC6->SetLineWidth(lw);
	histC6->SetLineColor(kRed);
	histC6->Draw();
	histC6->Write();

	histC7->SetLineWidth(lw);
	histC7->SetLineColor(kGreen + 1);
	histC7->Draw();
	histC7->Write();

	histC8->SetLineWidth(lw);
	histC8->SetLineColor(kBlue);
	histC8->Draw();
	histC8->Write();

	histC9->SetLineWidth(lw);
	histC9->SetLineColor(kMagenta + 1);
	histC9->Draw();
	histC9->Write();

	histC10->SetLineWidth(lw);
	histC10->SetLineColor(kMagenta);
	histC10->Draw();
	histC10->Write();

	histC11->SetLineWidth(lw);
	histC11->SetLineColor(kYellow + 1);
	histC11->Draw();
	histC11->Write();

	histC12->SetLineWidth(lw);
	histC12->SetLineColor(kGray);
	histC12->Draw();
	histC12->Write();

	histC13->SetLineWidth(lw);
	histC13->SetLineColor(kBlack);
	histC13->Draw();
	histC13->Write();

	histC14->SetLineWidth(lw);
	histC14->SetLineColor(kBlack);
	histC14->Draw();
	histC14->Write();

	histC15->SetLineWidth(lw);
	histC15->SetLineColor(kOrange);
	histC15->Draw();
	histC15->Write();
	
	histk0->SetLineWidth(lw);
	histk0->SetLineColor(kBlack);
	histk0->Draw();
	histk0->Write();

	histk1->SetLineWidth(lw);
	histk1->SetLineColor(kRed);
	histk1->Draw();
	histk1->Write();

	histk2->SetLineWidth(lw);
	histk2->SetLineColor(kGreen+1);
	histk2->Draw();
	histk2->Write();

	histk3->SetLineWidth(lw);
	histk3->SetLineColor(kMagenta+1);
	histk3->Draw();
	histk3->Write();

	histk4->SetLineWidth(lw);
	histk4->SetLineColor(kOrange+7);
	histk4->Draw();
	histk4->Write();

	histk5->SetLineWidth(lw);
	histk5->SetLineColor(kBlue);
	histk5->Draw();
	histk5->Write();

	histk6->SetLineWidth(lw);
	histk6->SetLineColor(kGreen+1);
	histk6->Draw();
	histk6->Write();

	histk7->SetLineWidth(lw);
	histk7->SetLineColor(kRed);
	histk7->Draw();
	histk7->Write();

	histk8->SetLineWidth(lw);
	histk8->SetLineColor(kBlack);
	histk8->Draw();
	histk8->Write();

	histk9->SetLineWidth(lw);
	histk9->SetLineColor(kBlack);
	histk9->Draw();
	histk9->Write();

	histk10->SetLineWidth(lw);
	histk10->SetLineColor(kBlack);
	histk10->Draw();
	histk10->Write();
	
	THStack* hs = new THStack("hs", "2005-2007 H1 Data #Lambda+#bar{#Lambda} Mass Spectrum");

	hs->Add(hist0);
	hs->Add(hist1);
	hs->Add(hist2);
	hs->Add(hist3);
	hs->Add(hist4);
	hs->Add(hist5);
	hs->Add(hist6);
	hs->Add(hist7);
	hs->Add(hist8);
	hs->Add(hist9);
	hs->Add(hist10);
	hs->Add(hist11);
	hs->Add(hist12);
	hs->Add(hist13);
	hs->Add(hist14);
	hs->Draw("nostack");
	hs->GetXaxis()->SetTitle("Mass [GeV]");
	hs->GetYaxis()->SetTitle("Entries per GeV");
	canvas->SetName("Lambda Cut Stack");
	canvas->Write();

	//LambdaC
	THStack* hsC = new THStack("hs", "Stacked Total LambdaC Mass Plots");

	hsC->Add(histC0);
	hsC->Add(histC1);
	hsC->Add(histC2);
	hsC->Add(histC3);
	hsC->Add(histC4);
	hsC->Add(histC5);
	hsC->Add(histC6);
	hsC->Add(histC7);
	hsC->Add(histC8);
	hsC->Add(histC9);
	hsC->Add(histC10);
	hsC->Add(histC11);
	hsC->Add(histC12);
	hsC->Add(histC13);
	hsC->Add(histC14);
	hsC->Draw("nostack");
	hsC->GetXaxis()->SetTitle("Mass [GeV]");
	hsC->GetYaxis()->SetTitle("Entries per GeV");
	canvas->SetName("LambdaC Cut Stack");
	canvas->Write();

	THStack* hsk = new THStack("hsk", "2005-2007 H1 Data K^{0}_{s} Mass Spectrum");
	hsk->Add(histk0);
	hsk->Add(histk1);
	hsk->Add(histk2);
	hsk->Add(histk3);
	hsk->Add(histk4);
	hsk->Add(histk5);
	hsk->Add(histk6);
	hsk->Add(histk7);
	hsk->Add(histk8);
	hsk->Add(histk9);
	hsk->Add(histk10);
	hsk->Draw("nostack");
	hsk->GetXaxis()->SetTitle("Mass [GeV]");
	hsk->GetYaxis()->SetTitle("Entries per GeV");
	canvas->SetName("Kaon Cut Stack");
	canvas->Write();

	//Lambda Data vs MC RAPGAP and DJANGOH
	THStack* hsLambdaDataVsMC = new THStack("hs", "Lambda Data Vs MC Mass Plots");
	hsLambdaDataVsMC->Add(finalDataLambdaHist);
	hsLambdaDataVsMC->Add(finalRAPGAPLambdaHist);
	hsLambdaDataVsMC->Add(finalDJANGOHLambdaHist);
	hsLambdaDataVsMC->Draw("nostack");
	TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	//legend->SetHeader(Form("#splitline{Count- %.2f #pm %.2f (stat.)}{Stochastic MLP- %s Network Trained %ix}", kcount, kcounterror, network[particlecode].c_str(), trainnum[particlecode])); // option "C" allows to center the header
	legend->AddEntry(finalDataLambdaHist, "2005-2007 H1 Data", "l");
	legend->AddEntry(finalRAPGAPLambdaHist, "#frac{L_{Data}}{L_{RAPGAP}} RAPGAP", "l");
	legend->AddEntry(finalDJANGOHLambdaHist, "#frac{L_{Data}}{L_{DJANGOH}} DJANGOH", "l");
	legend->Draw("same");
	hsLambdaDataVsMC->GetXaxis()->SetTitle("Mass [GeV]");
	hsLambdaDataVsMC->GetYaxis()->SetTitle("Entries per GeV");
	canvas->SetName("Lambda Data vs MC");
	canvas->Write();

	outputfile.Write();
	outputfile.Close();

	cout << "Done" << endl;

};