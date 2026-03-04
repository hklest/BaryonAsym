//Analysis Code Root Macro 
//Importing TTrees

using namespace std;

////////////////////////////////////////////////
// User Inputs in this section
////////////////////////////////////////////////

//Lambda and Kaon Extraction May require slightly different parameters
//Convecntion here: element[0] = Lambda ; element[1] = Kaon

vector<int> trainnum = { 10,10 }; //number of time NN is trained
vector<string> network = { "Mass:350:Count", "Mass:350:Count" }; //defining NN

//Vectors of fit subtraction values for lambda=0 and k0=1 particles,
//vector<double> leftfitbound1 = { 1.082, 0.40 };
//vector<double> leftfitbound2 = { 1.1, 0.435 };
//vector<double> rightfitbound1 = { 1.128, 0.555 };
//vector<double> rightfitbound2 = { 1.15, 0.68 };
//vector<double> leftsubbound = { 1.1, 0.44 };
//vector<double> rightsubbound = { 1.128, 0.55 };

vector<double> leftfitbound1 = { 1.082, 0.40 };
vector<double> leftfitbound2 = { 1.1, 0.425 };
vector<double> rightfitbound1 = { 1.13, 0.59 };
vector<double> rightfitbound2 = { 1.15, 0.68 };
vector<double> leftsubbound = { 1.1, 0.43 };
vector<double> rightsubbound = { 1.13, 0.58 };

//defining vectors of the binning definitions for each of the kinematic variables we are looking at
//when playing with binning, have to adjust these vectors and hand specify the number of bins for defining Hist arrays
vector<double> pTbins = {0.5, 0.6, 0.7, 0.8, 0.9, 1.1, 1.3, 1.6, 2.2, 3.5};
vector<double> Etabins = {-1.3, -1.0, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1.0, 1.3};
vector<double> xbins = {0.00004, 0.0001, 0.0002, 0.0004, 0.001, 0.01};
vector<double> Q2ebins = {2.0, 2.5, 3.0, 4.0, 5.0, 7.0, 10.0, 15.0, 25.0, 100.0};
const int pTentries = 9;

//string names for file paths of 5 Trees we need to access
string filelambdaTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/lambdaTree06.root";
string filekaonTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/kaonTree06.root";

string filelambdaTreeMC = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/lambdaTreeMC06.root";
string filekaonTreeMC = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/kaonTreeMC06.root";
string filegenMCTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree06.root";

string outputfilename = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.root";
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
			if (temp_content > 0) {
				kcount = kcount + temp_content;
				bgcount = bgcount + mlp->Evaluate(0, inputs);
			}

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
	//double invMasslambda, invMasskaon, pT, Eta, x, Q2e, Ye;
	double invMass, pT, Eta, x, Q2e, Ye;
	string invMassName[2] = {"invMasslambda","invMasskaon"};

	//Defining the branch addresses for the Trees to access these stored variables
	tree->SetBranchAddress(invMassName[particlecode].c_str(), &invMass);
	tree->SetBranchAddress("pT", &pT);
	tree->SetBranchAddress("Eta", &Eta);
	tree->SetBranchAddress("x", &x);
	tree->SetBranchAddress("Q2e", &Q2e);
	tree->SetBranchAddress("Ye", &Ye);

	int m;
	int j;
	//Have to enforce pT and Eta bounds here
	for (m = 0; m < entries; m++) {
		tree->GetEntry(m);
		for (j = 0; j < binentries; j++) {
			if (pT>0.5 && pT<3.5) {
				if (abs(Eta)<1.3) {
					if (pT > binsvector[j] && pT < binsvector[j + 1]) {
						HistArray[j]->Fill(invMass);
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
	//double invMasslambda, invMasskaon, pT, Eta, x, Q2e, Ye;
	double pT, Eta, x, Q2e, Ye;

	//Defining the branch addresses for the Trees to access these stored variables
	//tree->SetBranchAddress("invMass", &invMass);
	tree->SetBranchAddress("pT", &pT);
	tree->SetBranchAddress("Eta", &Eta);
	tree->SetBranchAddress("x", &x);
	tree->SetBranchAddress("Q2e", &Q2e);
	tree->SetBranchAddress("Ye", &Ye);

	int m;
	int j;
	for (m = 0; m < entries; m++) {
		tree->GetEntry(m);
		for (j = 0; j < binentries; j++) {
			if (pT > 0.5 && pT < 3.5) {
				if (abs(Eta) < 1.3) {
					if (pT > binsvector[j] && pT < binsvector[j + 1]) {
						GenCounts[j] = GenCounts[j] + 1;
					}
				}
			}
		}
	}

	return GenCounts;

};
//pTentries=9
//PlotData("Lambda Cross Section vs pT", "d#sigma(ep #rightarrow e #Lambda X)/dp_{T}", "p_{T} [GeV]", "d#sigma/dp_{T} [nb/GeV]",cs, pTentries, X, y, xerr, y1err, dylow, dyup, dylow2, dyup2);

int PlotData(string plotcanvasname, string plottitle, string xaxistitle, string yaxistitle, TGraphErrors* cs, int binentries, double X[binentries], double y[binentries], double xerr[binentries], double y1err[binentries], double dylow[binentries], double dyup[binentries], double dylow2[binentries], double dyup2[binentries]) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

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
	//outputfile.cd();
	c5->Write();

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



void Analysis() {
	//Defining integrated Lumi values for 05,06,07 data set obtained separately from oolumi
	double DataIntLumi05 = 108796.1875; //nb-1
	double DataIntLumi06 = 132495.265625; //nb-1
	double DataIntLumi07 = 46577.761719; //nb-1
	double MCIntLumi05 = 253760.0; //nb-1
	double MCIntLumi06 = 126880.0; //nb-1
	double MCIntLumi07 = 253760.0; //nb-1
	//Branching Ratios
	double BRlambda = 0.639;
	double BRkaon = 0.692;
	
	// defining arrays of histograms corresponding to particles within kinematic bins
	//must be done for each particle, for data
	//TH1F* LambdaTotDatapT[pTentries];

	vector<TH1F*> LambdaTotDatapT;
	vector<TH1F*> LambdaRegDatapT;
	vector<TH1F*> LambdaBarDatapT;
	vector<TH1F*> LambdaTotMCpT;
	vector<TH1F*> LambdaRegMCpT;
	vector<TH1F*> LambdaBarMCpT;
	vector<TH1F*> KaonDatapT;
	vector<TH1F*> KaonMCpT;
	//I don't have Histograms for particle level generator data because counts for these
	//will be done by just going through the generate TTree and adding events up that are within our conditions
	for (int a = 0; a < pTentries; a++) {
		string namestr1 = "Lambda Tot Data pT Bin " + to_string(a + 1);
		string namestr2 = "Lambda Reg Data pT Bin " + to_string(a + 1);
		string namestr3 = "Lambda Bar Data pT Bin " + to_string(a + 1);
		string namestr4 = "Lambda Tot MC pT Bin " + to_string(a + 1);
		string namestr5 = "Lambda Reg MC pT Bin " + to_string(a + 1);
		string namestr6 = "Lambda Bar MC pT Bin " + to_string(a + 1);
		string namestr7 = "Kaon Data pT Bin " + to_string(a + 1);
		string namestr8 = "Kaon MC pT Bin " + to_string(a + 1);

		
		//LambdaTotDatapT[a] = new TH1F(namestr1.c_str(), namestr1.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange);
		LambdaTotDatapT.push_back(new TH1F(namestr1.c_str(), namestr1.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaRegDatapT.push_back(new TH1F(namestr2.c_str(), namestr2.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarDatapT.push_back(new TH1F(namestr3.c_str(), namestr3.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaTotMCpT.push_back(new TH1F(namestr4.c_str(), namestr4.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaRegMCpT.push_back(new TH1F(namestr5.c_str(), namestr5.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarMCpT.push_back(new TH1F(namestr6.c_str(), namestr6.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		KaonDatapT.push_back(new TH1F(namestr7.c_str(), namestr7.c_str(), kaonbins, kaonlowerbinrange, kaonupperbinrange));
		KaonMCpT.push_back(new TH1F(namestr8.c_str(), namestr8.c_str(), kaonbins, kaonlowerbinrange, kaonupperbinrange));
	}


	//
	//Space here for doing similar code as above for the other three variables used for binning
	//


	//do this after creating histograms
	//Creating the canvases used for storing MLP fit plots, good for troubleshooting
	TCanvas* c3 = new TCanvas("Lambda Extraction", "Lambda Extraction", 10, 10, 800, 400);
	TCanvas* c1 = new TCanvas("Training", "Training", 400, 400);
	
	//Defining vectors now that will contain the count and count error data for each paticle at MC, Data, and gen level (gen level has no count error, just count)
	//These vectors will be for all counting binning for all variables, can just clear them inbetween uses
	vector<vector<double>> LambdaTotDataCount;
	vector<vector<double>> LambdaRegDataCount;
	vector<vector<double>> LambdaBarDataCount;
	vector<vector<double>> LambdaTotMCCount;
	vector<vector<double>> LambdaRegMCCount;
	vector<vector<double>> LambdaBarMCCount;
	vector<vector<double>> KaonDataCount;
	vector<vector<double>> KaonMCCount;
	
	//Gen particle count
	vector<double> LambdaTotGenCount;
	vector<double> LambdaRegGenCount;
	vector<double> LambdaBarGenCount;
	vector<double> KaonGenCount;

	//Histogram vector names
	//vector<TH1F*> LambdaTotDatapT;
	//vector<TH1F*> LambdaRegDatapT;
	//vector<TH1F*> LambdaBarDatapT;
	//vector<TH1F*> LambdaTotMCpT;
	//vector<TH1F*> LambdaRegMCpT;
	//vector<TH1F*> LambdaBarMCpT;
	//vector<TH1F*> KaonDatapT;
	//vector<TH1F*> KaonMCpT;

	//Analyzing Tot Lambda Counts for pT bins for MC and Data
	LambdaTotDataCount = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Tot", pTentries, 0, LambdaTotDatapT, pTbins, "Lambda Tot Data pT Extraction ", c3, c1);
	LambdaTotMCCount = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Tot", pTentries, 0, LambdaTotMCpT, pTbins, "Lambda Tot MC pT Extraction ", c3, c1);

	LambdaRegDataCount = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Reg", pTentries, 0, LambdaRegDatapT, pTbins, "Lambda Reg Data pT Extraction ", c3, c1);
	LambdaRegMCCount = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Reg", pTentries, 0, LambdaRegMCpT, pTbins, "Lambda Reg MC pT Extraction ", c3, c1);

	LambdaBarDataCount = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Bar", pTentries, 0, LambdaBarDatapT, pTbins, "Lambda Bar Data pT Extraction ", c3, c1);
	LambdaBarMCCount = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Bar", pTentries, 0, LambdaBarMCpT, pTbins, "Lambda Bar MC pT Extraction ", c3, c1);

	KaonDataCount = AnalyzeTree(new TFile(filekaonTree.c_str(), "read"), "K0", pTentries, 1, KaonDatapT, pTbins, "Kaon Tot Data pT Extraction ", c3, c1);
	KaonMCCount = AnalyzeTree(new TFile(filekaonTreeMC.c_str(), "read"), "K0", pTentries, 1, KaonMCpT, pTbins, "Kaon Tot MC pT Extraction ", c3, c1);

	//Analyzing Gen Counts
	LambdaTotGenCount = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Tot", pTentries, pTbins);
	LambdaRegGenCount = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Reg", pTentries, pTbins);
	LambdaBarGenCount = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Bar", pTentries, pTbins);
	KaonGenCount = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "K0", pTentries, pTbins);
	

	
	double y1[pTentries] = {};
	double xerr[pTentries] = { 0.05,0.05,0.05,0.05,0.1,0.1,0.15,0.3,0.65 };
	double y1err[pTentries] = {};
	double x[pTentries] = { 0.55,0.65,0.75,0.85,1.0,1.2,1.45,1.9,2.85 };
	//double dx[pTentries] = { 0.1,0.1,0.1,0.1,0.2,0.2,0.3,0.6,1.3 };

	double effregplot[pTentries] = {};
	double effbarplot[pTentries] = {};
	double erreffregplot[pTentries] = {};
	double erreffbarplot[pTentries] = {};

	for (int k = 0; k < pTentries; k++) {
		double templameff = (LambdaTotMCCount[k][0]) / (LambdaTotGenCount[k]);
		
		double templamefferr = GetRatioError(templameff, LambdaTotMCCount[k][0], LambdaTotMCCount[k][1], LambdaTotGenCount[k], 0);
		double templamcrosssection = (LambdaTotDataCount[k][0]) / (templameff * BRlambda * DataIntLumi06 * (pTbins[k+1]-pTbins[k]));
		
		double templamcrosssectionerr = GetRatioError(templamcrosssection, (LambdaTotDataCount[k][0]), (LambdaTotDataCount[k][1]), (templameff * BRlambda * DataIntLumi06 * (pTbins[k + 1] - pTbins[k])), templamefferr);
		
		y1[k] = templamcrosssection;
		y1err[k] = templamcrosssectionerr;
		effregplot[k] = templameff;
		//effbarplot[k] = templambareff;
		erreffregplot[k] = templamefferr;
		//erreffbarplot[k] = templambarefferr;

		//y2[k - 1] = 100 * (1-(countdata[k][0]) / (bgref));
		//y1err[k - 1] = (100*(countdata[k][0]) / (countref))*sqrt((Square(countdata[k][1])/(Square(countdata[k][0])))+Square(errref)/(Square(countref)));
		//y2err[k - 1] = (100 * (countdata[k][2]) / (bgref))*sqrt((Square(countdata[k][1]) / (Square(countdata[k][2]))) + Square(errref) / (Square(bgref)));
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Lambda Cross section results from 2009 H1 Paper
	double X[9];
	double y[9];
	double dxlow[9];
	double dxup[9];
	double dylow[9];
	double dyup[9];
	int n = 0;
	X[n] = 0.541833;	y[n] = 8.4;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.4;	dyup[n] = 0.4;	n++;
	X[n] = 0.643426;	y[n] = 8.0;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.25;	dyup[n] = 0.25;	n++;
	X[n] = 0.74502;	y[n] = 7.9;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.25;	dyup[n] = 0.25;	n++;
	X[n] = 0.870518;	y[n] = 6.8;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.15;	dyup[n] = 0.15;	n++;
	X[n] = 1.01992;	y[n] = 6.1;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.1;	dyup[n] = 0.1;	n++;
	X[n] = 1.19323;	y[n] = 4.70;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.09;	dyup[n] = 0.09;	n++;
	X[n] = 1.44422;	y[n] = 3.05;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.05;	dyup[n] = 0.05;	n++;
	X[n] = 1.89243;	y[n] = 1.52;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.025;	dyup[n] = 0.025;	n++;
	X[n] = 2.84861;	y[n] = 0.42;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.01;	dyup[n] = 0.01;	n++;
	

	double dylow2[9];
	double dyup2[9];
	int m = 0;
	dylow2[m] = 0.8;	dyup2[m] = 0.8;	m++;
	dylow2[m] = 0.4;	dyup2[m] = 0.5;	m++;
	dylow2[m] = 0.5;	dyup2[m] = 0.5;	m++;
	dylow2[m] = 0.5;	dyup2[m] = 0.5;	m++;
	dylow2[m] = 0.4;	dyup2[m] = 0.4;	m++;
	dylow2[m] = 0.29;	dyup2[m] = 0.3;	m++;
	dylow2[m] = 0.20;	dyup2[m] = 0.2;	m++;
	dylow2[m] = 0.09;	dyup2[m] = 0.09; m++;
	dylow2[m] = 0.02;	dyup2[m] = 0.02; m++;

	
	//Plotting Lambda Efficiency
	TCanvas* ceff = new TCanvas("Lambda Cross Section vs pT", "Lambda Cross Section vs pT", 10, 10, 800, 400);
	ceff->SetName("Lambda Reconstruction Eff vs p_{T}");

	auto effreg = new TGraphErrors(pTentries, X, effregplot, xerr, erreffregplot);
	//auto effbar = new TGraphErrors(arraysize, X, effbarplot, xerr, erreffbarplot);
	effreg->SetMarkerColor(kRed);
	effreg->SetLineColor(kRed);
	effreg->SetMarkerSize(0.01);
	//effbar->SetMarkerColor(kBlue);
	//effbar->SetLineColor(kBlue);
	//effbar->SetMarkerSize(0.5);

	effreg->SetTitle("#Lambda Reconstruction Eff vs p_{T}");
	effreg->GetXaxis()->SetTitle("p_{T} [GeV]");
	effreg->GetYaxis()->SetTitle("Efficiency");
	effreg->Draw();
	//effbar->Draw("SAME");
	TLegend* legend3 = new TLegend();
	legend3->AddEntry(effreg, "#Lambda", "l");
	//legend3->AddEntry(effbar, "#bar{#Lambda}", "l");
	legend3->Draw("SAME");
	outputfile.cd();
	ceff->Write();
	//End Plotting Lambda Efficiency

	auto cs = new TGraphErrors(pTentries, X, y1, xerr, y1err);
	PlotData("Lambda Cross Section vs pT", "d#sigma(ep #rightarrow e [#Lambda + #bar{#Lambda}] X)/dp_{T}", "p_{T} [GeV]", "d#sigma/dp_{T} [nb/GeV]", cs, pTentries, X, y, xerr, y1err, dylow, dyup, dylow2, dyup2);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Lambda Asymm Data from 2009 paper obtained through xyscan, no syst error published for this, so just stat. error
	n = 0;
	X[n] = 0.541833;	y[n] = 0.0773109;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.087395;	dyup[n] = 0.087395;	n++;
	X[n] = 0.643426;	y[n] = 0.0252101;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0605042;	dyup[n] = 0.0605042;	n++;
	X[n] = 0.74502;	y[n] = -0.0705882;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0554622;	dyup[n] = 0.0554622;	n++;
	X[n] = 0.870518;	y[n] = -0.0336134;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0378151;	dyup[n] = 0.0378151;	n++;
	X[n] = 1.01992;	y[n] = -0.0218487;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0386555;	dyup[n] = 0.0386555;	n++;
	X[n] = 1.19323;	y[n] = -0.0369748;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0369748;	dyup[n] = 0.0369748;	n++;
	X[n] = 1.44422;	y[n] = 0.00336134;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0336134;	dyup[n] = 0.0336134;	n++;
	X[n] = 1.89243;	y[n] = 0.0151261;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0403361;	dyup[n] = 0.0403361;	n++;
	X[n] = 2.84861;	y[n] = 0.010084;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0487395;	dyup[n] = 0.0487395;	n++;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	

	for (int k = 0; k < pTentries; k++) {
		double templamregeff = (LambdaRegMCCount[k][0]) / (LambdaRegGenCount[k]);
		double templambareff = (LambdaBarMCCount[k][0]) / (LambdaBarGenCount[k]);
		double templamregefferr = GetRatioError(templamregeff, LambdaRegMCCount[k][0], LambdaRegMCCount[k][1], LambdaRegGenCount[k], 0);
		double templambarefferr = GetRatioError(templambareff, LambdaBarMCCount[k][0], LambdaBarMCCount[k][1], LambdaBarGenCount[k], 0);
		double templamreg = (LambdaRegDataCount[k][0]) / (templamregeff);
		double templambar = (LambdaBarDataCount[k][0]) / (templambareff);
		double templamregerr = GetRatioError((LambdaRegDataCount[k][0]) / (templamregeff), (LambdaRegDataCount[k][0]), (LambdaRegDataCount[k][1]), (templamregeff), templamregefferr);
		double templambarerr = GetRatioError((LambdaBarDataCount[k][0]) / (templambareff), (LambdaBarDataCount[k][0]), (LambdaBarDataCount[k][1]), (templambareff), templambarefferr);
		y1[k] = (templamreg - templambar) / ((templamreg + templambar));
		y1err[k] = GetRatioError(((templamreg - templambar) / ((templamreg + templambar))), (templamreg - templambar), (templamregerr + templambarerr), ((templamreg + templambar)), ((templamregerr + templambarerr)));
		
		//cout << k+1 << " Lambda reg eff err: " << templamregefferr << endl;
		//cout << k+1 << " Lambda bar eff err: " << templambarefferr << endl;
		//cout << k+1 << " Lambda Reg err: " << templamregerr << endl;
		//cout << k+1 << " Lambda Bar err: " << templambarerr << endl;
		//cout << k+1 << " Asymm err: " << y1err[k] << endl;

		effregplot[k] = templamregeff;
		effbarplot[k] = templambareff;
		erreffregplot[k] = templamregefferr;
		erreffbarplot[k] = templambarefferr;
	}

	auto cs2 = new TGraphErrors(pTentries, X, y1, xerr, y1err);
	PlotData("Lambda Asymm vs pT", "[#sigma(ep #rightarrow e #Lambda X) - #sigma(ep #rightarrow e #bar{#Lambda} X)] / [#sigma(ep #rightarrow e #Lambda X) + #sigma(ep #rightarrow e #bar{#Lambda} X)]", "p_{T} [GeV]", "A_{#Lambda}", cs2, pTentries, X, y, xerr, y1err, dylow, dyup, dylow, dyup);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//K0 Cross section results from 2009 H1 Paper
	n = 0;
	X[n] = 0.541833;	y[n] = 34.6;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.25;	dyup[n] = 0.25;	n++;
	X[n] = 0.643426;	y[n] = 29.6;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.2;	dyup[n] = 0.2;	n++;
	X[n] = 0.74502;	y[n] = 25.5;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.2;	dyup[n] = 0.2;	n++;
	X[n] = 0.870518;	y[n] = 20.4;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.15;	dyup[n] = 0.15;	n++;
	X[n] = 1.01992;	y[n] = 15.2;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.1;	dyup[n] = 0.1;	n++;
	X[n] = 1.19323;	y[n] = 10.46;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.07;	dyup[n] = 0.07;	n++;
	X[n] = 1.44422;	y[n] = 6.91;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.05;	dyup[n] = 0.05;	n++;
	X[n] = 1.89243;	y[n] = 3.13;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.02;	dyup[n] = 0.02;	n++;
	X[n] = 2.84861;	y[n] = 0.83;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.01;	dyup[n] = 0.01;	n++;

	m = 0;
	dylow2[m] = 2.1;	dyup2[m] = 2.0;	m++;
	dylow2[m] = 1.7;	dyup2[m] = 1.7;	m++;
	dylow2[m] = 1.5;	dyup2[m] = 1.4;	m++;
	dylow2[m] = 1.2;	dyup2[m] = 1.1;	m++;
	dylow2[m] = 0.9;	dyup2[m] = 0.9;	m++;
	dylow2[m] = 0.63;	dyup2[m] = 0.61;	m++;
	dylow2[m] = 0.46;	dyup2[m] = 0.46;	m++;
	dylow2[m] = 0.20;	dyup2[m] = 0.20; m++;
	dylow2[m] = 0.06;	dyup2[m] = 0.06; m++;

	
	for (int k = 0; k < pTentries; k++) {
		double tempk0eff = (KaonMCCount[k][0]) / (KaonGenCount[k]);

		double tempk0efferr = GetRatioError(tempk0eff, KaonMCCount[k][0], KaonMCCount[k][1], KaonGenCount[k], 0);
		double tempk0crosssection = (KaonDataCount[k][0]) / (tempk0eff * BRkaon * DataIntLumi06 * (pTbins[k + 1] - pTbins[k]));

		double tempk0crosssectionerr = GetRatioError(tempk0crosssection, (KaonDataCount[k][0]), (KaonDataCount[k][1]), (tempk0eff * BRkaon * DataIntLumi06 * (pTbins[k + 1] - pTbins[k])), tempk0efferr);

		y1[k] = tempk0crosssection;
		y1err[k] = tempk0crosssectionerr;
		effregplot[k] = tempk0eff;
		erreffregplot[k] = tempk0efferr;

	}



	//Plotting Lambda Efficiency
	TCanvas* ckeff = new TCanvas("K0 Cross Section vs pT", "K0 Cross Section vs pT", 10, 10, 800, 400);
	ckeff->SetName("K0 Reconstruction Eff vs p_{T}");

	auto effreg2 = new TGraphErrors(pTentries, X, effregplot, xerr, erreffregplot);
	//auto effbar = new TGraphErrors(arraysize, X, effbarplot, xerr, erreffbarplot);
	effreg2->SetMarkerColor(kRed);
	effreg2->SetLineColor(kRed);
	effreg2->SetMarkerSize(0.01);
	//effbar->SetMarkerColor(kBlue);
	//effbar->SetLineColor(kBlue);
	//effbar->SetMarkerSize(0.5);

	effreg2->SetTitle("K^{0}_{S} Reconstruction Eff vs p_{T}");
	effreg2->GetXaxis()->SetTitle("p_{T} [GeV]");
	effreg2->GetYaxis()->SetTitle("Efficiency");
	effreg2->Draw();
	//effbar->Draw("SAME");
	TLegend* legend4 = new TLegend();
	legend4->AddEntry(effreg2, "K^{0}_{S}", "l");
	//legend3->AddEntry(effbar, "#bar{#Lambda}", "l");
	legend4->Draw("SAME");
	outputfile.cd();
	ckeff->Write();
	//End Plotting Lambda Efficiency

	auto cs3 = new TGraphErrors(pTentries, X, y1, xerr, y1err);
	PlotData("K0 Cross Section vs pT", "d#sigma(ep #rightarrow e K^{0}_{S} X)/dp_{T}", "p_{T} [GeV]", "d#sigma/dp_{T} [nb/GeV]", cs3, pTentries, X, y, xerr, y1err, dylow, dyup, dylow2, dyup2);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//K0/Lambda Ratio Cross section results from 2009 H1 Paper
	n = 0;
	X[n] = 0.541833;	y[n] = 0.24;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.01;	dyup[n] = 0.01;	n++;
	X[n] = 0.643426;	y[n] = 0.268;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0085;	dyup[n] = 0.0085;	n++;
	X[n] = 0.74502;	y[n] = 0.309;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.01;	dyup[n] = 0.01;	n++;
	X[n] = 0.870518;	y[n] = 0.334;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.007;	dyup[n] = 0.007;	n++;
	X[n] = 1.01992;	y[n] = 0.402;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0085;	dyup[n] = 0.0085;	n++;
	X[n] = 1.19323;	y[n] = 0.450;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.009;	dyup[n] = 0.009;	n++;
	X[n] = 1.44422;	y[n] = 0.442;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.008;	dyup[n] = 0.008;	n++;
	X[n] = 1.89243;	y[n] = 0.485;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.008;	dyup[n] = 0.008;	n++;
	X[n] = 2.84861;	y[n] = 0.505;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0135;	dyup[n] = 0.0135;	n++;

	m = 0;
	dylow2[m] = 0.02;	dyup2[m] = 0.02;	m++;
	dylow2[m] = 0.01;	dyup2[m] = 0.009;	m++;
	dylow2[m] = 0.016;	dyup2[m] = 0.015;	m++;
	dylow2[m] = 0.017;	dyup2[m] = 0.016;	m++;
	dylow2[m] = 0.016;	dyup2[m] = 0.015;	m++;
	dylow2[m] = 0.016;	dyup2[m] = 0.015;	m++;
	dylow2[m] = 0.022;	dyup2[m] = 0.021;	m++;
	dylow2[m] = 0.02;	dyup2[m] = 0.019; m++;
	dylow2[m] = 0.032;	dyup2[m] = 0.031; m++;


	for (int k = 0; k < pTentries; k++) {
		double tempk0eff = (KaonMCCount[k][0]) / (KaonGenCount[k]);

		double tempk0efferr = GetRatioError(tempk0eff, KaonMCCount[k][0], KaonMCCount[k][1], KaonGenCount[k], 0);
		double tempk0crosssection = (KaonDataCount[k][0]) / (tempk0eff * BRkaon * DataIntLumi06 * (pTbins[k + 1] - pTbins[k]));

		double tempk0crosssectionerr = GetRatioError(tempk0crosssection, (KaonDataCount[k][0]), (KaonDataCount[k][1]), (tempk0eff * BRkaon * DataIntLumi06 * (pTbins[k + 1] - pTbins[k])), tempk0efferr);

		//y1[k] = tempk0crosssection;
		//y1err[k] = tempk0crosssectionerr;
		//effregplot[k] = tempk0eff;
		//erreffregplot[k] = tempk0efferr;
		//
		double templameff = (LambdaTotMCCount[k][0]) / (LambdaTotGenCount[k]);

		double templamefferr = GetRatioError(templameff, LambdaTotMCCount[k][0], LambdaTotMCCount[k][1], LambdaTotGenCount[k], 0);
		double templamcrosssection = (LambdaTotDataCount[k][0]) / (templameff * BRlambda * DataIntLumi06 * (pTbins[k + 1] - pTbins[k]));

		double templamcrosssectionerr = GetRatioError(templamcrosssection, (LambdaTotDataCount[k][0]), (LambdaTotDataCount[k][1]), (templameff * BRlambda * DataIntLumi06 * (pTbins[k + 1] - pTbins[k])), templamefferr);
		//y1[k] = templamcrosssection;
		//y1err[k] = templamcrosssectionerr;
		//effregplot[k] = templameff;
		//erreffregplot[k] = templamefferr;
		//

		double tempratio = (templamcrosssection) / (tempk0crosssection);
		double tempratioerr = GetRatioError(tempratio, templamcrosssection, templamcrosssectionerr, tempk0crosssection, tempk0crosssectionerr);
		y1[k] = tempratio;
		y1err[k] = tempratioerr;

	}

	auto cs4 = new TGraphErrors(pTentries, X, y1, xerr, y1err);
	PlotData("Lambda/K0 Cross Section vs pT", "[d#sigma(ep #rightarrow e #Lambda X)]/[d#sigma(ep #rightarrow e K^{0}_{S} X)]", "p_{T} [GeV]", "#Lambda/K^{0}_{S}", cs4, pTentries, X, y, xerr, y1err, dylow, dyup, dylow2, dyup2);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Now doing peak extractions and analysis for inclusive measurements
	vector<TH1F*> LambdaTotDatapTincl;
	vector<TH1F*> LambdaRegDatapTincl;
	vector<TH1F*> LambdaBarDatapTincl;
	vector<TH1F*> LambdaTotMCpTincl;
	vector<TH1F*> LambdaRegMCpTincl;
	vector<TH1F*> LambdaBarMCpTincl;
	vector<TH1F*> KaonDatapTincl;
	vector<TH1F*> KaonMCpTincl;
	//I don't have Histograms for particle level generator data because counts for these
	//will be done by just going through the generate TTree and adding events up that are within our conditions
	for (int a = 0; a < 1; a++) {
		string namestr1 = "Lambda Tot Data";
		string namestr2 = "Lambda Reg Data pT Bin";
		string namestr3 = "Lambda Bar Data pT Bin";
		string namestr4 = "Lambda Tot MC pT Bin";
		string namestr5 = "Lambda Reg MC pT Bin";
		string namestr6 = "Lambda Bar MC pT Bin";
		string namestr7 = "Kaon Data pT Bin";
		string namestr8 = "Kaon MC pT Bin";


		//LambdaTotDatapT[a] = new TH1F(namestr1.c_str(), namestr1.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange);
		LambdaTotDatapTincl.push_back(new TH1F(namestr1.c_str(), namestr1.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaRegDatapTincl.push_back(new TH1F(namestr2.c_str(), namestr2.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarDatapTincl.push_back(new TH1F(namestr3.c_str(), namestr3.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaTotMCpTincl.push_back(new TH1F(namestr4.c_str(), namestr4.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaRegMCpTincl.push_back(new TH1F(namestr5.c_str(), namestr5.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarMCpTincl.push_back(new TH1F(namestr6.c_str(), namestr6.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		KaonDatapTincl.push_back(new TH1F(namestr7.c_str(), namestr7.c_str(), kaonbins, kaonlowerbinrange, kaonupperbinrange));
		KaonMCpTincl.push_back(new TH1F(namestr8.c_str(), namestr8.c_str(), kaonbins, kaonlowerbinrange, kaonupperbinrange));
	}



	//Defining vectors now that will contain the count and count error data for each paticle at MC, Data, and gen level (gen level has no count error, just count)
	//These vectors will be for all counting binning for all variables, can just clear them inbetween uses
	vector<vector<double>> LambdaTotDataCountincl;
	vector<vector<double>> LambdaRegDataCountincl;
	vector<vector<double>> LambdaBarDataCountincl;
	vector<vector<double>> LambdaTotMCCountincl;
	vector<vector<double>> LambdaRegMCCountincl;
	vector<vector<double>> LambdaBarMCCountincl;
	vector<vector<double>> KaonDataCountincl;
	vector<vector<double>> KaonMCCountincl;

	//Gen particle count
	vector<double> LambdaTotGenCountincl;
	vector<double> LambdaRegGenCountincl;
	vector<double> LambdaBarGenCountincl;
	vector<double> KaonGenCountincl;

	//Analyzing Tot Lambda Counts for pT bins for MC and Data
	LambdaTotDataCountincl = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Tot", 1, 0, LambdaTotDatapTincl, {pTbins[0],pTbins[pTentries]}, "Lambda Tot Incl Data pT Extraction ", c3, c1);
	LambdaTotMCCountincl = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Tot", 1, 0, LambdaTotMCpTincl, { pTbins[0],pTbins[pTentries] }, "Lambda Tot Incl MC pT Extraction ", c3, c1);

	LambdaRegDataCountincl = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegDatapTincl, { pTbins[0],pTbins[pTentries] }, "Lambda Reg Incl Data pT Extraction ", c3, c1);
	LambdaRegMCCountincl = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegMCpTincl, { pTbins[0],pTbins[pTentries] }, "Lambda Reg Incl MC pT Extraction ", c3, c1);

	LambdaBarDataCountincl = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarDatapTincl, { pTbins[0],pTbins[pTentries] }, "Lambda Bar Incl Data pT Extraction ", c3, c1);
	LambdaBarMCCountincl = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarMCpTincl, { pTbins[0],pTbins[pTentries] }, "Lambda Bar Incl MC pT Extraction ", c3, c1);

	KaonDataCountincl = AnalyzeTree(new TFile(filekaonTree.c_str(), "read"), "K0", 1, 1, KaonDatapTincl, { pTbins[0],pTbins[pTentries] }, "Kaon Incl Data pT Extraction ", c3, c1);
	KaonMCCountincl = AnalyzeTree(new TFile(filekaonTreeMC.c_str(), "read"), "K0", 1, 1, KaonMCpTincl, { pTbins[0],pTbins[pTentries] }, "Kaon Incl MC pT Extraction ", c3, c1);

	//Analyzing Gen Counts
	LambdaTotGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Tot", 1, { pTbins[0],pTbins[pTentries] });
	LambdaRegGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Reg", 1, { pTbins[0],pTbins[pTentries] });
	LambdaBarGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Bar", 1, { pTbins[0],pTbins[pTentries] });
	KaonGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "K0", 1, { pTbins[0],pTbins[pTentries] });

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//lambda reg + lambda bar cross section
	
	double incllameff = (LambdaTotMCCountincl[0][0]) / (LambdaTotGenCountincl[0]);

	double incllamefferr = GetRatioError(incllameff, LambdaTotMCCountincl[0][0], LambdaTotMCCountincl[0][1], LambdaTotGenCountincl[0], 0);
	double incllamcrosssection = (LambdaTotDataCountincl[0][0]) / (incllameff * BRlambda * DataIntLumi06);
	double incllamcrosssectionerr = GetRatioError(incllamcrosssection, (LambdaTotDataCountincl[0][0]), (LambdaTotDataCountincl[0][1]), (incllameff * BRlambda * DataIntLumi06), incllamefferr);

	//lambda reg cross section
	double incllamregeff = (LambdaRegMCCountincl[0][0]) / (LambdaRegGenCountincl[0]);

	double incllamregefferr = GetRatioError(incllamregeff, LambdaRegMCCountincl[0][0], LambdaRegMCCountincl[0][1], LambdaRegGenCountincl[0], 0);
	double incllamregcrosssection = (LambdaRegDataCountincl[0][0]) / (incllamregeff * BRlambda * DataIntLumi06);
	double incllamregcrosssectionerr = GetRatioError(incllamregcrosssection, (LambdaRegDataCountincl[0][0]), (LambdaRegDataCountincl[0][1]), (incllamregeff * BRlambda * DataIntLumi06), incllamregefferr);

	//lambda bar cross section
	double incllambareff = (LambdaBarMCCountincl[0][0]) / (LambdaBarGenCountincl[0]);

	double incllambarefferr = GetRatioError(incllambareff, LambdaBarMCCountincl[0][0], LambdaBarMCCountincl[0][1], LambdaBarGenCountincl[0], 0);
	double incllambarcrosssection = (LambdaBarDataCountincl[0][0]) / (incllambareff * BRlambda * DataIntLumi06);
	double incllambarcrosssectionerr = GetRatioError(incllambarcrosssection, (LambdaBarDataCountincl[0][0]), (LambdaBarDataCountincl[0][1]), (incllambareff * BRlambda * DataIntLumi06), incllambarefferr);

	//lambda asymmetry
	double incllamasymm = (incllamregcrosssection - incllambarcrosssection) / (incllamregcrosssection + incllambarcrosssection);
	double incllamasymmerr = GetRatioError(((incllamregcrosssection - incllambarcrosssection) / (incllamregcrosssection + incllambarcrosssection)), (incllamregcrosssection - incllambarcrosssection), (incllamregcrosssectionerr + incllambarcrosssectionerr), (incllamregcrosssection + incllambarcrosssection), (incllamregcrosssectionerr + incllambarcrosssectionerr));

	//K0 cross section

	double inclK0eff = (KaonMCCountincl[0][0]) / (KaonGenCountincl[0]);

	double inclK0efferr = GetRatioError(inclK0eff, KaonMCCountincl[0][0], KaonMCCountincl[0][1], KaonGenCountincl[0], 0);
	double inclK0crosssection = (KaonDataCountincl[0][0]) / (inclK0eff * BRkaon * DataIntLumi06);
	double inclK0crosssectionerr = GetRatioError(inclK0crosssection, (KaonDataCountincl[0][0]), (KaonDataCountincl[0][1]), (inclK0eff * BRkaon * DataIntLumi06), inclK0efferr);

	//Lambda:K0 Ratio

	double inclratio = incllamcrosssection/ inclK0crosssection;
	double inclratioerr = GetRatioError(incllamcrosssection / inclK0crosssection, incllamcrosssection, incllamcrosssectionerr, inclK0crosssection, inclK0crosssectionerr);

	cout << "Lambda+Lambda Bar Inclusive Reconstruction Eff: " << incllameff << " pm " << incllamefferr << endl;
	cout << "Lambda+Lambda Bar Inclusive Cross Section: " << incllamcrosssection << " pm " << incllamcrosssectionerr << endl;
	cout << "Lambda Reg Inclusive Reconstruction Eff: " << incllamregeff << " pm " << incllamregefferr << endl;
	cout << "Lambda Reg Inclusive Cross Section: " << incllamregcrosssection << " pm " << incllamregcrosssectionerr << endl;
	cout << "Lambda Bar Inclusive Reconstruction Eff: " << incllambareff << " pm " << incllambarefferr << endl;
	cout << "Lambda Bar Inclusive Cross Section: " << incllambarcrosssection << " pm " << incllambarcrosssectionerr << endl;
	cout << "Lambda Asymm Inclusive Measurement: " << incllamasymm << " pm " << incllamasymmerr << endl;
	cout << "Kaon Inclusive Reconstruction Eff: " << inclK0eff << " pm " << inclK0efferr << endl;
	cout << "Kaon Inclusive Cross Section: " << inclK0crosssection << " pm " << inclK0crosssectionerr << endl;
	cout << "Lambda:Kaon Inclusive Ratio: " << inclratio << " pm " << inclratioerr << endl;
		
	
	////lambda asymmetry
	//for (int k = 0; k < pTentries; k++) {
	//	double templamregeff = (LambdaRegMCCountincl[k][0]) / (LambdaRegGenCount[k]);
	//	double templambareff = (LambdaBarMCCount[k][0]) / (LambdaBarGenCount[k]);
	//	double templamregefferr = GetRatioError(templamregeff, LambdaRegMCCount[k][0], LambdaRegMCCount[k][1], LambdaRegGenCount[k], 0);
	//	double templambarefferr = GetRatioError(templambareff, LambdaBarMCCount[k][0], LambdaBarMCCount[k][1], LambdaBarGenCount[k], 0);
	//	double templamreg = (LambdaRegDataCount[k][0]) / (templamregeff);
	//	double templambar = (LambdaBarDataCount[k][0]) / (templambareff);
	//	double templamregerr = GetRatioError((LambdaRegDataCount[k][0]) / (templamregeff), (LambdaRegDataCount[k][0]), (LambdaRegDataCount[k][1]), (templamregeff), templamregefferr);
	//	double templambarerr = GetRatioError((LambdaBarDataCount[k][0]) / (templambareff), (LambdaBarDataCount[k][0]), (LambdaBarDataCount[k][1]), (templambareff), templambarefferr);
	//	y1[k] = (templamreg - templambar) / ((templamreg + templambar));
	//	y1err[k] = GetRatioError(((templamreg - templambar) / ((templamreg + templambar))), (templamreg - templambar), (templamregerr + templambarerr), ((templamreg + templambar)), ((templamregerr + templambarerr)));

	//	//cout << k+1 << " Lambda reg eff err: " << templamregefferr << endl;
	//	//cout << k+1 << " Lambda bar eff err: " << templambarefferr << endl;
	//	//cout << k+1 << " Lambda Reg err: " << templamregerr << endl;
	//	//cout << k+1 << " Lambda Bar err: " << templambarerr << endl;
	//	//cout << k+1 << " Asymm err: " << y1err[k] << endl;

	//	effregplot[k] = templamregeff;
	//	effbarplot[k] = templambareff;
	//	erreffregplot[k] = templamregefferr;
	//	erreffbarplot[k] = templambarefferr;
	//}

	////Kaon cross section
	//for (int k = 0; k < pTentries; k++) {
	//	double tempk0eff = (KaonMCCount[k][0]) / (KaonGenCount[k]);

	//	double tempk0efferr = GetRatioError(tempk0eff, KaonMCCount[k][0], KaonMCCount[k][1], KaonGenCount[k], 0);
	//	double tempk0crosssection = (KaonDataCount[k][0]) / (tempk0eff * BRkaon * DataIntLumi06 * (pTbins[k + 1] - pTbins[k]));

	//	double tempk0crosssectionerr = GetRatioError(tempk0crosssection, (KaonDataCount[k][0]), (KaonDataCount[k][1]), (tempk0eff * BRkaon * DataIntLumi06 * (pTbins[k + 1] - pTbins[k])), tempk0efferr);

	//	y1[k] = tempk0crosssection;
	//	y1err[k] = tempk0crosssectionerr;
	//	effregplot[k] = tempk0eff;
	//	erreffregplot[k] = tempk0efferr;

	//}

	////kaon:lambda ratio
	//for (int k = 0; k < pTentries; k++) {
	//	double tempk0eff = (KaonMCCount[k][0]) / (KaonGenCount[k]);

	//	double tempk0efferr = GetRatioError(tempk0eff, KaonMCCount[k][0], KaonMCCount[k][1], KaonGenCount[k], 0);
	//	double tempk0crosssection = (KaonDataCount[k][0]) / (tempk0eff * BRkaon * DataIntLumi06 * (pTbins[k + 1] - pTbins[k]));

	//	double tempk0crosssectionerr = GetRatioError(tempk0crosssection, (KaonDataCount[k][0]), (KaonDataCount[k][1]), (tempk0eff * BRkaon * DataIntLumi06 * (pTbins[k + 1] - pTbins[k])), tempk0efferr);

	//	//y1[k] = tempk0crosssection;
	//	//y1err[k] = tempk0crosssectionerr;
	//	//effregplot[k] = tempk0eff;
	//	//erreffregplot[k] = tempk0efferr;
	//	//
	//	double templameff = (LambdaTotMCCount[k][0]) / (LambdaTotGenCount[k]);

	//	double templamefferr = GetRatioError(templameff, LambdaTotMCCount[k][0], LambdaTotMCCount[k][1], LambdaTotGenCount[k], 0);
	//	double templamcrosssection = (LambdaTotDataCount[k][0]) / (templameff * BRlambda * DataIntLumi06 * (pTbins[k + 1] - pTbins[k]));

	//	double templamcrosssectionerr = GetRatioError(templamcrosssection, (LambdaTotDataCount[k][0]), (LambdaTotDataCount[k][1]), (templameff * BRlambda * DataIntLumi06 * (pTbins[k + 1] - pTbins[k])), templamefferr);
	//	//y1[k] = templamcrosssection;
	//	//y1err[k] = templamcrosssectionerr;
	//	//effregplot[k] = templameff;
	//	//erreffregplot[k] = templamefferr;
	//	//

	//	double tempratio = (templamcrosssection) / (tempk0crosssection);
	//	double tempratioerr = GetRatioError(tempratio, templamcrosssection, templamcrosssectionerr, tempk0crosssection, tempk0crosssectionerr);
	//	y1[k] = tempratio;
	//	y1err[k] = tempratioerr;

	//}

	outputfile.Write();
	outputfile.Close();

	cout << "Done" << endl;

};