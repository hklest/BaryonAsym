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

string fileEventTreePythia = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/EventLists_PythiaBG/merged.root";

string fileEventTreeDjangoh = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/EventLists/merged.root";
string outputfilename = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/MigrationPlots.root";


// Histograms Binning
Double_t lambdabins = 1000;
Double_t lambdaupperbinrange = 1.5;
Double_t lambdalowerbinrange = 1.0;
Double_t kaonbins = 500;
Double_t kaonupperbinrange = 1.0;
Double_t kaonlowerbinrange = 0.0;






void MigrationPlots() {
	
	//Canvas
	TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 800);
	//Defining 2D Hists For Migration Plots
	//Q2 Bins: (5,7),(7,10),(10,15),(15,25),(25,100)
	//TH2F* hQ2 = new TH2F("Q2 Migration Plot", "Q2 Migration Plot", 110, 0, 110, 110, 0, 110);
	TH2F* hQ2finerbinning = new TH2F("Q2 Pythia Migration Plot", "Q2 Pythia Migration Plot", 100, 0, 100, 50, 0, 5);
	
	//TH2F* hyfinerbinning = new TH2F("y Migration Plot Finer Binning", "y Migration Plot Finer Binning", 20, 0, 1, 20, 0, 1);

	TH1F* hQ2genPythia = new TH1F("Q2gen Pythia", "Q2gen Pythia",1000 , 0, 100);
	TH1F* hQ2genDjangoh = new TH1F("Q2gen Djangoh", "Q2gen Djangoh", 1000, 0, 100);

	double PythiaLumi = 47500; //Pythia nb-1
	double DjangohLumi = 2268127.00; //DJANGOH nb-1

	double totaleventsrec = 0;
	double BGeventsrec = 0;

	//Data File
	TFile* Data = new TFile(fileEventTreePythia.c_str(), "read");
	//Getting Event Tree from the Data file
	
	TTree* EventTreePythia = (TTree*)Data->Get("Data Event Tree");
	TTree* InitialEventTreePythia = (TTree*)Data->Get("Event Migrations");
	
	

	//Define the variables and banch addresses for data we want from the Event Tree entries
	double fRunType, fMyRunYear, weight1, weight2, elecPhi, elecTheta, Epz, elecE, x, Q2s, Ys, Q2sGen, YsGen, RTheta, ClusterRadius, Zvertex, ProtonEnergy, ElectronEnergy, Xcluster, Ycluster, Zcluster, RunNumber, RunLumi, fposEX, fposEY; //, RCluster,;
	
																																																											 //Defining Tree Branches
	
	EventTreePythia->SetBranchAddress("x", &x);
	EventTreePythia->SetBranchAddress("Q2s", &Q2s);
	EventTreePythia->SetBranchAddress("Ys", &Ys);
	EventTreePythia->SetBranchAddress("Q2sGen", &Q2sGen);
	EventTreePythia->SetBranchAddress("YsGen", &YsGen);
	EventTreePythia->SetBranchAddress("weight1", &weight1);
	EventTreePythia->SetBranchAddress("weight2", &weight2);

	InitialEventTreePythia->SetBranchAddress("x", &x);
	InitialEventTreePythia->SetBranchAddress("Q2s", &Q2s);
	InitialEventTreePythia->SetBranchAddress("Ys", &Ys);
	InitialEventTreePythia->SetBranchAddress("Q2sGen", &Q2sGen);
	InitialEventTreePythia->SetBranchAddress("YsGen", &YsGen);
	InitialEventTreePythia->SetBranchAddress("weight1", &weight1);
	InitialEventTreePythia->SetBranchAddress("weight2", &weight2);
	//Fill
	vector<double> q2bins = {0, 5.0, 7.0, 10.0, 15.0, 25.0, 100.0,200.0 };
	const int q2entries = 7;
	int n;
	int u;
	
	

	//defining loop integers
	int m;
	int entries;//get number of entries in the Event Tree we just loaded in
	entries = EventTreePythia->GetEntries();
	double eventcount = 0;
	for (m = 0; m < entries; m++) {
		EventTreePythia->GetEntry(m);
		
		eventcount = eventcount + weight1 * weight2;
		hQ2finerbinning->Fill(Q2s, Q2sGen, weight1 * weight2);

		if (Q2sGen < 2) {
			BGeventsrec = BGeventsrec + weight1 * weight2 / PythiaLumi;
		}
	}

	//cout << "Total Event Count: " << eventcount << endl;
	
	entries = InitialEventTreePythia->GetEntries();
	//cout << entries << endl;
	int k;
	for (k = 0; k < entries; k++) {
		InitialEventTreePythia->GetEntry(k);
		//cout << Q2sGen << endl;
		hQ2genPythia->Fill(Q2sGen, weight1 * weight2/ PythiaLumi);
	}

	//Got All my data and filled Hists, so close data file
	Data->Close();
	TFile* DataDjangoh = new TFile(fileEventTreeDjangoh.c_str(), "read");
	TTree* InitialEventTreeDjangoh = (TTree*)DataDjangoh->Get("Event Migrations");
	TTree* EventTreeDjangoh = (TTree*)DataDjangoh->Get("Event Migrations");

	InitialEventTreeDjangoh->SetBranchAddress("x", &x);
	InitialEventTreeDjangoh->SetBranchAddress("Q2s", &Q2s);
	InitialEventTreeDjangoh->SetBranchAddress("Ys", &Ys);
	InitialEventTreeDjangoh->SetBranchAddress("Q2sGen", &Q2sGen);
	InitialEventTreeDjangoh->SetBranchAddress("YsGen", &YsGen);
	InitialEventTreeDjangoh->SetBranchAddress("weight1", &weight1);
	InitialEventTreeDjangoh->SetBranchAddress("weight2", &weight2);

	EventTreeDjangoh->SetBranchAddress("x", &x);
	EventTreeDjangoh->SetBranchAddress("Q2s", &Q2s);
	EventTreeDjangoh->SetBranchAddress("Ys", &Ys);
	EventTreeDjangoh->SetBranchAddress("Q2sGen", &Q2sGen);
	EventTreeDjangoh->SetBranchAddress("YsGen", &YsGen);
	EventTreeDjangoh->SetBranchAddress("weight1", &weight1);
	EventTreeDjangoh->SetBranchAddress("weight2", &weight2);

	entries = InitialEventTreeDjangoh->GetEntries();
	//cout << entries << endl;
	int l;
	for (l = 0; l < entries; l++) {
		InitialEventTreeDjangoh->GetEntry(l);
		//cout << Q2sGen << endl;
		hQ2genDjangoh->Fill(Q2sGen,weight1*weight2/ DjangohLumi);
	}

	entries = EventTreeDjangoh->GetEntries();
	//cout << entries << endl;
	int p;
	for (p = 0; p < entries; p++) {
		EventTreeDjangoh->GetEntry(p);
		//if (Q2sGen < 5) {
		//	BGeventsrec = BGeventsrec + weight1 * weight2 / DjangohLumi;
		//}

		if (Q2sGen > 5 && Q2sGen < 100) {
			totaleventsrec = totaleventsrec + weight1 * weight2 / DjangohLumi;
		}
		
		//cout << Q2sGen << endl;
		//hQ2genDjangoh->Fill(Q2sGen, weight1 * weight2 / DjangohLumi);
	}

	cout << "BG Events Rec: " << BGeventsrec << endl;
	cout << "Total Events Rec: " << totaleventsrec << endl;
	cout << "% BG Contamination: " << BGeventsrec / (totaleventsrec+BGeventsrec) << endl;

	DataDjangoh->Close();
	//Create output file

	TFile outputfile(outputfilename.c_str(), "RECREATE");
	//Draw Histograms and write to output file
	hQ2finerbinning->GetXaxis()->SetTitle("Q2 Rec");
	hQ2finerbinning->GetYaxis()->SetTitle("Q2 Gen");
	hQ2finerbinning->Draw("colz");
	//gPad->SetLogz();
	canvas->SetName("Q2 Migration for All Events Passing Selections");
	canvas->Write();

	hQ2genPythia->Draw();
	canvas->SetName("Pythia Q2gen Dist");
	canvas->Write();

	hQ2genDjangoh->Draw();
	canvas->SetName("Djangoh Q2gen Dist");
	canvas->Write();

	THStack* hs = new THStack("hs", "Pythia and Djangoh MC Set");
	hQ2genPythia->SetLineColor(kRed);
	hQ2genDjangoh->SetLineColor(kBlack);
	TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	legend->AddEntry(hQ2genPythia, "Pythia", "l");
	legend->AddEntry(hQ2genDjangoh, "Djangoh", "l");
	

	hs->Add(hQ2genPythia);
	hs->Add(hQ2genDjangoh);
	
	hs->Draw("nostack");
	hs->GetXaxis()->SetTitle("Q2");
	hs->GetYaxis()->SetTitle("Events/Lumi");
	legend->Draw("same");
	canvas->SetName("Pythia and Djangoh");
	canvas->Write();

	//Write and close output file
	outputfile.Write();
	outputfile.Close();

	cout << "Done" << endl;

};