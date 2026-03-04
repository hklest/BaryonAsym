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
//vector<double> q2bins = {2.0, 2.5, 3.0, 4.0, 5.0, 7.0, 10.0, 15.0, 25.0, 100.0};
vector<double> q2bins = { 5.0, 7.0, 10.0, 15.0, 25.0, 100.0 };
const int pTentries = 9;
const int Etaentries = 10;
const int xentries = 5;
const int q2entries = 5;

//string names for file paths of 5 Trees we need to access
string filelambdaTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/lambdaTree06_Ecra.root";
string filekaonTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/kaonTree06_Ecra.root"; //_electrononly

string filelambdaTreeMC = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/lambdaTreeMC06combined_Ecra.root";
string filekaonTreeMC = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/kaonTreeMC06combined_Ecra.root";
string filegenMCTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree_Ecra.root";

string outputfilename = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/ParticlesPerEvent.root";
string dummyfile = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/dummyfile.root";

string fileDataEvents = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/EventDists06_full.root";
string fileDataMCEvents = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/EventDistsMC.root";

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

int AnalyzeTree(TFile* Data, string TreeName, int particlecode, TH1F* Hist, string canvasname, string histname, string xaxistitle, string yaxistitle, TCanvas* c3) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters
	//vector<vector<double>> ExtractionDataVector = {};

	//have files for lambdaTree, kaonTree, lambdaTreeMC, kaonTreeMC, genMCTree
	//TFile* Data = new TFile(filelambdaTree.c_str(), "read");
	//grab Tree from file above
	TTree* tree = (TTree*)Data->Get(TreeName.c_str());

	int entries;

	entries = tree->GetEntries();

	//Defining the variables that are stored in the Tree branches
	//double invMasslambda, invMasskaon, pT, Eta, x, Q2s, Ys;
	double invMass, pT, Eta, x, Q2s, Ys, Angle, v0chi2;
	string invMassName[2] = {"invMasslambda","invMasskaon"};

	//Defining the branch addresses for the Trees to access these stored variables
	tree->SetBranchAddress(invMassName[particlecode].c_str(), &invMass);
	tree->SetBranchAddress("pT", &pT);
	tree->SetBranchAddress("Eta", &Eta);
	tree->SetBranchAddress("x", &x);
	tree->SetBranchAddress("Q2s", &Q2s);
	tree->SetBranchAddress("Ys", &Ys);
	tree->SetBranchAddress("Angle", &Angle);
	tree->SetBranchAddress("v0chi2", &v0chi2);

	int m;
	//Have to enforce pT and Eta bounds here
	for (m = 0; m < entries; m++) {
		tree->GetEntry(m);
		
			if (pT>0.5 && pT<3.5) {
				if (abs(Eta)<1.3) {
					if (Ys > 0.1 && Ys < 0.6) {
						if (Q2s > 5 && Q2s < 100) {
							if (invMass > leftsubbound[particlecode] && invMass < rightsubbound[particlecode]) {
								Hist->Fill(Q2s);
								//Hist->Fill(invMass);
							}
						}
					}
				}
			}
		
	}
	c3->SetName(canvasname.c_str());

	c3->cd();
	Hist->GetXaxis()->SetTitle(xaxistitle.c_str());
	Hist->GetYaxis()->SetTitle(yaxistitle.c_str());
	Hist->SetNameTitle(histname.c_str(), histname.c_str());
	Hist->Draw();

	outputfile.cd();
	c3->Write();

	outputfile.Write();
	//f->cd();

	return 0;

};


//
int AnalyzeEventTree(TFile* Data, string TreeName, int particlecode, TH1F* Hist, string canvasname, string histname, string xaxistitle, string yaxistitle, TCanvas* c3) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters
	//vector<vector<double>> ExtractionDataVector = {};

	//have files for lambdaTree, kaonTree, lambdaTreeMC, kaonTreeMC, genMCTree
	//TFile* Data = new TFile(filelambdaTree.c_str(), "read");
	//grab Tree from file above
	TTree* tree = (TTree*)Data->Get(TreeName.c_str());

	int entries;

	entries = tree->GetEntries();

	//Defining the variables that are stored in the Tree branches
	//double invMasslambda, invMasskaon, pT, Eta, x, Q2s, Ys;
	double invMass, pT, Eta, x, Q2s, Ys, Angle, v0chi2;
	string invMassName[2] = { "invMasslambda","invMasskaon" };

	//Defining the branch addresses for the Trees to access these stored variables
	tree->SetBranchAddress("Q2s", &Q2s);

	int m;
	//Have to enforce pT and Eta bounds here
	for (m = 0; m < entries; m++) {
		tree->GetEntry(m);
		Hist->Fill(Q2s);
	}
	
	c3->SetName(canvasname.c_str());

	c3->cd();
	Hist->GetXaxis()->SetTitle(xaxistitle.c_str());
	Hist->GetYaxis()->SetTitle(yaxistitle.c_str());
	Hist->SetNameTitle(histname.c_str(), histname.c_str());
	Hist->Draw();

	outputfile.cd();
	c3->Write();

	outputfile.Write();
	//f->cd();

	return 0;

};
//



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



void ParticlesPerEvent() {
	//Defining integrated Lumi values for 05,06,07 data set obtained separately from oolumi
	double DataIntLumi05 = 108796.1875; //nb-1
	//double DataIntLumi06 = 132495.265625; //nb-1
	double DataIntLumi06 = 144634.0; //nb-1
	double DataIntLumi07 = 46577.761719; //nb-1
	double MCIntLumi05 = 253760.0; //nb-1
	double MCIntLumi06 = 126880.0; //nb-1
	//double MCIntLumi06 = 13786701.0; //nb-1
	double MCIntLumi07 = 253760.0; //nb-1
	//Branching Ratios
	double BRlambda = 0.639;
	double BRkaon = 0.692;
	
	
	//TH1F* Hist = new TH1F("Lambda", "Lambda", lambdabins, lambdalowerbinrange, lambdaupperbinrange);
	
	TH1F* DataHist = new TH1F("Lambda", "Lambda", 500 , 0, 105);

	TH1F* MCHist = new TH1F("Lambda", "Lambda", 500, 0, 105);

	TH1F* EventHist = new TH1F("Data Events", "Data Events", 500, 0, 105);

	TH1F* EventMCHist = new TH1F("MC Events", "MC Events", 500, 0, 105);

	//do this after creating histograms
	//Creating the canvases used for storing MLP fit plots, good for troubleshooting
	TCanvas* c3 = new TCanvas("Lambda Extraction", "Lambda Extraction", 10, 10, 800, 400);
	//TCanvas* c1 = new TCanvas("Training", "Training", 400, 400);
	
	int run = 0;
	//run = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Tot", 0, Hist, "2006 Data Total Lambda per Event Q^{2}", "Count", "Mass [GeV]", "Lambda Count", c3);

	//run = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Tot", 0, Hist, "MC Total Lambda per Event Q^{2}","Count", "Mass [GeV]", "Lambda Count", c3);

	run = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Tot", 0, DataHist, "2006 Data Total Lambda per Event Q^{2}", "2006 Data Total Lambda per Event Q^{2}", "Q^{2} [GeV^{2}]", "Lambda Count", c3);

	run = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Tot", 0, MCHist, "MC Total Lambda per Event Q^{2}","MC Total Lambda per Event Q^{2}", "Q^{2} [GeV^{2}]", "Lambda Count", c3);


	run = AnalyzeEventTree(new TFile(fileDataEvents.c_str(), "read"), "Data Event Tree", 0, EventHist, "Q^{2} Event Dist", "Q^{2} Event Dist", "Q^{2} [GeV^{2}]", "Events", c3);
	run = AnalyzeEventTree(new TFile(fileDataMCEvents.c_str(), "read"), "Data Event Tree", 0, EventMCHist, "Q^{2} MC Event Dist", "Q^{2} MC Event Dist", "Q^{2} [GeV^{2}]", "Events", c3);
	
	
	
	c3->cd();
	c3->SetName("Data vs MC");
	DataHist->SetNameTitle("Data vs MC", "Data vs MC");

	DataHist->SetLineColor(kBlack);
	MCHist->SetLineColor(kRed);
	//gPad->SetLogx();
	//gPad->SetLogy();
	//DataHist->Scale(1 / (DataIntLumi06));
	//MCHist->Scale(1 / (MCIntLumi06));
	//DataHist->Draw();
	//MCHist->Draw("SAME");
	TH1F* DataHist2 = (TH1F*)DataHist->Clone();
	DataHist2->Reset("ICESM");

	TH1F* MCHist2 = (TH1F*)MCHist->Clone();
	MCHist2->Reset("ICESM");
	double check;
	double x;
	for (int i = 1; i < MCHist->GetNbinsX() + 1; i++) {
		x = ((TAxis*)MCHist->GetXaxis())->GetBinCenter(i);
		if (x > 5 && x < 100) {
			
			DataHist2->SetBinContent(i, DataHist->GetBinContent(i) - ((DataHist->GetBinContent(i)) / (151975)) * (51000));
			MCHist2->SetBinContent(i, MCHist->GetBinContent(i) - ((MCHist->GetBinContent(i)) / (291774)) * (99000));
			check = check + ((DataHist->GetBinContent(i)) / (151975)) * (51000);

		}
	}
	//DataHist2->Scale(1 / (DataIntLumi06));
	//MCHist2->Scale(1 / (MCIntLumi06));
	cout << check << endl;
	DataHist2->Draw();
	MCHist2->Draw("SAME");
	
	TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	//legend->SetHeader(Form("#splitline{Count- %.2f #pm %.2f (stat.)}{Stochastic MLP- %s Network Trained %ix}", kcount, kcounterror, network[particlecode].c_str(), trainnum[particlecode])); // option "C" allows to center the header
	legend->AddEntry(DataHist, "2006 Data", "l");
	legend->AddEntry(MCHist, "MC", "l");
	
	legend->Draw("same");


	outputfile.cd();
	c3->Write();

	EventHist->SetLineColor(kBlack);
	EventMCHist->SetLineColor(kRed);
	EventHist->Scale(1 / (DataIntLumi06));
	EventMCHist->Scale(1 / (MCIntLumi06));
	//gPad->SetLogx();
	//gPad->SetLogy();
	EventHist->Draw();
	EventMCHist->Draw("SAME");
	legend->Draw("same");
	c3->SetName("Event Q2");
	//DataHist->SetNameTitle("Data vs MC", "Data vs MC");
	c3->Write();


	DataHist2->Divide(EventHist);
	MCHist2->Divide(EventMCHist);

	DataHist2->Draw();
	MCHist2->Draw("SAME");

	//TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	//legend->SetHeader(Form("#splitline{Count- %.2f #pm %.2f (stat.)}{Stochastic MLP- %s Network Trained %ix}", kcount, kcounterror, network[particlecode].c_str(), trainnum[particlecode])); // option "C" allows to center the header
	//legend->AddEntry(DataHist, "2006 Data", "l");
	//legend->AddEntry(MCHist, "MC", "l");

	legend->Draw("same");
	
	c3->SetName("Lambdas Per Event Data vs MC");
	//DataHist->SetNameTitle("Data vs MC", "Data vs MC");
	c3->Write();

	/// <summary>
	


	/// </summary>

	outputfile.Write();
	outputfile.Close();

	cout << "Done" << endl;

};