// Code Root Macro 
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
vector<double> xbins = {0.0001, 0.0002, 0.0004, 0.001, 0.01};
vector<double> q2bins = { 5.0, 7.0, 10.0, 15.0, 25.0, 100.0 };
const int pTentries = 9;
const int Etaentries = 10;
const int xentries = 4;
const int q2entries = 5;

//What will be used in analysis, pulled from values above
const int variableentries = Etaentries;
vector<double> bins = { -1.3, -1.0, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1.0, 1.3 };
//string names for file paths of 5 Trees we need to access
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DATA
string filekaonTree = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/KaonTree/2006/merged.root";

string filelambdaTree = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/LambdaTree/2006/merged.root";
//END DATA
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MC
string filekaonTreeMC = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/RAPGAP31/2006/merged.root";


string filelambdaTreeMC = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2006/merged.root";


string filegenMCTree = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/genMCTree_Rad/RAPGAP31/2006/merged.root"; //rad RAPGAP


string fileNonRadgenMCTree = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/genMCTree_NonRad/DJANGOH14/2006/merged.root"; //non rad DJANGOH
string fileRadgenMCTree = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/genMCTree_Rad/DJANGOH14/2006/merged.root";
//END MC
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//output files
string outputfilename = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/AnalysisEta.root";
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

//Defining functions used for calculations
double Square(double value) {
	return value * value;
}

double GetRatioError(double ratio, double numerator, double errnumerator, double denominator, double errdenominator) {
	return abs(ratio) * sqrt(Square(errnumerator / numerator) + Square(errdenominator / denominator));
}


//creating file to output histograms to
TFile outputfile(outputfilename.c_str(), "RECREATE");
TFile* f = new TFile(dummyfile.c_str(),"RECREATE");

//MLP function, this function does the mlp fit and peak extraction, output is a 3 component vecotr {total particle peak count after BG subtraction, error in the peak count, total BG count}
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

//This function takes in raw tree from TTree input files and creates histograms with appropriate binning for peak extractions
//after it has all histrograms for appropriate bins, it extracts counts with mlpfunciton, output is a vector of mlpfunciton vector outputs
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
								if (Eta > binsvector[j] && Eta < binsvector[j + 1]) {
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

//This takes the raw gen MC trees and gets MC total counts for efficiency calculation in the appropraiate bin defined by inputs, function output is the count
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
										if (Eta > binsvector[j] && Eta < binsvector[j + 1]) {
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
											if (Eta > binsvector[j] && Eta < binsvector[j + 1]) {
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

//Function plots data
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
	//mgcs->Add(xyscan, "AP");
	//mgcs->Add(xyscan2, "AP");
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
	//legend2->AddEntry(xyscan, "1999-2000 H1 Analysis", "l");
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



void AnalysisEta() {
	
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
	
	// defining arrays of histograms corresponding to particles within kinematic bins
	//must be done for each particle, for data
	
	//Bjorken x data
	vector<TH1F*> LambdaTotData;
	vector<TH1F*> LambdaRegData;
	vector<TH1F*> LambdaBarData;
	vector<TH1F*> LambdaTotMC;
	vector<TH1F*> LambdaRegMC;
	vector<TH1F*> LambdaBarMC;
	vector<TH1F*> KaonData;
	vector<TH1F*> KaonMC;

	//I don't have Histograms for particle level generator data because counts for these
	//will be done by just going through the generate TTree and adding events up that are within our conditions
	for (int a = 0; a < variableentries; a++) {
		string namestr1 = "Lambda Tot Data Eta Bin " + to_string(a + 1);
		string namestr2 = "Lambda Reg Data Eta Bin " + to_string(a + 1);
		string namestr3 = "Lambda Bar Data Eta Bin " + to_string(a + 1);
		string namestr4 = "Lambda Tot MC Eta Bin " + to_string(a + 1);
		string namestr5 = "Lambda Reg MC Eta Bin " + to_string(a + 1);
		string namestr6 = "Lambda Bar MC Eta Bin " + to_string(a + 1);
		string namestr7 = "Kaon Data Eta Bin " + to_string(a + 1);
		string namestr8 = "Kaon MC Eta Bin " + to_string(a + 1);

		
		//LambdaTotDatapT[a] = new TH1F(namestr1.c_str(), namestr1.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange);
		LambdaTotData.push_back(new TH1F(namestr1.c_str(), namestr1.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaRegData.push_back(new TH1F(namestr2.c_str(), namestr2.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarData.push_back(new TH1F(namestr3.c_str(), namestr3.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaTotMC.push_back(new TH1F(namestr4.c_str(), namestr4.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaRegMC.push_back(new TH1F(namestr5.c_str(), namestr5.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarMC.push_back(new TH1F(namestr6.c_str(), namestr6.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		KaonData.push_back(new TH1F(namestr7.c_str(), namestr7.c_str(), kaonbins, kaonlowerbinrange, kaonupperbinrange));
		KaonMC.push_back(new TH1F(namestr8.c_str(), namestr8.c_str(), kaonbins, kaonlowerbinrange, kaonupperbinrange));
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
	//Gen particle counting for Rad Corrections
	vector<double> LambdaGenCountRad;
	vector<double> KaonGenCountRad;
	vector<double> LambdaGenCountNonRad;
	vector<double> KaonGenCountNonRad;

	//Histogram vector names
	//vector<TH1F*> LambdaTotDatapT;
	//vector<TH1F*> LambdaRegDatapT;
	//vector<TH1F*> LambdaBarDatapT;
	//vector<TH1F*> LambdaTotMCpT;
	//vector<TH1F*> LambdaRegMCpT;
	//vector<TH1F*> LambdaBarMCpT;
	//vector<TH1F*> KaonDatapT;
	//vector<TH1F*> KaonMCpT;

	//Analyzing Tot Lambda Counts for Eta bins for MC and Data
	LambdaTotDataCount = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Tot", variableentries, 0, LambdaTotData, bins, "Lambda Tot Data Eta Extraction ", c3, c1);
	LambdaTotMCCount = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Tot", variableentries, 0, LambdaTotMC, bins, "Lambda Tot MC Eta Extraction ", c3, c1);

	LambdaRegDataCount = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Reg", variableentries, 0, LambdaRegData, bins, "Lambda Reg Data Eta Extraction ", c3, c1);
	LambdaRegMCCount = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Reg", variableentries, 0, LambdaRegMC, bins, "Lambda Reg MC Eta Extraction ", c3, c1);

	LambdaBarDataCount = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Bar", variableentries, 0, LambdaBarData, bins, "Lambda Bar Data Eta Extraction ", c3, c1);
	LambdaBarMCCount = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Bar", variableentries, 0, LambdaBarMC, bins, "Lambda Bar MC Eta Extraction ", c3, c1);

	KaonDataCount = AnalyzeTree(new TFile(filekaonTree.c_str(), "read"), "K0", variableentries, 1, KaonData, bins, "Kaon Tot Data Eta Extraction ", c3, c1);
	KaonMCCount = AnalyzeTree(new TFile(filekaonTreeMC.c_str(), "read"), "K0", variableentries, 1, KaonMC, bins, "Kaon Tot MC Eta Extraction ", c3, c1);

	//Analyzing Gen Counts
	LambdaTotGenCount = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Tot Channel", variableentries, bins);
	LambdaRegGenCount = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Reg Channel", variableentries, bins);
	LambdaBarGenCount = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Bar Channel", variableentries, bins);
	KaonGenCount = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "K0s Channel", variableentries, bins);
	
	//Analyzing Gen counts for radiative and non Radiative MC
	//Non Radiative Gen Counting for Radiative Corrections
	//LambdaTotGenCountinclNonRad = AnalyzeGenTreeRadCorr(new TFile(fileNonRadgenMCTree.c_str(), "read"), "Lambda Tot", 1, { bins[0],bins[variableentries] });
	//LambdaTotGenCountinclRad = AnalyzeGenTreeRadCorr(new TFile(filegenMCTree.c_str(), "read"), "Lambda Tot", 1, { bins[0],bins[variableentries] });
	//KaonGenCountinclNonRad = AnalyzeGenTreeRadCorr(new TFile(fileNonRadgenMCTree.c_str(), "read"), "K0s", 1, { bins[0],bins[variableentries] });
	//KaonGenCountinclRad = AnalyzeGenTreeRadCorr(new TFile(filegenMCTree.c_str(), "read"), "K0s", 1, { bins[0],bins[variableentries] });
	KaonGenCountRad = AnalyzeGenTreeRadCorr(new TFile(fileRadgenMCTree.c_str(), "read"), "K0s", variableentries, bins);
	KaonGenCountNonRad = AnalyzeGenTreeRadCorr(new TFile(fileNonRadgenMCTree.c_str(), "read"), "K0s", variableentries, bins);
	LambdaGenCountRad = AnalyzeGenTreeRadCorr(new TFile(fileRadgenMCTree.c_str(), "read"), "Lambda Tot", variableentries, bins);
	LambdaGenCountNonRad = AnalyzeGenTreeRadCorr(new TFile(fileNonRadgenMCTree.c_str(), "read"), "Lambda Tot", variableentries, bins);

	cout << "KaonGenCountRad: " << KaonGenCountRad[0] << endl;
	cout << "KaonGenCountNonRad: " << KaonGenCountNonRad[0] << endl;
	cout << "LambdaGenCountRad: " << LambdaGenCountRad[0] << endl;
	cout << "LambdaGenCountNonRad: " << LambdaGenCountNonRad[0] << endl;

	double y1[variableentries] = {};
	double ourresultsoverpaper[variableentries] = {};
	double ourresultsoverpaperk0[variableentries] = {};
	//vector<double> pTbins = { 0.5, 0.6, 0.7, 0.8, 0.9, 1.1, 1.3, 1.6, 2.2, 3.5 };
	//vector<double> Etabins = { -1.3, -1.0, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1.0, 1.3 };
	//vector<double> xbins = { 0.0001, 0.0002, 0.0004, 0.001, 0.01 };
	//vector<double> q2bins = { 5.0, 7.0, 10.0, 15.0, 25.0, 100.0 };
	//const int pTentries = 9;
	//const int Etaentries = 10;
	//const int xentries = 4;
	//const int q2entries = 5;

	double xerr[variableentries] = { 0.15, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.15 };
	double xvals[variableentries] = { -1.15, -0.875, -0.625, -0.375, -0.125, 0.125, 0.375, 0.625, 0.875, 1.15 };
	double y1err[variableentries] = {};
	//double q2[variableentries] = { 0.55,0.65,0.75,0.85,1.0,1.2,1.45,1.9,2.85 };
	//double dx[variableentries] = { 0.1,0.1,0.1,0.1,0.2,0.2,0.3,0.6,1.3 };

	double effregplot[variableentries] = {};
	double effbarplot[variableentries] = {};
	double erreffregplot[variableentries] = {};
	double erreffbarplot[variableentries] = {};
	double lambdaradcorrplot[variableentries] = {};
	double k0radcorrplot[variableentries] = {};
	double lambdaradcorrploterrors[variableentries] = {};
	
	//Lambda Cross section results from 2009 H1 Paper
	double X[10];
	double y[10];
	double dxlow[10];
	double dxup[10];
	double dylow[10];
	double dyup[10];
	int n = 0;
	X[n] = xvals[n];	y[n] = 2.67;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.07;	dyup[n] = 0.07;	n++;
	X[n] = xvals[n];	y[n] = 2.87;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.07;	dyup[n] = 0.07;	n++;
	X[n] = xvals[n];	y[n] = 3.03;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.075;	dyup[n] = 0.075;	n++;
	X[n] = xvals[n];	y[n] = 2.76;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.065;	dyup[n] = 0.065;	n++;
	X[n] = xvals[n];	y[n] = 2.74;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.075;	dyup[n] = 0.075;	n++;
	X[n] = xvals[n];	y[n] = 2.92;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.075;	dyup[n] = 0.075;	n++;
	X[n] = xvals[n];	y[n] = 2.95;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.07;	dyup[n] = 0.07;	n++;
	X[n] = xvals[n];	y[n] = 3.36;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.085;	dyup[n] = 0.085;	n++;
	X[n] = xvals[n];	y[n] = 3.43;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.075;	dyup[n] = 0.075;	n++;
	X[n] = xvals[n];	y[n] = 3.88;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.08;	dyup[n] = 0.08;	n++;
	//

	for (int k = 0; k < variableentries; k++) {
		double templameff = (LambdaTotMCCount[k][0]) / (LambdaTotGenCount[k]);
		double templamefferr = GetRatioError(templameff, LambdaTotMCCount[k][0], LambdaTotMCCount[k][1], LambdaTotGenCount[k], 0);
		
		//cout << "Lambda Rad Correction: " << (LambdaTotGenCountinclRad[0] / LambdaTotGenCountinclNonRad[0]) * (MCDjangoh14nonradgen / MCDjangoh14radgen) << endl;
		double templambdaradcorr = (LambdaGenCountRad[k] / LambdaGenCountNonRad[k]) * (MCDjangoh14nonradgen / MCDjangoh14radgen);
		lambdaradcorrplot[k] = (templambdaradcorr-1)*100;
		lambdaradcorrploterrors[k] = 0;
		cout << "Lambda Rad Cor Eta Bin " << k + 1 << ": " << templambdaradcorr << endl;

		double templamcrosssection = (LambdaTotDataCount[k][0]) / (templameff * BRlambda * DataIntLumi06 * templambdaradcorr * (bins[k+1]-bins[k]));
		//cout << "Lambda Tot Data Count: " << LambdaTotDataCount[k][0] << endl;
		//cout << "Lambda Bin Cross Section: " << templamcrosssection << endl;
		//cout << "Lambda BR: " << BRlambda << endl;
		//cout << "Lambda Bin Eff: " << templameff << endl;
		//cout << "Lambda Tot Data Lumi: " << DataIntLumi06 << endl;
		//cout << "Bin Width: " << bins[k + 1] - bins[k] << endl;
		cout << "Our Results Over H1 Results in Bin " << k+1 << " : " << templamcrosssection/y[k] << endl;
		ourresultsoverpaper[k] = templamcrosssection / y[k];

		double templamcrosssectionerr = GetRatioError(templamcrosssection, (LambdaTotDataCount[k][0]), (LambdaTotDataCount[k][1]), (templameff * BRlambda * DataIntLumi06 * (bins[k + 1] - bins[k])), templamefferr);
		
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
	/*double X[5];
	double y[5];
	double dxlow[5];
	double dxup[5];
	double dylow[5];
	double dyup[5];
	int n = 0;
	X[n] = xvals[n];	y[n] = 0.462;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.009;	dyup[n] = 0.009;	n++;
	X[n] = xvals[n];	y[n] = 0.282;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.006;	dyup[n] = 0.006;	n++;
	X[n] = xvals[n];	y[n] = 0.153;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.003;	dyup[n] = 0.003;	n++;
	X[n] = xvals[n];	y[n] = 0.071;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0015;	dyup[n] = 0.0015;	n++;
	X[n] = xvals[n];	y[n] = 0.0120;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0002;	dyup[n] = 0.0002;	n++;*/
	////
	
	double dylow2[10];
	double dyup2[10];
	int m = 0;
	dylow2[m] = 0.15;	dyup2[m] = 0.15;	m++;
	dylow2[m] = 0.17;	dyup2[m] = 0.16;	m++;
	dylow2[m] = 0.17;	dyup2[m] = 0.17;	m++;
	dylow2[m] = 0.18;	dyup2[m] = 0.18;	m++;
	dylow2[m] = 0.20;	dyup2[m] = 0.19;	m++;
	dylow2[m] = 0.22;	dyup2[m] = 0.21;	m++;
	dylow2[m] = 0.22;	dyup2[m] = 0.21;	m++;
	dylow2[m] = 0.23;	dyup2[m] = 0.23;	m++;
	dylow2[m] = 0.25;	dyup2[m] = 0.25;	m++;
	dylow2[m] = 0.31;	dyup2[m] = 0.28;	m++;
	
	

	//Plotting Lambda Efficiency
	TCanvas* ceff = new TCanvas("Lambda Cross Section vs Eta", "Lambda Cross Section vs Eta", 10, 10, 800, 400);
	ceff->SetName("Lambda Reconstruction Eff vs Eta");

	auto effreg = new TGraphErrors(variableentries, X, effregplot, xerr, erreffregplot);
	//auto effbar = new TGraphErrors(arraysize, X, effbarplot, xerr, erreffbarplot);
	effreg->SetMarkerColor(kRed);
	effreg->SetLineColor(kRed);
	effreg->SetMarkerSize(0.01);
	//effbar->SetMarkerColor(kBlue);
	//effbar->SetLineColor(kBlue);
	//effbar->SetMarkerSize(0.5);

	effreg->SetTitle("#Lambda Reconstruction Eff vs Eta");
	effreg->GetXaxis()->SetTitle("Eta");
	effreg->GetYaxis()->SetTitle("Efficiency");
	effreg->Draw();
	//effbar->Draw("SAME");
	TLegend* legend3 = new TLegend();
	legend3->AddEntry(effreg, "#Lambda", "l");
	//legend3->AddEntry(effbar, "#bar{#Lambda}", "l");
	legend3->Draw("SAME");
	outputfile.cd();
	ceff->Write();
	f->cd();
	//End Plotting Lambda Efficiency

	auto cs = new TGraphErrors(variableentries, X, y1, xerr, y1err);
	PlotData("Lambda Cross Section vs Eta", "d#sigma(ep #rightarrow e [#Lambda + #bar{#Lambda}] X)/dx", "Eta", "d#sigma/dx",0,4.2, cs, variableentries, X, y, xerr, y1err, dylow, dyup, dylow2, dyup2);
	for (int k = 0; k < variableentries; k++) {
		cout << "Lambda Eta Cross Section Entry " << k + 1 << " : " << y1[k] << " X: " << X[k] << "p/m: " << y1err[k] << endl;
	}
	//auto g = new TGraph(5, X, ourresultsoverpaper);
	//g->SetTitle("Our Results Over Paper's;Q^{2};Cross Section Ratio");
	//g->Draw("AC*");
	//outputfile.cd();
	//g->Write();
	//f->cd();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Lambda Asymm Data from 2009 paper obtained through xyscan, no syst error published for this, so just stat. error
	
	n = 0;

	X[n] = xvals[n];	y[n] = -0.0272;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.049;	dyup[n] = 0.049;	n++;
	X[n] = xvals[n];	y[n] = 0.033887;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0438;	dyup[n] = 0.0438;	n++;
	X[n] = xvals[n];	y[n] = 0.0578;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.04916;	dyup[n] = 0.04916;	n++;
	X[n] = xvals[n];	y[n] = 0.021926;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.04916;	dyup[n] = 0.04916;	n++;
	X[n] = xvals[n];	y[n] = -0.0299;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0558;	dyup[n] = 0.0558;	n++;
	X[n] = xvals[n];	y[n] = -0.071096;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0465;	dyup[n] = 0.0465;	n++;
	X[n] = xvals[n];	y[n] = 0.003322;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0451;	dyup[n] = 0.0451;	n++;
	X[n] = xvals[n];	y[n] = 0.00598;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0478;	dyup[n] = 0.0478;	n++;
	X[n] = xvals[n];	y[n] = -0.0006644;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0411;	dyup[n] = 0.0411;	n++;
	X[n] = xvals[n];	y[n] = -0.0259;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0398;	dyup[n] = 0.0398;	n++;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	

	for (int k = 0; k < variableentries; k++) {
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

	auto cs2 = new TGraphErrors(variableentries, X, y1, xerr, y1err);
	PlotData("Lambda Asymm vs Eta", "[#sigma(ep #rightarrow e #Lambda X) - #sigma(ep #rightarrow e #bar{#Lambda} X)] / [#sigma(ep #rightarrow e #Lambda X) + #sigma(ep #rightarrow e #bar{#Lambda} X)]", "Eta", "A_{#Lambda}",-0.3,0.3, cs2, variableentries, X, y, xerr, y1err, dylow, dyup, dylow, dyup);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//K0 Cross section results from 2009 H1 Paper
	n = 0;
	X[n] = xvals[n];	y[n] = 8.08;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.06;	dyup[n] = 0.06;	n++;
	X[n] = xvals[n];	y[n] = 8.69;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.065;	dyup[n] = 0.065;	n++;
	X[n] = xvals[n];	y[n] = 8.64;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.06;	dyup[n] = 0.06;	n++;
	X[n] = xvals[n];	y[n] = 8.56;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.065;	dyup[n] = 0.065;	n++;
	X[n] = xvals[n];	y[n] = 8.79;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.08;	dyup[n] = 0.08;	n++;
	X[n] = xvals[n];	y[n] = 8.65;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.07;	dyup[n] = 0.07;	n++;
	X[n] = xvals[n];	y[n] = 7.58;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.065;	dyup[n] = 0.065;	n++;
	X[n] = xvals[n];	y[n] = 7.99;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.065;	dyup[n] = 0.065;	n++;
	X[n] = xvals[n];	y[n] = 7.98;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.075;	dyup[n] = 0.075;	n++;
	X[n] = xvals[n];	y[n] = 8.06;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.065;	dyup[n] = 0.065;	n++;
	//
	m = 0;
	dylow2[m] = 0.42;	dyup2[m] = 0.41;	m++;
	dylow2[m] = 0.52;	dyup2[m] = 0.51;	m++;
	dylow2[m] = 0.46;	dyup2[m] = 0.44;	m++;
	dylow2[m] = 0.50;	dyup2[m] = 0.47;	m++;
	dylow2[m] = 0.59;	dyup2[m] = 0.56;	m++;
	dylow2[m] = 0.60;	dyup2[m] = 0.58;	m++;
	dylow2[m] = 0.52;	dyup2[m] = 0.52;	m++;
	dylow2[m] = 0.56;	dyup2[m] = 0.55;	m++;
	dylow2[m] = 0.54;	dyup2[m] = 0.54;	m++;
	dylow2[m] = 0.54;	dyup2[m] = 0.54;	m++;


	
	for (int k = 0; k < variableentries; k++) {
		double tempk0eff = (KaonMCCount[k][0]) / (KaonGenCount[k]);
		double tempk0efferr = GetRatioError(tempk0eff, KaonMCCount[k][0], KaonMCCount[k][1], KaonGenCount[k], 0);

		//cout << "K0s Rad Correction: " << (KaonGenCountinclRad[0] / KaonGenCountinclNonRad[0]) * (MCDjangoh14nonradgen / MCDjangoh14radgen) << endl;
		double tempk0radcorr = (KaonGenCountRad[k] / KaonGenCountNonRad[k]) * (MCDjangoh14nonradgen / MCDjangoh14radgen);
		k0radcorrplot[k] = (tempk0radcorr-1)*100;
		cout << "K0 Rad Cor Eta Bin " << k + 1 << ": " << tempk0radcorr << endl;

		double tempk0crosssection = (KaonDataCount[k][0]) / (tempk0eff * BRkaon * DataIntLumi06 * tempk0radcorr * (bins[k + 1] - bins[k]));

		double tempk0crosssectionerr = GetRatioError(tempk0crosssection, (KaonDataCount[k][0]), (KaonDataCount[k][1]), (tempk0eff * BRkaon * DataIntLumi06 * (bins[k + 1] - bins[k])), tempk0efferr);
		
		cout << "stat err: " << tempk0crosssectionerr << endl;

		y1[k] = tempk0crosssection;
		y1err[k] = tempk0crosssectionerr;
		effregplot[k] = tempk0eff;
		erreffregplot[k] = tempk0efferr;

		ourresultsoverpaperk0[k] = tempk0crosssection / y[k];

	}

	//auto gk0 = new TGraph(5, X, ourresultsoverpaperk0);
	//TCanvas* cgk = new TCanvas("Results Ratio", "Results Ratio", 10, 10, 800, 400);
	//cgk->SetName("Results Ratio");
	//gk0->SetTitle("Our Results Over Paper's;Q^{2};Cross Section Ratio");
	//gk0->Draw();
	//gk0->SetLineColor(kBlack);
	//g->SetLineColor(kRed);
	//g->Draw("SAME");
	//g->SetLineStyle("AC*")
	//TLegend* legendgk = new TLegend();
	//legendgk->AddEntry(g, "#Lambda HERA1:HERA2", "l");
	//legendgk->AddEntry(gk0, "K^{0}_{s} HERA1:HERA2", "l");
	//legend3->AddEntry(effbar, "#bar{#Lambda}", "l");
	//legendgk->Draw("SAME");
	//outputfile.cd();
	//cgk->Write();
	//gk0->Write();
	//f->cd();

	//Plotting K0 Efficiency
	TCanvas* ckeff = new TCanvas("K0 Cross Section vs Eta", "K0 Cross Section vs Eta", 10, 10, 800, 400);
	ckeff->SetName("K0 Reconstruction Eff vs Eta");

	auto effreg2 = new TGraphErrors(variableentries, X, effregplot, xerr, erreffregplot);
	//auto effbar = new TGraphErrors(arraysize, X, effbarplot, xerr, erreffbarplot);
	effreg2->SetMarkerColor(kRed);
	effreg2->SetLineColor(kRed);
	effreg2->SetMarkerSize(0.01);
	//effbar->SetMarkerColor(kBlue);
	//effbar->SetLineColor(kBlue);
	//effbar->SetMarkerSize(0.5);

	effreg2->SetTitle("K^{0}_{S} Reconstruction Eff vs Eta");
	effreg2->GetXaxis()->SetTitle("Eta");
	effreg2->GetYaxis()->SetTitle("Efficiency");
	effreg2->Draw();
	//effbar->Draw("SAME");
	TLegend* legend4 = new TLegend();
	legend4->AddEntry(effreg2, "K^{0}_{S}", "l");
	//legend3->AddEntry(effbar, "#bar{#Lambda}", "l");
	legend4->Draw("SAME");
	outputfile.cd();
	ckeff->Write();
	//End Plotting K0 Efficiency

	//Plotting radiative Corrections
	TCanvas* cradcorr = new TCanvas("Radiative Corrections", "Radiative Corrections", 10, 10, 800, 400);
	cradcorr->SetName("Radiaitve Corrections vs Eta");

	auto radcorrlambda = new TGraphErrors(variableentries, X, lambdaradcorrplot, xerr, lambdaradcorrploterrors);
	auto radcorrk0 = new TGraphErrors(variableentries, X, k0radcorrplot, xerr, lambdaradcorrploterrors);
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

	radcorrlambda->SetTitle("Radiative Corrections vs Eta");
	radcorrlambda->GetXaxis()->SetTitle("Eta");
	radcorrlambda->GetYaxis()->SetTitle("#delta_{QED} [%]");
	//radcorrlambda->Draw();
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

	auto cs3 = new TGraphErrors(variableentries, X, y1, xerr, y1err);
	PlotData("K0 Cross Section vs Eta", "d#sigma(ep #rightarrow e K^{0}_{S} X)/dx", "Eta", "d#sigma/dx [nb]",0,10.5, cs3, variableentries, X, y, xerr, y1err, dylow, dyup, dylow2, dyup2);
	for (int k = 0; k < variableentries; k++) {
		cout << "K0 Eta Cross Section Entry " << k + 1 << " : " << y1[k] << " X: " << X[k] << "p/m: " << y1err[k] << endl;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//K0/Lambda Ratio Cross section results from 2009 H1 Paper
	n = 0;
	X[n] = xvals[n];	y[n] = 0.331;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.009;	dyup[n] = 0.009;	n++;
	X[n] = xvals[n];	y[n] = 0.330;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0085;	dyup[n] = 0.0085;	n++;
	X[n] = xvals[n];	y[n] = 0.350;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.009;	dyup[n] = 0.009;	n++;
	X[n] = xvals[n];	y[n] = 0.323;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.008;	dyup[n] = 0.008;	n++;
	X[n] = xvals[n];	y[n] = 0.311;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.009;	dyup[n] = 0.009;	n++;
	X[n] = xvals[n];	y[n] = 0.337;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.009;	dyup[n] = 0.009;	n++;
	X[n] = xvals[n];	y[n] = 0.389;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0095;	dyup[n] = 0.0095;	n++;
	X[n] = xvals[n];	y[n] = 0.420;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0115;	dyup[n] = 0.0115;	n++;
	X[n] = xvals[n];	y[n] = 0.430;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.010;	dyup[n] = 0.010;	n++;
	X[n] = xvals[n];	y[n] = 0.480;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.010;	dyup[n] = 0.010;	n++;
	
	m = 0;
	dylow2[m] = 0.015;	dyup2[m] = 0.015;	m++;
	dylow2[m] = 0.018;	dyup2[m] = 0.018;	m++;
	dylow2[m] = 0.014;	dyup2[m] = 0.013;	m++;
	dylow2[m] = 0.016;	dyup2[m] = 0.015;	m++;
	dylow2[m] = 0.017;	dyup2[m] = 0.017;	m++;
	dylow2[m] = 0.018;	dyup2[m] = 0.018;	m++;
	dylow2[m] = 0.018;	dyup2[m] = 0.017;	m++;
	dylow2[m] = 0.015;	dyup2[m] = 0.014;	m++;
	dylow2[m] = 0.018;	dyup2[m] = 0.017;	m++;
	dylow2[m] = 0.02;	dyup2[m] = 0.02;	m++;



	for (int k = 0; k < variableentries; k++) {
		double tempk0eff = (KaonMCCount[k][0]) / (KaonGenCount[k]);

		double tempk0efferr = GetRatioError(tempk0eff, KaonMCCount[k][0], KaonMCCount[k][1], KaonGenCount[k], 0);
		double tempk0crosssection = (KaonDataCount[k][0]) / (tempk0eff * BRkaon * DataIntLumi06 * (bins[k + 1] - bins[k]));

		double tempk0crosssectionerr = GetRatioError(tempk0crosssection, (KaonDataCount[k][0]), (KaonDataCount[k][1]), (tempk0eff * BRkaon * DataIntLumi06 * (bins[k + 1] - bins[k])), tempk0efferr);

		//y1[k] = tempk0crosssection;
		//y1err[k] = tempk0crosssectionerr;
		//effregplot[k] = tempk0eff;
		//erreffregplot[k] = tempk0efferr;
		//
		double templameff = (LambdaTotMCCount[k][0]) / (LambdaTotGenCount[k]);

		double templamefferr = GetRatioError(templameff, LambdaTotMCCount[k][0], LambdaTotMCCount[k][1], LambdaTotGenCount[k], 0);
		double templamcrosssection = (LambdaTotDataCount[k][0]) / (templameff * BRlambda * DataIntLumi06 * (bins[k + 1] - bins[k]));

		double templamcrosssectionerr = GetRatioError(templamcrosssection, (LambdaTotDataCount[k][0]), (LambdaTotDataCount[k][1]), (templameff * BRlambda * DataIntLumi06 * (bins[k + 1] - bins[k])), templamefferr);
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

	auto cs4 = new TGraphErrors(variableentries, X, y1, xerr, y1err);
	PlotData("Lambda/K0 Cross Section vs Eta", "[d#sigma(ep #rightarrow e #Lambda X)]/[d#sigma(ep #rightarrow e K^{0}_{S} X)]", "Eta", "#Lambda/K^{0}_{S}",0,0.51, cs4, variableentries, X, y, xerr, y1err, dylow, dyup, dylow2, dyup2);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Now doing peak extractions and analysis for inclusive measurements
	vector<TH1F*> LambdaTotDataxincl;
	vector<TH1F*> LambdaRegDataxincl;
	vector<TH1F*> LambdaBarDataxincl;
	vector<TH1F*> LambdaTotMCxincl;
	vector<TH1F*> LambdaRegMCxincl;
	vector<TH1F*> LambdaBarMCxincl;
	vector<TH1F*> KaonDataxincl;
	vector<TH1F*> KaonMCxincl;
	//I don't have Histograms for particle level generator data because counts for these
	//will be done by just going through the generate TTree and adding events up that are within our conditions
	for (int a = 0; a < 1; a++) {
		string namestr1 = "Lambda Tot Data";
		string namestr2 = "Lambda Reg Data Eta Bin";
		string namestr3 = "Lambda Bar Data Eta Bin";
		string namestr4 = "Lambda Tot MC Eta Bin";
		string namestr5 = "Lambda Reg MC Eta Bin";
		string namestr6 = "Lambda Bar MC Eta Bin";
		string namestr7 = "Kaon Data Eta Bin";
		string namestr8 = "Kaon MC Eta Bin";


		//LambdaTotDataQ2[a] = new TH1F(namestr1.c_str(), namestr1.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange);
		LambdaTotDataxincl.push_back(new TH1F(namestr1.c_str(), namestr1.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaRegDataxincl.push_back(new TH1F(namestr2.c_str(), namestr2.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarDataxincl.push_back(new TH1F(namestr3.c_str(), namestr3.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaTotMCxincl.push_back(new TH1F(namestr4.c_str(), namestr4.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaRegMCxincl.push_back(new TH1F(namestr5.c_str(), namestr5.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarMCxincl.push_back(new TH1F(namestr6.c_str(), namestr6.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		KaonDataxincl.push_back(new TH1F(namestr7.c_str(), namestr7.c_str(), kaonbins, kaonlowerbinrange, kaonupperbinrange));
		KaonMCxincl.push_back(new TH1F(namestr8.c_str(), namestr8.c_str(), kaonbins, kaonlowerbinrange, kaonupperbinrange));
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
	vector<double> LambdaTotGenCountinclNonRad;
	vector<double> LambdaTotGenCountinclRad;
	vector<double> KaonGenCountinclNonRad;
	vector<double> KaonGenCountinclRad;

	//Analyzing Tot Lambda Counts for Eta bins for MC and Data
	//vector<vector<double>> AnalyzeTree(TFile * Data, string TreeName, int binentries, int particlecode, vector<TH1F*> HistArray, vector<double> binsvector, string extractionReadback, TCanvas * c3, TCanvas * c1) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

	LambdaTotDataCountincl = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Tot", 1, 0, LambdaTotDataxincl, { bins[0],bins[variableentries] }, "Lambda Tot Incl Data Eta Extraction ", c3, c1);
	LambdaTotMCCountincl = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Tot", 1, 0, LambdaTotMCxincl, { bins[0],bins[variableentries] }, "Lambda Tot Incl MC Eta Extraction ", c3, c1);

	LambdaRegDataCountincl = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegDataxincl, { bins[0],bins[variableentries] }, "Lambda Reg Incl Data Eta Extraction ", c3, c1);
	LambdaRegMCCountincl = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegMCxincl, { bins[0],bins[variableentries] }, "Lambda Reg Incl MC Eta Extraction ", c3, c1);

	LambdaBarDataCountincl = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarDataxincl, { bins[0],bins[variableentries] }, "Lambda Bar Incl Data Eta Extraction ", c3, c1);
	LambdaBarMCCountincl = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarMCxincl, { bins[0],bins[variableentries] }, "Lambda Bar Incl MC Eta Extraction ", c3, c1);

	KaonDataCountincl = AnalyzeTree(new TFile(filekaonTree.c_str(), "read"), "K0", 1, 1, KaonDataxincl, { bins[0],bins[variableentries] }, "Kaon Incl Data Eta Extraction ", c3, c1);
	KaonMCCountincl = AnalyzeTree(new TFile(filekaonTreeMC.c_str(), "read"), "K0", 1, 1, KaonMCxincl, { bins[0],bins[variableentries] }, "Kaon Incl MC Eta Extraction ", c3, c1);

	
	//LambdaTotDataCountincl = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Tot", 1, 0, LambdaTotDataxincl, { 10,bins[variableentries] }, "Lambda Tot Incl Data Eta Extraction ", c3, c1);
	//LambdaTotMCCountincl = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Tot", 1, 0, LambdaTotMCxincl, { 10,bins[variableentries] }, "Lambda Tot Incl MC Eta Extraction ", c3, c1);

	//LambdaRegDataCountincl = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegDataxincl, { 10,bins[variableentries] }, "Lambda Reg Incl Data Eta Extraction ", c3, c1);
	//LambdaRegMCCountincl = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegMCxincl, { 10,bins[variableentries] }, "Lambda Reg Incl MC Eta Extraction ", c3, c1);

	//LambdaBarDataCountincl = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarDataxincl, { 10,bins[variableentries] }, "Lambda Bar Incl Data Eta Extraction ", c3, c1);
	//LambdaBarMCCountincl = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarMCxincl, { 10,bins[variableentries] }, "Lambda Bar Incl MC Eta Extraction ", c3, c1);

	//KaonDataCountincl = AnalyzeTree(new TFile(filekaonTree.c_str(), "read"), "K0", 1, 1, KaonDataxincl, { 10,bins[variableentries] }, "Kaon Incl Data Eta Extraction ", c3, c1);
	//KaonMCCountincl = AnalyzeTree(new TFile(filekaonTreeMC.c_str(), "read"), "K0", 1, 1, KaonMCxincl, { 10,bins[variableentries] }, "Kaon Incl MC Eta Extraction ", c3, c1);

	//Analyzing Gen Counts
	//vector<double> AnalyzeGenTree(TFile* Data, string TreeName, int binentries, vector<double> binsvector) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

	LambdaTotGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Tot Channel", 1, { bins[0],bins[variableentries] });
	LambdaRegGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Reg Channel", 1, { bins[0],bins[variableentries] });
	LambdaBarGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Bar Channel", 1, { bins[0],bins[variableentries] });
	KaonGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "K0s Channel", 1, { bins[0],bins[variableentries] });

	//Non Radiative Gen Counting for Radiative Corrections
	LambdaTotGenCountinclNonRad = AnalyzeGenTreeRadCorr(new TFile(fileNonRadgenMCTree.c_str(), "read"), "Lambda Tot", 1, { bins[0],bins[variableentries] });
	LambdaTotGenCountinclRad = AnalyzeGenTreeRadCorr(new TFile(fileRadgenMCTree.c_str(), "read"), "Lambda Tot", 1, { bins[0],bins[variableentries] });
	KaonGenCountinclNonRad = AnalyzeGenTreeRadCorr(new TFile(fileNonRadgenMCTree.c_str(), "read"), "K0s", 1, { bins[0],bins[variableentries] });
	KaonGenCountinclRad = AnalyzeGenTreeRadCorr(new TFile(fileRadgenMCTree.c_str(), "read"), "K0s", 1, { bins[0],bins[variableentries] });

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

	//LambdaTotGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Tot", 1, { 10,bins[variableentries] });
	//LambdaRegGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Reg", 1, { 10,bins[variableentries] });
	//LambdaBarGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Bar", 1, { 10,bins[variableentries] });
	//KaonGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "K0s", 1, { 10,bins[variableentries] });

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//lambda reg + lambda bar cross section
	
	double incllameff = (LambdaTotMCCountincl[0][0]) / (LambdaTotGenCountincl[0]);

	double incllamefferr = GetRatioError(incllameff, LambdaTotMCCountincl[0][0], LambdaTotMCCountincl[0][1], LambdaTotGenCountincl[0], 0);
	double incllamcrosssection = (LambdaTotDataCountincl[0][0]) / (incllameff * BRlambda * DataIntLumi06 * lambdatotincradcorr);
	double incllamcrosssectionerr = GetRatioError(incllamcrosssection, (LambdaTotDataCountincl[0][0]), (LambdaTotDataCountincl[0][1]), (incllameff * BRlambda * DataIntLumi06), incllamefferr);

	//MC Lumi Calc
	double MCincllamcrosssection = (LambdaTotGenCountincl[0]) / (MCIntLumi06);

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
	double inclK0crosssection = (KaonDataCountincl[0][0]) / (inclK0eff * BRkaon * DataIntLumi06* k0totincradcorr);
	double inclK0crosssectionerr = GetRatioError(inclK0crosssection, (KaonDataCountincl[0][0]), (KaonDataCountincl[0][1]), (inclK0eff * BRkaon * DataIntLumi06), inclK0efferr);

	double MCinclK0crosssection = (KaonGenCountincl[0]) / (MCIntLumi06);

	//Lambda:K0 Ratio

	double inclratio = incllamcrosssection/ inclK0crosssection;
	double inclratioerr = GetRatioError(incllamcrosssection / inclK0crosssection, incllamcrosssection, incllamcrosssectionerr, inclK0crosssection, inclK0crosssectionerr);


	/////////////////////////////////////////////////////////////////////////////////
	//Creating Histograms from Trees I want to see


	/////////////////////////////////////////////////////////////////////////////////



	cout << "Lambda Total Gen Count: " << LambdaTotGenCountincl[0] << endl;
	cout << "K0 Total Gen Count: " << KaonGenCountincl[0] << endl;
	cout << "Lambda+Lambda Bar Inclusive Gen Cross Section: " << MCincllamcrosssection << endl;
	cout << "Lambda+Lambda Bar Inclusive Gen Cross Section: " << MCinclK0crosssection << endl;
	cout << "Lambda+Lambda Bar Inclusive Reconstruction Eff: " << incllameff << " pm " << incllamefferr << endl;
	cout << "Lambda+Lambda Bar Inclusive Cross Section: " << incllamcrosssection << " pm " << incllamcrosssectionerr << endl;
	cout << "Lambda Reg Inclusive Reconstruction Eff: " << incllamregeff << " pm " << incllamregefferr << endl;
	cout << "Lambda Reg Inclusive Cross Section: " << incllamregcrosssection << " pm " << incllamregcrosssectionerr << endl;
	cout << "Lambda Bar Inclusive Reconstruction Eff: " << incllambareff << " pm " << incllambarefferr << endl;
	cout << "Lambda Bar Inclusive Cross Section: " << incllambarcrosssection << " pm " << incllambarcrosssectionerr << endl;
	cout << "Lambda Asymm Inclusive Measurement: " << incllamasymm << " pm " << incllamasymmerr << endl;
	cout << "Kaon Inclusive Reconstruction Eff: " << inclK0eff << " pm " << inclK0efferr << endl;
	cout << "Kaon Inclusive Cross Section: " << inclK0crosssection << " pm " << inclK0crosssectionerr << endl;
	cout << "Kaon Inclusive Gen Cross Section: " << MCinclK0crosssection << endl;
	cout << "Lambda:Kaon Inclusive Ratio: " << inclratio << " pm " << inclratioerr << endl;
		
	
	

	outputfile.Write();
	outputfile.Close();

	cout << "Done" << endl;

};