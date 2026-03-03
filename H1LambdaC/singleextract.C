//Analysis Code Root Macro 
//Importing TTrees

using namespace std;

////////////////////////////////////////////////
// User Inputs in this section
////////////////////////////////////////////////

//Lambda and Kaon Extraction May require slightly different parameters
//Convecntion here: element[0] = Lambda ; element[1] = Kaon

vector<int> trainnum = { 100,100 }; //number of time NN is trained
vector<string> network = { "Mass:350:Count", "Mass:350:Count" }; //defining NN

//Vectors of fit subtraction values for lambda=0 and k0=1 particles,
//vector<double> leftfitbound1 = { 1.082, 0.40 };
//vector<double> leftfitbound2 = { 1.1, 0.435 };
//vector<double> rightfitbound1 = { 1.128, 0.555 };
//vector<double> rightfitbound2 = { 1.15, 0.68 };
//vector<double> leftsubbound = { 1.1, 0.44 };
//vector<double> rightsubbound = { 1.128, 0.55 };


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

//string names for file paths of 5 Trees we need to access
// DATA
//string filelambdaTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/lambdaTree06_NoDuplicates.root";
//string filekaonTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/kaonTree06_NoDuplicates.root";

//string filekaonTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/merged_KaonTreeMC_DJANGOH.root";
//string filekaonTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/kaonTreeMC_RAPGAP_1_25_24.root";
string filekaonTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/merged.root";

//string filelambdaTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/lambdaTree06_11_8_23.root";
//string filekaonTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/kaonTree06_11_8_23.root";
//string filelambdaTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/lambdaTree06_11_28_23.root";

//string filelambdaTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/merged_LambdaTreeMC_DJANGOH.root";
//string filelambdaTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/lambdaTreeMC_RAPGAP_1_25_24.root";
string filelambdaTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/merged.root";

//string filekaonTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/kaonTree06_11_28_23.root";
//END DATA

//MC


//string filekaonTreeMC = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/kaonTreeMC_RAPGAP_1_25_24.root";
string filekaonTreeMC = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/merged.root";


//string filelambdaTreeMC = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/lambdaTreeMC_RAPGAP_1_25_24.root";
string filelambdaTreeMC = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/merged.root";


//string filegenMCTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree_2_8_24.root"; //RAPGAP
//string filegenMCTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree_newsteeringtest_1_28_24.root"; //RAPGAP with new steering
//string filegenMCTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree_DJANGOH14RAD/merged.root"; //rad DJANGOH
string filegenMCTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/merged.root"; //rad RAPGAP


string fileNonRadgenMCTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree_DJANGOH14NONRAD/merged.root"; //non rad DJANGOH
string fileRadgenMCTree = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree_DJANGOH14RAD/merged.root";
//END MC

string outputfilename = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/singleextract.root";
string dummyfile = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/dummyfile.root";

string plotname = "Test";

// Histograms Binning
Double_t lambdabins = 1000;
Double_t lambdaupperbinrange = 1.5;
Double_t lambdalowerbinrange = 1.0;
Double_t kaonbins = 500;
Double_t kaonupperbinrange = 1.0;
Double_t kaonlowerbinrange = 0.0;




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//read data file with histograms
//TFile* f = new TFile(loadfilein.c_str());
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//creating file to output histograms to
TFile outputfile(outputfilename.c_str(), "RECREATE");
TFile* f = new TFile(dummyfile.c_str(),"RECREATE");




//End of MLP function and inputs, actual analysis code follows
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void singleextract() {
	TNtuple* ntp_bg = new TNtuple("datatree", "datatree", "Mass:Count");
	int particlecode = 0;
	double leftfitbound1 = 2.018;
	double leftfitbound2 = 2.20;
	double rightfitbound1 = 2.326;
	double rightfitbound2 = 2.6;
	
	double leftsubbound = 2.20;
	double rightsubbound = 2.326;

	TFile* Data = new TFile("/data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/merged.root");
	TH1F* inHist = (TH1F*)Data->Get("LambdaC Cut 13");
	string outputfilename = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/singleextract.root";
	TCanvas* c3 = new TCanvas("LambdaC Extraction", "LambdaC Extraction", 10, 10, 800, 400);
	TCanvas* c1 = new TCanvas("Training", "Training", 400, 400);

	// Read in the bin center and bin content from the histogram and prepare the text file

	for (int i = 1; i < (int)inHist->GetNbinsX() + 1; i++) {

		double bcx = inHist->GetBinCenter(i);

		if (isinf(inHist->GetBinContent(i))) inHist->SetBinContent(i, 0);

		if ((bcx > leftfitbound1 && bcx < leftfitbound2) || (bcx > rightfitbound1 && bcx < rightfitbound2))
			//ofl << bcx << "\t" << inHist->GetBinContent(i) << endl;
			ntp_bg->Fill(bcx, inHist->GetBinContent(i));
	}

	c1->cd();
	TMultiLayerPerceptron* mlp = new TMultiLayerPerceptron("Mass:350:Count", ntp_bg); //Defining the architecture - 1 hidden layer with 100 nodes
	mlp->Train(200, "graph, current, update=1000");

	outputfile.cd();
	c1->Write();
	outputfile.Write();
	f->cd();




	TH1F* fgHist = (TH1F*)inHist->Clone();


	Double_t inputs[1];

	TH2F* fgData = (TH2F*)fgHist->Clone();
	fgData->Reset("ICESM");

	TH2F* signalHist = (TH2F*)fgHist->Clone();
	signalHist->Reset("ICESM");

	TH2F* fitHistsubrange = (TH2F*)fgHist->Clone();
	fitHistsubrange->Reset("ICESM");

	TH2F* xaxis = (TH2F*)fgHist->Clone();
	xaxis->Reset("ICESM");
	xaxis->SetLineColor(kBlack);


	TH2F* fitHist = (TH2F*)fgHist->Clone();
	fitHist->Reset("ICESM");

	fgHist->SetLineColor(kBlack);
	signalHist->SetLineColor(kRed);
	fitHist->SetLineColor(kMagenta);
	fgData->SetLineColor(kBlack);
	fitHistsubrange->SetLineColor(kBlue);

	double kcount = 0;
	//double kcounterror = 0;
	double bgcount = 0;
	double totalcountwithbg = 0;

	for (int i = 1; i < fgHist->GetNbinsX() + 1; i++) {

		inputs[0] = ((TAxis*)fgHist->GetXaxis())->GetBinCenter(i);
		fgData->SetBinContent(i, fgHist->GetBinContent(i));
		fgData->SetBinError(i, fgHist->GetBinError(i));


		if ((inputs[0] >= leftfitbound1 && inputs[0] < leftfitbound2) || (inputs[0] >= rightfitbound1 && inputs[0] < rightfitbound2)) {
			fitHist->SetBinContent(i, mlp->Evaluate(0, inputs));
			double temp_content = fgHist->GetBinContent(i) - mlp->Evaluate(0, inputs);
			fitHist->SetBinContent(i, mlp->Evaluate(0, inputs));
			signalHist->SetBinContent(i, temp_content);
			signalHist->SetBinError(i, fgHist->GetBinError(i));
		}

		if (inputs[0] >= leftsubbound && inputs[0] < rightsubbound) {
			double temp_content = fgHist->GetBinContent(i) - mlp->Evaluate(0, inputs);
			signalHist->SetBinContent(i, temp_content);
			//fitHist->SetBinContent(i, mlp->Evaluate(0, inputs));
			signalHist->SetBinError(i, fgHist->GetBinError(i));
			fitHistsubrange->SetBinContent(i, mlp->Evaluate(0, inputs));
			totalcountwithbg = totalcountwithbg + fgHist->GetBinContent(i);
			//kcounterror = kcounterror + fgHist->GetBinError(i);
			//if (temp_content > 0) {
			kcount = kcount + temp_content;
			bgcount = bgcount + mlp->Evaluate(0, inputs);
			//}

		}

	}

	double kcounterror = sqrt(totalcountwithbg);

	c3->cd();
	fgHist->GetXaxis()->SetTitle("Mass [Gev]");
	fgHist->GetYaxis()->SetTitle("Count");
	fgHist->SetNameTitle(plotname.c_str(), plotname.c_str());
	fgHist->Draw();

	signalHist->Draw("same");
	fitHist->Draw("same");
	fitHistsubrange->Draw("same");
	xaxis->Draw("same");

	

	TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	legend->SetHeader(Form("#splitline{Count- %.2f #pm %.2f (stat.)}{Stochastic MLP- %s Network Trained %ix}", kcount, kcounterror, network[particlecode].c_str(), trainnum[particlecode])); // option "C" allows to center the header
	legend->AddEntry(fgHist, "Data", "l");
	legend->AddEntry(fitHist, "MLP Fit Accross Data Training Range", "l");
	legend->AddEntry(fitHistsubrange, "MLP Fit Extended under Peak", "l");
	legend->AddEntry(signalHist, "Data - MLP Fit", "lep");
	legend->Draw("same");

	outputfile.cd();
	c3->Write();

	outputfile.Write();
	f->cd();


		
	
	

	outputfile.Write();
	outputfile.Close();

	cout << "Done" << endl;

};