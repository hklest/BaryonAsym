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

string filelambdaTree = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/merged.root";
string filekaonTree = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/merged.root";
string outputfilename = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/CutStack.root";


// Histograms Binning
Double_t lambdabins = 1000;
Double_t lambdaupperbinrange = 1.5;
Double_t lambdalowerbinrange = 1.0;
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
	//Data File
	TFile* Data = new TFile(filelambdaTree.c_str());
	//Getting Histograms
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

	TFile* DataKaon = new TFile(filekaonTree.c_str());
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
	
	//creating file to output histograms to
	TFile outputfile(outputfilename.c_str(), "RECREATE");
	hist0->Draw();
	hist0->Write();

	hist1->Draw();
	hist1->Write();

	hist2->Draw();
	hist2->Write();

	hist3->Draw();
	hist3->Write();

	hist4->Draw();
	hist4->Write();

	hist5->Draw();
	hist5->Write();

	hist6->Draw();
	hist6->Write();

	hist7->Draw();
	hist7->Write();

	hist8->Draw();
	hist8->Write();

	hist9->Draw();
	hist9->Write();

	hist10->Draw();
	hist10->Write();

	hist11->Draw();
	hist11->Write();

	hist12->Draw();
	hist12->Write();

	hist13->Draw();
	hist13->Write();

	hist14->Draw();
	hist14->Write();
	
	histk0->Draw();
	histk0->Write();

	histk1->Draw();
	histk1->Write();

	histk2->Draw();
	histk2->Write();

	histk3->Draw();
	histk3->Write();

	histk4->Draw();
	histk4->Write();

	histk5->Draw();
	histk5->Write();

	histk6->Draw();
	histk6->Write();

	histk7->Draw();
	histk7->Write();

	histk8->Draw();
	histk8->Write();

	histk9->Draw();
	histk9->Write();

	histk10->Draw();
	histk10->Write();
	
	THStack* hs = new THStack("hs", "Stacked Total Lambda Mass Plots");

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
	canvas->SetName("Lambda Cut Stack");
	canvas->Write();

	THStack* hsk = new THStack("hsk", "Stacked Total K0 Mass Plots");
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
	canvas->SetName("Kaon Cut Stack");
	canvas->Write();

	outputfile.Write();
	outputfile.Close();

	cout << "Done" << endl;

};