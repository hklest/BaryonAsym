//Analysis Code Root Macro 
//Importing TTrees

using namespace std;

////////////////////////////////////////////////
// User Inputs in this section
////////////////////////////////////////////////


//string names for file paths of 3 Event Trees we need to access
string fileDataEvents = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/EventDists06_Ecra.root";
string fileMCrecEvents = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/EventDists06_MCNEW.root";
string filegenEvents = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree_Ecra.root";

//output file name
string outputfilename = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Events06.root";
string dummyfile = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/dummyfile.root";
TFile* f = new TFile(dummyfile.c_str(), "RECREATE");

// Histogram Binning
//variables: Q^2, y, x, E_e, Theta_E, Phi_E, E-pZ
double Q2lowerbin = 0;
double Q2upperbin = 110;
double nQ2bins = 100;

double ylowerbin = 0;
double yupperbin = 0.7;
double nybins = 100;

double xlowerbin = -0.001;
double xupperbin = 0.01;
double nxbins = 100;

double philowerbin = -3.5;
double phiupperbin = 3.5;
double nphibins = 100;

double thetalowerbin = 2.6;
double thetaupperbin = 3.1;
double nthetabins = 100;

double Elowerbin = 10;
double Eupperbin = 26;
double nEbins = 100;

double Epzlowerbin = 20;
double Epzupperbin = 75;
double nEpzbins = 100;



/////////////////////////////////////////////////
// End of Input Section
/////////////////////////////////////////////////

//creating output file
TFile outputfile(outputfilename.c_str(), "RECREATE");

int AnalyzeTree(TFile* Data, string TreeName, vector<TH1F*> HistArray, int a) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters
	
	//grab Tree from file above
	TTree* tree = (TTree*)Data->Get(TreeName.c_str());

	int entries;

	entries = tree->GetEntries();

	//Defining the variables that are stored in the Tree branches
	double elecPhi, elecTheta, Epz, elecE, x, Q2s, Ys;
	//Defining Tree Branches
	tree->SetBranchAddress("elecPhi", &elecPhi);
	tree->SetBranchAddress("elecTheta", &elecTheta);
	tree->SetBranchAddress("Epz", &Epz);
	tree->SetBranchAddress("elecE", &elecE);
	tree->SetBranchAddress("x", &x);
	tree->SetBranchAddress("Q2s", &Q2s);
	tree->SetBranchAddress("Ys", &Ys);

	int m;
	//Have to enforce pT and Eta bounds here
	for (m = 0; m < entries; m++) {
		tree->GetEntry(m);
		HistArray[0]->Fill(elecPhi);
		HistArray[1]->Fill(elecTheta);
		HistArray[2]->Fill(Epz);
		HistArray[3]->Fill(elecE);
		HistArray[4]->Fill(x);
		HistArray[5]->Fill(Q2s);
		HistArray[6]->Fill(Ys);		
	}

	outputfile.cd();

	HistArray[0]->GetXaxis()->SetTitle("Phi");
	HistArray[0]->GetYaxis()->SetTitle("Events");
	HistArray[0]->SetLineColor(kBlack + a);
	HistArray[0]->Draw();
	HistArray[0]->Write();

	HistArray[1]->GetXaxis()->SetTitle("Theta");
	HistArray[1]->GetYaxis()->SetTitle("Events");
	HistArray[1]->SetLineColor(kBlack + a);
	HistArray[1]->Draw();
	HistArray[1]->Write();

	HistArray[2]->GetXaxis()->SetTitle("Epz [GeV]");
	HistArray[2]->GetYaxis()->SetTitle("Events");
	HistArray[2]->SetLineColor(kBlack + a);
	HistArray[2]->Draw();
	HistArray[2]->Write();

	HistArray[3]->GetXaxis()->SetTitle("Electron Energy [GeV]");
	HistArray[3]->GetYaxis()->SetTitle("Events");
	HistArray[3]->SetLineColor(kBlack + a);
	HistArray[3]->Draw();
	HistArray[3]->Write();

	HistArray[4]->GetXaxis()->SetTitle("x");
	HistArray[4]->GetYaxis()->SetTitle("Events");
	HistArray[4]->SetLineColor(kBlack + a);
	HistArray[4]->Draw();
	HistArray[4]->Write();

	HistArray[5]->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
	HistArray[5]->GetYaxis()->SetTitle("Events");
	HistArray[5]->SetLineColor(kBlack + a);
	HistArray[5]->Draw();
	HistArray[5]->Write();

	HistArray[6]->GetXaxis()->SetTitle("y");
	HistArray[6]->GetYaxis()->SetTitle("Events");
	HistArray[6]->SetLineColor(kBlack + a);
	HistArray[6]->Draw();
	HistArray[6]->Write();

	return 0;

};

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



void Events() {
	//Defining integrated Lumi values for 05,06,07 data set obtained separately from oolumi
	double DataIntLumi05 = 108796.1875; //nb-1
	double DataIntLumi06 = 144634.0; //nb-1
	//double DataIntLumi06 = 46710.156250; //nb-1
	double DataIntLumi07 = 46577.761719; //nb-1
	double MCIntLumi05 = 253760.0; //nb-1
	double MCIntLumi06 = 126880.0; //nb-1
	double MCIntLumi07 = 253760.0; //nb-1
	
	TCanvas* canvas = new TCanvas("Plot", "Plot", 10, 10, 800, 800);
	
	vector<TH1F*> DataEventHists;
	vector<TH1F*> MCrecEventHists;
	vector<TH1F*> GenEventHists;

	f->cd();
	//double elecPhi, elecTheta, Epz, elecE, x, Q2e, Ye;
	DataEventHists.push_back(new TH1F("Data Phi Electron", "Data Phi Electron", nphibins, philowerbin, phiupperbin));
	DataEventHists.push_back(new TH1F("Data Theta Electron", "Data Theta Electron", nthetabins, thetalowerbin, thetaupperbin));
	DataEventHists.push_back(new TH1F("Data Epz", "Data Epz", nEpzbins, Epzlowerbin, Epzupperbin));
	DataEventHists.push_back(new TH1F("Data Electron Energy", "Data Electron Enerygy", nEbins, Elowerbin, Eupperbin));
	DataEventHists.push_back(new TH1F("Data x", "Data x", nxbins, xlowerbin, xupperbin));
	DataEventHists.push_back(new TH1F("Data Q2", "Data Q2", nQ2bins, Q2lowerbin, Q2upperbin));
	DataEventHists.push_back(new TH1F("Data y", "Data y", nybins, ylowerbin, yupperbin));

	MCrecEventHists.push_back(new TH1F("MC Rec Phi Electron", "MC Rec Phi Electron", nphibins, philowerbin, phiupperbin));
	MCrecEventHists.push_back(new TH1F("MC Rec Theta Electron", "MC Rec Theta Electron", nthetabins, thetalowerbin, thetaupperbin));
	MCrecEventHists.push_back(new TH1F("MC Rec Epz", "Data Epz", nEpzbins, Epzlowerbin, Epzupperbin));
	MCrecEventHists.push_back(new TH1F("MC Rec Electron Energy", "MC Rec Electron Enerygy", nEbins, Elowerbin, Eupperbin));
	MCrecEventHists.push_back(new TH1F("MC Rec x", "MC Rec x", nxbins, xlowerbin, xupperbin));
	MCrecEventHists.push_back(new TH1F("MC Rec Q2", "MC Rec Q2", nQ2bins, Q2lowerbin, Q2upperbin));
	MCrecEventHists.push_back(new TH1F("MC Rec y", "MC Rec y", nybins, ylowerbin, yupperbin));

	GenEventHists.push_back(new TH1F("Gen Phi Electron", "Gen Phi Electron", nphibins, philowerbin, phiupperbin));
	GenEventHists.push_back(new TH1F("Gen Theta Electron", "Gen Theta Electron", nthetabins, thetalowerbin, thetaupperbin));
	GenEventHists.push_back(new TH1F("Gen Epz", "Gen Epz", nEpzbins, Epzlowerbin, Epzupperbin));
	GenEventHists.push_back(new TH1F("Gen Electron Energy", "Gen Electron Enerygy", nEbins, Elowerbin, Eupperbin));
	GenEventHists.push_back(new TH1F("Gen x", "Gen x", nxbins, xlowerbin, xupperbin));
	GenEventHists.push_back(new TH1F("Gen Q2", "Gen Q2", nQ2bins, Q2lowerbin, Q2upperbin));
	GenEventHists.push_back(new TH1F("Gen y", "Gen y", nybins, ylowerbin, yupperbin));
	
	//LambdaTotDataCount = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Tot", pTentries, 0, LambdaTotDatapT, pTbins, "Lambda Tot Data pT Extraction ", c3, c1);


	//This will analyze Trees for Events and individually plot distributions
	AnalyzeTree(new TFile(fileDataEvents.c_str(), "read"), "Data Event Tree", DataEventHists,0);
	//AnalyzeTree(new TFile(fileMCrecEvents.c_str(), "read"), "MC Rec Event Tree", MCrecEventHists,1);
	AnalyzeTree(new TFile(fileMCrecEvents.c_str(), "read"), "Data Event Tree", MCrecEventHists, 1);
	AnalyzeTree(new TFile(filegenEvents.c_str(), "read"), "Gen Event Tree", GenEventHists,2);
	
	vector<string> XaxisLabels = { "Phi", "Theta", "Epz [GeV]", "E_{e} [GeV]","x","Q^{2} [GeV^{2}]","y"};
	vector<string> PlotNames = { "Phi Distributions","Theta Distributions","Epz Distributions","E_e Distributions", "x Distributions", "Q2 Distributions", "y Distributions"};
	vector<string> PlotNames2 = { "Phi CS","Theta CS","Epz CS","E_e CS", "x CS", "Q2 CS", "y CS" };
	vector<string> PlotNames3 = { "Phi Eff","Theta Eff","Epz Eff","E_e Eff", "x Eff", "Q2 Eff", "y Eff" };
	//Now plot 7 plots for 7 variables with Data, MC rec, and MC gen on same plots
	for (int a = 0; a < 7; a++) {
		THStack* s = new THStack(PlotNames[a].c_str(), PlotNames[a].c_str());
		s->Add(DataEventHists[a]);
		s->Add(MCrecEventHists[a]);
		s->Add(GenEventHists[a]);
		s->Draw("nostack");
		s->GetXaxis()->SetTitle(XaxisLabels[a].c_str());
		s->GetYaxis()->SetTitle("N_{Events}");
		TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
		//legend->SetHeader(Form("#splitline{Count- %.2f #pm %.2f (stat.)}{Stochastic MLP- %s Network Trained %ix}", kcount, kcounterror, network[particlecode].c_str(), trainnum[particlecode])); // option "C" allows to center the header
		legend->AddEntry(DataEventHists[0], "Data", "l");
		legend->AddEntry(MCrecEventHists[0], "MC Rec", "l");
		legend->AddEntry(GenEventHists[0], "MC Gen", "l");
		legend->Draw("same");
		canvas->SetName(PlotNames[a].c_str());
		canvas->Write();
	}

	//creating new vectors
	vector<TH1F*> DataCS;
	vector<TH1F*> MCrecCS;
	vector<TH1F*> GenCS;
	//Now plot 7 plots for 7 variables with Data, MC rec, and MC gen on same plots but scale by 1/Lumi
	for (int a = 0; a < 7; a++) {
		THStack* s = new THStack(PlotNames2[a].c_str(), PlotNames2[a].c_str());
		//making copies of hists before scaling so I still have original hists before the scaling
		f->cd();
		DataCS.push_back((TH1F*)DataEventHists[a]->Clone());
		MCrecCS.push_back((TH1F*)MCrecEventHists[a]->Clone());
		GenCS.push_back((TH1F*)GenEventHists[a]->Clone());
		outputfile.cd();
		DataCS[a]->Scale(1/(DataIntLumi06));
		MCrecCS[a]->Scale(1 / (MCIntLumi06));
		GenCS[a]->Scale(1 / (MCIntLumi06));
		DataCS[a]->Sumw2(0);
		MCrecCS[a]->Sumw2(0);
		GenCS[a]->Sumw2(0);
		s->Add(DataCS[a]);
		s->Add(MCrecCS[a]);
		s->Add(GenCS[a]);
		s->Draw("nostack");
		s->GetXaxis()->SetTitle(XaxisLabels[a].c_str());
		s->GetYaxis()->SetTitle("N_{Events}/L [nb]");
		TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
		//legend->SetHeader(Form("#splitline{Count- %.2f #pm %.2f (stat.)}{Stochastic MLP- %s Network Trained %ix}", kcount, kcounterror, network[particlecode].c_str(), trainnum[particlecode])); // option "C" allows to center the header
		legend->AddEntry(DataEventHists[0], "Data", "l");
		legend->AddEntry(MCrecEventHists[0], "MC Rec", "l");
		legend->AddEntry(GenEventHists[0], "MC Gen", "l");
		legend->Draw("same");
		canvas->SetName(PlotNames2[a].c_str());
		canvas->Write();
	}


	//creating new vectors
	vector<TH1F*> EventEff;
	//Now plot 7 plots for 7 variables with Data, MC rec, and MC gen on same plots but scale by 1/Lumi
	for (int a = 0; a < 7; a++) {
		THStack* s = new THStack(PlotNames3[a].c_str(), PlotNames3[a].c_str());
		//making copies of hists before scaling so I still have original hists before the scaling
		f->cd();
		EventEff.push_back((TH1F*)MCrecEventHists[a]->Clone());
		EventEff[a]->Divide(GenEventHists[a]);
		outputfile.cd();
		EventEff[a]->Sumw2(0);
		s->Add(EventEff[a]);
		s->Draw("nostack");
		s->GetXaxis()->SetTitle(XaxisLabels[a].c_str());
		s->GetYaxis()->SetTitle("N_{MC Rec}/N_{MC Gen}");
		canvas->SetName(PlotNames3[a].c_str());
		canvas->Write();
	}




	outputfile.cd();
	outputfile.Write();
	outputfile.Close();

	cout << "Done" << endl;

};