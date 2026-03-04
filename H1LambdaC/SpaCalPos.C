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

//Data
//string fileEventTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/EventLists/merged.root";

//MC
string fileEventTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/EventLists/DJANGOH14/merged.root";

string outputfilename = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/SpaCalPos.root";


// Histograms Binning
Double_t lambdabins = 1000;
Double_t lambdaupperbinrange = 1.5;
Double_t lambdalowerbinrange = 1.0;
Double_t kaonbins = 500;
Double_t kaonupperbinrange = 1.0;
Double_t kaonlowerbinrange = 0.0;






void SpaCalPos() {
	//TFile* f = new TFile("pi0_pp200.root"); 				//read in your data file
	//TH1F* inHist = (TH1F*)f->Get("fg2d_ggmass_sameArm");   //read in the histogram of interest

	//inHist->AddDirectory(0);

	//TH1F* checkHist = (TH1F*)inHist->Clone();
	//Canvas
	TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 900, 900);
	canvas->SetRightMargin(0.15);
	canvas->SetBottomMargin(0.12);
	//Defining 2D Hists For Migration Plots
	//Q2 Bins: (5,7),(7,10),(10,15),(15,25),(25,100)
	//TH2F* hQ2 = new TH2F("Q2 Migration Plot", "Q2 Migration Plot", 110, 0, 110, 110, 0, 110);
	TH2F* hXYcluster = new TH2F("XY SpaCal Cluster Positions 2005-2007 DJANGOH", "XY SpaCal Cluster Positions 2005-2007 DJANGOH", 1000, -80, 80, 1000, -80, 80);
	

	//Data File
	TFile* Data = new TFile(fileEventTree.c_str(), "read");
	//Getting Event Tree from the Data file
	//TTree* EventTree = (TTree*)Data->Get("Event Migrations");
	TTree* EventTree = (TTree*)Data->Get("Data Event Tree");
	
	int entries;//get number of entries in the Event Tree we just loaded in
	entries = EventTree->GetEntries();

	//Define the variables and banch addresses for data we want from the Event Tree entries
	double fRunType, fMyRunYear, weight1, weight2, elecPhi, elecTheta, Epz, elecE, x, Q2s, Ys, Q2sGen, YsGen, RTheta, ClusterRadius, Zvertex, ProtonEnergy, ElectronEnergy, Xcluster, Ycluster, Zcluster, RunNumber, RunLumi, fposEX, fposEY; //, RCluster,;
	
																																																											 //Defining Tree Branches
	//EventTree->SetBranchAddress("elecPhi", &elecPhi);
	//EventTree->SetBranchAddress("elecTheta", &elecTheta);
	//EventTree->SetBranchAddress("x", &x);
	//EventTree->SetBranchAddress("Q2s", &Q2s);
	//EventTree->SetBranchAddress("Ys", &Ys);
	//EventTree->SetBranchAddress("Q2sGen", &Q2sGen);
	//EventTree->SetBranchAddress("YsGen", &YsGen);
	//EventTree->SetBranchAddress("Epz", &Epz);
	//EventTree->SetBranchAddress("elecE", &elecE);
	//EventTree->SetBranchAddress("RTheta", &RTheta);
	//EventTree->SetBranchAddress("ClusterRadius", &ClusterRadius);
	//EventTree->SetBranchAddress("Zvertex", &Zvertex);
	//EventTree->SetBranchAddress("ProtonEnergy", &ProtonEnergy);
	//EventTree->SetBranchAddress("ElectronEnergy", &ElectronEnergy);
	EventTree->SetBranchAddress("Xcluster", &Xcluster);
	EventTree->SetBranchAddress("Ycluster", &Ycluster);
	//EventTree->SetBranchAddress("Zcluster", &Zcluster);
	//EventTree->SetBranchAddress("RunNumber", &RunNumber);
	//EventTree->SetBranchAddress("RunLumi", &RunLumi);
	//EventTree->SetBranchAddress("fRunType", &fRunType);
	//EventTree->SetBranchAddress("fMyRunYear", &fMyRunYear);
	//EventTree->SetBranchAddress("fposEX", &fposEX);
	//EventTree->SetBranchAddress("fposEY", &fposEY);
	EventTree->SetBranchAddress("weight1", &weight1);
	EventTree->SetBranchAddress("weight2", &weight2);
	double count = 0;
	//Fill
	vector<double> q2bins = {0, 5.0, 7.0, 10.0, 15.0, 25.0, 100.0,200.0 };
	const int q2entries = 7;
	int n;
	int u;
	
	//for (n = 0; n < q2entries; n++) { //x coordinate loop
	//	for (u = 0; u < q2entries; u++) { //y coordinate loop
	//		if (Q2s > q2bins[n] && Q2s < q2bins[n + 1]) {
	//			if (Q2sGen > q2bins[u] && Q2sGen < q2bins[u + 1]) {
	//				hQ2->SetBinContent(i,j, weight1 * weight2);
	//			}
	//		}
	//	}
	//}

	//defining loop integers
	int m;
	int i;
	int j;
	for (m = 0; m < entries; m++) {
		EventTree->GetEntry(m);
		//hQ2finerbinning->Fill(Q2s, Q2sGen, weight1 * weight2);
		//hQ2rec->Fill(Q2s, weight1 * weight2);
		//hQ2gen->Fill(Q2sGen, weight1 * weight2);
		//hQ2finerbinning->Fill(Q2s, Q2sGen);
		//hyfinerbinning->Fill(Ys, YsGen);
		//hQ2rec->Fill(Q2s);
		//hQ2gen->Fill(Q2sGen);
		hXYcluster->Fill(Xcluster, Ycluster, weight1+weight2);
		count = count + 1 * weight1 * weight2;
		//hXYcluster->Fill(Xcluster, Ycluster);
		//This whole mess is to use the same Q2 binning in analysis
		//for (i = 1; i < (int)hQ2->GetNbinsX() + 1; i++) {
		//	for (j = 1; j < (int)hQ2->GetNbinsY() + 1; j++) {
		//		double tempx = hQ2->GetXaxis()->GetBinCenter(i);
		//		double tempy = hQ2->GetYaxis()->GetBinCenter(j);
		//		for (n = 0; n < q2entries; n++) { //x coordinate loop
		//			for (u = 0; u < q2entries; u++) { //y coordinate loop
		//				if ((Q2s > q2bins[n] && tempx > q2bins[n]) && (Q2s < q2bins[n + 1] && tempx < q2bins[n + 1])) {
		//					if ((Q2sGen > q2bins[u] && tempy > q2bins[u]) && (Q2sGen < q2bins[u + 1] && tempy < q2bins[u + 1])) {
		//						hQ2->SetBinContent(i, j, weight1 * weight2);
		//					}
		//				}
		//			}
		//		}
		//	}
		//}
	}

	

	//Got All my data and filled Hists, so close data file
	Data->Close();
	
	//Create output file

	TFile outputfile(outputfilename.c_str(), "RECREATE");
	//Draw Histograms and write to output file
	hXYcluster->Scale(1/count);
	hXYcluster->GetXaxis()->SetTitle("X Cluster Position [cm]");
	hXYcluster->GetYaxis()->SetTitle("Y Cluster Position [cm]");
	hXYcluster->Draw("colz");
	hXYcluster->SetStats(0);
	//gPad->SetLogz();
	canvas->SetName("SpaCal Cluster Positions After Event Selections");
	canvas->Write();


	//Write and close output file
	outputfile.Write();
	outputfile.Close();

	cout << "Done" << endl;

};