//Analysis Code Root Macro 
//Importing TTrees

using namespace std;

////////////////////////////////////////////////
// User Inputs in this section
////////////////////////////////////////////////

const int binentries = 9;


//Branching Ratios
double BRlambda = 0.639;
double BRkaon = 0.692;


//output files
string outputfilename = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root";
//string dummyfile = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/dummyfile.root";

//creating file to output histograms to
TFile outputfile(outputfilename.c_str(), "RECREATE");
//TFile* f = new TFile(dummyfile.c_str(), "RECREATE");

// Histograms Binning
Double_t lambdabins = 800;
Double_t lambdaupperbinrange = 1.5;
Double_t lambdalowerbinrange = 1.0;
Double_t kaonbins = 500;
Double_t kaonupperbinrange = 1.0;
Double_t kaonlowerbinrange = 0.0;

// Get pion mass
double mPion = TDatabasePDG::Instance()->GetParticle(211)->Mass();
// Get proton mass
double mProton = TDatabasePDG::Instance()->GetParticle(2212)->Mass();
// Get Lambda mass
double mLambda = TDatabasePDG::Instance()->GetParticle(3122)->Mass();


/////////////////////////////////////////////////
// End of Input Section
/////////////////////////////////////////////////

vector<double> Extract(TTree* tree, double lowbinvalue, double highbinvalue, string plotname, TCanvas* canvas, vector<double> fitparaminit, vector<double> rangedefs) {

	}




void NewAsym() {
	TCanvas* canvas = new TCanvas("Lambda Extraction", "Lambda Extraction", 10, 10, 800, 400);
	
	string DataFileString = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/merged.root";
	//string RAPGAPFileString = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/merged.root";
	//string DJANGOHFileString = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/merged.root";
	
	TFile* DataFile = new TFile(DataFileString.c_str());
	//TFile DJANGOHFile(RAPGAPFileString.c_str(), "read");
	//TFile RAPGAPFile(DJANGOHFileString.c_str(), "read");

	string TreeNameString = "Lambda Reg";

	TTree* DataTree = (TTree*)DataFile->Get(TreeNameString.c_str());

	Extract(DataTree, 0.5, 0.6, "Test", canvas, );
	

	//int entries = DataTree->GetEntries();

}