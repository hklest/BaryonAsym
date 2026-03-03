//Analysis Code Root Macro 
//Importing TTrees

using namespace std;

////////////////////////////////////////////////
// User Inputs in this section
////////////////////////////////////////////////

//Lambda and Kaon Extraction May require slightly different parameters
//Convecntion here: element[0] = Lambda ; element[1] = Kaon

vector<int> trainnum = { 50,50 }; //number of time NN is trained
vector<string> network = { "Mass:350:Count", "Mass:350:Count" }; //defining NN

//Vectors of fit subtraction values for lambda=0 and k0=1 particles,
vector<double> leftfitbound1 = { 1.082, 0.40 };
vector<double> leftfitbound2 = { 1.1, 0.435 };
vector<double> rightfitbound1 = { 1.128, 0.555 };
vector<double> rightfitbound2 = { 1.15, 0.68 };
vector<double> leftsubbound = { 1.1, 0.44 };
vector<double> rightsubbound = { 1.128, 0.55 };

//defining vectors of the binning definitions for each of the kinematic variables we are looking at
//when playing with binning, have to adjust these vectors and hand specify the number of bins for defining Hist arrays
vector<double> pTbins = {0.5, 0.6, 0.7, 0.8, 0.9, 1.1, 1.3, 1.6, 2.2, 3.5};
vector<double> Etabins = {-1.3, -1.0, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1.0, 1.3};
vector<double> xbins = {0.00004, 0.0001, 0.0002, 0.0004, 0.001, 0.01};
vector<double> Q2ebins = {2.0, 2.5, 3.0, 4.0, 5.0, 7.0, 10.0, 15.0, 25.0, 100.0};
const int pTentries = 9;

//string names for file paths of 5 Trees we need to access
string filelambdaTree = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/lambdaTree06.root";
string filekaonTree = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/kaonTree06.root";

string filelambdaTreeMC = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/lambdaTreeMC06.root";//old
string filekaonTreeMC = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/kaonTreeMC06.root";
string filegenMCTree = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/genMCTree06.root";

string outputfilename = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/AnalysisTest.root";
string dummyfile = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/dummyfile.root";

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
	return ratio * sqrt(Square(errnumerator / numerator) + Square(errdenominator / denominator));
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
	legend->SetHeader(Form("#splitline{#Lambda Count- %.2f #pm %.2f (stat.)}{Stochastic MLP- %s Network Trained %ix}", kcount, kcounterror, network[particlecode].c_str(), trainnum[particlecode])); // option "C" allows to center the header
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
	cs->SetMarkerSize(0.5);
	xyscan->SetMarkerSize(0.5);
	xyscan->SetMarkerStyle(21);
	xyscan2->SetMarkerSize(0.5);
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



void AnalysisTest() {
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


	//Analyzing Tot Lambda Counts for pT bins for MC and Data
	LambdaTotDataCount = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Tot", pTentries, 0, LambdaTotDatapT, pTbins, "Lambda Tot Data pT Extraction ", c3, c1);
	LambdaTotMCCount = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Tot", pTentries, 0, LambdaTotMCpT, pTbins, "Lambda Tot MC pT Extraction ", c3, c1);
	//Analyzing Gen Counts
	LambdaTotGenCount = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Tot", pTentries, pTbins);
	

	
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
	ceff->SetName("Reconstruction Eff vs p_{T}");

	auto effreg = new TGraphErrors(pTentries, X, effregplot, xerr, erreffregplot);
	//auto effbar = new TGraphErrors(arraysize, X, effbarplot, xerr, erreffbarplot);
	effreg->SetMarkerColor(kRed);
	effreg->SetLineColor(kRed);
	effreg->SetMarkerSize(0.5);
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
	PlotData("Lambda Cross Section vs pT", "d#sigma(ep #rightarrow e #Lambda X)/dp_{T}", "p_{T} [GeV]", "d#sigma/dp_{T} [nb/GeV]", cs, pTentries, X, y, xerr, y1err, dylow, dyup, dylow2, dyup2);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	n = 0;
	X[n] = 0.541833;	y[n] = 8.4;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.4;	dyup[n] = 0.4;	n++;
	X[n] = 0.643426;	y[n] = 8.0;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.25;	dyup[n] = 0.25;	n++;
	X[n] = 0.74502;	y[n] = 7.9;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.25;	dyup[n] = 0.25;	n++;
	X[n] = 0.870518;	y[n] = 6.8;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.15;	dyup[n] = 0.15;	n++;
	X[n] = 1.01992;	y[n] = 6.1;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.1;	dyup[n] = 0.1;	n++;
	X[n] = 1.19323;	y[n] = 4.70;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.09;	dyup[n] = 0.09;	n++;
	X[n] = 1.44422;	y[n] = 3.05;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.05;	dyup[n] = 0.05;	n++;
	X[n] = 1.89243;	y[n] = 1.52;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.025;	dyup[n] = 0.025;	n++;
	X[n] = 2.84861;	y[n] = 0.42;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.01;	dyup[n] = 0.01;	n++;

	m = 0;
	dylow2[m] = 0.8;	dyup2[m] = 0.8;	m++;
	dylow2[m] = 0.4;	dyup2[m] = 0.5;	m++;
	dylow2[m] = 0.5;	dyup2[m] = 0.5;	m++;
	dylow2[m] = 0.5;	dyup2[m] = 0.5;	m++;
	dylow2[m] = 0.4;	dyup2[m] = 0.4;	m++;
	dylow2[m] = 0.29;	dyup2[m] = 0.3;	m++;
	dylow2[m] = 0.20;	dyup2[m] = 0.2;	m++;
	dylow2[m] = 0.09;	dyup2[m] = 0.09; m++;
	dylow2[m] = 0.02;	dyup2[m] = 0.02; m++;

	

	for (int k = 0; k < pTentries; k++) {
		double templameff = (LambdaTotMCCount[k][0]) / (LambdaTotGenCount[k]);

		double templamefferr = GetRatioError(templameff, LambdaTotMCCount[k][0], LambdaTotMCCount[k][1], LambdaTotGenCount[k], 0);
		double templamcrosssection = (LambdaTotDataCount[k][0]) / (templameff * BRlambda * DataIntLumi06 * (pTbins[k + 1] - pTbins[k]));

		double templamcrosssectionerr = GetRatioError(templamcrosssection, (LambdaTotDataCount[k][0]), (LambdaTotDataCount[k][1]), (templameff * BRlambda * DataIntLumi06 * (pTbins[k + 1] - pTbins[k])), templamefferr);

		y1[k] = templamcrosssection;
		y1err[k] = templamcrosssectionerr;
		effregplot[k] = templameff;
		erreffregplot[k] = templamefferr;

	}

	PlotData("Lambda Cross Section vs pT", "d#sigma(ep #rightarrow e #Lambda X)/dp_{T}", "p_{T} [GeV]", "d#sigma/dp_{T} [nb/GeV]", cs, pTentries, X, y, xerr, y1err, dylow, dyup, dylow2, dyup2);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	outputfile.Write();
	outputfile.Close();

	cout << "Done" << endl;

};