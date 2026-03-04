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

vector<double> leftfitbound1 = { 1.082, 0.35 };
vector<double> leftfitbound2 = { 1.1, 0.405 };
vector<double> rightfitbound1 = { 1.132, 0.59 };
vector<double> rightfitbound2 = { 1.16, 0.68 };
vector<double> leftsubbound = { 1.1, 0.405 };
vector<double> rightsubbound = { 1.132, 0.59 };

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
//string filelambdaTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/lambdaTree06_NoDuplicates.root";
//string filekaonTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/kaonTree06_NoDuplicates.root";

//string filekaonTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/merged_KaonTreeMC_DJANGOH.root";
//string filekaonTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/kaonTreeMC_RAPGAP_1_25_24.root";
string filekaonTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/merged.root";

//string filelambdaTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/lambdaTree06_11_8_23.root";
//string filekaonTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/kaonTree06_11_8_23.root";
//string filelambdaTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/lambdaTree06_11_28_23.root";

//string filelambdaTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/merged_LambdaTreeMC_DJANGOH.root";
//string filelambdaTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/lambdaTreeMC_RAPGAP_1_25_24.root";
string filelambdaTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/merged.root";

//string filekaonTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/kaonTree06_11_28_23.root";
//END DATA

//MC


//string filekaonTreeMC = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/kaonTreeMC_RAPGAP_1_25_24.root";
string filekaonTreeMC = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/merged.root";


//string filelambdaTreeMC = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/lambdaTreeMC_RAPGAP_1_25_24.root";
string filelambdaTreeMC = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/merged.root";


//string filegenMCTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree_2_8_24.root"; //RAPGAP
//string filegenMCTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree_newsteeringtest_1_28_24.root"; //RAPGAP with new steering
//string filegenMCTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree_DJANGOH14RAD/merged.root"; //rad DJANGOH
string filegenMCTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/merged.root"; //rad RAPGAP


string fileNonRadgenMCTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree_DJANGOH14NONRAD/merged.root"; //non rad DJANGOH
string fileRadgenMCTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree_DJANGOH14RAD/merged.root";
//END MC

string outputfilename = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/RadCorr.root";
string dummyfile = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/dummyfile.root";

// Histograms Binning
Double_t lambdabins = 1000;
Double_t lambdaupperbinrange = 1.5;
Double_t lambdalowerbinrange = 1.0;
Double_t kaonbins = 500;
Double_t kaonupperbinrange = 1.0;
Double_t kaonlowerbinrange = 0.0;


/////////////////////////////////////////////////
// End of Input Section
/////////////////////////////////////////////////

//Defining a couple functions used for calculations
double Square(double value) {
	return value * value;
}

double GetRatioError(double ratio, double numerator, double errnumerator, double denominator, double errdenominator) {
	return abs(ratio) * sqrt(Square(errnumerator / numerator) + Square(errdenominator / denominator));
}

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

//MLP function
vector<double> mlpfunction(TH1F* inHist, string plotname, int particlecode, TCanvas* c3, TCanvas* c1) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

	TNtuple* ntp_bg = new TNtuple("datatree", "datatree", "Mass:Count");

	// Read in the bin center and bin content from the histogram and prepare the text file

	for (int i = 1; i < (int)inHist->GetNbinsX() + 1; i++) {

		double bcx = inHist->GetBinCenter(i);

		if (isinf(inHist->GetBinContent(i))) inHist->SetBinContent(i, 0);

		if ((bcx > leftfitbound1[particlecode] && bcx < leftfitbound2[particlecode]) || (bcx > rightfitbound1[particlecode] && bcx < rightfitbound2[particlecode]))
			//ofl << bcx << "\t" << inHist->GetBinContent(i) << endl;
			ntp_bg->Fill(bcx, inHist->GetBinContent(i));
	}

	c1->cd();
	TMultiLayerPerceptron* mlp = new TMultiLayerPerceptron(network[particlecode].c_str(), ntp_bg); //Defining the architecture - 1 hidden layer with 100 nodes
	mlp->Train(trainnum[particlecode], "graph, current, update=1000");

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


		if ((inputs[0] >= leftfitbound1[particlecode] && inputs[0] < leftfitbound2[particlecode]) || (inputs[0] >= rightfitbound1[particlecode] && inputs[0] < rightfitbound2[particlecode])) {
			fitHist->SetBinContent(i, mlp->Evaluate(0, inputs));
		}

		if (inputs[0] >= leftsubbound[particlecode] && inputs[0] < rightsubbound[particlecode]) {
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
	legend->AddEntry(signalHist, "Subtracted Peak", "lep");
	legend->Draw("same");

	outputfile.cd();
	c3->Write();

	outputfile.Write();
	f->cd();



	//delete training data after each iteration
	ntp_bg->Reset();
	return { kcount,kcounterror, bgcount };
};

vector<vector<double>> AnalyzeTree(TFile* Data, string TreeName, int binentries, int particlecode, vector<TH1F*> HistArray, vector<double> binsvector, string extractionReadback, TCanvas* c3, TCanvas* c1) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters
	vector<vector<double>> ExtractionDataVector = {};

	//have files for lambdaTree, kaonTree, lambdaTreeMC, kaonTreeMC, genMCTree
	//TFile* Data = new TFile(filelambdaTree.c_str(), "read");
	//grab Tree from file above
	TTree* tree = (TTree*)Data->Get(TreeName.c_str());

	int entries;

	entries = tree->GetEntries();

	//Defining the variables that are stored in the Tree branches
	//double invMasslambda, invMasskaon, pT, Eta, x, Q2s, Ys;
	double invMass, pT, Eta, x, Q2s, Ys, Angle, weight1, weight2;
	string invMassName[2] = {"invMasslambda","invMasskaon"};

	//Defining the branch addresses for the Trees to access these stored variables
	tree->SetBranchAddress(invMassName[particlecode].c_str(), &invMass);
	tree->SetBranchAddress("pT", &pT);
	tree->SetBranchAddress("Eta", &Eta);
	tree->SetBranchAddress("x", &x);
	tree->SetBranchAddress("Q2s", &Q2s);
	tree->SetBranchAddress("Ys", &Ys);
	tree->SetBranchAddress("Angle", &Angle);
	tree->SetBranchAddress("weight1", &weight1);
	tree->SetBranchAddress("weight2", &weight2);

	int m;
	int j;
	//Have to enforce pT and Eta bounds here
	for (m = 0; m < entries; m++) {
		tree->GetEntry(m);
		for (j = 0; j < binentries; j++) {
			if (pT>0.5 && pT<3.5) {
				if (abs(Eta)<1.3) {
					if (Ys > 0.1 && Ys < 0.6) {
						if (Q2s > 5 && Q2s < 100) {
							//if (abs(Angle) <= 10.0) {
								if (Q2s > binsvector[j] && Q2s < binsvector[j + 1]) {
									HistArray[j]->Fill(invMass,weight1*weight2);
								}
							//}
						}
					}
				}
			}
		}
	}

	for (int j = 0; j < binentries; j++) {
		//Read in histogram of interest
		//TH1F* inHist = (TH1F*)f3->Get(Peaks2[j].c_str());
		//mlpfunction(inHist, PlotNames[j]);
		//mlpfunction writes training plot and extraction plot to external root file, then outputs count and statistical count error values
		cout << extractionReadback << j + 1 << " of " << binentries << endl;
		string trainingnamestr = "Training " + extractionReadback + to_string(j + 1);
		string extractionnamestr = extractionReadback + to_string(j + 1);
		c3->SetName(extractionnamestr.c_str());
		c1->SetName(trainingnamestr.c_str());
		ExtractionDataVector.push_back(mlpfunction(HistArray[j], extractionReadback.c_str() + to_string(j + 1), particlecode, c3, c1));
	}
	
	return ExtractionDataVector;

};

vector<double> AnalyzeGenTree(TFile* Data, string TreeName, int binentries, vector<double> binsvector) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

	vector<double> GenCounts = {};
	//initialize all GenCounts for each bin to zero in the array
	int p;
	for (p = 0; p < binentries; p++) {
		GenCounts.push_back(0);
	}

	//have files for lambdaTree, kaonTree, lambdaTreeMC, kaonTreeMC, genMCTree
	//TFile* Data = new TFile(filelambdaTree.c_str(), "read");
	//grab Tree from file above
	TTree* tree = (TTree*)Data->Get(TreeName.c_str());
	int entries;

	entries = tree->GetEntries();

	//Defining the variables that are stored in the Tree branches
	//double invMasslambda, invMasskaon, pT, Eta, x, Q2s, Ys;
	double pT, Eta, x, Q2s, Ys, weight1, weight2, Empz, GenElecEn, VtxZgen;

	//Defining the branch addresses for the Trees to access these stored variables
	//tree->SetBranchAddress("invMass", &invMass);
	tree->SetBranchAddress("pT", &pT);
	tree->SetBranchAddress("Eta", &Eta);
	tree->SetBranchAddress("x", &x);
	tree->SetBranchAddress("Q2s", &Q2s);
	tree->SetBranchAddress("Ys", &Ys);
	tree->SetBranchAddress("weight1", &weight1);
	tree->SetBranchAddress("weight2", &weight2);
	tree->SetBranchAddress("Empz", &Empz);
	tree->SetBranchAddress("GenElecEn", &GenElecEn);
	//tree->SetBranchAddress("H1GenBool", &H1GenBool);
	tree->SetBranchAddress("VtxZgen", &VtxZgen);

	int m;
	int j;
	for (m = 0; m < entries; m++) {
		tree->GetEntry(m);
		for (j = 0; j < binentries; j++) {
			if (pT > 0.5 && pT < 3.5) {
				if (abs(Eta) < 1.3) {
					if (Ys > 0.1 && Ys < 0.6) {
						if (Q2s > 5 && Q2s < 100) {
							if (GenElecEn>11) {
								if (Empz>35 && Empz<70) {
									if (abs(VtxZgen)<30) {
										if (Q2s > binsvector[j] && Q2s < binsvector[j + 1]) {
											GenCounts[j] = GenCounts[j] + 1 * weight1 * weight2;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return GenCounts;

};

vector<double> AnalyzeGenTreeRadCorr(TFile* Data, string TreeName, int binentries, vector<double> binsvector) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

	vector<double> GenCounts = {};
	//initialize all GenCounts for each bin to zero in the array
	int p;
	for (p = 0; p < binentries; p++) {
		GenCounts.push_back(0);
	}

	//have files for lambdaTree, kaonTree, lambdaTreeMC, kaonTreeMC, genMCTree
	//TFile* Data = new TFile(filelambdaTree.c_str(), "read");
	//grab Tree from file above
	TTree* tree = (TTree*)Data->Get(TreeName.c_str());
	int entries;

	entries = tree->GetEntries();

	//Defining the variables that are stored in the Tree branches
	//double invMasslambda, invMasskaon, pT, Eta, x, Q2s, Ys;
	double pT, Eta, x, Q2s, Ys, weight1, weight2, Empz, GenElecEn, H1GenBool, VtxZgen;

	//Defining the branch addresses for the Trees to access these stored variables
	//tree->SetBranchAddress("invMass", &invMass);
	tree->SetBranchAddress("pT", &pT);
	tree->SetBranchAddress("Eta", &Eta);
	tree->SetBranchAddress("x", &x);
	tree->SetBranchAddress("Q2s", &Q2s);
	tree->SetBranchAddress("Ys", &Ys);
	tree->SetBranchAddress("weight1", &weight1);
	tree->SetBranchAddress("weight2", &weight2);
	tree->SetBranchAddress("Empz", &Empz);
	tree->SetBranchAddress("GenElecEn", &GenElecEn);
	tree->SetBranchAddress("H1GenBool", &H1GenBool);
	tree->SetBranchAddress("VtxZgen", &VtxZgen);

	int m;
	int j;
	for (m = 0; m < entries; m++) {
		tree->GetEntry(m);
		for (j = 0; j < binentries; j++) {
			if (pT > 0.5 && pT < 3.5) {
				if (abs(Eta) < 1.3) {
					if (Ys > 0.1 && Ys < 0.6) {
						if (Q2s > 5 && Q2s < 100) {
							if (H1GenBool == 1) {
								if (abs(VtxZgen)<30) {
									if(GenElecEn>11){
										if (Empz > 35 && Empz < 70) {
											if (Q2s > binsvector[j] && Q2s < binsvector[j + 1]) {
												GenCounts[j] = GenCounts[j] + 1 * weight1 * weight2;
											}
										}
								    }
								}
							}
						}
					}
				}
			}
		}
	}

	return GenCounts;

};

int PlotData(string plotcanvasname, string plottitle, string xaxistitle, string yaxistitle, double ylowbound, double yupbound, TGraphErrors* cs, int binentries, double X[binentries], double y[binentries], double xerr[binentries], double y1err[binentries], double dylow[binentries], double dyup[binentries], double dylow2[binentries], double dyup2[binentries]) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

	TCanvas* c5 = new TCanvas(plotcanvasname.c_str(), plotcanvasname.c_str(), 10, 10, 800, 400);
	TGraphAsymmErrors* xyscan = new TGraphAsymmErrors(binentries, X, y, xerr, xerr, dylow, dyup);
	TGraphAsymmErrors* xyscan2 = new TGraphAsymmErrors(binentries, X, y, xerr, xerr, dylow2, dyup2);
	xyscan->SetMarkerColor(kBlack);
	xyscan->SetLineColor(kBlack);
	xyscan2->SetMarkerColor(kBlack);
	xyscan2->SetLineColor(kBlack);
	//auto cs = new TGraphErrors(pTentries, X, y1, xerr, y1err);
	//auto cs = new TGraphErrors(binentries, X, y1, xerr, y1err);
	cs->SetMarkerSize(0.01);
	xyscan->SetMarkerSize(0.01);
	xyscan->SetMarkerStyle(21);
	xyscan2->SetMarkerSize(0.01);
	xyscan2->SetMarkerStyle(21);
	cs->SetMarkerStyle(21);
	cs->SetMarkerColor(kRed);
	cs->SetLineColor(kRed);
	TMultiGraph* mgcs = new TMultiGraph();

	mgcs->Add(cs, "AP");
	mgcs->Add(xyscan, "AP");
	mgcs->Add(xyscan2, "AP");
	cs->Draw("AP");
	cs->GetYaxis()->SetRange(ylowbound, yupbound);
	//MCcrosssection->Draw("AP");
	mgcs->Draw("SAME");
	mgcs->SetTitle(plottitle.c_str());
	mgcs->GetXaxis()->SetTitle(xaxistitle.c_str());
	mgcs->GetYaxis()->SetTitle(yaxistitle.c_str());
	//MCcrosssection->Draw("SAME");
	TLegend* legend2 = new TLegend();
	//legend2->AddEntry(MCcrosssection, "RAPGAP31", "l");
	legend2->AddEntry(cs, "2006 Data", "l");
	legend2->AddEntry(xyscan, "1999-2000 H1 Analysis", "l");
	legend2->Draw("SAME");
	c5->SetGridy();
	outputfile.cd();
	c5->Write();
	f->cd();

	//c5->Clear();
	//c5->SetName("RAPGAP Lambda Cross Section");
	//MCcrosssection->Draw();
	//c5->Write();

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return 0;

};

//End of MLP function and inputs, actual analysis code follows
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void RadCorr() {
	//Defining integrated Lumi values for 05,06,07 data set obtained separately from oolumi
	double DataIntLumi05 = 108796.1875; //nb-1
	
	//old
	double DataIntLumi06 = 144634.0; //DATA nb-1
	//double DataIntLumi06 = 2268127.00; //DJANGOH nb-1
	//double DataIntLumi06 = 126880.0; //RAPGAP nb-1
	
	double DataIntLumi07 = 46577.761719; //nb-1
	double MCIntLumi05 = 253760.0; //nb-1
	
	//old
	//double MCIntLumi06 = 126880.0; //nb-1
	//double MCIntLumi06 = 126880.0; //RAPGAP nb-1
	double MCIntLumi06 = 2268127.00; //DJANGOH nb-1
	double MCDjangoh14radgen = 2268127.00;
	//double MCDjangoh14radgen = 2331962.00;//radiative gen level DJANGOH nb-1
	double MCDjangoh14nonradgen = 3560342.00;//non-radiatice gen level DJANGOH nb-1

	double MCIntLumi07 = 253760.0; //nb-1
	//Branching Ratios
	double BRlambda = 0.639;
	double BRkaon = 0.692;
	
	//Gen particle count
	vector<double> LambdaTotGenCount;
	vector<double> LambdaRegGenCount;
	vector<double> LambdaBarGenCount;
	vector<double> KaonGenCount;
	//Gen particle counting for Rad Corrections
	vector<double> LambdaGenCountRad;
	vector<double> KaonGenCountRad;
	vector<double> LambdaGenCountNonRad;
	vector<double> KaonGenCountNonRad;


	//Gen particle count
	vector<double> LambdaTotGenCountincl;
	vector<double> LambdaRegGenCountincl;
	vector<double> LambdaBarGenCountincl;
	vector<double> KaonGenCountincl;
	vector<double> LambdaTotGenCountinclNonRad;
	vector<double> LambdaTotGenCountinclRad;
	vector<double> KaonGenCountinclNonRad;
	vector<double> KaonGenCountinclRad;


	double xvals[q2entries] = { 6,8.5,12.5,20,62.5 };
	double xerr[q2entries] = { 1,1.5,2.5,5,37.5 };
	double X[5];
	double dxlow[5];
	double dxup[5];
	int n = 0;
	X[n] = xvals[n];	dxlow[n] = 0;	dxup[n] = 0;	n++;
	X[n] = xvals[n];	dxlow[n] = 0;	dxup[n] = 0;    n++;
	X[n] = xvals[n];	dxlow[n] = 0;	dxup[n] = 0;    n++;
	X[n] = xvals[n];	dxlow[n] = 0;	dxup[n] = 0;    n++;
	X[n] = xvals[n];	dxlow[n] = 0;	dxup[n] = 0;	n++;
	////


	//Analyzing Gen Counts
	LambdaTotGenCount = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Tot Channel", q2entries, q2bins);
	LambdaRegGenCount = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Reg Channel", q2entries, q2bins);
	LambdaBarGenCount = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Bar Channel", q2entries, q2bins);
	KaonGenCount = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "K0s Channel", q2entries, q2bins);
	
	//Analyzing Gen counts for radiative and non Radiative MC
	//Non Radiative Gen Counting for Radiative Corrections
	//LambdaTotGenCountinclNonRad = AnalyzeGenTreeRadCorr(new TFile(fileNonRadgenMCTree.c_str(), "read"), "Lambda Tot", 1, { q2bins[0],q2bins[q2entries] });
	//LambdaTotGenCountinclRad = AnalyzeGenTreeRadCorr(new TFile(filegenMCTree.c_str(), "read"), "Lambda Tot", 1, { q2bins[0],q2bins[q2entries] });
	//KaonGenCountinclNonRad = AnalyzeGenTreeRadCorr(new TFile(fileNonRadgenMCTree.c_str(), "read"), "K0s", 1, { q2bins[0],q2bins[q2entries] });
	//KaonGenCountinclRad = AnalyzeGenTreeRadCorr(new TFile(filegenMCTree.c_str(), "read"), "K0s", 1, { q2bins[0],q2bins[q2entries] });
	KaonGenCountRad = AnalyzeGenTreeRadCorr(new TFile(fileRadgenMCTree.c_str(), "read"), "K0s", q2entries, q2bins);
	KaonGenCountNonRad = AnalyzeGenTreeRadCorr(new TFile(fileNonRadgenMCTree.c_str(), "read"), "K0s", q2entries, q2bins);
	LambdaGenCountRad = AnalyzeGenTreeRadCorr(new TFile(fileRadgenMCTree.c_str(), "read"), "Lambda Tot", q2entries, q2bins);
	LambdaGenCountNonRad = AnalyzeGenTreeRadCorr(new TFile(fileNonRadgenMCTree.c_str(), "read"), "Lambda Tot", q2entries, q2bins);


	double effregplot[q2entries] = {};
	double effbarplot[q2entries] = {};
	double erreffregplot[q2entries] = {};
	double erreffbarplot[q2entries] = {};
	double lambdaradcorrplot[q2entries] = {};
	double k0radcorrplot[q2entries] = {};
	double lambdaradcorrploterrors[q2entries] = {};

	

	for (int k = 0; k < q2entries; k++) {
		
		
		//cout << "Lambda Rad Correction: " << (LambdaTotGenCountinclRad[0] / LambdaTotGenCountinclNonRad[0]) * (MCDjangoh14nonradgen / MCDjangoh14radgen) << endl;
		double templambdaradcorr = (LambdaGenCountRad[k] / LambdaGenCountNonRad[k]) * (MCDjangoh14nonradgen / MCDjangoh14radgen);
		lambdaradcorrplot[k] = (templambdaradcorr-1)*100;
		lambdaradcorrploterrors[k] = 0;
		cout << "Lambda Rad Cor Q2 Bin " << k + 1 << ": " << templambdaradcorr << endl;
		
	}

	for (int k = 0; k < q2entries; k++) {

		double tempk0radcorr = (KaonGenCountRad[k] / KaonGenCountNonRad[k]) * (MCDjangoh14nonradgen / MCDjangoh14radgen);
		k0radcorrplot[k] = (tempk0radcorr - 1) * 100;
		cout << "K0 Rad Cor Q2 Bin " << k + 1 << ": " << tempk0radcorr << endl;

	}


	//Plotting radiative Corrections
	TCanvas* cradcorr = new TCanvas("Radiative Corrections", "Radiative Corrections", 10, 10, 800, 400);
	cradcorr->SetName("Radiaitve Corrections vs q2");

	auto radcorrlambda = new TGraphErrors(q2entries, X, lambdaradcorrplot, xerr, lambdaradcorrploterrors);
	auto radcorrk0 = new TGraphErrors(q2entries, X, k0radcorrplot, xerr, lambdaradcorrploterrors);
	//auto effbar = new TGraphErrors(arraysize, X, effbarplot, xerr, erreffbarplot);
	radcorrlambda->SetMarkerColor(kRed);
	radcorrlambda->SetLineColor(kRed);
	radcorrlambda->SetMarkerSize(0.01);
	radcorrk0->SetMarkerColor(kBlue);
	radcorrk0->SetLineColor(kBlue);
	radcorrk0->SetMarkerSize(0.01);
	//effbar->SetMarkerColor(kBlue);
	//effbar->SetLineColor(kBlue);
	//effbar->SetMarkerSize(0.5);

	radcorrlambda->SetTitle("Radiative Corrections vs Q^{2}");
	radcorrlambda->GetXaxis()->SetTitle("Q^{2}");
	radcorrlambda->GetYaxis()->SetTitle("#delta_{QED} [%]");
	radcorrlambda->Draw();
	radcorrk0->Draw("SAME");
	//effbar->Draw("SAME");
	TLegend* legendradcorr = new TLegend();
	legendradcorr->AddEntry(radcorrlambda, "#Lambda", "l");
	legendradcorr->AddEntry(radcorrk0, "K^{0}_{s}", "l");
	//legend3->AddEntry(effbar, "#bar{#Lambda}", "l");
	legendradcorr->Draw("SAME");
	outputfile.cd();
	cradcorr->Write();
	//End plotting radiative corrections

	f->cd();

	LambdaTotGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Tot Channel", 1, { q2bins[0],q2bins[q2entries] });
	LambdaRegGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Reg Channel", 1, { q2bins[0],q2bins[q2entries] });
	LambdaBarGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Bar Channel", 1, { q2bins[0],q2bins[q2entries] });
	KaonGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "K0s Channel", 1, { q2bins[0],q2bins[q2entries] });

	//Non Radiative Gen Counting for Radiative Corrections
	LambdaTotGenCountinclNonRad = AnalyzeGenTreeRadCorr(new TFile(fileNonRadgenMCTree.c_str(), "read"), "Lambda Tot", 1, { q2bins[0],q2bins[q2entries] });
	LambdaTotGenCountinclRad = AnalyzeGenTreeRadCorr(new TFile(fileRadgenMCTree.c_str(), "read"), "Lambda Tot", 1, { q2bins[0],q2bins[q2entries] });
	KaonGenCountinclNonRad = AnalyzeGenTreeRadCorr(new TFile(fileNonRadgenMCTree.c_str(), "read"), "K0s", 1, { q2bins[0],q2bins[q2entries] });
	KaonGenCountinclRad = AnalyzeGenTreeRadCorr(new TFile(fileRadgenMCTree.c_str(), "read"), "K0s", 1, { q2bins[0],q2bins[q2entries] });

	//double MCDjangoh14radgen = 2331962.00;//radiative gen level DJANGOH nb-1
	//double MCDjangoh14nonradgen = 3560342.00;//non-radiatice gen level DJANGOH nb-1
	cout << "Lambda Tot Gen Count Rad Channel: " << LambdaTotGenCountincl[0] << endl;
	cout << "Lambda Tot Gen Count Rad Tot: " << LambdaTotGenCountinclRad[0] << endl;
	cout << "Lambda Tot Gen Count Non Rad Tot: " << LambdaTotGenCountinclNonRad[0] << endl;
	double lambdatotincradcorr = (LambdaTotGenCountinclRad[0] / LambdaTotGenCountinclNonRad[0]) * (MCDjangoh14nonradgen / MCDjangoh14radgen);
	cout << "Lambda Rad Correction: " << (LambdaTotGenCountinclRad[0] / LambdaTotGenCountinclNonRad[0]) * (MCDjangoh14nonradgen / MCDjangoh14radgen) << endl;
	//cout << "Lambda Rad Correction: " << (LambdaTotGenCountinclRad[0] / LambdaTotGenCountinclNonRad[0]) * (MCDjangoh14nonradgen / MCIntLumi06) << endl;
	
	double k0totincradcorr = (KaonGenCountinclRad[0] / KaonGenCountinclNonRad[0]) * (MCDjangoh14nonradgen / MCDjangoh14radgen);
	cout << "K0s Rad Correction: " << (KaonGenCountinclRad[0] / KaonGenCountinclNonRad[0]) * (MCDjangoh14nonradgen / MCDjangoh14radgen) << endl;
	//cout << "K0s Rad Correction: " << (KaonGenCountinclRad[0] / KaonGenCountinclNonRad[0]) * (MCDjangoh14nonradgen / MCIntLumi06) << endl;


	outputfile.Write();
	outputfile.Close();

	cout << "Done" << endl;

};