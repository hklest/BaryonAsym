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
string fileEventTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree_DJANGOH14NONRAD/merged.root";

string outputfilename = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/check.root";


// Histograms Binning
Double_t lambdabins = 1000;
Double_t lambdaupperbinrange = 1.5;
Double_t lambdalowerbinrange = 1.0;
Double_t kaonbins = 500;
Double_t kaonupperbinrange = 1.0;
Double_t kaonlowerbinrange = 0.0;






void check() {
	
	TFile* Data = new TFile(fileEventTree.c_str(), "read");
	//Getting Event Tree from the Data file
	//TTree* EventTree = (TTree*)Data->Get("Event Migrations");
	TTree* EventTree = (TTree*)Data->Get("Lambda Tot");
	
	int entries;//get number of entries in the Event Tree we just loaded in
	entries = EventTree->GetEntries();
	int count = 0;
	//Define the variables and banch addresses for data we want from the Event Tree entries
	double fRunType, fMyRunYear, weight1, weight2, pT, Eta, x, Q2s, Ys; //, RCluster,;
	
	EventTree->SetBranchAddress("pT", &pT);
	EventTree->SetBranchAddress("Eta", &Eta);
	EventTree->SetBranchAddress("x", &x);
	EventTree->SetBranchAddress("Q2s", &Q2s);
	EventTree->SetBranchAddress("Ys", &Ys);
	EventTree->SetBranchAddress("weight1", &weight1);
	EventTree->SetBranchAddress("weight2", &weight2);																																																										 //Defining Tree Branches
	


	//defining loop integers
	int m;
	int i;
	int j;
	for (m = 0; m < entries; m++) {
		EventTree->GetEntry(m);
		if (pT > 0.5 && pT < 3.5) {
			if (abs(Eta) < 1.3) {
				if (Ys > 0.1 && Ys < 0.6) {
					if (Q2s > 5 && Q2s < 100) {
						
						count = count + 1 * weight1 * weight2;
						
					}
				}
			}
		}
	}

	cout << "Count: " << count << endl;
	

	//Got All my data and filled Hists, so close data file
	Data->Close();
	
	//Create output file

	TFile outputfile(outputfilename.c_str(), "RECREATE");
	//Draw Histograms and write to output file

	
	//Write and close output file
	outputfile.Write();
	outputfile.Close();

	cout << "Done" << endl;

};